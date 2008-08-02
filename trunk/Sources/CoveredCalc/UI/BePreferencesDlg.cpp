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
	@file		BePreferencesDlg.cpp
	@brief		Implementation of BePreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.03.12 created
*/

#include "Prefix.h"
#include "BePreferencesDlg.h"
#include <InterfaceKit.h>
#include "BeDialogDesign.h"
#include "CommandID.h"
#include "LangFileInfo.h"
#include "CoveredCalcApp.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "BeDataMenuItem.h"
#include "BeXMLLangFile.h"
#include "BeCoveredCalcApp.h"
#include "DialogID.h"
#include "BeEditKeymapDlg.h"
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

#if defined(ZETA)
static const char PREFERENCES_DIALOG_LANGMENU_LOCALEKIT[]	= "Use ZETA's Locale Kit";
#endif // defined(ZETA)

static const char STR_PREFERENCES[]		= "Preferences";
static const char STR_LANGUAGE_BOX[]	= "Language";
static const char STR_LANGUAGE[]		= "Language:";
static const char STR_LANG_NOTICE[]		= "Change of language will take effect after you restart CoveredCalc application.";
static const char STR_KEYMAPPING_BOX[]	= "Keyboard";
static const char STR_KEYMAPPING[]		= "Keyboard:";
static const char STR_EDIT[]			= "Edit";
static const char STR_DUPLICATE[]		= "Duplicate";
static const char STR_DELETE[]			= "Delete";
static const char STR_CANCEL[]			= "Cancel";
static const char STR_OK[]				= "OK";

/**
 *	@brief	Constructor
 */
