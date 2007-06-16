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
	@file		BeKeyMappingManager.cpp
	@brief		Implementation of BeKeyMappingManager class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.08.29 created
*/

#include "Prefix.h"
#include "BeKeyMappingManager.h"
#include <be/interface/InterfaceDefs.h>	// TODO: キーカスタマイズが実現したときは、ここでは必要なくなるはず。
#include "MainWindowKeyFunc.h"		// TODO: キーカスタマイズが実現したときは、ここでは必要なくなるはず。

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
BeKeyMappingManager::BeKeyMappingManager()
{
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
BeKeyMappingManager::~BeKeyMappingManager()
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
SInt32 BeMainWindowKeyMappingManager::GetFunction(
	const KeyEventParameter& parameter		//!< キーボードパラメータ
)
{
#define	HANDLE_1BYTE_KEY(_code, _keyFunc)			\
		case _code:									\
			return _keyFunc;						\
			break;

	const char* bytes = parameter.GetBytes();
	int32 numBytes = parameter.GetNumBytes();
	
	if (1 == numBytes)
	{
		switch (bytes[0])
		{
		HANDLE_1BYTE_KEY('0',			MainWindowKeyFunc::KeyFunc_0)
		HANDLE_1BYTE_KEY('1',			MainWindowKeyFunc::KeyFunc_1)
		HANDLE_1BYTE_KEY('2',			MainWindowKeyFunc::KeyFunc_2)
		HANDLE_1BYTE_KEY('3',			MainWindowKeyFunc::KeyFunc_3)
		HANDLE_1BYTE_KEY('4',			MainWindowKeyFunc::KeyFunc_4)
		HANDLE_1BYTE_KEY('5',			MainWindowKeyFunc::KeyFunc_5)
		HANDLE_1BYTE_KEY('6',			MainWindowKeyFunc::KeyFunc_6)
		HANDLE_1BYTE_KEY('7',			MainWindowKeyFunc::KeyFunc_7)
		HANDLE_1BYTE_KEY('8',			MainWindowKeyFunc::KeyFunc_8)
		HANDLE_1BYTE_KEY('9',			MainWindowKeyFunc::KeyFunc_9)
		HANDLE_1BYTE_KEY('A',			MainWindowKeyFunc::KeyFunc_A)
		HANDLE_1BYTE_KEY('a',			MainWindowKeyFunc::KeyFunc_A)
		HANDLE_1BYTE_KEY('B',			MainWindowKeyFunc::KeyFunc_B)
		HANDLE_1BYTE_KEY('b',			MainWindowKeyFunc::KeyFunc_B)
		HANDLE_1BYTE_KEY('C',			MainWindowKeyFunc::KeyFunc_C)
		HANDLE_1BYTE_KEY('c',			MainWindowKeyFunc::KeyFunc_C)
		HANDLE_1BYTE_KEY('D',			MainWindowKeyFunc::KeyFunc_D)
		HANDLE_1BYTE_KEY('d',			MainWindowKeyFunc::KeyFunc_D)
		HANDLE_1BYTE_KEY('E',			MainWindowKeyFunc::KeyFunc_E)
		HANDLE_1BYTE_KEY('e',			MainWindowKeyFunc::KeyFunc_E)
		HANDLE_1BYTE_KEY('F',			MainWindowKeyFunc::KeyFunc_F)
		HANDLE_1BYTE_KEY('f',			MainWindowKeyFunc::KeyFunc_F)
		HANDLE_1BYTE_KEY('.', 			MainWindowKeyFunc::KeyFunc_Point)
		HANDLE_1BYTE_KEY(B_ESCAPE,		MainWindowKeyFunc::KeyFunc_Clear)
		HANDLE_1BYTE_KEY(B_BACKSPACE,	MainWindowKeyFunc::KeyFunc_BS)
		HANDLE_1BYTE_KEY('=',			MainWindowKeyFunc::KeyFunc_Equal)
		HANDLE_1BYTE_KEY(B_ENTER,		MainWindowKeyFunc::KeyFunc_Equal)
		HANDLE_1BYTE_KEY('+',			MainWindowKeyFunc::KeyFunc_Plus)
		HANDLE_1BYTE_KEY('-',			MainWindowKeyFunc::KeyFunc_Minus)
		HANDLE_1BYTE_KEY('*',			MainWindowKeyFunc::KeyFunc_Times)
		HANDLE_1BYTE_KEY('/',			MainWindowKeyFunc::KeyFunc_Div)
		HANDLE_1BYTE_KEY('N',			MainWindowKeyFunc::KeyFunc_Negate)
		HANDLE_1BYTE_KEY('n',			MainWindowKeyFunc::KeyFunc_Negate)
		}
	}

	return MainWindowKeyFunc::KeyFunc_None;

#undef	HANDLE_1BYTE_KEY
}
