/*
    Copyright (c) Johnny Shaw. All rights reserved.
*/
#include <vfdynf.h>
#include <hooks.h>

VOID AVrfpPreFuzzKeyValueLength(
    _Inout_ PULONG Length,
    _In_ ULONG Type
    )
{
    switch (AVrfFuzzRandom() % 10)
    {
        case 1: // do nothing
        {
            break;
        }
        case 2: // small multiple
        {
            *Length *= (2 + (AVrfFuzzRandom() % 5));
            break;
        }
        default: // round up to 4k boundary
        {
            if ((Type == REG_BINARY) ||
                (Type == REG_SZ) ||
                (Type == REG_EXPAND_SZ) ||
                (Type == REG_MULTI_SZ))
            {
                *Length = (((ULONG64)*Length + 0xfff) & ~0xfff);
            }
            break;
        }
    }
}

VOID AVrfpFuzzKeyInformation(
    _In_ KEY_INFORMATION_CLASS KeyInformationClass,
    _Inout_updates_bytes_(Length) PVOID KeyInformation,
    _In_ ULONG Length,
    _In_ PVOID Context
    )
{
    AVRF_HOOK_WITH_CONTEXT(Context);

    switch (KeyInformationClass)
    {
        case KeyBasicInformation:
        {
            if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
            {
                PKEY_BASIC_INFORMATION info;
                ULONG length;

                info = KeyInformation;
                length = (Length - FIELD_OFFSET(KEY_BASIC_INFORMATION, Name));

                AVrfFuzzBuffer(info->Name, length, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
                AVrfFuzzSizeTruncateWideString(&info->NameLength);
            }
            break;
        }
        case KeyNodeInformation:
        {
            if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
            {
                PKEY_NODE_INFORMATION info;
                ULONG length;

                info = KeyInformation;
                length = (Length - FIELD_OFFSET(KEY_NODE_INFORMATION, Name));

                AVrfFuzzBuffer(info->Name, length, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
                AVrfFuzzSizeTruncateWideString(&info->NameLength);
                AVrfFuzzSizeTruncateWideString(&info->ClassLength);
            }
            break;
        }
        case KeyFullInformation:
        {
            if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
            {
                PKEY_FULL_INFORMATION info;
                ULONG length;

                info = KeyInformation;
                length = (Length - FIELD_OFFSET(KEY_FULL_INFORMATION, Class));

                AVrfFuzzBuffer(info->Class, length, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
                AVrfFuzzSizeTruncateWideString(&info->ClassLength);
            }
            break;
        }
        case KeyNameInformation:
        {
            if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
            {
                PKEY_NAME_INFORMATION info;
                ULONG length;

                info = KeyInformation;
                length = (Length - FIELD_OFFSET(KEY_NAME_INFORMATION, Name));

                AVrfFuzzBuffer(info->Name, length, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
                AVrfFuzzSizeTruncateWideString(&info->NameLength);
            }
            break;
        }
        case KeyCachedInformation:
        {
            if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
            {
                PKEY_CACHED_INFORMATION info;
                ULONG length;

                info = KeyInformation;
                length = (Length - FIELD_OFFSET(KEY_CACHED_INFORMATION, Name));

                AVrfFuzzBuffer(info->Name, length, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
                AVrfFuzzSizeTruncateWideString(&info->NameLength);
            }
            break;
        }
    }
}

VOID AVrfpFuzzKeyValueInformation(
    _In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    _Inout_updates_bytes_(Length) PVOID KeyValueInformation,
    _In_ ULONG Length,
    _In_ PVOID Context
    )
{
    AVRF_HOOK_WITH_CONTEXT(Context);

    switch (KeyValueInformationClass)
    {
        case KeyValueBasicInformation:
        {
            if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
            {
                PKEY_VALUE_BASIC_INFORMATION info;
                ULONG length;

                info = KeyValueInformation;
                length = (Length - FIELD_OFFSET(KEY_VALUE_BASIC_INFORMATION, Name));

                AVrfFuzzBuffer(info->Name, length, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
                AVrfFuzzSizeTruncateWideString(&info->NameLength);
            }
            break;
        }
        case KeyValueFullInformation:
        case KeyValueFullInformationAlign64:
        {
            if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
            {
                PKEY_VALUE_FULL_INFORMATION info;
                ULONG length;

                info = KeyValueInformation;
                length = (Length - FIELD_OFFSET(KEY_VALUE_FULL_INFORMATION, Name));

                AVrfFuzzBuffer(info->Name, length, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
                AVrfFuzzSizeTruncateWideString(&info->DataLength);

                if ((info->Type == REG_SZ) ||
                    (info->Type == REG_EXPAND_SZ) ||
                    (info->Type == REG_MULTI_SZ))
                {
                    AVrfFuzzSizeTruncateWideString(&info->DataLength);
                }
                else
                {
                    AVrfFuzzSizeTruncateULong(&info->DataLength);
                }
            }
            break;
        }
        case KeyValuePartialInformation:
        {
            if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
            {
                PKEY_VALUE_PARTIAL_INFORMATION info;
                ULONG length;

                info = KeyValueInformation;
                length = (Length - FIELD_OFFSET(KEY_VALUE_PARTIAL_INFORMATION, Data));

                AVrfFuzzBuffer(info->Data, length, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);

                if ((info->Type == REG_SZ) ||
                    (info->Type == REG_EXPAND_SZ) ||
                    (info->Type == REG_MULTI_SZ))
                {
                    AVrfFuzzSizeTruncateWideString(&info->DataLength);
                }
                else
                {
                    AVrfFuzzSizeTruncateULong(&info->DataLength);
                }
            }
            break;
        }
        case KeyValuePartialInformationAlign64:
        {
            if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
            {
                PKEY_VALUE_PARTIAL_INFORMATION_ALIGN64 info;
                ULONG length;

                info = KeyValueInformation;
                length = (Length - FIELD_OFFSET(KEY_VALUE_PARTIAL_INFORMATION_ALIGN64, Data));

                AVrfFuzzBuffer(info->Data, length, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);

                if ((info->Type == REG_SZ) ||
                    (info->Type == REG_EXPAND_SZ) ||
                    (info->Type == REG_MULTI_SZ))
                {
                    AVrfFuzzSizeTruncateWideString(&info->DataLength);
                }
                else
                {
                    AVrfFuzzSizeTruncateULong(&info->DataLength);
                }
            }
            break;
        }
    }
}

NTSTATUS
NTAPI
Hook_NtCreateKey(
    _Out_ PHANDLE KeyHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Reserved_ ULONG TitleIndex,
    _In_opt_ PUNICODE_STRING Class,
    _In_ ULONG CreateOptions,
    _Out_opt_ PULONG Disposition
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        return STATUS_NO_MEMORY;
    }

    return Orig_NtCreateKey(KeyHandle,
                            DesiredAccess,
                            ObjectAttributes,
                            TitleIndex,
                            Class,
                            CreateOptions,
                            Disposition);
}

NTSTATUS
NTAPI
Hook_NtOpenKey(
    _Out_ PHANDLE KeyHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        return STATUS_NO_MEMORY;
    }

    return Orig_NtOpenKey(KeyHandle, DesiredAccess, ObjectAttributes);
}

NTSTATUS
NTAPI
Hook_NtSetValueKey(
    _In_ HANDLE KeyHandle,
    _In_ PUNICODE_STRING ValueName,
    _In_opt_ ULONG TitleIndex,
    _In_ ULONG Type,
    _In_reads_bytes_opt_(DataSize) PVOID Data,
    _In_ ULONG DataSize
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        return STATUS_NO_MEMORY;
    }

    if (AVrfProperties.EnableWriteFuzzedDataChecks &&
        AVrfHookShouldVerifierStop() &&
        AVrfBufferIsPossiblyFuzzed(Data, DataSize))
    {
        VerifierStopMessageEx(&AVrfLayerDescriptor,
                              VFDYNF_CODE_WRITE_FUZZED_DATA_REG,
                              (ULONG_PTR)Data,
                              DataSize,
                              0,
                              0,
                              0);
    }

    return Orig_NtSetValueKey(KeyHandle,
                              ValueName,
                              TitleIndex,
                              Type,
                              Data,
                              DataSize);
}

NTSTATUS
NTAPI
Hook_NtQueryKey(
    _In_ HANDLE KeyHandle,
    _In_ KEY_INFORMATION_CLASS KeyInformationClass,
    _Out_writes_bytes_opt_(Length) PVOID KeyInformation,
    _In_ ULONG Length,
    _Out_ PULONG ResultLength
    )
{
    NTSTATUS status;

    AVRF_HOOK_CONTEXT();

    if (KeyInformation &&
        AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(KeyInformation, Length);
    }

    status = Orig_NtQueryKey(KeyHandle,
                             KeyInformationClass,
                             KeyInformation,
                             Length,
                             ResultLength);

    if ((status == STATUS_INFO_LENGTH_MISMATCH) ||
        (status == STATUS_BUFFER_TOO_SMALL) ||
        (status == STATUS_BUFFER_OVERFLOW))
    {
        if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(ResultLength, REG_BINARY);
        }

        return status;
    }

    if (NT_SUCCESS(status) && KeyInformation)
    {
        AVrfpFuzzKeyInformation(KeyInformationClass,
                                KeyInformation,
                                Length,
                                AVrfHookGetContext());
    }

    return status;
}

NTSTATUS
NTAPI
Hook_NtQueryValueKey(
    _In_ HANDLE KeyHandle,
    _In_ PUNICODE_STRING ValueName,
    _In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    _Out_writes_bytes_opt_(Length) PVOID KeyValueInformation,
    _In_ ULONG Length,
    _Out_ PULONG ResultLength
    )
{
    NTSTATUS status;

    AVRF_HOOK_CONTEXT();

    if (KeyValueInformation &&
        AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(KeyValueInformation, Length);
    }

    status = Orig_NtQueryValueKey(KeyHandle,
                                  ValueName,
                                  KeyValueInformationClass,
                                  KeyValueInformation,
                                  Length,
                                  ResultLength);

    if ((status == STATUS_INFO_LENGTH_MISMATCH) ||
        (status == STATUS_BUFFER_TOO_SMALL) ||
        (status == STATUS_BUFFER_OVERFLOW))
    {
        if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(ResultLength, REG_BINARY);
        }

        return status;
    }

    if (NT_SUCCESS(status) && KeyValueInformation)
    {
        AVrfpFuzzKeyValueInformation(KeyValueInformationClass,
                                     KeyValueInformation,
                                     Length,
                                     AVrfHookGetContext());
    }

    return status;
}

NTSTATUS
NTAPI
Hook_NtQueryMultipleValueKey(
    _In_ HANDLE KeyHandle,
    _Inout_updates_(EntryCount) PKEY_VALUE_ENTRY ValueEntries,
    _In_ ULONG EntryCount,
    _Out_writes_bytes_(*BufferLength) PVOID ValueBuffer,
    _Inout_ PULONG BufferLength,
    _Out_opt_ PULONG RequiredBufferLength
    )
{
    NTSTATUS status;
    ULONG inputLength;

    AVRF_HOOK_CONTEXT();

    inputLength = *BufferLength;

    if (AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(ValueBuffer, inputLength);
    }

    status = Orig_NtQueryMultipleValueKey(KeyHandle,
                                          ValueEntries,
                                          EntryCount,
                                          ValueBuffer,
                                          BufferLength,
                                          RequiredBufferLength);

    if ((status == STATUS_INFO_LENGTH_MISMATCH) ||
        (status == STATUS_BUFFER_TOO_SMALL) ||
        (status == STATUS_BUFFER_OVERFLOW))
    {
        if (RequiredBufferLength &&
            AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(RequiredBufferLength, REG_BINARY);
        }

        return status;
    }

    if (!NT_SUCCESS(status))
    {
        return status;
    }

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzBuffer(ValueBuffer,
                       inputLength,
                       VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateULong(BufferLength);
    }

    return status;
}

NTSTATUS
NTAPI
Hook_NtEnumerateKey(
    _In_ HANDLE KeyHandle,
    _In_ ULONG Index,
    _In_ KEY_INFORMATION_CLASS KeyInformationClass,
    _Out_writes_bytes_opt_(Length) PVOID KeyInformation,
    _In_ ULONG Length,
    _Out_ PULONG ResultLength
    )
{
    NTSTATUS status;

    AVRF_HOOK_CONTEXT();

    if (KeyInformation &&
        AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(KeyInformation, Length);
    }

    status = Orig_NtEnumerateKey(KeyHandle,
                                 Index,
                                 KeyInformationClass,
                                 KeyInformation,
                                 Length,
                                 ResultLength);

    if ((status == STATUS_INFO_LENGTH_MISMATCH) ||
        (status == STATUS_BUFFER_TOO_SMALL) ||
        (status == STATUS_BUFFER_OVERFLOW))
    {
        if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(ResultLength, REG_BINARY);
        }

        return status;
    }

    if (NT_SUCCESS(status) && KeyInformation)
    {
        AVrfpFuzzKeyInformation(KeyInformationClass,
                                KeyInformation,
                                Length,
                                AVrfHookGetContext());
    }

    return status;
}

NTSTATUS
NTAPI
Hook_NtEnumerateValueKey(
    _In_ HANDLE KeyHandle,
    _In_ ULONG Index,
    _In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    _Out_writes_bytes_opt_(Length) PVOID KeyValueInformation,
    _In_ ULONG Length,
    _Out_ PULONG ResultLength
    )
{
    NTSTATUS status;

    AVRF_HOOK_CONTEXT();

    if (KeyValueInformation &&
        AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(KeyValueInformation, Length);
    }

    status = Orig_NtEnumerateValueKey(KeyHandle,
                                      Index,
                                      KeyValueInformationClass,
                                      KeyValueInformation,
                                      Length,
                                      ResultLength);

    if ((status == STATUS_INFO_LENGTH_MISMATCH) ||
        (status == STATUS_BUFFER_TOO_SMALL) ||
        (status == STATUS_BUFFER_OVERFLOW))
    {
        if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(ResultLength, REG_BINARY);
        }

        return status;
    }

    if (NT_SUCCESS(status) && KeyValueInformation)
    {
        AVrfpFuzzKeyValueInformation(KeyValueInformationClass,
                                     KeyValueInformation,
                                     Length,
                                     AVrfHookGetContext());
    }

    return status;
}

