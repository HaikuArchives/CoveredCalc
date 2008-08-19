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
	@file		WinMessageBoxProvider.h
	@brief		Definition of WinMessageBoxProvider class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.12 created
*/

#ifndef _WINMESSAGEBOXPROVIDER_H_
#define _WINMESSAGEBOXPROVIDER_H_

#include "MessageBoxProvider.h"

// ---------------------------------------------------------------------
//! This class provides alert message box for Windows.
// ---------------------------------------------------------------------
class WinMessageBoxProvider : public MessageBoxProvider
{
public:
						WinMessageBoxProvider(HWND hWnd);
	virtual				~WinMessageBoxProvider();

	virtual Button		DoMessageBox(ConstAStr message, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);
	virtual Button		DoMessageBox(SInt32 messageId, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);

private:
	HWND				hWnd;			// handle of owner window
};

#endif // _WINMESSAGEBOXPROVIDER_H_