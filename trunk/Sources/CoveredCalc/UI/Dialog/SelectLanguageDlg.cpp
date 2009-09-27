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
	@file		SelectLanguageDlg.cpp
	@brief		Implementation of SelectLanguageDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.9.13 created
*/

#include "Prefix.h"
#include "SelectLanguageDlg.h"
#include "LangFileInfo.h"
#include "CoveredCalcApp.h"
#include "UICEventCode.h"
#include "StringID.h"

/**
 *	@brief	Constructor
 */
SelectLanguageDlg::SelectLanguageDlg()
{
}

/**
 *	@brief	Destructor
 */
SelectLanguageDlg::~SelectLanguageDlg()
{
}

/**
 *	@brief	Loads preferences data to this dialog.
 */
void SelectLanguageDlg::readyToShow()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();

	// language
	const LangFileInfoCollection* langFileInfos = app->GetLangFileInfos();
	langSelectHelper.Init(getLanguageListBox());
	langSelectHelper.SetToComponent(langFileInfos, 
		app->MakeAbsoluteLangFilePath(langFilePath)
#if defined(ZETA)
		, isLocaleKitAvailable
#endif // defined(ZETA)
	);
}

/**
 *	@brief	Handles UIComponent event.
 *	@param[in]	componentID	component ID
 *	@param[in]	eventCode	event code
 *	@param[in]	param1		parameter 1
 *	@param[in]	param2		parameter 2
 */
void SelectLanguageDlg::HandleUICEvent(SInt32 componentID, int eventCode, SInt32 /* param1 */, void* /* param2 */)
{
#define HANDLE_EVENT(cid, evt, handler)		\
	if (cid == componentID && evt == eventCode)	handler

	HANDLE_EVENT(CID_OKButton,				UICE_ButtonClicked,			handleOK());
	HANDLE_EVENT(CID_CancelButton,			UICE_ButtonClicked,			handleCancel());

#undef HANDLE_EVENT
}

/**
 *	@brief	Called when OK button is clicked.
 */
void SelectLanguageDlg::handleOK()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();

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
		return;		
	}

	this->langFilePath = app->MakeRelativeLangFilePath(langFilePath);
#if defined(ZETA)
	this->isLocaleKitAvailable = isLocaleKitAvailable;
#endif
	closeDialog(true);
}

/**
 *	@brief	Called when cancel button is clicked.
 */
void SelectLanguageDlg::handleCancel()
{
	closeDialog(false);
}
