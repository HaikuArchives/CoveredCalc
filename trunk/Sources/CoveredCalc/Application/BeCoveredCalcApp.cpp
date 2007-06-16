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
	@file		BeCoveredCalcApp.cpp
	@brief		Implementation of BeCoveredCalcApp class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.16 created
*/

#include "Prefix.h"
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <SupportKit.h>
#if defined(ZETA)
#include <locale/Locale.h>
#endif // defined(ZETA)
#include "FileException.h"
#include "ExceptionMessageUtils.h"
#include "BeMainWindow.h"
#include "BeCoverBrowser.h"
#include "BeMessageBoxProvider.h"
#include "DialogDef.h"
#include "BeCoveredCalcApp.h"
#include "BeDialogDesign.h"

#if defined(ZETA)
/**
 *	@brief	Retrieves localized string on ZETA.
 *	@note	If user don't select locale kit in language setting, this function returns just a key argument.
 */
const char* ZetaLocaleString(const char* key)
{
	if (BeCoveredCalcApp::GetInstance()->IsLocaleKitAvailable())
	{
		return _T(key);
	}
	else
	{
		return key;
	}
}
#endif // defined(ZETA)

// ---------------------------------------------------------------------
//! Retrives unique application object.
/*!
	@return CoveredCalcApp object
*/
// ---------------------------------------------------------------------
CoveredCalcApp* CoveredCalcApp::GetInstance() { return BeCoveredCalcApp::GetInstance(); }

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeCoveredCalcApp::BeCoveredCalcApp()
	: BApplication("application/x-vnd.Hironytic-CoveredCalc")
{
	mainWindow = NULL;
	coverBrowser = NULL;
	commandLineParamProcessed = false;
#if defined(ZETA)
	isLocaleKitAvailable = true;
#endif // defined(ZETA)	
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeCoveredCalcApp::~BeCoveredCalcApp()
{
}

// ---------------------------------------------------------------------
//! カバーブラウザの表示状態を切り替えます。
// ---------------------------------------------------------------------
void BeCoveredCalcApp::ShowCoverBrowser(
	bool isShow								//!< true なら表示、false なら非表示
)
{
	if (NULL != coverBrowser)
	{
		if (isShow)
		{
			coverBrowser->Show();
		}
		else
		{
			coverBrowser->Hide();
		}				

		GetAppSettings()->SetCoverBrowserVisible(IsCoverBrowserVisible());	// TODO: このタイミングで保存はしたくないが、最後に保存されないのは困る。modify flag が欲しいな…。		
	}	
}

// ---------------------------------------------------------------------
//! カバーブラウザが表示されているかどうかを取得します。
/*!
	@retval	true	表示されています。
	@retval	false	表示されていません。
*/
// ---------------------------------------------------------------------
bool BeCoveredCalcApp::IsCoverBrowserVisible()
{
	if (NULL == coverBrowser)
		return false;
	
	BAutolock locker(coverBrowser);
	if (locker.IsLocked())
		return !coverBrowser->IsHidden();
	else
		return true;	// たぶん表示されていて何かやっているからロックできないんだろう...
}

// ---------------------------------------------------------------------
//! Shows a message box
/*!
	@return the button which user selected.
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button BeCoveredCalcApp::DoMessageBox(
	ConstAStr message,							//!< message text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	BeMessageBoxProvider messageBoxProvider;
	return messageBoxProvider.DoMessageBox(message, buttonType, alertType, defaultButton);
}

// ---------------------------------------------------------------------
//! Shows a message box
/*!
	@return the button which user selected.
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button BeCoveredCalcApp::DoMessageBox(
	SInt32 messageId,							//!< message id of the text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	BeMessageBoxProvider messageBoxProvider;
	return messageBoxProvider.DoMessageBox(messageId, buttonType, alertType, defaultButton);
}

// ---------------------------------------------------------------------
//! 処理中の UI を開始します。BeOS 版では得に何もしません。
// ---------------------------------------------------------------------
void BeCoveredCalcApp::BeginWaitingUI()
{
}

// ---------------------------------------------------------------------
//! 処理中の UI を終了します。BeOS 版では得に何もしません。
// ---------------------------------------------------------------------
void BeCoveredCalcApp::EndWaitingUI()
{
}

// ---------------------------------------------------------------------
//! 指定したパスのフォルダが存在しなければ作成します。
// ---------------------------------------------------------------------
void BeCoveredCalcApp::createFolder(
	const Path& path				//!< 作成するフォルダのパス
)
{
	const char* pathString = path.GetPathString();
	BEntry entry(pathString);
	if (entry.Exists())				// 既に存在する
	{
		if (!entry.IsDirectory())
		{
			// フォルダでなければいけない
			throw new FileExceptions::FileAlreadyExists(path);
		}

		// 既に存在するので何もしない
		return;
	}

	// 作成
	status_t result = create_directory(pathString, 0755);
	if (B_OK != result)
	{
		switch (result)
		{
		case B_ENTRY_NOT_FOUND:
			throw new FileExceptions::FileNotFound(path);
			break;
		
		case B_FILE_EXISTS:
			throw new FileExceptions::FileAlreadyExists(path);
			break;
		
		case B_NOT_ALLOWED:
		case B_PERMISSION_DENIED:
			throw new FileExceptions::AccessDenied(path);
			break;
	
		case B_DEVICE_FULL:
			throw new FileExceptions::DeviceFull(path);
			break;
		
		default:
			throw new FileException(path);
			break;
		}
	}
}

// ---------------------------------------------------------------------
//! デフォルトの設定ファイルのパスを取得します。パスが存在しなければ作成も行います。
// ---------------------------------------------------------------------
void BeCoveredCalcApp::readyDefaultSettingFilePath(
	Path& settingFilePath			//!< OUTPUT. デフォルトの設定ファイルのパスが返ります。
)
{
	Path path;

	// settings の下
	BPath objBPath;
	status_t result = find_directory(B_USER_SETTINGS_DIRECTORY, &objBPath);
	if (B_OK == result)
	{
		path.Assign(objBPath.Path());
	}
	
	if (!path.IsEmpty())
	{
		path = path.Append("CoveredCalc");
		createFolder(path);
	}

	if (!path.IsEmpty())
	{
		settingFilePath = path.Append("Setting.xml");
	}		
}

/**
 *	@brief	Loads language file.
 */
void BeCoveredCalcApp::loadLangFile(
	const Path& path		///< path of the lang file.
)
{
	langFile.Load(path);
	
	// load current language code.
	MBCString langCode;
	langFile.GetLanguageCode(langCode);
	setCurrentLanguageCode(langCode);
}

#if defined(ZETA)
/**
 *	@brief	Retrieves the language code of current language.
 */
void BeCoveredCalcApp::GetCurrentLanguageCode(
	MBCString& outLanguage		///< the string is returned, which represents language code. (OUTPUT)
)
{
	if (isLocaleKitAvailable)
	{
		outLanguage = be_locale.GetLanguageShortName().String();
	}
	else
	{
		CoveredCalcAppBase::GetCurrentLanguageCode(outLanguage);
	}
}
#endif // defined(ZETA)

// ---------------------------------------------------------------------
//! Called to ready application before running.
// ---------------------------------------------------------------------
void BeCoveredCalcApp::ReadyToRun()
{
	// 通常、1回目のArgvReceived()はReadyToRun()より先にくるはず。
	// ArgvReceived() が来ないでReadyToRun()が呼ばれたなら、引数がなかったとみなす。
	commandLineParamProcessed = true;

	bool langFileLoaded = false;

	// ベースクラス初期化
	init();

	// リソースローダ初期化
//	resLoader.SetResources(AppResources());

	//  アプリケーションがあるフォルダのパスを取得
	app_info appInfo;
	GetAppInfo(&appInfo);
	BPath path(&appInfo.ref);
	path.GetParent(&path);
	appFolderPath = Path(path.Path());
	
	CommandLineParam* clParam = GetCommandLineParam();
	
	// 言語ファイルの読み込み
	if (clParam->IsLangFileSpecified())
	{
		try
		{
			loadLangFile(clParam->GetLangFile());
			langFileLoaded = true;
#if defined(ZETA)
			isLocaleKitAvailable = false;
#endif // defined(ZETA)
		}
		catch (Exception* ex)
		{
			// コマンドラインパラメータで指定された言語ファイルが読み込めなかったので無視します。
			ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_LOAD_COMMANDLINE_LANGFILE,
																	MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
			ex->Delete();
		}
	}
	
	// 設定ファイルを準備
	Path settingFile;
	if (clParam->IsSettingFileSpecified())
	{
		// コマンドライン引数で指定された設定ファイルを使う
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
			be_app->PostMessage(B_QUIT_REQUESTED, be_app);
			return;
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
		be_app->PostMessage(B_QUIT_REQUESTED, be_app);
		return;
	}

	// 設定に保存された言語ファイルを読み込む
	bool isBuiltIn = true;
	if (!langFileLoaded)
	{
		AppSettings* appSettings = GetAppSettings();
		Path langFilePath = appSettings->GetLanguageFilePath();
		if (!langFilePath.IsEmpty())
		{
			Path langFileFullPath = MakeAbsoluteLangFilePath(langFilePath);
			if (!langFileFullPath.IsEmpty())
			{
				isBuiltIn = false;
				try
				{
					loadLangFile(langFileFullPath);
					langFileLoaded = true;
#if defined(ZETA)
					isLocaleKitAvailable = false;
					GetAppSettings()->SetLocaleKitAvailable(false);
#endif // defined(ZETA)
				}
				catch (Exception* ex)
				{
					// 言語ファイルが読めません。
					ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_LOAD_SETTING_LANGFILE,
																	MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
					ex->Delete();
				}
			}
		}
	}
	
	if (!langFileLoaded)
	{
		Path builtInLangFileFullPath = MakeAbsoluteLangFilePath(Path("enUS.cclxb"));
		try
		{
			loadLangFile(builtInLangFileFullPath);
			langFileLoaded = true;
#if defined(ZETA)
			if (isBuiltIn)
			{
				// もともと built-in だったのならLocakeKit利用の設定を読み込み
				isLocaleKitAvailable = GetAppSettings()->IsLocaleKitAvailable();
			}
			else
			{
				// もともと build-in じゃなかったのなら強制的にLocaleKit利用に設定
				isLocaleKitAvailable = true;
				GetAppSettings()->SetLocaleKitAvailable(true);
			}
#endif // defined(ZETA)
		}
		catch (Exception* ex)
		{
			// デフォルトの言語ファイルが読めません。
			ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_LOAD_DEFAULT_LANGFILE,
															MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
			ex->Delete();
			be_app->PostMessage(B_QUIT_REQUESTED, be_app);
			return;
		}
	
		GetAppSettings()->SetLanguageFilePath(AppSettings::Value_LangFileBuiltIn);
	}

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
			// ダメでした。
			be_app->PostMessage(B_QUIT_REQUESTED, be_app);
			return;
		}
	}	

	// ウィンドウは直接メンバにもっておく。ウィンドウが削除されるときに勝手に delete されてしまうが、
	// そもそも、アプリケーション起動中はウィンドウが削除されることを想定していないし。
	
	// メインウィンドウ生成
	MBCString appName;
	GetMessageProvider()->GetMessage(IDS_APPNAME, appName);
	const Point32& lastMainWindowPos = GetAppSettings()->GetLastMainWindowPos();
	bool isAlwaysOnTop = GetAppSettings()->IsMainWindowAlwaysOnTop();
	mainWindow = new BeMainWindow(BRect(lastMainWindowPos.x, lastMainWindowPos.y, lastMainWindowPos.x, lastMainWindowPos.y), appName, B_CURRENT_WORKSPACE, isAlwaysOnTop);
	try
	{
		mainWindow->Init();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(this, ex, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		ex->Delete();
	
		// デフォルトカバーにして再チャレンジ
		bool restored = false;
		if (restoreByDefaultCoverDef())
		{
			try
			{
				mainWindow->Init();
				restored = true;
			}
			catch (Exception* exRestore)
			{
				ExceptionMessageUtils::DoExceptionMessageBox(this, exRestore, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
				exRestore->Delete();
			}
		}
		
		if (!restored)
		{
			DoMessageBox(IDS_EMSG_CREATE_MAIN_WINDOW, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
			be_app->PostMessage(B_QUIT_REQUESTED, be_app);
			return;
		}
	}
	
	// カバーブラウザ生成
	try
	{
		Path baseFolderPath = GetAppSettings()->GetBaseFolder();
		if (baseFolderPath.IsEmpty())
		{
			baseFolderPath = getAppFolderPath();
		}
	
		BeDialogDesign* dialogDesign = GetLangFile()->LoadDialogDesign(IDD_COVER_BROWSER);
		try
		{
			coverBrowser = new BeCoverBrowser(dialogDesign);
			dialogDesign = NULL;
		}
		catch (...)
		{
			delete dialogDesign;
			throw;
		}
		coverBrowser->SetCoversFolderPath(baseFolderPath.Append("Covers"));
		coverBrowser->Init();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_CREATE_COVER_BROWSER,
																MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
		ex->Delete();
	}

	// メインウィンドウ表示 + スレッド生成
	mainWindow->Show();

	// カバーブラウザ表示（必要に応じて） + スレッド生成
	if (!GetAppSettings()->IsCoverBrowserVisible())
	{
		coverBrowser->Hide();		// 表示しないために 1 回 Hide() を呼んでおく
	}
	coverBrowser->Show();	// スレッド生成を兼ねている
	coverBrowser->SendBehind(mainWindow);
}

// ---------------------------------------------------------------------
//! The application is requested to quit.
/*!
	@retval	true	quit this application.
	@retval	false	don't quit this application.
*/
// ---------------------------------------------------------------------
bool BeCoveredCalcApp::QuitRequested()
{
	// カバーブラウザは常に終了しないで～と返すので
	// こいつは強制的に終了してやる
	if (NULL != coverBrowser)
	{
		coverBrowser->Lock();
		coverBrowser->Quit();
	}
	
	return BApplication::QuitRequested();
}

// ---------------------------------------------------------------------
//! The application is requested to show about dialog.
// ---------------------------------------------------------------------
void BeCoveredCalcApp::AboutRequested()
{
	if (NULL != mainWindow)
	{
		mainWindow->ShowAboutDialog();
	}
}

/**
 *	@brief	Called when command line argument is used to launch.
 */
void BeCoveredCalcApp::ArgvReceived(
	int32 argc,		///< number of command line parameters.
	char** argv		///< array of each command line parameter.
)
{
	if (!commandLineParamProcessed)
	{
		CommandLineParam* param = GetCommandLineParam();
		param->SetParameter(argc, argv);
		commandLineParamProcessed = true;
	}
}

// ---------------------------------------------------------------------
//! Entry point of this program.
/*!
	@retval 0 Success
*/
// ---------------------------------------------------------------------
int main()
{
	BeCoveredCalcApp app;
	app.Run();
	return 0;
}
