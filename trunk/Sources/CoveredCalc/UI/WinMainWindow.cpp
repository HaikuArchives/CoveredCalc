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
	@file		WinMainWindow.cpp
	@brief		WinMainWindow クラスの実装
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.24 created
*/

#include "Prefix.h"
#include "WinCoveredCalcApp.h"
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
const AChar WinMainWindow::windowClassName[] = ALITERAL("CoveredCalcMainWindowClass");

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
		uiManager->Init(this, this, CoveredCalcApp::GetInstance()->GetKeyMappingManagerForMainWindow());
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

/**
 *	@brief	Shows main ui context menu.
 *	@param[in]	menuPos	position of the menu (in screen coordinates).
 */
void WinMainWindow::ShowMainUIContextMenu(Point32 menuPos)
{
		showContextMenu(IDM_MAIN_CONTEXT, menuPos);
}

/**
 *	@brief	Returns menu item's command.
 *	@param[in] menuID	menu ID.
 *	@return	command id is returned.
 */
SInt32 WinMainWindow::getMenuCommand(UINT menuID)
{
	SInt32 commandID;

#define HANDLE_MENU_ID(menu, buttonClass)		\
	case menu:									\
		commandID = buttonClass;				\
		break;

	switch (menuID)
	{
		HANDLE_MENU_ID(ID_COVER_BROWSER, CoverMainWindowInfo::ButtonClass_ShowHideCoverBrowser)
		HANDLE_MENU_ID(ID_RADIX_HEX, CoverMainWindowInfo::ButtonClass_Hex)
		HANDLE_MENU_ID(ID_RADIX_DECIMAL, CoverMainWindowInfo::ButtonClass_Dec)
		HANDLE_MENU_ID(ID_RADIX_OCTAL, CoverMainWindowInfo::ButtonClass_Oct)
		HANDLE_MENU_ID(ID_RADIX_BINARY, CoverMainWindowInfo::ButtonClass_Bin)
		HANDLE_MENU_ID(ID_MAIN_ALWAYS_ON_TOP, CoverMainWindowInfo::ButtonClass_ToggleAlwaysOnTop)
		HANDLE_MENU_ID(ID_MAIN_LOCK_POS, CoverMainWindowInfo::ButtonClass_ToggleLockPos)
		HANDLE_MENU_ID(ID_PREFERENCES, CoverMainWindowInfo::ButtonClass_ShowPreferencesDialog)
		HANDLE_MENU_ID(ID_MAIN_ABOUT_COVER, CoverMainWindowInfo::ButtonClass_ShowCurrentCoverInfo)
		HANDLE_MENU_ID(ID_ABOUT, CoverMainWindowInfo::ButtonClass_About)
		HANDLE_MENU_ID(ID_MAIN_MINIMIZE, CoverMainWindowInfo::ButtonClass_Minimize)
		HANDLE_MENU_ID(IDCLOSE, CoverMainWindowInfo::ButtonClass_Close)
		HANDLE_MENU_ID(ID_MAIN_CLOSE, CoverMainWindowInfo::ButtonClass_Close)
	
	default:
		commandID = UIManager::Command_None;
		break;
	}

	return commandID;
}

/**
 *	@brief	Shows "About" dialog.
 */
void WinMainWindow::ShowAboutDialog()
{
	WinAboutDlg dlg;
	dlg.DoModal(m_hWnd);
}

/**
 *	@brief	Shows "About Current Cover" dialog.
 */
void WinMainWindow::ShowAboutCurrentCoverDialog()
{
	WinAboutCurrentCoverDlg dlg;
	dlg.DoModal(m_hWnd);
}

/**
 *	@brief	Shows "Preferences" dialog.
 */
void WinMainWindow::ShowPreferencesDialog()
{
	WinPreferencesDlg dlg;
	dlg.DoModal(m_hWnd);
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
