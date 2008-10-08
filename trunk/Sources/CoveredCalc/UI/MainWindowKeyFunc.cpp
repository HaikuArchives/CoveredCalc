/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2008 CoveredCalc Project Contributors
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
	@file		MainWindowKeyFunc.cpp
	@brief		Implementation of MainWindowKeyFunc class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.10.13 created
*/

#include "Prefix.h"
#include "MainWindowKeyFunc.h"
#include "UTF8String.h"
#include "UTF8Utils.h"

static const UTF8Char STR_FUNC_0[]			= "0";
static const UTF8Char STR_FUNC_1[]			= "1";
static const UTF8Char STR_FUNC_2[]			= "2";
static const UTF8Char STR_FUNC_3[]			= "3";
static const UTF8Char STR_FUNC_4[]			= "4";
static const UTF8Char STR_FUNC_5[]			= "5";
static const UTF8Char STR_FUNC_6[]			= "6";
static const UTF8Char STR_FUNC_7[]			= "7";
static const UTF8Char STR_FUNC_8[]			= "8";
static const UTF8Char STR_FUNC_9[]			= "9";
static const UTF8Char STR_FUNC_A[]			= "A";
static const UTF8Char STR_FUNC_B[]			= "B";
static const UTF8Char STR_FUNC_C[]			= "C";
static const UTF8Char STR_FUNC_D[]			= "D";
static const UTF8Char STR_FUNC_E[]			= "E";
static const UTF8Char STR_FUNC_F[]			= "F";
static const UTF8Char STR_FUNC_POINT[]		= "Point";
static const UTF8Char STR_FUNC_CLEAR[]		= "Clear";
static const UTF8Char STR_FUNC_BS[]			= "BS";
static const UTF8Char STR_FUNC_EQUAL[]		= "Equal";
static const UTF8Char STR_FUNC_PLUS[]		= "Plus";
static const UTF8Char STR_FUNC_MINUS[]		= "Minus";
static const UTF8Char STR_FUNC_TIMES[]		= "Times";
static const UTF8Char STR_FUNC_DIV[]		= "Div";
static const UTF8Char STR_FUNC_NEGATE[]		= "Negate";
static const UTF8Char STR_FUNC_HEX[]		= "Hex";
static const UTF8Char STR_FUNC_DEC[]		= "Dec";
static const UTF8Char STR_FUNC_OCT[]		= "Oct";
static const UTF8Char STR_FUNC_BIN[]		= "Bin";
static const UTF8Char STR_FUNC_MINIMIZE[]	= "Minimize";
static const UTF8Char STR_FUNC_CLOSE[]		= "Close";
static const UTF8Char STR_FUNC_COVER_BROWSER[] = "CoverBrowser";
static const UTF8Char STR_FUNC_ALWAYS_ON_TOP[] = "AlwaysOnTop";
static const UTF8Char STR_FUNC_LOCK_POS[] = "LockPosition";
static const UTF8Char STR_FUNC_PREFERENCES[] = "Preferences";
static const UTF8Char STR_FUNC_CURRENT_COVER[] = "AboutCurrentCover";
static const UTF8Char STR_FUNC_ABOUT[]		= "About";

/**
 *	@brief	Converts a function name string to a keyfunc ID.
 *	@param[in]	function name
 *	@return		keyfunc ID.
 */
SInt32 MainWindowKeyFunc::FuncNameToKeyFunc(ConstUTF8Str funcName) const
{
#define FK_DEF(fn, kf)									\
	if (0 == UTF8Utils::UTF8StrCmp(funcName, fn))		\
	{													\
		return kf;										\
	}
	
	FK_DEF(STR_FUNC_0,				KeyFunc_0)
	FK_DEF(STR_FUNC_1,				KeyFunc_1)
	FK_DEF(STR_FUNC_2,				KeyFunc_2)
	FK_DEF(STR_FUNC_3,				KeyFunc_3)
	FK_DEF(STR_FUNC_4,				KeyFunc_4)
	FK_DEF(STR_FUNC_5,				KeyFunc_5)
	FK_DEF(STR_FUNC_6,				KeyFunc_6)
	FK_DEF(STR_FUNC_7,				KeyFunc_7)
	FK_DEF(STR_FUNC_8,				KeyFunc_8)
	FK_DEF(STR_FUNC_9,				KeyFunc_9)
	FK_DEF(STR_FUNC_A,				KeyFunc_A)
	FK_DEF(STR_FUNC_B,				KeyFunc_B)
	FK_DEF(STR_FUNC_C,				KeyFunc_C)
	FK_DEF(STR_FUNC_D,				KeyFunc_D)
	FK_DEF(STR_FUNC_E,				KeyFunc_E)
	FK_DEF(STR_FUNC_F,				KeyFunc_F)
	FK_DEF(STR_FUNC_POINT,			KeyFunc_Point)
	FK_DEF(STR_FUNC_CLEAR,			KeyFunc_Clear)
	FK_DEF(STR_FUNC_BS,				KeyFunc_BS)
	FK_DEF(STR_FUNC_EQUAL,			KeyFunc_Equal)
	FK_DEF(STR_FUNC_PLUS,			KeyFunc_Plus)
	FK_DEF(STR_FUNC_MINUS,			KeyFunc_Minus)
	FK_DEF(STR_FUNC_TIMES,			KeyFunc_Times)
	FK_DEF(STR_FUNC_DIV,			KeyFunc_Div)
	FK_DEF(STR_FUNC_NEGATE,			KeyFunc_Negate)
	FK_DEF(STR_FUNC_HEX,			KeyFunc_Hex)
	FK_DEF(STR_FUNC_DEC,			KeyFunc_Dec)
	FK_DEF(STR_FUNC_OCT,			KeyFunc_Oct)
	FK_DEF(STR_FUNC_BIN,			KeyFunc_Bin)
	FK_DEF(STR_FUNC_MINIMIZE,		KeyFunc_Minimize)
	FK_DEF(STR_FUNC_CLOSE,			KeyFunc_Close)
	FK_DEF(STR_FUNC_COVER_BROWSER,	KeyFunc_ShowHideCoverBrowser)
	FK_DEF(STR_FUNC_ALWAYS_ON_TOP,	KeyFunc_ToggleAlwaysOnTop)
	FK_DEF(STR_FUNC_LOCK_POS,		KeyFunc_ToggleLockPos)
	FK_DEF(STR_FUNC_PREFERENCES,	KeyFunc_ShowPreferencesDialog)
	FK_DEF(STR_FUNC_CURRENT_COVER,	KeyFunc_ShowCurrentCoverInfo)
	FK_DEF(STR_FUNC_ABOUT,			KeyFunc_About)
	
	return KeyFunc_None;
	
#undef FK_DEF
}

