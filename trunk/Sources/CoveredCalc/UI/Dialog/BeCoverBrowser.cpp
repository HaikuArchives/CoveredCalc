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
	@file		BeCoverBrowser.cpp
	@brief		Implementation of BeCoverBrowser class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.09.12 created
*/

#include "Prefix.h"
#include <AppKit.h>
#include <InterfaceKit.h>
#include <Autolock.h>
#include "BeCCColumnListView.h"
#include "ColumnTypes.h"
#include "DialogID.h"
#include "StringID.h"
#include "CommandID.h"
#include "GenericException.h"
#include "MBCString.h"
#include "BeCoveredCalcApp.h"
#include "CoverListItem.h"
#include "UTF8Conv.h"
#include "ExceptionMessageUtils.h"
#include "BeCoverBrowser.h"
#include "BeDialogControlHelper.h"
#include "UICEventCode.h"

////////////////////////////////////////
#define baseWindow	BeDialog
////////////////////////////////////////

static const char COVER_BROWSER_VIEW_BASE_VIEW[]		= "BaseView";
static const char COVER_BROWSER_VIEW_CLOSE_BUTTON[]		= "Close";
static const char COVER_BROWSER_VIEW_RELOAD_BUTTON[]	= "Reload";
static const char COVER_BROWSER_VIEW_APPLY_BUTTON[]		= "Apply";
static const char COVER_BROWSER_VIEW_COVER_LIST[]		= "CoverList";
static const char COVER_BROWSER_VIEW_COVER_LIST_LABEL[]	= "CoverListLabel";

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeCoverBrowser::BeCoverBrowser()
	: BeDialog(IDD_COVER_BROWSER,
				B_TITLED_WINDOW,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	coverList = NULL;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeCoverBrowser::~BeCoverBrowser()
{
}

/**
 *	@brief	Creates child views.
 */
void BeCoverBrowser::createViews()
{
	BeDialogControlHelper dch(getDialogLayout());
	NativeStringLoader* nsl = CoveredCalcApp::GetInstance();

	rgb_color viewColor = { 216, 216, 216, 255 };
	
	// dialog title
	SetTitle(nsl->LoadNativeString(NSID_COVER_BROWSER_TITLE).CString());

	// BaseView
	BView* baseView = new BView(Bounds(), COVER_BROWSER_VIEW_BASE_VIEW,
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(baseView);

	baseView->SetViewColor(viewColor);
	
	// CoverListLabel
	BStringView* coverListLabelView = new BStringView(dch.GetItemRect(ALITERAL("IDC_SELECT_COVER"), ITEMNAME_WINDOW),
									COVER_BROWSER_VIEW_COVER_LIST_LABEL, nsl->LoadNativeString(NSID_COVER_BROWSER_SELECT_COVER));
	baseView->AddChild(coverListLabelView);
	
	// CoverList
	coverList = new BeCCColumnListView(dch.GetItemRect(ALITERAL("IDC_COVER_LIST"), ITEMNAME_WINDOW),
										COVER_BROWSER_VIEW_COVER_LIST,
										B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	baseView->AddChild(coverList);
	
	// initialize the cover list.
	coverList->AddColumn(new BStringColumn(nsl->LoadNativeString(NSID_COVER_BROWSER_COLUMN_NAME), 150, 50, INT_MAX, B_TRUNCATE_END, B_ALIGN_LEFT), 0);
	coverList->AddColumn(new BStringColumn(nsl->LoadNativeString(NSID_COVER_BROWSER_COLUMN_DESCRIPTION), 200, 50, INT_MAX, B_TRUNCATE_END, B_ALIGN_LEFT), 1);	
	coverList->SetColumnFlags(B_ALLOW_COLUMN_RESIZE);
	coverList->SetSelectionMode(B_SINGLE_SELECTION_LIST);
	coverList->SetInvocationMessage(new BMessage(ID_COVERBROWSER_INVOKE_ITEM));
	uicCoverList.Init(coverList);
	
	// Reload
	BButton* reloadButton = new BButton(dch.GetItemRect(ALITERAL("IDC_RELOAD"), ITEMNAME_WINDOW),
									COVER_BROWSER_VIEW_RELOAD_BUTTON,
									nsl->LoadNativeString(NSID_COVER_BROWSER_RELOAD).CString(), new BMessage(ID_COVERBROWSER_RELOAD));
	baseView->AddChild(reloadButton);
	
	// Apply
	BButton* applyButton = new BButton(dch.GetItemRect(ALITERAL("IDC_APPLY"), ITEMNAME_WINDOW),
									COVER_BROWSER_VIEW_APPLY_BUTTON,
									nsl->LoadNativeString(NSID_COVER_BROWSER_APPLY).CString(), new BMessage(ID_COVERBROWSER_APPLY));
	baseView->AddChild(applyButton);
	
	// Close
	BButton* closeButton = new BButton(dch.GetItemRect(ALITERAL("IDCLOSE"), ITEMNAME_WINDOW),
									COVER_BROWSER_VIEW_CLOSE_BUTTON,
									nsl->LoadNativeString(NSID_COVER_BROWSER_CLOSE).CString(), new BMessage(ID_COVERBROWSER_CLOSE));
	baseView->AddChild(closeButton);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed by LocaleKit.
 */
void BeCoverBrowser::languageChanged()
{
	NativeStringLoader* nsl = CoveredCalcApp::GetInstance();

	// dialog title
	SetTitle(nsl->LoadNativeString(NSID_COVER_BROWSER_TITLE).CString());
	
	// CoverListLabel
	BStringView* coverListLabelView = dynamic_cast<BStringView*>(FindView(COVER_BROWSER_VIEW_COVER_LIST_LABEL));
	if (NULL != coverListLabelView)
	{
		coverListLabelView->SetText(nsl->LoadNativeString(NSID_COVER_BROWSER_SELECT_COVER));
	}
	
	// CoverList
	BStringColumn* column;
	column = dynamic_cast<BStringColumn*>(coverList->ColumnAt(0));
	if (NULL != column)
	{
		column->SetTitle(nsl->LoadNativeString(NSID_COVER_BROWSER_COLUMN_NAME).CString());
	}
	column = dynamic_cast<BStringColumn*>(coverList->ColumnAt(1));
	if (NULL != column)
	{
		column->SetTitle(nsl->LoadNativeString(NSID_COVER_BROWSER_COLUMN_DESCRIPTION).CString());
	}
	coverList->Hide();	// reflesh coverList's title
	coverList->Show();
	
	// Reload
	BButton* reloadButton = dynamic_cast<BButton*>(FindView(COVER_BROWSER_VIEW_RELOAD_BUTTON));
	if (NULL != reloadButton)
	{
		reloadButton->SetLabel(nsl->LoadNativeString(NSID_COVER_BROWSER_RELOAD).CString());
	}

	// Apply
	BButton* applyButton = dynamic_cast<BButton*>(FindView(COVER_BROWSER_VIEW_APPLY_BUTTON));
	if (NULL != applyButton)
	{
		applyButton->SetLabel(nsl->LoadNativeString(NSID_COVER_BROWSER_APPLY).CString());
	}

	// Close
	BButton* closeButton = dynamic_cast<BButton*>(FindView(COVER_BROWSER_VIEW_CLOSE_BUTTON));
	if (NULL != closeButton)
	{
		closeButton->SetLabel(nsl->LoadNativeString(NSID_COVER_BROWSER_CLOSE).CString());
	}	
}
#endif

// ---------------------------------------------------------------------
//! Initialize.
// ---------------------------------------------------------------------
void BeCoverBrowser::initDialog()
{
	createViews();
	moveToInitialLocation();
	
	// make the list and show it.
	try
	{
		readyToShow();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
		ex->Delete();
	}
}

/**
 *	@brief	Moves window to initial location
 */
void BeCoverBrowser::moveToInitialLocation()
{
	BAutolock locker(this);
	if (locker.IsLocked())
	{
		Point32 topLeft = getInitialLocation();
		MoveTo(topLeft.x, topLeft.y);
	}
}

// ---------------------------------------------------------------------
//! Retrives rectangele of UI in screen coordinates.
// ---------------------------------------------------------------------
void BeCoverBrowser::GetUIRect(
	Rect32& rect				//!< OUTPUT. UI rectangle is returned.
) const
{
	BRect frame = Frame();
	rect.left = static_cast<SInt32>(frame.left);
	rect.top = static_cast<SInt32>(frame.top);
	rect.right = static_cast<SInt32>(frame.right + 1);
	rect.bottom = static_cast<SInt32>(frame.bottom + 1);
}

/**
 *	@brief	Really destroys this cover browser window.
 */
void BeCoverBrowser::Quit()
{
	onDestroy();
	baseWindow::Quit();
}

// ---------------------------------------------------------------------
//! Message handler.
// ---------------------------------------------------------------------
void BeCoverBrowser::MessageReceived(
	BMessage *message		//!< received message.
)
{
	try
	{
		switch (message->what)
		{
		case ID_COVERBROWSER_INVOKE_ITEM:
			HandleUICEvent(CID_CoverList, UICE_ListItemInvoked, 0, NULL);
			break;
		case ID_COVERBROWSER_RELOAD:
			HandleUICEvent(CID_ReloadButton, UICE_ButtonClicked, 0, NULL);
			break;
		case ID_COVERBROWSER_APPLY:
			HandleUICEvent(CID_ApplyButton, UICE_ButtonClicked, 0, NULL);
			break;
		case ID_COVERBROWSER_CLOSE:
			HandleUICEvent(CID_CloseButton, UICE_ButtonClicked, 0, NULL);
			break;
		default:
			baseWindow::MessageReceived(message);
			break;
		}
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
		ex->Delete();		
	}	
}

// ---------------------------------------------------------------------
//! The window is requested to quit.
/*!
	@retval	true	quit this window.
	@retval	false	don't quit this window.
*/
// ---------------------------------------------------------------------
bool BeCoverBrowser::QuitRequested()
{
	// hide it and don't quit.
	if (!IsHidden())
	{
		try
		{
			HandleUICEvent(CID_CloseButton, UICE_ButtonClicked, 0, NULL);
		}
		catch (Exception* ex)
		{
			ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
			ex->Delete();		
		}	
	}
	return false;
}
