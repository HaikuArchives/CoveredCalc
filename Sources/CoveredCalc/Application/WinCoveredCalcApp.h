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
	@file		WinCoveredCalcApp.h
	@brief		WinCoveredCalcApp クラスの定義
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.22 created
*/

#ifndef _COVEREDCALCWINAPP_H_
#define _COVEREDCALCWINAPP_H_

#include "HrnApp.h"
#include "CoveredCalcAppBase.h"
#include "WinMainWindow.h"
#include "WinCoverBrowser.h"
#include "WinMonitorInfo.h"
#include "WinLayeredWindowAPI.h"

class WinMessageFilter;

// ---------------------------------------------------------------------
//! CoveredCalc Windows 版 アプリケーションクラス
/*!
	CoveredCalc Windows 版のアプリケーション全体を表します。
	このクラスはシングルトンであり、唯一のオブジェクトしか実体化しません。
*/
// ---------------------------------------------------------------------
class WinCoveredCalcApp : public CHrnApp, public CoveredCalcAppBase
{
public:
	static void						CreateInstance();
	static void						DeleteInstance();

	static WinCoveredCalcApp*		GetInstance() { return theInstance; }

	void							Quit();

	void							EnableCoveredCalcWindows(bool isEnabled);
	void							ActivateModalDialog();

	virtual void					ShowCoverBrowser(bool isShow);
	virtual bool					IsCoverBrowserVisible();

	virtual bool					CheckPlatform(ConstUTF8Str platform);

	WinMonitorInfo*					GetMonitorInformation()			{ return &monitorInfo; }
	const WinMonitorInfo*			GetMonitorInformation() const	{ return &monitorInfo; }

	virtual void					LoadDialogFont(SInt32 dialogId, DialogFont& outFont);

	const WinLayeredWindowAPI*		GetLayeredWindowAPI() const		{ return &apiLayeredWindow; }

	const WinMainWindow*			GetMainWindow() const			{ return &mainWindow; }

	void							InstallMessageFilter(WinMessageFilter* filter) { messageFilterManager.InstallMessageFilter(filter); }
	void							UninstallMessageFilter(WinMessageFilter* filter) {  messageFilterManager.UninstallMessageFilter(filter); }
	bool							CallMessageFilters(MSG* msg) { return messageFilterManager.CallFilters(msg); }

	// MessageBoxProvider インタフェースの実装
	virtual Button					DoMessageBox(ConstAStr message, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);
	virtual Button					DoMessageBox(SInt32 messageId, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);

	// WaitingUI インタフェースの実装
	virtual void					BeginWaitingUI();
	virtual void					EndWaitingUI();

protected:
	virtual void					loadLangFile(const Path& path);

private:
									WinCoveredCalcApp();
									~WinCoveredCalcApp();
	
	virtual BOOL					initInstance();
	virtual void					exitInstance();
	virtual int						messageLoop();

protected:
	virtual const Path&				getAppFolderPath();
	virtual const Path&				getUserSettingsPath();
	
private:
	void							loadKeyMappingsOnInit();
	void							loadKeyNameDB();
	
private:
	static WinCoveredCalcApp*		theInstance;		//!< 唯一のインスタンス
	Path							appFolderPath;		//!< アプリケーションのあるフォルダのパス
	Path							userSettingsPath;	///< ユーザー設定を保存するフォルダのパス
	WinMainWindow					mainWindow;			//!< メインウィンドウ
	WinCoverBrowser					coverBrowser;		//!< カバーブラウザウィンドウ
	HCURSOR							waitCursor;			//!< 処理中を示すウェイトカーソル
	HCURSOR							originalCursor;		//!< ウェイトカーソルにする前のカーソル
	SInt32							waitingUICount;		//!< 次の EndWaitingUI() の前に BeginWaitingUI() が呼び出された回数
	bool							isInEnableCoveredCalcWindows;	//!< EnableCoveredCalcWindows の処理中かどうか
	WinMonitorInfo					monitorInfo;		//!< モニタ情報
	WinLayeredWindowAPI				apiLayeredWindow;	///< レイヤードウィンドウ関連の API ラッパー
	DialogFont*						dialogFont;			///< Dialog font.

private:
	class MessageFilterManager
	{
	public:
									MessageFilterManager();
									~MessageFilterManager();
									
		void						InstallMessageFilter(WinMessageFilter* filter);
		void						UninstallMessageFilter(WinMessageFilter* filter);

		bool						CallFilters(MSG* msg);

	private:
		bool						canEraseItem()	{ return (enumerating > 0) ? false : true; }
		void						eraseNoUseItems();
		
	private:
		typedef	std::vector<WinMessageFilter*>	FilterVector;
		FilterVector				filters;		///< installed filters
		SInt32						enumerating;	///< incremented at beginning of DoForEach func and decremented at endding of DoForEach func.
	};
	MessageFilterManager			messageFilterManager;	///< Manager for WinMessageFilters
};

#endif // _COVEREDCALCWINAPP_H_