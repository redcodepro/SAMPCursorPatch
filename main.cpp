#include "main.h"

HWND &hwnd = *reinterpret_cast<HWND*>(0xC97C1C);
int &screen_x = *reinterpret_cast<int*>(0xC9C040);
int &screen_y = *reinterpret_cast<int*>(0xC9C044);

DWORD dwSAMPAddr			= 0;
DWORD dwSAMPInputOffset		= 0;
DWORD dwSAMPDialogOffset	= 0;
DWORD aSuperSecretAddress	= 0; // CLocalPlayer::EnterVehicleAsPassenger()
DWORD dwSAMPDialogDrawFunc	= 0;

inline stInputInfo* GetInput()
{
	return *reinterpret_cast<stInputInfo**>(dwSAMPAddr + dwSAMPInputOffset);
}

inline stDialogInfo* GetDialog()
{
	return *reinterpret_cast<stDialogInfo**>(dwSAMPAddr + dwSAMPDialogOffset);
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

bool getGameCursorPos(LPPOINT lpPoint)
{
	POINT temp;
	if (GetCursorPos(&temp) && ScreenToClient(hwnd, &temp))
	{
		lpPoint->x = temp.x;
		lpPoint->y = temp.y;
		return true;
	}
	return false;
}

bool isMouseHovered(LPPOINT lpPoint, int x, int y, int w, int h)
{
	if (lpPoint->x > x && lpPoint->x < x + w && lpPoint->y > y && lpPoint->y < y + h)
		return true;
	return false;
}

typedef void(__fastcall* hookedDialogDraw_t)(stDialogInfo*, DWORD);
hookedDialogDraw_t orig_DialogDraw;
void __fastcall hooked_DialogDraw(stDialogInfo* _this, DWORD edx)
{
	static bool move = false;
	static int offset[2] = { 0, 0 };
	static bool key_downed = false;

	bool now_downed = (GetKeyState(VK_LBUTTON) & 0x8000);
	bool key_pressed = now_downed && !key_downed;
	//bool key_released = !now_downed && key_downed;

	key_downed = now_downed;

	if (_this->iIsActive)
	{
		POINT curPos;
		getGameCursorPos(&curPos);
		
		if (move)
		{
			if (key_downed)
			{
				int pos_x = curPos.x - offset[0];
				int pos_y = curPos.y - offset[1];

				if (pos_x + _this->pDialog->m_width < 5)
					pos_x = 5 - _this->pDialog->m_width;

				if (pos_y + _this->pDialog->m_nCaptionHeight < 5)
					pos_y = 5 - _this->pDialog->m_nCaptionHeight;

				if (pos_x > (screen_x - 5))
					pos_x = (screen_x - 5);

				if (pos_y > (screen_y - 5))
					pos_y = (screen_y - 5);

				_this->iTextPoxX = pos_x;
				_this->iTextPoxY = pos_y;
				_this->pDialog->m_x = pos_x;
				_this->pDialog->m_y = pos_y;
			}
			else move = false;
		}
		else
		{
			if (key_pressed && isMouseHovered(&curPos, _this->pDialog->m_x, _this->pDialog->m_y,_this->pDialog->m_width, _this->pDialog->m_nCaptionHeight))
			{
				offset[0] = curPos.x - _this->pDialog->m_x;
				offset[1] = curPos.y - _this->pDialog->m_y;
				move = true;
			}
		}
	}
	return orig_DialogDraw(_this, edx);
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
					dwSAMPDialogDrawFunc = dwSAMPAddr + 0x6B240;

					SAMP_HOOKENTER_CURSOR_MODE0	= dwSAMPAddr + 0x9BEA1;
					SAMP_HOOKEXIT_CURSOR_MODE0	= dwSAMPAddr + 0x9BEA6;

					SAMP_HOOKENTER_CURSOR_MODE2	= dwSAMPAddr + 0x9BD3F;
					SAMP_HOOKEXIT_CURSOR_MODE2	= dwSAMPAddr + 0x9BD94;

					dwSAMPInputOffset = 0x21A0E8;
					dwSAMPDialogOffset = 0x21A0B8;

					aSuperSecretAddress = dwSAMPAddr + 0x6D90;
				}
				break;
			case 0x3195DD: // R2
				{
					//dwSAMPDialogDrawFunc = dwSAMPAddr + 0x6BA70;

					SAMP_HOOKENTER_CURSOR_MODE0	= dwSAMPAddr + 0x9BF41;
					SAMP_HOOKEXIT_CURSOR_MODE0	= dwSAMPAddr + 0x9BF46;

					SAMP_HOOKENTER_CURSOR_MODE2	= dwSAMPAddr + 0x9BDDF;
					SAMP_HOOKEXIT_CURSOR_MODE2	= dwSAMPAddr + 0x9BE34;

					dwSAMPInputOffset = 0x21A0F0;
					dwSAMPDialogOffset = 0x21A0C0;

					aSuperSecretAddress = dwSAMPAddr + 0x6D60;
				}
				break;
			case 0xCC4D0: // R3
				{
					dwSAMPDialogDrawFunc = dwSAMPAddr + 0x6F140;

					SAMP_HOOKENTER_CURSOR_MODE0	= dwSAMPAddr + 0xA0151;
					SAMP_HOOKEXIT_CURSOR_MODE0	= dwSAMPAddr + 0xA0156;

					SAMP_HOOKENTER_CURSOR_MODE2	= dwSAMPAddr + 0x9FFEF;
					SAMP_HOOKEXIT_CURSOR_MODE2	= dwSAMPAddr + 0xA0044;

					dwSAMPInputOffset = 0x26E8CC;
					dwSAMPDialogOffset = 0x26E898;

					aSuperSecretAddress = dwSAMPAddr + 0x6DA0;
				}
				break;
			case 0xCBCB0: // R4
				{
					dwSAMPDialogDrawFunc = dwSAMPAddr + 0x6F860;

					SAMP_HOOKENTER_CURSOR_MODE0	= dwSAMPAddr + 0xA0891;
					SAMP_HOOKEXIT_CURSOR_MODE0	= dwSAMPAddr + 0xA0896;

					SAMP_HOOKENTER_CURSOR_MODE2	= dwSAMPAddr + 0xA072F;
					SAMP_HOOKEXIT_CURSOR_MODE2	= dwSAMPAddr + 0xA0784;

					dwSAMPInputOffset = 0x26E9FC;
					dwSAMPDialogOffset = 0x26E9C8;

					aSuperSecretAddress = dwSAMPAddr + 0x6FD0;
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

			if (dwSAMPDialogDrawFunc)
			{
				MH_CreateHookEx((void*)dwSAMPDialogDrawFunc, &hooked_DialogDraw, &orig_DialogDraw);
			}
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			MH_RemoveHook((void*)SAMP_HOOKENTER_CURSOR_MODE0);
			MH_RemoveHook((void*)SAMP_HOOKENTER_CURSOR_MODE2);

			MH_RemoveHook((void*)dwSAMPDialogDrawFunc);

			MH_Uninitialize();
		}
		break;
	}
	return true;
}
