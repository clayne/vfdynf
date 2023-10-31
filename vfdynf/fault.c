/*
    Copyright (c) Johnny Shaw. All rights reserved.
*/
#include <vfdynf.h>

typedef struct _VFDYNF_FAULT_CONTEXT
{
    BOOLEAN Initialized;
    ULONG TypeBase;
    RTL_CRITICAL_SECTION CriticalSection;
    ULONG64 LastClear;
    AVRF_STACK_TABLE StackTable;
    BOOLEAN ExclusionsRegexInitialized;
    ULONG ExclusionsRegexCount;
    PPCRE2_CONTEXT ExclusionsRegex;
    BYTE SymInfoBuffer[sizeof(SYMBOL_INFOW) + ((MAX_SYM_NAME + 1) * sizeof(WCHAR))];
    WCHAR SymbolBuffer[MAX_SYM_NAME + 1 + MAX_PATH];
    WCHAR StackSymbolBuffer[UNICODE_STRING_MAX_CHARS];
} VFDYNF_FAULT_CONTEXT, *PVFDYNF_FAULT_CONTEXT;

static VFDYNF_FAULT_CONTEXT AVrfpFaultContext =
{
    .Initialized = FALSE,
    .TypeBase = ULONG_MAX,
    .CriticalSection = { 0 },
    .LastClear = 0,
    .StackTable = { 0 },
    .ExclusionsRegexInitialized = FALSE,
    .ExclusionsRegex = NULL,
    .SymInfoBuffer = { 0 },
    .SymbolBuffer = { 0 },
    .StackSymbolBuffer = { 0 },
};

ULONG AVrfpFaultTypeClass(
    _In_ ULONG FaultType
    )
{
    ULONG index;

    AVFR_ASSERT(AVrfpFaultContext.TypeBase != ULONG_MAX);

    BitScanReverse(&index, FaultType);

    return (AVrfpFaultContext.TypeBase + index);
}

BOOL CALLBACK AVrfpSymbolRegsteredCallback(
    _In_ HANDLE hProcess,
    _In_ ULONG ActionCode,
    _In_opt_ ULONG64 CallbackData,
    _In_opt_ ULONG64 UserContext
    )
{
    PIMAGEHLP_DEFERRED_SYMBOL_LOADW64 info;

    UNREFERENCED_PARAMETER(hProcess);
    UNREFERENCED_PARAMETER(UserContext);

    info = (PIMAGEHLP_DEFERRED_SYMBOL_LOADW64)(ULONG_PTR)CallbackData;

    switch (ActionCode)
    {
        case CBA_DEFERRED_SYMBOL_LOAD_COMPLETE:
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_INFO_LEVEL,
                       "AVRF: loaded symbols %ls\n",
                       info->FileName);
            break;
        }
        case CBA_DEFERRED_SYMBOL_LOAD_FAILURE:
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_ERROR_LEVEL,
                       "AVRF: failed to loaded symbols %ls\n",
                       info->FileName);
            break;
        }
        case CBA_DEFERRED_SYMBOL_LOAD_PARTIAL:
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_WARNING_LEVEL,
                       "AVRF: partially loaded symbols %ls\n",
                       info->FileName);
            break;
        }
        case CBA_SYMBOLS_UNLOADED:
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_INFO_LEVEL,
                       "AVRF: unloaded symbols %ls\n",
                       info->FileName);
            break;
        }
        default:
        {
            return FALSE;
        }
    }

    return TRUE;
}

