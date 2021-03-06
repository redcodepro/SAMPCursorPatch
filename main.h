#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <string>

#include "minhook/MinHook.h"
#pragma comment(lib, "minhook/libMinHook.x86.lib")

#include "game_sa/CPad.h"

#pragma pack(push, 1)

// DXUT
struct stDXUTEditBox
{
	void *pUnknown;
	bool m_bVisible;                // Shown/hidden flag
	bool m_bMouseOver;              // Mouse pointer is above control
	bool m_bHasFocus;               // Control has input focus

	// ...
	// blah blah blah
};

// SAMP
typedef void(__cdecl *CMDPROC)(PCHAR);
struct stInputInfo
{
	void									*pD3DDevice;
	void									*pDXUTDialog;
	struct stDXUTEditBox					*pDXUTEditBox;
	CMDPROC									pCMDs[144];
	char									szCMDNames[144][33];
	int										iCMDCount;
	int										iInputEnabled;
	char									szInputBuffer[129];
	char									szRecallBufffer[10][129];
	char									szCurrentBuffer[129];
	int										iCurrentRecall;
	int										iTotalRecalls;
	CMDPROC									pszDefaultCMD;
};

struct stDialogInfo
{
	void									*pVTBL;
	int										iTextPoxX;
	int										iTextPoxY;
	int										iTextSizeX;
	int										iTextSizeY;
	int										iBtnOffsetX;
	int										iBtnOffsetY;
	struct stDXUTDialog						*pDialog;
	struct stDXUTListBox					*pList;
	struct stDXUTEditBox					*pEditBox;
	int										iIsActive;
	int										iType;
	uint32_t								DialogID;
	char									*pText;
	uint32_t								font;
	uint32_t								font2;
	char									szCaption[64];
	uint8_t									byteUnknown;
	int										bServerside;
};

#pragma pack(pop)

#endif