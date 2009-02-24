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
#include "ExceptionMessageUtils.h"
#include "FileException.h"
#include <time.h>
#include "StorageUtils.h"
#include "MessageFormatter.h"
#include "StringID.h"
#include "UICButton.h"
#include "UICEventCode.h"

#if defined (WIN32)
#include "WinCoveredCalcApp.h"
#include "UICSlider.h"
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

#if defined (WIN32)
#define MIN_OPACITY				25
#define MAX_OPACITY				255
#define TICK_OPACITY			11
#define MIN_EDGE_SMOOTHING		0
#define MAX_EDGE_SMOOTHING		10
#define TICK_EDGE_SMOOTHING		11
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
}

/**
 *	@brief	Loads preferences data to this dialog.
 */
void PreferencesDlg::readyToShow()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();
	ASSERT(NULL != app);
	AppSettings* appSettings = app->GetAppSettings();
	ASSERT(NULL != appSettings);

	// opacity and edge-smoothing
#if defined(WIN32)
	UICSlider* opacitySlider = getOpacitySlider();
	opacitySlider->SetMinValue(MIN_OPACITY);
	opacitySlider->SetMaxValue(MAX_OPACITY);
	opacitySlider->SetTickMarkCount(TICK_OPACITY);
	opacitySlider->SetValue(appSettings->GetMainWindowOpacity());

	UICSlider* edgeSmoothingSlider = getEdgeSmoothingSlider();
	edgeSmoothingSlider->SetMinValue(MIN_EDGE_SMOOTHING);
	edgeSmoothingSlider->SetMaxValue(MAX_EDGE_SMOOTHING);
	edgeSmoothingSlider->SetTickMarkCount(TICK_EDGE_SMOOTHING);
	edgeSmoothingSlider->SetValue(appSettings->GetMainWindowEdgeSmoothing());

	WinCoveredCalcApp* winApp = static_cast<WinCoveredCalcApp*>(app);
	const WinLayeredWindowAPI* lwApi = winApp->GetLayeredWindowAPI();
	if (NULL != lwApi && lwApi->IsSupported_UpdateLayeredWindow())
	{
		opacitySlider->Enable(true);
		edgeSmoothingSlider->Enable(true);
	}
	else
	{
		opacitySlider->Enable(false);
		edgeSmoothingSlider->Enable(false);
	}
#endif

	// language
	const LangFileInfoCollection* langFileInfos = app->GetLangFileInfos();
	langSelectHelper.Init(getLanguageListBox());
	langSelectHelper.SetToComponent(langFileInfos, app->MakeAbsoluteLangFilePath(appSettings->GetLanguageFilePath())
#if defined(ZETA)
				, CoveredCalcApp::GetInstance()->GetAppSettings()->IsLocaleKitAvailable()
#endif // defined(ZETA)
	);
	
	// key-mapping
	keymapSelectHelper.Init(getKeyMapListBox(), CID_KeyMapListBox, this);
	keymapSelectHelper.ReloadKeyMappingsInfos(appSettings->GetKeymapFilePath());
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
	SInt32 opacity = getOpacitySlider()->GetValue();
	SInt32 edgeSmoothing = getEdgeSmoothingSlider()->GetValue();
#endif // defined(WIN32)	

	// language
	Path langFilePath;
#if defined(ZETA)
	bool isLocaleKitAvailable = false;
