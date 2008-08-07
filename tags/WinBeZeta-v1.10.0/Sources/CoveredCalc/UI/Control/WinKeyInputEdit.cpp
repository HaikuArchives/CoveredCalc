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
	@file		WinKeyInputEdit.cpp
	@brief		Implementation of WinKeyInputEdit class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.5.13 created
*/

#include "Prefix.h"
#include "WinKeyInputEdit.h"
#include "KeyNameDB.h"
#include "WinCoveredCalcApp.h"

////////////////////////////////////////
#define base	CHrnWnd
////////////////////////////////////////

/**
 *	@brief	Constructor.
 */
WinKeyInputEdit::WinKeyInputEdit()
{
	keyNameDB = NULL;
	hImcOrg = NULL;
}

/**
 *	@brief	Destructor
 */
WinKeyInputEdit::~WinKeyInputEdit()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	keyNameDB	key-name database
 */
void WinKeyInputEdit::Init(const KeyNameDB* keyNameDB)
{
	this->keyNameDB = keyNameDB;
}

/**
 *	@brief	Attaches existing edit control.
 *	@param[in]	hWnd	the window handle of edit control.
 */
void WinKeyInputEdit::Attach(HWND hWnd)
{
	if (NULL == m_hWnd)
	{
		// subclass window.
		Subclass(hWnd);
		
		// disable IME
		hImcOrg = ImmAssociateContext(hWnd, NULL);
		
		// install message filter
		WinCoveredCalcApp::GetInstance()->InstallMessageFilter(this);
	}
}

/**
 *	@brief	Detaches edit control.
 */
void WinKeyInputEdit::Detach()
{
	if (NULL != m_hWnd)
	{
		// uninstall message filter
		WinCoveredCalcApp::GetInstance()->UninstallMessageFilter(this);
	
		// restore IME
		ImmAssociateContext(m_hWnd, hImcOrg);
		
		// unsubclass window
		UnSubclass();
	}
}

/**
 *	@brief	Sets current value.
 *	@param[in]	param	a value to set to this control.
 */
void WinKeyInputEdit::SetKeyEventParameter(const KeyEventParameter& param)
{
	this->value = param;
	
	// create text of the value.
	MBCString valueText;
	keyNameDB->GetKeyName(&param, valueText);
	
	Edit_SetSel(m_hWnd, 0, -1);
	Edit_ReplaceSel(m_hWnd, valueText.CString());
}

/**
 *	@brief	Window procedure.
 */
LRESULT WinKeyInputEdit::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_GETDLGCODE:
		return onGetDlgCode(hWnd, uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		return onKeyDown(hWnd, uMsg, wParam, lParam);
		break;
	case WM_CHAR:
	case WM_SYSCHAR:
		return onChar(hWnd, uMsg, wParam, lParam);
		break;
	case WM_CONTEXTMENU:
		return onContextMenu(hWnd, uMsg, wParam, lParam);
		break;
	default:
		return base::wndProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}

/**
 *	@brief	WM_GETDLGCODE handler.
 *	@param[in]	hWnd	handle to window.
 *	@param[in]	uMsg	WM_GETDLGCODE
 *	@param[in]	wParam	not used.
 *	@param[in]	lParam	Pointer to an MSG structure (or NULL if the system is performing a query).
 *	@return	DLGC_xxxx value indicating which type of input.
 */
LRESULT WinKeyInputEdit::onGetDlgCode(HWND /* hWnd */, UINT /* uMsg */, WPARAM /* wParam */, LPARAM /* lParam */)
{
	return DLGC_WANTALLKEYS;
}

/**
 *	@brief	Filters message.
 *	@param[in,out] msg	message.
 *	@return false to skip this message. true to dispatch this message.
 */
bool WinKeyInputEdit::FilterMessage(MSG* msg)
{
	if (msg->hwnd != m_hWnd)
		return true;
	
	if (WM_KEYDOWN == msg->message || WM_SYSKEYDOWN == msg->message)
	{
		switch (msg->wParam)
		{
		case VK_LBUTTON:
		case VK_RBUTTON:
		case VK_MBUTTON:
		case VK_SHIFT:
		case VK_CONTROL:
		case VK_MENU:
			return true;	// it ignores mouse button or modifier key.
		}

		KeyEventParameter parameter;
		UInt32 modifiers = 0;
		modifiers |= ((::GetKeyState(VK_SHIFT)   & 0x8000) ? KeyEventParameter::ModifierMask_Shift : 0);
		modifiers |= ((::GetKeyState(VK_CONTROL) & 0x8000) ? KeyEventParameter::ModifierMask_Ctrl  : 0);
		modifiers |= ((::GetKeyState(VK_MENU)    & 0x8000) ? KeyEventParameter::ModifierMask_Alt   : 0);
		DWORD keyCode = static_cast<DWORD>(msg->wParam);
		parameter.SetKeyCode(keyCode);
		parameter.SetModifiers(modifiers);
		SetKeyEventParameter(parameter);
		return false;
	}
	else if (WM_CHAR == msg->message || WM_CONTEXTMENU == msg->message)
	{
		// it does not pass this message to Edit control.
		return false;
	}
	
	return true;
}

/**
 *	@brief	WM_KEYDOWN handler.
 *	@param[in]	hWnd	handle to window.
 *	@param[in]	uMsg	WM_KEYDOWN
 *	@param[in]	wParam	Specifies the virtual-key code of the nonsystem key.
 *	@param[in]	lParam	Specifies the repeat count, scan code, extended-key flag, context code, previous key-state flag, and transition-state flag.
 *	@return	zero if it processes this message.
 */
LRESULT WinKeyInputEdit::onKeyDown(HWND /* hWnd */, UINT /* uMsg */, WPARAM /* wParam */, LPARAM /* lParam */)
{
	// it does not pass this message to Edit control.
	//base::wndProc(hWnd, uMsg, wParam, lParam);
	return 0;
}

/**
 *	@brief	WM_CHAR handler.
 *	@param[in]	hWnd	handle to window.
 *	@param[in]	uMsg	WM_CHAR
 *	@param[in]	wParam	
 *	@param[in]	lParam	
 *	@return	zero if it processes this message.
 */
LRESULT WinKeyInputEdit::onChar(HWND /* hWnd */, UINT /* uMsg */, WPARAM /* wParam */, LPARAM /* lParam */)
{
	// it does not pass this message to Edit control.
	//base::wndProc(hWnd, uMsg, wParam, lParam);
	return 0;
}

/**
 *	@brief	WM_CONTEXTMENU handler.
 *	@param[in]	hWnd	handle to window.
 *	@param[in]	uMsg	WM_CONTEXTMENU
 *	@param[in]	wParam	
 *	@param[in]	lParam	
 *	@return	zero if it processes this message.
 */
LRESULT WinKeyInputEdit::onContextMenu(HWND /* hWnd */, UINT /* uMsg */, WPARAM /* wParam */, LPARAM /* lParam */)
{
	// it does not pass this message to Edit control.
	//base::wndProc(hWnd, uMsg, wParam, lParam);
	return 0;
}