LSTATUS
APIENTRY
Hook_Common_RegCreateKeyA(
    _In_ PFunc_RegCreateKeyA Orig_RegCreateKeyA,
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        *phkResult = NULL;
        return ERROR_OUTOFMEMORY;
    }

    return Orig_RegCreateKeyA(hKey, lpSubKey, phkResult);
}

LSTATUS
APIENTRY
Hook_Common_RegCreateKeyW(
    _In_ PFunc_RegCreateKeyW Orig_RegCreateKeyW,
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        *phkResult = NULL;
        return ERROR_OUTOFMEMORY;
    }

    return Orig_RegCreateKeyW(hKey, lpSubKey, phkResult);
}

LSTATUS
APIENTRY
Hook_Common_RegCreateKeyExA(
    _In_ PFunc_RegCreateKeyExA Orig_RegCreateKeyExA,
    _In_ HKEY hKey,
    _In_ LPCSTR lpSubKey,
    _Reserved_ DWORD Reserved,
    _In_opt_ LPSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_opt_ LPDWORD lpdwDisposition
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        *phkResult = NULL;
        return ERROR_OUTOFMEMORY;
    }

    return Orig_RegCreateKeyExA(hKey,
                                lpSubKey,
                                Reserved,
                                lpClass,
                                dwOptions,
                                samDesired,
                                lpSecurityAttributes,
                                phkResult,
                                lpdwDisposition);
}

LSTATUS
APIENTRY
Hook_Common_RegCreateKeyExW(
    _In_ PFunc_RegCreateKeyExW Orig_RegCreateKeyExW,
    _In_ HKEY hKey,
    _In_ LPCWSTR lpSubKey,
    _Reserved_ DWORD Reserved,
    _In_opt_ LPWSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_opt_ LPDWORD lpdwDisposition
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        *phkResult = NULL;
        return ERROR_OUTOFMEMORY;
    }

    return Orig_RegCreateKeyExW(hKey,
                                lpSubKey,
                                Reserved,
                                lpClass,
                                dwOptions,
                                samDesired,
                                lpSecurityAttributes,
                                phkResult,
                                lpdwDisposition);
}

LSTATUS
APIENTRY
Hook_Common_RegOpenKeyA(
    _In_ PFunc_RegOpenKeyA Orig_RegOpenKeyA,
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        *phkResult = NULL;
        return ERROR_OUTOFMEMORY;
    }

    return Orig_RegOpenKeyA(hKey, lpSubKey, phkResult);
}

LSTATUS
APIENTRY
Hook_Common_RegOpenKeyW(
    _In_ PFunc_RegOpenKeyW Orig_RegOpenKeyW,
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        *phkResult = NULL;
        return ERROR_OUTOFMEMORY;
    }

    return Orig_RegOpenKeyW(hKey, lpSubKey, phkResult);
}

LSTATUS
APIENTRY
Hook_Common_RegOpenKeyExA(
    _In_ PFunc_RegOpenKeyExA Orig_RegOpenKeyExA,
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _In_opt_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Out_ PHKEY phkResult
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        *phkResult = NULL;
        return ERROR_OUTOFMEMORY;
    }

    return Orig_RegOpenKeyExA(hKey,
                              lpSubKey,
                              ulOptions,
                              samDesired,
                              phkResult);
}

