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
	@file		WinDialog.cpp
	@brief		Implementation of WinDialog class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.03.22 created
*/

#include "Prefix.h"
#include "WinDialog.h"
#include "WinCoveredCalcApp.h"
#include "WinMessageBoxProvider.h"

////////////////////////////////////////
#define base	CHrnDlg
////////////////////////////////////////

/**
 *	@brief	���[�_���_�C�A���O��\�����܂��B
 *	@return	�_�C�A���O�̖߂�l
 */
int WinDialog::DoModal(
	HWND hParent	///< �e�E�B���h�E
)
{
	return base::DoModal(hParent, WinCoveredCalcApp::GetInstance()->GetLangResHandle());
}

/**
 *	@brief	���[�h���X�_�C�A���O�𐶐����܂��B�E
 *	@retval	TRUE	����
 *	@retval	FALSE	���s
 */
BOOL WinDialog::Create(
	HWND hParent	///< �e�E�B���h�E
)
{
	return base::Create(hParent, WinCoveredCalcApp::GetInstance()->GetLangResHandle());
}

// ---------------------------------------------------------------------
//! �E�B���h�E�v���V�[�W��
/*!
	@return �e���b�Z�[�W�ɑΉ�����߂�l
*/
// ---------------------------------------------------------------------
LRESULT WinDialog::wndProc(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< ���b�Z�[�W
	WPARAM wParam,		//!< ���b�Z�[�W�� WPARAM
	LPARAM lParam		//!< ���b�Z�[�W�� LPARAM
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return onInitDialog(hWnd, uMsg, wParam, lParam);
		break;		
	default:
		return base::wndProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}

// ---------------------------------------------------------------------
//! WM_INITDIALOG �n���h��
/*!
	@retval TRUE wParam �Ŏw�肳�ꂽ�R���g���[���Ƀt�H�[�J�X��^���Ă��������B
	@retval FALSE �t�H�[�J�X��^���Ȃ��ł��������B
*/
// ---------------------------------------------------------------------
LRESULT WinDialog::onInitDialog(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< WM_INITDIALOG
	WPARAM wParam,		//!< handle to the control to receive the default keyboard focus
	LPARAM lParam		//!< additional initialization data
)
{
	LRESULT result = base::wndProc(hWnd, uMsg, wParam, lParam);
	
	// ��ʊO�ɂ͂ݏo�����Ȃ�␳����
	const WinMonitorInfo* monitorInfo = WinCoveredCalcApp::GetInstance()->GetMonitorInformation();
	const MONITORINFO* monInfo = monitorInfo->GetMonitorInfoFromWindow(m_hWnd);
	if (NULL != monInfo)
	{
		RECT windowRect;
		::GetWindowRect(m_hWnd, &windowRect);
		
		LONG delta;
		bool moved = false;
		
		// ���[
		delta = windowRect.bottom - monInfo->rcWork.bottom;
		if (delta > 0)
		{
			::OffsetRect(&windowRect, 0, -delta);
			moved = true;
		}
		
		// �E�[
		delta = windowRect.right - monInfo->rcWork.right;
		if (delta > 0)
		{
			::OffsetRect(&windowRect, -delta, 0);
			moved = true;
		}
		
		// ��[
		delta = windowRect.top - monInfo->rcWork.top;
		if (delta < 0)
		{
			::OffsetRect(&windowRect, 0, -delta);
			moved = true;
		}
		
		// ���[
		delta = windowRect.left - monInfo->rcWork.left;
		if (delta < 0)
		{
			::OffsetRect(&windowRect, -delta, 0);
			moved = true;
		}

		if (moved)
		{		
			::SetWindowPos(m_hWnd, NULL, windowRect.left, windowRect.top, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
		}
	}
	
	return result;
}

// ---------------------------------------------------------------------
//! ���b�Z�[�W�{�b�N�X��\�����܂��B
/*!
	@return ���[�U���I�������{�^��
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button WinDialog::DoMessageBox(
	ConstAStr message,							//!< message text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	HWND hOwner;
	if (NULL != m_hWnd && ::IsWindow(m_hWnd))
	{
		hOwner = m_hWnd;
	}
	else
	{
		hOwner = NULL;
	}
	WinMessageBoxProvider messageBoxProvider(hOwner);
	return messageBoxProvider.DoMessageBox(message, buttonType, alertType, defaultButton);
}

// ---------------------------------------------------------------------
//! ���b�Z�[�W�{�b�N�X��\�����܂��B
/*!
	@return ���[�U���I�������{�^��
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button WinDialog::DoMessageBox(
	SInt32 messageId,							//!< message id of the text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	HWND hOwner;
	if (NULL != m_hWnd && ::IsWindow(m_hWnd))
	{
		hOwner = m_hWnd;
	}
	else
	{
		hOwner = NULL;
	}
	WinMessageBoxProvider messageBoxProvider(hOwner);
	return messageBoxProvider.DoMessageBox(messageId, buttonType, alertType, defaultButton);
}
