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
	@file		BePreferencesDlg.cpp
	@brief		Implementation of BePreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.03.12 created
*/

#include "Prefix.h"
#include "BePreferencesDlg.h"
#include <InterfaceKit.h>
#include "CommandID.h"
#include "CoveredCalcApp.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "DialogID.h"
#include "StringID.h"
#include "BeEditKeymapDlg.h"
#include "BeDialogControlHelper.h"
#include "UICEventCode.h"
#if defined(ZETA)
#include <locale/Locale.h>
#endif // defined(ZETA)

////////////////////////////////////////
#define baseWindow	BeDialog
#define base		PreferencesDlg
////////////////////////////////////////

static const bigtime_t MODAL_POLLING_WAIT = 100000;

static const char PREFERENCES_DIALOG_VIEW_BASE_VIEW[]			= "BaseView";
static const char PREFERENCES_DIALOG_VIEW_LANG_BOX[]			= "LangBox";
static const char PREFERENCES_DIALOG_VIEW_LANG_POPUP[]			= "LangPopup";
static const char PREFERENCES_DIALOG_VIEW_LANG_NOTICE[]			= "LangNotice";
static const char PREFERENCES_DIALOG_VIEW_KEYMAPPING_BOX[]		= "KeyMappingBox";
static const char PREFERENCES_DIALOG_VIEW_KEYMAPPING_POPUP[]	= "KeyMappingPopup";
static const char PREFERENCES_DIALOG_VIEW_EDIT_KEYMAPPING_BUTTON[] = "EditKeyMappingButton";
static const char PREFERENCES_DIALOG_VIEW_DUPLICATE_KEYMAPPING_BUTTON[] = "DuplicateKeyMappingButton";
static const char PREFERENCES_DIALOG_VIEW_DELETE_KEYMAPPING_BUTTON[] = "DeleteKeyMappingButton";
static const char PREFERENCES_DIALOG_VIEW_OK[]					= "OKButton";
static const char PREFERENCES_DIALOG_VIEW_CANCEL[]				= "CancelButton";


/**
 *	@brief	Constructor
 */
BePreferencesDlg::BePreferencesDlg()
	: BeDialog(IDD_PREFERENCES,
				B_TITLED_WINDOW,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_MINIMIZABLE)
{
	langMenu = NULL;
}

/**
 *	@brief	Destructor
 */
BePreferencesDlg::~BePreferencesDlg()
{
}

/**
 *	@brief	Creates child views.
 */
