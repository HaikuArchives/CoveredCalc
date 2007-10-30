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
	@brief		WinMainWindow クラスの実装
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
//! メインウィンドウのウィンドウクラス名
// ---------------------------------------------------------------------
const AChar WinMainWindow::windowClassName[] = "CoveredCalcMainWindowClass";

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinMainWindow::WinMainWindow()
{
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinMainWindow::~WinMainWindow()
{
}

// ---------------------------------------------------------------------
//! このウィンドウのウィンドウクラスを登録します。
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
//! このウィンドウ用の UIManager オブジェクトを生成します。
/*!
	@return UIManager オブジェクト
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
//! このウィンドウ用の UIManager オブジェクトを削除します。
/*!
	@return UIManager オブジェクト
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
//! 指定されたダイアログに対応する WinDialog オブジェクトを生成します。
/*!
	@return WinDialog オブジェクト
	@throw UIControllerExceptions::FailedToShowDialog オブジェクトが生成できなかったとき
*/
// ---------------------------------------------------------------------
WinDialog* WinMainWindow::createDialogObject(
	DialogInfo* dialogInfo					//!< ダイアログの情報
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
//! ウィンドウプロシージャ
/*!
	@return 各メッセージに対応する戻り値
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::wndProc(
	HWND hWnd,			// ウィンドウハンドル
	UINT uMsg,			// メッセージ
	WPARAM wParam,		// メッセージの WPARAM
	LPARAM lParam		// メッセージの LPARAM
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
//! WM_CREATE ハンドラ
/*!
	@retval 0 成功
	@retval -1 失敗
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::onCreate(
	HWND hWnd,		// ウィンドウハンドル
	UINT uMsg,		// WM_CREATE
	WPARAM wParam,	// 利用しないパラメータ
	LPARAM lParam	// ウィンドウ作成情報を持つ CREATESTRUCT 構造体へのポインタ
)
{
	if (-1 == base::wndProc(hWnd, uMsg, wParam, lParam))
	{
		return -1;
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_CLOSE ハンドラ
/*!
	メインウィンドウが閉じられるときにアプリケーション自身を終了させます。
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::onClose(
	HWND /*hWnd*/,		// ウィンドウハンドル
	UINT /*uMsg*/,		// WM_DESTROY
	WPARAM /*wParam*/,	// 利用しないパラメータ
	LPARAM /*lParam*/	// 利用しないパラメータ
)
{
	// 親のウィンドウプロシージャは呼び出さない（DestroyWindowは WinCoveredCalcApp::Quit() に呼ばせる）
	WinCoveredCalcApp::GetInstance()->Quit();	
	return 0;
}

// ---------------------------------------------------------------------
//! WM_ENABLE ハンドラ
/*!
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::onEnable(
	HWND hWnd,			// ウィンドウハンドル
	UINT uMsg,			// WM_ENABLE
	WPARAM wParam,		// TRUE なら有効になる / FALSE なら無効になる
	LPARAM lParam		// 利用しないパラメータ
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	bool isEnabled = (wParam) ? true : false;
	WinCoveredCalcApp::GetInstance()->EnableCoveredCalcWindows(isEnabled);
	return 0;
}

// ---------------------------------------------------------------------
//! WM_COMMAND ハンドラ
/*!
	@retval 0 このメッセージを処理した
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::onCommand(
	HWND hWnd,			//!< ウィンドウハンドル
	UINT uMsg,			//!< WM_COMMAND
	WPARAM wParam,		//!< 上位ワードが通知コード、下位ワードがコマンドID
	LPARAM lParam		//!< このメッセージを送ったコントロールのハンドル
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
//! WM_DISPLAYCHANGE ハンドラ
/*!
	@retval 0 常に
*/
// ---------------------------------------------------------------------
LRESULT WinMainWindow::onDisplayChange(
	HWND hWnd,			//!< ウィンドウハンドル
	UINT uMsg,			//!< WM_DISPLAYCHANGE
	WPARAM wParam,		//!< bit per pixel で表した image depth
	LPARAM lParam		//!< 下位ワードが水平方向の解像度、上位ワードが垂直方向の解像度
)
{
	// モニタ情報を最新に更新
	WinCoveredCalcApp::GetInstance()->GetMonitorInformation()->Update();

	return base::wndProc(hWnd, uMsg, wParam, lParam);
}
