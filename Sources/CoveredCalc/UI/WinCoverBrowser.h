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
	@file		WinCoverBrowser.h
	@brief		WinCoverBrowser クラスの定義
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.10 created
*/

#ifndef _WINCOVERBROWSER_H_
#define _WINCOVERBROWSER_H_

#include "WinDialog.h"
#include "CoverBrowser.h"

// ---------------------------------------------------------------------
//! カバーブラウザ for Windows
// ---------------------------------------------------------------------
class WinCoverBrowser : public WinDialog, public CoverBrowser
{
public:
									WinCoverBrowser();
	virtual							~WinCoverBrowser();

	virtual	void					GetUIRect(Rect32& rect) const;

protected:
	virtual void					clearListUI();
	virtual void					setDataToListUI();
	virtual const CoverListItem*	getSelectedItem();

	virtual DWORD					getDialogStyle();
	virtual LRESULT					wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void							createControls();

private:
	LRESULT							onInitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT							onEnable(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT							onActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT							onCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT							onNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT							onDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT							onClose(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	LRESULT							onCommandReload(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT							onCommandApply(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT							onCommandClose(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	LRESULT							onCoverListNotifyDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HICON							smallIcon;		///< カバーブラウザに表示するアイコン (小)
	HICON							largeIcon;		///< カバーブラウザに表示するアイコン (大)
};

#endif // _WINCOVERBROWSER_H_