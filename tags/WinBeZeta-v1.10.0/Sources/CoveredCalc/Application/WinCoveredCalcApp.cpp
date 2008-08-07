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
	@file		WinCoveredCalcApp.cpp
	@brief		WinCoveredCalcApp クラスの実装
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.22 created
*/

#include "Prefix.h"
#include <io.h>
#include <shlobj.h>
#include "Exception.h"
#include "FileException.h"
#include "ExceptionMessageUtils.h"
#include "WinMainWindow.h"
#include "WinMessageBoxProvider.h"
#include "WinCoveredCalcApp.h"
#include "MBCString.h"
#include "PathUtils.h"
#include "XMLParseException.h"
#include "KeyMappings.h"
#include "KeyMappingsException.h"
#include "UTF8Utils.h"
#include "VirtualPathNames.h"
#include "WinMessageFilter.h"

static const char LANG_CODE_JAJP[] = "jaJP";		///< language code for Japanese.

static const UTF8Char STR_PLATFORM_WINDOWS[] = "Windows";			///< keymap platform for Windows.

////////////////////////////////////////
#define base	CHrnApp
////////////////////////////////////////

//! 唯一の WinCoveredCalcApp オブジェクト
WinCoveredCalcApp* WinCoveredCalcApp::theInstance = NULL;

// ---------------------------------------------------------------------
//! 唯一のアプリケーションオブジェクトを取得します。
/*!
	@return CoveredCalcApp オブジェクト
*/
// ---------------------------------------------------------------------
CoveredCalcApp* CoveredCalcApp::GetInstance() { return WinCoveredCalcApp::GetInstance(); }

// ---------------------------------------------------------------------
//! 唯一のインスタンスを生成します。
// ---------------------------------------------------------------------
void WinCoveredCalcApp::CreateInstance()
{
	theInstance = new WinCoveredCalcApp();
}

