/*
    Copyright (c) Johnny Shaw. All rights reserved.
*/

#include <phnt_windows.h>
#include <phnt.h>

#include <pcre2.h>

#include <pcre2_vfdynf.h>

NTSTATUS Pcre2ErrorToNtStatus(
    _In_ int ErrorCode
    )
{
    switch (ErrorCode)
    {
        case PCRE2_ERROR_UTF8_ERR1:
        case PCRE2_ERROR_UTF8_ERR2:
        case PCRE2_ERROR_UTF8_ERR3:
        case PCRE2_ERROR_UTF8_ERR4:
        case PCRE2_ERROR_UTF8_ERR5:
        case PCRE2_ERROR_UTF8_ERR6:
        case PCRE2_ERROR_UTF8_ERR7:
        case PCRE2_ERROR_UTF8_ERR8:
        case PCRE2_ERROR_UTF8_ERR9:
        case PCRE2_ERROR_UTF8_ERR10:
        case PCRE2_ERROR_UTF8_ERR11:
        case PCRE2_ERROR_UTF8_ERR12:
        case PCRE2_ERROR_UTF8_ERR13:
        case PCRE2_ERROR_UTF8_ERR14:
        case PCRE2_ERROR_UTF8_ERR15:
        case PCRE2_ERROR_UTF8_ERR16:
        case PCRE2_ERROR_UTF8_ERR17:
        case PCRE2_ERROR_UTF8_ERR18:
        case PCRE2_ERROR_UTF8_ERR19:
        case PCRE2_ERROR_UTF8_ERR20:
        case PCRE2_ERROR_UTF8_ERR21:
        case PCRE2_ERROR_UTF16_ERR1:
        case PCRE2_ERROR_UTF16_ERR2:
        case PCRE2_ERROR_UTF16_ERR3:
        case PCRE2_ERROR_UTF32_ERR1:
        case PCRE2_ERROR_UTF32_ERR2:
        case PCRE2_ERROR_BADDATA:
        case PCRE2_ERROR_MIXEDTABLES:
        case PCRE2_ERROR_BADMAGIC:
        case PCRE2_ERROR_BADMODE:
        case PCRE2_ERROR_BADOFFSET:
        case PCRE2_ERROR_BADOPTION:
        case PCRE2_ERROR_BADREPLACEMENT:
        case PCRE2_ERROR_BADUTFOFFSET:
        case PCRE2_ERROR_CALLOUT:
        case PCRE2_ERROR_DFA_BADRESTART:
        case PCRE2_ERROR_DFA_RECURSE:
        case PCRE2_ERROR_DFA_UCOND:
        case PCRE2_ERROR_DFA_UFUNC:
        case PCRE2_ERROR_DFA_UITEM:
        case PCRE2_ERROR_DFA_WSSIZE:
        case PCRE2_ERROR_JIT_BADOPTION:
        case PCRE2_ERROR_JIT_STACKLIMIT:
        case PCRE2_ERROR_MATCHLIMIT:
        case PCRE2_ERROR_NOSUBSTRING:
        case PCRE2_ERROR_NOUNIQUESUBSTRING:
        case PCRE2_ERROR_NULL:
        case PCRE2_ERROR_RECURSELOOP:
        case PCRE2_ERROR_DEPTHLIMIT: // PCRE2_ERROR_RECURSIONLIMIT
        case PCRE2_ERROR_UNAVAILABLE:
        case PCRE2_ERROR_UNSET:
        case PCRE2_ERROR_BADOFFSETLIMIT:
        case PCRE2_ERROR_BADREPESCAPE:
        case PCRE2_ERROR_REPMISSINGBRACE:
        case PCRE2_ERROR_BADSUBSTITUTION:
        case PCRE2_ERROR_BADSUBSPATTERN:
        case PCRE2_ERROR_TOOMANYREPLACE:
        case PCRE2_ERROR_BADSERIALIZEDDATA:
        case PCRE2_ERROR_CONVERT_SYNTAX:
        case PCRE2_ERROR_INTERNAL_DUPMATCH:
        case PCRE2_ERROR_DFA_UINVALID_UTF:
            return STATUS_BAD_DATA;
        case PCRE2_ERROR_HEAPLIMIT:
        case PCRE2_ERROR_NOMEMORY:
            return STATUS_INSUFFICIENT_RESOURCES;
        case PCRE2_ERROR_INTERNAL:
            return STATUS_INTERNAL_ERROR;
        case PCRE2_ERROR_NOMATCH:
            return STATUS_NO_MATCH;
        case PCRE2_ERROR_PARTIAL:
            return STATUS_PARTIAL_COPY;
        default:
            return STATUS_UNSUCCESSFUL;
    }
}

VOID Pcre2Close(
    _In_ PCRE2_HANDLE Pcre2Handle
    )
{
    pcre2_code_free((pcre2_code*)Pcre2Handle);
}

_Must_inspect_result_
NTSTATUS Pcre2Compile(
    _Out_ PPCRE2_HANDLE Pcre2Handle,
    _In_ PUNICODE_STRING Pattern
    )
{
    pcre2_code* code;
    int errorCode;
    size_t errorOffset;

    *Pcre2Handle = NULL;

    code = pcre2_compile((PCRE2_SPTR16)Pattern->Buffer,
                         Pattern->Length / sizeof(WCHAR),
                         PCRE2_DOTALL,
                         &errorCode,
                         &errorOffset,
                         NULL);
    if (!code)
    {
        return Pcre2ErrorToNtStatus(errorCode);
    }

    *Pcre2Handle = (PCRE2_HANDLE)code;

    return STATUS_SUCCESS;
}

_Must_inspect_result_
NTSTATUS Pcre2MatchEx(
    _In_ PCRE2_HANDLE Pcre2Handle,
    _In_ PUNICODE_STRING String,
    _Out_ PBOOLEAN Match
    )
{
    pcre2_code* code;
    pcre2_match_data* matchData;
    int offset;

    *Match = FALSE;

    code = (pcre2_code*)Pcre2Handle;

    matchData = pcre2_match_data_create_from_pattern(code, NULL);
    if (!matchData)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    offset = pcre2_match(code,
                         (PCRE2_SPTR16)String->Buffer,
                         String->Length / sizeof(WCHAR),
                         0,
                         0,
                         matchData,
                         NULL);

    pcre2_match_data_free(matchData);

    *Match = (offset >= 0);

    return STATUS_SUCCESS;
}


BOOLEAN Pcre2Match(
    _In_ PCRE2_HANDLE Pcre2Handle,
    _In_ PUNICODE_STRING String
    )
{
    BOOLEAN match;

    if (NT_SUCCESS(Pcre2MatchEx(Pcre2Handle, String, &match)))
    {
        return match;
    }

    return FALSE;
}
