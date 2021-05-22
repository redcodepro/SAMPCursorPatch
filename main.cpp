#include "main.h"

int &screen_x = *reinterpret_cast<int*>(0xC9C040);
int &screen_y = *reinterpret_cast<int*>(0xC9C044);

DWORD dwSAMPAddr	= 0;
DWORD dwSAMPVersion	= 0;

DWORD dwSAMPInputOffset[] = { 0, 0x21A0E8, 0x21A0F0, 0x26E8CC, 0x26E9FC };
DWORD dwSAMPDialogOffset[] = { 0, 0x21A0B8, 0x21A0C0, 0x26E898, 0x26E9C8 };

DWORD aSuperSecretAddress	= 0; // CLocalPlayer::EnterVehicleAsPassenger()

inline stInputInfo* GetInput()
{
	return *reinterpret_cast<stInputInfo**>(dwSAMPAddr + dwSAMPInputOffset[dwSAMPVersion]);
}

inline stDialogInfo* GetDialog()
{
	return *reinterpret_cast<stDialogInfo**>(dwSAMPAddr + dwSAMPDialogOffset[dwSAMPVersion]);
}

inline bool getGameCursorPos(LPPOINT lpPoint)
{
	return (GetCursorPos(lpPoint) && ScreenToClient(GetActiveWindow(), lpPoint));
}

