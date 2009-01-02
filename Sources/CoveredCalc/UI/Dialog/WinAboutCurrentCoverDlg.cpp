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
	@file		WinAboutCurrentCoverDlg.cpp
	@brief		Implementation of WinAboutCurrentCoverDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.06 created
*/

#include "Prefix.h"
#include "WinAboutCurrentCoverDlg.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcApp.h"
#include "Exception.h"
#include "WinDialogControlCreator.h"
#include "XMLLangFile.h"
#include "StringID.h"
#include "DialogID.h"

////////////////////////////////////////
#define base	WinDialog
////////////////////////////////////////

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinAboutCurrentCoverDlg::WinAboutCurrentCoverDlg() : base(IDD_ABOUT_COVER)
{
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinAboutCurrentCoverDlg::~WinAboutCurrentCoverDlg()
{
}

// ---------------------------------------------------------------------
//! �E�B���h�E�v���V�[�W��
/*!
	@return �e���b�Z�[�W�ɑΉ�����߂�l
*/
// ---------------------------------------------------------------------
LRESULT WinAboutCurrentCoverDlg::wndProc(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< ���b�Z�[�W
	WPARAM wParam,		//!< ���b�Z�[�W�� WPARAM
	LPARAM lParam		//!< ���b�Z�[�W�� LPARAM
)
{
	try
	{
		switch (uMsg)
		{
		case WM_INITDIALOG:
			return onInitDialog(hWnd, uMsg, wParam, lParam);
			break;
		case WM_DESTROY:
			return onDestroy(hWnd, uMsg, wParam, lParam);
			break;
		default:
			return base::wndProc(hWnd, uMsg, wParam, lParam);
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
void WinAboutCurrentCoverDlg::createControls()
{
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
	WinDialogControlCreator dcc(m_hWnd, getDialogLayout());
	MBCString label;
	HWND hControl;
	
	// set dialog title
	SetWindowText(m_hWnd, stringLoader->LoadNativeString(NSID_ABOUT_COVER_TITLE).CString());

	// "OK" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_ABOUT_COVER_OK));
	hControl = dcc.CreateButton(ALITERAL("IDOK"), IDOK, label, WS_GROUP | BS_DEFPUSHBUTTON, BS_PUSHBUTTON, 0, 0);

	// "Name" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_ABOUT_COVER_NAME));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_NAME"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Name" edit (read-only)
	hControl = dcc.CreateEdit(ALITERAL("IDC_EDIT_NAME"), IDC_EDIT_NAME, ALITERAL(""), ES_READONLY, 0, 0, WS_EX_CLIENTEDGE);
	uicNameTextEdit.Init(hControl);

	// "Description" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_ABOUT_COVER_DESCRIPTION));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_DESCRIPTION"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Description" edit (read-only)
	hControl = dcc.CreateEdit(ALITERAL("IDC_EDIT_DESCRIPTION"), IDC_EDIT_DESCRIPTION, ALITERAL(""), ES_READONLY, 0, 0, WS_EX_CLIENTEDGE);
	uicDescriptionTextEdit.Init(hControl);

	// "Cover Author Info" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_ABOUT_COVER_ABOUT));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_ABOUT"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Cover Authro Info" edit (read-only)
	hControl = dcc.CreateEdit(ALITERAL("IDC_EDIT_ABOUT"), IDC_EDIT_ABOUT, ALITERAL(""), WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY, 0, 0, 0);
	uicAboutTextEdit.Init(hControl);
}

// ---------------------------------------------------------------------
//! WM_INITDIALOG �n���h��
/*!
	@retval TRUE wParam �Ŏw�肳�ꂽ�R���g���[���Ƀt�H�[�J�X��^���Ă��������B
	@retval FALSE �t�H�[�J�X��^���Ȃ��ł��������B
*/
// ---------------------------------------------------------------------
LRESULT WinAboutCurrentCoverDlg::onInitDialog(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< WM_INITDIALOG
	WPARAM wParam,		//!< handle to the control to receive the default keyboard focus
	LPARAM lParam		//!< additional initialization data
)
{
	LRESULT ret = base::wndProc(hWnd, uMsg, wParam, lParam);

	// create controls
	createControls();

	// set focus to OK button
	SetFocus(GetDlgItem(m_hWnd, IDOK));

	initialize();
	return ret;
}

// ---------------------------------------------------------------------
//! WM_DESTROY �n���h��
/*!
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinAboutCurrentCoverDlg::onDestroy(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_DESTROY
	WPARAM wParam,	//!< ���p���Ȃ��p�����[�^
	LPARAM lParam	//!< ���p���Ȃ��p�����[�^
)
{
	terminate();
	return base::wndProc(hWnd, uMsg, wParam, lParam);
}
