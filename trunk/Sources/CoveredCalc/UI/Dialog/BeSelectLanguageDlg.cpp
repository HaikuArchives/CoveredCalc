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
	@file		BeSelectLanguageDlg.cpp
	@brief		Implementation of BeSelectLanguageDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.09.19 created
*/

#include "Prefix.h"
#include "BeSelectLanguageDlg.h"
#include <InterfaceKit.h>
#include "CommandID.h"
#include "CoveredCalcApp.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "DialogID.h"
#include "StringID.h"
#include "BeDialogControlHelper.h"
#include "UICEventCode.h"
#if defined(ZETA)
#include <locale/Locale.h>
#endif // defined(ZETA)

////////////////////////////////////////
#define baseWindow	BeModalDialogWindow
#define base		SelectLanguageDlg
////////////////////////////////////////

static const char SELECT_LANGUAGE_DIALOG_VIEW_BASE_VIEW[]			= "BaseView";
static const char SELECT_LANGUAGE_DIALOG_VIEW_LANG_MESSAGE[]		= "LangMessage";
static const char SELECT_LANGUAGE_DIALOG_VIEW_LANG_POPUP[]			= "LangPopup";
static const char SELECT_LANGUAGE_DIALOG_VIEW_OK[]					= "OKButton";
static const char SELECT_LANGUAGE_DIALOG_VIEW_CANCEL[]				= "CancelButton";

/**
 *	@brief	Constructor
 */
BeSelectLanguageDlg::BeSelectLanguageDlg()
{
}

/**
 *	@brief	Destructor
 */
BeSelectLanguageDlg::~BeSelectLanguageDlg()
{
}

/**
 * @brief Initializes the object.
 */
void BeSelectLanguageDlg::Init()
{
	DialogWindow* dlg = new DialogWindow();
	try
	{
		dlg->Init(this);
	}
	catch (...)
	{
		delete dlg;
		throw;
	}

	dialogWindow = dlg;
}

/**
 *	@brief Returns semaphore name
 *	@return semaphore name
 */
ConstAStr BeSelectLanguageDlg::getSemaphoreName()
{
	return "BeSelectLanguageDlg";
}

/**
 *	@brief	Returns MessageBoxProvider object.
 *	@return	MessageBoxProvider object.
 */
MessageBoxProvider*	BeSelectLanguageDlg::getMessageBoxProvider()
{
	return CoveredCalcApp::GetInstance();
}

/**
 *	@brief	Close this dialog.
 *	@param[in]	isOK	true if dialog is closing by OK button.
 */
void BeSelectLanguageDlg::closeDialog(bool isOK)
{
	if (isOK)
	{
		dialogResult = BeModalDialog::DlgResult_OK;
	}
	else
	{
		dialogResult = BeModalDialog::DlgResult_Cancel;
	}	
	dialogWindow->PostMessage(B_QUIT_REQUESTED);
}

/**
 * @brief Constructor
 */
BeSelectLanguageDlg::DialogWindow::DialogWindow()
	: baseWindow(IDD_SELECT_LANGUAGE,
				B_TITLED_WINDOW,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_MINIMIZABLE)
{
	langMenu = NULL;
}

/**
 * @brief Destructor
 */
BeSelectLanguageDlg::DialogWindow::~DialogWindow()
{
}

/**
 *	@brief	Creates child views.
 */
