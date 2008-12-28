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
	@brief		WinCoveredCalcApp �N���X�̎���
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
#include "StringID.h"
#include "MemoryException.h"

static const UTF8Char STR_PLATFORM_WINDOWS[] = "Windows";	///< keymap platform for Windows.

////////////////////////////////////////
#define base	CHrnApp
////////////////////////////////////////

//! �B��� WinCoveredCalcApp �I�u�W�F�N�g
WinCoveredCalcApp* WinCoveredCalcApp::theInstance = NULL;

// ---------------------------------------------------------------------
//! �B��̃A�v���P�[�V�����I�u�W�F�N�g���擾���܂��B
/*!
	@return CoveredCalcApp �I�u�W�F�N�g
*/
// ---------------------------------------------------------------------
CoveredCalcApp* CoveredCalcApp::GetInstance() { return WinCoveredCalcApp::GetInstance(); }

// ---------------------------------------------------------------------
//! �B��̃C���X�^���X�𐶐����܂��B
// ---------------------------------------------------------------------
void WinCoveredCalcApp::CreateInstance()
{
	theInstance = new WinCoveredCalcApp();
}

// ---------------------------------------------------------------------
//! �B��̃C���X�^���X���폜���܂��B
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
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinCoveredCalcApp::WinCoveredCalcApp()
{
	waitCursor = NULL;
	originalCursor = NULL;
	waitingUICount = 0;
	isInEnableCoveredCalcWindows = false;
	dialogFont = NULL;
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinCoveredCalcApp::~WinCoveredCalcApp()
{
	if (NULL != dialogFont)
	{
		delete dialogFont;
	}
}

// ---------------------------------------------------------------------
//! �A�v���P�[�V�������I�����܂��B
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
//! ���b�Z�[�W�{�b�N�X��\�����܂��B
/*!
	@return ���[�U���I�������{�^��
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
//! ���b�Z�[�W�{�b�N�X��\�����܂��B
/*!
	@return ���[�U���I�������{�^��
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
//! ���������������߂Ƀ}�E�X�J�[�\���������v�ɂ��܂��B
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
//! ���������������߂̃}�E�X�J�[�\�������ɖ߂��܂��B
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
//! CoveredCalc �̃E�B���h�E��L��/�����ɂ��܂��B
// ---------------------------------------------------------------------
void WinCoveredCalcApp::EnableCoveredCalcWindows(
	bool isEnabled			//!< true �Ȃ�L���ɁAfalse �Ȃ疳���ɂ���
)
{
	if (isInEnableCoveredCalcWindows)
	{
		return;				// �������͖�������
	}

	isInEnableCoveredCalcWindows = true;
	try
	{
		HWND hWnds[] =	{
							mainWindow.m_hWnd,		// ���C���E�B���h�E
							coverBrowser.m_hWnd,	// �J�o�[�u���E�U
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
//! ActiateModalDialog() �� EnumThreadWindows �ŌĂ΂��R�[���o�b�N�֐�
/*!
	@retval TRUE �񋓂𑱍s
	@retval FALSE �񋓂𒆎~
*/
// ---------------------------------------------------------------------
static BOOL CALLBACK enumModalDialogProc(
	HWND hWnd,			//!< �E�B���h�E�̃n���h��
	LPARAM lParam		//!< �A�v���P�[�V������`�̒l
)
{
	DWORD dwStyle = GetWindowStyle(hWnd);
	if ((dwStyle & (WS_POPUP | WS_DISABLED | WS_VISIBLE)) == (WS_POPUP | WS_VISIBLE))	// Popup �� Enabled �� Visible
	{
		HWND* hResultWnd = reinterpret_cast<HWND*>(lParam);
		ASSERT(NULL != hResultWnd);
		*hResultWnd = hWnd;
		return FALSE;
	}
	
	return TRUE;
}

// ---------------------------------------------------------------------
//! ���[�_����Ԃŕ\������Ă���_�C�A���O���A�N�e�B�u�ɂ��܂��B
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
//! �J�o�[�u���E�U�̕\��/��\����؂�ւ��܂��B
// ---------------------------------------------------------------------
void WinCoveredCalcApp::ShowCoverBrowser(
	bool isShow				//!< true �Ȃ�\���Afalse �Ȃ��\��
)
{
	if (!::IsWindow(coverBrowser.m_hWnd))
	{
		return;
	}
	
	::ShowWindow(coverBrowser.m_hWnd, (isShow) ? SW_SHOW : SW_HIDE);
	GetAppSettings()->SetCoverBrowserVisible(IsCoverBrowserVisible());	// TODO: ���̃^�C�~���O�ŕۑ��͂������Ȃ����A�Ō�ɕۑ�����Ȃ��͍̂���Bmodify flag ���~�����ȁc�B
}

// ---------------------------------------------------------------------
//! �J�o�[�u���E�U���\������Ă��邩�ǂ����𒲂ׂ܂��B
/*!
	@retval true �\������Ă���
	@retval false �\������Ă��Ȃ�
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
 *	@brief	Loads a language file.
 *	@param[in]	path	path of language file
 */
void WinCoveredCalcApp::loadLangFile(const Path& path)
{
	CoveredCalcAppBase::loadLangFile(path);

	if (NULL != dialogFont)
	{
		delete dialogFont;
		dialogFont = NULL;
	}
}

static bool isMatchTargetOSVersion(const OSVERSIONINFO& osv, const MBCString& target)
{
	const MBCString wildcard = ALITERAL('*');
	enum Phase
	{
		Phase_Platform = 0,
		Phase_Major,
		Phase_Minor,
		Phase_BuildNo,
		Phase_End
	};
	SInt32 phase = Phase_Platform;
	SInt32 ix = 0;
	while (phase < Phase_End)
	{
		if (ix >= target.Length())
		{
			return false;
		}

		MBCString elem;
		SInt32 sep = target.Find(ALITERAL('.'), ix);
		if (0 > sep)
		{
			elem = target.SubString(ix);
			ix = target.Length();
		}
		else
		{
			elem = target.SubString(ix, sep - ix);
			ix = sep + 1;
		}
		
		if (0 != elem.Compare(wildcard))
		{
			DWORD num = ttoi(elem.CString());
			DWORD cmp = 0xffffffff;
			switch (phase)
			{
			case Phase_Platform:
				cmp = osv.dwPlatformId;
				break;
			case Phase_Major:
				cmp = osv.dwMajorVersion;
				break;
			case Phase_Minor:
				cmp = osv.dwMinorVersion;
				break;
			case Phase_BuildNo:
				cmp = osv.dwBuildNumber;
				break;
			}
			if (num != cmp)
			{
				return false;
			}
		}
		++phase;
	}

	return true;
}

/**
 *	@brief	Loads dialog font.
 *	@param[in]	dialogId	dialog ID.
 *	@param[out]	outFont		font information is returned.
 */
void WinCoveredCalcApp::LoadDialogFont(SInt32 /*dialogId*/, DialogFont& outFont)
{
	if (NULL == dialogFont)
	{
		OSVERSIONINFO osv;
		ZeroMemory(&osv, sizeof(OSVERSIONINFO));
		osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osv);

		XMLLangFile::DialogFontVector dfVector;
		LoadedLangFilePtrVector::const_reverse_iterator itr;
		for (itr = langFiles.rbegin(); NULL == dialogFont && itr != langFiles.rend(); ++itr)
		{
			const XMLLangFile& langFile = (*itr)->langFile;
			langFile.LoadDialogFont(dfVector);
			XMLLangFile::DialogFontVector::const_iterator dfItr;
			for (dfItr = dfVector.begin(); dfItr != dfVector.end(); ++dfItr)
			{
				const DialogFont& df = *dfItr;
				if (isMatchTargetOSVersion(osv, df.TargetOSVersion))
				{
					dialogFont = new DialogFont;
					if (NULL == dialogFont)
					{
						MemoryException::Throw();
					}
					*dialogFont = df;
					break;
				}
			}
		}

		// catch for a rainy day
		if (NULL == dialogFont)
		{
			dialogFont = new DialogFont;
			if (NULL == dialogFont)
			{
				MemoryException::Throw();
			}
			dialogFont->Charset = DEFAULT_CHARSET;
			dialogFont->PointSize = 8;
			dialogFont->Typeface = TypeConv::AsUTF8("MS Shell Dlg");
		}
	}

	outFont = *dialogFont;
	return;
}

