#pragma once
#include <Windows.h>
#include <iostream>
//================ APi => NtQueryInformationFile ======================
typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID    Pointer;
	};
	ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

typedef ULONG (*_NtQueryInformationFile)(
	HANDLE FileHandle,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID  FileInformation, 
	ULONG Length,
	ULONG FileInformationClass
);

//================ Լ��ͨ�Žṹ�� ======================
typedef enum _COMMUNICATE_NUMBER
{
	IsCr3ToCr0 = 0x1234,
	DriverRead
}COMMUNICATE_NUMBER, * PCOMMUNICATE_NUMBER;

typedef struct _PACKAGE
{
	COMMUNICATE_NUMBER communicateID;
	COMMUNICATE_NUMBER communicateFun;
	ULONG64 content;
	ULONG64 length;
	ULONG64 result;
}PACKAGE, * PPACKAGE;

//�����ݽṹ��
typedef struct _MMREAD
{
	ULONG64 ProcessId;
	PVOID readAddress;
	ULONG64 readSize;
	PVOID readBuff;
}MMREAD, * PMMREAD;