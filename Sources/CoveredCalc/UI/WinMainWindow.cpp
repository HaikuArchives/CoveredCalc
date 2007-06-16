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
	@file		WinMainWindow.cpp
	@brief		WinMainWindow �N���X�̎���
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.24 created
*/

#include "Prefix.h"
#include "WinCoveredCalcApp.h"
#include "DialogInfo.h"
#include "UIControllerException.h"
#include "MainUIManager.h"
#include "WinAboutDlg.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "WinMainWindow.h"
#include "WinAboutCurrentCoverDlg.h"
#include "WinPreferencesDlg.h"

////////////////////////////////////////
#define base	WinSkinWindow
////////////////////////////////////////

// ---------------------------------------------------------------------
//! ���C���E�B���h�E�̃E�B���h�E�N���X��
// ---------------------------------------------------------------------
const AChar WinMainWindow::windowClassName[] = "CoveredCalcMainWindowClass";

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinMainWindow::WinMainWindow()
{
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinMainWindow::~WinMainWindow()
{
}

// ---------------------------------------------------------------------
//! ���̃E�B���h�E�̃E�B���h�E�N���X��o�^���܂��B
/*!
	@return ATOM
*/
// ---------------------------------------------------------------------
ATOM WinMainWindow::RegisterClass()
{
	HINSTANCE hInstance = CHrnApp::GetAppObject()->GetInstanceHandle();
	return CHrnWnd::RegisterClass(
			0,
			0,
			0,
			hInstance,
			LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON)),
			LoadCursor(NULL, IDC_ARROW),
			NULL,
			NULL,
			windowClassName,
			NULL);
}

// ---------------------------------------------------------------------
//! ���̃E�B���h�E�p�� UIManager �I�u�W�F�N�g�𐶐����܂��B
/*!
	@return UIManager �I�u�W�F�N�g
*/
// ---------------------------------------------------------------------
UIManager* WinMainWindow::createUIManager()
{
	MainUIManager* uiManager = new MainUIManager();
	try
	{
		uiManager->Init(this);
		uiManager->Create();
	}
	catch (...)
	{
		delete uiManager;
		throw;
	}
	return uiManager;	
}

// ---------------------------------------------------------------------
//! ���̃E�B���h�E�p�� UIManager �I�u�W�F�N�g���폜���܂��B
/*!
	@return UIManager �I�u�W�F�N�g
*/
// ---------------------------------------------------------------------
void WinMainWindow::deleteUIManager(UIManager* uiManager)
{
	if (NULL != uiManager)
	{
		MainUIManager* mainUIManager = dynamic_cast<MainUIManager*>(uiManager);
		ASSERT(NULL != mainUIManager);
		if (NULL != mainUIManager)
		{
			mainUIManager->Destroy();
		}

		delete uiManager;
	}
}

// ---------------------------------------------------------------------
//! �w�肳�ꂽ�_�C�A���O�ɑΉ����� WinDialog �I�u�W�F�N�g�𐶐����܂��B
/*!
	@return WinDialog �I�u�W�F�N�g
	@throw UIControllerExceptions::FailedToShowDialog �I�u�W�F�N�g�������ł��Ȃ������Ƃ�
*/
// ---------------------------------------------------------------------
WinDialog* WinMainWindow::createDialogObject(
	DialogInfo* dialogInfo					//!< �_�C�A���O�̏��
)
{
	WinDialog* retDlg = NULL;

	switch (dialogInfo->GetDialogID())
	{
	case IDD_ABOUT:
		retDlg = new WinAboutDlg();
		break;
	case IDD_ABOUT_COVER:
		retDlg = new WinAboutCurrentCoverDlg();
		break;
	case IDD_PREFERENCES:
		retDlg = new WinPreferencesDlg();
		break;
	default:
		throw new UIControllerExceptions::FailedToShowDialog(dialogInfo->GetDialogID());
		break;
	}
	
	return retDlg;
}

