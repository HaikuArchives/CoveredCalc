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
	@file		PreferencesDlg.cpp
	@brief		Implementation of PreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.1.17 created
*/

#include "Prefix.h"
#include "PreferencesDlg.h"
#include "CoveredCalcApp.h"
#include "AppSettings.h"
#include "KeyMappings.h"
#include "UTF8Conv.h"
#include "Exception.h"
#include "VirtualPathNames.h"
#include <vector>
#include <algorithm>
#include "UIMessageProvider.h"
#include "ExceptionMessageUtils.h"
#include "FileException.h"
#include <time.h>
#include "StorageUtils.h"

#if defined (WIN32)
#include "WinCoveredCalcApp.h"
#endif	// defined (WIN32)

#if defined (BEOS)
#include <Directory.h>
#include <Entry.h>
#include <Path.h>
#endif	// defined (BEOS)

const AChar STR_USER_KEYMAP_PREFIX[] = ALITERAL("User");
#if defined (WIN32)
const AChar STR_USER_KEYMAP_POSTFIX[] = ALITERAL(".cckxw");
#elif defined (BEOS)
const AChar STR_USER_KEYMAP_POSTFIX[] = ALITERAL(".cckxb");
#endif

/**
 *	@brief	Constructor
 */
PreferencesDlg::PreferencesDlg()
{
}

/**
 *	@brief	Destructor
 */
PreferencesDlg::~PreferencesDlg()
{
	unloadKeyMappingsInfos();
}

/**
 *	@brief	Loads preferences data to this dialog.
 */
void PreferencesDlg::loadToDialog()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();
	ASSERT(NULL != app);
	AppSettings* appSettings = app->GetAppSettings();
	ASSERT(NULL != appSettings);

	// opacity and edge-smoothing
#if defined(WIN32)
	setOpacity(appSettings->GetMainWindowOpacity());
	setEdgeSmoothing(appSettings->GetMainWindowEdgeSmoothing());
	WinCoveredCalcApp* winApp = static_cast<WinCoveredCalcApp*>(app);
	const WinLayeredWindowAPI* lwApi = winApp->GetLayeredWindowAPI();
	if (NULL != lwApi && lwApi->IsSupported_UpdateLayeredWindow())
	{
		enableOpacity(true);
		enableEdgeSmoothing(true);
	}
	else
	{
		enableOpacity(false);
		enableEdgeSmoothing(false);
	}
#endif

	// language
	const LangFileInfoCollection* langFileInfos = app->GetLangFileInfos();
	ASSERT(NULL != langFileInfos);
	if (NULL != langFileInfos)
	{
		setLanguage(*langFileInfos, app->MakeAbsoluteLangFilePath(appSettings->GetLanguageFilePath())
#if defined(ZETA)
				, CoveredCalcApp::GetInstance()->GetAppSettings()->IsLocaleKitAvailable()
#endif // defined(ZETA)
		);
	}
	
	// key-mapping
	loadKeyMappingsInfos();
	setKeyMapping(keyMappingsInfos, appSettings->GetKeymapFilePath());
}

/**
 *	@brief	Saves preferences data from this dialog.
 *	@return	return false if error has occurred.
 */
bool PreferencesDlg::saveFromDialog()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();
	ASSERT(NULL != app);
	AppSettings* appSettings = app->GetAppSettings();
	ASSERT(NULL != appSettings);

	// opacity and edge-smoothing
#if defined(WIN32)
	SInt32 opacity = getOpacity();
	SInt32 edgeSmoothing = getEdgeSmoothing();
#endif // defined(WIN32)	

	// language
	Path langFilePath;
#if defined(ZETA)
	bool isLocaleKitAvailable = false;
#endif // defined(ZETA)
	if (!getLanguage(langFilePath
#if defined(ZETA)
			, isLocaleKitAvailable
#endif // defined(ZETA)
		))
	{
		return false;
	}
	
	// key-mapping
	Path keyMappingFilePath;
	const KeyMappingsInfo* info = getKeyMapping(true);
	if (NULL == info)
	{
		return false;
	}
	keyMappingFilePath = info->keyMapFilePath;
	
	// set values to appSettings
