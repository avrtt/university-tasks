#include "stdafx.h"
#include "library.h"
#define __cpuid __mycpuidex
#define c_cpuid __mycpuidex

// получение информации о длине строки  ЁЎа и количестве функциональных клавиш 
LIBRARY_API int GetInfo(library_info* info) {

	info -> funcKeysCount = GetKeyboardType(2); // количество функциональных клавиш

	// проверка, поддерживаетс€ ли CPUID
	__asm {
		PUSHFD // размещение регистра EFLAGS в стеке
		POP EAX // извлечение значени€ EFLAGS в EAX
		MOV EBX, EAX // сохранение значени€ в EBX
		XOR EAX, 00200000h // изменение 21-го бита
		PUSH EAX // размещение нового значени€ в стеке
		POPFD // сохранение нового значени€ в EFLAGS
		PUSHFD // снова размещение EFLAGS в стеке
		POP EAX  // значение EFLAGS теперь в EAX
		cmp EAX, EBX // проверка 21-го бита
		JZ RET_ARG // бит не изменилс€ - CPUID не поддерживаетс€
	}

	// получение длины строки  ЁЎа (схема AMD)
	__asm {
		MOV EAX, 0x80000006
		cpuid
		and EDX, 0xFF
		MOV EAX, info
		MOV[EAX], EDX
		JMP RET_ARG
	}

	__asm { RET_ARG: }
}

// точка входа динамической библиотеки
BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_DETACH: // отключение DLL
		break;
	}
	return TRUE;
}