LSTATUS
APIENTRY
Hook_Common_RegOpenKeyExW(
    _In_ PFunc_RegOpenKeyExW Orig_RegOpenKeyExW,
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_opt_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Out_ PHKEY phkResult
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        *phkResult = NULL;
        return ERROR_OUTOFMEMORY;
    }

    return Orig_RegOpenKeyExW(hKey,
                              lpSubKey,
                              ulOptions,
                              samDesired,
                              phkResult);
}

LSTATUS
APIENTRY
Hook_Common_RegSetValueA(
    _In_ PFunc_RegSetValueA Orig_RegSetValueA,
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) LPCSTR lpData,
    _In_ DWORD cbData
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        return ERROR_OUTOFMEMORY;
    }

    if (AVrfProperties.EnableWriteFuzzedDataChecks &&
        AVrfHookShouldVerifierStop() &&
        AVrfBufferIsPossiblyFuzzed(lpData, cbData))
    {
        VerifierStopMessageEx(&AVrfLayerDescriptor,
                              VFDYNF_CODE_WRITE_FUZZED_DATA_REG,
                              (ULONG_PTR)lpData,
                              cbData,
                              0,
                              0,
                              0);
    }

    return Orig_RegSetValueA(hKey, lpSubKey, dwType, lpData, cbData);
}

LSTATUS
APIENTRY
Hook_Common_RegSetValueW(
    _In_ PFunc_RegSetValueW Orig_RegSetValueW,
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) LPCWSTR lpData,
    _In_ DWORD cbData
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        return ERROR_OUTOFMEMORY;
    }

    if (AVrfProperties.EnableWriteFuzzedDataChecks &&
        AVrfHookShouldVerifierStop() &&
        AVrfBufferIsPossiblyFuzzed(lpData, cbData))
    {
        VerifierStopMessageEx(&AVrfLayerDescriptor,
                              VFDYNF_CODE_WRITE_FUZZED_DATA_REG,
                              (ULONG_PTR)lpData,
                              cbData,
                              0,
                              0,
                              0);
    }

    return Orig_RegSetValueW(hKey, lpSubKey, dwType, lpData, cbData);
}

LSTATUS
APIENTRY
Hook_Common_RegSetValueExA(
    _In_ PFunc_RegSetValueExA Orig_RegSetValueExA,
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpValueName,
    _Reserved_ DWORD Reserved,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) CONST BYTE* lpData,
    _In_ DWORD cbData
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        return ERROR_OUTOFMEMORY;
    }

    if (AVrfProperties.EnableWriteFuzzedDataChecks &&
        AVrfHookShouldVerifierStop() &&
        AVrfBufferIsPossiblyFuzzed(lpData, cbData))
    {
        VerifierStopMessageEx(&AVrfLayerDescriptor,
                              VFDYNF_CODE_WRITE_FUZZED_DATA_REG,
                              (ULONG_PTR)lpData,
                              cbData,
                              0,
                              0,
                              0);
    }

    return Orig_RegSetValueExA(hKey,
                               lpValueName,
                               Reserved,
                               dwType,
                               lpData,
                               cbData);
}

LSTATUS
APIENTRY
Hook_Common_RegSetValueExW(
    _In_ PFunc_RegSetValueExW Orig_RegSetValueExW,
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpValueName,
    _Reserved_ DWORD Reserved,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) CONST BYTE* lpData,
    _In_ DWORD cbData
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_REG))
    {
        return ERROR_OUTOFMEMORY;
    }

    if (AVrfProperties.EnableWriteFuzzedDataChecks &&
        AVrfHookShouldVerifierStop() &&
        AVrfBufferIsPossiblyFuzzed(lpData, cbData))
    {
        VerifierStopMessageEx(&AVrfLayerDescriptor,
                              VFDYNF_CODE_WRITE_FUZZED_DATA_REG,
                              (ULONG_PTR)lpData,
                              cbData,
                              0,
                              0,
                              0);
    }

    return Orig_RegSetValueExW(hKey,
                               lpValueName,
                               Reserved,
                               dwType,
                               lpData,
                               cbData);
}

LSTATUS
APIENTRY
Hook_Common_RegQueryValueA(
    _In_ PFunc_RegQueryValueA Orig_RegQueryValueA,
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPSTR lpData,
    _Inout_opt_ PLONG lpcbData
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldVerifierStop())
    {
        VerifierStopMessageEx(&AVrfLayerDescriptor,
                              VFDYNF_CODE_DEPRECATED_FUNCTION,
                              0,
                              0,
                              0,
                              0,
                              0,
                              L"RegQueryValueA",
                              L"RegQueryValueExA");
    }

    return Orig_RegQueryValueA(hKey, lpSubKey, lpData, lpcbData);
}

LSTATUS
APIENTRY
Hook_Common_RegQueryValueW(
    _In_ PFunc_RegQueryValueW Orig_RegQueryValueW,
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPWSTR lpData,
    _Inout_opt_ PLONG lpcbData
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldVerifierStop())
    {
        VerifierStopMessageEx(&AVrfLayerDescriptor,
                              VFDYNF_CODE_DEPRECATED_FUNCTION,
                              0,
                              0,
                              0,
                              0,
                              0,
                              L"RegQueryValueW",
                              L"RegQueryValueExW");
    }

    return Orig_RegQueryValueW(hKey, lpSubKey, lpData, lpcbData);
}

