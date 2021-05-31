#ifndef __MAIN_H__
#define __MAIN_H__

#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <d3d9.h>
#include <dinput.h>

#include "minhook/MinHook.h"
#pragma comment(lib, "minhook/libMinHook.x86.lib")

#include "game_sa/CPad.h"

#include "samp.h"
#include "stuff.h"

#include "ci_cursorpatch.h"

extern CSAMP *SAMP;

#endif