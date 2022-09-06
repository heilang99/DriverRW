#pragma once
#include <ntifs.h>
//================ 全局变量 ======================
extern ULONG64 OldDisQueryAttr;
extern ULONG64 OldDisSetAttr;

//================ 约定通信结构体 ======================
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

//读数据通信 结构体
typedef struct _MMREAD
{
    ULONG ProcessId;
    PVOID readAddress;
    ULONG readSize;
    PVOID readBuff;
}MMREAD,*PMMREAD;

//写数据通信 结构体
typedef struct _MMWRITE
{
    ULONG64 ProcessId;
    PVOID writeAddress;
    ULONG64 writeSize;
    PVOID targetWriteAddr;
}MMWRITE, * PMMWRITE;

//内存信息通信 结构体
typedef struct _MMTYPE {
    ULONG64 ProcessId;
    PVOID checkAddress;
    PVOID MMInfoBuff;
} MMTYPE, * PMMTYPE;

//进程保护通信 结构体
typedef struct _INPROTECT {
    HANDLE pid;
} INPROTECT, * PINPROTECT;

//线程劫持通信 结构体
typedef struct _INJECTTHREAD {
    HANDLE pid;
    ULONG Argc;
    ULONG Arg[4];
} INJECTTHREAD, * PINJECTTHREAD;


//================ 注册回调函数 ======================
typedef NTSTATUS(*_AttributeInformationCallback)(HANDLE handle, PVOID arg1);

typedef struct _RWCALL_BACK_FUN
{
    _AttributeInformationCallback ExpDisQueryAttributeInformation;
    _AttributeInformationCallback ExpDisSetAttributeInformation;
}RWCALL_BACK_FUNC, * PRWCALL_BACK_FUN;

typedef NTSTATUS(*_ExRegisterAttributeInformationCallback)(PRWCALL_BACK_FUN backFun);

//================ 函数声明 ======================
NTSTATUS RegisterCallBack();                                        //注册回调 
NTSTATUS RegisterCallBackWin7();                                    //注册回调 WIN7
NTSTATUS RegisterCallBackWin10();                                   //注册回调 WIN10
NTSTATUS MyQueryAttributeInformation(HANDLE handle, PVOID arg1);    //回调函数 WIN7
NTSTATUS MySetAttributeInformation(HANDLE handle, PVOID arg1);      //回调函数 WIN7

NTSTATUS CallbackFuncWin10(PVOID package, PVOID arg2, PVOID arg3);  //回调函数 WIN10

NTSTATUS DisPatchCallEntry(PPACKAGE package);                       //分发函数
void UnloadCallBack();                                              //卸载注册
void UnloadCallBackWin7();                                          //卸载注册 WIN7
void UnloadCallBackWin10();                                         //卸载注册 WIN10