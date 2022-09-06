#include "RwSearchOpCode.h"
#include <ntimage.h>
//字节转换成字符串
BOOLEAN ByteToChar(const PUCHAR source, PUCHAR dest, ULONG sourceLen)
{
	if (!MmIsAddressValid(source) && !MmIsAddressValid(source + sourceLen))
	{
		DbgPrint("地址有误");
		return FALSE;
	}
	UCHAR highByte = 0;
	UCHAR lowByte = 0;
	for (ULONG i = 0; i < sourceLen; i++)
	{
		//分割字节，高低位
		highByte = source[i] >> 4;
		lowByte = source[i] & 0x0f;
		//处理高位
		highByte += 0x30;
		if (highByte > 0x39)
			dest[i * 2] = highByte + 0x07;
		else
			dest[i * 2] = highByte;
		//处理低位
		lowByte += 0x30;
		if (lowByte > 0x39)
			dest[i * 2 + 1] = lowByte + 0x07;
		else
			dest[i * 2 + 1] = lowByte;
	}
	return TRUE;
}

//搜索特征码
ULONG64 SearchOpCode(ULONG64 BeginAddr, ULONG64 EndAddr, PUCHAR opCode, ULONG codeSize)
{
	ULONG64 opCodePosition = 0;
	PUCHAR destBuff = ExAllocatePool(NonPagedPool, codeSize);
	 //特征码是双数
	if (destBuff == NULL || codeSize % 2 != 0)
	{
		DbgPrint("SearchOpCode错误");
		return 0;
	}
	if (BeginAddr == 0 || EndAddr == 0)
	{
		DbgPrint("BeginAddr错误");
		return 0;
	}
	ULONG64 CurrentPos = BeginAddr;
	ULONG transferSize = codeSize / 2;
	for (ULONG64 i = BeginAddr; i < EndAddr - codeSize; i++)
	{
		//********* 注意：Mark每次复制为TRUE *********
		BOOLEAN Mark = TRUE;
		RtlZeroMemory(destBuff, codeSize);
		if (!ByteToChar(CurrentPos, destBuff, transferSize))
		{
			return 0;
		}
		for (size_t j = 0; j < codeSize; j++)
		{
			if (opCode[j] == '?'|| destBuff[j] == opCode[j])
			{
				continue;
			}
			else
			{
				Mark = FALSE;
				break;
			}
		}
		if (Mark == TRUE)
		{
			opCodePosition = CurrentPos;
			break;
		}
		CurrentPos++;
	}
	ExFreePool(destBuff);
	return opCodePosition;
}

//搜索特征码 对外的方法
ULONG64 RwSearchOpCode(PCHAR opCode,int offset)
{
	ULONG codeSize = strlen(opCode);
	ULONG64 opCodePosition = 0;
	ULONG64 moduleSize = 0;
	ULONG64 BeginAddr = 0;
	ULONG64 EndAddr = 0;
	BeginAddr = RwGetModuleSuper("ntoskrnl.exe", &moduleSize);
	if (BeginAddr == 0)
	{
		return 0;
	}

	// 看PAGE段是否有要找的特征码
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)BeginAddr;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(BeginAddr + pDosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER pSectionForPAGE = IMAGE_FIRST_SECTION(pNtHeader);

	for (size_t i = 0; i < pNtHeader->FileHeader.NumberOfSections; i++)
	{
		char name[9] = { 0 };
		memcpy(name, pSectionForPAGE->Name, 8);
		if (strcmp(name,"PAGE")==0)
		{
			BeginAddr = BeginAddr + pSectionForPAGE->VirtualAddress;
			EndAddr = BeginAddr + pSectionForPAGE->Misc.VirtualSize;
			break;
		}
		pSectionForPAGE++;
	}
	opCodePosition = SearchOpCode(BeginAddr, EndAddr, opCode, codeSize);
	if (opCodePosition != 0)
	{
		return opCodePosition + offset;
	}

	// ********.text段搜索特征码出错了，前面没事，搜着搜着就蓝了，可能是win10某个函数不可读 *******
	// 	  可以尝试MDL读取
	/*PIMAGE_SECTION_HEADER pSectionForTEXT = IMAGE_FIRST_SECTION(pNtHeader);
	for (size_t i = 0; i < pNtHeader->FileHeader.NumberOfSections; i++)
	{
		char name[9] = { 0 };
		memcpy(name, pSectionForTEXT->Name, 8);
		if (strcmp(name, ".text") == 0)
		{
			BeginAddr = BeginAddr + pSectionForTEXT->VirtualAddress;
			EndAddr = BeginAddr + pSectionForTEXT->Misc.VirtualSize;
			break;
		}
		pSectionForTEXT++;
	}
	opCodePosition = SearchOpCode(BeginAddr, EndAddr, opCode, codeSize);
	if (opCodePosition != 0)
	{
		return opCodePosition + offset;
	}*/
	
	return 0;
}