#endif // defined(ZETA)
	LangSelectHelper::CheckResult langCheckResult = langSelectHelper.GetFromComponent(langFilePath
#if defined(ZETA)
			, isLocaleKitAvailable
#endif // defined(ZETA)
	);
	if (LangSelectHelper::Check_OK != langCheckResult)
	{
		if (LangSelectHelper::Check_GET_SELECTION == langCheckResult)
		{
			CoveredCalcApp::GetInstance()->DoMessageBox(NSID_EMSG_GET_LANGUAGE,
					MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);	
		}
		else if (LangSelectHelper::Check_INVALID_SELECTION == langCheckResult)
		{
			CoveredCalcApp::GetInstance()->DoMessageBox(NSID_EMSG_INVALID_LANGUAGE,
					MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		}
		else
		{
			ASSERT(false);
		}
		return false;		
	}
	
	// key-mapping
	const KeymapSelectHelper::KeyMappingsInfo* keyMappingsInfo;
	KeymapSelectHelper::CheckResult keymapCheckResult = keymapSelectHelper.GetCurrentKeymap(keyMappingsInfo);
	if (KeymapSelectHelper::Check_OK != keymapCheckResult)
	{
		if (KeymapSelectHelper::Check_GET_SELECTION == keymapCheckResult)
		{
			CoveredCalcApp::GetInstance()->DoMessageBox(NSID_EMSG_GET_KEYMAPPINGS,
					MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);	
		}
		else if (KeymapSelectHelper::Check_INVALID_SELECTION == keymapCheckResult)
		{
			CoveredCalcApp::GetInstance()->DoMessageBox(NSID_EMSG_INVALID_KEYMAPPINGS,
					MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		}
		else
		{
			ASSERT(false);
		}
		return false;
	}
	const Path& keyMappingFilePath = keyMappingsInfo->keyMapFilePath;
	
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

/**
 *	@brief	Handles UIComponent event.
 *	@param[in]	componentID	component ID
 *	@param[in]	eventCode	event code
 *	@param[in]	param1		parameter 1
 *	@param[in]	param2		parameter 2
 */
void PreferencesDlg::HandleUICEvent(SInt32 componentID, int eventCode, SInt32 /* param1 */, void* /* param2 */)
{
#define HANDLE_EVENT(cid, evt, handler)		\
	if (cid == componentID && evt == eventCode)	handler

	HANDLE_EVENT(CID_KeyMapListBox,			UICE_SelectionChanged,		handleKeyMappingSelectionChanged());
	HANDLE_EVENT(CID_EditKeyMapButton,		UICE_ButtonClicked,			handleEditKeyMappingButtonClicked());
	HANDLE_EVENT(CID_DuplicateKeyMapButton,	UICE_ButtonClicked,			handleDuplicateKeyMappingButtonClicked());
	HANDLE_EVENT(CID_DeleteKeyMapButton,	UICE_ButtonClicked,			handleDeleteKeyMappingButtonClicked());
	HANDLE_EVENT(CID_OKButton,				UICE_ButtonClicked,			handleOK());
	HANDLE_EVENT(CID_CancelButton,			UICE_ButtonClicked,			handleCancel());

#undef HANDLE_EVENT
}


/**
 *	@brief	Called when key-mapping selection has been changed.
 */
void PreferencesDlg::handleKeyMappingSelectionChanged()
{
	UICButton* editKeymapButton = getEditKeyMapButton();
	UICButton* duplicateKeymapButton = getDuplicateKeyMapButton();
	UICButton* deleteKeymapButton = getDeleteKeyMapButton();

	const KeymapSelectHelper::KeyMappingsInfo* currentInfo;
	KeymapSelectHelper::CheckResult keymapCheckResult = keymapSelectHelper.GetCurrentKeymap(currentInfo);
	if (KeymapSelectHelper::Check_OK != keymapCheckResult ||
		KeymapSelectHelper::KMCategory_Invalid == currentInfo->category)
	{
		editKeymapButton->Enable(false);
		duplicateKeymapButton->Enable(false);
		deleteKeymapButton->Enable(false);
		return;
	}
	
	if (KeymapSelectHelper::KMCategory_Application == currentInfo->category)
	{
		editKeymapButton->Enable(true);
		duplicateKeymapButton->Enable(true);
		deleteKeymapButton->Enable(false);
	}
	else
	{
		editKeymapButton->Enable(true);
		duplicateKeymapButton->Enable(true);
		deleteKeymapButton->Enable(true);
	}
}

/**
 *	@brief	Shows a dialog which edit the current key-mapping.
 */
void PreferencesDlg::handleEditKeyMappingButtonClicked()
{
	const KeymapSelectHelper::KeyMappingsInfo* currentInfo;
	KeymapSelectHelper::CheckResult keymapCheckResult = keymapSelectHelper.GetCurrentKeymap(currentInfo);
	if (KeymapSelectHelper::Check_OK != keymapCheckResult)
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
		
		bool isReadOnly = (currentInfo->category == KeymapSelectHelper::KMCategory_User) ? false : true;
		if (showEditKeyMapDialog(isReadOnly, keyMappings))
		{
			if (!isReadOnly)
			{
				// save to file.
				keyMappings.Save(filePath);

				// update UI
				keymapSelectHelper.ReloadKeyMappingsInfos(virtualPath);
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
void PreferencesDlg::handleDuplicateKeyMappingButtonClicked()
{
	const KeymapSelectHelper::KeyMappingsInfo* currentInfo;
	KeymapSelectHelper::CheckResult keymapCheckResult = keymapSelectHelper.GetCurrentKeymap(currentInfo);
	if (KeymapSelectHelper::Check_OK != keymapCheckResult)
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
		MBCString postStr = CoveredCalcApp::GetInstance()->LoadNativeString(NSID_KEYMAPPINGS_COPY);
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
		keymapSelectHelper.ReloadKeyMappingsInfos(virtualPath);
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
		sntprintf(buf, sizeof(buf)/sizeof(AChar) - 1, ALITERAL("%s%lx%s"), STR_USER_KEYMAP_PREFIX, num, STR_USER_KEYMAP_POSTFIX);
		buf[sizeof(buf)/sizeof(AChar) - 1] = ALITERAL('\0');
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
void PreferencesDlg::handleDeleteKeyMappingButtonClicked()
{
	const KeymapSelectHelper::KeyMappingsInfo* currentInfo;
	KeymapSelectHelper::CheckResult keymapCheckResult = keymapSelectHelper.GetCurrentKeymap(currentInfo);
	if (KeymapSelectHelper::Check_OK != keymapCheckResult)
	{
		return;
	}

	// stop if key-mapping in use is being deleted. because this dialog can be closed by "Cancel" button.
	MBCString message;
	if (0 == CoveredCalcApp::GetInstance()->GetAppSettings()->GetKeymapFilePath().Compare(currentInfo->keyMapFilePath))
	{
		MessageFormatter::Format(message, 
			CoveredCalcApp::GetInstance()->LoadNativeString(NSID_EMSG_DELETE_KEYMAPPINGS_IN_USE),
			currentInfo->title.CString());
		getMessageBoxProvider()->DoMessageBox(
			message,
			MessageBoxProvider::ButtonType_OK,
			MessageBoxProvider::AlertType_Warning);
		return;
	}

	MessageFormatter::Format(message,
		CoveredCalcApp::GetInstance()->LoadNativeString(NSID_QMSG_DELETE_KEYMAPPINGS),
		currentInfo->title.CString());
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

		keymapSelectHelper.DeleteFromList(currentInfo);
	}
}

/**
 *	@brief	Called when OK button is clicked.
 */
void PreferencesDlg::handleOK()
{
	if (saveFromDialog())
	{
		closeDialog(true);
	}
}

/**
 *	@brief	Called when cancel button is clicked.
 */
void PreferencesDlg::handleCancel()
{
	closeDialog(false);
}
