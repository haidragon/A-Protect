//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         禁止回写文件
//**************************************************************************
#include "file.h"

NTSTATUS __stdcall NewZwWriteFile(
	__in      HANDLE FileHandle,
	__in_opt  HANDLE Event,
	__in_opt  PIO_APC_ROUTINE ApcRoutine,
	__in_opt  PVOID ApcContext,
	__out     PIO_STATUS_BLOCK IoStatusBlock,
	__in      PVOID Buffer,
	__in      ULONG Length,
	__in_opt  PLARGE_INTEGER ByteOffset,
	__in_opt  PULONG Key
	)
{
	ZWWRITEFILE OldZwWriteFile;

	if (!bIsInitSuccess)
		goto _FunctionRet;

	if (RPsGetCurrentProcess)
	{
		if (RPsGetCurrentProcess() == ProtectEProcess)
		{
			goto _FunctionRet;
		}
	}
	if (bDisWriteFile == FALSE)
	{
		return STATUS_UNSUCCESSFUL;
	}
_FunctionRet:

	if (MmIsAddressValid(OriginalServiceDescriptorTable->ServiceTable[ZwWriteFileIndex]))
	{
		OldZwWriteFile = OriginalServiceDescriptorTable->ServiceTable[ZwWriteFileIndex];
	}else
		OldZwWriteFile = KeServiceDescriptorTable->ServiceTable[ZwWriteFileIndex];

	return OldZwWriteFile(
		FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		Buffer,
		Length,
		ByteOffset,
		Key
		);
}
BOOL InitWriteFile()
{
		UNICODE_STRING UnicdeFunction;

		//RtlInitUnicodeString(&UnicdeFunction,L"ZwWriteFile");
		if (SystemCallEntryTableHook(
			"ZwWriteFile",
			&ZwWriteFileIndex,
			NewZwWriteFile) == TRUE)
		{
			if (DebugOn)
				KdPrint(("Create Control Thread success 4\r\n"));
		}
}