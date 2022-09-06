#include <Windows.h>
#include <iostream>
_declspec (naked) void func1()
{
	__asm
	{
		push ebp
		mov ebp,esp
		sub esp,0x30

	}
	printf("线程劫持了\n");
	MessageBox(0, L"线程劫持了", L"提示", MB_OK);
	__asm
	{
		mov esp, ebp
		pop ebp
		// NtDelayExecution函数的堆栈平衡，内平栈
		add esp,0x4
		retn 0x8
	}
}
int main()
{
	int index = 0;
	printf("func1:%x \n", (DWORD)func1);
	while (true)
	{
		printf("程序执行第几次:%d\n", index);
		index++;
		Sleep(2000);
	}
	return 0;
}