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
	printf("�߳̽ٳ���\n");
	MessageBox(0, L"�߳̽ٳ���", L"��ʾ", MB_OK);
	__asm
	{
		mov esp, ebp
		pop ebp
		// NtDelayExecution�����Ķ�ջƽ�⣬��ƽջ
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
		printf("����ִ�еڼ���:%d\n", index);
		index++;
		Sleep(2000);
	}
	return 0;
}