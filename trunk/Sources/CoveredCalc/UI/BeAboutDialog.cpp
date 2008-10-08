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
	@file		BeAboutDialog.cpp
	@brief		Definition of BeAboutDialog class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.12.16 created
*/

#include "Prefix.h"
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <support/Autolock.h>
#include <translation/TranslationUtils.h>
#include "BeAboutDialog.h"
#include "DialogID.h"
#include "CommandID.h"
#include "GenericException.h"
#include "BeBitmapView.h"
#include "ResourceID.h"
#include "Copyright.h"
#include "BeDialogDesign.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcApp.h"

////////////////////////////////////////
#define base	BeDialog
////////////////////////////////////////

static const char ABOUT_DIALOG_VIEW_BASE_VIEW[]			= "BaseView";
static const char ABOUT_DIALOG_VIEW_APP_NAME_LABEL[]	= "AppNameLabel";
static const char ABOUT_DIALOG_VIEW_VERSION_LABEL[]		= "VersionLabel";
static const char ABOUT_DIALOG_VIEW_COPYRIGHT_LABEL[]	= "CopyrightLabel";
static const char ABOUT_DIALOG_VIEW_LOGO_ICON[]			= "LogoIcon";
static const char ABOUT_DIALOG_VIEW_OK[]				= "OKButton";

static const char STR_ABOUT_COVERED_CALC[]	= "About CoveredCalc";
static const char STR_OK[]					= "OK";

/**
 *	@brief	Constructor
 */
BeAboutDialog::BeAboutDialog(
	BeDialogDesign* dialogDesign		///< dialog design information
)
	: BeDialog(dialogDesign->GetFrame(BRect(0.0, 0.0, 392.0, 111.0)),
				LT(dialogDesign->GetTitle(STR_ABOUT_COVERED_CALC)),
				B_TITLED_WINDOW,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_MINIMIZABLE)
{
	this->dialogDesign = dialogDesign;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeAboutDialog::~BeAboutDialog()
{
	if (NULL != dialogDesign)
		delete dialogDesign;
}

/**
 *	@brief	Creates child views.
 */
void BeAboutDialog::createViews()
{
	const BeDialogDesignItem* item;

	rgb_color highColor;
	highColor.red = 0x00;
	highColor.green = 0x40;
	highColor.blue = 0x80;
	highColor.alpha = 0xff;

	// BaseView
	BView* baseView = new BView(Bounds(), ABOUT_DIALOG_VIEW_BASE_VIEW,
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(baseView);
	
	baseView->SetViewColor(0xff, 0xff, 0xff, 0xff);
	
	// LogoIcon
	item = dialogDesign->FindItem(ABOUT_DIALOG_VIEW_LOGO_ICON);
	BeBitmapView* logoView = new BeBitmapView(item->GetFrame(BRect(12.0, 12.0, 76.0, 76.0)), ABOUT_DIALOG_VIEW_LOGO_ICON,
								B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	baseView->AddChild(logoView);
	BBitmap* bitmap = BTranslationUtils::GetBitmap('PNG ', IDB_APPICON64);
	if (NULL != bitmap)
	{
		// resize view and center image
		BRect bitmapRect = bitmap->Bounds();
		BRect orgRect = logoView->Bounds();
		logoView->ResizeTo(bitmapRect.Width(), bitmapRect.Height());
		logoView->MoveBy((orgRect.Width() - bitmapRect.Width()) / 2, (orgRect.Height() - bitmapRect.Height()) / 2);

		// set bitmap to view.
		logoView->SetBitmap(bitmap);
	}
	
	// AppNameLabel
	item = dialogDesign->FindItem(ABOUT_DIALOG_VIEW_APP_NAME_LABEL);
	BStringView* appNameLabelView = new BStringView(item->GetFrame(BRect(88.0, 12.0, 380.0, 24.0)),
										ABOUT_DIALOG_VIEW_APP_NAME_LABEL, "");
	baseView->AddChild(appNameLabelView);
	
	appNameLabelView->SetHighColor(highColor);
	appNameLabelView->SetText(ApplicationTitleString);

	// VersionLabel
	item = dialogDesign->FindItem(ABOUT_DIALOG_VIEW_VERSION_LABEL);
	BStringView* versionView = new BStringView(item->GetFrame(BRect(88.0, 30.0, 380.0, 42.0)),
										ABOUT_DIALOG_VIEW_VERSION_LABEL, "");
	baseView->AddChild(versionView);

	versionView->SetHighColor(highColor);
	
	MBCString versionString;
	makeVersionString(versionString);
	versionView->SetText(versionString);
	
	// CopyrightLabel
	item = dialogDesign->FindItem(ABOUT_DIALOG_VIEW_COPYRIGHT_LABEL);
	BStringView* copyLabelView = new BStringView(item->GetFrame(BRect(88.0, 48.0, 380.0, 60.0)),
										ABOUT_DIALOG_VIEW_COPYRIGHT_LABEL, "");
	baseView->AddChild(copyLabelView);
	
	copyLabelView->SetHighColor(highColor);
	copyLabelView->SetText(CopyrightString1);
	
	// OKButton
	item = dialogDesign->FindItem(ABOUT_DIALOG_VIEW_OK);
	BButton* okButton = new BButton(item->GetFrame(BRect(300.0, 75.0, 380.0, 99.0)), ABOUT_DIALOG_VIEW_OK,
								LT(item->GetLabel(STR_OK)), new BMessage(ID_DIALOG_OK));
	baseView->AddChild(okButton);
	
	SetDefaultButton(okButton);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed by LocaleKit.
 */
void BeAboutDialog::languageChanged()
{
	const BeDialogDesignItem* item;

	// dialog title
	SetTitle(LT(dialogDesign->GetTitle(STR_ABOUT_COVERED_CALC)));

	// OKButton
	item = dialogDesign->FindItem(ABOUT_DIALOG_VIEW_OK);
	BButton* applyButton = dynamic_cast<BButton*>(FindView(ABOUT_DIALOG_VIEW_OK));
	if (NULL != applyButton)
	{
		applyButton->SetLabel(LT(item->GetLabel(STR_OK)));
	}
}
#endif

// ---------------------------------------------------------------------
//! Initialize.
// ---------------------------------------------------------------------
void BeAboutDialog::Init()
{
	createViews();
	base::Init();
	
	moveToCenterOfScreen();	
}

// ---------------------------------------------------------------------
//! Makes version string.
// ---------------------------------------------------------------------
void BeAboutDialog::makeVersionString(
	MBCString& versionString		//!< [out] version string is returned
)
{
	versionString = "Version ";
	
	app_info appInfo;
	BFile file;
	BAppFileInfo appFileInfo;
	
	be_app->GetAppInfo(&appInfo);
	file.SetTo(&appInfo.ref, B_READ_ONLY);
	appFileInfo.SetTo(&file);
	
	version_info verInfo;
	if (B_OK == appFileInfo.GetVersionInfo(&verInfo, B_APP_VERSION_KIND))
	{
		versionString += verInfo.short_info;
	}
}

// ---------------------------------------------------------------------
//! Message handler.
// ---------------------------------------------------------------------
void BeAboutDialog::MessageReceived(
	BMessage *message		//!< received message.
)
{
	try
	{
		switch (message->what)
		{
		case ID_DIALOG_OK:
			doCloseDialog();
			break;
		default:
			base::MessageReceived(message);
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
//! Closes this dialog.
// ---------------------------------------------------------------------
void BeAboutDialog::doCloseDialog()
{
	PostMessage(B_QUIT_REQUESTED);
}
