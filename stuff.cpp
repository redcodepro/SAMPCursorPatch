#include "main.h"

void __memset(DWORD dst, int val, size_t size, bool unprotect)
{
	if (unprotect)
	{
		DWORD d;
		if (!VirtualProtect((void*)dst, size, PAGE_EXECUTE_READWRITE, &d))
			return;
	}

	memset((void*)dst, val, size);
}

void __memcpy(DWORD dst, void* src, size_t size, bool unprotect)
{
	if (unprotect)
	{
		DWORD d;
		if (!VirtualProtect((void*)dst, size, PAGE_EXECUTE_READWRITE, &d))
			return;
	}

	memcpy((void*)dst, src, size);
}

bool __memcmp(DWORD src0, void* src1, size_t size, bool unprotect)
{
	DWORD dwOldProtect;
	if (unprotect)
	{
		if (!VirtualProtect((void*)src0, size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
			return false;
	}

	bool result = (memcmp((void*)src0, src1, size) == 0);

	if (unprotect)
	{
		VirtualProtect((void*)src0, size, dwOldProtect, &dwOldProtect);
	}

	return result;
}