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
	@file		WinSelectLanguageDlg.cpp
	@brief		Implementation of WinSelectLanguageDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.9.13 created
*/

#include "Prefix.h"
#include "WinSelectLanguageDlg.h"
#include "StringID.h"
#include "WinDialogControlCreator.h"
#include "DialogID.h"
#include "UICEventCode.h"
#include "ExceptionMessageUtils.h"
#include "NativeStringLoader.h"
#include "CoveredCalcApp.h"
#include "XMLLangFile.h"

////////////////////////////////////////
#define baseDialog	WinDialog
#define base		SelectLanguageDlg
////////////////////////////////////////

/**
 * @brief Constructor
 */
WinSelectLanguageDlg::WinSelectLanguageDlg() : baseDialog(IDD_SELECT_LANGUAGE)
{
}

/**
 * @brief Destructor
 */
WinSelectLanguageDlg::~WinSelectLanguageDlg()
{
}

/**
 * @brief window procedure
 * @return return value for each message
 */
LRESULT WinSelectLanguageDlg::wndProc(
	HWND hWnd,			///< handle of window
	UINT uMsg,			///< message
	WPARAM wParam,		///< WPARAM of the message
	LPARAM lParam		///< LPARAM of the message
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
void WinSelectLanguageDlg::createControls()
{
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
	WinDialogControlCreator dcc(m_hWnd, getDialogLayout());
	MBCString label;
	HWND hControl;

	// set dialog title
	SetWindowText(m_hWnd, stringLoader->LoadNativeString(NSID_SELECT_LANGUAGE_TITLE));

	// message
	label = stringLoader->LoadNativeString(NSID_SELECT_LANGUAGE_MESSAGE);
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_MESSAGE"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Language" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_SELECT_LANGUAGE_LANGUAGE));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_LANGUAGE"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Language" combo box
	hControl = dcc.CreateComboBox(ALITERAL("IDC_CMB_LANGUAGE"), IDC_CMB_LANGUAGE, WS_VSCROLL | CBS_DROPDOWNLIST, 0, 0, 0);
	uicLanguageListBox.Init(hControl);

	// "OK" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_SELECT_LANGUAGE_OK));
	hControl = dcc.CreateButton(ALITERAL("IDOK"), IDOK, label, WS_GROUP | BS_DEFPUSHBUTTON, BS_PUSHBUTTON, 0, 0);
	
	// "Cancel" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_SELECT_LANGUAGE_CANCEL));
	hControl = dcc.CreateButton(ALITERAL("IDCANCEL"), IDCANCEL, label, 0, 0, 0, 0);
}

/**
 *	@brief	WM_INITDIALOG handler
 *	@retval TRUE give input focus to a control which specified in wParam
 *	@retval FALSE don't give input focus
 */
LRESULT WinSelectLanguageDlg::onInitDialog(
	HWND hWnd,			///< handle of window
	UINT uMsg,			///< message
	WPARAM wParam,		///< WPARAM of the message
	LPARAM lParam		///< LPARAM of the message
)
{
	baseDialog::wndProc(hWnd, uMsg, wParam, lParam);

	// create controls
	createControls();

	readyToShow();

	return TRUE;
}

/**
 *	@brief	WM_COMMAND handler
 *	@retval 0 processed this message.
 */
LRESULT WinSelectLanguageDlg::onCommand(
	HWND hWnd,			///< handle of window
	UINT uMsg,			///< message
	WPARAM wParam,		///< WPARAM of the message
	LPARAM lParam		///< LPARAM of the message
)
{
	WORD command = LOWORD(wParam);
	switch (command)
	{
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
 *	@brief	Close this dialog.
 *	@param[in]	isOK	true if dialog is closing by OK button.
 */
void WinSelectLanguageDlg::closeDialog(bool isOK)
{
	EndDialog(m_hWnd, (isOK) ? IDOK : IDCANCEL);
}
