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
	@file		WinMainWindow.h
	@brief		WinMainWindow クラスの定義
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.24 created
*/

#ifndef _WINMAINWINDOW_H_
#define _WINMAINWINDOW_H_

#include "WinSkinWindow.h"
#include "MainUIController.h"

// ---------------------------------------------------------------------
//! メインウィンドウ for Windows
// ---------------------------------------------------------------------
class WinMainWindow : public WinSkinWindow, public MainUIController
{
public:
							WinMainWindow();
	virtual					~WinMainWindow();

	static ATOM				RegisterClass();
	static ConstAStr		GetWindowClassName() { return windowClassName; }

	// MainUIController interface
	virtual void			ShowMainUIContextMenu(Point32 menuPos);
	virtual void			ShowAboutDialog();
	virtual void			ShowAboutCurrentCoverDialog();
	virtual void			ShowPreferencesDialog();

protected:
	virtual UIManager*		createUIManager();
	virtual void			deleteUIManager(UIManager* uiManager);
	virtual SInt32			getMenuCommand(UINT menuID);
	
	virtual LRESULT			wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LRESULT					onCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onClose(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onEnable(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onDisplayChange(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static const AChar		windowClassName[];
};

#endif // _WINMAINWINDOW_H_