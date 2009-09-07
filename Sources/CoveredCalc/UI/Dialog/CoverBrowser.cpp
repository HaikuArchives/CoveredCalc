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
#include "UICMultiColumnList.h"
#include "UTF8Conv.h"
#include "UICEventCode.h"

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

/**
 *	@brief	Handles UIComponent event.
 *	@param[in]	componentID	component ID
 *	@param[in]	eventCode	event code
 *	@param[in]	param1		parameter 1
 *	@param[in]	param2		parameter 2
 */
void CoverBrowser::HandleUICEvent(SInt32 componentID, int eventCode, SInt32 /* param1 */, void* /* param2 */)
{
#define HANDLE_EVENT(cid, evt, handler)		\
	if (cid == componentID && evt == eventCode)	handler

	HANDLE_EVENT(CID_CoverList,				UICE_ListItemInvoked,		handleApplyButtonClicked());
	HANDLE_EVENT(CID_ReloadButton,			UICE_ButtonClicked,			handleReloadButtonClicked());
	HANDLE_EVENT(CID_ApplyButton,			UICE_ButtonClicked,			handleApplyButtonClicked());
	HANDLE_EVENT(CID_CloseButton,			UICE_ButtonClicked,			handleCloseButtonClicked());

#undef HANDLE_EVENT
}

/**
 *	@brief	called before showing dialog.
 */
void CoverBrowser::readyToShow()
{
	doUpdateList();
}

// ---------------------------------------------------------------------
//! Updates cover list
// ---------------------------------------------------------------------
void CoverBrowser::doUpdateList()
{
	WaitingUIKeeper	waitingUIKeeper(CoveredCalcApp::GetInstance());
	waitingUIKeeper.Start();

	UICMultiColumnList* coverList = getCoverList();
	coverList->RemoveAllItem();

	listManager.MakeList(coversFolderPath);

	MBCString title;
	MBCString description;
	const CoverListVector* items = listManager.GetItems();
	CoverListVector::const_iterator iterator;
	for (iterator=items->begin(); iterator!=items->end(); iterator++)
	{
		UTF8Conv::ToMultiByte(title, (*iterator)->GetTitle());
		UTF8Conv::ToMultiByte(description, (*iterator)->GetDescription());
		ConstAStr texts[2] = { title.CString(), description.CString() };
		coverList->AddItem(texts, *iterator);
	}
}

/**
 *	@brief	Called when reload button is clicked.
 */
void CoverBrowser::handleReloadButtonClicked()
{
	doUpdateList();
}

/**
 *	@brief	Called when apply button is clicked.
 */
void CoverBrowser::handleApplyButtonClicked()
{
	UICMultiColumnList* coverList = getCoverList();
	SInt32 selectedIndex = coverList->GetSelectedItem();
	if (selectedIndex < 0)
	{
		return;
	}
	const CoverListItem* item = static_cast<CoverListItem*>(coverList->GetItemData(selectedIndex));
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

/**
 *	@brief	Called when close button is clicked.
 */
void CoverBrowser::handleCloseButtonClicked()
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
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(app, ex, NSID_EMSG_SAVE_SETTING);
		ex->Delete();		
	}	
}
