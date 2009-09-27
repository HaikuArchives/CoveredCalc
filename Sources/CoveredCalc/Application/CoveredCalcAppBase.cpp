/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2009 CoveredCalc Project Contributors
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
	@file		CoveredCalcAppBase.cpp
	@brief		Implementation of CoveredCalcAppBase class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.11 created
*/

#include "Prefix.h"
#include "FileException.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcAppBase.h"
#include "KeyMappings.h"
#include "KeyMappingManager.h"
#include "KeyMappingsException.h"
#include "MainWindowKeyFunc.h"
#include "PathException.h"
#include "StorageUtils.h"
#include "VirtualPathNames.h"
#include "XMLLangFile.h"
#include "XMLLangFileException.h"
#include "UTF8Conv.h"
#include "StringID.h"
#include "DialogID.h"
#include "MessageFormatter.h"
#include "DialogLayout.h"

static const AChar	ApplicationName[] = ALITERAL("CoveredCalc");

static const AChar	LangFileFolderName[] = ALITERAL("NLS");				///< the folder which contains language files
static const AChar	KeymapsFolderName[] = ALITERAL("Keymaps");			///< the folder which contains key-mapping files
static const AChar	UserKeymapsFolderName[] = ALITERAL("UserKeymaps");	///< the folder which contains user-defined key-mapping files