BePreferencesDlg::BePreferencesDlg(
	BeDialogDesign* dialogDesign	///< dialog design information
)
	: BeDialog(dialogDesign->GetFrame(BRect(0.0, 0.0, 300.0, 246.0)),
				LT(dialogDesign->GetTitle(STR_PREFERENCES)),
				B_TITLED_WINDOW,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_MINIMIZABLE)
{
	this->dialogDesign = dialogDesign;
	langMenu = NULL;
	keyMappingMenu = NULL;
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
	const BeDialogDesignItem* item;

	rgb_color viewColor = { 216, 216, 216, 255 };
	rgb_color highColor = { 0, 0, 0, 255 };

	// BaseView
	BView* baseView = new BView(Bounds(), PREFERENCES_DIALOG_VIEW_BASE_VIEW,
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(baseView);
	
	baseView->SetViewColor(viewColor);
	
	// LangBox
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_LANG_BOX);
	BBox* langBox = new BBox(item->GetFrame(BRect(12.0, 12.0, 288.0, 100.0)), PREFERENCES_DIALOG_VIEW_LANG_BOX);
	baseView->AddChild(langBox);
	
	langBox->SetLabel(LT(item->GetLabel(STR_LANGUAGE_BOX)));
	
	// LangPopup
	langMenu = new BMenu("");
	langMenu->SetLabelFromMarked(true);
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_LANG_POPUP);
	BMenuField* langPopup = new BMenuField(item->GetFrame(BRect(12.0, 16.0, 264.0, 38.0)), PREFERENCES_DIALOG_VIEW_LANG_POPUP,
									LT(item->GetLabel(STR_LANGUAGE)), langMenu);
	langBox->AddChild(langPopup);
	
	langPopup->SetDivider(item->GetDivider(64.0));
	langPopup->SetAlignment(B_ALIGN_LEFT);
	
	// LangNotice
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_LANG_NOTICE);
	BRect frameRect = item->GetFrame(BRect(12.0, 50.0, 264.0, 76.0));
	BRect textRect = frameRect;
	textRect.OffsetTo(0, 0);
	BTextView* langNotice = new BTextView(frameRect, PREFERENCES_DIALOG_VIEW_LANG_NOTICE,
									textRect, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	langBox->AddChild(langNotice);
	
	const char* text = LT(item->GetLabel(STR_LANG_NOTICE));
	langNotice->SetText(text);
	langNotice->SetFontAndColor(0, strlen(text), be_plain_font, B_FONT_ALL, &highColor);
	langNotice->SetViewColor(viewColor);
	langNotice->MakeEditable(false);

	// KeyMappingBox
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_KEYMAPPING_BOX);
	BBox* keyMappingBox = new BBox(item->GetFrame(BRect(12.0, 112.0, 288.0, 198.0)), PREFERENCES_DIALOG_VIEW_KEYMAPPING_BOX);
	baseView->AddChild(keyMappingBox);

	keyMappingBox->SetLabel(LT(item->GetLabel(STR_KEYMAPPING_BOX)));

	// KeyMappingPopup
	keyMappingMenu = new BMenu("");
	keyMappingMenu->SetLabelFromMarked(true);
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_KEYMAPPING_POPUP);
	BMenuField* keyMappingPopup = new BMenuField(item->GetFrame(BRect(12.0, 16.0, 264.0, 38.0)), PREFERENCES_DIALOG_VIEW_KEYMAPPING_POPUP,
										LT(item->GetLabel(STR_KEYMAPPING)), keyMappingMenu);
	keyMappingBox->AddChild(keyMappingPopup);
	
	keyMappingPopup->SetDivider(item->GetDivider(64.0));
	keyMappingPopup->SetAlignment(B_ALIGN_LEFT);

	// EditKeyMappingButton
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_EDIT_KEYMAPPING_BUTTON);
	BButton* editKeymapButton = new BButton(item->GetFrame(BRect(12.0, 50.0, 92.0, 74.0)), PREFERENCES_DIALOG_VIEW_EDIT_KEYMAPPING_BUTTON,
								LT(item->GetLabel(STR_EDIT)), new BMessage(ID_PREF_EDIT_KEYMAP));
	keyMappingBox->AddChild(editKeymapButton);
								
	// DuplicateKeyMappingButton
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_DUPLICATE_KEYMAPPING_BUTTON);
	BButton* dupKeymapButton = new BButton(item->GetFrame(BRect(98.0, 50.0, 178.0, 74.0)), PREFERENCES_DIALOG_VIEW_DUPLICATE_KEYMAPPING_BUTTON,
								LT(item->GetLabel(STR_DUPLICATE)), new BMessage(ID_PREF_DUPLICATE_KEYMAP));
	keyMappingBox->AddChild(dupKeymapButton);

	// DeleteKeyMappingButton
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_DELETE_KEYMAPPING_BUTTON);
	BButton* delKeymapButton = new BButton(item->GetFrame(BRect(184.0, 50.0, 264.0, 74.0)), PREFERENCES_DIALOG_VIEW_DELETE_KEYMAPPING_BUTTON,
								LT(item->GetLabel(STR_DELETE)), new BMessage(ID_PREF_DELETE_KEYMAP));
	keyMappingBox->AddChild(delKeymapButton);

	// CancelButton
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_CANCEL);
	BButton* cancelButton = new BButton(item->GetFrame(BRect(122.0, 210.0, 202.0, 234.0)), PREFERENCES_DIALOG_VIEW_CANCEL,
								LT(item->GetLabel(STR_CANCEL)), new BMessage(ID_DIALOG_CANCEL));
	baseView->AddChild(cancelButton);

	// OKButton
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_OK);
	BButton* okButton = new BButton(item->GetFrame(BRect(208.0, 210.0, 288.0, 234.0)), PREFERENCES_DIALOG_VIEW_OK,
								LT(item->GetLabel(STR_OK)), new BMessage(ID_DIALOG_OK));
	baseView->AddChild(okButton);
	
	SetDefaultButton(okButton);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed by LocaleKit.
 */