LSTATUS
APIENTRY
Hook_Common_RegQueryMultipleValuesA(
    _In_ PFunc_RegQueryMultipleValuesA Orig_RegQueryMultipleValuesA,
    _In_ HKEY hKey,
    _Out_writes_(num_vals) PVALENTA val_list,
    _In_ DWORD num_vals,
    _Out_writes_bytes_to_opt_(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPSTR lpValueBuf,
    _Inout_opt_ LPDWORD ldwTotsize
    )
{
    LSTATUS status;
    ULONG inputLength;

    AVRF_HOOK_CONTEXT();

    inputLength = ldwTotsize ? *ldwTotsize : 0;

    if (lpValueBuf && AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(lpValueBuf, inputLength);
    }

    status = Orig_RegQueryMultipleValuesA(hKey,
                                          val_list,
                                          num_vals,
                                          lpValueBuf,
                                          ldwTotsize);

    if ((status == ERROR_BAD_LENGTH) ||
        (status == ERROR_INSUFFICIENT_BUFFER) ||
        (status == ERROR_MORE_DATA))
    {
        if (ldwTotsize && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(ldwTotsize, REG_BINARY);
        }

        return status;
    }

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (lpValueBuf && ldwTotsize && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzBuffer(lpValueBuf, inputLength, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateULong(ldwTotsize);
    }

    return status;
}

LSTATUS
APIENTRY
Hook_Common_RegQueryMultipleValuesW(
    _In_ PFunc_RegQueryMultipleValuesW Orig_RegQueryMultipleValuesW,
    _In_ HKEY hKey,
    _Out_writes_(num_vals) PVALENTW val_list,
    _In_ DWORD num_vals,
    _Out_writes_bytes_to_opt_(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPWSTR lpValueBuf,
    _Inout_opt_ LPDWORD ldwTotsize
    )
{
    LSTATUS status;
    ULONG inputLength;

    AVRF_HOOK_CONTEXT();

    inputLength = ldwTotsize ? *ldwTotsize : 0;

    if (lpValueBuf && AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(lpValueBuf, inputLength);
    }

    status = Orig_RegQueryMultipleValuesW(hKey,
                                          val_list,
                                          num_vals,
                                          lpValueBuf,
                                          ldwTotsize);

    if ((status == ERROR_BAD_LENGTH) ||
        (status == ERROR_INSUFFICIENT_BUFFER) ||
        (status == ERROR_MORE_DATA))
    {
        if (ldwTotsize && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(ldwTotsize, REG_BINARY);
        }

        return status;
    }

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (lpValueBuf && ldwTotsize && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzBuffer(lpValueBuf, inputLength, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateULong(ldwTotsize);
    }

    return status;
}

LSTATUS
APIENTRY
Hook_Common_RegQueryValueExA(
    _In_ PFunc_RegQueryValueExA Orig_RegQueryValueExA,
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _When_(lpData == NULL, _Out_opt_) _When_(lpData != NULL, _Inout_opt_) LPDWORD lpcbData
    )
{
    LSTATUS status;
    ULONG inputLength;
    ULONG type;

    AVRF_HOOK_CONTEXT();

    inputLength = lpcbData ? *lpcbData : 0;

    if (lpData && AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(lpData, inputLength);
    }

    status = Orig_RegQueryValueExA(hKey,
                                   lpValueName,
                                   lpReserved,
                                   &type,
                                   lpData,
                                   lpcbData);

    if (lpType)
    {
        *lpType = type;
    }

    if ((status == ERROR_BAD_LENGTH) ||
        (status == ERROR_INSUFFICIENT_BUFFER) ||
        (status == ERROR_MORE_DATA))
    {
        if (lpcbData && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(lpcbData, type);
        }

        return status;
    }

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (lpData && lpcbData && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzBuffer(lpData, inputLength, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateULong(lpcbData);
    }

    return status;
}

LSTATUS
APIENTRY
Hook_Common_RegQueryValueExW(
    _In_ PFunc_RegQueryValueExW Orig_RegQueryValueExW,
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _When_(lpData == NULL, _Out_opt_) _When_(lpData != NULL, _Inout_opt_) LPDWORD lpcbData
    )
{
    LSTATUS status;
    ULONG inputLength;
    ULONG type;

    AVRF_HOOK_CONTEXT();

    inputLength = lpcbData ? *lpcbData : 0;

    if (lpData && AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(lpData, inputLength);
    }

    status = Orig_RegQueryValueExW(hKey,
                                   lpValueName,
                                   lpReserved,
                                   &type,
                                   lpData,
                                   lpcbData);

    if (lpType)
    {
        *lpType = type;
    }

    if ((status == ERROR_BAD_LENGTH) ||
        (status == ERROR_INSUFFICIENT_BUFFER) ||
        (status == ERROR_MORE_DATA))
    {
        if (lpcbData && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(lpcbData, type);
        }

        return status;
    }

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (lpData && lpcbData && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzBuffer(lpData, inputLength, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateULong(lpcbData);
    }

    return status;
}

#pragma prefast(push)
#pragma prefast(disable : 6054) // String 'pvData' might not be zero-terminated.
LSTATUS
APIENTRY
Hook_Common_RegGetValueA(
    _In_ PFunc_RegGetValueA Orig_RegGetValueA,
    _In_ HKEY hkey,
    _In_opt_ LPCSTR lpSubKey,
    _In_opt_ LPCSTR lpValue,
    _In_ DWORD dwFlags,
    _Out_opt_ LPDWORD pdwType,
    _When_((dwFlags & 0x7F) == RRF_RT_REG_SZ ||
               (dwFlags & 0x7F) == RRF_RT_REG_EXPAND_SZ ||
               (dwFlags & 0x7F) == (RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ) ||
               *pdwType == REG_SZ ||
               *pdwType == REG_EXPAND_SZ, _Post_z_)
        _When_((dwFlags & 0x7F) == RRF_RT_REG_MULTI_SZ ||
               *pdwType == REG_MULTI_SZ, _Post_ _NullNull_terminated_)
    _Out_writes_bytes_to_opt_(*pcbData,*pcbData) PVOID pvData,
    _Inout_opt_ LPDWORD pcbData
    )
{
    LSTATUS status;
    ULONG inputLength;
    ULONG type;

    AVRF_HOOK_CONTEXT();

    inputLength = pcbData ? *pcbData : 0;

    if (pvData && AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(pvData, inputLength);
    }

    status = Orig_RegGetValueA(hkey,
                               lpSubKey,
                               lpValue,
                               dwFlags,
                               &type,
                               pvData,
                               pcbData);

    if (pdwType)
    {
        *pdwType = type;
    }

    if ((status == ERROR_BAD_LENGTH) ||
        (status == ERROR_INSUFFICIENT_BUFFER) ||
        (status == ERROR_MORE_DATA))
    {
        if (pcbData && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(pcbData, type);
        }

        return status;
    }

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (pvData && pcbData && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzBuffer(pvData, inputLength, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateULong(pcbData);

        if ((*pcbData >= sizeof(CHAR)) && (type == REG_SZ) || (type == REG_EXPAND_SZ))
        {
            ((PCHAR)pvData)[*pcbData - 1] = ANSI_NULL;
        }

        if ((*pcbData >= (sizeof(CHAR) * 2)) && (type == REG_MULTI_SZ))
        {
            ((PCHAR)pvData)[*pcbData - 1] = ANSI_NULL;
            ((PCHAR)pvData)[*pcbData - 2] = ANSI_NULL;
        }
    }

    return status;
}
#pragma prefast(pop)

#pragma prefast(push)
#pragma prefast(disable : 6054) // String 'pvData' might not be zero-terminated.
LSTATUS
APIENTRY
Hook_Common_RegGetValueW(
    _In_ PFunc_RegGetValueW Orig_RegGetValueW,
    _In_ HKEY hkey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_opt_ LPCWSTR lpValue,
    _In_ DWORD dwFlags,
    _Out_opt_ LPDWORD pdwType,
    _When_((dwFlags & 0x7F) == RRF_RT_REG_SZ ||
               (dwFlags & 0x7F) == RRF_RT_REG_EXPAND_SZ ||
               (dwFlags & 0x7F) == (RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ) ||
               *pdwType == REG_SZ ||
               *pdwType == REG_EXPAND_SZ, _Post_z_)
        _When_((dwFlags & 0x7F) == RRF_RT_REG_MULTI_SZ ||
               *pdwType == REG_MULTI_SZ, _Post_ _NullNull_terminated_)
    _Out_writes_bytes_to_opt_(*pcbData,*pcbData) PVOID pvData,
    _Inout_opt_ LPDWORD pcbData
    )
{
    LSTATUS status;
    ULONG inputLength;
    ULONG type;

    AVRF_HOOK_CONTEXT();

    inputLength = pcbData ? *pcbData : 0;

    if (pvData && AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzFillMemory(pvData, inputLength);
    }

    status = Orig_RegGetValueW(hkey,
                               lpSubKey,
                               lpValue,
                               dwFlags,
                               &type,
                               pvData,
                               pcbData);

    if (pdwType)
    {
        *pdwType = type;
    }

    if ((status == ERROR_BAD_LENGTH) ||
        (status == ERROR_INSUFFICIENT_BUFFER) ||
        (status == ERROR_MORE_DATA))
    {
        if (pcbData && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(pcbData, type);
        }

        return status;
    }

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (pvData && pcbData && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        AVrfFuzzBuffer(pvData, inputLength, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);

        if ((type == REG_SZ) ||
            (type == REG_EXPAND_SZ) ||
            (type == REG_MULTI_SZ))
        {
            AVrfFuzzSizeTruncateWideString(pcbData);
        }
        else
        {
            AVrfFuzzSizeTruncateULong(pcbData);
        }

        if ((*pcbData >= sizeof(WCHAR)) && (type == REG_SZ) || (type == REG_EXPAND_SZ))
        {
            ((PWCHAR)pvData)[(*pcbData / sizeof(WCHAR)) - 1] = UNICODE_NULL;
        }

        if ((*pcbData >= (sizeof(WCHAR) * 2)) && (type == REG_MULTI_SZ))
        {
            ((PWCHAR)pvData)[(*pcbData / sizeof(WCHAR)) - 1] = UNICODE_NULL;
            ((PWCHAR)pvData)[(*pcbData / sizeof(WCHAR)) - 2] = UNICODE_NULL;
        }
    }

    return status;
}
#pragma prefast(pop)

LSTATUS
APIENTRY
Hook_Common_RegEnumKeyA(
    _In_ PFunc_RegEnumKeyA Orig_RegEnumKeyA,
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(cchName) LPSTR lpName,
    _In_ DWORD cchName
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldVerifierStop())
    {
        VerifierStopMessageEx(&AVrfLayerDescriptor,
                              VFDYNF_CODE_DEPRECATED_FUNCTION,
                              0,
                              0,
                              0,
                              0,
                              0,
                              L"RegEnumKeyA",
                              L"RegEnumKeyExA");
    }

    return Orig_RegEnumKeyA(hKey, dwIndex, lpName, cchName);
}

LSTATUS
APIENTRY
Hook_Common_RegEnumKeyW(
    _In_ PFunc_RegEnumKeyW Orig_RegEnumKeyW,
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(cchName) LPWSTR lpName,
    _In_ DWORD cchName
    )
{
    AVRF_HOOK_CONTEXT();

    if (AVrfHookShouldVerifierStop())
    {
        VerifierStopMessageEx(&AVrfLayerDescriptor,
                              VFDYNF_CODE_DEPRECATED_FUNCTION,
                              0,
                              0,
                              0,
                              0,
                              0,
                              L"RegEnumKeyW",
                              L"RegEnumKeyExW");
    }

    return Orig_RegEnumKeyW(hKey, dwIndex, lpName, cchName);
}

LSTATUS
APIENTRY
Hook_Common_RegEnumKeyExA(
    _In_ PFunc_RegEnumKeyExA Orig_RegEnumKeyExA,
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPSTR lpName,
    _Inout_ LPDWORD lpcchName,
    _Reserved_ LPDWORD lpReserved,
    _Out_writes_to_opt_(*lpcchClass,*lpcchClass + 1) LPSTR lpClass,
    _Inout_opt_ LPDWORD lpcchClass,
    _Out_opt_ PFILETIME lpftLastWriteTime
    )
{
    LSTATUS status;
    ULONG inputNameLength;
    ULONG inputClassLength;

    inputNameLength = *lpcchName;
    inputClassLength = lpcchClass ? *lpcchClass : 0;

    AVRF_HOOK_CONTEXT();

    if (AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        if (lpName)
        {
            AVrfFuzzFillMemory(lpName, inputNameLength * sizeof(CHAR));
        }

        if (lpClass)
        {
            AVrfFuzzFillMemory(lpClass, inputClassLength * sizeof(CHAR));
        }
    }

    status = Orig_RegEnumKeyExA(hKey,
                                dwIndex,
                                lpName,
                                lpcchName,
                                lpReserved,
                                lpClass,
                                lpcchClass,
                                lpftLastWriteTime);

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (!AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        return status;
    }

    if (lpName)
    {
        AVrfFuzzBuffer(lpName,
                       inputNameLength * sizeof(CHAR),
                       VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateULong(lpcchName);
    }

    if (lpClass && lpcchClass)
    {
        AVrfFuzzBuffer(lpClass,
                       inputClassLength * sizeof(CHAR),
                       VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateULong(lpcchClass);
    }

    return status;
}

LSTATUS
APIENTRY
Hook_Common_RegEnumKeyExW(
    _In_ PFunc_RegEnumKeyExW Orig_RegEnumKeyExW,
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPWSTR lpName,
    _Inout_ LPDWORD lpcchName,
    _Reserved_ LPDWORD lpReserved,
    _Out_writes_to_opt_(*lpcchClass,*lpcchClass + 1) LPWSTR lpClass,
    _Inout_opt_ LPDWORD lpcchClass,
    _Out_opt_ PFILETIME lpftLastWriteTime
    )
{
    LSTATUS status;
    ULONG inputNameLength;
    ULONG inputClassLength;

    AVRF_HOOK_CONTEXT();

    inputNameLength = *lpcchName;
    inputClassLength = lpcchClass ? *lpcchClass : 0;

    if (AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        if (lpName)
        {
            AVrfFuzzFillMemory(lpName, inputNameLength * sizeof(WCHAR));
        }

        if (lpClass)
        {
            AVrfFuzzFillMemory(lpClass, inputClassLength * sizeof(WCHAR));
        }
    }

    status = Orig_RegEnumKeyExW(hKey,
                                dwIndex,
                                lpName,
                                lpcchName,
                                lpReserved,
                                lpClass,
                                lpcchClass,
                                lpftLastWriteTime);

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (!AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        return status;
    }

    if (lpName)
    {
        AVrfFuzzBuffer(lpName,
                       inputNameLength * sizeof(WCHAR),
                       VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateWideString(lpcchName);
    }

    if (lpClass && lpcchClass)
    {
        AVrfFuzzBuffer(lpClass,
                       inputClassLength * sizeof(WCHAR),
                       VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateWideString(lpcchClass);
    }

    return status;
}

LSTATUS
APIENTRY
Hook_Common_RegEnumValueA(
    _In_ PFunc_RegEnumValueA Orig_RegEnumValueA,
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
    _Inout_ LPDWORD lpcchValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
    )
{
    LSTATUS status;
    ULONG inputNameLength;
    ULONG inputLength;
    ULONG type;

    AVRF_HOOK_CONTEXT();

    inputNameLength = *lpcchValueName;
    inputLength = lpcbData ? *lpcbData : 0;

    if (AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        if (lpValueName)
        {
            AVrfFuzzFillMemory(lpValueName, inputNameLength * sizeof(CHAR));
        }

        if (lpData)
        {
            AVrfFuzzFillMemory(lpData, inputLength);
        }
    }

    status = Orig_RegEnumValueA(hKey,
                                dwIndex,
                                lpValueName,
                                lpcchValueName,
                                lpReserved,
                                &type,
                                lpData,
                                lpcbData);

    if (lpType)
    {
        *lpType = type;
    }

    if ((status == ERROR_BAD_LENGTH) ||
        (status == ERROR_INSUFFICIENT_BUFFER) ||
        (status == ERROR_MORE_DATA))
    {
        if (lpcbData && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(lpcbData, type);
        }

        return status;
    }

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (!AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        return status;
    }

    if (lpValueName)
    {
        AVrfFuzzBuffer(lpValueName,
                       inputNameLength * sizeof(CHAR),
                       VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateULong(lpcchValueName);
    }

    if (lpData && lpcbData)
    {
        AVrfFuzzBuffer(lpData, inputLength, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);

        if ((type == REG_SZ) ||
            (type == REG_EXPAND_SZ) ||
            (type == REG_MULTI_SZ))
        {
            AVrfFuzzSizeTruncateWideString(lpcbData);
        }
        else
        {
            AVrfFuzzSizeTruncateULong(lpcbData);
        }
    }

    return status;
}

LSTATUS
APIENTRY
Hook_Common_RegEnumValueW(
    _In_ PFunc_RegEnumValueW Orig_RegEnumValueW,
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPWSTR lpValueName,
    _Inout_ LPDWORD lpcchValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
    )
{
    LSTATUS status;
    ULONG inputNameLength;
    ULONG inputLength;
    ULONG type;

    AVRF_HOOK_CONTEXT();

    inputNameLength = *lpcchValueName;
    inputLength = lpcbData ? *lpcbData : 0;

    if (AVrfHookIsCallerIncluded(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        if (lpValueName)
        {
            AVrfFuzzFillMemory(lpValueName, inputNameLength * sizeof(WCHAR));
        }

        if (lpData)
        {
            AVrfFuzzFillMemory(lpData, inputLength);
        }
    }

    status = Orig_RegEnumValueW(hKey,
                                dwIndex,
                                lpValueName,
                                lpcchValueName,
                                lpReserved,
                                &type,
                                lpData,
                                lpcbData);

    if (lpType)
    {
        *lpType = type;
    }

    if ((status == ERROR_BAD_LENGTH) ||
        (status == ERROR_INSUFFICIENT_BUFFER) ||
        (status == ERROR_MORE_DATA))
    {
        if (lpcbData && AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
        {
            AVrfpPreFuzzKeyValueLength(lpcbData, type);
        }

        return status;
    }

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (!AVrfHookShouldFaultInject(VFDYNF_FAULT_TYPE_FUZZ_REG))
    {
        return status;
    }

    if (lpValueName)
    {
        AVrfFuzzBuffer(lpValueName,
                       inputNameLength * sizeof(WCHAR),
                       VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);
        AVrfFuzzSizeTruncateWideString(lpcchValueName);
    }

    if (lpData && lpcbData)
    {
        AVrfFuzzBuffer(lpData, inputLength, VFDYNF_FAULT_TYPE_INDEX_FUZZ_REG);

        if ((type == REG_SZ) ||
            (type == REG_EXPAND_SZ) ||
            (type == REG_MULTI_SZ))
        {
            AVrfFuzzSizeTruncateWideString(lpcbData);
        }
        else
        {
            AVrfFuzzSizeTruncateULong(lpcbData);
        }
    }

    return status;
}

LSTATUS
APIENTRY
Hook_Kernel32_RegCreateKeyA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegCreateKeyA,
                                    hKey,
                                    lpSubKey,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegCreateKeyW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegCreateKeyW,
                                    hKey,
                                    lpSubKey,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegCreateKeyExA(
    _In_ HKEY hKey,
    _In_ LPCSTR lpSubKey,
    _Reserved_ DWORD Reserved,
    _In_opt_ LPSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_opt_ LPDWORD lpdwDisposition
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegCreateKeyExA,
                                    hKey,
                                    lpSubKey,
                                    Reserved,
                                    lpClass,
                                    dwOptions,
                                    samDesired,
                                    lpSecurityAttributes,
                                    phkResult,
                                    lpdwDisposition);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegCreateKeyExW(
    _In_ HKEY hKey,
    _In_ LPCWSTR lpSubKey,
    _Reserved_ DWORD Reserved,
    _In_opt_ LPWSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_opt_ LPDWORD lpdwDisposition
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegCreateKeyExW,
                                    hKey,
                                    lpSubKey,
                                    Reserved,
                                    lpClass,
                                    dwOptions,
                                    samDesired,
                                    lpSecurityAttributes,
                                    phkResult,
                                    lpdwDisposition);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegOpenKeyA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegOpenKeyA,
                                    hKey,
                                    lpSubKey,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegOpenKeyW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegOpenKeyW,
                                    hKey,
                                    lpSubKey,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegOpenKeyExA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _In_opt_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegOpenKeyExA,
                                    hKey,
                                    lpSubKey,
                                    ulOptions,
                                    samDesired,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegOpenKeyExW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_opt_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegOpenKeyExW,
                                    hKey,
                                    lpSubKey,
                                    ulOptions,
                                    samDesired,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegSetValueA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) LPCSTR lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegSetValueA,
                                    hKey,
                                    lpSubKey,
                                    dwType,
                                    lpData,
                                    cbData);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegSetValueW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) LPCWSTR lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegSetValueW,
                                    hKey,
                                    lpSubKey,
                                    dwType,
                                    lpData,
                                    cbData);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegSetValueExA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpValueName,
    _Reserved_ DWORD Reserved,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) CONST BYTE* lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegSetValueExA,
                                    hKey,
                                    lpValueName,
                                    Reserved,
                                    dwType,
                                    lpData,
                                    cbData);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegSetValueExW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpValueName,
    _Reserved_ DWORD Reserved,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) CONST BYTE* lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegSetValueExW,
                                    hKey,
                                    lpValueName,
                                    Reserved,
                                    dwType,
                                    lpData,
                                    cbData);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegQueryValueA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPSTR lpData,
    _Inout_opt_ PLONG lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegQueryValueA,
                                    hKey,
                                    lpSubKey,
                                    lpData,
                                    lpcbData);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegQueryValueW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPWSTR lpData,
    _Inout_opt_ PLONG lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegQueryValueW,
                                    hKey,
                                    lpSubKey,
                                    lpData,
                                    lpcbData);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegQueryMultipleValuesA(
    _In_ HKEY hKey,
    _Out_writes_(num_vals) PVALENTA val_list,
    _In_ DWORD num_vals,
    _Out_writes_bytes_to_opt_(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPSTR lpValueBuf,
    _Inout_opt_ LPDWORD ldwTotsize
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegQueryMultipleValuesA,
                                    hKey,
                                    val_list,
                                    num_vals,
                                    lpValueBuf,
                                    ldwTotsize);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegQueryMultipleValuesW(
    _In_ HKEY hKey,
    _Out_writes_(num_vals) PVALENTW val_list,
    _In_ DWORD num_vals,
    _Out_writes_bytes_to_opt_(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPWSTR lpValueBuf,
    _Inout_opt_ LPDWORD ldwTotsize
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegQueryMultipleValuesW,
                                    hKey,
                                    val_list,
                                    num_vals,
                                    lpValueBuf,
                                    ldwTotsize);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegQueryValueExA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _When_(lpData == NULL, _Out_opt_) _When_(lpData != NULL, _Inout_opt_) LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegQueryValueExA,
                                    hKey,
                                    lpValueName,
                                    lpReserved,
                                    lpType,
                                    lpData,
                                    lpcbData);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegQueryValueExW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _When_(lpData == NULL, _Out_opt_) _When_(lpData != NULL, _Inout_opt_) LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegQueryValueExW,
                                    hKey,
                                    lpValueName,
                                    lpReserved,
                                    lpType,
                                    lpData,
                                    lpcbData);
}

#pragma prefast(push)
#pragma prefast(disable : 6054) // String 'pvData' might not be zero-terminated.
LSTATUS
APIENTRY
Hook_Kernel32_RegGetValueA(
    _In_ HKEY hkey,
    _In_opt_ LPCSTR lpSubKey,
    _In_opt_ LPCSTR lpValue,
    _In_ DWORD dwFlags,
    _Out_opt_ LPDWORD pdwType,
    _When_((dwFlags & 0x7F) == RRF_RT_REG_SZ ||
               (dwFlags & 0x7F) == RRF_RT_REG_EXPAND_SZ ||
               (dwFlags & 0x7F) == (RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ) ||
               *pdwType == REG_SZ ||
               *pdwType == REG_EXPAND_SZ, _Post_z_)
        _When_((dwFlags & 0x7F) == RRF_RT_REG_MULTI_SZ ||
               *pdwType == REG_MULTI_SZ, _Post_ _NullNull_terminated_)
    _Out_writes_bytes_to_opt_(*pcbData,*pcbData) PVOID pvData,
    _Inout_opt_ LPDWORD pcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegGetValueA,
                                    hkey,
                                    lpSubKey,
                                    lpValue,
                                    dwFlags,
                                    pdwType,
                                    pvData,
                                    pcbData);
}
#pragma prefast(pop)

#pragma prefast(push)
#pragma prefast(disable : 6054) // String 'pvData' might not be zero-terminated.
LSTATUS
APIENTRY
Hook_Kernel32_RegGetValueW(
    _In_ HKEY hkey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_opt_ LPCWSTR lpValue,
    _In_ DWORD dwFlags,
    _Out_opt_ LPDWORD pdwType,
    _When_((dwFlags & 0x7F) == RRF_RT_REG_SZ ||
               (dwFlags & 0x7F) == RRF_RT_REG_EXPAND_SZ ||
               (dwFlags & 0x7F) == (RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ) ||
               *pdwType == REG_SZ ||
               *pdwType == REG_EXPAND_SZ, _Post_z_)
        _When_((dwFlags & 0x7F) == RRF_RT_REG_MULTI_SZ ||
               *pdwType == REG_MULTI_SZ, _Post_ _NullNull_terminated_)
    _Out_writes_bytes_to_opt_(*pcbData,*pcbData) PVOID pvData,
    _Inout_opt_ LPDWORD pcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegGetValueW,
                                    hkey,
                                    lpSubKey,
                                    lpValue,
                                    dwFlags,
                                    pdwType,
                                    pvData,
                                    pcbData);
}
#pragma prefast(pop)