// ---------------------------------------------------------------------
//! �E�B���h�E�v���V�[�W��
/*!
	@return �e���b�Z�[�W�ɑΉ�����߂�l
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::wndProc(
	HWND hWnd,			// �E�B���h�E�n���h��
	UINT uMsg,			// ���b�Z�[�W
	WPARAM wParam,		// ���b�Z�[�W�� WPARAM
	LPARAM lParam		// ���b�Z�[�W�� LPARAM
)
{
	try
	{
		switch (uMsg)
		{
		case WM_CREATE:
			return onCreate(hWnd, uMsg, wParam, lParam);
			break;
		case WM_CLOSE:
			return onClose(hWnd, uMsg, wParam, lParam);
			break;
		case WM_ENABLE:
			return onEnable(hWnd, uMsg, wParam, lParam);
			break;
		case WM_COMMAND:
			return onCommand(hWnd, uMsg, wParam, lParam);
			break;
		case WM_DISPLAYCHANGE:
			return onDisplayChange(hWnd, uMsg, wParam, lParam);
			break;
		default:
			return base::wndProc(hWnd, uMsg, wParam, lParam);
			break;
		}
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
		ex->Delete();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_CREATE �n���h��
/*!
	@retval 0 ����
	@retval -1 ���s
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::onCreate(
	HWND hWnd,		// �E�B���h�E�n���h��
	UINT uMsg,		// WM_CREATE
	WPARAM wParam,	// ���p���Ȃ��p�����[�^
	LPARAM lParam	// �E�B���h�E�쐬�������� CREATESTRUCT �\���̂ւ̃|�C���^
)
{
	if (-1 == base::wndProc(hWnd, uMsg, wParam, lParam))
	{
		return -1;
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_CLOSE �n���h��
/*!
	���C���E�B���h�E��������Ƃ��ɃA�v���P�[�V�������g���I�������܂��B
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::onClose(
	HWND /*hWnd*/,		// �E�B���h�E�n���h��
	UINT /*uMsg*/,		// WM_DESTROY
	WPARAM /*wParam*/,	// ���p���Ȃ��p�����[�^
	LPARAM /*lParam*/	// ���p���Ȃ��p�����[�^
)
{
	// �e�̃E�B���h�E�v���V�[�W���͌Ăяo���Ȃ��iDestroyWindow�� WinCoveredCalcApp::Quit() �ɌĂ΂���j
	WinCoveredCalcApp::GetInstance()->Quit();	
	return 0;
}

// ---------------------------------------------------------------------
//! WM_ENABLE �n���h��
/*!
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::onEnable(
	HWND hWnd,			// �E�B���h�E�n���h��
	UINT uMsg,			// WM_ENABLE
	WPARAM wParam,		// TRUE �Ȃ�L���ɂȂ� / FALSE �Ȃ疳���ɂȂ�
	LPARAM lParam		// ���p���Ȃ��p�����[�^
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	bool isEnabled = (wParam) ? true : false;
	WinCoveredCalcApp::GetInstance()->EnableCoveredCalcWindows(isEnabled);
	return 0;
}

// ---------------------------------------------------------------------
//! WM_COMMAND �n���h��
/*!
	@retval 0 ���̃��b�Z�[�W����������
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::onCommand(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< WM_COMMAND
	WPARAM wParam,		//!< ��ʃ��[�h���ʒm�R�[�h�A���ʃ��[�h���R�}���hID
	LPARAM lParam		//!< ���̃��b�Z�[�W�𑗂����R���g���[���̃n���h��
)
{
	WORD command = LOWORD(wParam);
	UIManager* uiManager = getUIManager();
	MainUIManager* mainUIManager = dynamic_cast<MainUIManager*>(uiManager);
	if (mainUIManager == NULL)
	{
		return base::wndProc(hWnd, uMsg, wParam, lParam);		
	}

	switch (command)
	{
	case ID_COVER_BROWSER:
		mainUIManager->DoFuncCoverBrowser();
		break;
	case ID_RADIX_HEX:
		mainUIManager->DoFuncChangeRadix(CalcCore::DigitForm_16);
		break;
	case ID_RADIX_DECIMAL:
		mainUIManager->DoFuncChangeRadix(CalcCore::DigitForm_10);
		break;
	case ID_RADIX_OCTAL:
		mainUIManager->DoFuncChangeRadix(CalcCore::DigitForm_8);
		break;
	case ID_RADIX_BINARY:
		mainUIManager->DoFuncChangeRadix(CalcCore::DigitForm_2);
		break;	
	case ID_MAIN_ALWAYS_ON_TOP:
		mainUIManager->DoFuncMainWindowAlwaysOnTop();
		break;
	case ID_MAIN_LOCK_POS:
		mainUIManager->DoFuncMainWindowLockPos();
		break;
	case ID_PREFERENCES:
		mainUIManager->DoFuncPreferences();
		break;
	case ID_ABOUT:
		mainUIManager->DoFuncAbout();
		break;
	case ID_MAIN_ABOUT_COVER:
		mainUIManager->DoFuncAboutCurrentCover();
		break;
	case ID_MAIN_MINIMIZE:
		mainUIManager->DoFuncMainMinimize();
		break;
	case IDCLOSE:
	case ID_MAIN_CLOSE:
		mainUIManager->DoFuncClose();
		break;
	default:
		return base::wndProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	
	return 0;
}

// ---------------------------------------------------------------------
//! WM_DISPLAYCHANGE �n���h��
/*!
	@retval 0 ���
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::onDisplayChange(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< WM_DISPLAYCHANGE
	WPARAM wParam,		//!< bit per pixel �ŕ\���� image depth
	LPARAM lParam		//!< ���ʃ��[�h�����������̉𑜓x�A��ʃ��[�h�����������̉𑜓x
)
{
	// ���j�^�����ŐV�ɍX�V
	WinCoveredCalcApp::GetInstance()->GetMonitorInformation()->Update();

	return base::wndProc(hWnd, uMsg, wParam, lParam);
}