static const AChar STR_STRING_NOT_DEFINED[] = ALITERAL("[[%0:s%]]");

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoveredCalcAppBase::CoveredCalcAppBase()
{
	isIgnoreCoverChangeEvent = false;
	coverManager.RegisterCoverChangeEventHandler(this);
	isReadyLangFileInfos = false;
	currentLangCode.Empty();
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoveredCalcAppBase::~CoveredCalcAppBase()
{
	coverManager.UnregisterCoverChangeEventHandler(this);

	LoadedLangFilePtrVector::iterator itr;
	for (itr = langFiles.begin(); itr != langFiles.end(); itr++)
	{
		delete *itr;
	}
}

// ---------------------------------------------------------------------
//! Converts CoverDef.xml file path in relative path to absolute path.
/*!
	@return result absolute path.
	@note	if base path is empty, the folder which application is installed
			is used for base path.
*/
// ---------------------------------------------------------------------
Path CoveredCalcAppBase::makeAbsoluteCoverDefPath(
	const Path& basePath,			//!< base path
	const Path& relativePath		//!< relative path
)
{
	if (basePath.IsEmpty())
	{
		return getAppFolderPath().Append(relativePath);
	}
	else
	{
		return basePath.Append(relativePath);
	}
}

// ---------------------------------------------------------------------
//! Converts CoverDef.xml file path in absolute path to relative path.
/*!
	@return result relative path.
	@note	if base path is empty, the folder which application is installed
			is used for base path.
*/
// ---------------------------------------------------------------------
Path CoveredCalcAppBase::makeRelativeCoverDefPath(
	const Path& basePath,			//!< base path
	const Path& absolutePath		//!< absolute path
)
{
	Path ret;
	if (basePath.IsEmpty())
	{
		getAppFolderPath().MakeRelativePath(absolutePath, ret);
	}
	else
	{
		basePath.MakeRelativePath(absolutePath, ret);
	}
	return ret;
}

/**
 *	@brief	Converts language file path in absolute path to relative path.
 *	@return	result relative path.
 */
Path CoveredCalcAppBase::MakeRelativeLangFilePath(
	const Path& absolutePath		///< absolute path
)
{
	Path ret;
	if (absolutePath.IsEmpty())
	{
		ret.Empty();
	}
	else
	{
		Path langFileFolderPath = getAppFolderPath().Append(LangFileFolderName);
		langFileFolderPath.MakeRelativePath(absolutePath, ret);
	}
	return ret;
}

/**
 *	@breif	Converts language file path in relative path to absolute path.
 *	@return	result absolute path.
 */
Path CoveredCalcAppBase::MakeAbsoluteLangFilePath(
	const Path& relativePath		///< relative path
)
{
	Path ret;
	if (relativePath.IsEmpty())
	{
		ret.Empty();
	}
	else
	{
		Path langFileFolderPath = getAppFolderPath().Append(LangFileFolderName);		
		ret = langFileFolderPath.Append(relativePath);
	}
	return ret;
}

/**
 *	@brief	Retrieves the language code of current language.
 */
void CoveredCalcAppBase::GetCurrentLanguageCode(
	MBCString& outLanguage		///< the string is returned, which represents language code. (OUTPUT)
)
{
	outLanguage = currentLangCode;
}

/**
 *	@brief	Get absolute path for specified virtual path name.
 *	@param[in]	virtualPathName	name of virtual path.
 *				e.g. the name of the path "${Hoge}" is "Hoge".
 *	@return		result absolute path.
 *	@throw		PathExceptions::VirtualPathNameNotInterpreted
 */
Path CoveredCalcAppBase::resolveVirtualPathName(const MBCString& virtualPathName)
{
	if (0 == virtualPathName.CompareNoCase(VPATH_APP_KEYMAPS))
	{
		return getAppFolderPath().Append(KeymapsFolderName);
	}
	else if (0 == virtualPathName.CompareNoCase(VPATH_USER_KEYMAPS))
	{
		return getUserSettingsPath().Append(UserKeymapsFolderName);
	}
	else
	{
		throw new PathExceptions::VirtualPathNameNotInterpreted(virtualPathName);
	}
}

/**
 *	@brief	Converts virtual path to absolute path.
 *	@param[in]	virtrualPath	virtual path.
 *	@return	result absolute path.
 */
Path CoveredCalcAppBase::ExpandVirtualPath(const Path& virtualPath)
{
	if (virtualPath.IsEmpty())
	{
		return virtualPath;
	}
	if (virtualPath.IsRoot())
	{
		return virtualPath;
	}

	MBCString fileName;
	virtualPath.GetFileName(fileName);
	SInt32 length = fileName.Length();
	if (length > 4 &&
		'$' == fileName.GetAt(0) &&
		'{' == fileName.GetAt(1) &&
		'}' == fileName.GetAt(length - 1))		// "${virtualname}"
	{
		MBCString virtualPathName = fileName.SubString(2, fileName.Length() - 3);
		try
		{
			return resolveVirtualPathName(virtualPathName);
		}
		catch (PathExceptions::VirtualPathNameNotInterpreted* ex)
		{
			ex->Delete();
			return virtualPath;
		}
	}
	else
	{
		return ExpandVirtualPath(virtualPath.GetParent()).Append(fileName);
	}
}

/**
 *	@brief	Replaces a part of specified absolute path with virtual path placeholder (i.e. ${virtualname}).
 *	@param[in]	absolutePath	absolute path to replace.
 *	@param[in]	virtualPathName	name of virtual path.
 *	@return	result virtual path. if it couldn't make virtual path empty path is returned.
 */
Path CoveredCalcAppBase::MakeVirtualPath(const Path& absolutePath, ConstAStr virtualPathName)
{
	try
	{
		Path virtualPathTop = resolveVirtualPathName(virtualPathName);
		Path relativePath;
		virtualPathTop.MakeRelativePath(absolutePath, relativePath);
		
		MBCString placeholder = ALITERAL("${");
		placeholder += virtualPathName;
		placeholder += ALITERAL("}");
		
		return Path(placeholder).Append(relativePath);
	}
	catch (PathException* ex)
	{
		ex->Delete();
		return Path();
	}
}

/**
 *	@brief	Retrives default setting file path. It also creates setting folder if not exist.
 *	@param[out]	settingFilePath	a path of default setting file is returned.
 */
void CoveredCalcAppBase::readyDefaultSettingFilePath(Path& settingFilePath)
{
	const Path path = getUserSettingsPath();
	if (!path.IsEmpty())
	{
		StorageUtils::ReadyFolder(path);
		settingFilePath = path.Append(ALITERAL("Setting.xml"));
	}
}

/**
 *	@brief	Loads key-mapping definition.
 *	@param[in]	keymapFile keymap file. (absolute path)
 */
void CoveredCalcAppBase::LoadKeyMappings(const Path& keymapFile)
{
	// clear managers
	KeyMappingManager*	mainWindowKMM = GetKeyMappingManagerForMainWindow();
	mainWindowKMM->Clear();

	// load keymap file.	
	KeyMappings keyMappings;
	keyMappings.Load(keymapFile);
	
	// check platform
	UTF8String platform;
	if (!keyMappings.GetPlatform(platform) || !CheckPlatform(platform))
	{
		throw new KeyMappingsExceptions::LoadFailed(ALITERAL("The key-mapping definition is not for this platform."));
	}

	// initialize managers.
	// --- main window.
	{
		MainWindowKeyFunc keyFunc;
		mainWindowKMM->Create(&keyMappings, KeyMappings::STR_CATEGORY_MAIN_WINDOW, &keyFunc);
	}		
}

/**
 *	@brief	Initializes the object.
 *	@return false when the initialization is failed.
 */
bool CoveredCalcAppBase::init()
{
	// load default language file
	const LangFileInfoCollection* infoCollection = GetLangFileInfos();
	SInt32 defaultLangFileIndex = infoCollection->GetDefaultLangFileIndex();
	if (defaultLangFileIndex < 0 || defaultLangFileIndex >= infoCollection->GetCount())
	{
		DoMessageBox(NSID_EMSG_LOAD_DEFAULT_LANGFILE, ButtonType_OK, AlertType_Stop);
		return false;
	}
	try
	{
		const LangFileInfo& langFileInfo = infoCollection->GetAt(defaultLangFileIndex);
		loadLangFile(langFileInfo.GetPath());
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, NSID_EMSG_LOAD_DEFAULT_LANGFILE, ButtonType_OK, AlertType_Stop); 
		ex->Delete();
		return false;
	}

	// ready ExceptionMessageGenerator object
	exMessageGenerator.Init(this);

	return true;
}