BOOLEAN AVrfpInitExclusionsRegex(
    VOID
    )
{
    ULONG offset;
    ULONG count;

    //
    // The exclusions regular expressions is a REG_MULTI_SZ from the properties
    // verifier loads on our behalf. Parse each block of the multi terminated
    // string into the regex vector. We do this so we don't have to construct
    // the regex object every time.
    //
    AVFR_ASSERT(!AVrfpFaultContext.ExclusionsRegexInitialized);

    offset = 0;
    count = 0;
    for (;;)
    {
        UNICODE_STRING pattern;

        RtlInitUnicodeString(&pattern, &AVrfProperties.ExclusionsRegex[offset]);
        if (!pattern.Length)
        {
            break;
        }

        count++;

        offset += ((pattern.Length / sizeof(WCHAR)) + 1);
    }

    AVrfpFaultContext.ExclusionsRegexCount = count;
    AVrfpFaultContext.ExclusionsRegex = RtlAllocateHeap(RtlProcessHeap(),
                                                        0,
                                                        count * sizeof(PCRE2_CONTEXT));
    if (!AVrfpFaultContext.ExclusionsRegex)
    {
        DbgPrintEx(DPFLTR_VERIFIER_ID,
                   DPFLTR_ERROR_LEVEL,
                   "AVRF: failed to allocate exclusion regex!\n");
        __debugbreak();
        return FALSE;
    }

    offset = 0;
    count = 0;
    for (;;)
    {
        NTSTATUS status;
        UNICODE_STRING pattern;
        PCRE2_CONTEXT pcre2;

        RtlInitUnicodeString(&pattern, &AVrfProperties.ExclusionsRegex[offset]);
        if (!pattern.Length)
        {
            break;
        }

        status = Pcre2Compile(&pcre2, &pattern);
        if (!NT_SUCCESS(status))
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_ERROR_LEVEL,
                       "AVRF: failed processing regex! (0x%08x)\n",
                       status);
            __debugbreak();
            return FALSE;
        }

        AVFR_ASSERT(count < AVrfpFaultContext.ExclusionsRegexCount);

        AVrfpFaultContext.ExclusionsRegex[count++] = pcre2;

        offset += ((pattern.Length / sizeof(WCHAR)) + 1);
    }

    AVrfpFaultContext.ExclusionsRegexInitialized = TRUE;
    return TRUE;
}

BOOLEAN AVrfpIsStackOverriddenByRegex(
    _In_ PUNICODE_STRING StackSymbols
    )
{
    AVFR_ASSERT(AVrfpFaultContext.ExclusionsRegexInitialized);

    for (size_t i = 0; i < AVrfpFaultContext.ExclusionsRegexCount; i++)
    {
        if (Pcre2Match(&AVrfpFaultContext.ExclusionsRegex[i], StackSymbols))
        {
            return TRUE;
        }
    }

    return FALSE;
}

