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
	@file		WinSkinWindow.cpp
	@brief		WinSkinWindow クラスの実装
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.24 created
*/

#include "Prefix.h"
#include <windowsx.h>
#include "UIManager.h"
#include "CoveredCalcApp.h"
#include "WinCoveredCalcApp.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "WinSkinWindow.h"
#include "UIControllerException.h"
#include "MenuInfo.h"
#include "WinNormalAppearance.h"
#include "WinLayeredAppearance.h"
#include "MemoryException.h"
#include "CoverManager.h"
#include "CoverDef.h"
#include "CoverInfo.h"
#include "CoverMainWindowInfo.h"

////////////////////////////////////////
#define base	CHrnWnd
////////////////////////////////////////

#define AVAILABLE_AREA_MARGIN		32		//!< 有効な範囲を上下左右それぞれの端からの距離で指定する

#define TIMERID_MOUSEHOVER			100		///< TimerID_MouseHover に対応するタイマID

// ---------------------------------------------------------------------
// ユーザ定義メッセージ
// ---------------------------------------------------------------------
#define UM_ACTIVATED		(WM_USER + 100)		//!< ウィンドウがアクティブ/非アクティブ化された

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinSkinWindow::WinSkinWindow()
{
	uiManager = NULL;
	hToolTipWnd = NULL;
	szToolTipText[0] = '\0';
	mouseCaptureNestCount = 0;
	SInt32 index;
	for (index = 0; index < TimerID_Max; index++)
	{
		timerNestCount[index] = 0;
		timerElapse[index] = 0;
	}
	mousePositionAvailable = false;
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinSkinWindow::~WinSkinWindow()
{
	if (NULL != uiManager)
	{
		ASSERT(false);		// この時点では既に削除されているはず。
		delete uiManager;	// 派生先オブジェクトのデストラクタは既に呼ばれているので deleteUIManager は呼べない。
	}
	ASSERT(0 == mouseCaptureNestCount);
#ifdef _DEBUG
	SInt32 index;
	for (index = 0; index < TimerID_Max; index++)
	{
		ASSERT(0 == timerNestCount[index]);
	}
#endif
}

/**
 *	@brief	ColorCodedSkinAppearance を初期化して返します。
 *	@return	初期化した ColorCodedSkinAppearance オブジェクト。
 */
ColorCodedSkinAppearance* WinSkinWindow::InitSkinAppearance()
{
	const WinLayeredWindowAPI* layeredWindowAPI = WinCoveredCalcApp::GetInstance()->GetLayeredWindowAPI();
	WinCCSAppearance* appearance = NULL;
	if (layeredWindowAPI->IsSupported_UpdateLayeredWindow())
	{
		WinLayeredAppearance* layeredAppearance = new WinLayeredAppearance();
		if (NULL == layeredAppearance)
		{
			MemoryException::Throw();
		}
		appearance = layeredAppearance;
		
		const AppSettings* appSettings = CoveredCalcApp::GetInstance()->GetAppSettings();
		
		const CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
		const CoverMainWindowInfo* mainWindowInfo = manager->GetCoverDef()->GetCoverInfo(manager->GetCurrentCover())->GetMainWindowInfo();
		
		bool ignoreClipping = !mainWindowInfo->IsTransparentRegionUsedAgainstAlpha();
		SInt32 opacity = appSettings->GetMainWindowOpacity();
		SInt32 edgeSmoothingLevel = mainWindowInfo->GetEdgeSmoothingLevel();
		if (edgeSmoothingLevel < 0)
		{
			// auto が指定された場合はアプリケーションのプリファレンスから取得
			edgeSmoothingLevel = appSettings->GetMainWindowEdgeSmoothing();
		}
		layeredAppearance->Init(m_hWnd, ignoreClipping, edgeSmoothingLevel, opacity);
	}
	else
	{
		WinNormalAppearance* normalAppearance = new WinNormalAppearance();
		if (NULL == normalAppearance)
		{
			MemoryException::Throw();
		}
		appearance = normalAppearance;
		
		normalAppearance->Init(m_hWnd);
	}
	return appearance;
}

/**
 *	@brief	不要になった ColorCodedSkinAppearance を捨てます。
 *	@param[in]	appearance	不要になった ColorCodedSkinAppearance オブジェクト。
 */
void WinSkinWindow::DisposeSkinAppearance(ColorCodedSkinAppearance* appearance)
{
	delete appearance;
}

/**
 *	@brief	メッセージを受け取ったときのマウス位置がまだ取得されていなければ取得します.
 */
void WinSkinWindow::readyMousePosition() const
{
	if (!mousePositionAvailable)
	{
		DWORD pos = GetMessagePos();
		POINT winPoint;
		winPoint.x = GET_X_LPARAM(pos);
		winPoint.y = GET_Y_LPARAM(pos);
		::ScreenToClient(m_hWnd, &winPoint);

		WinSkinWindow* nonConstThis = const_cast<WinSkinWindow*>(this);
		nonConstThis->mousePosition.x = winPoint.x;
		nonConstThis->mousePosition.y = winPoint.y;
		nonConstThis->mousePositionAvailable = true;
	}
}

// ---------------------------------------------------------------------
//! マウス位置をスクリーン座標で取得します。
// ---------------------------------------------------------------------
Point32 WinSkinWindow::GetMouseScreenPosition() const
{
	readyMousePosition();

	POINT winPoint;
	winPoint.x = mousePosition.x;
	winPoint.y = mousePosition.y;
	::ClientToScreen(m_hWnd, &winPoint);
	
	Point32 retPoint;
	retPoint.x = winPoint.x;
	retPoint.y = winPoint.y;
	return retPoint;
}

// ---------------------------------------------------------------------
//! UIを囲む矩形の座標（スクリーン座標）を取得します。
// ---------------------------------------------------------------------
void WinSkinWindow::GetUIRect(
	Rect32& rect			//!< OUTPUT. UIを囲む矩形の座標が返る
) const
{
	if (::IsIconic(m_hWnd) || ::IsZoomed(m_hWnd))
	{
		RECT workArea;
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
		WINDOWPLACEMENT windowPlacement;
		ZeroMemory(&windowPlacement, sizeof(windowPlacement));
		windowPlacement.length = sizeof(WINDOWPLACEMENT);
		::GetWindowPlacement(m_hWnd, &windowPlacement);
		rect.left = windowPlacement.rcNormalPosition.left + workArea.left;
		rect.top = windowPlacement.rcNormalPosition.top + workArea.top;
		rect.right = windowPlacement.rcNormalPosition.right + workArea.left;
		rect.bottom = windowPlacement.rcNormalPosition.bottom + workArea.top;
	}
	else
	{
		RECT winRect;
		::GetWindowRect(m_hWnd, &winRect);
		rect.left = winRect.left;
		rect.top = winRect.top;
		rect.right = winRect.right;
		rect.bottom = winRect.bottom;
	}
}

// ---------------------------------------------------------------------
//! UIの位置を移動します。
// ---------------------------------------------------------------------
void WinSkinWindow::MoveUI(
	const Point32& topLeft			//!< UI の左上座標
)
{
	// 完全に画面外に出そうなときは少し画面内に残すようにしてそれ以上移動しないようにする
	// (ただし、透明領域が広すぎるカバーについては知らないーっと)
	RECT winRect;
	::GetWindowRect(m_hWnd, &winRect);
	::OffsetRect(&winRect, topLeft.x - winRect.left, topLeft.y - winRect.top);

	bool isOutOfWorkArea = true;
	const WinMonitorInfo* monInfoStore = WinCoveredCalcApp::GetInstance()->GetMonitorInformation();
	SInt32 monCount = monInfoStore->GetMonitorCount();
	SInt32 monIndex;
	for (monIndex=0; monIndex<monCount; monIndex++)
	{
		const MONITORINFO* info = monInfoStore->GetMonitorInfoAt(monIndex);
		RECT workRect = info->rcWork;
		::InflateRect(&workRect, -(AVAILABLE_AREA_MARGIN), -(AVAILABLE_AREA_MARGIN));
		RECT intersectRect;
		if (::IntersectRect(&intersectRect, &winRect, &workRect))
		{
			isOutOfWorkArea = false;
			break;
		}
	}

	Point32 correctTopLeft = topLeft;
	if (isOutOfWorkArea)
	{
		const MONITORINFO* info = monInfoStore->GetMonitorInfoFromRect(winRect);
		if (NULL != info)
		{
			SInt32 width = winRect.right - winRect.left;
			SInt32 height = winRect.bottom - winRect.top;
		
			RECT workRect = info->rcWork;
			::InflateRect(&workRect, -(AVAILABLE_AREA_MARGIN), -(AVAILABLE_AREA_MARGIN));

			if (correctTopLeft.x >= workRect.right)
			{
				correctTopLeft.x = workRect.right - 1;
			}
			else if (correctTopLeft.x + width < workRect.left)
			{
				correctTopLeft.x = workRect.left - width;
			}
			if (correctTopLeft.y >= workRect.bottom)
			{
				correctTopLeft.y = workRect.bottom -1;
			}
			else if (correctTopLeft.y + height < workRect.top)
			{
				correctTopLeft.y = workRect.top - height;
			}			
		}
	}

	::SetWindowPos(m_hWnd, NULL, correctTopLeft.x, correctTopLeft.y, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}

// ---------------------------------------------------------------------
//! マウスドラッグを開始するときに呼ばれます。
/*!
	ドラッグ中のメッセージをすべて受け取れるように、ウィンドウをキャプチャします。
*/
// ---------------------------------------------------------------------
void WinSkinWindow::BeginMouseCapture()
{
	if (0 == mouseCaptureNestCount)
		::SetCapture(m_hWnd);
	
	mouseCaptureNestCount++;
}

// ---------------------------------------------------------------------
//! マウスドラッグを終了するときに呼ばれます。
/*!
	ドラッグ用のキャプチャを解除します。
*/
// ---------------------------------------------------------------------
void WinSkinWindow::EndMouseCapture()
{
	if (mouseCaptureNestCount > 0)
	{
		mouseCaptureNestCount--;

		if (0 == mouseCaptureNestCount && ::GetCapture() == m_hWnd)
		{
			::ReleaseCapture();
		}
	}
}

// ---------------------------------------------------------------------
//! ウィンドウを閉じます。
// ---------------------------------------------------------------------
void WinSkinWindow::CloseUI()
{
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
}

// ---------------------------------------------------------------------
//! ウィンドウがアクティブかどうかを返します。
/*!
	@retval true アクティブです。
	@retval false アクティブではありません。
*/
// ---------------------------------------------------------------------
bool WinSkinWindow::IsUIActive() const
{
	return (::GetActiveWindow() == m_hWnd) ? true : false;
}

// ---------------------------------------------------------------------
//! ウィンドウを表示します。
// ---------------------------------------------------------------------
void WinSkinWindow::Show()
{
	::ShowWindow(m_hWnd, SW_SHOW);
}

// ---------------------------------------------------------------------
//! ウィンドウを非表示にします。
// ---------------------------------------------------------------------
void WinSkinWindow::Hide()
{
	::ShowWindow(m_hWnd, SW_HIDE);
}

// ---------------------------------------------------------------------
//! ウィンドウを最小化します。
// ---------------------------------------------------------------------
void WinSkinWindow::Minimize()
{
	::ShowWindow(m_hWnd, SW_MINIMIZE);
}

// ---------------------------------------------------------------------
//! 最小化されているかどうかを調べます。
/*!
	@retval true 最小化されています。
	@retval false 最小化されていません。
*/
// ---------------------------------------------------------------------
bool WinSkinWindow::IsUIMinimized() const
{
	return (::IsMinimized(m_hWnd)) ? true : false;
}

// ---------------------------------------------------------------------
//! 最小化されたウィンドウを元に戻します。
// ---------------------------------------------------------------------
void WinSkinWindow::Restore()
{
	::ShowWindow(m_hWnd, SW_RESTORE);
}

// ---------------------------------------------------------------------
//! コンテキストメニューを表示します。
// ---------------------------------------------------------------------
void WinSkinWindow::ShowContextMenu(
	MenuInfo* menuInfo					//!< メニューに関する情報
)
{
	HMENU menu = ::LoadMenu(WinCoveredCalcApp::GetInstance()->GetLangResHandle(), MAKEINTRESOURCE(menuInfo->GetMenuID()));
	if (NULL == menu)
	{
		throw new UIControllerExceptions::FailedToShowContextMenu(menuInfo->GetMenuID());
	}

	try
	{
		HMENU subMenu = ::GetSubMenu(menu, 0);
		if (NULL != subMenu)
		{
			// メニューアイテムの状態を設定
			updateMenuItemStates(subMenu, menuInfo);
			
			// メニューを表示		
			Point32 menuPos = menuInfo->GetMenuPosition();
			::TrackPopupMenu(subMenu, TPM_RIGHTBUTTON, menuPos.x, menuPos.y, 0, m_hWnd, NULL);
		}
		
		::DestroyMenu(menu);
	}
	catch (...)
	{
		::DestroyMenu(menu);
		throw;
	}
}

// ---------------------------------------------------------------------
//! 指定したメニュー内の各アイテムの状態を更新します。
/*!
	@note サブメニューがあれば、その中のアイテムの状態も更新します。
*/
// ---------------------------------------------------------------------
void WinSkinWindow::updateMenuItemStates(
	HMENU menu,				//!< メニューハンドル
	MenuInfo* menuInfo		//!< メニューの状態を取得するための MenuInfo オブジェクト
)
{
	int count = ::GetMenuItemCount(menu);
	int index;
	for (index=0; index<count; index++)
	{
		MENUITEMINFO itemInfo;
		ZeroMemory(&itemInfo, sizeof(itemInfo));
		itemInfo.cbSize = sizeof(MENUITEMINFO);
		itemInfo.fMask = MIIM_ID | MIIM_SUBMENU | MIIM_TYPE;
		::GetMenuItemInfo(menu, index, TRUE, &itemInfo);
		if (NULL != itemInfo.hSubMenu)
		{
			updateMenuItemStates(itemInfo.hSubMenu, menuInfo);
		}
		else if (!(itemInfo.fType & MFT_SEPARATOR))
		{
			bool isEnabled = true;
			bool isChecked = false;
			UInt32 itemStates = menuInfo->GetMenuItemStates(itemInfo.wID);
			if (itemStates & MenuInfo::MenuItemState_Checked)
			{
				isChecked = true;
			}
			if (itemStates & MenuInfo::MenuItemState_Disabled)
			{
				isEnabled = false;
			}

			::EnableMenuItem(menu, index, MF_BYPOSITION | ((isEnabled) ? MF_ENABLED : MF_GRAYED));
			if (itemStates & MenuInfo::MenuItemState_Radio)
			{
				if (isChecked)
				{
					::CheckMenuRadioItem(menu, index, index, index, MF_BYPOSITION);
				}
				else
				{
					::CheckMenuItem(menu, index, MF_BYPOSITION | MF_UNCHECKED);
				}
			}
			else
			{
				::CheckMenuItem(menu, index, MF_BYPOSITION | ((isChecked) ? MF_CHECKED : MF_UNCHECKED));
			}
		}
	}
}

// ---------------------------------------------------------------------
//! ダイアログを表示します。
/*!
	ダイアログを表示します。
	ダイアログがモーダルになるかモードレスになるかは OS によって異なります。
	
	@note Windows 版ではモーダルダイアログで表示します。
*/
// ---------------------------------------------------------------------
void WinSkinWindow::ShowDialog(
	DialogInfo* dialogInfo			//!< 表示するダイアログに関する情報
)
{
	WinDialog* dialog = createDialogObject(dialogInfo);
	if (NULL != dialog)
	{
		dialog->DoModal(m_hWnd);
	}
	deleteDialogObject(dialog);
}

// ---------------------------------------------------------------------
//! createDialogObject で生成した WinDialog オブジェクトを削除します。
// ---------------------------------------------------------------------
void WinSkinWindow::deleteDialogObject(
	WinDialog* dialog				//!< ダイアログオブジェクト
)
{
	delete dialog;
}

// ---------------------------------------------------------------------
//! すぐにウィンドウを更新します。（無効領域をなくします）
// ---------------------------------------------------------------------
void WinSkinWindow::UpdateUI()
{
	::UpdateWindow(m_hWnd);
}

// ---------------------------------------------------------------------
//! 指定した期間だけ待機します。
// ---------------------------------------------------------------------
void WinSkinWindow::Wait(
	UInt32 milliseconds		//!< 待機する期間 (ミリ秒)
)
{
	::Sleep(milliseconds);
}

/**
 *	@brief	ツールチップテキストとして表示する文字列を設定します。
 */
void WinSkinWindow::SetToolTipText(
	ConstAStr text			///< ツールチップテキスト
)
{
	lstrcpyn(szToolTipText, text, sizeof(szToolTipText)/sizeof(AChar));
	
	TOOLINFO ti;
	ZeroMemory(&ti, sizeof(ti));
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = 0;
	ti.hwnd = m_hWnd;
	ti.uId = 1;

	// 前のを削除
	::SendMessage(hToolTipWnd, TTM_DELTOOL, 0, reinterpret_cast<LPARAM>(&ti));
	
	// 新しく追加
	if (szToolTipText[0] != '\0')
	{
		::GetClientRect(m_hWnd, &(ti.rect));
		ti.lpszText = szToolTipText;
		::SendMessage(hToolTipWnd, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));
	}
}

/**
 *	@brief	常に手前に表示フラグをセット/リセットします。
 */
void WinSkinWindow::SetAlwaysOnTopFlag(
	bool isFlagOn			///< セットするならtrue, リセットするならfalse
)
{
	if (isFlagOn)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);	
	}
	else
	{
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
	}
}