// ---------------------------------------------------------------------
//! �A�v���P�[�V����������t�H���_�̃p�X���擾���܂��B
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
		
		// Application Data �̉�
		ITEMIDLIST* pIdList;
		HRESULT hResult = ::SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pIdList);
		if(S_OK == hResult)
		{
			// ����ꂽ ITEMIDLIST ����t�H���_�����擾
			TCHAR pathString[MAX_PATH];
			if (::SHGetPathFromIDList(pIdList, pathString))
			{
				path.Assign(pathString);
			}
			
			// �V�X�e�����m�ۂ��� ITEMIDLIST �����
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
			userSettingsPath = path.Append(ALITERAL("Hironytic")).Append(ALITERAL("CoveredCalc"));
		}
		else
		{
			// Application Data �������Ȃ�������A�A�v���P�[�V�����̂���t�H���_�ɕۑ�
			userSettingsPath = getAppFolderPath();
		}
	}
	return userSettingsPath;
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
 *	@brief	�L�[��DB��ǂݍ��݂܂��BinitInstance �̏����ŌĂ΂�܂��B
 */
void WinCoveredCalcApp::loadKeyNameDB()
{
	Path keynameFile;
	keynameFile.AssignFromSlashSeparated(ALITERAL("${") VPATH_APP_KEYMAPS ALITERAL("}/keyname.ccknw"));
	Path absolutePath = ExpandVirtualPath(keynameFile);
	try
	{
		CoveredCalcAppBase::loadKeyNameDB(absolutePath);
	}
	catch (Exception* ex)
	{
		// �L�[��DB���ǂ߂Ȃ��Ă�����͂���̂ŃX���[����
		ex->Delete();
	}
}

// FIXME: ���̃g���b�N�̂��߂ɂ��� �g���b�N���̂����邩�Ȃ��H
static const AChar LANG_CODE_JAJP[] = ALITERAL("jaJP");		///< language code for Japanese.

/**
 *	@brief	�L�[�}�b�v��`��ǂݍ��݂܂��BinitInstance �̏����ŌĂ΂�܂��B
 */
void WinCoveredCalcApp::loadKeyMappingsOnInit()
{
	Path keymapFile;
	AppSettings* appSettings = GetAppSettings();
	keymapFile = appSettings->GetKeymapFilePath();
	if (keymapFile.IsEmpty())
	{
		// �ݒ�ɂȂ���΃f�t�H���g
		
		// �g���b�N�F
		// 1.8.x �܂ł̓J�X�^�}�C�Y���Ȃ��ē��{�� JIS �L�[�{�[�h�����Ȃ������B
		// �f�t�H���g�ݒ�Ƃ��Ă� US QWERTY�i�W���I�� 101/104 �g���L�[�{�[�h�j���悢���A
		// ���ꂪ���{��̏ꍇ�͈ȑO�̂܂ܓ��{�� JIS �L�[�{�[�h�̕����D�܂����B
		// ����ȊO�̏ꍇ�� US QWERTY �L�[�{�[�h�ɂ���B
		MBCString langCode;
		GetCurrentLanguageCode(langCode);
		if (0 == langCode.Compare(LANG_CODE_JAJP))
		{
			keymapFile.AssignFromSlashSeparated(ALITERAL("${") VPATH_APP_KEYMAPS ALITERAL("}/JapaneseJIS.cckxw"));
		}
		else
		{
			keymapFile.AssignFromSlashSeparated(ALITERAL("${") VPATH_APP_KEYMAPS ALITERAL("}/UsQWERTY.cckxw"));
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
		// �L�[�}�b�s���O��`���ǂݍ��߂܂���ł����B
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, NSID_EMSG_LOAD_KEYMAPPINGS,
									MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		ex->Delete();
	}	
}

// ---------------------------------------------------------------------
//! �A�v���P�[�V�����̏�����
/*!
	@retval TRUE ����
	@retval FALSE ���s
*/
// ---------------------------------------------------------------------
BOOL WinCoveredCalcApp::initInstance()
{
	bool langFileLoaded = false;

	if (!base::initInstance())
	{
		return FALSE;
	}

	// �R�����R���g���[��������
	::InitCommonControls();

	// ���C���[�h�E�B���h�E�֘A�� API
	apiLayeredWindow.Initialize();
	
	// �E�F�C�g�J�[�\�����擾���Ă���
	waitCursor = ::LoadCursor(NULL, IDC_WAIT);

	// ���j�^�����擾���Ă���
	monitorInfo.Update();

	// �x�[�X�N���X������
	if (!init())
	{
		return FALSE;
	}

	// �R�}���h���C���p�����[�^���
	CommandLineParam* clParam = GetCommandLineParam();
	clParam->SetParameter(__argc, __targv);

	// ����t�@�C���̓ǂݍ���
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

			// �R�}���h���C���p�����[�^�Ŏw�肳�ꂽ����t�@�C�����ǂݍ��߂Ȃ������̂Ŗ������܂��B
			DoMessageBox(NSID_EMSG_LOAD_COMMANDLINE_LANGFILE, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		}
	}

	// �E�B���h�E�N���X�̓o�^
	WinMainWindow::RegisterClass();

	//�ݒ�t�@�C��������
	Path settingFile;
	if (clParam->IsSettingFileSpecified())
	{
		// �R�}���h���C���Őݒ�t�@�C�����w�肳��Ă���΂�����g��
		settingFile.Assign(clParam->GetSettingFile());
	}
	else
	{
		// �f�t�H���g�ݒ�t�@�C�����g��
		try
		{
			readyDefaultSettingFilePath(settingFile);
		}
		catch (Exception* ex)
		{
			ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, NSID_EMSG_READY_DEFAULT_SETTING_FILE,
											MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
			ex->Delete();
			return FALSE;
		}
	}
	
	// �ݒ�̓ǂݍ���
	try
	{
		loadSettings(settingFile);
	}
	catch (Exception* ex)
	{
		ex->Delete();
		return FALSE;
	}

	// �ݒ�ɕۑ����ꂽ����t�@�C����ǂݍ���
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

					// �ݒ�t�@�C���ɏ����ꂽ����t�@�C�����ǂ߂܂���B
					DoMessageBox(NSID_EMSG_LOAD_SETTING_LANGFILE, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
				}
			}
		}
	}
	
	if (!langFileLoaded)
	{
		// �ݒ�ɕۑ�����Ă��Ȃ���΁A���[�U�[�ɖ₢���킹��H
		// FIXME:
#if 0
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
#endif
	}

	// FIXME: �r���g�C�����Ă̂͂Ȃ��͂��Ȃ̂łǁ[�ɂ�����Ƃ�����I
	if (!langFileLoaded)
	{
		GetAppSettings()->SetLanguageFilePath(AppSettings::Value_LangFileBuiltIn);
	}
	
	// �L�[��`�� DB �̃��[�h
	loadKeyNameDB();
	
	// �L�[�}�b�s���O�ǂݍ���
	loadKeyMappingsOnInit();
	
	// �J�o�[�ǂݍ���
	try
	{
		AppSettings* appSettings = GetAppSettings();
		loadCoverDef(appSettings->GetBaseFolder(), appSettings->GetLastCoverDef(), appSettings->GetLastCoverNo());
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(this, ex);
		ex->Delete();

		// �f�t�H���g�J�o�[�ŕ��������݂�
		if (!restoreByDefaultCoverDef())
		{
			// �_���ł����c�B
			return FALSE;
		}
	}

	// ���C���E�B���h�E����
	DWORD exStyle = 0;
	if (GetAppSettings()->IsMainWindowAlwaysOnTop())
	{
		exStyle = WS_EX_TOPMOST;
	}
	const Point32& lastMainWindowPos = GetAppSettings()->GetLastMainWindowPos();
	if (!mainWindow.CreateEx(exStyle, WinMainWindow::GetWindowClassName(), ALITERAL("CoveredCalc"), WS_SYSMENU | WS_POPUP | WS_MINIMIZEBOX, lastMainWindowPos.x, lastMainWindowPos.y, 0, 0, NULL, NULL))
	{
		// �f�t�H���g�J�o�[�ɂ��čă`�������W
		bool restored = false;
		if (restoreByDefaultCoverDef())
		{
			if (mainWindow.CreateEx(exStyle, WinMainWindow::GetWindowClassName(), ALITERAL("CoveredCalc"), WS_SYSMENU | WS_POPUP | WS_MINIMIZEBOX, lastMainWindowPos.x, lastMainWindowPos.y, 0, 0, NULL, NULL))
			{
				restored = true;
			}
		}
		
		if (!restored)
		{
			DoMessageBox(NSID_EMSG_CREATE_MAIN_WINDOW, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
			return FALSE;
		}
	}
	::ShowWindow(mainWindow.m_hWnd, SW_SHOW);

	// �J�o�[�u���E�U����
	Path baseFolderPath = GetAppSettings()->GetBaseFolder();
	if (baseFolderPath.IsEmpty())
	{
		baseFolderPath = getAppFolderPath();
	}
	coverBrowser.SetCoversFolderPath(baseFolderPath.Append(ALITERAL("Covers")));
	if (!coverBrowser.Create(NULL))
	{
		DoMessageBox(NSID_EMSG_CREATE_COVER_BROWSER, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
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
//! �A�v���P�[�V�����̏I������
// ---------------------------------------------------------------------
void WinCoveredCalcApp::exitInstance()
{
	// ���C���[�h�E�B���h�E�֘A�� API
	apiLayeredWindow.Terminate();

	base::exitInstance();
}

// ---------------------------------------------------------------------
//! �A�v���P�[�V�����̃��C�����b�Z�[�W���[�v
/*!	
	@return �A�v���P�[�V�����̖߂�l
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
 *	@brief	�R���X�g���N�^
 */
WinCoveredCalcApp::MessageFilterManager::MessageFilterManager()
{
	enumerating = 0;
}

/**
 *	@brief	�f�X�g���N�^
 */
WinCoveredCalcApp::MessageFilterManager::~MessageFilterManager()
{
}

/**
 *	@brief	���b�Z�[�W�t�B���^�̃C���X�g�[��
 *	@param[in]	filter	�C���X�g�[������t�B���^
 */
void WinCoveredCalcApp::MessageFilterManager::InstallMessageFilter(WinMessageFilter* filter)
{
	filters.push_back(filter);
}

/**
 *	@brief	���b�Z�[�W�t�B���^�̃A���C���X�g�[��
 *	@param[in]	filter	�A���C���X�g�[������t�B���^
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
 *	@brief	���b�Z�[�W�t�B���^�̌Ăяo��
 *	@param[in]	�Ăяo�����b�Z�[�W
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
 *	@brief	�A���C���X�g�[�����ꂽ�t�B���^�̃A�C�e�����폜���܂��B
 *	@note	CallFilters ���ɍs��ꂽ�t�B���^�̃A���C���X�g�[���ł͂��̎��_�ł�filters���珜�����ꂸ����� NULL ���ݒ肳��܂��B
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
//! �v���O�����̃G���g���|�C���g
/*!	
	@retval 0 ����I��
*/
// ---------------------------------------------------------------------
int APIENTRY _tWinMain(
	HINSTANCE hInstance,		//!< �C���X�^���X�n���h��
	HINSTANCE hPrevInstance,	//!< ���ɑ��݂���C���X�^���X�n���h�� (Win32�ł͏��NULL)
	LPTSTR     lpCmdLine,		//!< �R�}���h���C��������
	int       nCmdShow			//!< �\�����[�h
)
{
	// �f�o�b�O�łŃ��������[�N�����o
	_CrtSetDbgFlag(_crtDbgFlag | _CRTDBG_LEAK_CHECK_DF);
	
	int	retValue = 0;

	// ������Ȃ痎���Ă���Ƃ������Ƃł����Ă����ł͗�O���L���b�`���Ȃ��B
	WinCoveredCalcApp::CreateInstance();
	retValue = WinCoveredCalcApp::GetInstance()->WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	WinCoveredCalcApp::DeleteInstance();

	return retValue;
}
