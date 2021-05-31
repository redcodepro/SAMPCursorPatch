#include "main.h"

/////                               -  0.3.7 R1  0.3.7 R2  0.3.7 R3  0.3.7 R4  0.3.7 R4-2
DWORD dwSAMPEntryPointAddr[]	= { 0, 0x31DF13, 0x3195DD, 0x0CC4D0, 0x0CBCB0, 0x0CBCD0 };
DWORD dwSAMPInputOffset[]		= { 0, 0x21A0E8, 0x21A0F0, 0x26E8CC, 0x26E9FC, 0x26E9FC };
DWORD dwSAMPDialogOffset[]		= { 0, 0x21A0B8, 0x21A0C0, 0x26E898, 0x26E9C8, 0x26E9C8 };
DWORD dwSAMPTDSelectionOffset[]	= { 0, 0x21A0CC, 0x21A114, 0x26E8B0, 0x26EA24, 0x26EA24 }; // not tested

bool CSAMP::Init()
{
	if ((dwSAMPAddr = (DWORD)GetModuleHandle("samp.dll")) == NULL)
		return false;

	IMAGE_DOS_HEADER* dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(dwSAMPAddr);
	IMAGE_NT_HEADERS* ntHeader = reinterpret_cast<IMAGE_NT_HEADERS*>((uint8_t*)dosHeader + dosHeader->e_lfanew);

	for (size_t i = 0; i < _countof(dwSAMPEntryPointAddr); ++i)
		if (ntHeader->OptionalHeader.AddressOfEntryPoint == dwSAMPEntryPointAddr[i])
		{
			dwSAMPVersion = i;
			break;
		}

	if (dwSAMPVersion == SAMP_UNKNOWN)
		return true;

	m_pInput = *reinterpret_cast<CInput**>(dwSAMPAddr + dwSAMPInputOffset[dwSAMPVersion]);
	if (m_pInput == nullptr)
		return false;

	m_pDialog = *reinterpret_cast<CDialog**>(dwSAMPAddr + dwSAMPDialogOffset[dwSAMPVersion]);
	if (m_pDialog == nullptr)
		return false;

	m_pTDSelection = *reinterpret_cast<CTextDrawSelection**>(dwSAMPAddr + dwSAMPTDSelectionOffset[dwSAMPVersion]);
	if (m_pTDSelection == nullptr)
		return false;

	return true;
}