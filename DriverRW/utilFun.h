#pragma once
#include <ntifs.h>
//================ ȫ�ֱ��� ======================
extern ULONG64 OldDisQueryAttr;
extern ULONG64 OldDisSetAttr;

//================ Լ��ͨ�Žṹ�� ======================
typedef enum _COMMUNICATE_NUMBER
{
    IsCr3ToCr0 = 0x1234,
    DriverRead,
    DriverWrite,
    GetMMType,
    InstallProtect,
    UnstallProtect,
    ModifyHeaderFlag,
    RestoreHeaderFlag,
    InJectThread
}COMMUNICATE_NUMBER, * PCOMMUNICATE_NUMBER;

typedef struct _PACKAGE
{
    COMMUNICATE_NUMBER communicateID;
    COMMUNICATE_NUMBER communicateFun;
    ULONG64 content;
    ULONG64 length;
    ULONG64 result;
}PACKAGE, * PPACKAGE;

//������ͨ�� �ṹ��
typedef struct _MMREAD
{
    ULONG ProcessId;
    PVOID readAddress;
    ULONG readSize;
    PVOID readBuff;
}MMREAD,*PMMREAD;

//д����ͨ�� �ṹ��
typedef struct _MMWRITE
{
    ULONG64 ProcessId;
    PVOID writeAddress;
    ULONG64 writeSize;
    PVOID targetWriteAddr;
}MMWRITE, * PMMWRITE;

//�ڴ���Ϣͨ�� �ṹ��
typedef struct _MMTYPE {
    ULONG64 ProcessId;
    PVOID checkAddress;
    PVOID MMInfoBuff;
} MMTYPE, * PMMTYPE;

//���̱���ͨ�� �ṹ��
typedef struct _INPROTECT {
    HANDLE pid;
} INPROTECT, * PINPROTECT;

//�߳̽ٳ�ͨ�� �ṹ��
typedef struct _INJECTTHREAD {
    HANDLE pid;
    ULONG Argc;
    ULONG Arg[4];
} INJECTTHREAD, * PINJECTTHREAD;


//================ ע��ص����� ======================
typedef NTSTATUS(*_AttributeInformationCallback)(HANDLE handle, PVOID arg1);

typedef struct _RWCALL_BACK_FUN
{
    _AttributeInformationCallback ExpDisQueryAttributeInformation;
    _AttributeInformationCallback ExpDisSetAttributeInformation;
}RWCALL_BACK_FUNC, * PRWCALL_BACK_FUN;

typedef NTSTATUS(*_ExRegisterAttributeInformationCallback)(PRWCALL_BACK_FUN backFun);

//================ �������� ======================
NTSTATUS RegisterCallBack();                                        //ע��ص� 
NTSTATUS RegisterCallBackWin7();                                    //ע��ص� WIN7
NTSTATUS RegisterCallBackWin10();                                   //ע��ص� WIN10
NTSTATUS MyQueryAttributeInformation(HANDLE handle, PVOID arg1);    //�ص����� WIN7
NTSTATUS MySetAttributeInformation(HANDLE handle, PVOID arg1);      //�ص����� WIN7

NTSTATUS CallbackFuncWin10(PVOID package, PVOID arg2, PVOID arg3);  //�ص����� WIN10

NTSTATUS DisPatchCallEntry(PPACKAGE package);                       //�ַ�����
void UnloadCallBack();                                              //ж��ע��
void UnloadCallBackWin7();                                          //ж��ע�� WIN7
void UnloadCallBackWin10();                                         //ж��ע�� WIN10