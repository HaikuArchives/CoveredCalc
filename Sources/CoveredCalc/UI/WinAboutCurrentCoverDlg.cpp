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
	@file		WinAboutCurrentCoverDlg.cpp
	@brief		Implementation of WinAboutCurrentCoverDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.06 created
*/

#include "Prefix.h"
#include "WinAboutCurrentCoverDlg.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcApp.h"
#include "UTF8Conv.h"
#include "CoverDef.h"
#include "Exception.h"

////////////////////////////////////////
#define base	WinDialog
////////////////////////////////////////

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinAboutCurrentCoverDlg::WinAboutCurrentCoverDlg() : base(IDD_ABOUT_COVER)
{
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinAboutCurrentCoverDlg::~WinAboutCurrentCoverDlg()
{
}

// ---------------------------------------------------------------------
//! カバーデータをダイアログに表示します。
// ---------------------------------------------------------------------
void WinAboutCurrentCoverDlg::setDataToDialog(
	const CoverDef* coverDef			///< 表示するデータ
)
{
	if (NULL == coverDef)
	{
		SetDlgItemText(m_hWnd, IDC_EDIT_NAME, "");
		SetDlgItemText(m_hWnd, IDC_EDIT_DESCRIPTION, "");
		SetDlgItemText(m_hWnd, IDC_EDIT_ABOUT, "");
	}
	else
	{
		MBCString mbcString;
		UTF8Conv::ToMultiByte(mbcString, coverDef->GetTitle());
		SetDlgItemText(m_hWnd, IDC_EDIT_NAME, mbcString);
		UTF8Conv::ToMultiByte(mbcString, coverDef->GetDescription());
		SetDlgItemText(m_hWnd, IDC_EDIT_DESCRIPTION, mbcString);
		UTF8Conv::ToMultiByteWithLineEnding(mbcString, coverDef->GetAbout());
		SetDlgItemText(m_hWnd, IDC_EDIT_ABOUT, mbcString);
	}
}

// ---------------------------------------------------------------------
//! ウィンドウプロシージャ
/*!
	@return 各メッセージに対応する戻り値
*/
// ---------------------------------------------------------------------
LRESULT WinAboutCurrentCoverDlg::wndProc(
	HWND hWnd,			//!< ウィンドウハンドル
	UINT uMsg,			//!< メッセージ
	WPARAM wParam,		//!< メッセージの WPARAM
	LPARAM lParam		//!< メッセージの LPARAM
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

// ---------------------------------------------------------------------
//! WM_INITDIALOG ハンドラ
/*!
	@retval TRUE wParam で指定されたコントロールにフォーカスを与えてください。
	@retval FALSE フォーカスを与えないでください。
*/
// ---------------------------------------------------------------------
LRESULT WinAboutCurrentCoverDlg::onInitDialog(
	HWND hWnd,			//!< ウィンドウハンドル
	UINT uMsg,			//!< WM_INITDIALOG
	WPARAM wParam,		//!< handle to the control to receive the default keyboard focus
	LPARAM lParam		//!< additional initialization data
)
{
	LRESULT ret = base::wndProc(hWnd, uMsg, wParam, lParam);
	initialize();
	return ret;
}

// ---------------------------------------------------------------------
//! WM_DESTROY ハンドラ
/*!
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT WinAboutCurrentCoverDlg::onDestroy(
	HWND hWnd,		//!< ウィンドウハンドル
	UINT uMsg,		//!< WM_DESTROY
	WPARAM wParam,	//!< 利用しないパラメータ
	LPARAM lParam	//!< 利用しないパラメータ
)
{
	terminate();
	return base::wndProc(hWnd, uMsg, wParam, lParam);
}
