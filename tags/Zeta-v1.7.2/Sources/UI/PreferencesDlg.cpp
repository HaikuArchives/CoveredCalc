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
	@file		PreferencesDlg.cpp
	@brief		Implementation of PreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.1.17 created
*/

#include "Prefix.h"
#include "PreferencesDlg.h"
#include "CoveredCalcApp.h"
#include "AppSettings.h"

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
void PreferencesDlg::loadToDialog()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();
	ASSERT(NULL != app);
	AppSettings* appSettings = app->GetAppSettings();
	ASSERT(NULL != appSettings);

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
	
	// set values to appSettings
	appSettings->SetLanguageFilePath(app->MakeRelativeLangFilePath(langFilePath));
#if defined(ZETA)
	appSettings->SetLocaleKitAvailable(isLocaleKitAvailable);
#endif // defined(ZETA)
	appSettings->Save();
	return true;
}