void BePreferencesDlg::languageChanged()
{
	const BeDialogDesignItem* item;
	
	// dialog title
	SetTitle(LT(dialogDesign->GetTitle(STR_PREFERENCES)));

	// LangBox	
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_LANG_BOX);
	BBox* langBox = dynamic_cast<BBox*>(FindView(PREFERENCES_DIALOG_VIEW_LANG_BOX));
	if (NULL != langBox)
	{
		langBox->SetLabel(LT(item->GetLabel(STR_LANGUAGE_BOX)));
	}
	
	// LangPopup
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_LANG_POPUP);
	BMenuField* langPopup = dynamic_cast<BMenuField*>(FindView(PREFERENCES_DIALOG_VIEW_LANG_POPUP));
	if (NULL != langPopup)
	{
		langPopup->SetLabel(LT(item->GetLabel(STR_LANGUAGE)));
	}
	
	// LangNotice
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_LANG_NOTICE);
	BTextView* langNotice = dynamic_cast<BTextView*>(FindView(PREFERENCES_DIALOG_VIEW_LANG_NOTICE));
	if (NULL != langNotice)
	{
		langNotice->SetText(LT(item->GetLabel(STR_LANG_NOTICE)));
	}
	
	// KeyMappingBox
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_KEYMAPPING_BOX);
	BBox* keyMappingBox = dynamic_cast<BBox*>(FindView(PREFERENCES_DIALOG_VIEW_KEYMAPPING_BOX));
	if (NULL != keyMappingBox)
	{
		keyMappingBox->SetLabel(LT(item->GetLabel(STR_KEYMAPPING_BOX)));
	}
	
	// KeyMappingPopup
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_KEYMAPPING_POPUP);
	BMenuField* keyMappingPopup = dynamic_cast<BMenuField*>(FindView(PREFERENCES_DIALOG_VIEW_KEYMAPPING_POPUP));
	if (NULL != keyMappingPopup)
	{
		keyMappingPopup->SetLabel(LT(item->GetLabel(STR_KEYMAPPING)));
	}

	// EditKeyMappingButton
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_EDIT_KEYMAPPING_BUTTON);
	BButton* editKeymapButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_EDIT_KEYMAPPING_BUTTON));
	if (NULL != editKeymapButton)
	{
		editKeymapButton->SetLabel(LT(item->GetLabel(STR_EDIT)));
	}
	
	// DuplicateKeyMappingButton
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_DUPLICATE_KEYMAPPING_BUTTON);
	BButton* dupKeymapButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_DUPLICATE_KEYMAPPING_BUTTON));
	if (NULL != dupKeymapButton)
	{
		dupKeymapButton->SetLabel(LT(item->GetLabel(STR_DUPLICATE)));
	}

	// DeleteKeyMappingButton
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_DELETE_KEYMAPPING_BUTTON);
	BButton* delKeymapButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_DELETE_KEYMAPPING_BUTTON));
	if (NULL != delKeymapButton)
	{
		delKeymapButton->SetLabel(LT(item->GetLabel(STR_DELETE)));
	}
	
	// CancelButton
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_CANCEL);
	BButton* cancelButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_CANCEL));
	if (NULL != cancelButton)
	{
		cancelButton->SetLabel(LT(item->GetLabel(STR_CANCEL)));
	}
	
	// OKButton
	item = dialogDesign->FindItem(PREFERENCES_DIALOG_VIEW_OK);
	BButton* okButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_OK));
	if (NULL != okButton)
	{
		okButton->SetLabel(LT(item->GetLabel(STR_OK)));
	}
}
#endif


/**
 *	@brief	Initialize.
 */
void BePreferencesDlg::Init()
{
	createViews();
	baseWindow::Init();
	moveToCenterOfScreen();
	
	loadToDialog();
}

/**
 *	@brief	Returns MessageBoxProvider object.
 *	@return	MessageBoxProvider object.
 */
MessageBoxProvider*	BePreferencesDlg::getMessageBoxProvider()
{
	return CoveredCalcApp::GetInstance();
}

bool BePreferencesDlg::showEditKeyMapDialog(bool isReadOnly, KeyMappings& keyMappings)
{
	const BeXMLLangFile* langFile = BeCoveredCalcApp::GetInstance()->GetLangFile();
	if (NULL == langFile)
	{
		return false;
	}

	sem_id semDialog = create_sem(0, "BeEditKeymapDlg");
	if (semDialog < B_NO_ERROR)
	{
		return false;
	}

	BeDialogDesign* dialogDesign = langFile->LoadDialogDesign(IDD_EDIT_KEYMAP);
	BeEditKeymapDlg* dlg = new BeEditKeymapDlg(dialogDesign);
	dlg->SetKeyMappings(&keyMappings);
	dlg->Init(this, semDialog, isReadOnly, CoveredCalcApp::GetInstance()->GetKeyNameDB());
	dlg->Show();
	
	status_t acquireResult;
	do
	{
		acquireResult = acquire_sem_etc(semDialog, 1, B_RELATIVE_TIMEOUT, MODAL_POLLING_WAIT);
		UpdateIfNeeded();
	}
	while (B_NO_ERROR != acquireResult);

	delete_sem(semDialog);	
	
	return dlg->IsDialogClosedByOK();
}

/**
 *	@brief	compare function for sorting language item.
 */
static int langFileInfoCompareFunc(const void* item1, const void* item2)
{
	const LangFileInfo* info1 = *static_cast<const LangFileInfo* const *>(item1);
	const LangFileInfo* info2 = *static_cast<const LangFileInfo* const *>(item2);
	
	const MBCString& name1 = info1->GetLanguageName();
	const MBCString& name2 = info2->GetLanguageName();
	
	return name1.Compare(name2);
}