LSTATUS
APIENTRY
Hook_Kernel32_RegEnumKeyA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(cchName) LPSTR lpName,
    _In_ DWORD cchName
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegEnumKeyA,
                                    hKey,
                                    dwIndex,
                                    lpName,
                                    cchName);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegEnumKeyW(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(cchName) LPWSTR lpName,
    _In_ DWORD cchName
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegEnumKeyW,
                                    hKey,
                                    dwIndex,
                                    lpName,
                                    cchName);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegEnumKeyExA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPSTR lpName,
    _Inout_ LPDWORD lpcchName,
    _Reserved_ LPDWORD lpReserved,
    _Out_writes_to_opt_(*lpcchClass,*lpcchClass + 1) LPSTR lpClass,
    _Inout_opt_ LPDWORD lpcchClass,
    _Out_opt_ PFILETIME lpftLastWriteTime
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegEnumKeyExA,
                                    hKey,
                                    dwIndex,
                                    lpName,
                                    lpcchName,
                                    lpReserved,
                                    lpClass,
                                    lpcchClass,
                                    lpftLastWriteTime);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegEnumKeyExW(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPWSTR lpName,
    _Inout_ LPDWORD lpcchName,
    _Reserved_ LPDWORD lpReserved,
    _Out_writes_to_opt_(*lpcchClass,*lpcchClass + 1) LPWSTR lpClass,
    _Inout_opt_ LPDWORD lpcchClass,
    _Out_opt_ PFILETIME lpftLastWriteTime
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegEnumKeyExW,
                                    hKey,
                                    dwIndex,
                                    lpName,
                                    lpcchName,
                                    lpReserved,
                                    lpClass,
                                    lpcchClass,
                                    lpftLastWriteTime);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegEnumValueA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
    _Inout_ LPDWORD lpcchValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegEnumValueA,
                                    hKey,
                                    dwIndex,
                                    lpValueName,
                                    lpcchValueName,
                                    lpReserved,
                                    lpType,
                                    lpData,
                                    lpcbData);
}

