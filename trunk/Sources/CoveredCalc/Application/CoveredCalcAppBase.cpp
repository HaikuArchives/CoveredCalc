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
	@file		CoveredCalcAppBase.cpp
	@brief		Implementation of CoveredCalcAppBase class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.11 created
*/

#include "Prefix.h"
#include "FileException.h"
#include "UIMessageProvider.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcAppBase.h"
#include "KeyMappings.h"
#include "KeyMappingManager.h"
#include "KeyMappingsException.h"
#include "MainWindowKeyFunc.h"

static const AChar	LangFileFolderName[] = "NLS";		///< the folder which contains language files
static const AChar	KeymapsFolderName[] = "Keymaps";	///< the folder which contains key-mapping files

static const UTF8Char STR_CATEGORY_MAIN_WINDOW[] = "MainWindow";	///< keymap category of main window.

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
		ret = AppSettings::Value_LangFileBuiltIn;
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
	if (0 == relativePath.Compare(AppSettings::Value_LangFileBuiltIn))
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
 *	@brief	Converts virtual kay-mapping file path to absolute path.
 *	@param[in]	virtrualPath	virtual path.
 *	@return	result absolute path.
 */
Path CoveredCalcAppBase::ExpandVirtualKeymapFilePath(const Path& virtualPath)
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
	if (0 == fileName.CompareNoCase("${AppKeymaps}"))
	{
		return getAppFolderPath().Append(KeymapsFolderName);
	}
//	else if (0 == fileName.CompareNoCase("${UserKeymaps}"))
//	{
//		return Path();	// TODO:
//	}
	else
	{
		return ExpandVirtualKeymapFilePath(virtualPath.GetParent()).Append(fileName);
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
	checkKeymappingsPlatform(&keyMappings);

	// initialize managers.
	// --- main window.
	{
		MainWindowKeyFunc keyFunc;
		mainWindowKMM->Create(&keyMappings, STR_CATEGORY_MAIN_WINDOW, &keyFunc);
	}		
}

// ---------------------------------------------------------------------
//! Initializes the object
// ---------------------------------------------------------------------
void CoveredCalcAppBase::init()
{
	uiMessageProvider.Init();
	exMessageGenerator.Init(&uiMessageProvider);
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
			ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_SAVE_SETTING);
			ex->Delete();
		}
	}
	catch (Exception *ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_LOAD_SETTING);
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
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, exSave, IDS_EMSG_SAVE_SETTING);
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
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_SAVE_SETTING);
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
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(this, ex, IDS_EMSG_SAVE_SETTING);
		ex->Delete();		
	}	
}