/**
 *	@brief	Sets the content of language popup menu and update current selection.
 */
void BePreferencesDlg::setLanguage(
	const LangFileInfoCollection& langFileInfos,	///< collection of language popup menu item
	const Path& currentLangFilePath					///< current selection
#if defined(ZETA)
	, bool isLocaleKitAvailable						///< whether uses locale kit
#endif // defined(ZETA)
)
{
	// make language infos.
	langMenuItemInfo.MakeEmpty();
	const LangFileInfo* selectedInfo = NULL;
	SInt32 count = langFileInfos.GetCount();
	SInt32 index;
	for (index=0; index<count; index++)
	{
		const LangFileInfo& info = langFileInfos.GetAt(index);
		langMenuItemInfo.AddItem(const_cast<LangFileInfo*>(&info));
		
		if (0 == info.GetPath().Compare(currentLangFilePath))
		{
			selectedInfo = &info;
		}
	}
	langMenuItemInfo.SortItems(langFileInfoCompareFunc);
	
	// add to menu.
#if defined(ZETA)
	BMenuItem* menuItem = new BMenuItem(_T(PREFERENCES_DIALOG_LANGMENU_LOCALEKIT), NULL);
	if (currentLangFilePath.IsEmpty() && isLocaleKitAvailable)
	{
		menuItem->SetMarked(true);
		selectedInfo = NULL;
	}
	langMenu->AddItem(menuItem);
#endif // defined(ZETA)
	for (index=0; index<count; index++)
	{
		const LangFileInfo* info = static_cast<LangFileInfo*>(langMenuItemInfo.ItemAt(index));
		BMenuItem* menuItem = new BMenuItem(info->GetLanguageName().CString(), NULL);
		menuItem->SetMarked((info == selectedInfo) ? true : false);
		langMenu->AddItem(menuItem);
	}
}

/**
 *	@brief	Retrieves current language popup selection.
 *	@return	true if succeeded, otherwise false.
 *	@note	when failed, the error message is shown in this function.
 */
