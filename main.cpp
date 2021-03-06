#include "main.h"

DWORD dwSAMPAddr			= 0;
DWORD dwSAMPInputOffset		= 0;
DWORD dwSAMPDialogOffset	= 0;

inline stInputInfo* GetInput()
{
	return *reinterpret_cast<stInputInfo**>(dwSAMPAddr + dwSAMPInputOffset);
}

inline stDialogInfo* GetDialog()
{
	return *reinterpret_cast<stDialogInfo**>(dwSAMPAddr + dwSAMPDialogOffset);
}

void __memset(DWORD dst, int val, size_t size, bool unfuck = true)
{
	if (unfuck)
	{	
		DWORD d;
		if (!VirtualProtect((void*)dst, size, PAGE_EXECUTE_READWRITE, &d))
			return;
	}

	memset((void*)dst, val, size);
}

void __memcpy(DWORD dst, void* src, size_t size, bool unfuck = true)
{
	if (unfuck)
	{
		DWORD d;
		if (!VirtualProtect((void*)dst, size, PAGE_EXECUTE_READWRITE, &d))
			return;
	}

	memcpy((void*)dst, src, size);
}

bool __memcmp(DWORD src0, void* src1, size_t size, bool unprotect = true)
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

DWORD SAMP_HOOKENTER_CURSOR_MODE0 = 0x00000000;
DWORD SAMP_HOOKEXIT_CURSOR_MODE0 = 0x00000000;
__declspec(naked) void hook_curmode_0(void)
{
	__asm pushad;

	CPad::GetPad(0)->bDisablePlayerEnterCar = 0;

	__memcpy(0x540670, "\x66\x83\xB9", 3);

	if (__memcmp(0x747FB6, "\x00", 1))
	{
		__memcpy(0x74542B, "\x50\x51\xFF\x15\x00\x83\x85\x00", 8);
	}

	__asm popad;

	__asm
	{
		// overwritten code
		mov ecx, [ebp + 0x0C]
		xor eax, eax

		// Exit
		jmp SAMP_HOOKEXIT_CURSOR_MODE0;
	}
}

