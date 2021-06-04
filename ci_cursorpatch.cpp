#include "main.h"

DWORD aSuperSecretAddress = 0; // CLocalPlayer::EnterVehicleAsPassenger()

DWORD SAMP_HOOKENTER_CURSOR_MODE0 = 0;
DWORD SAMP_HOOKEXIT_CURSOR_MODE0 = 0;
DWORD SAMP_HOOKENTER_CURSOR_MODE2 = 0;
DWORD SAMP_HOOKEXIT_CURSOR_MODE2 = 0;

int CURSOR_MODE = 0;

inline bool isUnlockAvailable()
{
	return (SAMP->getInput()->m_bEnabled || SAMP->getDialog()->m_bIsActive || SAMP->getTDSelection()->m_bIsActive);
}

inline bool isDialogInputEnabled()
{
	return (SAMP->getDialog()->m_bIsActive && SAMP->getDialog()->m_pEditbox->m_bHasFocus && (SAMP->getDialog()->m_nType == DIALOG_INPUT || SAMP->getDialog()->m_nType == DIALOG_PASSWORD));
}

__declspec(naked) void hook_curmode_0(void)
{
	__asm pushad;

	CURSOR_MODE = 0;

	CPad::GetPad(0)->bDisablePlayerEnterCar = 0;

	__memset(0x6194A0, 0xE9, 1, true);
	__memset(aSuperSecretAddress, 0x53, 1);
	__memcpy(0x541DF5, "\xE8\x46\xF3\xFE\xFF", 5, true);

	__asm popad;

	__asm
	{
		mov ecx, [ebp + 0x0C]
		xor eax, eax

		jmp SAMP_HOOKEXIT_CURSOR_MODE0;
	}
}

__declspec(naked) void hook_curmode_2(void)
{
	__asm pushad;

	CURSOR_MODE = 2;

	CPad::GetPad(0)->bDisablePlayerEnterCar = 1;

	__memset(0x6194A0, 0xC3, 1, true);
	__memset(aSuperSecretAddress, 0xC3, 1);

	if (SAMP->getInput()->m_bEnabled || isDialogInputEnabled()/* || !isUnlockAvailable()*/)
	{
		__memset(0x541DF5, 0x90, 5, true);
	}
	else
	{
		__memcpy(0x541DF5, "\xE8\x46\xF3\xFE\xFF", 5, true);
	}

	__asm popad;

	__asm jmp SAMP_HOOKEXIT_CURSOR_MODE2;
}

HRESULT(__cdecl* orig_CPad__getMouseState)(DIMOUSESTATE2 *dm) = nullptr;
HRESULT __cdecl hooked_CPad__getMouseState(DIMOUSESTATE2 *dm)
{
	static int lock_state = 0;
	static DIMOUSESTATE2 mouse_state;

	HRESULT hr = orig_CPad__getMouseState(dm);

	if (CURSOR_MODE)
		lock_state = 3;
	else
		if (lock_state > 0)
			lock_state--;

	if (lock_state)
	{
		memcpy(&mouse_state, dm, sizeof(DIMOUSESTATE2));

		if (dm->rgbButtons[1]/* && isUnlockAvailable()*/)
			ZeroMemory(dm->rgbButtons, sizeof(dm->rgbButtons));
		else
			ZeroMemory(dm, sizeof(DIMOUSESTATE2));
	}
	else
	{
		for (size_t i = 0; i < 5; ++i)
			if (mouse_state.rgbButtons[i])
				if (dm->rgbButtons[i])
					dm->rgbButtons[i] = 0;
				else
					mouse_state.rgbButtons[i] = 0;
	}

	return hr;
}

void InstallCursorPatch(DWORD dwSAMPAddr, int ver)
{
	DWORD dwSAMPCursorMode0Enter[]	= { 0, 0x09BEA1, 0x09BF41, 0x0A0151, 0x0A0891, 0x0A08C1 };
	DWORD dwSAMPCursorMode2Enter[]	= { 0, 0x09BD3F, 0x09BDDF, 0x09FFEF, 0x0A072F, 0x0A075F };
	DWORD dwSAMPEnterVehicleAddr[]	= { 0, 0x006D90, 0x006D60, 0x006DA0, 0x006FD0, 0x006FE0 };

	if (ver == SAMP_UNKNOWN)
		return;

	SAMP_HOOKENTER_CURSOR_MODE0 = dwSAMPAddr + dwSAMPCursorMode0Enter[ver];
	SAMP_HOOKEXIT_CURSOR_MODE0 = SAMP_HOOKENTER_CURSOR_MODE0 + 0x05;

	SAMP_HOOKENTER_CURSOR_MODE2 = dwSAMPAddr + dwSAMPCursorMode2Enter[ver];
	SAMP_HOOKEXIT_CURSOR_MODE2 = SAMP_HOOKENTER_CURSOR_MODE2 + 0x55;

	aSuperSecretAddress = dwSAMPAddr + dwSAMPEnterVehicleAddr[ver];

	MH_CreateHookA(SAMP_HOOKENTER_CURSOR_MODE0, &hook_curmode_0, (void**)0);
	MH_CreateHookA(SAMP_HOOKENTER_CURSOR_MODE2, &hook_curmode_2, (void**)0);

	MH_CreateHookA(0x746ED0, &hooked_CPad__getMouseState, &orig_CPad__getMouseState);
}

void RemoveCursorPatch()
{
	MH_RemoveHookA(SAMP_HOOKENTER_CURSOR_MODE0);
	MH_RemoveHookA(SAMP_HOOKENTER_CURSOR_MODE2);

	MH_RemoveHookA(0x746ED0);
}