BOOLEAN AvrfShouldFaultInject(
    _In_ ULONG FaultType,
    _In_opt_ _Maybenull_ PVOID CallerAddress
    )
{
    BOOLEAN result;
    ULONG hash;
    PVOID frames[250];
    USHORT count;
    PAVRF_STACK_ENTRY stackEntry;

    if (!AVrfpFaultContext.Initialized)
    {
        DbgPrintEx(DPFLTR_VERIFIER_ID,
                   DPFLTR_WARNING_LEVEL,
                   "AVRF: fault injection not yet initialized\n");
        return FALSE;
    }

    if (!CallerAddress)
    {
        DbgPrintEx(DPFLTR_VERIFIER_ID,
                   DPFLTR_WARNING_LEVEL,
                   "AVRF: caller address is null\n");
        return FALSE;
    }

    if (!BooleanFlagOn(AVrfProperties.EnableFaultMask, FaultType))
    {
        //
        // Fault type is not enabled.
        //
        return FALSE;
    }

    if (!VerifierShouldFaultInject(AVrfpFaultTypeClass(FaultType), CallerAddress))
    {
        return FALSE;
    }

    count = RtlCaptureStackBackTrace(2, ARRAYSIZE(frames), frames, &hash);

    RtlEnterCriticalSection(&AVrfpFaultContext.CriticalSection);

    if (AVrfProperties.DynamicFaultPeroid)
    {
        if (!AVrfpFaultContext.LastClear)
        {
            AVrfpFaultContext.LastClear = NtGetTickCount64();
        }
        else if ((AVrfpFaultContext.LastClear + AVrfProperties.DynamicFaultPeroid)
                 <= NtGetTickCount64())
        {
            AVrfpFaultContext.LastClear = NtGetTickCount64();
            AVrfClearStackTable(&AVrfpFaultContext.StackTable);
        }
    }

    stackEntry = AVrfLookupStackEntry(&AVrfpFaultContext.StackTable, hash);
    if (stackEntry && (stackEntry->Hash == hash))
    {
        //
        // We already evaluated this stack.
        // 1. it's excluded
        // 2. we should inject a fault of an unseen type
        // 3. we already injected a fault and shouldn't
        //

        if (stackEntry->Excluded)
        {
            result = FALSE;
            goto Exit;
        }

        if (!BooleanFlagOn(stackEntry->FaultMask, FaultType))
        {
            SetFlag(stackEntry->FaultMask, FaultType);
            result = TRUE;
            goto Exit;
        }

        //
        // We've already injected a fault for this stack and fault type.
        //
        result = FALSE;
        goto Exit;
    }

    //
    // We haven't yet evaluated this stack, do so now.
    //

    stackEntry = AVrfInsertStackEntry(&AVrfpFaultContext.StackTable, stackEntry, hash);
    if (!stackEntry)
    {
        DbgPrintEx(DPFLTR_VERIFIER_ID,
                   DPFLTR_ERROR_LEVEL,
                   "AVRF: failed to insert new stack entry!\n");

        result = FALSE;
        goto Exit;
    }

    if (AVrfProperties.ExclusionsRegex[0] == L'\0')
    {
        //
        // There are no exclusion expressions, skip the work below.
        //
        SetFlag(stackEntry->FaultMask, FaultType);
        result = TRUE;
        goto Exit;
    }

    DbgPrintEx(DPFLTR_VERIFIER_ID,
               DPFLTR_MASK | 0x10,
               "AVRF: Cid %04x.%04x stack frame count: %hu\n",
               HandleToULong(NtCurrentTeb()->ClientId.UniqueProcess),
               HandleToULong(NtCurrentTeb()->ClientId.UniqueThread),
               count);

    //
    // Classify the stack. Check for overrides by symbols/etc. We build a
    // complete string representation of the stack enabling the regex to span
    // multiple frames. This is the easiest way to enable an author of
    // overrides to write expressions for an entire stack.
    //

    //
    // N.B. This processing is always done under the critical section so it's
    // safe for us to reuse the preallocated buffer in this path. We use a
    // preallocated buffer that will satisfy the maximum possible length of
    // a UNICODE_STRING.
    //
    UNICODE_STRING stackSymbols;
    stackSymbols.Length = 0;
    stackSymbols.MaximumLength = sizeof(AVrfpFaultContext.StackSymbolBuffer);
    stackSymbols.Buffer = AVrfpFaultContext.StackSymbolBuffer;

    for (WORD i = 0; i < count; i++)
    {
        NTSTATUS status;
        PSYMBOL_INFOW info;
        ULONG64 disp;
        PVOID ldrCookie;
        ULONG ldrDisp;
        PLDR_DATA_TABLE_ENTRY data;
        PLIST_ENTRY modList;
        UNICODE_STRING symbol;

        info = (PSYMBOL_INFOW)AVrfpFaultContext.SymInfoBuffer;

        RtlZeroMemory(info, sizeof(*info));
        info->SizeOfStruct = sizeof(SYMBOL_INFOW);
        info->MaxNameLen = MAX_SYM_NAME;

        if (!SymFromAddrW(NtCurrentProcess(), (ULONG64)frames[i], &disp, info))
        {
            //
            // Refresh the module list and try again.
            //
            SymRefreshModuleList(NtCurrentProcess());

            if (!SymFromAddrW(NtCurrentProcess(), (ULONG64)frames[i], &disp, info))
            {
                DbgPrintEx(DPFLTR_VERIFIER_ID,
                           DPFLTR_WARNING_LEVEL,
                           "AVRF: failed to get symbol info %p (%lu)\n",
                           frames[i],
                           NtCurrentTeb()->LastErrorValue);

                //
                // Zero the structure, we'll handle this below. The analysis
                // will be given a frame with only a module name and no symbol.
                // e.g. "ntdll.dll!"
                //
                RtlZeroMemory(info, sizeof(*info));
            }
        }

        //
        // To minimize the tracking we have to do and since we're already
        // injected into the process, we're going to just use the loader
        // module list to identify the module of the symbol. First, try
        // to get the loader lock - if we can't we just concede and get out
        // of the way of the system - we won't inject a fault.
        //
        status = LdrLockLoaderLock(LDR_LOCK_LOADER_LOCK_FLAG_TRY_ONLY,
                                   &ldrDisp,
                                   &ldrCookie);
        if (!NT_SUCCESS(status))
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_WARNING_LEVEL,
                       "AVRF: failed to acquire loader lock (0x%08x)!\n",
                       status);

            AVrfRemoveStackEntry(&AVrfpFaultContext.StackTable, stackEntry);

            result = FALSE;
            goto Exit;
        }
        if (ldrDisp != LDR_LOCK_LOADER_LOCK_DISPOSITION_LOCK_ACQUIRED)
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_WARNING_LEVEL,
                       "AVRF: loader lock is busy!\n");

            AVrfRemoveStackEntry(&AVrfpFaultContext.StackTable, stackEntry);

            result = FALSE;
            goto Exit;
        }

        data = NULL;
        modList = &NtCurrentPeb()->Ldr->InLoadOrderModuleList;

        for (PLIST_ENTRY entry = modList->Flink;
             entry != modList;
             entry = entry->Flink)
        {
            PLDR_DATA_TABLE_ENTRY item;

            item = CONTAINING_RECORD(entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
            if (info->ModBase != 0)
            {
                if (item->DllBase == (PVOID)(ULONG_PTR)info->ModBase)
                {
                    data = item;
                    break;
                }
            }
            else
            {
                PVOID end;

                //
                // Either dbghelp didn't give us a module base or the call to
                // resolve the symbol name failed. Identify the module by the
                // extents.
                //

                end = Add2Ptr(item->DllBase, item->SizeOfImage);
                if ((frames[i] >= item->DllBase) && (frames[i] < end))
                {
                    data = item;
                    break;
                }
            }
        }

        //
        // Build the symbol string.
        //

        symbol.Length = 0;
        symbol.MaximumLength = sizeof(AVrfpFaultContext.SymbolBuffer);
        symbol.Buffer = AVrfpFaultContext.SymbolBuffer;

        if (!data)
        {
            RtlAppendUnicodeToString(&symbol, L"(null)");
        }
        else
        {
            RtlAppendUnicodeStringToString(&symbol, &data->BaseDllName);
        }

        //
        // We're done with the loader lock.
        //
        LdrUnlockLoaderLock(0, ldrCookie);

        RtlAppendUnicodeToString(&symbol, L"!");
        RtlAppendUnicodeToString(&symbol, info->Name);

        DbgPrintEx(DPFLTR_VERIFIER_ID,
                   DPFLTR_MASK | 0x10,
                   "AVRF: %wZ\n",
                   &symbol);

        //
        // If we fail here it means we've run out of the maximum Unicode string
        // length which is innately impossible to extend further. Rather than
        // consider this a failure, we'll just stop appending symbols, debug
        // print that an error occurred, and break out to classify the stack
        // that we do have.
        //

        status = RtlAppendUnicodeStringToString(&stackSymbols, &symbol);
        if (!NT_SUCCESS(status))
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_WARNING_LEVEL,
                       "AVRF: failed to append symbol to stack string (0x%08x)!\n",
                       status);
            break;
        }

        status = RtlAppendUnicodeToString(&stackSymbols, L"\n");
        if (!NT_SUCCESS(status))
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_WARNING_LEVEL,
                       "AVRF: failed to append new line to stack string (0x%08x)!\n",
                       status);
            break;
        }
    }

    if (stackSymbols.Length >= sizeof(WCHAR))
    {
        //
        // Pop the last new line.
        //
        stackSymbols.Length -= sizeof(WCHAR);

        if (AVrfpIsStackOverriddenByRegex(&stackSymbols))
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_INFO_LEVEL,
                       "AVRF: stack excluded by regular expression\n");

            //
            // Cache the decision for this stack hash.
            //
            stackEntry->Excluded = TRUE;
            result = FALSE;
            goto Exit;
        }
    }

    //
    // New entry that we need to inject a fault for. Track that we've done so.
    //
    SetFlag(stackEntry->FaultMask, FaultType);
    result = TRUE;