inline bool isMouseHovered(LPPOINT lpPoint, int x, int y, int w, int h)
{
	return (lpPoint->x > x && lpPoint->x < x + w && lpPoint->y > y && lpPoint->y < y + h);
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

	__memset(aSuperSecretAddress, 0x53, 1);

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

	__memset(aSuperSecretAddress, 0xC3, 1);

	static bool lock = false;
	if (GetKeyState(VK_RBUTTON) & 0x8000)
	{
		__memcpy(0x53F417, "\xE8\xB4\x7A\x20\x00", 5);
		__memcpy(0x53F41F, "\x85\xC0\x0F\x8C", 4);
		__memset(0x6194A0, 0xE9, 1);
	}
	else
	{
		if (GetInput() && GetDialog() && (GetInput()->iInputEnabled || (GetDialog()->iIsActive && (GetDialog()->iType == DIALOG_STYLE_INPUT || GetDialog()->iType == DIALOG_STYLE_PASSWORD) && GetDialog()->pEditBox->m_bHasFocus)))
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

void (__cdecl* orig_CGame__Proccess)(void) = nullptr;
void __cdecl hooked_CGame__Proccess()
{
	static bool inited = false;
	if (!inited)
	{
		if ((dwSAMPAddr = (DWORD)GetModuleHandle("samp.dll")) == NULL)
			return orig_CGame__Proccess();

		IMAGE_DOS_HEADER* dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(dwSAMPAddr);
		IMAGE_NT_HEADERS* ntHeader = reinterpret_cast<IMAGE_NT_HEADERS*>((uint8_t*)dosHeader + dosHeader->e_lfanew);
		switch (ntHeader->OptionalHeader.AddressOfEntryPoint)
		{
			case 0x31DF13: // R1
				{
					dwSAMPVersion = 1;

					SAMP_HOOKENTER_CURSOR_MODE0 = dwSAMPAddr + 0x9BEA1;
					SAMP_HOOKEXIT_CURSOR_MODE0 = dwSAMPAddr + 0x9BEA6;

					SAMP_HOOKENTER_CURSOR_MODE2 = dwSAMPAddr + 0x9BD3F;
					SAMP_HOOKEXIT_CURSOR_MODE2 = dwSAMPAddr + 0x9BD94;

					aSuperSecretAddress = dwSAMPAddr + 0x6D90;
				}
				break;
			case 0x3195DD: // R2
				{
					dwSAMPVersion = 2;

					SAMP_HOOKENTER_CURSOR_MODE0 = dwSAMPAddr + 0x9BF41;
					SAMP_HOOKEXIT_CURSOR_MODE0 = dwSAMPAddr + 0x9BF46;

					SAMP_HOOKENTER_CURSOR_MODE2 = dwSAMPAddr + 0x9BDDF;
					SAMP_HOOKEXIT_CURSOR_MODE2 = dwSAMPAddr + 0x9BE34;

					aSuperSecretAddress = dwSAMPAddr + 0x6D60;
				}
				break;
			case 0xCC4D0: // R3
				{
					dwSAMPVersion = 3;

					SAMP_HOOKENTER_CURSOR_MODE0 = dwSAMPAddr + 0xA0151;
					SAMP_HOOKEXIT_CURSOR_MODE0 = dwSAMPAddr + 0xA0156;

					SAMP_HOOKENTER_CURSOR_MODE2 = dwSAMPAddr + 0x9FFEF;
					SAMP_HOOKEXIT_CURSOR_MODE2 = dwSAMPAddr + 0xA0044;

					aSuperSecretAddress = dwSAMPAddr + 0x6DA0;
				}
				break;
			case 0xCBCB0: // R4
				{
					dwSAMPVersion = 4;

					SAMP_HOOKENTER_CURSOR_MODE0 = dwSAMPAddr + 0xA0891;
					SAMP_HOOKEXIT_CURSOR_MODE0 = dwSAMPAddr + 0xA0896;

					SAMP_HOOKENTER_CURSOR_MODE2 = dwSAMPAddr + 0xA072F;
					SAMP_HOOKEXIT_CURSOR_MODE2 = dwSAMPAddr + 0xA0784;

					aSuperSecretAddress = dwSAMPAddr + 0x6FD0;
				}
				break;
			default:
				MessageBox(NULL, "Unknown SA-MP version.", "SAMPCursorPatch.asi", MB_OK | MB_ICONERROR);
				break;
		}

		if (dwSAMPVersion)
		{
			MH_CreateHookA(SAMP_HOOKENTER_CURSOR_MODE0, &hook_curmode_0, (void**)0);
			MH_CreateHookA(SAMP_HOOKENTER_CURSOR_MODE2, &hook_curmode_2, (void**)0);
		}

		inited = true;
	}
	
	if (dwSAMPVersion)
	{
		static bool move = false;
		static int offset[2] = { 0, 0 };
		static bool key_downed = false;

		bool now_downed = (GetKeyState(VK_LBUTTON) & 0x8000);
		bool key_pressed = now_downed && !key_downed;

		key_downed = now_downed;

		if (GetDialog() && GetDialog()->iIsActive)
		{
			POINT curPos;
			getGameCursorPos(&curPos);

			if (move)
			{
				if (key_downed)
				{
					int pos_x = curPos.x - offset[0];
					int pos_y = curPos.y - offset[1];

					if (pos_x + GetDialog()->pDialog->m_width < 5)
						pos_x = 5 - GetDialog()->pDialog->m_width;

					if (pos_y + GetDialog()->pDialog->m_nCaptionHeight < 5)
						pos_y = 5 - GetDialog()->pDialog->m_nCaptionHeight;

					if (pos_x > (screen_x - 5))
						pos_x = (screen_x - 5);

					if (pos_y > (screen_y - 5))
						pos_y = (screen_y - 5);

					GetDialog()->iTextPoxX = pos_x;
					GetDialog()->iTextPoxY = pos_y;
					GetDialog()->pDialog->m_x = pos_x;
					GetDialog()->pDialog->m_y = pos_y;
				}
				else move = false;
			}
			else
			{
				if (key_pressed && isMouseHovered(&curPos, GetDialog()->pDialog->m_x, GetDialog()->pDialog->m_y, GetDialog()->pDialog->m_width, GetDialog()->pDialog->m_nCaptionHeight))
				{
					offset[0] = curPos.x - GetDialog()->pDialog->m_x;
					offset[1] = curPos.y - GetDialog()->pDialog->m_y;
					move = true;
				}
			}
		}
	}
	return orig_CGame__Proccess();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);

			MH_Initialize();

			MH_CreateHookA(0x53BEE0, &hooked_CGame__Proccess, &orig_CGame__Proccess);
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			MH_RemoveHookA(0x53BEE0);

			if (dwSAMPVersion)
			{
				MH_RemoveHookA(SAMP_HOOKENTER_CURSOR_MODE0);
				MH_RemoveHookA(SAMP_HOOKENTER_CURSOR_MODE2);
			}

			MH_Uninitialize();
		}
		break;
	}
	return true;
}