#if defined(WIN32)
	appSettings->SetMainWindowOpacity(opacity);
	appSettings->SetMainWindowEdgeSmoothing(edgeSmoothing);
#endif // defined(WIN32)	
	appSettings->SetLanguageFilePath(app->MakeRelativeLangFilePath(langFilePath));
#if defined(ZETA)
	appSettings->SetLocaleKitAvailable(isLocaleKitAvailable);
#endif // defined(ZETA)
	appSettings->SetKeymapFilePath(keyMappingFilePath);

	appSettings->Save();

	// load key-mapping again.
	app->LoadKeyMappings(app->ExpandVirtualPath(keyMappingFilePath));

	return true;
}

class KeyMappingsInfoComparator
{
public:
	bool operator()(PreferencesDlg::KeyMappingsInfo* first, PreferencesDlg::KeyMappingsInfo* second)
	{
		if (first->category > second->category)
		{
			return false;
		}
		else if (first->category < second->category)
		{
			return true;
		}
		else
		{
			return (0 > first->title.Compare(second->title));
		}
	}
};

/**
 *	@brief	Loads informations about installed key-mapping files.
 */
void PreferencesDlg::loadKeyMappingsInfos()
{
	unloadKeyMappingsInfos();
	
	// load files in ${AppKeymaps} folder.
	Path virtualAppKeymaps(ALITERAL("${") VPATH_APP_KEYMAPS ALITERAL("}"));
	loadKeyMappingsInfosInFolder(virtualAppKeymaps, KMCategory_Application);
	
	// load files in ${UserKeymaps} folder.
	Path virtualUserKeymaps(ALITERAL("${") VPATH_USER_KEYMAPS ALITERAL("}"));
	loadKeyMappingsInfosInFolder(virtualUserKeymaps, KMCategory_User);
	
	// sort
	std::sort(keyMappingsInfos.begin(), keyMappingsInfos.end(), KeyMappingsInfoComparator());	
}

/**
 *	@breif	Loads informations about installed key-mapping files in specified folder.
 *	@param[in]	virtualFolderPath	folder (in virtual path)
 *	@param[in]	category	category of loading informations.
 */
void PreferencesDlg::loadKeyMappingsInfosInFolder(const Path& virtualFolderPath, PreferencesDlg::KMCategory category)
{
	Path folder = CoveredCalcApp::GetInstance()->ExpandVirtualPath(virtualFolderPath);

#if defined (WIN32)
	Path findPath = folder.Append(ALITERAL("*.cckxw"));
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile(findPath.GetPathString(), &findData);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				continue;
			}
			loadOneKeyMappingsInfo(folder.Append(findData.cFileName), virtualFolderPath.Append(findData.cFileName), category);
		}
		while (::FindNextFile(hFind, &findData));
		::FindClose(hFind);
	}
#elif defined (BEOS)
	BDirectory findDir(folder.GetPathString());
	if (B_OK == findDir.InitCheck())
	{
		BEntry entry;
		char filename[B_FILE_NAME_LENGTH];

		findDir.Rewind();
		while (B_NO_ERROR == findDir.GetNextEntry(&entry))
		{
			if (!entry.IsDirectory())
			{
				entry.GetName(filename);
				SInt32 length = strlen(filename);
				if (6 < length && 0 == strcmp(filename + length - 6, ALITERAL(".cckxb")))
				{
					BPath bpath;
					entry.GetPath(&bpath);
					Path keymapFilePath(bpath.Path());
					loadOneKeyMappingsInfo(keymapFilePath, virtualFolderPath.Append(filename), category);
				}
			}
		}
	}
#endif	
}

/**
 *	@brief	Loads and validates specified key-mapping file, and append it to keyMappingsInfos if it is valid.
 *	@param[in]	realKeymapFilePath		path of a key-mapping file (in real path)
 *	@param[in]	virtualKeymapFilePath	path of a key-mapping file (in virtual path)
 *	@param[in]	category	category of loading key-mapping.
 */
