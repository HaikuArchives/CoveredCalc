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
	@file		WinPreferencesDlg.cpp
	@brief		Implementation of WinPreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.1.14 created
*/

#include "Prefix.h"
#include "WinPreferencesDlg.h"
#include <vector>
#include "resource.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcApp.h"
#include "LangFileInfo.h"
#include "WinCoveredCalcApp.h"
#include "WinEditKeymapDlg.h"
#include "StringID.h"
#include "WinDialogControlCreator.h"
#include "DialogID.h"
#include "UICEventCode.h"

////////////////////////////////////////
#define baseDialog	WinDialog
#define base		PreferencesDlg
////////////////////////////////////////

/**
 *	@brief	コンストラクタ
 */
WinPreferencesDlg::WinPreferencesDlg() : baseDialog(IDD_PREFERENCES)
{
	langComboInfos = NULL;
}

/**
 *	@brief	デストラクタ
 */
WinPreferencesDlg::~WinPreferencesDlg()
{
}

/**
 *	@brief	Shows "Edit Keyboard" dialog and wait until it is closed.
 *	@param[in]		isReadOnly	true when read-only mode.
 *	@param[in,out]	keyMappings	Key-mappings definition.
 *	@retval	true	user closes the dialog by OK button.
 *	@retval	false	user closes the dialog by Cancel button.
 */
