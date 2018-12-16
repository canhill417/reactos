/* Copyright (c) Mark Harmstone 2016-17
 *
 * This file is part of WinBtrfs.
 *
 * WinBtrfs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public Licence as published by
 * the Free Software Foundation, either version 3 of the Licence, or
 * (at your option) any later version.
 *
 * WinBtrfs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 *
 * You should have received a copy of the GNU Lesser General Public Licence
 * along with WinBtrfs.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#define ISOLATION_AWARE_ENABLED 1
#define STRSAFE_NO_DEPRECATE

#ifndef __REACTOS__
#define WINVER 0x0A00 // Windows 10
#define _WIN32_WINNT 0x0A00
#endif

#ifndef __REACTOS__
#include <windows.h>
#include <winternl.h>
#else
#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#include <strsafe.h>
#include <ndk/iofuncs.h>
#include <ndk/obfuncs.h>
#endif
#include <string>
#include <vector>
#include <stdint.h>
#ifndef __REACTOS__
#include "../btrfs.h"
#include "../btrfsioctl.h"
#else
#include "btrfs.h"
#include "btrfsioctl.h"
#endif

using namespace std;

#ifndef __REACTOS__
#define STATUS_SUCCESS                  (NTSTATUS)0x00000000
#define STATUS_BUFFER_OVERFLOW          (NTSTATUS)0x80000005
#define STATUS_END_OF_FILE              (NTSTATUS)0xc0000011
#define STATUS_MORE_PROCESSING_REQUIRED (NTSTATUS)0xc0000016
#define STATUS_BUFFER_TOO_SMALL         (NTSTATUS)0xc0000023
#define STATUS_DEVICE_NOT_READY         (NTSTATUS)0xc00000a3
#define STATUS_CANNOT_DELETE            (NTSTATUS)0xc0000121
#define STATUS_NOT_FOUND                (NTSTATUS)0xc0000225
#endif

#define BLOCK_FLAG_DATA         0x001
#define BLOCK_FLAG_SYSTEM       0x002
#define BLOCK_FLAG_METADATA     0x004
#define BLOCK_FLAG_RAID0        0x008
#define BLOCK_FLAG_RAID1        0x010
#define BLOCK_FLAG_DUPLICATE    0x020
#define BLOCK_FLAG_RAID10       0x040
#define BLOCK_FLAG_RAID5        0x080
#define BLOCK_FLAG_RAID6        0x100

#define BTRFS_TYPE_FILE      1
#define BTRFS_TYPE_DIRECTORY 2
#define BTRFS_TYPE_CHARDEV   3
#define BTRFS_TYPE_BLOCKDEV  4
#define BTRFS_TYPE_FIFO      5
#define BTRFS_TYPE_SOCKET    6
#define BTRFS_TYPE_SYMLINK   7

#ifdef _MSC_VER
#define funcname __FUNCTION__
#else
#define funcname __func__
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4800)
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifndef __REACTOS__
NTSYSCALLAPI NTSTATUS NTAPI NtFsControlFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
                                            PIO_STATUS_BLOCK IoStatusBlock, ULONG FsControlCode, PVOID InputBuffer, ULONG InputBufferLength,
                                            PVOID OutputBuffer, ULONG OutputBufferLength);

NTSTATUS NTAPI NtReadFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer,
                          ULONG Length, PLARGE_INTEGER ByteOffset, PULONG Key);
#endif

NTSTATUS WINAPI RtlUTF8ToUnicodeN(PWSTR UnicodeStringDestination, ULONG UnicodeStringMaxWCharCount,
                                  PULONG UnicodeStringActualWCharCount, PCCH UTF8StringSource,
                                  ULONG UTF8StringByteCount);

NTSTATUS NTAPI RtlUnicodeToUTF8N(PCHAR UTF8StringDestination, ULONG UTF8StringMaxByteCount,
                                  PULONG UTF8StringActualByteCount, PCWCH UnicodeStringSource,
                                  ULONG UnicodeStringByteCount);

#ifndef __REACTOS__
NTSTATUS WINAPI NtSetEaFile(HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length);

NTSTATUS WINAPI NtSetSecurityObject(HANDLE Handle, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor);

NTSTATUS NTAPI NtQueryInformationFile(HANDLE hFile, PIO_STATUS_BLOCK io, PVOID ptr, ULONG len, FILE_INFORMATION_CLASS FileInformationClass);
#ifdef __cplusplus
}
#endif
#else
BOOL
WINAPI
SetFileInformationByHandle(HANDLE hFile, FILE_INFO_BY_HANDLE_CLASS FileInformationClass, LPVOID lpFileInformation, DWORD dwBufferSize);
BOOL
WINAPI
GetFileInformationByHandleEx(HANDLE hFile, FILE_INFO_BY_HANDLE_CLASS FileInformationClass, LPVOID lpFileInformation, DWORD dwBufferSize);
#ifdef __cplusplus
}
#endif
#endif

#ifndef __REACTOS__
typedef struct _REPARSE_DATA_BUFFER {
    ULONG ReparseTag;
    USHORT ReparseDataLength;
    USHORT Reserved;

    union {
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            ULONG Flags;
            WCHAR PathBuffer[1];
        } SymbolicLinkReparseBuffer;

        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            WCHAR PathBuffer[1];
        } MountPointReparseBuffer;

        struct {
            UCHAR DataBuffer[1];
        } GenericReparseBuffer;
    };
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;

#define SYMLINK_FLAG_RELATIVE 1
#endif

#ifndef FILE_SUPPORTS_BLOCK_REFCOUNTING

typedef struct _DUPLICATE_EXTENTS_DATA {
    HANDLE FileHandle;
    LARGE_INTEGER SourceFileOffset;
    LARGE_INTEGER TargetFileOffset;
    LARGE_INTEGER ByteCount;
} DUPLICATE_EXTENTS_DATA, *PDUPLICATE_EXTENTS_DATA;

#define FSCTL_DUPLICATE_EXTENTS_TO_FILE CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 209, METHOD_BUFFERED, FILE_WRITE_ACCESS)

typedef struct _FSCTL_GET_INTEGRITY_INFORMATION_BUFFER {
    WORD ChecksumAlgorithm;
    WORD Reserved;
    DWORD Flags;
    DWORD ChecksumChunkSizeInBytes;
    DWORD ClusterSizeInBytes;
} FSCTL_GET_INTEGRITY_INFORMATION_BUFFER, *PFSCTL_GET_INTEGRITY_INFORMATION_BUFFER;

typedef struct _FSCTL_SET_INTEGRITY_INFORMATION_BUFFER {
    WORD ChecksumAlgorithm;
    WORD Reserved;
    DWORD Flags;
} FSCTL_SET_INTEGRITY_INFORMATION_BUFFER, *PFSCTL_SET_INTEGRITY_INFORMATION_BUFFER;

#define FSCTL_GET_INTEGRITY_INFORMATION CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 159, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_SET_INTEGRITY_INFORMATION CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 160, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)

#endif

class win_handle {
public:
    win_handle() {
    }

    win_handle(HANDLE nh) {
        h = nh;
    }

    ~win_handle() {
        if (h != INVALID_HANDLE_VALUE)
            CloseHandle(h);
    }

    operator HANDLE() const {
        return h;
    }

    win_handle& operator=(const HANDLE nh) {
        if (h != INVALID_HANDLE_VALUE)
            CloseHandle(h);

        h = nh;

        return *this;
    }

    HANDLE* operator&() {
        return &h;
    }

private:
    HANDLE h = INVALID_HANDLE_VALUE;
};

class fff_handle {
public:
    fff_handle() {
    }

    fff_handle(HANDLE nh) {
        h = nh;
    }

    ~fff_handle() {
        if (h != INVALID_HANDLE_VALUE)
            FindClose(h);
    }

    operator HANDLE() const {
        return h;
    }

    fff_handle& operator=(const HANDLE nh) {
        if (h != INVALID_HANDLE_VALUE)
            FindClose(h);

        h = nh;

        return *this;
    }

    HANDLE* operator&() {
        return &h;
    }

private:
    HANDLE h = INVALID_HANDLE_VALUE;
};

class nt_handle {
public:
    nt_handle() {
    }

    nt_handle(HANDLE nh) {
        h = nh;
    }

    ~nt_handle() {
        if (h != INVALID_HANDLE_VALUE)
            NtClose(h);
    }

    operator HANDLE() const {
        return h;
    }

    nt_handle& operator=(const HANDLE nh) {
        if (h != INVALID_HANDLE_VALUE)
            NtClose(h);

        h = nh;

        return *this;
    }

    HANDLE* operator&() {
        return &h;
    }

private:
    HANDLE h = INVALID_HANDLE_VALUE;
};

class string_error : public exception {
public:
    string_error(int resno, ...);

    const char* what() const noexcept {
        return msg.c_str();
    }

private:
    string msg;
};


class last_error : public exception {
public:
    last_error(DWORD errnum);

    const char* what() const noexcept {
        return msg.c_str();
    }

private:
    string msg;
};

class ntstatus_error : public exception {
public:
    ntstatus_error(NTSTATUS Status);

    const char* what() const noexcept {
        return msg.c_str();
    }

private:
    string msg;
};

#ifdef __REACTOS__
inline wstring to_wstring(uint8_t a) { WCHAR buffer[16]; swprintf(buffer, L"%d", a); return wstring(buffer); } 
inline wstring to_wstring(uint16_t a) { WCHAR buffer[16]; swprintf(buffer, L"%d", a); return wstring(buffer); }
inline wstring to_wstring(uint32_t a) { WCHAR buffer[32]; swprintf(buffer, L"%ld", a); return wstring(buffer); }
inline wstring to_wstring(uint64_t a) { WCHAR buffer[64]; swprintf(buffer, L"%I64d", a); return wstring(buffer); }
#endif

extern HMODULE module;
void format_size(uint64_t size, wstring& s, bool show_bytes);
void set_dpi_aware();
wstring format_message(ULONG last_error);
wstring format_ntstatus(NTSTATUS Status);
bool load_string(HMODULE module, UINT id, wstring& s);
void wstring_sprintf(wstring& s, wstring fmt, ...);
void command_line_to_args(LPWSTR cmdline, vector<wstring> args);
void utf8_to_utf16(const string& utf8, wstring& utf16);
void utf16_to_utf8(const wstring& utf16, string& utf8);
void error_message(HWND hwnd, const char* msg);