// ---------------------------------------------------------------------
//! 唯一のインスタンスを削除します。
// ---------------------------------------------------------------------
void WinCoveredCalcApp::DeleteInstance()
{
	if (NULL != theInstance)
	{
		delete theInstance;
		theInstance = NULL;
	}
}

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinCoveredCalcApp::WinCoveredCalcApp()
{
	waitCursor = NULL;
	originalCursor = NULL;
	waitingUICount = 0;
	isInEnableCoveredCalcWindows = false;
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinCoveredCalcApp::~WinCoveredCalcApp()
{
}

// ---------------------------------------------------------------------
//! アプリケーションを終了します。
// ---------------------------------------------------------------------
void WinCoveredCalcApp::Quit()
{
	if (::IsWindow(mainWindow.m_hWnd))
	{
		::DestroyWindow(mainWindow.m_hWnd);
	}
	if (::IsWindow(coverBrowser.m_hWnd))
	{
		::DestroyWindow(coverBrowser.m_hWnd);
	}
	
	::PostQuitMessage(0);
}

// ---------------------------------------------------------------------
//! メッセージボックスを表示します。
/*!
	@return ユーザが選択したボタン
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button WinCoveredCalcApp::DoMessageBox(
	ConstAStr message,							//!< message text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	HWND hOwner;
	if (NULL != mainWindow.m_hWnd && ::IsWindow(mainWindow.m_hWnd))
	{
		hOwner = mainWindow.m_hWnd;
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
MessageBoxProvider::Button WinCoveredCalcApp::DoMessageBox(
	SInt32 messageId,							//!< message id of the text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	HWND hOwner;
	if (NULL != mainWindow.m_hWnd && ::IsWindow(mainWindow.m_hWnd))
	{
		hOwner = mainWindow.m_hWnd;
	}
	else
	{
		hOwner = NULL;
	}
	WinMessageBoxProvider messageBoxProvider(hOwner);
	return messageBoxProvider.DoMessageBox(messageId, buttonType, alertType, defaultButton);
}

// ---------------------------------------------------------------------
//! 処理中を示すためにマウスカーソルを砂時計にします。
// ---------------------------------------------------------------------
void WinCoveredCalcApp::BeginWaitingUI()
{
	if (0 == waitingUICount)
	{
		originalCursor = ::SetCursor(waitCursor);
	}
	waitingUICount++;
}

// ---------------------------------------------------------------------
//! 処理中を示すためのマウスカーソルを元に戻します。
// ---------------------------------------------------------------------
void WinCoveredCalcApp::EndWaitingUI()
{
	if (waitingUICount <= 0)
	{
		return;
	}

	waitingUICount--;
	if (0 == waitingUICount)
	{
		::SetCursor(originalCursor);
		originalCursor = NULL;
	}
}

// ---------------------------------------------------------------------
//! CoveredCalc のウィンドウを有効/無効にします。
// ---------------------------------------------------------------------
void WinCoveredCalcApp::EnableCoveredCalcWindows(
	bool isEnabled			//!< true なら有効に、false なら無効にする
)
{
	if (isInEnableCoveredCalcWindows)
	{
		return;				// 処理中は無視する
	}

	isInEnableCoveredCalcWindows = true;
	try
	{
		HWND hWnds[] =	{
							mainWindow.m_hWnd,		// メインウィンドウ
							coverBrowser.m_hWnd,	// カバーブラウザ
						};

		SInt32 index;
		for (index=0; index<sizeof(hWnds)/sizeof(HWND); index++)
		{
			HWND hWndTarget = hWnds[index];
			if (::IsWindow(hWndTarget))
			{
				bool isEnabledNow = (::IsWindowEnabled(hWndTarget)) ? true : false;
				if ((isEnabled && !isEnabledNow) || (!isEnabled && isEnabledNow))
				{
					::EnableWindow(hWndTarget, isEnabled);
				}
			}
		}
	}
	catch (...)
	{
		isInEnableCoveredCalcWindows = false;
		throw;
	}
	isInEnableCoveredCalcWindows = false;
}

// ---------------------------------------------------------------------
//! ActiateModalDialog() の EnumThreadWindows で呼ばれるコールバック関数
/*!
	@retval TRUE 列挙を続行
	@retval FALSE 列挙を中止
*/
// ---------------------------------------------------------------------
static BOOL CALLBACK enumModalDialogProc(
	HWND hWnd,			//!< ウィンドウのハンドル
	LPARAM lParam		//!< アプリケーション定義の値
)
{
	DWORD dwStyle = GetWindowStyle(hWnd);
	if ((dwStyle & (WS_POPUP | WS_DISABLED | WS_VISIBLE)) == (WS_POPUP | WS_VISIBLE))	// Popup で Enabled で Visible
	{
		HWND* hResultWnd = reinterpret_cast<HWND*>(lParam);
		ASSERT(NULL != hResultWnd);
		*hResultWnd = hWnd;
		return FALSE;
	}
	
	return TRUE;
}

// ---------------------------------------------------------------------
//! モーダル状態で表示されているダイアログをアクティブにします。
// ---------------------------------------------------------------------
void WinCoveredCalcApp::ActivateModalDialog()
{
	HWND hWnd = NULL;
	::EnumThreadWindows(::GetCurrentThreadId(), enumModalDialogProc, reinterpret_cast<LPARAM>(&hWnd));
	if (NULL != hWnd)
	{
		::SetActiveWindow(hWnd);
	}
}

// ---------------------------------------------------------------------
//! カバーブラウザの表示/非表示を切り替えます。
// ---------------------------------------------------------------------
void WinCoveredCalcApp::ShowCoverBrowser(
	bool isShow				//!< true なら表示、false なら非表示
)
{
	if (!::IsWindow(coverBrowser.m_hWnd))
	{
		return;
	}
	
	::ShowWindow(coverBrowser.m_hWnd, (isShow) ? SW_SHOW : SW_HIDE);
	GetAppSettings()->SetCoverBrowserVisible(IsCoverBrowserVisible());	// TODO: このタイミングで保存はしたくないが、最後に保存されないのは困る。modify flag が欲しいな…。
}

// ---------------------------------------------------------------------
//! カバーブラウザが表示されているかどうかを調べます。
/*!
	@retval true 表示されている
	@retval false 表示されていない
*/
// ---------------------------------------------------------------------
bool WinCoveredCalcApp::IsCoverBrowserVisible()
{
	if (::IsWindow(coverBrowser.m_hWnd) && ::IsWindowVisible(coverBrowser.m_hWnd))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 *	@brief	現在の言語ファイルのインスタンスハンドルを取得します。
 *	@return	インスタンスハンドル
 */
HINSTANCE WinCoveredCalcApp::GetLangResHandle() const
{
	HINSTANCE hResHandle = langFile.GetHandle();
	if (NULL == hResHandle)		// 言語ファイルを読み込んでいない場合
	{
		return GetInstanceHandle();
	}
	else
	{
		return hResHandle;
	}
}

// ---------------------------------------------------------------------
//! アプリケーションがあるフォルダのパスを取得します。
// ---------------------------------------------------------------------
const Path& WinCoveredCalcApp::getAppFolderPath()
{
	if (appFolderPath.IsEmpty())
	{
		AChar moduleFileName[MAX_PATH];
		::GetModuleFileName(NULL, moduleFileName, sizeof(moduleFileName)/sizeof(AChar));
		MBCString appFolderString = PathUtils::RemoveFileSpec(moduleFileName);
		appFolderPath.Assign(appFolderString);
	}
	return appFolderPath;
}

/**
 *	@brief	Returns the path of folder in which user settings is stored.
 *	@return user settings folder path.
 */
const Path& WinCoveredCalcApp::getUserSettingsPath()
{
	if (userSettingsPath.IsEmpty())
	{
		Path path;
		
		// Application Data の下
		ITEMIDLIST* pIdList;
		HRESULT hResult = ::SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pIdList);
		if(S_OK == hResult)
		{
			// 得られた ITEMIDLIST からフォルダ名を取得
			char pathString[MAX_PATH];
			if (::SHGetPathFromIDList(pIdList, pathString))
			{
				path.Assign(pathString);
			}
			
			// システムが確保した ITEMIDLIST を解放
			IMalloc* pMalloc;
			::SHGetMalloc(&pMalloc);
			if (NULL != pMalloc)
			{
				pMalloc->Free(pIdList);
				pMalloc->Release();
			}
		}
		if (!path.IsEmpty())
		{
			userSettingsPath = path.Append("Hironytic").Append("CoveredCalc");
		}
		else
		{
			// Application Data が得られなかったら、アプリケーションのあるフォルダに保存
			userSettingsPath = getAppFolderPath();
		}
	}
	return userSettingsPath;
}

/**
 *	@brief	言語ファイルを読み込みます。
 */
void WinCoveredCalcApp::loadLangFile(
	const Path& path		///< 言語ファイルのパス
)
{
	langFile.Load(path);
	
	if (!langFile.CheckVersion())
	{
		langFile.Unload();
		throw new Exception();	// TODO: 適切な例外を投げる
	}
	
	// 言語コードを求める
	MBCString langCode;
	langFile.GetLanguageCode(langCode);
	setCurrentLanguageCode(langCode);
}

/**
 *	@brief	langFileInfos にある言語ファイルの中から自動的に適切な言語ファイルを選択します。
 *	@return	langFileInfos のインデックス (-1ならexe埋め込みを使う)
 */
SInt32 WinCoveredCalcApp::autoSelectLangFile()
{
	LANGID userLangID = GetUserDefaultLangID();
	LANGID englishUSLangID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);

	const LangFileInfoCollection* infoCollection = GetLangFileInfos();

	// システムの言語と同じものを探す
	SInt32 count = infoCollection->GetCount();
	SInt32 englishUSIndex = -1;
	SInt32 index;
	for (index=0; index<count; index++)
	{
		const LangFileInfo& info = infoCollection->GetAt(index);
		LANGID infoLangID = info.GetLangID();
		if (userLangID == infoLangID)
		{
			return index;
		}
		else if (englishUSLangID == infoLangID)
		{
			englishUSIndex = index;
		}
	}
	
	// 同じものが見つからなければ、英語（US）を選択する
	if (-1 != englishUSIndex)
	{
		return englishUSIndex;
	}
	// 英語（US）も見つからなければ、exe埋め込みを利用
	else
	{
		return -1;
	}
}