void BePreferencesDlg::createViews()
{
	BeDialogControlHelper dch(getDialogLayout());
	NativeStringLoader* nsl = CoveredCalcApp::GetInstance();

	rgb_color viewColor = { 216, 216, 216, 255 };
	rgb_color highColor = { 0, 0, 0, 255 };

	// dialog title
	SetTitle(nsl->LoadNativeString(NSID_PREFERENCES_TITLE));

	// BaseView
	BView* baseView = new BView(Bounds(), PREFERENCES_DIALOG_VIEW_BASE_VIEW,
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(baseView);
	
	baseView->SetViewColor(viewColor);
	
	// LangBox
	MBCString itemnameLangBox = ALITERAL("IDC_GROUP_LANGUAGE");
	BBox* langBox = new BBox(dch.GetItemRect(itemnameLangBox, ITEMNAME_WINDOW), PREFERENCES_DIALOG_VIEW_LANG_BOX);
	baseView->AddChild(langBox);
	
	langBox->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_GROUP_LANGUAGE));
	
	// LangPopup
	langMenu = new BMenu("");
	langMenu->SetLabelFromMarked(true);
	BMenuField* langPopup = new BMenuField(dch.GetItemRect(ALITERAL("IDC_CMB_LANGUAGE"), itemnameLangBox), PREFERENCES_DIALOG_VIEW_LANG_POPUP,
									nsl->LoadNativeString(NSID_PREFERENCES_LANGUAGE), langMenu);
	langBox->AddChild(langPopup);
	
	langPopup->SetDivider(dch.GetItemPos(true, ALITERAL("IDC_CMB_LANGUAGE.divider"), ALITERAL("IDC_CMB_LANGUAGE.left")));
	langPopup->SetAlignment(B_ALIGN_LEFT);
	uicLanguageListBox.Init(langPopup, ID_NULL);
	
	// LangNotice
	BRect frameRect = dch.GetItemRect(ALITERAL("IDC_STATIC_LANGUAGE_MESSAGE"), itemnameLangBox);
	BRect textRect = frameRect;
	textRect.OffsetTo(0, 0);
	BTextView* langNotice = new BTextView(frameRect, PREFERENCES_DIALOG_VIEW_LANG_NOTICE,
									textRect, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	langBox->AddChild(langNotice);
	
	const char* text = nsl->LoadNativeString(NSID_PREFERENCES_LANGUAGE_MESSAGE);
	langNotice->SetText(text);
	langNotice->SetFontAndColor(0, strlen(text), be_plain_font, B_FONT_ALL, &highColor);
	langNotice->SetViewColor(viewColor);
	langNotice->MakeEditable(false);

	// KeyMappingBox
	MBCString itemnameKeyMappingBox = ALITERAL("IDC_GROUP_KEYMAP");
	BBox* keyMappingBox = new BBox(dch.GetItemRect(itemnameKeyMappingBox, ITEMNAME_WINDOW), PREFERENCES_DIALOG_VIEW_KEYMAPPING_BOX);
	baseView->AddChild(keyMappingBox);

	keyMappingBox->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_GROUP_KEYMAP));

	// KeyMappingPopup
	BMenu* keyMappingMenu = new BMenu("");
	keyMappingMenu->SetLabelFromMarked(true);
	BMenuField* keyMappingPopup = new BMenuField(dch.GetItemRect(ALITERAL("IDC_CMB_KEYMAPPINGS"), itemnameKeyMappingBox), PREFERENCES_DIALOG_VIEW_KEYMAPPING_POPUP,
										nsl->LoadNativeString(NSID_PREFERENCES_KEYMAP), keyMappingMenu);
	keyMappingBox->AddChild(keyMappingPopup);
	
	keyMappingPopup->SetDivider(dch.GetItemPos(true, ALITERAL("IDC_CMB_KEYMAPPINGS.divider"), ALITERAL("IDC_CMB_KEYMAPPINGS.left")));
	keyMappingPopup->SetAlignment(B_ALIGN_LEFT);
	uicKeyMapListBox.Init(keyMappingPopup, ID_PREF_KEYMAP_SELECTED);

	// EditKeyMappingButton
	BButton* editKeymapButton = new BButton(dch.GetItemRect(ALITERAL("IDC_EDIT_KEYMAPPING"), itemnameKeyMappingBox), PREFERENCES_DIALOG_VIEW_EDIT_KEYMAPPING_BUTTON,
								nsl->LoadNativeString(NSID_PREFERENCES_EDIT_KEYMAP), new BMessage(ID_PREF_EDIT_KEYMAP));
	keyMappingBox->AddChild(editKeymapButton);
	uicEditKeyMapButton.Init(editKeymapButton);
								
	// DuplicateKeyMappingButton
	BButton* dupKeymapButton = new BButton(dch.GetItemRect(ALITERAL("IDC_DUPLICATE_KEYMAPPING"), itemnameKeyMappingBox), PREFERENCES_DIALOG_VIEW_DUPLICATE_KEYMAPPING_BUTTON,
								nsl->LoadNativeString(NSID_PREFERENCES_DUPLICATE_KEYMAP), new BMessage(ID_PREF_DUPLICATE_KEYMAP));
	keyMappingBox->AddChild(dupKeymapButton);
	uicDuplicateKeyMapButton.Init(dupKeymapButton);

	// DeleteKeyMappingButton
	BButton* delKeymapButton = new BButton(dch.GetItemRect(ALITERAL("IDC_DELETE_KEYMAPPING"), itemnameKeyMappingBox), PREFERENCES_DIALOG_VIEW_DELETE_KEYMAPPING_BUTTON,
								nsl->LoadNativeString(NSID_PREFERENCES_DELETE_KEYMAP), new BMessage(ID_PREF_DELETE_KEYMAP));
	keyMappingBox->AddChild(delKeymapButton);
	uicDeleteKeyMapButton.Init(delKeymapButton);

	// CancelButton
	BButton* cancelButton = new BButton(dch.GetItemRect(ALITERAL("IDCANCEL"), ITEMNAME_WINDOW), PREFERENCES_DIALOG_VIEW_CANCEL,
								nsl->LoadNativeString(NSID_PREFERENCES_CANCEL), new BMessage(ID_DIALOG_CANCEL));
	baseView->AddChild(cancelButton);

	// OKButton
	BButton* okButton = new BButton(dch.GetItemRect(ALITERAL("IDOK"), ITEMNAME_WINDOW), PREFERENCES_DIALOG_VIEW_OK,
								nsl->LoadNativeString(NSID_PREFERENCES_OK), new BMessage(ID_DIALOG_OK));
	baseView->AddChild(okButton);
	
	SetDefaultButton(okButton);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed by LocaleKit.
 */