// ---------------------------------------------------------------------
//! Loads setting from setting file
// ---------------------------------------------------------------------
void CoveredCalcAppBase::loadSettings(
	const Path& settingFile			//!< file path of setting file
)
{
	try
	{
		appSettings.Load(settingFile);
	}
	catch (FileExceptions::FileNotFound* ex)
	{
		ex->Delete();
		appSettings.LoadDefault();
		try
		{
			appSettings.SaveAs(settingFile);
		}
		catch (Exception* ex)
		{
			ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, NSID_EMSG_SAVE_SETTING);
			ex->Delete();
		}
	}
	catch (Exception *ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, NSID_EMSG_LOAD_SETTING);
		throw;
	}
}

// ---------------------------------------------------------------------
//! Loads default CoverDef file to restore from broken cover
/*!
	@retval	true	success
	@retval	false	failed
*/
// ---------------------------------------------------------------------
bool CoveredCalcAppBase::restoreByDefaultCoverDef()
{
	// get default cover path
	AppSettings defaultSettings;
	defaultSettings.LoadDefault();
	AppSettings* appSettings = GetAppSettings();
	if (0 == appSettings->GetBaseFolder().Compare(defaultSettings.GetBaseFolder()) &&
			0 == appSettings->GetLastCoverDef().Compare(defaultSettings.GetLastCoverDef()))
	{
		// already default cover. give up.
		return false;
	}
	
	// load default cover
	try
	{
		loadCoverDef(defaultSettings.GetBaseFolder(), defaultSettings.GetLastCoverDef(), defaultSettings.GetLastCoverNo());
	}
	catch (Exception* ex2ndTry)
	{
		// give up when failed again.
		ex2ndTry->Delete();
		return false;
	}

	// when OK, change settings to use default cover.
	appSettings->SetBaseFolder(defaultSettings.GetBaseFolder());
	appSettings->SetLastCoverDef(defaultSettings.GetLastCoverDef());
	appSettings->SetLastCoverNo(defaultSettings.GetLastCoverNo());
	try
	{
		appSettings->Save();
	}
	catch (Exception* exSave)
	{
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, exSave, NSID_EMSG_SAVE_SETTING);
		exSave->Delete();
	}
	
	return true;
}