/**
 *	@brief	Checks the platform string is suitable for this app.
 *	@return	true is suitable.
 */
bool WinCoveredCalcApp::CheckPlatform(ConstUTF8Str platform)
{
	return (0 == UTF8Utils::UTF8StrCmpI(STR_PLATFORM_WINDOWS, platform));
}

/**
 *	@brief	キー名DBを読み込みます。initInstance の処理で呼ばれます。
 */
void WinCoveredCalcApp::loadKeyNameDB()
{
	Path keynameFile;
	keynameFile.AssignFromSlashSeparated("${" VPATH_APP_KEYMAPS "}/keyname.ccknw");
	Path absolutePath = ExpandVirtualPath(keynameFile);
	try
	{
		CoveredCalcAppBase::loadKeyNameDB(absolutePath);
	}
	catch (Exception* ex)
	{
		// キー名DBが読めなくても動作はするのでスルーする
		ex->Delete();
	}
}

/**
 *	@brief	キーマップ定義を読み込みます。initInstance の処理で呼ばれます。
 */
void WinCoveredCalcApp::loadKeyMappingsOnInit()
{
	Path keymapFile;
	AppSettings* appSettings = GetAppSettings();
	keymapFile = appSettings->GetKeymapFilePath();
	if (keymapFile.IsEmpty())
	{
		// 設定になければデフォルト
		
		// トリック：
		// 1.8.x まではカスタマイズがなくて日本語 JIS キーボードしかなかった。
		// デフォルト設定としては US QWERTY（標準的な 101/104 拡張キーボード）がよいが、
		// 言語が日本語の場合は以前のまま日本語 JIS キーボードの方が好ましい。
		// それ以外の場合に US QWERTY キーボードにする。
		MBCString langCode;
		GetCurrentLanguageCode(langCode);
		if (0 == langCode.Compare(LANG_CODE_JAJP))
		{
			keymapFile.AssignFromSlashSeparated("${" VPATH_APP_KEYMAPS "}/JapaneseJIS.cckxw");
		}
		else
		{
			keymapFile.AssignFromSlashSeparated("${" VPATH_APP_KEYMAPS "}/UsQWERTY.cckxw");
		}
		appSettings->SetKeymapFilePath(keymapFile);
	}
	
	Path absolutePath = ExpandVirtualPath(keymapFile);
	try
	{
		LoadKeyMappings(absolutePath);
	}
	catch (Exception* ex)
	{
		// キーマッピング定義が読み込めませんでした。
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_LOAD_KEYMAPPINGS,
																MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		ex->Delete();
	}	
}

