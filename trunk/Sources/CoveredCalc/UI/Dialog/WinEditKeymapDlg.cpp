/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2009 CoveredCalc Project Contributors
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
#include "WinEditKeymapDlg.h"
#include "CoveredCalcApp.h"
#include "WinDialogControlCreator.h"
#include "XMLLangFile.h"
#include "StringID.h"
#include "DialogID.h"
#include "UICEventCode.h"

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
 *	@brief	Creates dialog controls.
 */
void WinEditKeymapDlg::createControls()
{
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
	WinDialogControlCreator dcc(m_hWnd, getDialogLayout());
	MBCString label;
	HWND hControl;

	// set dialog title
	SetWindowText(m_hWnd, stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_TITLE).CString());

	// "Name" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_NAME));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_NAME"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Name" edit
	hControl = dcc.CreateEdit(ALITERAL("IDC_EDIT_NAME"), IDC_EDIT_NAME, ALITERAL(""), 0, 0, 0, 0);
	uicNameTextEdit.Init(hControl);

	// "Function" group box
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_GROUP_FUNCTION));
	hControl = dcc.CreateGroupBox(ALITERAL("IDC_GROUP_FUNCTION"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Function" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_FUNCTION));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_FUNCTION"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// Function listbox
	hControl = dcc.CreateListBox(ALITERAL("IDC_LIST_FUNCTION"), IDC_LIST_FUNCTION, WS_VSCROLL | LBS_NOTIFY, 0, 0, 0);
	uicFunctionListBox.Init(hControl);

	// "Current Key" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_CURRENT_KEY));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_CURRENT_KEY"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Current Key listbox
	hControl = dcc.CreateListBox(ALITERAL("IDC_LIST_CURRENT_KEY"), IDC_LIST_CURRENT_KEY, WS_VSCROLL | LBS_NOTIFY, 0, 0, 0);
	uicCurrentKeysListBox.Init(hControl);

	// "Remove" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_REMOVE));
	hControl = dcc.CreateButton(ALITERAL("IDC_REMOVE"), IDC_REMOVE, label, WS_GROUP, 0, 0, 0);
	uicRemoveButton.Init(hControl);

	// "Key" group box
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_GROUP_KEY));
	hControl = dcc.CreateGroupBox(ALITERAL("IDC_GROUP_KEY"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Key" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_KEY));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_KEY"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// Key edit
	hControl = dcc.CreateEdit(ALITERAL("IDC_EDIT_KEY"), IDC_EDIT_KEY, ALITERAL(""), 0, 0, 0, 0);
	ctrlKeyInput.Init(keyNameDB);
	ctrlKeyInput.Attach(hControl);
	uicKeyInput.Init(&ctrlKeyInput);

	// "Assigned Function" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_ASSIGNED_FUNCTION));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_ASSIGNED_FUNCTION"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// Assigned function edit
	hControl = dcc.CreateEdit(ALITERAL("IDC_EDIT_ASSIGNED_FUNCTION"), IDC_EDIT_ASSIGNED_FUNCTION, ALITERAL(""), ES_READONLY, 0, 0, 0);
	uicAssignedFunctionTextEdit.Init(hControl);

	// "Assign" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_ASSIGN));
	hControl = dcc.CreateButton(ALITERAL("IDC_ASSIGN"), IDC_ASSIGN, label, WS_GROUP, 0, 0, 0);
	uicAssignButton.Init(hControl);

	// OK button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_OK));
	hControl = dcc.CreateButton(ALITERAL("IDOK"), IDOK, label, WS_GROUP | BS_DEFPUSHBUTTON, BS_PUSHBUTTON, 0, 0);
	uicOkButton.Init(hControl);

	// Cancel button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_EDIT_KEYMAP_CANCEL));
	hControl = dcc.CreateButton(ALITERAL("IDCANCEL"), IDCANCEL, label, 0, 0, 0, 0);
	uicCancelButton.Init(hControl);
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

	// create controls
	createControls();

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
				HandleUICEvent(CID_NameTextEdit, UICE_TextChanged, 0, NULL);
			}
			isProcessed = true;
		}
		break;
	
	case IDC_LIST_FUNCTION:
		if (LBN_SELCHANGE == code)
		{
			if (!uicFunctionListBox.GetRawAdapter()->IsSelectionChangedNotificationStopped())
			{
				HandleUICEvent(CID_FunctionListBox, UICE_SelectionChanged, 0, NULL);
			}
			isProcessed = true;
		}
		break;
	
	case IDC_LIST_CURRENT_KEY:
		if (LBN_SELCHANGE == code)
		{
			if (!uicCurrentKeysListBox.GetRawAdapter()->IsSelectionChangedNotificationStopped())
			{
				HandleUICEvent(CID_CurrentKeysListBox, UICE_SelectionChanged, 0, NULL);
			}
			isProcessed = true;
		}
		break;
	
	case IDC_EDIT_KEY:
		if (EN_CHANGE == code)
		{
			if (!uicKeyInput.GetRawAdapter()->IsValueChangedNotificationStopped())
			{
				HandleUICEvent(CID_KeyInput, UICE_TextChanged, 0, NULL);
			}
			isProcessed = true;
		}
		break;
	
	case IDC_ASSIGN:
		HandleUICEvent(CID_AssignButton, UICE_ButtonClicked, 0, NULL);
		isProcessed = true;
		break;
		
	case IDC_REMOVE:
		HandleUICEvent(CID_RemoveButton, UICE_ButtonClicked, 0, NULL);
		isProcessed = true;
		break;
	
	case IDCANCEL:
		HandleUICEvent(CID_CancelButton, UICE_ButtonClicked, 0, NULL);
		isProcessed = true;
		break;

	case IDOK:
		HandleUICEvent(CID_OKButton, UICE_ButtonClicked, 0, NULL);
		isProcessed = true;
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

/**
 *	@brief	Close this dialog.
 *	@param[in]	isOK	true if dialog is closing by OK button.
 */
void WinEditKeymapDlg::closeDialog(bool isOK)
{
	EndDialog((isOK) ? IDOK : IDCANCEL);
}
