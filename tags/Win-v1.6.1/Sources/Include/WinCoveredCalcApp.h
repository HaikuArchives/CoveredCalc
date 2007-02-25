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
#include "WinLangFile.h"

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

	WinMonitorInfo*					GetMonitorInformation()			{ return &monitorInfo; }
	const WinMonitorInfo*			GetMonitorInformation() const	{ return &monitorInfo; }

	HINSTANCE						GetLangResHandle() const;

	// MessageBoxProvider インタフェースの実装
	virtual Button					DoMessageBox(ConstAStr message, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);
	virtual Button					DoMessageBox(SInt32 messageId, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);

	// WaitingUI インタフェースの実装
	virtual void					BeginWaitingUI();
	virtual void					EndWaitingUI();

private:
									WinCoveredCalcApp();
									~WinCoveredCalcApp();
	
	virtual BOOL					initInstance();
	virtual void					exitInstance();
	virtual int						messageLoop();

protected:
	virtual const Path&				getAppFolderPath() { return appFolderPath; }
	virtual void					readyDefaultSettingFilePath(Path& settingFilePath);

private:
	void							makeAppFolderPath();
	void							createFolder(const Path& path);
	void							loadLangFile(const Path& path);
	SInt32							autoSelectLangFile();
	
private:
	static WinCoveredCalcApp*		theInstance;		//!< 唯一のインスタンス
	Path							appFolderPath;		//!< アプリケーションのあるフォルダのパス
	WinMainWindow					mainWindow;			//!< メインウィンドウ
	WinCoverBrowser					coverBrowser;		//!< カバーブラウザウィンドウ
	HCURSOR							waitCursor;			//!< 処理中を示すウェイトカーソル
	HCURSOR							originalCursor;		//!< ウェイトカーソルにする前のカーソル
	SInt32							waitingUICount;		//!< 次の EndWaitingUI() の前に BeginWaitingUI() が呼び出された回数
	bool							isInEnableCoveredCalcWindows;	//!< EnableCoveredCalcWindows の処理中かどうか
	WinMonitorInfo					monitorInfo;		//!< モニタ情報
	WinLangFile						langFile;			///< 言語ファイル
};

#endif // _COVEREDCALCWINAPP_H_