// ---------------------------------------------------------------------
//! アプリケーションの初期化
/*!
	@retval TRUE 成功
	@retval FALSE 失敗
*/
// ---------------------------------------------------------------------
BOOL WinCoveredCalcApp::initInstance()
{
	bool langFileLoaded = false;

	if (!base::initInstance())
	{
		return FALSE;
	}

	// コモンコントロール初期化
	::InitCommonControls();

	// レイヤードウィンドウ関連の API
	apiLayeredWindow.Initialize();
	
	// ウェイトカーソルを取得しておく
	waitCursor = ::LoadCursor(NULL, IDC_WAIT);

	// モニタ情報を取得しておく
	monitorInfo.Update();

	// ベースクラス初期化
	init();
	setCurrentLanguageCode(LANG_CODE_JAJP);

	// コマンドラインパラメータ解析
	CommandLineParam* clParam = GetCommandLineParam();
	clParam->SetParameter(__argc, __argv);

	// 言語ファイルの読み込み
	if (clParam->IsLangFileSpecified())
	{
		try
		{
			loadLangFile(clParam->GetLangFile());
			langFileLoaded = true;
		}
		catch (Exception* ex)
		{
			ex->Delete();

			// コマンドラインパラメータで指定された言語ファイルが読み込めなかったので無視します。
			MBCString message;
			GetMessageProvider()->GetMessage(GetInstanceHandle(), IDS_EMSG_LOAD_COMMANDLINE_LANGFILE, message);
			DoMessageBox(message, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		}
	}

	// ウィンドウクラスの登録
	WinMainWindow::RegisterClass();

	//設定ファイルを準備
	Path settingFile;
	if (clParam->IsSettingFileSpecified())
	{
		// コマンドラインで設定ファイルが指定されていればそれを使う
		settingFile.Assign(clParam->GetSettingFile());
	}
	else
	{
		// デフォルト設定ファイルを使う
		try
		{
			readyDefaultSettingFilePath(settingFile);
		}
		catch (Exception* ex)
		{
			ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_READY_DEFAULT_SETTING_FILE,
																	MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
			ex->Delete();
			return FALSE;
		}
	}
	
	// 設定の読み込み
	try
	{
		loadSettings(settingFile);
	}
	catch (Exception* ex)
	{
		ex->Delete();
		return FALSE;
	}

	// 設定に保存された言語ファイルを読み込む
	if (!langFileLoaded)
	{
		AppSettings* appSettings = GetAppSettings();
		const Path settingPath = appSettings->GetLanguageFilePath();
		if (!settingPath.IsEmpty()) {
			Path langFileFullPath = MakeAbsoluteLangFilePath(settingPath);
			if (!langFileFullPath.IsEmpty())
			{
				try
				{
					loadLangFile(langFileFullPath);
					langFileLoaded = true;
				}
				catch (Exception* ex)
				{
					ex->Delete();

					// 言語ファイルが読めません。最適な言語を自動的に判別して起動します。
					MBCString message;
					GetMessageProvider()->GetMessage(GetInstanceHandle(), IDS_EMSG_LOAD_SETTING_LANGFILE, message);
					DoMessageBox(message, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
				}
			}
			else
			{
				// built-in
				langFileLoaded = true;
			}
		}
	}
	
	if (!langFileLoaded)
	{
		// 設定に保存されていなければ、LangID から適切なものを選択する。
		SInt32 langFileInfoIndex = autoSelectLangFile();
		if (-1 != langFileInfoIndex)
		{
			const LangFileInfoCollection* infoCollection = GetLangFileInfos();
			const LangFileInfo& info = infoCollection->GetAt(langFileInfoIndex);
			if (!info.GetPath().IsEmpty())
			{
				try
				{
					loadLangFile(info.GetPath());
					langFileLoaded = true;
				}
				catch (Exception* ex)
				{
					ex->Delete();
				}

				if (langFileLoaded)
				{
					AppSettings* appSettings = GetAppSettings();
					Path relativeLangFilePath = MakeRelativeLangFilePath(info.GetPath());
					appSettings->SetLanguageFilePath(relativeLangFilePath);
				}
			}
		}
	}

	if (!langFileLoaded)
	{
		GetAppSettings()->SetLanguageFilePath(AppSettings::Value_LangFileBuiltIn);
	}
	
	// キー定義名 DB のロード
	loadKeyNameDB();
	
	// キーマッピング読み込み
	loadKeyMappingsOnInit();
	
	// カバー読み込み
	try
	{
		AppSettings* appSettings = GetAppSettings();
		loadCoverDef(appSettings->GetBaseFolder(), appSettings->GetLastCoverDef(), appSettings->GetLastCoverNo());
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(this, ex);
		ex->Delete();

		// デフォルトカバーで復活を試みる
		if (!restoreByDefaultCoverDef())
		{
			// ダメでした…。
			return FALSE;
		}
	}

	// メインウィンドウ生成
	DWORD exStyle = 0;
	if (GetAppSettings()->IsMainWindowAlwaysOnTop())
	{
		exStyle = WS_EX_TOPMOST;
	}
	const Point32& lastMainWindowPos = GetAppSettings()->GetLastMainWindowPos();
	if (!mainWindow.CreateEx(exStyle, WinMainWindow::GetWindowClassName(), "CoveredCalc", WS_SYSMENU | WS_POPUP | WS_MINIMIZEBOX, lastMainWindowPos.x, lastMainWindowPos.y, 0, 0, NULL, NULL))
	{
		// デフォルトカバーにして再チャレンジ
		bool restored = false;
		if (restoreByDefaultCoverDef())
		{
			if (mainWindow.CreateEx(exStyle, WinMainWindow::GetWindowClassName(), "CoveredCalc", WS_SYSMENU | WS_POPUP | WS_MINIMIZEBOX, lastMainWindowPos.x, lastMainWindowPos.y, 0, 0, NULL, NULL))
			{
				restored = true;
			}
		}
		
		if (!restored)
		{
			DoMessageBox(IDS_EMSG_CREATE_MAIN_WINDOW, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
			return FALSE;
		}
	}
	::ShowWindow(mainWindow.m_hWnd, SW_SHOW);

	// カバーブラウザ生成
	Path baseFolderPath = GetAppSettings()->GetBaseFolder();
	if (baseFolderPath.IsEmpty())
	{
		baseFolderPath = getAppFolderPath();
	}
	coverBrowser.SetCoversFolderPath(baseFolderPath.Append("Covers"));
	if (!coverBrowser.Create(NULL))
	{
		DoMessageBox(IDS_EMSG_CREATE_COVER_BROWSER, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
	}

	::ShowWindow(mainWindow.m_hWnd, SW_SHOW);
	if (GetAppSettings()->IsCoverBrowserVisible())
	{
		::ShowWindow(coverBrowser.m_hWnd, SW_SHOW);
	}

	::SetForegroundWindow(mainWindow.m_hWnd);

	return TRUE;
}

// ---------------------------------------------------------------------
//! アプリケーションの終了処理
// ---------------------------------------------------------------------
void WinCoveredCalcApp::exitInstance()
{
	// 言語ファイルを読み込んでいたら解放する。
	langFile.Unload();

	// レイヤードウィンドウ関連の API
	apiLayeredWindow.Terminate();

	base::exitInstance();
}

// ---------------------------------------------------------------------
//! アプリケーションのメインメッセージループ
/*!	
	@return アプリケーションの戻り値
*/
// ---------------------------------------------------------------------
int WinCoveredCalcApp::messageLoop()
{
	MSG	msg;
	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		bool doDispatch = CallMessageFilters(&msg);
		if (doDispatch)
		{
			if (NULL == coverBrowser.m_hWnd || !::IsDialogMessage(coverBrowser.m_hWnd, &msg))
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
	}
	return msg.wParam;	
}

/**
 *	@brief	コンストラクタ
 */
WinCoveredCalcApp::MessageFilterManager::MessageFilterManager()
{
	enumerating = 0;
}

/**
 *	@brief	デストラクタ
 */
WinCoveredCalcApp::MessageFilterManager::~MessageFilterManager()
{
}

/**
 *	@brief	メッセージフィルタのインストール
 *	@param[in]	filter	インストールするフィルタ
 */
void WinCoveredCalcApp::MessageFilterManager::InstallMessageFilter(WinMessageFilter* filter)
{
	filters.push_back(filter);
}

/**
 *	@brief	メッセージフィルタのアンインストール
 *	@param[in]	filter	アンインストールするフィルタ
 */
void WinCoveredCalcApp::MessageFilterManager::UninstallMessageFilter(WinMessageFilter* filter)
{
	FilterVector::iterator ite;
	for (ite = filters.begin(); ite != filters.end(); ite++)
	{
		if (filter == *ite)
		{
			*ite = NULL;
			if (canEraseItem())
			{
				filters.erase(ite);
			}
			break;
		}
	}
}

/**
 *	@brief	メッセージフィルタの呼び出し
 *	@param[in]	呼び出すメッセージ
 */
bool WinCoveredCalcApp::MessageFilterManager::CallFilters(MSG* msg)
{
	enumerating++;

	bool ret = true;
	try
	{
		FilterVector::iterator ite;
		for (ite = filters.begin(); ite != filters.end(); ite++)
		{
			ret = (*ite)->FilterMessage(msg);
			if (!ret)
			{
				break;
			}
		}
	}
	catch (...)
	{
		enumerating--;
		eraseNoUseItems();
		throw;
	}
	enumerating--;
	eraseNoUseItems();

	return ret;
	
}

/**
 *	@brief	アンインストールされたフィルタのアイテムを削除します。
 *	@note	CallFilters 中に行われたフィルタのアンインストールではその時点ではfiltersから除去されず代わりに NULL が設定されます。
 */
void WinCoveredCalcApp::MessageFilterManager::eraseNoUseItems()
{
	if (!canEraseItem())
	{
		return;
	}

	FilterVector::iterator ite;
	for (ite = filters.begin(); ite != filters.end(); )
	{
		if (NULL == *ite)
		{
			ite = filters.erase(ite);
		}
		else
		{
			ite++;
		}
	}
}

// ---------------------------------------------------------------------
//! プログラムのエントリポイント
/*!	
	@retval 0 正常終了
*/
// ---------------------------------------------------------------------
int APIENTRY WinMain(
	HINSTANCE hInstance,		//!< インスタンスハンドル
	HINSTANCE hPrevInstance,	//!< 既に存在するインスタンスハンドル (Win32では常にNULL)
	LPSTR     lpCmdLine,		//!< コマンドライン文字列
	int       nCmdShow			//!< 表示モード
)
{
	// デバッグ版でメモリリークを検出
	_CrtSetDbgFlag(_crtDbgFlag | _CRTDBG_LEAK_CHECK_DF);
	
	int	retValue = 0;

	// 落ちるなら落ちてくれということであえてここでは例外をキャッチしない。
	WinCoveredCalcApp::CreateInstance();
	retValue = WinCoveredCalcApp::GetInstance()->WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	WinCoveredCalcApp::DeleteInstance();

	return retValue;
}