void PreferencesDlg::loadOneKeyMappingsInfo(const Path& realKeymapFilePath, const Path& virtualKeymapFilePath, PreferencesDlg::KMCategory category)
{
	try
	{
		KeyMappings keyMappings;
		keyMappings.Load(realKeymapFilePath);
		UTF8String platform;
		if (!keyMappings.GetPlatform(platform) || !CoveredCalcApp::GetInstance()->CheckPlatform(platform))
		{
			return;
		}
		
		UTF8String utf8Title;
		keyMappings.GetTitle(utf8Title);
		
		KeyMappingsInfo* info = new KeyMappingsInfo;
		info->category = category;
		UTF8Conv::ToMultiByte(info->title, utf8Title);
		info->keyMapFilePath = virtualKeymapFilePath;
		keyMappingsInfos.push_back(info);
	}
	catch (Exception* ex)
	{
		// ignore
		ex->Delete();
	}			
}

/**
 *	@brief	Unload informations about installed key-mapping files.
 */
void PreferencesDlg::unloadKeyMappingsInfos()
{
	KeyMappingsInfoPtrVector::iterator ite;
	for (ite = keyMappingsInfos.begin(); ite != keyMappingsInfos.end(); ite++)
	{
		KeyMappingsInfo* info = *ite;
		delete info;
	}
	keyMappingsInfos.clear();
}

/**
 *	@brief	Called when key-mapping selection has been changed.
 */
void PreferencesDlg::processKeyMappingSelectionChanged()
{
	const KeyMappingsInfo* currentInfo = getKeyMapping(false);
	if (NULL == currentInfo || KMCategory_Invalid == currentInfo->category)
	{
		enableEditKeyMapping(false);
		enableDuplicateKeyMapping(false);
		enableDeleteKeyMapping(false);
		return;
	}
	
	if (KMCategory_Application == currentInfo->category)
	{
		enableEditKeyMapping(true);
		enableDuplicateKeyMapping(true);
		enableDeleteKeyMapping(false);
	}
	else
	{
		enableEditKeyMapping(true);
		enableDuplicateKeyMapping(true);
		enableDeleteKeyMapping(true);
	}
}

/**
 *	@brief	Shows a dialog which edit the current key-mapping.
 */
void PreferencesDlg::doEditKeyMapping()
{
	const KeyMappingsInfo* currentInfo = getKeyMapping(false);
	if (NULL == currentInfo)
	{
		return;
	}
	
	try
	{	
		// load
		Path virtualPath = currentInfo->keyMapFilePath;
		Path filePath = CoveredCalcApp::GetInstance()->ExpandVirtualPath(virtualPath);
		KeyMappings keyMappings;
		keyMappings.Load(filePath);
		
		bool isReadOnly = (currentInfo->category == KMCategory_User) ? false : true;
		if (showEditKeyMapDialog(isReadOnly, keyMappings))
		{
			if (!isReadOnly)
			{
				// save to file.
				keyMappings.Save(filePath);

				// update UI
				loadKeyMappingsInfos();
				setKeyMapping(keyMappingsInfos, virtualPath);
			}
		}
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(getMessageBoxProvider(), ex);
		ex->Delete();
	}
}

/**
 *	@brief	Duplicates current key-mapping and select it.
 */
void PreferencesDlg::doDuplicateKeyMapping()
{
	const KeyMappingsInfo* currentInfo = getKeyMapping(false);
	if (NULL == currentInfo)
	{
		return;
	}

	try
	{	
		// load
		Path originalFilePath = CoveredCalcApp::GetInstance()->ExpandVirtualPath(currentInfo->keyMapFilePath);
		KeyMappings keyMappings;
		keyMappings.Load(originalFilePath);
		
		// change title
		UTF8String title;
		keyMappings.GetTitle(title);
		MBCString postStr;
		CoveredCalcApp::GetInstance()->GetMessageProvider()->GetMessage(IDS_KEYMAPPINGS_COPY, postStr);
		UTF8String utf8PostStr;
		UTF8Conv::FromMultiByte(utf8PostStr, postStr);
		title += utf8PostStr;
		keyMappings.SetTitle(title);
		
		// store to new file.
		Path virtualUserKeymaps(ALITERAL("${") VPATH_USER_KEYMAPS ALITERAL("}"));
		Path folderPath = CoveredCalcApp::GetInstance()->ExpandVirtualPath(virtualUserKeymaps);
		StorageUtils::ReadyFolder(folderPath);
		Path filePath = createUniqueUserKeyMappingFile(folderPath);
		keyMappings.Save(filePath);
		MBCString fileName;
		filePath.GetFileName(fileName);
		Path virtualPath = virtualUserKeymaps.Append(fileName);
		
		// update UI
		loadKeyMappingsInfos();
		setKeyMapping(keyMappingsInfos, virtualPath);
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(getMessageBoxProvider(), ex);
		ex->Delete();
	}
}

