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

#define BACK_COLOR			RGB(0xff, 0xff, 0xff)		///< 背景色
#define TEXT_COLOR			RGB(0x00, 0x40, 0x80)		///< テキスト色

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinAboutDlg::WinAboutDlg() : base(IDD_ABOUT)
{
	backBrush = NULL;
	iconImage = NULL;
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinAboutDlg::~WinAboutDlg()
{
	// 背景ブラシが残っていれば削除
	if (NULL != backBrush)
	{
		::DeleteObject(backBrush);
	}
}

// ---------------------------------------------------------------------
//! ウィンドウプロシージャ
/*!
	@return 各メッセージに対応する戻り値
*/
// ---------------------------------------------------------------------
LRESULT WinAboutDlg::wndProc(
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
//! WM_INITDIALOG ハンドラ
/*!
	@retval TRUE wParam で指定されたコントロールにフォーカスを与えてください。
	@retval FALSE フォーカスを与えないでください。
*/
// ---------------------------------------------------------------------
LRESULT WinAboutDlg::onInitDialog(
	HWND hWnd,			//!< ウィンドウハンドル
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

	// TODO: フォーカス

	// 背景ブラシを作成
	backBrush = ::CreateSolidBrush(BACK_COLOR);

	// バージョンをセット
	HWND hVersionStatic = ::GetDlgItem(m_hWnd, IDC_STC_VERSION);
	if (NULL != hVersionStatic)
	{
		MBCString versionMessage;
		makeVersionString(versionMessage);
		::SetWindowText(hVersionStatic, versionMessage);
	}

	// アプリケーション名をセット
	::SetDlgItemText(m_hWnd, IDC_STC_APPNAME, ApplicationTitleString);
	
	// 著作権情報をセット
	::SetDlgItemText(m_hWnd, IDC_STC_COPYRIGHT1, CopyrightString1);
	
	// アイコンイメージをセット
	HWND iconImageWnd = GetDlgItem(hWnd, IDC_ICON64);
	if (NULL != iconImageWnd)
	{
		iconImage = LoadBitmap(WinCoveredCalcApp::GetInstance()->GetInstanceHandle(), MAKEINTRESOURCE(IDB_APPICON64));
		SendMessage(iconImageWnd, STM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(iconImage));
	}
	
	return TRUE;
}

// ---------------------------------------------------------------------
//! WM_DESTROY ハンドラ
/*!
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT WinAboutDlg::onDestroy(
	HWND hWnd,		//!< ウィンドウハンドル
	UINT uMsg,		//!< WM_DESTROY
	WPARAM wParam,	//!< 利用しないパラメータ
	LPARAM lParam	//!< 利用しないパラメータ
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);
	
	// 背景ブラシを削除
	if (NULL != backBrush)
	{
		::DeleteObject(backBrush);
		backBrush = NULL;	
	}

	// アイコンイメージを削除
	if (NULL != iconImage)
	{
		::DeleteObject(iconImage);
		iconImage = NULL;
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_ERASEBKGND ハンドラ
/*!
	@retval !0	背景を消去したとき
	@retval	0	背景を消去しなかったとき
*/
// ---------------------------------------------------------------------
LRESULT WinAboutDlg::onEraseBkGnd(
	HWND hWnd,			//!< ウィンドウハンドル
	UINT /*uMsg*/,		//!< WM_ERASEBKGND
	WPARAM wParam,		//!< デバイスコンテキスト
	LPARAM /*lParam*/	//!< 利用しないパラメータ
)
{
	HDC dc = reinterpret_cast<HDC>(wParam);

	RECT clientRect;
	::GetClientRect(hWnd, &clientRect);
	
	::FillRect(dc, &clientRect, backBrush);

	return TRUE;
}

// ---------------------------------------------------------------------
//! WM_CTLCOLORSTATIC ハンドラ
/*!
	@return 背景を消去するためのブラシを返します。
*/
// ---------------------------------------------------------------------
LRESULT WinAboutDlg::onCtlColorStatic(
	HWND /*hWnd*/,		//!< ウィンドウハンドル
	UINT /*uMsg*/,		//!< WM_CTLCOLORSTATIC
	WPARAM wParam,		//!< デバイスコンテキスト
	LPARAM /*lParam*/	//!< スタティックコントロールのウィンドウハンドル
)
{
	HDC dc = reinterpret_cast<HDC>(wParam);
	
	::SetBkColor(dc, BACK_COLOR);
	::SetTextColor(dc, TEXT_COLOR);
	return reinterpret_cast<LRESULT>(backBrush);
}

// ---------------------------------------------------------------------
//! バージョン文字列を作成します。
// ---------------------------------------------------------------------
void WinAboutDlg::makeVersionString(
	MBCString& versionString			//!< OUTPUT. ここに作成した文字列が返される
)
{
	versionString = ALITERAL("Version ");
	TCHAR fileName[MAX_PATH];

	// ファイル名を取得
	::GetModuleFileName(NULL, fileName, sizeof(fileName) / sizeof(TCHAR));

	// 情報ブロックの実際のサイズを取得する
	DWORD handle;
	DWORD dwBytes = ::GetFileVersionInfoSize(fileName, &handle);
	if (0 != dwBytes)
	{
		// バージョン情報の実際のブロックを取得
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
						TCHAR subBlock[64];	// 本当はこんなにいらないけど、まあ余裕をみて。
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

