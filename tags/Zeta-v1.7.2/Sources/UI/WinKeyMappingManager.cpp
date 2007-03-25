/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2007 CoveredCalc Project Contributors
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*!
	@file		WinKeyMappingManager.cpp
	@brief		WinKeyMappingManager クラスの実装
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.24 created
*/

#include "Prefix.h"
#include "WinKeyMappingManager.h"
#include "MainWindowKeyFunc.h"		// TODO: キーカスタマイズが実現したときには、ここでは必要なくなるはず。

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinKeyMappingManager::WinKeyMappingManager()
{
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinKeyMappingManager::~WinKeyMappingManager()
{
}

// ---------------------------------------------------------------------
//! 指定したキーボードパラメータに対応する機能を取得します。
/*!
	@return 対応する機能番号
	@note
		今はまだ決めうちのキーしか使えない。
		キーカスタマイズが実現したらこのメソッドはなくなる。（このクラスがなくなる）
*/
// ---------------------------------------------------------------------
SInt32 WinMainWindowKeyMappingManager::GetFunction(
	const KeyEventParameter& parameter		//!< キーボードパラメータ
)
{
#define	HANDLE_KEY(_virtKeyCode, _modifierMask, _keyFunc)								\
	if (_virtKeyCode == virtualKeyCode && parameter.GetModifierMask() == _modifierMask)	\
	{																					\
		return _keyFunc;																\
	}

	DWORD virtualKeyCode = parameter.GetVirtualKeyCode();

	HANDLE_KEY(0x30 /* 0 */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_0)
	HANDLE_KEY(VK_NUMPAD0,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_0)
	HANDLE_KEY(0x31 /* 1 */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_1)
	HANDLE_KEY(VK_NUMPAD1,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_1)
	HANDLE_KEY(0x32 /* 2 */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_2)
	HANDLE_KEY(VK_NUMPAD2,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_2)
	HANDLE_KEY(0x33 /* 3 */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_3)
	HANDLE_KEY(VK_NUMPAD3,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_3)
	HANDLE_KEY(0x34 /* 4 */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_4)
	HANDLE_KEY(VK_NUMPAD4,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_4)
	HANDLE_KEY(0x35 /* 5 */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_5)
	HANDLE_KEY(VK_NUMPAD5,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_5)
	HANDLE_KEY(0x36 /* 6 */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_6)
	HANDLE_KEY(VK_NUMPAD6,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_6)
	HANDLE_KEY(0x37 /* 7 */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_7)
	HANDLE_KEY(VK_NUMPAD7,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_7)
	HANDLE_KEY(0x38 /* 8 */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_8)
	HANDLE_KEY(VK_NUMPAD8,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_8)
	HANDLE_KEY(0x39 /* 9 */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_9)
	HANDLE_KEY(VK_NUMPAD9,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_9)
	HANDLE_KEY(0x41 /* A */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_A)
	HANDLE_KEY(0x42 /* B */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_B)
	HANDLE_KEY(0x43 /* C */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_C)
	HANDLE_KEY(0x44 /* D */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_D)
	HANDLE_KEY(0x45 /* E */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_E)
	HANDLE_KEY(0x46 /* F */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_F)
	HANDLE_KEY(0xBE /* . */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Point)
	HANDLE_KEY(VK_DECIMAL,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Point)
	HANDLE_KEY(VK_ESCAPE,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Clear)
	HANDLE_KEY(VK_BACK,			WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_BS)
	HANDLE_KEY(0xBD /* - */,	WinKeyEventParameter::ModifierMask_Shift,	MainWindowKeyFunc::KeyFunc_Equal)
	HANDLE_KEY(VK_RETURN,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Equal)
	HANDLE_KEY(0xBB /* ; */,	WinKeyEventParameter::ModifierMask_Shift,	MainWindowKeyFunc::KeyFunc_Plus)
	HANDLE_KEY(VK_ADD,			WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Plus)
	HANDLE_KEY(0xBD /* - */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Minus)
	HANDLE_KEY(VK_SUBTRACT,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Minus)
	HANDLE_KEY(0xBA /* : */,	WinKeyEventParameter::ModifierMask_Shift,	MainWindowKeyFunc::KeyFunc_Times)
	HANDLE_KEY(VK_MULTIPLY,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Times)
	HANDLE_KEY(0xBF /* / */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Div)
	HANDLE_KEY(VK_DIVIDE,		WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Div)
	HANDLE_KEY(0x4E /* N */,	WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Negate)
	HANDLE_KEY(VK_F9,			WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Negate)	// windows standard calc compatible
	HANDLE_KEY(VK_SUBTRACT,		WinKeyEventParameter::ModifierMask_Shift,	MainWindowKeyFunc::KeyFunc_Negate)
	HANDLE_KEY(VK_F5,			WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Hex)
	HANDLE_KEY(VK_F6,			WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Dec)
	HANDLE_KEY(VK_F7,			WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Oct)
	HANDLE_KEY(VK_F8,			WinKeyEventParameter::ModifierMask_None,	MainWindowKeyFunc::KeyFunc_Bin)

	return MainWindowKeyFunc::KeyFunc_None;

#undef HANDKE_KEY
}