/**
 *	@brief	UIController::TimerID の値に対応するタイマIDを返します。
 *	@param	timerId		UIController::TimerID の値
 *	@return	SetTimer Win32API にセットするタイマID
 */
UINT_PTR WinSkinWindow::timerIdToWinTimerId(UIController::TimerID timerId)
{
	switch (timerId)
	{
	case TimerID_MouseHover:
		return TIMERID_MOUSEHOVER;
	default:
		ASSERT(false);
		return 0;
	}
}

/**
 *	@brief	タイマIDに対応する UIController::TimerID の値を返します。
 *	@param	winTimerId	SetTimer Win32API に渡したタイマID
 *	@return	UIController::TimerID の値
 */
UIController::TimerID WinSkinWindow::winTimerIdToTimerId(UINT_PTR winTimerId)
{
	switch (winTimerId)
	{
	case TIMERID_MOUSEHOVER:
		return TimerID_MouseHover;
	default:
		ASSERT(false);
		return TimerID_Max;
	}
}

/**
 *	@brief	タイマを開始します。
 *	@param	timerId	タイマID
 *	@param	elapse	時間間隔
 */
void WinSkinWindow::BeginTimer(UIController::TimerID timerId, UInt32 elapse)
{
	if (timerNestCount[timerId] == 0 || timerElapse[timerId] != elapse)
	{
		SetTimer(m_hWnd, timerIdToWinTimerId(timerId), elapse, NULL);
		timerElapse[timerId] = elapse;
	}
	timerNestCount[timerId]++;
}