DWORD SAMP_HOOKENTER_CURSOR_MODE2 = 0x00000000;
DWORD SAMP_HOOKEXIT_CURSOR_MODE2 = 0x00000000;
__declspec(naked) void hook_curmode_2(void)
{
	__asm pushad;

	CPad::GetPad(0)->bDisablePlayerEnterCar = 1;

	__memset(0x74542B, 0x90, 8);
	__memcpy(0x540670, "\x32\xC0\xC3", 3);

	static bool lock = false;
	if (GetKeyState(VK_RBUTTON) & 0x8000)
	{
		__memcpy(0x53F417, "\xE8\xB4\x7A\x20\x00", 5);
		__memcpy(0x53F41F, "\x85\xC0\x0F\x8C", 4);
		__memset(0x6194A0, 0xE9, 1);
	}
	else
	{
		if ((GetInput() && GetDialog()) && (GetInput()->iInputEnabled || (GetDialog()->iIsActive && GetDialog()->pEditBox->m_bHasFocus)))
		{
			__memset(0x541DF5, 0x90, 5);
		}
		else
		{
			__memcpy(0x541DF5, "\xE8\x46\xF3\xFE\xFF", 5);
		}
		__memset(0x53F417, 0x90, 5);
		__memcpy(0x53F41F, "\x33\xC0\x0F\x84", 4);
		__memset(0x6194A0, 0xC3, 1);

		lock = true;
	}

	if (lock)
	{
		CPad::NewMouseControllerState.X = 0;
		CPad::NewMouseControllerState.Y = 0;

		CPad::ClearMouseHistory();
		CPad::UpdatePads();

		if (GetKeyState(VK_RBUTTON) & 0x8000)
		{
			CPad::NewMouseControllerState.X = 0;
			CPad::NewMouseControllerState.Y = 0;

			CPad::ClearMouseHistory();

			lock = false;
		}
	}

	__asm popad;

	__asm jmp SAMP_HOOKEXIT_CURSOR_MODE2;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);

			dwSAMPAddr = (DWORD)GetModuleHandle("samp.dll");
			if (dwSAMPAddr == NULL)
			{
				MessageBoxA(NULL, "SA-MP was not loaded.", "SAMPCursorPatch.asi", MB_OK | MB_ICONERROR);
				return false;
			}

			IMAGE_NT_HEADERS *ntheader = reinterpret_cast<IMAGE_NT_HEADERS*>(dwSAMPAddr + reinterpret_cast<IMAGE_DOS_HEADER*>(dwSAMPAddr)->e_lfanew);
			uintptr_t ep = ntheader->OptionalHeader.AddressOfEntryPoint;
			switch (ep)
			{
			case 0x31DF13: // R1
				{
					SAMP_HOOKENTER_CURSOR_MODE0	= dwSAMPAddr + 0x9BEA1;
					SAMP_HOOKEXIT_CURSOR_MODE0	= dwSAMPAddr + 0x9BEA6;

					SAMP_HOOKENTER_CURSOR_MODE2	= dwSAMPAddr + 0x9BD3F;
					SAMP_HOOKEXIT_CURSOR_MODE2	= dwSAMPAddr + 0x9BD94;

					dwSAMPInputOffset = 0x21A0E8;
					dwSAMPDialogOffset = 0x21A0B8;
				}
				break;
			case 0x3195DD: // R2
				{
					SAMP_HOOKENTER_CURSOR_MODE0	= dwSAMPAddr + 0x9BF41;
					SAMP_HOOKEXIT_CURSOR_MODE0	= dwSAMPAddr + 0x9BF46;

					SAMP_HOOKENTER_CURSOR_MODE2	= dwSAMPAddr + 0x9BDDF;
					SAMP_HOOKEXIT_CURSOR_MODE2	= dwSAMPAddr + 0x9BE34;

					dwSAMPInputOffset = 0x21A0F0;
					dwSAMPDialogOffset = 0x21A0C0;
				}
				break;
			case 0xCC4D0: // R3
				{
					SAMP_HOOKENTER_CURSOR_MODE0	= dwSAMPAddr + 0xA0151;
					SAMP_HOOKEXIT_CURSOR_MODE0	= dwSAMPAddr + 0xA0156;

					SAMP_HOOKENTER_CURSOR_MODE2	= dwSAMPAddr + 0x9FFEF;
					SAMP_HOOKEXIT_CURSOR_MODE2	= dwSAMPAddr + 0xA0044;

					dwSAMPInputOffset = 0x26E8CC;
					dwSAMPDialogOffset = 0x26E898;
				}
				break;
			case 0xCBCB0: // R4
				{
					SAMP_HOOKENTER_CURSOR_MODE0	= dwSAMPAddr + 0xA0891;
					SAMP_HOOKEXIT_CURSOR_MODE0	= dwSAMPAddr + 0xA0896;

					SAMP_HOOKENTER_CURSOR_MODE2	= dwSAMPAddr + 0xA072F;
					SAMP_HOOKEXIT_CURSOR_MODE2	= dwSAMPAddr + 0xA0784;

					dwSAMPInputOffset = 0x26E9FC;
					dwSAMPDialogOffset = 0x26E9C8;
				}
				break;
			default:
				{
					MessageBoxA(NULL, "Unknown SA-MP version.", "SAMPCursorPatch.asi", MB_OK | MB_ICONERROR);
					return false;
				}
				break;
			}

			MH_Initialize();

			MH_CreateHookEx((void*)SAMP_HOOKENTER_CURSOR_MODE0, &hook_curmode_0, (void**)0);
			MH_CreateHookEx((void*)SAMP_HOOKENTER_CURSOR_MODE2, &hook_curmode_2, (void**)0);
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			MH_RemoveHook((void*)SAMP_HOOKENTER_CURSOR_MODE0);
			MH_RemoveHook((void*)SAMP_HOOKENTER_CURSOR_MODE2);

			MH_Uninitialize();
		}
		break;
	}
	return true;
}
