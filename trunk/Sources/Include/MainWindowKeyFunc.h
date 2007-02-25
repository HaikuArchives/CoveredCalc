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
	@file		MainWindowKeyFunc.h
	@brief		Definition of functions used in main window.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.25 created
*/

#ifndef _MAINWINDOWKEYFUNC_H_
#define _MAINWINDOWKEYFUNC_H_

namespace MainWindowKeyFunc
{
	enum KeyFunc
	{
		KeyFunc_None		= -1,
		KeyFunc_0			= 0,
		KeyFunc_1,
		KeyFunc_2,
		KeyFunc_3,
		KeyFunc_4,
		KeyFunc_5,
		KeyFunc_6,
		KeyFunc_7,
		KeyFunc_8,
		KeyFunc_9,
		KeyFunc_A,
		KeyFunc_B,
		KeyFunc_C,
		KeyFunc_D,
		KeyFunc_E,
		KeyFunc_F,
		KeyFunc_Point,
		KeyFunc_Clear,
		KeyFunc_BS,
		KeyFunc_Equal,
		KeyFunc_Plus,
		KeyFunc_Minus,
		KeyFunc_Times,
		KeyFunc_Div,
		KeyFunc_Negate,
		KeyFunc_Hex,
		KeyFunc_Dec,
		KeyFunc_Oct,
		KeyFunc_Bin,
		KeyFunc_Minimize,
		KeyFunc_Close,
	};
};

#endif // _MAINWINDOWKEYFUNC_H_
