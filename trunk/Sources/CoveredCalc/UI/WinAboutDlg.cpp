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
	@file		WinAboutDlg.cpp
	@brief		Implementation of WinAboutDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.20 created
*/

#include "Prefix.h"
#include "resource.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "WinCoveredCalcApp.h"
#include "MemoryException.h"
#include "WinAboutDlg.h"
#include "Copyright.h"
#include "WinDialogControlCreator.h"
#include "StringID.h"
#include "DialogID.h"

////////////////////////////////////////
#define base	WinDialog
////////////////////////////////////////

#define BACK_COLOR			RGB(0xff, 0xff, 0xff)		///< �w�i�F
#define TEXT_COLOR			RGB(0x00, 0x40, 0x80)		///< �e�L�X�g�F

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinAboutDlg::WinAboutDlg() : base(IDD_ABOUT)
{
	backBrush = NULL;
	iconImage = NULL;
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinAboutDlg::~WinAboutDlg()
{
	// �w�i�u���V���c���Ă���΍폜
	if (NULL != backBrush)
	{
		::DeleteObject(backBrush);
	}
}

// ---------------------------------------------------------------------
//! �E�B���h�E�v���V�[�W��
/*!
	@return �e���b�Z�[�W�ɑΉ�����߂�l
*/
// ---------------------------------------------------------------------
LRESULT WinAboutDlg::wndProc(
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
		case WM_ERASEBKGND:
			return onEraseBkGnd(hWnd, uMsg, wParam, lParam);
			break;
		case WM_CTLCOLORSTATIC:
			return onCtlColorStatic(hWnd, uMsg, wParam, lParam);
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
void WinAboutDlg::createControls()
{
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
	WinDialogControlCreator dcc(m_hWnd, getDialogLayout());
	MBCString label;
	HWND hControl;
	
	// "OK" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_ABOUT_OK));
	hControl = dcc.CreateButton(ALITERAL("IDOK"), IDOK, label, WS_GROUP | BS_DEFPUSHBUTTON, BS_PUSHBUTTON, 0, 0);

	// Icon
	hControl = dcc.CreateStatic(ALITERAL("IDC_ICON64"), IDC_ICON64, ALITERAL(""), WS_GROUP | SS_BITMAP | SS_CENTERIMAGE | SS_REALSIZEIMAGE, SS_LEFT, 0, 0);

	// Application name
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_APPNAME"), IDC_STC_APPNAME, ALITERAL(""), WS_GROUP, 0, 0, 0);

	// Version
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_VERSION"), IDC_STC_VERSION, ALITERAL(""), WS_GROUP, 0, 0, 0);

	// Copyright
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_COPYRIGHT1"), IDC_STC_COPYRIGHT1, ALITERAL(""), WS_GROUP, 0, 0, 0);
}

// ---------------------------------------------------------------------
//! WM_INITDIALOG �n���h��
/*!
	@retval TRUE wParam �Ŏw�肳�ꂽ�R���g���[���Ƀt�H�[�J�X��^���Ă��������B
	@retval FALSE �t�H�[�J�X��^���Ȃ��ł��������B
*/
// ---------------------------------------------------------------------
LRESULT WinAboutDlg::onInitDialog(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< WM_INITDIALOG
	WPARAM wParam,		//!< handle to the control to receive the default keyboard focus
	LPARAM lParam		//!< additional initialization data
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	// create controls
	createControls();

	// set dialog title
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
	SetWindowText(m_hWnd, stringLoader->LoadNativeString(NSID_ABOUT_TITLE).CString());

	// TODO: �t�H�[�J�X

	// �w�i�u���V���쐬
	backBrush = ::CreateSolidBrush(BACK_COLOR);

	// �o�[�W�������Z�b�g
	HWND hVersionStatic = ::GetDlgItem(m_hWnd, IDC_STC_VERSION);
	if (NULL != hVersionStatic)
	{
		MBCString versionMessage;
		makeVersionString(versionMessage);
		::SetWindowText(hVersionStatic, versionMessage);
	}

	// �A�v���P�[�V���������Z�b�g
	::SetDlgItemText(m_hWnd, IDC_STC_APPNAME, ApplicationTitleString);
	
	// ���쌠�����Z�b�g
	::SetDlgItemText(m_hWnd, IDC_STC_COPYRIGHT1, CopyrightString1);
	
	// �A�C�R���C���[�W���Z�b�g
	HWND iconImageWnd = GetDlgItem(hWnd, IDC_ICON64);
	if (NULL != iconImageWnd)
	{
		iconImage = LoadBitmap(WinCoveredCalcApp::GetInstance()->GetInstanceHandle(), MAKEINTRESOURCE(IDB_APPICON64));
		SendMessage(iconImageWnd, STM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(iconImage));
	}
	
	return TRUE;
}

// ---------------------------------------------------------------------
//! WM_DESTROY �n���h��
/*!
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinAboutDlg::onDestroy(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_DESTROY
	WPARAM wParam,	//!< ���p���Ȃ��p�����[�^
	LPARAM lParam	//!< ���p���Ȃ��p�����[�^
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);
	
	// �w�i�u���V���폜
	if (NULL != backBrush)
	{
		::DeleteObject(backBrush);
		backBrush = NULL;	
	}

	// �A�C�R���C���[�W���폜
	if (NULL != iconImage)
	{
		::DeleteObject(iconImage);
		iconImage = NULL;
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_ERASEBKGND �n���h��
/*!
	@retval !0	�w�i�����������Ƃ�
	@retval	0	�w�i���������Ȃ������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinAboutDlg::onEraseBkGnd(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT /*uMsg*/,		//!< WM_ERASEBKGND
	WPARAM wParam,		//!< �f�o�C�X�R���e�L�X�g
	LPARAM /*lParam*/	//!< ���p���Ȃ��p�����[�^
)
{
	HDC dc = reinterpret_cast<HDC>(wParam);

	RECT clientRect;
	::GetClientRect(hWnd, &clientRect);
	
	::FillRect(dc, &clientRect, backBrush);

	return TRUE;
}

// ---------------------------------------------------------------------
//! WM_CTLCOLORSTATIC �n���h��
/*!
	@return �w�i���������邽�߂̃u���V��Ԃ��܂��B
*/
// ---------------------------------------------------------------------
LRESULT WinAboutDlg::onCtlColorStatic(
	HWND /*hWnd*/,		//!< �E�B���h�E�n���h��
	UINT /*uMsg*/,		//!< WM_CTLCOLORSTATIC
	WPARAM wParam,		//!< �f�o�C�X�R���e�L�X�g
	LPARAM /*lParam*/	//!< �X�^�e�B�b�N�R���g���[���̃E�B���h�E�n���h��
)
{
	HDC dc = reinterpret_cast<HDC>(wParam);
	
	::SetBkColor(dc, BACK_COLOR);
	::SetTextColor(dc, TEXT_COLOR);
	return reinterpret_cast<LRESULT>(backBrush);
}

// ---------------------------------------------------------------------
//! �o�[�W������������쐬���܂��B
// ---------------------------------------------------------------------
void WinAboutDlg::makeVersionString(
	MBCString& versionString			//!< OUTPUT. �����ɍ쐬���������񂪕Ԃ����
)
{
	versionString = ALITERAL("Version ");
	TCHAR fileName[MAX_PATH];

	// �t�@�C�������擾
	::GetModuleFileName(NULL, fileName, sizeof(fileName) / sizeof(TCHAR));

	// ���u���b�N�̎��ۂ̃T�C�Y���擾����
	DWORD handle;
	DWORD dwBytes = ::GetFileVersionInfoSize(fileName, &handle);
	if (0 != dwBytes)
	{
		// �o�[�W�������̎��ۂ̃u���b�N���擾
		BYTE* block = static_cast<BYTE*>(malloc(dwBytes));
		if (NULL == block)
		{
			MemoryException::Throw();
		}
		try
		{
			if (::GetFileVersionInfo(fileName, handle, dwBytes, block))
			{
				struct LANGANDCODEPAGE {
					WORD wLanguage;
					WORD wCodePage;
				} *lpTranslate;
				UINT translateLen;
				if (::VerQueryValue(block, _T("\\VarFileInfo\\Translation"), reinterpret_cast<LPVOID*>(&lpTranslate), &translateLen))
				{
					if (translateLen >= 1)
					{
						TCHAR subBlock[64];	// �{���͂���Ȃɂ���Ȃ����ǁA�܂��]�T���݂āB
						wsprintf(subBlock, _T("\\StringFileInfo\\%04x%04x\\ProductVersion"), lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
						TCHAR* fileVersion;
						UINT fileVersionLen;
						if (::VerQueryValue(block, subBlock, reinterpret_cast<LPVOID*>(&fileVersion), &fileVersionLen))
						{
							versionString += fileVersion;
						}
					}
				}
			}
		}
		catch (...)
		{
			free(block);
			throw;
		}
		free(block);
	}
}

