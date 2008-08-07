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
 *	@brief	モーダルダイアログを表示します。
 *	@return	ダイアログの戻り値
 */
int WinDialog::DoModal(
	HWND hParent	///< 親ウィンドウ
)
{
	return base::DoModal(hParent, WinCoveredCalcApp::GetInstance()->GetLangResHandle());
}

/**
 *	@brief	モードレスダイアログを生成します。・
 *	@retval	TRUE	成功
 *	@retval	FALSE	失敗
 */
BOOL WinDialog::Create(
	HWND hParent	///< 親ウィンドウ
)
{
	return base::Create(hParent, WinCoveredCalcApp::GetInstance()->GetLangResHandle());
}

// ---------------------------------------------------------------------
//! ウィンドウプロシージャ
/*!
	@return 各メッセージに対応する戻り値
*/
// ---------------------------------------------------------------------
LRESULT WinDialog::wndProc(
	HWND hWnd,			//!< ウィンドウハンドル
	UINT uMsg,			//!< メッセージ
	WPARAM wParam,		//!< メッセージの WPARAM
	LPARAM lParam		//!< メッセージの LPARAM
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
//! WM_INITDIALOG ハンドラ
/*!
	@retval TRUE wParam で指定されたコントロールにフォーカスを与えてください。
	@retval FALSE フォーカスを与えないでください。
*/
// ---------------------------------------------------------------------
LRESULT WinDialog::onInitDialog(
	HWND hWnd,			//!< ウィンドウハンドル
	UINT uMsg,			//!< WM_INITDIALOG
	WPARAM wParam,		//!< handle to the control to receive the default keyboard focus
	LPARAM lParam		//!< additional initialization data
)
{
	LRESULT result = base::wndProc(hWnd, uMsg, wParam, lParam);
	
	// 画面外にはみ出そうなら補正する
	const WinMonitorInfo* monitorInfo = WinCoveredCalcApp::GetInstance()->GetMonitorInformation();
	const MONITORINFO* monInfo = monitorInfo->GetMonitorInfoFromWindow(m_hWnd);
	if (NULL != monInfo)
	{
		RECT windowRect;
		::GetWindowRect(m_hWnd, &windowRect);
		
		LONG delta;
		bool moved = false;
		
		// 下端
		delta = windowRect.bottom - monInfo->rcWork.bottom;
		if (delta > 0)
		{
			::OffsetRect(&windowRect, 0, -delta);
			moved = true;
		}
		
		// 右端
		delta = windowRect.right - monInfo->rcWork.right;
		if (delta > 0)
		{
			::OffsetRect(&windowRect, -delta, 0);
			moved = true;
		}
		
		// 上端
		delta = windowRect.top - monInfo->rcWork.top;
		if (delta < 0)
		{
			::OffsetRect(&windowRect, 0, -delta);
			moved = true;
		}
		
		// 左端
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
//! メッセージボックスを表示します。
/*!
	@return ユーザが選択したボタン
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
//! メッセージボックスを表示します。
/*!
	@return ユーザが選択したボタン
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