bool WinPreferencesDlg::showEditKeyMapDialog(bool isReadOnly, KeyMappings& keyMappings)
{
	WinEditKeymapDlg dlg;
	dlg.Init(isReadOnly, CoveredCalcApp::GetInstance()->GetKeyNameDB());
	dlg.SetKeyMappings(&keyMappings);
	if (IDOK == dlg.DoModal(m_hWnd))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 *	@brief	ウィンドウプロシージャ
	@return 各メッセージに対応する戻り値
 */
LRESULT WinPreferencesDlg::wndProc(
	HWND hWnd,			///< ウィンドウハンドル
	UINT uMsg,			///< メッセージ
	WPARAM wParam,		///< メッセージの WPARAM
	LPARAM lParam		///< メッセージの LPARAM
)
{
	try
	{
		switch (uMsg)
		{
		case WM_INITDIALOG:
			return onInitDialog(hWnd, uMsg, wParam, lParam);
			break;
		case WM_COMMAND:
			return onCommand(hWnd, uMsg, wParam, lParam);
			break;
		case WM_DESTROY:
			return onDestroy(hWnd, uMsg, wParam, lParam);
			break;
		default:
			return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
			break;
		}
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(this, ex);
		ex->Delete();
	}
	
	return 0;
}

/**
 *	@brief	Creates dialog controls.
 */
void WinPreferencesDlg::createControls()
{
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
	WinDialogControlCreator dcc(m_hWnd, getDialogLayout());
	MBCString label;
	HWND hControl;
	HWND hSlider, hLeftBuddy, hRightBuddy;

	// set dialog title
	SetWindowText(m_hWnd, stringLoader->LoadNativeString(NSID_PREFERENCES_TITLE));

	// "Transparency" group box
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_GROUP_TRANSPARENCY));
	hControl = dcc.CreateGroupBox(ALITERAL("IDC_GROUP_TRANSPARENCY"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Opacity" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_OPACITY));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_OPACITY"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Opacity" slider left label ("Transparent")
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_TRANSPARENT));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_TRANSPARENT"), IDC_STC_TRANSPARENT, label, SS_RIGHT, SS_LEFT | SS_CENTER, 0, 0);
	hLeftBuddy = hControl;

	// "Opacity" slider
	hControl = dcc.CreateTrackbar(ALITERAL("IDC_SLDR_OPACITY"), IDC_SLDR_OPACITY, TBS_AUTOTICKS | TBS_BOTTOM, 0, 0, 0);
	uicOpacitySlider.Init(hControl);
	hSlider = hControl;

	// "Opacity" slider right label ("Opaque")
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_OPAQUE));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_OPAQUE"), IDC_STC_OPAQUE, label, SS_LEFT, SS_RIGHT | SS_CENTER, 0, 0);
	hRightBuddy = hControl;

	// set "Opacity" slider buddies
	SendMessage(hSlider, TBM_SETBUDDY, TRUE, reinterpret_cast<LPARAM>(hLeftBuddy));
	SendMessage(hSlider, TBM_SETBUDDY, FALSE, reinterpret_cast<LPARAM>(hRightBuddy));

	// "Cover Edge Smoothing" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_SMOTHING));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_SMOOTHING"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Cover Edge Smoothing" slider left label ("Low")
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_LOW));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_LOW"), IDC_STC_LOW, label, SS_RIGHT, SS_LEFT | SS_CENTER, 0, 0);
	hLeftBuddy = hControl;

	// "Cover Edge Smoothing" slider
	hControl = dcc.CreateTrackbar(ALITERAL("IDC_SLDR_EDGE_SMOOTHING"), IDC_SLDR_EDGE_SMOOTHING, TBS_AUTOTICKS | TBS_BOTTOM, 0, 0, 0);
	uicEdgeSmoothingSlider.Init(hControl);
	hSlider = hControl;

	// "Cover Edge Smoothing" slider right label ("High")
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_HIGH));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_HIGH"), IDC_STC_HIGH, label, SS_LEFT, SS_RIGHT | SS_CENTER, 0, 0);
	hRightBuddy = hControl;

	// set "Cover Edge Smoothing" slider buddies
	SendMessage(hSlider, TBM_SETBUDDY, TRUE, reinterpret_cast<LPARAM>(hLeftBuddy));
	SendMessage(hSlider, TBM_SETBUDDY, FALSE, reinterpret_cast<LPARAM>(hRightBuddy));

	// message
	label = stringLoader->LoadNativeString(NSID_PREFERENCES_TRANSPARENCY_MESSAGE);
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_TRANSPARENCY_MESSAGE"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Language" group box
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_GROUP_LANGUAGE));
	hControl = dcc.CreateGroupBox(ALITERAL("IDC_GROUP_LANGUAGE"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Language" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_LANGUAGE));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_LANGUAGE"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Language" combo box
	hControl = dcc.CreateComboBox(ALITERAL("IDC_CMB_LANGUAGE"), IDC_CMB_LANGUAGE, WS_VSCROLL | CBS_DROPDOWNLIST, 0, 0, 0);
	uicLanguageListBox.Init(hControl);

	// message
	label = stringLoader->LoadNativeString(NSID_PREFERENCES_LANGUAGE_MESSAGE);
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_LANGUAGE_MESSAGE"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Keymap" group box
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_GROUP_KEYMAP));
	hControl = dcc.CreateGroupBox(ALITERAL("IDC_GROUP_KEYMAP"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Keymap" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_KEYMAP));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_KEYMAP"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Keymap" combo box
	hControl = dcc.CreateComboBox(ALITERAL("IDC_CMB_KEYMAPPINGS"), IDC_CMB_KEYMAPPINGS, WS_VSCROLL | CBS_DROPDOWNLIST, 0, 0, 0);
	keyMappingSeparators.Attach(hControl);
	uicKeyMapListBox.Init(hControl, &keyMappingSeparators);

	// "Edit" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_EDIT_KEYMAP));
	hControl = dcc.CreateButton(ALITERAL("IDC_EDIT_KEYMAPPING"), IDC_EDIT_KEYMAPPING, label, WS_GROUP, 0, 0, 0);
	uicEditKeyMapButton.Init(hControl);

	// "Duplicate" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_DUPLICATE_KEYMAP));
	hControl = dcc.CreateButton(ALITERAL("IDC_DUPLICATE_KEYMAPPING"), IDC_DUPLICATE_KEYMAPPING, label, 0, 0, 0, 0);
	uicDuplicateKeyMapButton.Init(hControl);

	// "Delete" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_DELETE_KEYMAP));
	hControl = dcc.CreateButton(ALITERAL("IDC_DELETE_KEYMAPPING"), IDC_DELETE_KEYMAPPING, label, 0, 0, 0, 0);
	uicDeleteKeyMapButton.Init(hControl);

	// "OK" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_OK));
	hControl = dcc.CreateButton(ALITERAL("IDOK"), IDOK, label, WS_GROUP | BS_DEFPUSHBUTTON, BS_PUSHBUTTON, 0, 0);
	
	// "Cancel" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_PREFERENCES_CANCEL));
	hControl = dcc.CreateButton(ALITERAL("IDCANCEL"), IDCANCEL, label, 0, 0, 0, 0);
}

/**
 *	@brief	WM_INITDIALOG ハンドラ
 *	@retval TRUE wParam で指定されたコントロールにフォーカスを与えてください。
 *	@retval FALSE フォーカスを与えないでください。
 */
LRESULT WinPreferencesDlg::onInitDialog(
	HWND hWnd,			///< ウィンドウハンドル
	UINT uMsg,			///< メッセージ
	WPARAM wParam,		///< メッセージの WPARAM
	LPARAM lParam		///< メッセージの LPARAM
)
{
	baseDialog::wndProc(hWnd, uMsg, wParam, lParam);

	// create controls
	createControls();

	readyToShow();

	return TRUE;
}

/**
 *	@brief	WM_COMMAND ハンドラ
 *	@retval 0 このメッセージを処理した。
 */
LRESULT WinPreferencesDlg::onCommand(
	HWND hWnd,			///< ウィンドウハンドル
	UINT uMsg,			///< メッセージ
	WPARAM wParam,		///< メッセージの WPARAM
	LPARAM lParam		///< メッセージの LPARAM
)
{
	WORD command = LOWORD(wParam);
	switch (command)
	{
	case IDC_CMB_KEYMAPPINGS:
		if (CBN_SELCHANGE == HIWORD(wParam))
		{
			HandleUICEvent(CID_KeyMapListBox, UICE_SelectionChanged, 0, NULL);
			return 0;
		}
		else
		{
			return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	case IDC_EDIT_KEYMAPPING:
		HandleUICEvent(CID_EditKeyMapButton, UICE_ButtonClicked, 0, NULL);
		return 0;
		break;
	case IDC_DUPLICATE_KEYMAPPING:
		HandleUICEvent(CID_DuplicateKeyMapButton, UICE_ButtonClicked, 0, NULL);
		return 0;
		break;
	case IDC_DELETE_KEYMAPPING:
		HandleUICEvent(CID_DeleteKeyMapButton, UICE_ButtonClicked, 0, NULL);
		return 0;
		break;
	case IDOK:
		HandleUICEvent(CID_OKButton, UICE_ButtonClicked, 0, NULL);
		return 0;
		break;
	case IDCANCEL:
		HandleUICEvent(CID_CancelButton, UICE_ButtonClicked, 0, NULL);
		return 0;
		break;
	default:
		return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}

/**
 *	@brief	WM_DESTROY ハンドラ
 *	@param[in]	hWnd	ウィンドウハンドル
 *	@param[in]	uMsg	WM_DESTROY
 *	@param[in]	wParam	使用しません。
 *	@param[in]	lParam	使用しません。
 *	@retval 0 このメッセージを処理した。 
 */
LRESULT WinPreferencesDlg::onDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	keyMappingSeparators.Detach();
	return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
}

/**
 *	@brief	Close this dialog.
 *	@param[in]	isOK	true if dialog is closing by OK button.
 */
void WinPreferencesDlg::closeDialog(bool isOK)
{
	EndDialog(m_hWnd, (isOK) ? IDOK : IDCANCEL);

	if (isOK)
	{
		PostMessage(WinCoveredCalcApp::GetInstance()->GetMainWindow()->m_hWnd, WinSkinWindow::UM_REREAD_SKIN, 0, 0);
	}
}
