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
	@file		CoverBrowser.cpp
	@brief		Implementation of WinCoverBrowser class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.11 created
*/

#include "Prefix.h"
#include "CoveredCalcApp.h"
#include "AppSettings.h"
#include "CoverListItem.h"
#include "CoverManager.h"
#include "WaitingUIKeeper.h"
#include "ExceptionMessageUtils.h"
#include "CoverBrowser.h"
#include "Exception.h"
#include "StringID.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverBrowser::CoverBrowser()
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverBrowser::~CoverBrowser()
{
}

// ---------------------------------------------------------------------
//! Returns initial location of this window.
// ---------------------------------------------------------------------
Point32 CoverBrowser::getInitialLocation()
{
	AppSettings* settings = CoveredCalcApp::GetInstance()->GetAppSettings();
	return settings->GetLastCoverBrowserPos();
}

// ---------------------------------------------------------------------
//! Makes cover list
// ---------------------------------------------------------------------
void CoverBrowser::makeList()
{
	clearListUI();
	listManager.MakeList(coversFolderPath);
}

// ---------------------------------------------------------------------
//! Updates cover list
// ---------------------------------------------------------------------
void CoverBrowser::doUpdateList()
{
	WaitingUIKeeper	waitingUIKeeper(CoveredCalcApp::GetInstance());
	waitingUIKeeper.Start();

	makeList();
	setDataToListUI();
}

// ---------------------------------------------------------------------
//! Applies the selected cover to main window.
// ---------------------------------------------------------------------
void CoverBrowser::doApplySelectedCover()
{
	const CoverListItem* item = getSelectedItem();
	if (NULL == item)
	{
		return;
	}

	WaitingUIKeeper	waitingUIKeeper(CoveredCalcApp::GetInstance());
	waitingUIKeeper.Start();
	
	const Path& coverPath = item->GetPath();
	CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
	ASSERT(manager);
	if (NULL != manager)
	{
		manager->Load(coverPath, 0);
	}
}

// ---------------------------------------------------------------------
//! Hides cover browser.
// ---------------------------------------------------------------------
void CoverBrowser::doClose()
{
	CoveredCalcApp::GetInstance()->ShowCoverBrowser(false);
}

// ---------------------------------------------------------------------
//! Called when cover browser is destorying.
// ---------------------------------------------------------------------
void CoverBrowser::onDestroy()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();

	// save current location
	Rect32 uiRect;
	GetUIRect(uiRect);
	Point32 windowPos;
	windowPos.x = uiRect.left;
	windowPos.y = uiRect.top;
	AppSettings* appSettings = app->GetAppSettings();
	appSettings->SetLastCoverBrowserPos(windowPos);
	try
	{
		appSettings->Save();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(app, ex, IDS_EMSG_SAVE_SETTING);
		ex->Delete();		
	}	
}