// ---------------------------------------------------------------------
//! Loads CoverDef file
// ---------------------------------------------------------------------
void CoveredCalcAppBase::loadCoverDef(
	const Path& basePath,					//!< file path of base folder.
	const Path& relativeCoverDefPath,		//!< file path of CoverDef.xml in relative path from base folder
	SInt32 coverNo							//!< cover no
)
{
	isIgnoreCoverChangeEvent = true;
	try
	{
		const Path absolutePath = makeAbsoluteCoverDefPath(basePath, relativeCoverDefPath);
		coverManager.Load(absolutePath, coverNo);
	}
	catch (Exception* /*ex*/)
	{
		isIgnoreCoverChangeEvent = false;
		throw;
	}
	isIgnoreCoverChangeEvent = false;
}

/**
 *	@brief	Loads installed language file informations to langFileInfos.
 *			If already loaded, it doesn't do nothing.
 */
void CoveredCalcAppBase::readyLangFileInfos()
{
	if (isReadyLangFileInfos)
		return;

	langFileInfos.Load(getAppFolderPath().Append(LangFileFolderName));
	isReadyLangFileInfos = true;
}

/**
 *	@brief	Returns installed langauge file informations.
 *	@return	collection of the language file informations.
 */
const LangFileInfoCollection* CoveredCalcAppBase::GetLangFileInfos()
{
	readyLangFileInfos();
	return &langFileInfos;
}

// ---------------------------------------------------------------------
//! Called when cover definition is changed.
// ---------------------------------------------------------------------
void CoveredCalcAppBase::CoverDefChanged()
{
	if (isIgnoreCoverChangeEvent)
	{
		return;
	}

	appSettings.SetLastCoverDef(makeRelativeCoverDefPath(GetAppSettings()->GetBaseFolder(), coverManager.GetCoverDefFilePath()));
	appSettings.SetLastCoverNo(coverManager.GetCurrentCover());
	try
	{
		appSettings.Save();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, NSID_EMSG_SAVE_SETTING);
		ex->Delete();
	}
}

// ---------------------------------------------------------------------
//! Called when current cover number is changed.
// ---------------------------------------------------------------------
void CoveredCalcAppBase::CurrentCoverChanged()
{
	if (isIgnoreCoverChangeEvent)
	{
		return;
	}

	appSettings.SetLastCoverNo(coverManager.GetCurrentCover());
	try
	{
		appSettings.Save();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, NSID_EMSG_SAVE_SETTING);
		ex->Delete();		
	}	
}

/**
 *	@brief	Load keynameDB
 */
void CoveredCalcAppBase::loadKeyNameDB(const Path& keyNameDefFile)
{
	keyNameDB.Init(keyNameDefFile);
}

/**
 *	@brief	Loads a language file.
 *	@param[in]	path	path of language file
 */
void CoveredCalcAppBase::loadLangFile(const Path& path)
{
	loadOneLangFile(path, langFiles.end());

	int count = langFiles.size();
	if (0 < count)
	{
		LoadedLangFile* llf = langFiles[count - 1];

		MBCString langCode;
		llf->langFile.GetLanguageCode(langCode);
		setCurrentLanguageCode(langCode);
	}
}