LSTATUS
APIENTRY
Hook_Kernel32_RegEnumValueW(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPWSTR lpValueName,
    _Inout_ LPDWORD lpcchValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Kernel32,
                                    KernelBase,
                                    RegEnumValueW,
                                    hKey,
                                    dwIndex,
                                    lpValueName,
                                    lpcchValueName,
                                    lpReserved,
                                    lpType,
                                    lpData,
                                    lpcbData);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegCreateKeyA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegCreateKeyA,
                                   hKey,
                                   lpSubKey,
                                   phkResult);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegCreateKeyW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegCreateKeyW,
                                   hKey,
                                   lpSubKey,
                                   phkResult);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegCreateKeyExA(
    _In_ HKEY hKey,
    _In_ LPCSTR lpSubKey,
    _Reserved_ DWORD Reserved,
    _In_opt_ LPSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_opt_ LPDWORD lpdwDisposition
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegCreateKeyExA,
                                   hKey,
                                   lpSubKey,
                                   Reserved,
                                   lpClass,
                                   dwOptions,
                                   samDesired,
                                   lpSecurityAttributes,
                                   phkResult,
                                   lpdwDisposition);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegCreateKeyExW(
    _In_ HKEY hKey,
    _In_ LPCWSTR lpSubKey,
    _Reserved_ DWORD Reserved,
    _In_opt_ LPWSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_opt_ LPDWORD lpdwDisposition
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegCreateKeyExW,
                                   hKey,
                                   lpSubKey,
                                   Reserved,
                                   lpClass,
                                   dwOptions,
                                   samDesired,
                                   lpSecurityAttributes,
                                   phkResult,
                                   lpdwDisposition);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegOpenKeyA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegOpenKeyA,
                                   hKey,
                                   lpSubKey,
                                   phkResult);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegOpenKeyW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegOpenKeyW,
                                   hKey,
                                   lpSubKey,
                                   phkResult);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegOpenKeyExA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _In_opt_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegOpenKeyExA,
                                   hKey,
                                   lpSubKey,
                                   ulOptions,
                                   samDesired,
                                   phkResult);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegOpenKeyExW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_opt_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegOpenKeyExW,
                                   hKey,
                                   lpSubKey,
                                   ulOptions,
                                   samDesired,
                                   phkResult);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegSetValueA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) LPCSTR lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegSetValueA,
                                   hKey,
                                   lpSubKey,
                                   dwType,
                                   lpData,
                                   cbData);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegSetValueW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) LPCWSTR lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegSetValueW,
                                   hKey,
                                   lpSubKey,
                                   dwType,
                                   lpData,
                                   cbData);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegSetValueExA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpValueName,
    _Reserved_ DWORD Reserved,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) CONST BYTE* lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegSetValueExA,
                                   hKey,
                                   lpValueName,
                                   Reserved,
                                   dwType,
                                   lpData,
                                   cbData);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegSetValueExW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpValueName,
    _Reserved_ DWORD Reserved,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) CONST BYTE* lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegSetValueExW,
                                   hKey,
                                   lpValueName,
                                   Reserved,
                                   dwType,
                                   lpData,
                                   cbData);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegQueryValueA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPSTR lpData,
    _Inout_opt_ PLONG lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegQueryValueA,
                                   hKey,
                                   lpSubKey,
                                   lpData,
                                   lpcbData);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegQueryValueW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPWSTR lpData,
    _Inout_opt_ PLONG lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegQueryValueW,
                                   hKey,
                                   lpSubKey,
                                   lpData,
                                   lpcbData);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegQueryMultipleValuesA(
    _In_ HKEY hKey,
    _Out_writes_(num_vals) PVALENTA val_list,
    _In_ DWORD num_vals,
    _Out_writes_bytes_to_opt_(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPSTR lpValueBuf,
    _Inout_opt_ LPDWORD ldwTotsize
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegQueryMultipleValuesA,
                                   hKey,
                                   val_list,
                                   num_vals,
                                   lpValueBuf,
                                   ldwTotsize);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegQueryMultipleValuesW(
    _In_ HKEY hKey,
    _Out_writes_(num_vals) PVALENTW val_list,
    _In_ DWORD num_vals,
    _Out_writes_bytes_to_opt_(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPWSTR lpValueBuf,
    _Inout_opt_ LPDWORD ldwTotsize
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegQueryMultipleValuesW,
                                   hKey,
                                   val_list,
                                   num_vals,
                                   lpValueBuf,
                                   ldwTotsize);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegQueryValueExA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _When_(lpData == NULL, _Out_opt_) _When_(lpData != NULL, _Inout_opt_) LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegQueryValueExA,
                                   hKey,
                                   lpValueName,
                                   lpReserved,
                                   lpType,
                                   lpData,
                                   lpcbData);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegQueryValueExW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _When_(lpData == NULL, _Out_opt_) _When_(lpData != NULL, _Inout_opt_) LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegQueryValueExW,
                                   hKey,
                                   lpValueName,
                                   lpReserved,
                                   lpType,
                                   lpData,
                                   lpcbData);
}