/**
 *	@brief	Converts a keyfunc ID to a function name string.
 *	@param[in]	keyFunc		keyfunc ID.
 *	@param[out]	funcName	function name is returned.
 */
void MainWindowKeyFunc::KeyFuncToFuncName(SInt32 keyFunc, UTF8String& funcName) const
{
#define KF_DEF(kf, fn)		\
	if (kf == keyFunc)		\
	{						\
		funcName = fn;		\
		return;				\
	}

	KF_DEF(KeyFunc_0,						STR_FUNC_0)
	KF_DEF(KeyFunc_1,						STR_FUNC_1)
	KF_DEF(KeyFunc_2,						STR_FUNC_2)
	KF_DEF(KeyFunc_3,						STR_FUNC_3)
	KF_DEF(KeyFunc_4,						STR_FUNC_4)
	KF_DEF(KeyFunc_5,						STR_FUNC_5)
	KF_DEF(KeyFunc_6,						STR_FUNC_6)
	KF_DEF(KeyFunc_7,						STR_FUNC_7)
	KF_DEF(KeyFunc_8,						STR_FUNC_8)
	KF_DEF(KeyFunc_9,						STR_FUNC_9)
	KF_DEF(KeyFunc_A,						STR_FUNC_A)
	KF_DEF(KeyFunc_B,						STR_FUNC_B)
	KF_DEF(KeyFunc_C,						STR_FUNC_C)
	KF_DEF(KeyFunc_D,						STR_FUNC_D)
	KF_DEF(KeyFunc_E,						STR_FUNC_E)
	KF_DEF(KeyFunc_F,						STR_FUNC_F)
	KF_DEF(KeyFunc_Point,					STR_FUNC_POINT)
	KF_DEF(KeyFunc_Clear,					STR_FUNC_CLEAR)
	KF_DEF(KeyFunc_BS,						STR_FUNC_BS)
	KF_DEF(KeyFunc_Equal,					STR_FUNC_EQUAL)
	KF_DEF(KeyFunc_Plus,					STR_FUNC_PLUS)
	KF_DEF(KeyFunc_Minus,					STR_FUNC_MINUS)
	KF_DEF(KeyFunc_Times,					STR_FUNC_TIMES)
	KF_DEF(KeyFunc_Div,						STR_FUNC_DIV)
	KF_DEF(KeyFunc_Negate,					STR_FUNC_NEGATE)
	KF_DEF(KeyFunc_Hex,						STR_FUNC_HEX)
	KF_DEF(KeyFunc_Dec,						STR_FUNC_DEC)
	KF_DEF(KeyFunc_Oct,						STR_FUNC_OCT)
	KF_DEF(KeyFunc_Bin,						STR_FUNC_BIN)
	KF_DEF(KeyFunc_Minimize,				STR_FUNC_MINIMIZE)
	KF_DEF(KeyFunc_Close,					STR_FUNC_CLOSE)
	KF_DEF(KeyFunc_ShowHideCoverBrowser,	STR_FUNC_COVER_BROWSER)
	KF_DEF(KeyFunc_ToggleAlwaysOnTop,		STR_FUNC_ALWAYS_ON_TOP)
	KF_DEF(KeyFunc_ToggleLockPos,			STR_FUNC_LOCK_POS)
	KF_DEF(KeyFunc_ShowPreferencesDialog,	STR_FUNC_PREFERENCES)
	KF_DEF(KeyFunc_ShowCurrentCoverInfo,	STR_FUNC_CURRENT_COVER)
	KF_DEF(KeyFunc_About,					STR_FUNC_ABOUT)

	funcName.Empty();

#undef KF_DEF
}