/**
 *	@brief	Creates an empty user key-mapping file and returns its file path.
 *	@param[in]	folderPath	user key-mapping folder (real path)
 */
Path PreferencesDlg::createUniqueUserKeyMappingFile(const Path& folderPath)
{
	UInt32 num = static_cast<UInt32>(time(NULL));
	
	SInt32 retryCount = 0;
	for (retryCount = 0; retryCount < 64; retryCount++)
	{
		AChar buf[64];
		sntprintf(buf, sizeof(buf)/sizeof(AChar), ALITERAL("%s%lx%s"), STR_USER_KEYMAP_PREFIX, num, STR_USER_KEYMAP_POSTFIX);
		Path filePath = folderPath.Append(buf);
		try
		{
			File file;
			file.Open(filePath, FileConstants::OpenMode_WriteOnly, FileConstants::OpenOption_Create | FileConstants::OpenOption_FailIfExists);
			file.Close();
			return filePath;
		}
		catch (FileExceptions::FileAlreadyExists* ex)
		{
			ex->Delete();
		}
		catch (FileExceptions::AccessDenied* ex)
		{
			ex->Delete();
		}
		catch (FileExceptions::SharingViolation* ex)
		{
			ex->Delete();
		}
		num += 31;
	}
	throw new FileException();
}

/**
 *	@brief	Deletes current key-mapping.
 */
void PreferencesDlg::doDeleteKeyMapping()
{
	const KeyMappingsInfo* currentInfo = getKeyMapping(false);
	if (NULL == currentInfo || KMCategory_User != currentInfo->category)
	{
		return;
	}

	UInt32 infosIndex;
	for (infosIndex = 0; infosIndex < keyMappingsInfos.size(); infosIndex++)
	{
		if (keyMappingsInfos[infosIndex] == currentInfo)
		{
			break;
		}
	}
	if (infosIndex >= keyMappingsInfos.size())
	{
		return;
	}

	// stop if key-mapping in use is being deleted. because this dialog can be closed by "Cancel" button.
	MBCString message;
	if (0 == CoveredCalcApp::GetInstance()->GetAppSettings()->GetKeymapFilePath().Compare(currentInfo->keyMapFilePath))
	{
		CoveredCalcApp::GetInstance()->GetMessageProvider()->GetNFormatMessage(message, IDS_EMSG_DELETE_KEYMAPPINGS_IN_USE, currentInfo->title.CString());
		getMessageBoxProvider()->DoMessageBox(
			message,
			MessageBoxProvider::ButtonType_OK,
			MessageBoxProvider::AlertType_Warning);
		return;
	}

	CoveredCalcApp::GetInstance()->GetMessageProvider()->GetNFormatMessage(message, IDS_QMSG_DELETE_KEYMAPPINGS, currentInfo->title.CString());
	MessageBoxProvider::Button button = getMessageBoxProvider()->DoMessageBox(
			message,
			MessageBoxProvider::ButtonType_YesNo,
			MessageBoxProvider::AlertType_Warning,
			MessageBoxProvider::Button_No);
	
	if (button == MessageBoxProvider::Button_Yes)
	{
		Path realPath = CoveredCalcApp::GetInstance()->ExpandVirtualPath(currentInfo->keyMapFilePath);
		try
		{
			File::Remove(realPath);
		}
		catch (FileException* ex)
		{
			ExceptionMessageUtils::DoExceptionMessageBox(getMessageBoxProvider(), ex,
					MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
			ex->Delete();
			return;
		}

		keyMappingsInfos.erase(keyMappingsInfos.begin() + infosIndex);
		if (keyMappingsInfos.size() <= infosIndex)
		{
			infosIndex = keyMappingsInfos.size() - 1;
		}
		setKeyMapping(keyMappingsInfos, keyMappingsInfos[infosIndex]->keyMapFilePath);

		delete currentInfo;
	}
}
