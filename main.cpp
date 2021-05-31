#include "main.h"

CSAMP *SAMP = new CSAMP();

int &screen_x = *reinterpret_cast<int*>(0xC9C040);
int &screen_y = *reinterpret_cast<int*>(0xC9C044);

inline bool getGameCursorPos(LPPOINT lpPoint)
{
	return (GetCursorPos(lpPoint) && ScreenToClient(GetActiveWindow(), lpPoint));
}

inline bool isMouseHovered(LPPOINT lpPoint, int x, int y, int w, int h)
{
	return (lpPoint->x > x && lpPoint->x < x + w && lpPoint->y > y && lpPoint->y < y + h);
}

void(__cdecl* orig_CGame__Proccess)() = nullptr;
void __cdecl hooked_CGame__Proccess()
{
	static bool inited = false;
	if (!inited)
	{
		if (!SAMP->Init())
			return orig_CGame__Proccess();

		if (SAMP->getVersion() != SAMP_UNKNOWN)
		{
			InstallCursorPatch(SAMP->getAddress(), SAMP->getVersion());
		}
		else
		{
			MessageBox(NULL, "Unknown SA-MP version.", "SAMPCursorPatch.asi", MB_OK | MB_ICONERROR);
		}

		inited = true;
	}
	
	if (SAMP->getVersion() != SAMP_UNKNOWN)
	{
		static bool move = false;
		static int offset[2] = { 0, 0 };
		static bool key_downed = false;

		bool now_downed = (GetKeyState(VK_LBUTTON) & 0x8000);
		bool key_pressed = now_downed && !key_downed;

		key_downed = now_downed;

		if (SAMP->getDialog()->m_bIsActive)
		{
			POINT curPos;
			getGameCursorPos(&curPos);

			stDXUTDialog* pDXUTDialog = SAMP->getDialog()->m_pDialog;

			if (move)
			{
				if (key_downed)
				{
					int pos_x = curPos.x - offset[0];
					int pos_y = curPos.y - offset[1];

					if (pos_x + pDXUTDialog->m_size[0] < 5)
						pos_x = 5 - pDXUTDialog->m_size[0];

					if (pos_y + pDXUTDialog->m_nCaptionHeight < 5)
						pos_y = 5 - pDXUTDialog->m_nCaptionHeight;

					if (pos_x > (screen_x - 5))
						pos_x = (screen_x - 5);

					if (pos_y > (screen_y - 5))
						pos_y = (screen_y - 5);

					SAMP->getDialog()->m_position[0] = pos_x;
					SAMP->getDialog()->m_position[1] = pos_y;
					pDXUTDialog->m_pos[0] = pos_x;
					pDXUTDialog->m_pos[1] = pos_y;
				}
				else move = false;
			}
			else
			{
				if (key_pressed && isMouseHovered(&curPos, pDXUTDialog->m_pos[0], pDXUTDialog->m_pos[1], pDXUTDialog->m_size[0], pDXUTDialog->m_nCaptionHeight))
				{
					offset[0] = curPos.x - pDXUTDialog->m_pos[0];
					offset[1] = curPos.y - pDXUTDialog->m_pos[1];
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

			if (SAMP->getVersion() != SAMP_UNKNOWN)
			{
				RemoveCursorPatch();
			}

			MH_Uninitialize();
		}
		break;
	}
	return true;
}