bool BePreferencesDlg::getLanguage(
	Path &langFilePath					///< (OUTPUT) language file path
#if defined(ZETA)
	, bool& isLocaleKitAvailable		///< (OUTPUT) whether uses locale kit
#endif
)
{
	BMenuItem* markedItem = langMenu->FindMarked();
	if (NULL == markedItem)
	{
		CoveredCalcApp::GetInstance()->DoMessageBox(IDS_EMSG_INVALID_LANGUAGE,
				MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		return false;
	}
	int32 index = langMenu->IndexOf(markedItem);
	if (index < 0 ||
#if defined(ZETA)
			index >= langMenuItemInfo.CountItems() + 1
#else // !defined(ZETA)
			index >= langMenuItemInfo.CountItems()
#endif // defined(ZETA)	& !defined(ZETA)
	)
	{
		CoveredCalcApp::GetInstance()->DoMessageBox(IDS_EMSG_GET_LANGUAGE,
				MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);	
		return false;		
	}
#if defined(ZETA)
	if (0 == index)
	{
		langFilePath = Path();
		isLocaleKitAvailable = true;
		return true;
	}
	else
	{
		isLocaleKitAvailable = false;
		index--;
	}
#endif // defined(ZETA)	
	const LangFileInfo* info = static_cast<LangFileInfo*>(langMenuItemInfo.ItemAt(index));
	langFilePath = info->GetPath();
	return true;
}

typedef BeDataMenuItem<const PreferencesDlg::KeyMappingsInfo*> BeKMIMenuItem;

/**
 *	@brief	Sets key-mapping menu and current item.
 *	@param	keyMappingInfos			This collection contains informations about all key-mapping menu items.
 *	@param	currentKeyMappingPath	Current selection.
 *	@note	The keyMappingInfos object is valid until the next call of setKeyMapping, or until the dialog is closed.
 */
void BePreferencesDlg::setKeyMapping(const KeyMappingsInfoPtrVector& keyMappingsInfos, const Path& currentKeyMappingPath)
{
	// clear menu
	SInt32 oldCount = keyMappingMenu->CountItems();
	SInt32 ox;
	for (ox = oldCount - 1; ox >= 0; ox--)
	{
		BMenuItem* menuItem = keyMappingMenu->RemoveItem(ox);
		delete menuItem;
	}

	// add to menu
	KMCategory category = KMCategory_Invalid;
	SInt32 count = keyMappingsInfos.size();
	SInt32 index;
	for (index = 0; index < count; index++)
	{
		const KeyMappingsInfo* info = keyMappingsInfos[index];
		if (category != info->category)
		{
			if (KMCategory_Invalid != category)
			{
				keyMappingMenu->AddSeparatorItem();
			}
			category = info->category;
		}
		BeKMIMenuItem* menuItem = new BeKMIMenuItem(info->title.CString(), new BMessage(ID_PREF_KEYMAP_SELECTED));
		menuItem->SetItemData(info);
		menuItem->SetMarked(0 == info->keyMapFilePath.Compare(currentKeyMappingPath));
		keyMappingMenu->AddItem(menuItem);
	}
	processKeyMappingSelectionChanged();
}

/**
 *	@brief	Retrieves current item of key-mapping menu.
 *	@param[in]	doErrorProcessing	if this parameter is true and an error has occured, the error message is shown in this function.
 *	@return pointer to key-mapping file info which is selected on key-mapping menu. NULL should be returned when an error has occured.
 */
const PreferencesDlg::KeyMappingsInfo* BePreferencesDlg::getKeyMapping(bool doErrorProcessing)
{
	BMenuItem* markedItem = keyMappingMenu->FindMarked();
	if (NULL == markedItem)
	{
		if (doErrorProcessing)
		{
			CoveredCalcApp::GetInstance()->DoMessageBox(IDS_EMSG_INVALID_KEYMAPPINGS,
					MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		}
		return NULL;
	}
	
	const KeyMappingsInfo* info = NULL;
	BeKMIMenuItem* markedKMIItem = dynamic_cast<BeKMIMenuItem*>(markedItem);
	if (NULL != markedKMIItem)
	{
		info = markedKMIItem->GetItemData();
	}
	
	if (NULL == info)
	{
		if (doErrorProcessing)
		{
			CoveredCalcApp::GetInstance()->DoMessageBox(IDS_EMSG_GET_KEYMAPPINGS,
					MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
		}
		return NULL;
	}
	
	return info;
}

/**
 *	@brief	Enables or disables "Edit keymapping" button.
 *	@param	isEnabled	enables the control when true.
 */
void BePreferencesDlg::enableEditKeyMapping(bool isEnabled)
{
	BButton* editKeymapButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_EDIT_KEYMAPPING_BUTTON));
	if (NULL != editKeymapButton)
	{
		editKeymapButton->SetEnabled(isEnabled);
	}
}

/**
 *	@brief	Enables or disables "Duplicate keymapping" button.
 *	@param	isEnabled	enables the control when true.
 */
void BePreferencesDlg::enableDuplicateKeyMapping(bool isEnabled)
{
	BButton* dupKeymapButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_DUPLICATE_KEYMAPPING_BUTTON));
	if (NULL != dupKeymapButton)
	{
		dupKeymapButton->SetEnabled(isEnabled);
	}
}

/**
 *	@brief	Enables or disables "Delete keymapping" button.
 *	@param	isEnabled	enables the control when true.
 */
void BePreferencesDlg::enableDeleteKeyMapping(bool isEnabled)
{
	BButton* delKeymapButton = dynamic_cast<BButton*>(FindView(PREFERENCES_DIALOG_VIEW_DELETE_KEYMAPPING_BUTTON));
	if (NULL != delKeymapButton)
	{
		delKeymapButton->SetEnabled(isEnabled);
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
			processKeyMappingSelectionChanged();
			break;
		
		case ID_PREF_EDIT_KEYMAP:
			doEditKeyMapping();
			break;
			
		case ID_PREF_DUPLICATE_KEYMAP:
			doDuplicateKeyMapping();
			break;
		
		case ID_PREF_DELETE_KEYMAP:
			doDeleteKeyMapping();
			break;
		
		case ID_DIALOG_OK:
			if (saveFromDialog())
			{
				PostMessage(B_QUIT_REQUESTED);
			}
			break;
	
		case ID_DIALOG_CANCEL:
			PostMessage(B_QUIT_REQUESTED);
			break;

#if defined (ZETA)
		case B_LANGUAGE_CHANGED:
			if (NULL != langMenu && langMenu->CountItems() > 0)
			{
				BMenuItem* menuItem = langMenu->ItemAt(0);
				menuItem->SetLabel(_T(PREFERENCES_DIALOG_LANGMENU_LOCALEKIT));
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
