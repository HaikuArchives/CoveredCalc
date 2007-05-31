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
	@file		WinSkinWindow.h
	@brief		WinSkinWindow クラスの定義
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.24 created
*/

#ifndef _WINSKINWINDOW_H_
#define _WINSKINWINDOW_H_

#include "HrnWnd.h"
#include "UIController.h"
#include "WinCCSAppearance.h"

class UIManager;
class WinDialog;

// ---------------------------------------------------------------------
//! スキンウィンドウ for Windows
/*!
	CoveredCalc アプリケーションにおける、スキンを利用するウィンドウは
	このクラスを継承します。
*/
// ---------------------------------------------------------------------
class WinSkinWindow : public CHrnWnd, public UIController
{
public:
	/// ユーザ定義メッセージ
	enum
	{
		UM_ACTIVATED			= WM_USER + 100,	///< ウィンドウがアクティブ/非アクティブ化された
		UM_REREAD_SKIN			= WM_USER + 101,	///< スキンの再読込
	};

public:
							WinSkinWindow();
	virtual					~WinSkinWindow();

	// UIController インタフェースの実装
	virtual ColorCodedSkinAppearance*	InitSkinAppearance();
	virtual void			DisposeSkinAppearance(ColorCodedSkinAppearance* appearance);
	virtual Point32			GetMousePosition() const { readyMousePosition(); return mousePosition; }
	virtual Point32			GetMouseScreenPosition() const;
	virtual void			GetUIRect(Rect32& rect) const;
	virtual void			MoveUI(const Point32& topLeft);
	virtual void			BeginMouseCapture();
	virtual void			EndMouseCapture();
	virtual void			CloseUI();
	virtual bool			IsUIActive() const;
	virtual void			Show();
	virtual void			Hide();
	virtual void			Minimize();
	virtual bool			IsUIMinimized() const;
	virtual void			Restore();
	virtual void			ShowContextMenu(MenuInfo* menuInfo);
	virtual void			ShowDialog(DialogInfo* dialogInfo);
	virtual void			UpdateUI();
	virtual void			Wait(UInt32 milliseconds);
	virtual void			SetToolTipText(ConstAStr text);
	virtual void			SetAlwaysOnTopFlag(bool isFlagOn);
	virtual void			BeginTimer(UIController::TimerID timerId, UInt32 elapse);
	virtual void			EndTimer(UIController::TimerID timerId);

protected:
	virtual UIManager*		createUIManager() = 0;
	virtual void			deleteUIManager(UIManager* uiManager) = 0;
	virtual WinDialog*		createDialogObject(DialogInfo* /*dialogInfo*/) { return NULL; }	// どれも知らない TODO: 例外投げる必要があるかも
	virtual void			deleteDialogObject(WinDialog* dialog);


	UIManager*				getUIManager() { return uiManager; }
	virtual LRESULT			wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void					updateMenuItemStates(HMENU menu, MenuInfo* menuInfo);
	UINT_PTR				timerIdToWinTimerId(UIController::TimerID timerId);
	UIController::TimerID	winTimerIdToTimerId(UINT_PTR winTimerId);
	void					readyMousePosition() const;

	LRESULT					onCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onRButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onUMActivated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onRereadSkin(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	Point32					mousePosition;					//!< イベント発生時のマウス位置を記憶する
	bool					mousePositionAvailable;			///< mousePosition が有効かどうか
	UIManager*				uiManager;						//!< このウィンドウ用のUIマネージャ
	HWND					hToolTipWnd;					///< ツールチップウィンドウ
	AChar					szToolTipText[256];				///< ツールチップテキスト
	SInt32					mouseCaptureNestCount;			///< BeginMouseCapture のネスト回数
	SInt32					timerNestCount[TimerID_Max];	///< 同IDのBeginTimer のネスト回数
	UInt32					timerElapse[TimerID_Max];		///< IDごとのタイマ間隔
};

#endif // _WINSKINWINDOW_H_
