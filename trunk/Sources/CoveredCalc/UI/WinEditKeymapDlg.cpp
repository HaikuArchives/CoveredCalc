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
	@file		WinEditKeymapDlg.cpp
	@brief		Implementation of WinEditKeymapDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.5.5 created
*/

#include "Prefix.h"
#include "WinEditKeymapDlg.h."
#include "CoveredCalcApp.h"

////////////////////////////////////////
#define baseDialog	WinPseudoModalDialog
#define base		EditKeymapDlg
////////////////////////////////////////

/**
 *	@brief	Constructor.
 */
WinEditKeymapDlg::WinEditKeymapDlg() : baseDialog(IDD_EDIT_KEYMAP)
{
}

/**
 *	@brief	Destructor.
 */
WinEditKeymapDlg::~WinEditKeymapDlg()
{
}

/**
 *	@brief	Window procedure.
 */
LRESULT WinEditKeymapDlg::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return onInitDialog(hWnd, uMsg, wParam, lParam);
		break;
	case WM_DESTROY:
		return onDestroy(hWnd, uMsg, wParam, lParam);
		break;
	case WM_COMMAND:
		return onCommand(hWnd, uMsg, wParam, lParam);
		break;
	default:
		return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}

/**
 *	@brief	WM_INITDIALOG handler.
 *	@param[in]	hWnd	window handle.
 *	@param[in]	uMsg	WM_INITDIALOG
 *	@param[in]	wParam	Handle to the control to receive the default keyboard focus.
 *	@param[in]	lParam	Specifies additional initialization data.
 *	@return	TRUE to direct the system to set the keyboard focus to the control specified by wParam. Otherwise, it should return FALSE to prevent the system from setting the default keyboard focus.
 */
LRESULT WinEditKeymapDlg::onInitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	baseDialog::wndProc(hWnd, uMsg, wParam, lParam);

	// initialize key-input control
	HWND hEditKey = GetDlgItem(m_hWnd, IDC_EDIT_KEY);
	ctrlKeyInput.Init(keyNameDB);
	ctrlKeyInput.Attach(hEditKey);

	// init control adapters
	uicNameTextEdit.Init(GetDlgItem(m_hWnd, IDC_EDIT_NAME));
	uicFunctionListBox.Init(GetDlgItem(m_hWnd, IDC_LIST_FUNCTION));
	uicCurrentKeyListBox.Init(GetDlgItem(m_hWnd, IDC_LIST_CURRENT_KEY));
	uicKeyInput.Init(&ctrlKeyInput);
	uicAssignedFunctionTextEdit.Init(GetDlgItem(m_hWnd, IDC_EDIT_ASSIGNED_FUNCTION));
	uicAssignButton.Init(GetDlgItem(m_hWnd, IDC_ASSIGN));
	uicRemoveButton.Init(GetDlgItem(m_hWnd, IDC_REMOVE));
	uicOkButton.Init(GetDlgItem(m_hWnd, IDOK));
	uicCancelButton.Init(GetDlgItem(m_hWnd, IDCANCEL));
	
	// ready
	readyToShow();

	return TRUE;
}

/**
 *	@brief	WM_DESTROY handler.
 *	@param[in]	hWnd	window handle.
 *	@param[in]	uMsg	WM_DESTROY
 *	@param[in]	wParam	not used.
 *	@param[in]	lParam	not used.
 *	@return	0 if processed.
 */
LRESULT WinEditKeymapDlg::onDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ctrlKeyInput.Detach();
	
	return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
}

/**
 *	@brief	WM_COMMAND handler
 *	@param[in]	hWnd	window handle.
 *	@param[in]	uMsg	WM_COMMAND
 *	@param[in]	wParam	notification code and control id.
 *	@param[in]	lParam	window handle to the control sending this message.
 *	@return	0 if processed.
 */
LRESULT WinEditKeymapDlg::onCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool isProcessed = false;

	WORD code = HIWORD(wParam);
	WORD id = LOWORD(wParam);
	switch (id)
	{
	case IDC_EDIT_NAME:
		if (EN_CHANGE == code)
		{
			if (!uicNameTextEdit.GetRawAdapter()->IsModificationNotificationStopped())
			{
				processNameTextEditChanged();
			}
			isProcessed = true;
		}
		break;
	
	case IDC_LIST_FUNCTION:
		if (LBN_SELCHANGE == code)
		{
			if (!uicFunctionListBox.GetRawAdapter()->IsSelectionChangedNotificationStopped())
			{
				processFunctionListBoxSelectionChanged();
			}
			isProcessed = true;
		}
		break;
	
	case IDC_LIST_CURRENT_KEY:
		if (LBN_SELCHANGE == code)
		{
			if (!uicCurrentKeyListBox.GetRawAdapter()->IsSelectionChangedNotificationStopped())
			{
				processCurrentKeysListBoxSelectionChanged();
			}
			isProcessed = true;
		}
		break;
	
	case IDC_EDIT_KEY:
		if (EN_CHANGE == code)
		{
			if (!uicKeyInput.GetRawAdapter()->IsValueChangedNotificationStopped())
			{
				processKeyInputChanged();
			}
			isProcessed = true;
		}
		break;
	
	case IDC_ASSIGN:
		processAssignButtonClicked();
		isProcessed = true;
		break;
		
	case IDC_REMOVE:
		processRemoveButtonClicked();
		isProcessed = true;
		break;
	
	case IDCANCEL:
		if (!processCancel())
		{
			// do not pass this message to the base class's procedure.
			isProcessed = true;
		}
		break;

	case IDOK:
		if (!processOK())
		{
			// do not pass this message to the base class's procedure.
			isProcessed = true;
		}
		break;
	}
	
	if (isProcessed)
	{
		return 0;
	}
	else
	{
		return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
	}
}
