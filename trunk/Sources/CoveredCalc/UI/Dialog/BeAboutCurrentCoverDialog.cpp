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
	@file		BeAboutCurrentCoverDialog.cpp
	@brief		Implementation of BeAboutCurrentCoverDialog class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.24 created
*/

#include "Prefix.h"
#include "BeAboutCurrentCoverDialog.h"
#include <InterfaceKit.h>
#include <support/Autolock.h>
#include "DialogID.h"
#include "StringID.h"
#include "CommandID.h"
#include "MBCString.h"
#include "CoverDef.h"
#include "UTF8Conv.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcApp.h"
#include "BeDialogControlHelper.h"

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

/**
 *	@brief	Constructor
 */
BeAboutCurrentCoverDialog::BeAboutCurrentCoverDialog()
	: BeDialog(IDD_ABOUT_COVER,
				B_TITLED_WINDOW,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_MINIMIZABLE)
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeAboutCurrentCoverDialog::~BeAboutCurrentCoverDialog()
{
}

/**
 *	@brief	Creates child views.
 */
void BeAboutCurrentCoverDialog::createViews()
{
	BeDialogControlHelper dch(getDialogLayout());
	NativeStringLoader* nsl = CoveredCalcApp::GetInstance();

	rgb_color viewColor = { 216, 216, 216, 255 };
	rgb_color highColor = { 0, 0, 0, 255 };

	// dialog title
	SetTitle(nsl->LoadNativeString(NSID_ABOUT_COVER_TITLE).CString());

	// BaseView
	BView* baseView = new BView(Bounds(), ABOUT_CURCOV_DIALOG_VIEW_BASE_VIEW,
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(baseView);
	
	baseView->SetViewColor(viewColor);
	
	// NameLabel
	BStringView* nameLabelView = new BStringView(dch.GetItemRect(ALITERAL("IDC_STATIC_NAME"), ITEMNAME_WINDOW), ABOUT_CURCOV_DIALOG_VIEW_NAME_LABEL,
									nsl->LoadNativeString(NSID_ABOUT_COVER_NAME).CString());
	baseView->AddChild(nameLabelView);
	
	// NameText
	BTextView* nameTextView = new BTextView(dch.GetItemRect(ALITERAL("IDC_EDIT_NAME"), ITEMNAME_WINDOW), ABOUT_CURCOV_DIALOG_VIEW_NAME_TEXT, BRect(0, 0, 0, 0),
								B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	baseView->AddChild(nameTextView);
	
	nameTextView->SetTextRect(BRect(0, 0, 1024, nameTextView->Bounds().Height()));
	nameTextView->SetFontAndColor(be_plain_font, B_FONT_ALL, &highColor);
	nameTextView->SetViewColor(viewColor);
	nameTextView->SetWordWrap(false);
	nameTextView->MakeEditable(false);

	uicNameTextEdit.Init(nameTextView);
	
	// DescLabel
	BStringView* descLabelView = new BStringView(dch.GetItemRect(ALITERAL("IDC_STATIC_DESCRIPTION"), ITEMNAME_WINDOW), ABOUT_CURCOV_DIALOG_VIEW_DESC_LABEL,
									nsl->LoadNativeString(NSID_ABOUT_COVER_DESCRIPTION).CString());
	baseView->AddChild(descLabelView);
	
	// DescText
	BTextView* descTextView = new BTextView(dch.GetItemRect(ALITERAL("IDC_EDIT_DESCRIPTION"), ITEMNAME_WINDOW), ABOUT_CURCOV_DIALOG_VIEW_DESC_TEXT, BRect(0, 0, 0, 0),
								B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	baseView->AddChild(descTextView);
	
	descTextView->SetTextRect(BRect(0, 0, 1024, descTextView->Bounds().Height()));
	descTextView->SetFontAndColor(be_plain_font, B_FONT_ALL, &highColor);
	descTextView->SetViewColor(viewColor);
	descTextView->SetWordWrap(false);
	descTextView->MakeEditable(false);

	uicDescriptionTextEdit.Init(descTextView);
	
	// AboutCoverLabel
	BStringView* aboutCoverLabelView = new BStringView(dch.GetItemRect(ALITERAL("IDC_STATIC_ABOUT"), ITEMNAME_WINDOW), ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_LABEL,
										nsl->LoadNativeString(NSID_ABOUT_COVER_ABOUT));
	baseView->AddChild(aboutCoverLabelView);
	
	// AboutCoverText
	BRect frameRect = dch.GetItemRect(ALITERAL("IDC_EDIT_ABOUT"), ITEMNAME_WINDOW);
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
	
	uicAboutTextEdit.Init(aboutCoverTextView);

	// OKButton
	BButton* okButton = new BButton(dch.GetItemRect(ALITERAL("IDOK"), ITEMNAME_WINDOW), ABOUT_CURCOV_DIALOG_VIEW_OK,
								nsl->LoadNativeString(NSID_ABOUT_COVER_OK).CString(), new BMessage(ID_DIALOG_OK));
	baseView->AddChild(okButton);
	
	SetDefaultButton(okButton);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed by LocaleKit.
 */
void BeAboutCurrentCoverDialog::languageChanged()
{
	NativeStringLoader* nsl = CoveredCalcApp::GetInstance();

	// dialog title
	SetTitle(nsl->LoadNativeString(NSID_ABOUT_COVER_TITLE).CString());

	// NameLabel
	BStringView* nameLabelView = dynamic_cast<BStringView*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_NAME_LABEL));
	if (NULL != nameLabelView)
	{
		nameLabelView->SetText(nsl->LoadNativeString(NSID_ABOUT_COVER_NAME).CString());
	}		

	// DescLabel
	BStringView* descLabelView = dynamic_cast<BStringView*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_DESC_LABEL));
	if (NULL != descLabelView)
	{
		descLabelView->SetText(nsl->LoadNativeString(NSID_ABOUT_COVER_DESCRIPTION).CString());
	}

	// AboutCoverLabel
	BStringView* aboutCoverLabelView = dynamic_cast<BStringView*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_ABOUT_COVER_LABEL));
	if (NULL != aboutCoverLabelView)
	{
		aboutCoverLabelView->SetText(nsl->LoadNativeString(NSID_ABOUT_COVER_ABOUT).CString());
	}

	// OKButton
	BButton* okButton = dynamic_cast<BButton*>(FindView(ABOUT_CURCOV_DIALOG_VIEW_OK));
	if (NULL != okButton)
	{
		okButton->SetLabel(nsl->LoadNativeString(NSID_ABOUT_COVER_OK).CString());
	}
}
#endif

// ---------------------------------------------------------------------
//! Initialize.
// ---------------------------------------------------------------------
void BeAboutCurrentCoverDialog::initDialog()
{
	createViews();
	moveToCenterOfScreen();

	// invoke thread in invisible.
	Hide();
	Show();

	initialize();
}

/**
 *	@brief	Updates data in the dialog.
 */
void BeAboutCurrentCoverDialog::update()
{
	if (Thread() == find_thread(NULL))
	{
		base::update();
	}
	else
	{
		BMessage message(ID_ABOUTCURCOV_DO_UPDATE);
		BMessage reply;
		BMessenger(this).SendMessage(&message, &reply);		// wait until processed.
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
		
		case ID_ABOUTCURCOV_DO_UPDATE:
			try
			{
				base::update();
			}
			catch (...)
			{
				message->SendReply(ID_PROCESSED);
				throw;
			}
			message->SendReply(ID_PROCESSED);
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