Exit:

    RtlLeaveCriticalSection(&AVrfpFaultContext.CriticalSection);

    return result;
}

VOID AVrfpFaultSetRangeForType(
    _In_ ULONG FaultType
    )
{
    //
    // We ask for everything and handle excluding ranges ourself.
    //
    VerifierEnableFaultInjectionTargetRange(AVrfpFaultTypeClass(FaultType),
                                            NULL,
                                            Add2Ptr(NULL, MAXULONG_PTR));
}

VOID AVrfpFaultSetProbabilityForType(
    _In_ ULONG FaultType
    )
{
    VerifierSetFaultInjectionProbability(AVrfpFaultTypeClass(FaultType),
                                         AVrfProperties.FaultProbability);
}

BOOLEAN AVrfFaultProcessAttach(
    VOID
    )
{
    NTSTATUS status;
    ULONG err;

    if (AVrfpFaultContext.Initialized)
    {
        return TRUE;
    }

    AVrfInitializeStackTable(&AVrfpFaultContext.StackTable);

    if (!AVrfpInitExclusionsRegex())
    {
        DbgPrintEx(DPFLTR_VERIFIER_ID,
                   DPFLTR_ERROR_LEVEL,
                   "AVRF: failed to initialized exclusions regex!\n");
        __debugbreak();
        return FALSE;
    }

    if (AVrfProperties.SymbolSearchPath[0] == L'\0')
    {
        if (!SymInitializeW(NtCurrentProcess(), NULL, FALSE))
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_ERROR_LEVEL,
                       "AVRF: failed to initialize symbols (%lu)\n",
                       NtCurrentTeb()->LastErrorValue);
            return FALSE;
        }
    }
    else
    {
        if (!SymInitializeW(NtCurrentProcess(),
                            AVrfProperties.SymbolSearchPath,
                            FALSE))
        {
            DbgPrintEx(DPFLTR_VERIFIER_ID,
                       DPFLTR_ERROR_LEVEL,
                       "AVRF: failed to initialize symbols (%lu)\n",
                       NtCurrentTeb()->LastErrorValue);
            return FALSE;
        }
    }

    SymSetOptions(SymGetOptions() | SYMOPT_UNDNAME);
    SymRegisterCallbackW64(NtCurrentProcess(), AVrfpSymbolRegsteredCallback, 0);
    SymRefreshModuleList(NtCurrentProcess());

    err = VerifierRegisterFaultInjectProvider(VFDYNF_FAULT_TYPE_COUNT,
                                              &AVrfpFaultContext.TypeBase);
    if (err != ERROR_SUCCESS)
    {
        DbgPrintEx(DPFLTR_VERIFIER_ID,
                   DPFLTR_ERROR_LEVEL,
                   "AVRF: failed to register fault injection provider (%lu)\n",
                   err);
        return FALSE;
    }

    if (AVrfProperties.GracePeriod)
    {
        VerifierSuspendFaultInjection(AVrfProperties.GracePeriod);
    }

    //
    // We ask for everything and handle excluding ranges ourself.
    //
    AVrfpFaultSetRangeForType(VFDYNF_FAULT_TYPE_WAIT);
    AVrfpFaultSetRangeForType(VFDYNF_FAULT_TYPE_HEAP);
    AVrfpFaultSetRangeForType(VFDYNF_FAULT_TYPE_VMEM);
    AVrfpFaultSetRangeForType(VFDYNF_FAULT_TYPE_REG);
    AVrfpFaultSetRangeForType(VFDYNF_FAULT_TYPE_FILE);
    AVrfpFaultSetRangeForType(VFDYNF_FAULT_TYPE_EVENT);
    AVrfpFaultSetRangeForType(VFDYNF_FAULT_TYPE_SECTION);
    AVrfpFaultSetRangeForType(VFDYNF_FAULT_TYPE_OLE);

    //
    // By default the system doesn't rely on probability for fault injection.
    // However, there are properties to set the probability and seed if desired.
    //
    AVrfpFaultSetProbabilityForType(VFDYNF_FAULT_TYPE_WAIT);
    AVrfpFaultSetProbabilityForType(VFDYNF_FAULT_TYPE_HEAP);
    AVrfpFaultSetProbabilityForType(VFDYNF_FAULT_TYPE_VMEM);
    AVrfpFaultSetProbabilityForType(VFDYNF_FAULT_TYPE_REG);
    AVrfpFaultSetProbabilityForType(VFDYNF_FAULT_TYPE_FILE);
    AVrfpFaultSetProbabilityForType(VFDYNF_FAULT_TYPE_EVENT);
    AVrfpFaultSetProbabilityForType(VFDYNF_FAULT_TYPE_SECTION);
    AVrfpFaultSetProbabilityForType(VFDYNF_FAULT_TYPE_OLE);

    VerifierSetAPIClassName(AVrfpFaultTypeClass(VFDYNF_FAULT_TYPE_WAIT), L"Wait APIs");
    VerifierSetAPIClassName(AVrfpFaultTypeClass(VFDYNF_FAULT_TYPE_HEAP), L"Heap APIs");
    VerifierSetAPIClassName(AVrfpFaultTypeClass(VFDYNF_FAULT_TYPE_VMEM), L"Virtual Memory APIs");
    VerifierSetAPIClassName(AVrfpFaultTypeClass(VFDYNF_FAULT_TYPE_REG), L"Registry APIs");
    VerifierSetAPIClassName(AVrfpFaultTypeClass(VFDYNF_FAULT_TYPE_FILE), L"File APIs");
    VerifierSetAPIClassName(AVrfpFaultTypeClass(VFDYNF_FAULT_TYPE_EVENT), L"Event APIs");
    VerifierSetAPIClassName(AVrfpFaultTypeClass(VFDYNF_FAULT_TYPE_SECTION), L"Section APIs");
    VerifierSetAPIClassName(AVrfpFaultTypeClass(VFDYNF_FAULT_TYPE_OLE), L"OLE String APIs");

    if (!AVrfProperties.FaultSeed)
    {
        ULONG seed;
        ULONG rand;

        seed = HandleToULong(NtCurrentThreadId()) ^ NtGetTickCount();
        rand = RtlRandomEx(&seed);

        DbgPrintEx(DPFLTR_VERIFIER_ID,
                   DPFLTR_INFO_LEVEL,
                   "AVRF: generated and using random fault injection seed %lu\n",
                   rand);

        VerifierSetFaultInjectionSeed(rand);
    }
    else
    {
        VerifierSetFaultInjectionSeed(AVrfProperties.FaultSeed);
    }

    status = RtlInitializeCriticalSection(&AVrfpFaultContext.CriticalSection);
    if (!NT_SUCCESS(status))
    {
        DbgPrintEx(DPFLTR_VERIFIER_ID,
                   DPFLTR_ERROR_LEVEL,
                   "AVRF: failed to initialize critical section (0x%08x)\n",
                   status);
        __debugbreak();
        return FALSE;
    }

    DbgPrintEx(DPFLTR_VERIFIER_ID,
               DPFLTR_INFO_LEVEL,
               "AVRF: dynamic fault injection initialized\n");

    AVrfpFaultContext.Initialized = TRUE;
    return TRUE;
}

VOID AVrfFaultProcessDetach(
    VOID
    )
{
    if (AVrfpFaultContext.Initialized)
    {
        return;
    }

    AVrfpFaultContext.Initialized = FALSE;

    RtlDeleteCriticalSection(&AVrfpFaultContext.CriticalSection);

    if (AVrfpFaultContext.ExclusionsRegex)
    {
        RtlFreeHeap(RtlProcessHeap(), 0, AVrfpFaultContext.ExclusionsRegex);
        AVrfpFaultContext.ExclusionsRegex = NULL;
    }

    AVrfFreeStackTable(&AVrfpFaultContext.StackTable);

    SymCleanup(NtCurrentProcess());
}