/**
 *	@brief	タイマを終了します。
 *	@param	timerId	タイマID
 */
void WinSkinWindow::EndTimer(UIController::TimerID timerId)
{
	if (timerNestCount[timerId] > 0)
	{
		timerNestCount[timerId]--;
		if (timerNestCount[timerId] == 0)
		{
			KillTimer(m_hWnd, timerIdToWinTimerId(timerId));
		}
	}
}

// ---------------------------------------------------------------------
//! ウィンドウプロシージャ
/*!
	@return 各メッセージに対応する戻り値
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::wndProc(
	HWND hWnd,			// ウィンドウハンドル
	UINT uMsg,			// メッセージ
	WPARAM wParam,		// メッセージの WPARAM
	LPARAM lParam		// メッセージの LPARAM
)
{
	LRESULT ret = 0;
	bool isProcessed = false;
	if (uiManager != NULL)
	{
		WinCCSAppearance* appearance = static_cast<WinCCSAppearance*>(uiManager->GetSkinAppearance());
		if (appearance != NULL)
		{
			isProcessed = appearance->RelayWndProc(hWnd, uMsg, wParam, lParam, &ret);
		}
	}

	mousePositionAvailable = false;

	try
	{
		// ツールチップウィンドウへのリレー
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			{
				DWORD posVal;
				MSG msg;
				posVal = GetMessagePos();
				msg.hwnd = hWnd;
				msg.message = uMsg;
				msg.wParam = wParam;
				msg.lParam = lParam;
				msg.time = GetMessageTime();
				msg.pt.x = LOWORD(posVal);
				msg.pt.y = HIWORD(posVal);
				::SendMessage(hToolTipWnd, TTM_RELAYEVENT, 0, reinterpret_cast<LPARAM>(&msg));
			}
			break;
		}
	
		// 各メッセージのハンドリング
		switch (uMsg)
		{
		case WM_CREATE:
			return onCreate(hWnd, uMsg, wParam, lParam);
			break;
		case WM_DESTROY:
			return onDestroy(hWnd, uMsg, wParam, lParam);
			break;
		case WM_MOUSEMOVE:
			return onMouseMove(hWnd, uMsg, wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
			return onLButtonDown(hWnd, uMsg, wParam, lParam);
			break;
		case WM_LBUTTONUP:
			return onLButtonUp(hWnd, uMsg, wParam, lParam);
			break;
		case WM_RBUTTONDOWN:
			return onRButtonDown(hWnd, uMsg, wParam, lParam);
			break;
		case WM_RBUTTONUP:
			return onRButtonUp(hWnd, uMsg, wParam, lParam);
			break;
		case WM_ACTIVATE:
			return onActivate(hWnd, uMsg, wParam, lParam);
			break;
		case UM_ACTIVATED:
			return onUMActivated(hWnd, uMsg, wParam, lParam);
			break;
		case WM_KEYDOWN:
			return onKeyDown(hWnd, uMsg, wParam, lParam);
			break;
		case WM_KEYUP:
			return onKeyUp(hWnd, uMsg, wParam, lParam);
			break;
		case WM_TIMER:
			return onTimer(hWnd, uMsg, wParam, lParam);
			break;
		default:
			if (isProcessed)
			{
				return ret;
			}
			else
			{
				return base::wndProc(hWnd, uMsg, wParam, lParam);
			}
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
LRESULT WinSkinWindow::onCreate(
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

	// ツールチップウィンドウの作成
	hToolTipWnd = ::CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
						WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
						CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT,
						hWnd, NULL, CHrnApp::GetAppObject()->GetInstanceHandle(),
						NULL);
	::SendMessage(hToolTipWnd, TTM_ACTIVATE, static_cast<WPARAM>(TRUE), 0);
	
	// UI マネージャを作成
	if (NULL != uiManager)
	{
		deleteUIManager(uiManager);
	}

	try
	{
		uiManager = createUIManager();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		ex->Delete();
		return -1;
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_DESTROY ハンドラ
/*!
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onDestroy(
	HWND hWnd,		// ウィンドウハンドル
	UINT uMsg,		// WM_DESTROY
	WPARAM wParam,	// 利用しないパラメータ
	LPARAM lParam	// 利用しないパラメータ
)
{
	// マウスキャプチャが残っていたら解除
	if (mouseCaptureNestCount > 0)
	{
		if (GetCapture() == m_hWnd)
		{
			ReleaseCapture();
		}
		mouseCaptureNestCount = 0;
	}
	// タイマが残っていたら終了
	SInt32 index;
	for (index = 0; index < TimerID_Max; index++)
	{
		if (timerNestCount[index] > 0)
		{
			KillTimer(m_hWnd, timerIdToWinTimerId(static_cast<TimerID>(index)));
			timerNestCount[index] = 0;
		}
	}

	base::wndProc(hWnd, uMsg, wParam, lParam);

	if (NULL != uiManager)
	{
		deleteUIManager(uiManager);
		uiManager = NULL;
	}
	
	::DestroyWindow(hToolTipWnd);
	hToolTipWnd = NULL;
	
	return 0;
}

// ---------------------------------------------------------------------
//! WM_MOUSEMOVE ハンドラ
/*!
	UIManager にイベントを伝えます。
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onMouseMove(
	HWND hWnd,		//!< ウィンドウハンドル
	UINT uMsg,		//!< WM_MOUSEMOVE
	WPARAM wParam,	//!< 様々な仮想キーが押されているかどうのフラグ
	LPARAM lParam	//!< 下位ワードがマウスカーソルの X 座標、上位ワードが Y 座標
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	mousePosition.x = GET_X_LPARAM(lParam);
	mousePosition.y = GET_Y_LPARAM(lParam);
	mousePositionAvailable = true;
	
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->MouseMove();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_LBUTTONDOWN ハンドラ
/*!
	UIManager にイベントを伝えます。
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT	WinSkinWindow::onLButtonDown(
	HWND hWnd,		//!< ウィンドウハンドル
	UINT uMsg,		//!< WM_LBUTTONDOWN
	WPARAM wParam,	//!< 様々な仮想キーが押されているかどうのフラグ
	LPARAM lParam	//!< 下位ワードがマウスカーソルの X 座標、上位ワードが Y 座標
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	mousePosition.x = GET_X_LPARAM(lParam);
	mousePosition.y = GET_Y_LPARAM(lParam);
	mousePositionAvailable = true;

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->Button1Down();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_LBUTTONUP ハンドラ
/*!
	UIManager にイベントを伝えます。
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT	WinSkinWindow::onLButtonUp(
	HWND hWnd,		//!< ウィンドウハンドル
	UINT uMsg,		//!< WM_LBUTTONUP
	WPARAM wParam,	//!< 様々な仮想キーが押されているかどうのフラグ
	LPARAM lParam	//!< 下位ワードがマウスカーソルの X 座標、上位ワードが Y 座標
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	mousePosition.x = GET_X_LPARAM(lParam);
	mousePosition.y = GET_Y_LPARAM(lParam);
	mousePositionAvailable = true;

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->Button1Up();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_RBUTTONDOWN ハンドラ
/*!
	UIManager にイベントを伝えます。
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT	WinSkinWindow::onRButtonDown(
	HWND hWnd,		//!< ウィンドウハンドル
	UINT uMsg,		//!< WM_RBUTTONDOWN
	WPARAM wParam,	//!< 様々な仮想キーが押されているかどうのフラグ
	LPARAM lParam	//!< 下位ワードがマウスカーソルの X 座標、上位ワードが Y 座標
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	mousePosition.x = GET_X_LPARAM(lParam);
	mousePosition.y = GET_Y_LPARAM(lParam);
	mousePositionAvailable = true;

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->Button2Down();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_RBUTTONUP ハンドラ
/*!
	UIManager にイベントを伝えます。
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT	WinSkinWindow::onRButtonUp(
	HWND hWnd,		//!< ウィンドウハンドル
	UINT uMsg,		//!< WM_RBUTTONUP
	WPARAM wParam,	//!< 様々な仮想キーが押されているかどうのフラグ
	LPARAM lParam	//!< 下位ワードがマウスカーソルの X 座標、上位ワードが Y 座標
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	mousePosition.x = GET_X_LPARAM(lParam);
	mousePosition.y = GET_Y_LPARAM(lParam);
	mousePositionAvailable = true;

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->Button2Up();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_ACTIVATE ハンドラ
/*!
	UM_ACTIVATED を自分自身にポストします。
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onActivate(
	HWND hWnd,		//!< ウィンドウハンドル
	UINT uMsg,		//!< WM_ACTIVATE
	WPARAM wParam,	//!< 下位ワードがアクティブ化されるか非アクティブ化されるかを示し、上位ワードがウィンドウが最小化されているかどうかを示す
	LPARAM lParam	//!< アクティブ/非アクティブになるウィンドウのハンドル
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	// モーダルダイアログが出ているときの対処
	bool willBeActive = (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) ? true : false;
	if (willBeActive && !::IsWindowEnabled(m_hWnd))
	{
		// たぶん、モーダルダイアログが出ている状態でアクティブにしようとしたので、
		// ダイアログの方をアクティブにする
		WinCoveredCalcApp::GetInstance()->ActivateModalDialog();
		
		// 自分自身はアクティブにならない（なかったことにする）
		return 0;
	}

	// このメッセージの時点では、実際にアクティブ化/非アクティブ化は
	// まだされていないので、UM_ACTIVATED を自分自身にポストしておいて
	// そちらのハンドラで UIManager にイベントを伝える
	::PostMessage(m_hWnd, UM_ACTIVATED, 0, 0);

	return 0;
}

// ---------------------------------------------------------------------
//! UM_ACTIVATED ハンドラ
/*!
	UIManager にイベントを伝えます。
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onUMActivated(
	HWND /*hWnd*/,		//!< ウィンドウハンドル
	UINT /*uMsg*/,		//!< UM_ACTIVATED
	WPARAM /*wParam*/,	//!< 利用しないパラメータ
	LPARAM /*lParam*/	//!< 利用しないパラメータ
)
{
	UIManager* uiManager = getUIManager();
	if (NULL != uiManager)
	{
		uiManager->UIActivated();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_KEYDOWN ハンドラ
/*!
	UIManager にイベントを伝えます。
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onKeyDown(
	HWND hWnd,		//!< ウィンドウハンドル
	UINT uMsg,		//!< WM_KEYDOWN
	WPARAM wParam,	//!< 仮想キーコード
	LPARAM lParam	//!< フラグ
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	// マウスボタン、修飾キーは無視
	switch (wParam)
	{
	case VK_LBUTTON:
	case VK_RBUTTON:
	case VK_MBUTTON:
	case VK_SHIFT:
	case VK_CONTROL:
	case VK_MENU:
		return 0;
	}

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		KeyEventParameter parameter;
		parameter.SetVirtualKeyCode(static_cast<DWORD>(wParam));
		parameter.SetFlags(static_cast<DWORD>(lParam));
		parameter.SetShiftPressed((::GetKeyState(VK_SHIFT) & 0x8000) ? true : false);
		parameter.SetCtrlPressed((::GetKeyState(VK_CONTROL) & 0x8000) ? true : false);
		parameter.SetAltPressed((::GetKeyState(VK_MENU) & 0x8000) ? true : false);
		uiManager->KeyDown(parameter);
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_KEYUP ハンドラ
/*!
	UIManager にイベントを伝えます。
	@retval 0 処理したとき
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onKeyUp(
	HWND hWnd,		//!< ウィンドウハンドル
	UINT uMsg,		//!< WM_KEYUP
	WPARAM wParam,	//!< 仮想キーコード
	LPARAM lParam	//!< フラグ
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	// マウスボタンは無視
	switch (wParam)
	{
	case VK_LBUTTON:
	case VK_RBUTTON:
	case VK_MBUTTON:
	case VK_SHIFT:
	case VK_CONTROL:
	case VK_MENU:
		return 0;
	}

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		KeyEventParameter parameter;
		parameter.SetVirtualKeyCode(static_cast<DWORD>(wParam));
		parameter.SetFlags(static_cast<DWORD>(lParam));
		parameter.SetShiftPressed((::GetKeyState(VK_SHIFT) & 0x8000) ? true : false);
		parameter.SetCtrlPressed((::GetKeyState(VK_CONTROL) & 0x8000) ? true : false);
		parameter.SetAltPressed((::GetKeyState(VK_MENU) & 0x8000) ? true : false);
		uiManager->KeyUp(parameter);
	}

	return 0;
}

/**
 *	@brief	WM_TIMER ハンドラ
 *
 *	UIManager にイベントを伝えます。
 *	@retval	0	処理したとき
 */
LRESULT WinSkinWindow::onTimer(
   	HWND hWnd,		///< ウィンドウハンドル
	UINT uMsg,		///< WM_TIMER
	WPARAM wParam,	///< タイマID
	LPARAM lParam	///< コールバック関数
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->TimerEvent(winTimerIdToTimerId(static_cast<UINT_PTR>(wParam)));
	}

	return 0;
}
