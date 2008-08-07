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
	@file		BeAboutCurrentCoverDialog.cpp
	@brief		Implementation of BeAboutCurrentCoverDialog class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.24 created
*/

#include "Prefix.h"
#include "BeAboutCurrentCoverDialog.h"
#include <InterfaceKit.h>
#include <support/Autolock.h>
#include "DialogDef.h"
#include "MBCString.h"
#include "CoverDef.h"
#include "UTF8Conv.h"
#include "BeDialogDesign.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcApp.h"

////////////////////////////////////////
#define baseWindow	BeDialog
#define base		AboutCurrentCoverDlg
////////////////////////////////////////

static const char ABOUT_CURCOV_DIALOG_VIEW_BASE_VIEW[]			= "BaseView";
static const char ABOUT_CURCOV_DIALOG_VIEW_DESC_LABEL[]			= "DescLabel";
static const char ABOUT_CURCOV_DIALOG_VIEW_DESC_TEXT[]			= "DescText";
static const char ABOUT_CURCOV_DIALOG_VIEW_NAME_LABEL[]			= "NameLabel";
static const char ABOUT_CURCOV_DIALOG_VIEW_NAME_TEXT[]			= "NameText";
static const char ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_LABEL[]	= "AboutCoverLabel";
static const char ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_TEXT[]	= "AboutCoverText";
static const char ABOUT_CURCOV_DIALOG_VIEW_OK[]					= "OKButton";

static const char ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_SCROLLER[]	= "AboutCoverTextScroller";

static const char ABOUT_CURCOV_SETDATA_PARAM_COVERDEF[]			= "CoverDef";

static const char STR_COVER_INFO[]			= "Cover Info";
static const char STR_NAME[]				= "Name:";
static const char STR_DESCRIPTION[]			= "Description:";
static const char STR_COVER_AUTHOR_INFO[]	= "Cover Author Info:";
static const char STR_OK[]					= "OK";

/**
 *	@brief	Constructor
 */
