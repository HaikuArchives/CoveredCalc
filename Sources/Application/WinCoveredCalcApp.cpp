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
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinCoveredCalcApp::~WinCoveredCalcApp()
{
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
 *	@brief	���݂̌���t�@�C���̃C���X�^���X�n���h�����擾���܂��B
 *	@return	�C���X�^���X�n���h��
 */
HINSTANCE WinCoveredCalcApp::GetLangResHandle() const
{
	HINSTANCE hResHandle = langFile.GetHandle();
	if (NULL == hResHandle)		// ����t�@�C����ǂݍ���ł��Ȃ��ꍇ
	{
		return GetInstanceHandle();
	}
	else
	{
		return hResHandle;
	}
}

// ---------------------------------------------------------------------
//! �A�v���P�[�V����������t�H���_�̃p�X���擾���܂��B
// ---------------------------------------------------------------------
void WinCoveredCalcApp::makeAppFolderPath()
{
	AChar moduleFileName[MAX_PATH];
	::GetModuleFileName(NULL, moduleFileName, sizeof(moduleFileName)/sizeof(AChar));
	MBCString appFolderString = PathUtils::RemoveFileSpec(moduleFileName);
	appFolderPath.Assign(appFolderString);
}

// ---------------------------------------------------------------------
//! �w�肵���p�X�̃t�H���_�����݂��Ȃ���΍쐬���܂��B
// ---------------------------------------------------------------------
void WinCoveredCalcApp::createFolder(
	const Path& path				//!< �쐬����t�H���_�̃p�X
)
{
	LPCTSTR pathString = path.GetPathString();
	DWORD attributes = ::GetFileAttributes(pathString);
	if (0xffffffff != attributes)				// ���ɑ��݂���
	{
		if (!(attributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			// �f�B���N�g������Ȃ����Ǒ��݂��Ă���
			throw new FileExceptions::FileAlreadyExists(path);
		}
		
		// ���ɑ��݂���̂ŉ������Ȃ�
		return;
	}
	else										// ���݂��Ȃ���������Ȃ�
	{
		DWORD lastError = ::GetLastError();
		switch (lastError)
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			break;								// ���݂��Ȃ�
		case ERROR_ACCESS_DENIED:
			throw new FileExceptions::AccessDenied(path);
			break;
		default:
			throw new FileException(path);
			break;
		}
	}
	
	ASSERT(!path.IsRoot());		// ���[�g�����݂��Ȃ����Ă��Ƃ͂Ȃ��͂�����...
	if (path.IsRoot())
	{
		return;					// �����ɍċA���ăX�^�b�N�𔲂��Ă��܂�Ȃ��悤�ꉞ�`�F�b�N���Ă���
	}
	
	// �e�t�H���_���Ȃ���΍쐬
	createFolder(path.GetParent());

	// �w�肳�ꂽ�t�H���_���쐬
	if (!::CreateDirectory(pathString, NULL))
	{
		DWORD lastError = ::GetLastError();
		switch (lastError)
		{
		case ERROR_ACCESS_DENIED:
			throw new FileExceptions::AccessDenied(path);
			break;
		case ERROR_WRITE_FAULT:
			throw new FileExceptions::IOError(path);
			break;
		case ERROR_SHARING_VIOLATION:
		case ERROR_LOCK_VIOLATION:
			throw new FileExceptions::SharingViolation(path);
			break;
		case ERROR_ALREADY_EXISTS:
			throw new FileExceptions::FileAlreadyExists(path);
			break;
		default:
			throw new FileException(path);
			break;
		}
	}
}

// ---------------------------------------------------------------------
//! �f�t�H���g�̐ݒ�t�@�C���̃p�X���擾���܂��B�p�X�����݂��Ȃ���΍쐬���s���܂��B
// ---------------------------------------------------------------------
void WinCoveredCalcApp::readyDefaultSettingFilePath(
	Path& settingFilePath			//!< OUTPUT. �f�t�H���g�̐ݒ�t�@�C���̃p�X���Ԃ�܂��B
)
{
	Path path;

	// Application Data �̉�
	ITEMIDLIST* pIdList;
	HRESULT hResult = ::SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pIdList);
	if(S_OK == hResult)
	{
		// ����ꂽ ITEMIDLIST ����t�H���_�����擾
		char pathString[MAX_PATH];
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
		path = path.Append("Hironytic").Append("CoveredCalc");
		createFolder(path);
	}
	
	if (path.IsEmpty())
	{
		// Application Data �������Ȃ�������A�A�v���P�[�V�����̂���t�H���_�ɕۑ�
		char pathString[MAX_PATH];
		::GetModuleFileName(NULL, pathString, sizeof(pathString));
		path.Assign(PathUtils::GetParentDirectory(pathString));
	}

	if (!path.IsEmpty())
	{
		settingFilePath = path.Append("Setting.xml");
	}
	else
	{
		// TODO: ���s�������O�����Ȃ���c
	}
}

