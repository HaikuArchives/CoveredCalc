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
#include "DialogDef.h"
#include "GenericException.h"
#include "MBCString.h"
#include "UIMessageProvider.h"
#include "BeCoveredCalcApp.h"
#include "CoverListItem.h"
#include "UTF8Conv.h"
#include "ExceptionMessageUtils.h"
#include "BeCoverBrowser.h"
#include "BeDialogDesign.h"

////////////////////////////////////////
#define baseWindow	BeDialog
////////////////////////////////////////

static const char COVER_BROWSER_VIEW_BASE_VIEW[]		= "BaseView";
static const char COVER_BROWSER_VIEW_CLOSE_BUTTON[]		= "Close";
static const char COVER_BROWSER_VIEW_RELOAD_BUTTON[]	= "Reload";
static const char COVER_BROWSER_VIEW_APPLY_BUTTON[]		= "Apply";
static const char COVER_BROWSER_VIEW_COVER_LIST[]		= "CoverList";
static const char COVER_BROWSER_VIEW_COVER_LIST_LABEL[]	= "CoverListLabel";

static const char STR_COVER_BROWSER[]	= "Cover Browser - CoveredCalc";
static const char STR_SELECT_COVER[]	= "Select Cover:";
static const char STR_REFRESH[]			= "Refresh";
static const char STR_APPLY[]			= "Apply";
static const char STR_CLOSE[]			= "Close";

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeCoverBrowser::BeCoverBrowser(
	BeDialogDesign* dialogDesign	//!< dialog design information
)
	: BeDialog(dialogDesign->GetFrame(BRect(100.0, 100.0, 486.0, 384.0)),
				LT(dialogDesign->GetTitle(STR_COVER_BROWSER)),
				B_TITLED_WINDOW,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	this->dialogDesign = dialogDesign;
	coverList = NULL;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeCoverBrowser::~BeCoverBrowser()
{
	if (NULL != dialogDesign)
		delete dialogDesign;
}

/**
 *	@brief	Creates child views.
 */
void BeCoverBrowser::createViews()
{
	const BeDialogDesignItem* item;

	rgb_color viewColor = { 216, 216, 216, 255 };
	
	// BaseView
	BView* baseView = new BView(Bounds(), COVER_BROWSER_VIEW_BASE_VIEW,
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(baseView);

	baseView->SetViewColor(viewColor);
	
	// CoverListLabel
	item = dialogDesign->FindItem(COVER_BROWSER_VIEW_COVER_LIST_LABEL);
	BStringView* coverListLabelView = new BStringView(item->GetFrame(BRect(12.0, 12.0, 374.0, 24.0)),
									COVER_BROWSER_VIEW_COVER_LIST_LABEL, LT(item->GetLabel(STR_SELECT_COVER)));
	baseView->AddChild(coverListLabelView);
	
	// CoverList
	item = dialogDesign->FindItem(COVER_BROWSER_VIEW_COVER_LIST);
	coverList = new BeCCColumnListView(item->GetFrame(BRect(12.0, 27.0, 374.0, 236.0)),
										COVER_BROWSER_VIEW_COVER_LIST,
										B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	baseView->AddChild(coverList);
	
	// initialize the cover list.
	MBCString columnName;
	UIMessageProvider* messageProvider = CoveredCalcApp::GetInstance()->GetMessageProvider();
	messageProvider->GetMessage(IDS_COVER_BROWSER_COLUMN_NAME, columnName);
	coverList->AddColumn(new BStringColumn(columnName, 150, 50, INT_MAX, B_TRUNCATE_END, B_ALIGN_LEFT), 0);
	messageProvider->GetMessage(IDS_COVER_BROWSER_COLUMN_DESCRIPTION, columnName);
	coverList->AddColumn(new BStringColumn(columnName, 200, 50, INT_MAX, B_TRUNCATE_END, B_ALIGN_LEFT), 1);
	
	coverList->SetColumnFlags(B_ALLOW_COLUMN_RESIZE);
	coverList->SetSelectionMode(B_SINGLE_SELECTION_LIST);
	coverList->SetInvocationMessage(new BMessage(ID_COVERBROWSER_APPLY));
	
	// Reload
	item = dialogDesign->FindItem(COVER_BROWSER_VIEW_RELOAD_BUTTON);
	BButton* reloadButton = new BButton(item->GetFrame(BRect(12.0, 248.0, 92.0, 272.0)),
									COVER_BROWSER_VIEW_RELOAD_BUTTON,
									LT(item->GetLabel(STR_REFRESH)), new BMessage(ID_COVERBROWSER_RELOAD));
	baseView->AddChild(reloadButton);
	
	// Apply
	item = dialogDesign->FindItem(COVER_BROWSER_VIEW_APPLY_BUTTON);
	BButton* applyButton = new BButton(item->GetFrame(BRect(208.0, 248.0, 288.0, 272.0)),
									COVER_BROWSER_VIEW_APPLY_BUTTON,
									LT(item->GetLabel(STR_APPLY)), new BMessage(ID_COVERBROWSER_APPLY));
	baseView->AddChild(applyButton);
	
	// Close
	item = dialogDesign->FindItem(COVER_BROWSER_VIEW_CLOSE_BUTTON);
	BButton* closeButton = new BButton(item->GetFrame(BRect(294.0, 248.0, 374.0, 272.0)),
									COVER_BROWSER_VIEW_CLOSE_BUTTON,
									LT(item->GetLabel(STR_CLOSE)), new BMessage(ID_COVERBROWSER_CLOSE));
	baseView->AddChild(closeButton);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed by LocaleKit.
 */
void BeCoverBrowser::languageChanged()
{
	const BeDialogDesignItem* item;

	// dialog title
	SetTitle(LT(dialogDesign->GetTitle(STR_COVER_BROWSER)));
	
	// CoverListLabel
	item = dialogDesign->FindItem(COVER_BROWSER_VIEW_COVER_LIST_LABEL);
	BStringView* coverListLabelView = dynamic_cast<BStringView*>(FindView(COVER_BROWSER_VIEW_COVER_LIST_LABEL));
	if (NULL != coverListLabelView)
	{
		coverListLabelView->SetText(LT(item->GetLabel(STR_SELECT_COVER)));
	}
	
	// CoverList
	MBCString columnName;
	UIMessageProvider* messageProvider = CoveredCalcApp::GetInstance()->GetMessageProvider();
	BStringColumn* column;
	column = dynamic_cast<BStringColumn*>(coverList->ColumnAt(0));
	if (NULL != column)
	{
		messageProvider->GetMessage(IDS_COVER_BROWSER_COLUMN_NAME, columnName);
		column->SetTitle(columnName);
	}
	column = dynamic_cast<BStringColumn*>(coverList->ColumnAt(1));
	if (NULL != column)
	{
		messageProvider->GetMessage(IDS_COVER_BROWSER_COLUMN_DESCRIPTION, columnName);
		column->SetTitle(columnName);
	}
	coverList->Hide();	// reflesh coverList's title
	coverList->Show();
	
	// Reload
	item = dialogDesign->FindItem(COVER_BROWSER_VIEW_RELOAD_BUTTON);
	BButton* reloadButton = dynamic_cast<BButton*>(FindView(COVER_BROWSER_VIEW_RELOAD_BUTTON));
	if (NULL != reloadButton)
	{
		reloadButton->SetLabel(LT(item->GetLabel(STR_REFRESH)));
	}

	// Apply
	item = dialogDesign->FindItem(COVER_BROWSER_VIEW_APPLY_BUTTON);
	BButton* applyButton = dynamic_cast<BButton*>(FindView(COVER_BROWSER_VIEW_APPLY_BUTTON));
	if (NULL != applyButton)
	{
		applyButton->SetLabel(LT(item->GetLabel(STR_APPLY)));
	}

	// Close
	item = dialogDesign->FindItem(COVER_BROWSER_VIEW_CLOSE_BUTTON);
	BButton* closeButton = dynamic_cast<BButton*>(FindView(COVER_BROWSER_VIEW_CLOSE_BUTTON));
	if (NULL != closeButton)
	{
		closeButton->SetLabel(LT(item->GetLabel(STR_CLOSE)));
	}	
}
#endif

// ---------------------------------------------------------------------
//! Initialize.
// ---------------------------------------------------------------------
void BeCoverBrowser::Init()
{
	createViews();
	baseWindow::Init();
	moveToInitialLocation();
	
	// make the list and show it.
	try
	{
		doUpdateList();
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

// ---------------------------------------------------------------------
//! Clears the cover list.
// ---------------------------------------------------------------------
void BeCoverBrowser::clearListUI()
{
	coverList->Clear();
}

// ---------------------------------------------------------------------
//! Each row in cover list.
// ---------------------------------------------------------------------
class BeCoverBrowserRow : public BRow
{
public:
						BeCoverBrowserRow(float height = 16.0) : BRow(height), listItem(NULL) { }

	void				SetData(CoverListItem* listItem)
							{ this->listItem = listItem; }
	CoverListItem*		GetData() const
							{ return listItem; }

private:
	CoverListItem*		listItem;			///< list item attached with this row.
};

// ---------------------------------------------------------------------
//! Set cover data to the cover list.
// ---------------------------------------------------------------------
void BeCoverBrowser::setDataToListUI()
{
	clearListUI();

	MBCString text;
	const CoverListVector* items = getListItems();
	CoverListVector::const_iterator iterator;
	for (iterator=items->begin(); iterator!=items->end(); iterator++)
	{
		CoverListItem* item = *iterator;
		BeCoverBrowserRow* row = new BeCoverBrowserRow();
		row->SetData(item);
		UTF8Conv::ToMultiByte(text, item->GetTitle());
		row->SetField(new BStringField(text), 0);
		UTF8Conv::ToMultiByte(text, item->GetDescription());
		row->SetField(new BStringField(text), 1);
		coverList->AddRow(row);
	}
}

// ---------------------------------------------------------------------
//! Retrives selected item.
/*!
	@return selected item. (NULL when no item is selected)
*/
// ---------------------------------------------------------------------
const CoverListItem* BeCoverBrowser::getSelectedItem()
{
	BRow* selectedRow = coverList->CurrentSelection();
	BeCoverBrowserRow* cbRow = dynamic_cast<BeCoverBrowserRow*>(selectedRow);
	if (NULL != cbRow)
	{
		return cbRow->GetData();
	}
	else
	{
		return NULL;
	}
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
		case ID_COVERBROWSER_RELOAD:
			onReload();
			break;
		case ID_COVERBROWSER_APPLY:
			onApply();
			break;
		case ID_COVERBROWSER_CLOSE:
			onClose();
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
//! Called when 'reload' button is pressed.
// ---------------------------------------------------------------------
void BeCoverBrowser::onReload()
{
	try
	{
		doUpdateList();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
		ex->Delete();		
	}
}

// ---------------------------------------------------------------------
//! Called when 'apply' button is pressed.
// ---------------------------------------------------------------------
void BeCoverBrowser::onApply()
{
	try
	{
		doApplySelectedCover();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
		ex->Delete();		
	}	
}

// ---------------------------------------------------------------------
//! Called when 'close' button is pressed.
// ---------------------------------------------------------------------
void BeCoverBrowser::onClose()
{
	try
	{
		doClose();
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
			doClose();
		}
		catch (Exception* ex)
		{
			ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
			ex->Delete();		
		}	
	}
	return false;
}