#pragma prefast(push)
#pragma prefast(disable : 6054) // String 'pvData' might not be zero-terminated.
LSTATUS
APIENTRY
Hook_KernelBase_RegGetValueA(
    _In_ HKEY hkey,
    _In_opt_ LPCSTR lpSubKey,
    _In_opt_ LPCSTR lpValue,
    _In_ DWORD dwFlags,
    _Out_opt_ LPDWORD pdwType,
    _When_((dwFlags & 0x7F) == RRF_RT_REG_SZ ||
               (dwFlags & 0x7F) == RRF_RT_REG_EXPAND_SZ ||
               (dwFlags & 0x7F) == (RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ) ||
               *pdwType == REG_SZ ||
               *pdwType == REG_EXPAND_SZ, _Post_z_)
        _When_((dwFlags & 0x7F) == RRF_RT_REG_MULTI_SZ ||
               *pdwType == REG_MULTI_SZ, _Post_ _NullNull_terminated_)
    _Out_writes_bytes_to_opt_(*pcbData,*pcbData) PVOID pvData,
    _Inout_opt_ LPDWORD pcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegGetValueA,
                                   hkey,
                                   lpSubKey,
                                   lpValue,
                                   dwFlags,
                                   pdwType,
                                   pvData,
                                   pcbData);
}
#pragma prefast(pop)

#pragma prefast(push)
#pragma prefast(disable : 6054) // String 'pvData' might not be zero-terminated.
LSTATUS
APIENTRY
Hook_KernelBase_RegGetValueW(
    _In_ HKEY hkey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_opt_ LPCWSTR lpValue,
    _In_ DWORD dwFlags,
    _Out_opt_ LPDWORD pdwType,
    _When_((dwFlags & 0x7F) == RRF_RT_REG_SZ ||
               (dwFlags & 0x7F) == RRF_RT_REG_EXPAND_SZ ||
               (dwFlags & 0x7F) == (RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ) ||
               *pdwType == REG_SZ ||
               *pdwType == REG_EXPAND_SZ, _Post_z_)
        _When_((dwFlags & 0x7F) == RRF_RT_REG_MULTI_SZ ||
               *pdwType == REG_MULTI_SZ, _Post_ _NullNull_terminated_)
    _Out_writes_bytes_to_opt_(*pcbData,*pcbData) PVOID pvData,
    _Inout_opt_ LPDWORD pcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegGetValueW,
                                   hkey,
                                   lpSubKey,
                                   lpValue,
                                   dwFlags,
                                   pdwType,
                                   pvData,
                                   pcbData);
}
#pragma prefast(pop)

LSTATUS
APIENTRY
Hook_KernelBase_RegEnumKeyA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(cchName) LPSTR lpName,
    _In_ DWORD cchName
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegEnumKeyA,
                                   hKey,
                                   dwIndex,
                                   lpName,
                                   cchName);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegEnumKeyW(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(cchName) LPWSTR lpName,
    _In_ DWORD cchName
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegEnumKeyW,
                                   hKey,
                                   dwIndex,
                                   lpName,
                                   cchName);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegEnumKeyExA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPSTR lpName,
    _Inout_ LPDWORD lpcchName,
    _Reserved_ LPDWORD lpReserved,
    _Out_writes_to_opt_(*lpcchClass,*lpcchClass + 1) LPSTR lpClass,
    _Inout_opt_ LPDWORD lpcchClass,
    _Out_opt_ PFILETIME lpftLastWriteTime
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegEnumKeyExA,
                                   hKey,
                                   dwIndex,
                                   lpName,
                                   lpcchName,
                                   lpReserved,
                                   lpClass,
                                   lpcchClass,
                                   lpftLastWriteTime);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegEnumKeyExW(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPWSTR lpName,
    _Inout_ LPDWORD lpcchName,
    _Reserved_ LPDWORD lpReserved,
    _Out_writes_to_opt_(*lpcchClass,*lpcchClass + 1) LPWSTR lpClass,
    _Inout_opt_ LPDWORD lpcchClass,
    _Out_opt_ PFILETIME lpftLastWriteTime
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegEnumKeyExW,
                                   hKey,
                                   dwIndex,
                                   lpName,
                                   lpcchName,
                                   lpReserved,
                                   lpClass,
                                   lpcchClass,
                                   lpftLastWriteTime);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegEnumValueA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
    _Inout_ LPDWORD lpcchValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegEnumValueA,
                                   hKey,
                                   dwIndex,
                                   lpValueName,
                                   lpcchValueName,
                                   lpReserved,
                                   lpType,
                                   lpData,
                                   lpcbData);
}

