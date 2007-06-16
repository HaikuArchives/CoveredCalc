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
	@file		WinUIMessageProvider.h
	@brief		Definition of WinUIMessageProvider class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.01.12 created
*/

#ifndef _WINUIMESSAGEPROVIDER_H_
#define _WINUIMESSAGEPROVIDER_H_

#include "MBCString.h"

// ---------------------------------------------------------------------
//! This class provides message string for user interface for Windows.
// ---------------------------------------------------------------------
class WinUIMessageProvider
{
public:
						WinUIMessageProvider();
	virtual				~WinUIMessageProvider();

	void				Init();
	
	void				GetMessage(SInt32 messageId, MBCString& message);
	void				GetNFormatMessage(MBCString& message, SInt32 messageId, ...);

	void				GetMessage(HINSTANCE hInstance, SInt32 messageId, MBCString& message);
};

#endif // _WINUIMESSAGEPROVIDER_H_