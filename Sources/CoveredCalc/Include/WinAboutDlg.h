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
	@file		WinAboutDlg.h
	@brief		Definition of WinAboutDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.20 created
*/

#ifndef _WINABOUTDLG_H_
#define _WINABOUTDLG_H_

#include "WinDialog.h"

// ---------------------------------------------------------------------
//! バージョン情報ダイアログ for Windows
// ---------------------------------------------------------------------
class WinAboutDlg : public WinDialog
{
public:
						WinAboutDlg();
	virtual				~WinAboutDlg();

protected:
	virtual LRESULT		wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LRESULT				onInitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT				onDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT				onEraseBkGnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT				onCtlColorStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void				makeVersionString(MBCString& versionString);

private:
	HBRUSH				backBrush;	///< 背景ブラシ
	HBITMAP				iconImage;	///< アイコン画像
};

#endif // _WINABOUTDLG_H_
