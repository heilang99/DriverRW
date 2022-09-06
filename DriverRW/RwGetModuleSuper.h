#pragma once
#include <ntifs.h>
//联合体
typedef enum _SYSTEM_INFORMATION_CLASS     //    Q S  
{
    SystemBasicInformation,                // 00 Y N  
    SystemProcessorInformation,            // 01 Y N  
    SystemPerformanceInformation,          // 02 Y N  
    SystemTimeOfDayInformation,            // 03 Y N  
    SystemNotImplemented1,                 // 04 Y N  
    SystemProcessesAndThreadsInformation,  // 05 Y N  
    SystemCallCounts,                      // 06 Y N  
    SystemConfigurationInformation,        // 07 Y N  
    SystemProcessorTimes,                  // 08 Y N  
    SystemGlobalFlag,                      // 09 Y Y  
    SystemNotImplemented2,                 // 10 Y N  
    SystemModuleInformation,               // 11 Y N  
    SystemLockInformation,                 // 12 Y N  
    SystemNotImplemented3,                 // 13 Y N  
    SystemNotImplemented4,                 // 14 Y N  
    SystemNotImplemented5,                 // 15 Y N  
    SystemHandleInformation,               // 16 Y N  
    SystemObjectInformation,               // 17 Y N  
    SystemPagefileInformation,             // 18 Y N  
    SystemInstructionEmulationCounts,      // 19 Y N  
    SystemInvalidInfoClass1,               // 20  
    SystemCacheInformation,                // 21 Y Y  
    SystemPoolTagInformation,              // 22 Y N  
    SystemProcessorStatistics,             // 23 Y N  
    SystemDpcInformation,                  // 24 Y Y  
    SystemNotImplemented6,                 // 25 Y N  
    SystemLoadImage,                       // 26 N Y  
    SystemUnloadImage,                     // 27 N Y  
    SystemTimeAdjustment,                  // 28 Y Y  
    SystemNotImplemented7,                 // 29 Y N  
    SystemNotImplemented8,                 // 30 Y N  
    SystemNotImplemented9,                 // 31 Y N  
    SystemCrashDumpInformation,            // 32 Y N  
    SystemExceptionInformation,            // 33 Y N  
    SystemCrashDumpStateInformation,       // 34 Y Y/N  
    SystemKernelDebuggerInformation,       // 35 Y N  
    SystemContextSwitchInformation,        // 36 Y N  
    SystemRegistryQuotaInformation,        // 37 Y Y  
    SystemLoadAndCallImage,                // 38 N Y  
    SystemPrioritySeparation,              // 39 N Y  
    SystemNotImplemented10,                // 40 Y N  
    SystemNotImplemented11,                // 41 Y N  
    SystemInvalidInfoClass2,               // 42  
    SystemInvalidInfoClass3,               // 43  
    SystemTimeZoneInformation,             // 44 Y N  
    SystemLookasideInformation,            // 45 Y N  
    SystemSetTimeSlipEvent,                // 46 N Y  
    SystemCreateSession,                   // 47 N Y  
    SystemDeleteSession,                   // 48 N Y  
    SystemInvalidInfoClass4,               // 49  
    SystemRangeStartInformation,           // 50 Y N  
    SystemVerifierInformation,             // 51 Y Y  
    SystemAddVerifier,                     // 52 N Y  
    SystemSessionProcessesInformation      // 53 Y N  
} SYSTEM_INFORMATION_CLASS;

//每个模块的详细信息
typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
    HANDLE Section;
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR FullPathName[256]; //XX/XX/XX.SYS 
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

//查询的大结构体
typedef struct _RTL_PROCESS_MODULES
{
    ULONG64 NumberofSections;                   //有多少个模块
    RTL_PROCESS_MODULE_INFORMATION Modules[1];  //结构体数组
}RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

//未文档化函数
NTKERNELAPI  NTSTATUS ZwQuerySystemInformation(
    ULONG64 SystemInformationClass, PVOID SystemInformation, ULONG64 SystemInformationLength, PULONG64 ReturnLength);

//函数声明 获取内核模块地址
ULONG64 RwGetModuleSuper(PCHAR szModuleName, OUT PULONG64 ModuleSize);