void BeSelectLanguageDlg::DialogWindow::createViews()
{
	BeDialogControlHelper dch(getDialogLayout());
	NativeStringLoader* nsl = CoveredCalcApp::GetInstance();

	rgb_color viewColor = { 216, 216, 216, 255 };
	rgb_color highColor = { 0, 0, 0, 255 };

	// dialog title
	SetTitle(nsl->LoadNativeString(NSID_SELECT_LANGUAGE_TITLE));

	// BaseView
	BView* baseView = new BView(Bounds(), SELECT_LANGUAGE_DIALOG_VIEW_BASE_VIEW,
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(baseView);
	
	baseView->SetViewColor(viewColor);
	
	// LangMessage
	BRect frameRect = dch.GetItemRect(ALITERAL("IDC_STATIC_MESSAGE"), ITEMNAME_WINDOW);
	BRect textRect = frameRect;
	textRect.OffsetTo(0, 0);
	BTextView* langMessage = new BTextView(frameRect, SELECT_LANGUAGE_DIALOG_VIEW_LANG_MESSAGE,
									textRect, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	baseView->AddChild(langMessage);
	
	const char* text = nsl->LoadNativeString(NSID_SELECT_LANGUAGE_MESSAGE);
	langMessage->SetText(text);
	langMessage->SetFontAndColor(0, strlen(text), be_plain_font, B_FONT_ALL, &highColor);
	langMessage->SetViewColor(viewColor);
	langMessage->MakeEditable(false);

	// LangPopup
	langMenu = new BMenu("");
	langMenu->SetLabelFromMarked(true);
	BMenuField* langPopup = new BMenuField(dch.GetItemRect(ALITERAL("IDC_CMB_LANGUAGE"), ITEMNAME_WINDOW), SELECT_LANGUAGE_DIALOG_VIEW_LANG_POPUP,
									nsl->LoadNativeString(NSID_SELECT_LANGUAGE_LANGUAGE), langMenu);
	baseView->AddChild(langPopup);
	
	langPopup->SetDivider(dch.GetItemPos(true, ALITERAL("IDC_CMB_LANGUAGE.divider"), ALITERAL("IDC_CMB_LANGUAGE.left")));
	langPopup->SetAlignment(B_ALIGN_LEFT);
	owner->uicLanguageListBox.Init(langPopup, ID_NULL);
	
	// CancelButton
	BButton* cancelButton = new BButton(dch.GetItemRect(ALITERAL("IDCANCEL"), ITEMNAME_WINDOW), SELECT_LANGUAGE_DIALOG_VIEW_CANCEL,
								nsl->LoadNativeString(NSID_SELECT_LANGUAGE_CANCEL), new BMessage(ID_DIALOG_CANCEL));
	baseView->AddChild(cancelButton);

	// OKButton
	BButton* okButton = new BButton(dch.GetItemRect(ALITERAL("IDOK"), ITEMNAME_WINDOW), SELECT_LANGUAGE_DIALOG_VIEW_OK,
								nsl->LoadNativeString(NSID_SELECT_LANGUAGE_OK), new BMessage(ID_DIALOG_OK));
	baseView->AddChild(okButton);
	
	SetDefaultButton(okButton);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed by LocaleKit.
 */
void BeSelectLanguageDlg::DialogWindow::languageChanged()
{
	NativeStringLoader* nsl = CoveredCalcApp::GetInstance();
	
	// dialog title
	SetTitle(nsl->LoadNativeString(NSID_SELECT_LANGUAGE_TITLE));

	// LangMessage
	BTextView* langMessage = dynamic_cast<BTextView*>(FindView(SELECT_LANGUAGE_DIALOG_VIEW_LANG_MESSAGE));
	if (NULL != langMessage)
	{
		const char* text = nsl->LoadNativeString(NSID_SELECT_LANGUAGE_MESSAGE);
		langMessage->SetText(text);
	}

	// LangPopup
	BMenuField* langPopup = dynamic_cast<BMenuField*>(FindView(SELECT_LANGUAGE_DIALOG_VIEW_LANG_POPUP));
	if (NULL != langPopup)
	{
		langPopup->SetLabel(nsl->LoadNativeString(NSID_SELECT_LANGUAGE_LANGUAGE));
	}
	
	// CancelButton
	BButton* cancelButton = dynamic_cast<BButton*>(FindView(SELECT_LANGUAGE_DIALOG_VIEW_CANCEL));
	if (NULL != cancelButton)
	{
		cancelButton->SetLabel(nsl->LoadNativeString(NSID_SELECT_LANGUAGE_CANCEL));
	}
	
	// OKButton
	BButton* okButton = dynamic_cast<BButton*>(FindView(SELECT_LANGUAGE_DIALOG_VIEW_OK));
	if (NULL != okButton)
	{
		okButton->SetLabel(nsl->LoadNativeString(NSID_SELECT_LANGUAGE_OK));
	}
}
#endif

/**
 *	@brief	Initializes the object.
 *	@param[in]	owner	BeSelectLanguageDlg object
 */
void BeSelectLanguageDlg::DialogWindow::Init(BeSelectLanguageDlg* owner)
{
	this->owner = owner;
	baseWindow::Init();
}

/**
 *	@brief	Initialize.
 */
void BeSelectLanguageDlg::DialogWindow::initDialog()
{
	createViews();
	moveToCenterOfScreen();
	
	owner->readyToShow();
}

/**
 *	@brief	Message handler.
 */
void BeSelectLanguageDlg::DialogWindow::MessageReceived(
	BMessage *message		//!< received message.
)
{
	try
	{
		switch (message->what)
		{
		case ID_DIALOG_OK:
			owner->HandleUICEvent(CID_OKButton, UICE_ButtonClicked, 0, NULL);
			break;
	
		case ID_DIALOG_CANCEL:
			owner->HandleUICEvent(CID_CancelButton, UICE_ButtonClicked, 0, NULL);
			break;

#if defined (ZETA)
		case B_LANGUAGE_CHANGED:
			// always use locake kit to show "Use LocakeKit" item label.
			// note that languageChanged() is called only when current setting
			// is "Use LocaleKit". so do here.
			if (NULL != langMenu && langMenu->CountItems() > 0)
			{
				BMenuItem* menuItem = langMenu->ItemAt(0);
				menuItem->SetLabel(_T(LangSelectHelper::LANGMENU_LOCALEKIT));
				if (menuItem->IsMarked())
				{
					menuItem->SetMarked(false);
					menuItem->SetMarked(true);
				}
			}
			baseWindow::MessageReceived(message);
			break;
#endif

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