void CoveredCalcAppBase::loadOneLangFile(const Path& path, LoadedLangFilePtrVector::iterator insertBeforePos)
{
	// check if the language file is already loaded.
	LoadedLangFilePtrVector::const_iterator itr;
	for (itr = langFiles.begin(); itr != langFiles.end(); itr++)
	{
		if (0 == path.Compare((*itr)->path))
		{
			// already loaded
			return;
		}
	}

	LoadedLangFile* llf = new LoadedLangFile();
	try
	{
		llf->path = path;
		llf->langFile.Load(path);
	}
	catch (Exception*)
	{
		delete llf;
		throw;
	}
	LoadedLangFilePtrVector::iterator insertedPos = langFiles.insert(insertBeforePos, llf);
	MBCString baseDefSource;
	llf->langFile.GetBaseDefSource(baseDefSource);
	if (!baseDefSource.IsEmpty())
	{
			Path basePath(baseDefSource.CString());
			Path baseFullPath = MakeAbsoluteLangFilePath(basePath);
			if (!baseFullPath.IsEmpty())
			{
				loadOneLangFile(baseFullPath, insertedPos);
			}
	}
}


/**
 *	@brief	Loads native string
 *	@param[in] stringId	string ID.
 *	@return	loaded string
 *	@throw XMLLangFileExceptions::StringNotDefined when the string is not defined in language file.
 */
MBCString CoveredCalcAppBase::loadNativeStringSub(SInt32 stringId)
{
	switch (stringId)
	{
	case NSID_EMSG_LOAD_COMMANDLINE_LANGFILE:
		return ALITERAL("Failed to load the language file specified as a command line parameter.\nThe parameter is ignored.");
		break;
	case NSID_EMSG_LOAD_SETTING_LANGFILE:
		return ALITERAL("Failed to load the language file specified in your settings.");
		break;
	case NSID_EMSG_LOAD_DEFAULT_LANGFILE:
		return ALITERAL("Failed to load the default language file.");
		break;
	case NSID_EMSG_LOAD_LANGFILE:
		return ALITERAL("Failed to load the language file.");
		break;
	default:
		ConstUTF8Str name = ConvertStringID(stringId);	
		MBCString retString;

		LoadedLangFilePtrVector::const_reverse_iterator itr;
		for (itr = langFiles.rbegin(); itr != static_cast<LoadedLangFilePtrVector::const_reverse_iterator>(langFiles.rend()); ++itr)
		{
			const XMLLangFile& langFile = (*itr)->langFile;
			if (langFile.LoadString(name, retString))
			{
				return retString;
			}
		}

		MBCString mbcName;
		UTF8Conv::ToMultiByte(mbcName, name);
		throw new XMLLangFileExceptions::StringNotDefined(mbcName.CString());
	}
}

/**
 *	@brief	Loads native string
 *	@param[in] stringId	string ID.
 *	@return	loaded string
 */
MBCString CoveredCalcAppBase::LoadNativeString(SInt32 stringId)
{
	try
	{
		return loadNativeStringSub(stringId);
	}
	catch (XMLLangFileExceptions::StringNotDefined* ex)
	{
		// handle special case.
		if (NSID_APPNAME == stringId)
		{
			ex->Delete();
			return ApplicationName;
		}

		MBCString errorFormat;
		try
		{
			errorFormat = loadNativeStringSub(NSID_EMSG_LANG_STRING_NOT_DEFINED);
		}
		catch (XMLLangFileExceptions::StringNotDefined* ex2)
		{
			ex2->Delete();
			errorFormat = STR_STRING_NOT_DEFINED;
		}
		MBCString message;
		MessageFormatter::Format(message, errorFormat, ex->GetName());
		ex->Delete();
		return message;
	}
}

/**
 *	@brief	Loads dialog layout
 *	@param[in] dialogId	dialog ID.
 *	@param[out] outLayout the loaded layout is stored to this object.
 */
void CoveredCalcAppBase::LoadDialogLayout(SInt32 dialogId, DialogLayout& outLayout)
{
	outLayout.ClearAllLayoutItems();

	ConstUTF8Str name = ConvertDialogID(dialogId);
	LoadedLangFilePtrVector::const_iterator itr;
	for (itr = langFiles.begin(); itr != langFiles.end(); itr++)
	{
		const XMLLangFile& langFile = (*itr)->langFile;
		langFile.LoadDialogLayout(name, outLayout);
	}
}