BeAboutCurrentCoverDialog::BeAboutCurrentCoverDialog(
	BeDialogDesign* dialogDesign	///< dialog design information
)
	: BeDialog(dialogDesign->GetFrame(BRect(0.0, 0.0, 312.0, 230.0)),
				LT(dialogDesign->GetTitle(STR_COVER_INFO)),
				B_TITLED_WINDOW,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_MINIMIZABLE)
{
	this->dialogDesign = dialogDesign;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeAboutCurrentCoverDialog::~BeAboutCurrentCoverDialog()
{
	if (NULL != dialogDesign)
		delete dialogDesign;
}

/**
 *	@brief	Creates child views.
 */
void BeAboutCurrentCoverDialog::createViews()
{
	const BeDialogDesignItem* item;

	rgb_color viewColor = { 216, 216, 216, 255 };
	rgb_color highColor = { 0, 0, 0, 255 };

	// BaseView
	BView* baseView = new BView(Bounds(), ABOUT_CURCOV_DIALOG_VIEW_BASE_VIEW,
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(baseView);
	
	baseView->SetViewColor(viewColor);
	
	// NameLabel
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_NAME_LABEL);
	BStringView* nameLabelView = new BStringView(item->GetFrame(BRect(12.0, 12.0, 74.0, 24.0)), ABOUT_CURCOV_DIALOG_VIEW_NAME_LABEL,
									LT(item->GetLabel(STR_NAME)));
	baseView->AddChild(nameLabelView);
	
	// NameText
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_NAME_TEXT);
	BTextView* nameTextView = new BTextView(item->GetFrame(BRect(77.0, 11.0, 300.0, 24.0)), ABOUT_CURCOV_DIALOG_VIEW_NAME_TEXT, BRect(0, 0, 0, 0),
								B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	baseView->AddChild(nameTextView);
	
	nameTextView->SetTextRect(BRect(0, 0, 1024, nameTextView->Bounds().Height()));
	nameTextView->SetFontAndColor(be_plain_font, B_FONT_ALL, &highColor);
	nameTextView->SetViewColor(viewColor);
	nameTextView->SetWordWrap(false);
	nameTextView->MakeEditable(false);
	
	// DescLabel
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_DESC_LABEL);
	BStringView* descLabelView = new BStringView(item->GetFrame(BRect(12.0, 30.0, 74.0, 42.0)), ABOUT_CURCOV_DIALOG_VIEW_DESC_LABEL,
									LT(item->GetLabel(STR_DESCRIPTION)));
	baseView->AddChild(descLabelView);
	
	// DescText
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_DESC_TEXT);
	BTextView* descTextView = new BTextView(item->GetFrame(BRect(77.0, 29.0, 300.0, 42.0)), ABOUT_CURCOV_DIALOG_VIEW_DESC_TEXT, BRect(0, 0, 0, 0),
								B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	baseView->AddChild(descTextView);
	
	descTextView->SetTextRect(BRect(0, 0, 1024, descTextView->Bounds().Height()));
	descTextView->SetFontAndColor(be_plain_font, B_FONT_ALL, &highColor);
	descTextView->SetViewColor(viewColor);
	descTextView->SetWordWrap(false);
	descTextView->MakeEditable(false);
	
	// AboutCoverLabel
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_LABEL);
	BStringView* aboutCoverLabelView = new BStringView(item->GetFrame(BRect(12.0, 54.0, 100.0, 66.0)), ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_LABEL,
										LT(item->GetLabel(STR_COVER_AUTHOR_INFO)));
	baseView->AddChild(aboutCoverLabelView);
	
	// AboutCoverText
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_TEXT);
	BRect frameRect = item->GetFrame(BRect(12.0, 69.0, 300.0, 182.0));
	int32 borderSize = 2;
	frameRect.OffsetBy(borderSize, borderSize);
	frameRect.right = frameRect.right - (B_V_SCROLL_BAR_WIDTH + borderSize * 2);
	frameRect.bottom = frameRect.bottom - (B_H_SCROLL_BAR_HEIGHT + borderSize * 2);
	BTextView* aboutCoverTextView = new BTextView(frameRect, ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_TEXT, BRect(0, 0, 0, 0),
									B_FOLLOW_ALL_SIDES);
	BScrollView* scrollView = new BScrollView(ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_SCROLLER, aboutCoverTextView,
									B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, true, true, B_FANCY_BORDER);
	scrollView->SetViewColor(viewColor);
	baseView->AddChild(scrollView);
	
	aboutCoverTextView->SetTextRect(BRect(0, 0, 1024, aboutCoverTextView->Bounds().Height()));
	aboutCoverTextView->SetFontAndColor(be_plain_font, B_FONT_ALL, &highColor);
	aboutCoverTextView->SetViewColor(viewColor);
	aboutCoverTextView->SetWordWrap(false);
	aboutCoverTextView->MakeEditable(false);
	
	// OKButton
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_OK);
	BButton* okButton = new BButton(item->GetFrame(BRect(220.0, 194.0, 300.0, 218.0)), ABOUT_CURCOV_DIALOG_VIEW_OK,
								LT(item->GetLabel(STR_OK)), new BMessage(ID_DIALOG_OK));
	baseView->AddChild(okButton);
	
	SetDefaultButton(okButton);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed by LocaleKit.
 */
void BeAboutCurrentCoverDialog::languageChanged()
{
	const BeDialogDesignItem* item;

	// dialog title
	SetTitle(LT(dialogDesign->GetTitle(STR_COVER_INFO)));

	// NameLabel
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_NAME_LABEL);
	BStringView* nameLabelView = dynamic_cast<BStringView*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_NAME_LABEL));
	if (NULL != nameLabelView)
	{
		nameLabelView->SetText(LT(item->GetLabel(STR_NAME)));
	}		

	// DescLabel
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_DESC_LABEL);
	BStringView* descLabelView = dynamic_cast<BStringView*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_DESC_LABEL));
	if (NULL != descLabelView)
	{
		descLabelView->SetText(LT(item->GetLabel(STR_DESCRIPTION)));
	}

	// AboutCoverLabel
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_LABEL);
	BStringView* aboutCoverLabelView = dynamic_cast<BStringView*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_LABEL));
	if (NULL != aboutCoverLabelView)
	{
		aboutCoverLabelView->SetText(LT(item->GetLabel(STR_COVER_AUTHOR_INFO)));
	}

	// OKButton
	item = dialogDesign->FindItem(ABOUT_CURCOV_DIALOG_VIEW_OK);
	BButton* okButton = dynamic_cast<BButton*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_OK));
	if (NULL != okButton)
	{
		okButton->SetLabel(LT(item->GetLabel(STR_OK)));
	}
}
#endif