/**
 *	@brief	����t�@�C����ǂݍ��݂܂��B
 */
void WinCoveredCalcApp::loadLangFile(
	const Path& path		///< ����t�@�C���̃p�X
)
{
	langFile.Load(path);
	
	if (!langFile.CheckVersion())
	{
		langFile.Unload();
		throw new Exception();	// TODO: �K�؂ȗ�O�𓊂���
	}
	
	// ����R�[�h�����߂�
	MBCString langCode;
	langFile.GetLanguageCode(langCode);
	setCurrentLanguageCode(langCode);
}

/**
 *	@brief	langFileInfos �ɂ��錾��t�@�C���̒����玩���I�ɓK�؂Ȍ���t�@�C����I�����܂��B
 *	@return	langFileInfos �̃C���f�b�N�X (-1�Ȃ�exe���ߍ��݂��g��)
 */
SInt32 WinCoveredCalcApp::autoSelectLangFile()
{
	LANGID userLangID = GetUserDefaultLangID();
	LANGID englishUSLangID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);

	const LangFileInfoCollection* infoCollection = GetLangFileInfos();

	// �V�X�e���̌���Ɠ������̂�T��
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
	
	// �������̂�������Ȃ���΁A�p��iUS�j��I������
	if (-1 != englishUSIndex)
	{
		return englishUSIndex;
	}
	// �p��iUS�j��������Ȃ���΁Aexe���ߍ��݂𗘗p
	else
	{
		return -1;
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

	// �E�F�C�g�J�[�\�����擾���Ă���
	waitCursor = ::LoadCursor(NULL, IDC_WAIT);

	// ���j�^�����擾���Ă���
	monitorInfo.Update();

	// �x�[�X�N���X������
	init();
	setCurrentLanguageCode("jaJP");

	// �A�v���P�[�V����������t�H���_�̃p�X���擾
	makeAppFolderPath();	

	// �R�}���h���C���p�����[�^���
	CommandLineParam* clParam = GetCommandLineParam();
	clParam->SetParameter(__argc, __argv);

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
			MBCString message;
			GetMessageProvider()->GetMessage(GetInstanceHandle(), IDS_EMSG_LOAD_COMMANDLINE_LANGFILE, message);
			DoMessageBox(message, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
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
			ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_READY_DEFAULT_SETTING_FILE,
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

					// ����t�@�C�����ǂ߂܂���B�œK�Ȍ���������I�ɔ��ʂ��ċN�����܂��B
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
		// �ݒ�ɕۑ�����Ă��Ȃ���΁ALangID ����K�؂Ȃ��̂�I������B
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
	if (!mainWindow.CreateEx(exStyle, WinMainWindow::GetWindowClassName(), "CoveredCalc", WS_SYSMENU | WS_POPUP | WS_MINIMIZEBOX, lastMainWindowPos.x, lastMainWindowPos.y, 0, 0, NULL, NULL))
	{
		// �f�t�H���g�J�o�[�ɂ��čă`�������W
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

	// �J�o�[�u���E�U����
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
//! �A�v���P�[�V�����̏I������
// ---------------------------------------------------------------------
void WinCoveredCalcApp::exitInstance()
{
	// ����t�@�C����ǂݍ���ł�����������B
	langFile.Unload();

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
		if (NULL == coverBrowser.m_hWnd || !::IsDialogMessage(coverBrowser.m_hWnd, &msg))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
	return msg.wParam;	
}

// ---------------------------------------------------------------------
//! �v���O�����̃G���g���|�C���g
/*!	
	@retval 0 ����I��
*/
// ---------------------------------------------------------------------
int APIENTRY WinMain(
	HINSTANCE hInstance,		//!< �C���X�^���X�n���h��
	HINSTANCE hPrevInstance,	//!< ���ɑ��݂���C���X�^���X�n���h�� (Win32�ł͏��NULL)
	LPSTR     lpCmdLine,		//!< �R�}���h���C��������
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