LSTATUS
APIENTRY
Hook_KernelBase_RegEnumValueW(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPWSTR lpValueName,
    _Inout_ LPDWORD lpcchValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK(KernelBase,
                                   RegEnumValueW,
                                   hKey,
                                   dwIndex,
                                   lpValueName,
                                   lpcchValueName,
                                   lpReserved,
                                   lpType,
                                   lpData,
                                   lpcbData);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegCreateKeyA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegCreateKeyA,
                                    hKey,
                                    lpSubKey,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegCreateKeyW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegCreateKeyW,
                                    hKey,
                                    lpSubKey,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegCreateKeyExA(
    _In_ HKEY hKey,
    _In_ LPCSTR lpSubKey,
    _Reserved_ DWORD Reserved,
    _In_opt_ LPSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_opt_ LPDWORD lpdwDisposition
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegCreateKeyExA,
                                    hKey,
                                    lpSubKey,
                                    Reserved,
                                    lpClass,
                                    dwOptions,
                                    samDesired,
                                    lpSecurityAttributes,
                                    phkResult,
                                    lpdwDisposition);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegCreateKeyExW(
    _In_ HKEY hKey,
    _In_ LPCWSTR lpSubKey,
    _Reserved_ DWORD Reserved,
    _In_opt_ LPWSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_opt_ LPDWORD lpdwDisposition
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegCreateKeyExW,
                                    hKey,
                                    lpSubKey,
                                    Reserved,
                                    lpClass,
                                    dwOptions,
                                    samDesired,
                                    lpSecurityAttributes,
                                    phkResult,
                                    lpdwDisposition);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegOpenKeyA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegOpenKeyA,
                                    hKey,
                                    lpSubKey,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegOpenKeyW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegOpenKeyW,
                                    hKey,
                                    lpSubKey,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegOpenKeyExA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _In_opt_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegOpenKeyExA,
                                    hKey,
                                    lpSubKey,
                                    ulOptions,
                                    samDesired,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegOpenKeyExW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_opt_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Out_ PHKEY phkResult
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegOpenKeyExW,
                                    hKey,
                                    lpSubKey,
                                    ulOptions,
                                    samDesired,
                                    phkResult);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegSetValueA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) LPCSTR lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegSetValueA,
                                    hKey,
                                    lpSubKey,
                                    dwType,
                                    lpData,
                                    cbData);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegSetValueW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) LPCWSTR lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegSetValueW,
                                    hKey,
                                    lpSubKey,
                                    dwType,
                                    lpData,
                                    cbData);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegSetValueExA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpValueName,
    _Reserved_ DWORD Reserved,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) CONST BYTE* lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegSetValueExA,
                                    hKey,
                                    lpValueName,
                                    Reserved,
                                    dwType,
                                    lpData,
                                    cbData);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegSetValueExW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpValueName,
    _Reserved_ DWORD Reserved,
    _In_ DWORD dwType,
    _In_reads_bytes_opt_(cbData) CONST BYTE* lpData,
    _In_ DWORD cbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegSetValueExW,
                                    hKey,
                                    lpValueName,
                                    Reserved,
                                    dwType,
                                    lpData,
                                    cbData);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegQueryValueA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPSTR lpData,
    _Inout_opt_ PLONG lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegQueryValueA,
                                    hKey,
                                    lpSubKey,
                                    lpData,
                                    lpcbData);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegQueryValueW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPWSTR lpData,
    _Inout_opt_ PLONG lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegQueryValueW,
                                    hKey,
                                    lpSubKey,
                                    lpData,
                                    lpcbData);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegQueryMultipleValuesA(
    _In_ HKEY hKey,
    _Out_writes_(num_vals) PVALENTA val_list,
    _In_ DWORD num_vals,
    _Out_writes_bytes_to_opt_(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPSTR lpValueBuf,
    _Inout_opt_ LPDWORD ldwTotsize
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegQueryMultipleValuesA,
                                    hKey,
                                    val_list,
                                    num_vals,
                                    lpValueBuf,
                                    ldwTotsize);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegQueryMultipleValuesW(
    _In_ HKEY hKey,
    _Out_writes_(num_vals) PVALENTW val_list,
    _In_ DWORD num_vals,
    _Out_writes_bytes_to_opt_(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPWSTR lpValueBuf,
    _Inout_opt_ LPDWORD ldwTotsize
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegQueryMultipleValuesW,
                                    hKey,
                                    val_list,
                                    num_vals,
                                    lpValueBuf,
                                    ldwTotsize);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegQueryValueExA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _When_(lpData == NULL, _Out_opt_) _When_(lpData != NULL, _Inout_opt_) LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegQueryValueExA,
                                    hKey,
                                    lpValueName,
                                    lpReserved,
                                    lpType,
                                    lpData,
                                    lpcbData);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegQueryValueExW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _When_(lpData == NULL, _Out_opt_) _When_(lpData != NULL, _Inout_opt_) LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegQueryValueExW,
                                    hKey,
                                    lpValueName,
                                    lpReserved,
                                    lpType,
                                    lpData,
                                    lpcbData);
}

#pragma prefast(push)
#pragma prefast(disable : 6054) // String 'pvData' might not be zero-terminated.
LSTATUS
APIENTRY
Hook_Advapi32_RegGetValueA(
    _In_ HKEY hkey,
    _In_opt_ LPCSTR lpSubKey,
    _In_opt_ LPCSTR lpValue,
    _In_ DWORD dwFlags,
    _Out_opt_ LPDWORD pdwType,
    _When_((dwFlags & 0x7F) == RRF_RT_REG_SZ ||
               (dwFlags & 0x7F) == RRF_RT_REG_EXPAND_SZ ||
               (dwFlags & 0x7F) == (RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ) ||
               *pdwType == REG_SZ ||
               *pdwType == REG_EXPAND_SZ, _Post_z_)
        _When_((dwFlags & 0x7F) == RRF_RT_REG_MULTI_SZ ||
               *pdwType == REG_MULTI_SZ, _Post_ _NullNull_terminated_)
    _Out_writes_bytes_to_opt_(*pcbData,*pcbData) PVOID pvData,
    _Inout_opt_ LPDWORD pcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegGetValueA,
                                    hkey,
                                    lpSubKey,
                                    lpValue,
                                    dwFlags,
                                    pdwType,
                                    pvData,
                                    pcbData);
}
#pragma prefast(pop)

#pragma prefast(push)
#pragma prefast(disable : 6054) // String 'pvData' might not be zero-terminated.
LSTATUS
APIENTRY
Hook_Advapi32_RegGetValueW(
    _In_ HKEY hkey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_opt_ LPCWSTR lpValue,
    _In_ DWORD dwFlags,
    _Out_opt_ LPDWORD pdwType,
    _When_((dwFlags & 0x7F) == RRF_RT_REG_SZ ||
               (dwFlags & 0x7F) == RRF_RT_REG_EXPAND_SZ ||
               (dwFlags & 0x7F) == (RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ) ||
               *pdwType == REG_SZ ||
               *pdwType == REG_EXPAND_SZ, _Post_z_)
        _When_((dwFlags & 0x7F) == RRF_RT_REG_MULTI_SZ ||
               *pdwType == REG_MULTI_SZ, _Post_ _NullNull_terminated_)
    _Out_writes_bytes_to_opt_(*pcbData,*pcbData) PVOID pvData,
    _Inout_opt_ LPDWORD pcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegGetValueW,
                                    hkey,
                                    lpSubKey,
                                    lpValue,
                                    dwFlags,
                                    pdwType,
                                    pvData,
                                    pcbData);
}
#pragma prefast(pop)

LSTATUS
APIENTRY
Hook_Advapi32_RegEnumKeyA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(cchName) LPSTR lpName,
    _In_ DWORD cchName
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegEnumKeyA,
                                    hKey,
                                    dwIndex,
                                    lpName,
                                    cchName);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegEnumKeyW(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(cchName) LPWSTR lpName,
    _In_ DWORD cchName
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegEnumKeyW,
                                    hKey,
                                    dwIndex,
                                    lpName,
                                    cchName);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegEnumKeyExA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPSTR lpName,
    _Inout_ LPDWORD lpcchName,
    _Reserved_ LPDWORD lpReserved,
    _Out_writes_to_opt_(*lpcchClass,*lpcchClass + 1) LPSTR lpClass,
    _Inout_opt_ LPDWORD lpcchClass,
    _Out_opt_ PFILETIME lpftLastWriteTime
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegEnumKeyExA,
                                    hKey,
                                    dwIndex,
                                    lpName,
                                    lpcchName,
                                    lpReserved,
                                    lpClass,
                                    lpcchClass,
                                    lpftLastWriteTime);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegEnumKeyExW(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPWSTR lpName,
    _Inout_ LPDWORD lpcchName,
    _Reserved_ LPDWORD lpReserved,
    _Out_writes_to_opt_(*lpcchClass,*lpcchClass + 1) LPWSTR lpClass,
    _Inout_opt_ LPDWORD lpcchClass,
    _Out_opt_ PFILETIME lpftLastWriteTime
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegEnumKeyExW,
                                    hKey,
                                    dwIndex,
                                    lpName,
                                    lpcchName,
                                    lpReserved,
                                    lpClass,
                                    lpcchClass,
                                    lpftLastWriteTime);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegEnumValueA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
    _Inout_ LPDWORD lpcchValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegEnumValueA,
                                    hKey,
                                    dwIndex,
                                    lpValueName,
                                    lpcchValueName,
                                    lpReserved,
                                    lpType,
                                    lpData,
                                    lpcbData);
}

LSTATUS
APIENTRY
Hook_Advapi32_RegEnumValueW(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPWSTR lpValueName,
    _Inout_ LPDWORD lpcchValueName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
    )
{
    return VFDYNF_LINK_COMMON_HOOK2(Advapi32,
                                    KernelBase,
                                    RegEnumValueW,
                                    hKey,
                                    dwIndex,
                                    lpValueName,
                                    lpcchValueName,
                                    lpReserved,
                                    lpType,
                                    lpData,
                                    lpcbData);
}