// ---------------------------------------------------------------------
//! Initialize.
// ---------------------------------------------------------------------
void BeAboutCurrentCoverDialog::Init()
{
	createViews();
	baseWindow::Init();	
	moveToCenterOfScreen();

	// invoke thread in invisible.
	Hide();
	Show();

	initialize();
}

// ---------------------------------------------------------------------
//! Sets cover data to dialog.
// ---------------------------------------------------------------------
void BeAboutCurrentCoverDialog::setDataToDialog(
	const CoverDef* coverDef	//!< data which is set to dialog.
)
{
	if (Thread() == find_thread(NULL))
	{
		setDataToDialogInThisThread(coverDef);
	}
	else
	{
		BMessage message(ID_ABOUTCURCOV_SETDATA);
		message.AddPointer(ABOUT_CURCOV_SETDATA_PARAM_COVERDEF, coverDef);
		
		BMessage reply;
		BMessenger(this).SendMessage(&message, &reply);		// wait until processed.
	}
}

// ---------------------------------------------------------------------
//! Sets cover data to dialog in this window's thread.
// ---------------------------------------------------------------------
void BeAboutCurrentCoverDialog::setDataToDialogInThisThread(
	const CoverDef* coverDef	//!< data which is set to dialog.
)
{
	MBCString mbcString;
	BTextView* nameView = dynamic_cast<BTextView*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_NAME_TEXT));
	if (NULL != nameView)
	{
		mbcString.Empty();
		if (NULL != coverDef)
		{
			UTF8Conv::ToMultiByte(mbcString, coverDef->GetTitle());
		}
		nameView->SetText(mbcString);
	}
	
	BTextView* descView = dynamic_cast<BTextView*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_DESC_TEXT));
	if (NULL != descView)
	{
		mbcString.Empty();
		if (NULL != coverDef)
		{
			UTF8Conv::ToMultiByte(mbcString, coverDef->GetDescription());
		}	
		descView->SetText(mbcString);
	}
	
	BTextView* aboutView = dynamic_cast<BTextView*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_TEXT));
	if (NULL != aboutView)
	{
		mbcString.Empty();
		if (NULL != coverDef)
		{
			UTF8Conv::ToMultiByteWithLineEnding(mbcString, coverDef->GetAbout());
		}	
		aboutView->SetText(mbcString);
	}
}

// ---------------------------------------------------------------------
//! Message handler.
// ---------------------------------------------------------------------
void BeAboutCurrentCoverDialog::MessageReceived(
	BMessage *message		//!< received message.
)
{
	try
	{
		switch (message->what)
		{
		case ID_DIALOG_OK:
			PostMessage(B_QUIT_REQUESTED);
			break;
			
		case ID_ABOUTCURCOV_SETDATA:
			{
				void* pointer = NULL;
				if (B_OK == message->FindPointer(ABOUT_CURCOV_SETDATA_PARAM_COVERDEF, &pointer))
				{
					const CoverDef* coverDef = static_cast<CoverDef*>(pointer);
					setDataToDialogInThisThread(coverDef);
				}
				message->SendReply(ID_PROCESSED);
			}
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
//! Called when the window is requested to close.
// ---------------------------------------------------------------------
bool BeAboutCurrentCoverDialog::QuitRequested()
{
	terminate();
	
	return baseWindow::QuitRequested();
}