void BePreferencesDlg::languageChanged()
{
	NativeStringLoader* nsl = CoveredCalcApp::GetInstance();
	
	// dialog title
	SetTitle(nsl->LoadNativeString(NSID_PREFERENCES_TITLE));

	// LangBox	
	BBox* langBox = dynamic_cast<BBox*>(FindView(PREFERENCES_DIALOG_VIEW_LANG_BOX));
	if (NULL != langBox)
	{
		langBox->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_GROUP_LANGUAGE));
	}
	
	// LangPopup
	BMenuField* langPopup = dynamic_cast<BMenuField*>(FindView(PREFERENCES_DIALOG_VIEW_LANG_POPUP));
	if (NULL != langPopup)
	{
		langPopup->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_LANGUAGE));
	}
	
	// LangNotice
	BTextView* langNotice = dynamic_cast<BTextView*>(FindView(PREFERENCES_DIALOG_VIEW_LANG_NOTICE));
	if (NULL != langNotice)
	{
		langNotice->SetText(nsl->LoadNativeString(NSID_PREFERENCES_LANGUAGE_MESSAGE));
	}
	
	// KeyMappingBox
	BBox* keyMappingBox = dynamic_cast<BBox*>(FindView(PREFERENCES_DIALOG_VIEW_KEYMAPPING_BOX));
	if (NULL != keyMappingBox)
	{
		keyMappingBox->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_GROUP_KEYMAP));
	}
	
	// KeyMappingPopup
	BMenuField* keyMappingPopup = dynamic_cast<BMenuField*>(FindView(PREFERENCES_DIALOG_VIEW_KEYMAPPING_POPUP));
	if (NULL != keyMappingPopup)
	{
		keyMappingPopup->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_KEYMAP));
	}

	// EditKeyMappingButton
	BButton* editKeymapButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_EDIT_KEYMAPPING_BUTTON));
	if (NULL != editKeymapButton)
	{
		editKeymapButton->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_EDIT_KEYMAP));
	}
	
	// DuplicateKeyMappingButton
	BButton* dupKeymapButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_DUPLICATE_KEYMAPPING_BUTTON));
	if (NULL != dupKeymapButton)
	{
		dupKeymapButton->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_DUPLICATE_KEYMAP));
	}

	// DeleteKeyMappingButton
	BButton* delKeymapButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_DELETE_KEYMAPPING_BUTTON));
	if (NULL != delKeymapButton)
	{
		delKeymapButton->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_DELETE_KEYMAP));
	}
	
	// CancelButton
	BButton* cancelButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_CANCEL));
	if (NULL != cancelButton)
	{
		cancelButton->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_OK));
	}
	
	// OKButton
	BButton* okButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_OK));
	if (NULL != okButton)
	{
		okButton->SetLabel(nsl->LoadNativeString(NSID_PREFERENCES_CANCEL));
	}
}
#endif


/**
 *	@brief	Initialize.
 */
void BePreferencesDlg::initDialog()
{
	createViews();
	moveToCenterOfScreen();
	
	readyToShow();
}

/**
 *	@brief	Returns MessageBoxProvider object.
 *	@return	MessageBoxProvider object.
 */
MessageBoxProvider*	BePreferencesDlg::getMessageBoxProvider()
{
	return CoveredCalcApp::GetInstance();
}

/**
 *	@brief	Shows "Edit Keyboard" dialog and wait until it is closed.
 *	@param[in]		isReadOnly	true when read-only mode.
 *	@param[in,out]	keyMappings	Key-mappings definition.
 *	@retval	true	user closes the dialog by OK button.
 *	@retval	false	user closes the dialog by Cancel button.
 */
bool BePreferencesDlg::showEditKeyMapDialog(bool isReadOnly, KeyMappings& keyMappings)
{
	BeEditKeymapDlg dlg;
	dlg.SetKeyMappings(&keyMappings);
	dlg.Init(isReadOnly, CoveredCalcApp::GetInstance()->GetKeyNameDB());
	if (BeModalDialog::DlgResult_OK == dlg.DoModal(this))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 *	@brief	Message handler.
 */
void BePreferencesDlg::MessageReceived(
	BMessage *message		//!< received message.
)
{
	try
	{
		switch (message->what)
		{
		case ID_PREF_KEYMAP_SELECTED:
			HandleUICEvent(CID_KeyMapListBox, UICE_SelectionChanged, 0, NULL);
			break;
		
		case ID_PREF_EDIT_KEYMAP:
			HandleUICEvent(CID_EditKeyMapButton, UICE_ButtonClicked, 0, NULL);
			break;
			
		case ID_PREF_DUPLICATE_KEYMAP:
			HandleUICEvent(CID_DuplicateKeyMapButton, UICE_ButtonClicked, 0, NULL);
			break;
		
		case ID_PREF_DELETE_KEYMAP:
			HandleUICEvent(CID_DeleteKeyMapButton, UICE_ButtonClicked, 0, NULL);
			break;
		
		case ID_DIALOG_OK:
			HandleUICEvent(CID_OKButton, UICE_ButtonClicked, 0, NULL);
			break;
	
		case ID_DIALOG_CANCEL:
			HandleUICEvent(CID_CancelButton, UICE_ButtonClicked, 0, NULL);
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

/**
 *	@brief	Close this dialog.
 *	@param[in]	isOK	true if dialog is closing by OK button.
 */
void BePreferencesDlg::closeDialog(bool /* isOK */)
{
	PostMessage(B_QUIT_REQUESTED);
}
