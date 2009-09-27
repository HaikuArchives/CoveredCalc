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
	@file		BeEditKeymapDlg.cpp
	@brief		Implementation of BeEditKeymapDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.06.01 created
*/

#include "Prefix.h"
#include "BeEditKeymapDlg.h"
#include <InterfaceKit.h>
#include "CoveredCalcApp.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "DialogID.h"
#include "StringID.h"
#include "CommandID.h"
#include "BeKeyInputEdit.h"
#include "BeDialogControlHelper.h"
#include "UICEventCode.h"
#if defined(ZETA)
#include <locale/Locale.h>
#include "KeyNameDB.h"
#endif // defined(ZETA)

////////////////////////////////////////
#define baseWindow	BeModalDialogWindow
#define base		EditKeymapDlg
////////////////////////////////////////

static const char EDITKEYMAP_DIALOG_VIEW_BASE_VIEW[]			= "BaseView";
static const char EDITKEYMAP_DIALOG_VIEW_NAME_NAME_TEXT[]		= "NameText";
static const char EDITKEYMAP_DIALOG_VIEW_FUNCTION_BOX[]			= "FunctionBox";
static const char EDITKEYMAP_DIALOG_VIEW_FUNCTION_LABEL[]		= "FunctionLabel";
static const char EDITKEYMAP_DIALOG_VIEW_FUNCTION_LIST[]		= "FunctionList";
static const char EDITKEYMAP_DIALOG_VIEW_FUNCTION_SCROLLER[]	= "FunctionScroller";
static const char EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LABEL[]	= "CurrentKeyLabel";
static const char EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LIST[]		= "CurrentKeyList";
static const char EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_SCROLLER[]	= "CurrentKeyScroller";
static const char EDITKEYMAP_DIALOG_VIEW_ASSIGN_BUTTON[]		= "AssignButton";
static const char EDITKEYMAP_DIALOG_VIEW_REMOVE_BUTTON[]		= "RemoveButton";
static const char EDITKEYMAP_DIALOG_VIEW_KEY_BOX[]				= "KeyBox";
static const char EDITKEYMAP_DIALOG_VIEW_KEY_LABEL[]			= "KeyLabel";
static const char EDITKEYMAP_DIALOG_VIEW_KEY_INPUT[]			= "KeyInput";
static const char EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_LABEL[]	= "AssignedFuncLabel";
static const char EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_TEXT[]	= "AssignedFuncText";
static const char EDITKEYMAP_DIALOG_VIEW_OK[]					= "OKButton";
static const char EDITKEYMAP_DIALOG_VIEW_CANCEL[]				= "CancelButton";

/**
 *	@brief	Constructor
 */
BeEditKeymapDlg::BeEditKeymapDlg()
{
}

/**
 *	@brief	Destructor
 */
BeEditKeymapDlg::~BeEditKeymapDlg()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	isReadOnly	true when read-only mode.
 *	@param[in]	keyNameDB
 */
void BeEditKeymapDlg::Init(bool isReadOnly, const KeyNameDB* keyNameDB)
{
	base::Init(isReadOnly, keyNameDB);

	DialogWindow* dlg = new DialogWindow();
	dialogWindow = dlg;
	dlg->Init(this);
}

/**
 *	@brief Returns semaphore name
 *	@return semaphore name
 */
ConstAStr BeEditKeymapDlg::getSemaphoreName()
{
	return "BeEditKeymapDlg";
}

/**
 *	@brief	Returns MessageBoxProvider object.
 *	@return	MessageBoxProvider object.
 */
MessageBoxProvider*	BeEditKeymapDlg::getMessageBoxProvider()
{
	return CoveredCalcApp::GetInstance();
}

/**
 *	@brief	Close this dialog.
 *	@param[in]	isOK	true if dialog is closing by OK button.
 */
void BeEditKeymapDlg::closeDialog(bool isOK)
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
BeEditKeymapDlg::DialogWindow::DialogWindow()
	: baseWindow(IDD_EDIT_KEYMAP,
				B_TITLED_WINDOW_LOOK,
				B_MODAL_SUBSET_WINDOW_FEEL,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_MINIMIZABLE)
{
}

/**
 * @brief Destructor
 */
BeEditKeymapDlg::DialogWindow::~DialogWindow()
{
}

/**
 *	@brief	Creates child views.
 */
void BeEditKeymapDlg::DialogWindow::createViews()
{
	BeDialogControlHelper dch(getDialogLayout());
	NativeStringLoader* nsl = CoveredCalcApp::GetInstance();

	rgb_color viewColor = { 216, 216, 216, 255 };

	int32 borderSize = 2;
	BRect frameRect;

	// dialog title
	SetTitle(nsl->LoadNativeString(NSID_EDIT_KEYMAP_TITLE).CString());

	// BaseView
	BView* baseView = new BView(Bounds(), EDITKEYMAP_DIALOG_VIEW_BASE_VIEW,
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(baseView);
	
	baseView->SetViewColor(viewColor);
	
	// NameText
	BTextControl* nameText = new BTextControl(dch.GetItemRect(ALITERAL("IDC_EDIT_NAME"), ITEMNAME_WINDOW), EDITKEYMAP_DIALOG_VIEW_NAME_NAME_TEXT,
							nsl->LoadNativeString(NSID_EDIT_KEYMAP_NAME).CString(), "", NULL);
	baseView->AddChild(nameText);
	
	nameText->SetDivider(dch.GetItemPos(true, ALITERAL("IDC_EDIT_NAME.divider"), ALITERAL("IDC_EDIT_NAME.left")));
	nameText->SetModificationMessage(new BMessage(ID_EDITKEYMAP_NAME_MODIFIED));
	owner->uicNameTextEdit.Init(nameText);
	
	// FunctionBox
	MBCString itemnameFunctionBox = ALITERAL("IDC_GROUP_FUNCTION");
	BBox* functionBox = new BBox(dch.GetItemRect(itemnameFunctionBox, ITEMNAME_WINDOW), EDITKEYMAP_DIALOG_VIEW_FUNCTION_BOX);
	baseView->AddChild(functionBox);
	
	functionBox->SetLabel(nsl->LoadNativeString(NSID_EDIT_KEYMAP_GROUP_FUNCTION));
	
	// FunctionLabel
	BStringView* functionLabel = new BStringView(dch.GetItemRect(ALITERAL("IDC_STATIC_FUNCTION"), itemnameFunctionBox), EDITKEYMAP_DIALOG_VIEW_FUNCTION_LABEL,
							nsl->LoadNativeString(NSID_EDIT_KEYMAP_FUNCTION));
	functionBox->AddChild(functionLabel);
	
	// FunctionList
	frameRect = dch.GetItemRect(ALITERAL("IDC_LIST_FUNCTION"), itemnameFunctionBox);
	frameRect.OffsetBy(borderSize, borderSize);
	frameRect.right = frameRect.right - (B_V_SCROLL_BAR_WIDTH + borderSize * 2);
	frameRect.bottom = frameRect.bottom - (/* B_H_SCROLL_BAR_HEIGHT + */ borderSize * 2);
	BListView* functionList = new BListView(frameRect, EDITKEYMAP_DIALOG_VIEW_FUNCTION_LIST);
	BScrollView* functionListScroller = new BScrollView(EDITKEYMAP_DIALOG_VIEW_FUNCTION_SCROLLER, functionList,
							B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true, B_FANCY_BORDER);
	functionListScroller->SetViewColor(viewColor);
	functionBox->AddChild(functionListScroller);
	functionList->SetSelectionMessage(new BMessage(ID_EDITKEYMAP_FUNCTION_SELECTED));
	owner->uicFunctionListBox.Init(functionList);
	
	// CurrentKeyLabel
	BStringView* currentKeyLabel = new BStringView(dch.GetItemRect(ALITERAL("IDC_STATIC_CURRENT_KEY"), itemnameFunctionBox),
							EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LABEL, nsl->LoadNativeString(NSID_EDIT_KEYMAP_CURRENT_KEY));
	functionBox->AddChild(currentKeyLabel);
	
	// CurrentKeyList
	frameRect = dch.GetItemRect(ALITERAL("IDC_LIST_CURRENT_KEY"), itemnameFunctionBox);
	frameRect.OffsetBy(borderSize, borderSize);
	frameRect.right = frameRect.right - (B_V_SCROLL_BAR_WIDTH + borderSize * 2);
	frameRect.bottom = frameRect.bottom - (/* B_H_SCROLL_BAR_HEIGHT + */ borderSize * 2);
	BListView* currentKeyList = new BListView(frameRect, EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LIST);
	BScrollView* currentKeyListScroller = new BScrollView(EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_SCROLLER, currentKeyList,
							B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true, B_FANCY_BORDER);
	currentKeyListScroller->SetViewColor(viewColor);
	functionBox->AddChild(currentKeyListScroller);
	currentKeyList->SetSelectionMessage(new BMessage(ID_EDITKEYMAP_CURRENT_KEY_SELECTED));
	owner->uicCurrentKeyListBox.Init(currentKeyList);
	
	// RemoveButton
	BButton* removeButton = new BButton(dch.GetItemRect(ALITERAL("IDC_REMOVE"), ITEMNAME_WINDOW), EDITKEYMAP_DIALOG_VIEW_REMOVE_BUTTON,
							nsl->LoadNativeString(NSID_EDIT_KEYMAP_REMOVE), new BMessage(ID_EDITKEYMAP_REMOVE));
	baseView->AddChild(removeButton);
	owner->uicRemoveButton.Init(removeButton);
	
	// KeyBox
	MBCString itemnameKeyBox = ALITERAL("IDC_GROUP_KEY");
	BBox* keyBox = new BBox(dch.GetItemRect(itemnameKeyBox, ITEMNAME_WINDOW), EDITKEYMAP_DIALOG_VIEW_KEY_BOX);
	baseView->AddChild(keyBox);
	
	keyBox->SetLabel(nsl->LoadNativeString(NSID_EDIT_KEYMAP_GROUP_KEY));
	
	// KeyLabel
	BStringView* keyLabel = new BStringView(dch.GetItemRect(ALITERAL("IDC_STATIC_KEY"), itemnameKeyBox),
							EDITKEYMAP_DIALOG_VIEW_KEY_LABEL, nsl->LoadNativeString(NSID_EDIT_KEYMAP_KEY));
	keyBox->AddChild(keyLabel);
	
	// KeyInput
	BeKeyInputEdit* keyInput = new BeKeyInputEdit(dch.GetItemRect(ALITERAL("IDC_EDIT_KEY"), itemnameKeyBox), EDITKEYMAP_DIALOG_VIEW_KEY_INPUT,
							"", "", NULL);
	keyBox->AddChild(keyInput);
	
	keyInput->Init(owner->keyNameDB);
	keyInput->SetDivider(0.0);
	keyInput->SetValueChangeMessage(new BMessage(ID_EDITKEYMAP_KEYINPUT_CHANGED));
	owner->uicKeyInput.Init(keyInput);
	
	// AssignedFuncLabel
	BStringView* assignedFuncLabel = new BStringView(dch.GetItemRect(ALITERAL("IDC_STATIC_ASSIGNED_FUNCTION"), itemnameKeyBox),
							EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_LABEL, nsl->LoadNativeString(NSID_EDIT_KEYMAP_ASSIGNED_FUNCTION));
	keyBox->AddChild(assignedFuncLabel);
	
	// AssignedFuncText
	BTextControl* assignedFuncText = new BTextControl(dch.GetItemRect(ALITERAL("IDC_EDIT_ASSIGNED_FUNCTION"), itemnameKeyBox), EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_TEXT,
							"", "", NULL);
	keyBox->AddChild(assignedFuncText);
	
	assignedFuncText->SetDivider(0.0);
	assignedFuncText->TextView()->MakeEditable(false);
	owner->uicAssignedFunctionTextEdit.Init(assignedFuncText);
	
	// AssignButton
	BButton* assignButton = new BButton(dch.GetItemRect(ALITERAL("IDC_ASSIGN"), ITEMNAME_WINDOW), EDITKEYMAP_DIALOG_VIEW_ASSIGN_BUTTON,
							nsl->LoadNativeString(NSID_EDIT_KEYMAP_ASSIGN), new BMessage(ID_EDITKEYMAP_ASSIGN));
	baseView->AddChild(assignButton);
	owner->uicAssignButton.Init(assignButton);
	
	// CancelButton
	BButton* cancelButton = new BButton(dch.GetItemRect(ALITERAL("IDCANCEL"), ITEMNAME_WINDOW), EDITKEYMAP_DIALOG_VIEW_CANCEL,
								nsl->LoadNativeString(NSID_EDIT_KEYMAP_CANCEL), new BMessage(ID_DIALOG_CANCEL));
	baseView->AddChild(cancelButton);
	owner->uicCancelButton.Init(cancelButton);

	// OKButton
	BButton* okButton = new BButton(dch.GetItemRect(ALITERAL("IDOK"), ITEMNAME_WINDOW), EDITKEYMAP_DIALOG_VIEW_OK,
								nsl->LoadNativeString(NSID_EDIT_KEYMAP_OK), new BMessage(ID_DIALOG_OK));
	baseView->AddChild(okButton);
	
	SetDefaultButton(okButton);
	owner->uicOkButton.Init(okButton);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed by LocaleKit.
 */
void BeEditKeymapDlg::DialogWindow::languageChanged()
{
	NativeStringLoader* nsl = CoveredCalcApp::GetInstance();

	// dialog title
	SetTitle(nsl->LoadNativeString(NSID_EDIT_KEYMAP_TITLE).CString());
	
	// NameText
	BTextControl* nameText = dynamic_cast<BTextControl*>(FindView(EDITKEYMAP_DIALOG_VIEW_NAME_NAME_TEXT));
	if (NULL != nameText)
	{
		nameText->SetLabel(nsl->LoadNativeString(NSID_EDIT_KEYMAP_NAME));
	}
	
	// FunctionBox
	BBox* functionBox = dynamic_cast<BBox*>(FindView(EDITKEYMAP_DIALOG_VIEW_FUNCTION_BOX));
	if (NULL != functionBox)
	{
		functionBox->SetLabel(nsl->LoadNativeString(NSID_EDIT_KEYMAP_GROUP_FUNCTION));
	}

	// FunctionLabel
	BStringView* functionLabel = dynamic_cast<BStringView*>(FindView(EDITKEYMAP_DIALOG_VIEW_FUNCTION_LABEL));
	if (NULL != functionLabel)
	{
		functionLabel->SetText(nsl->LoadNativeString(NSID_EDIT_KEYMAP_FUNCTION));
	}
	
	// FunctionList
	BListView* functionList = dynamic_cast<BListView*>(FindView(EDITKEYMAP_DIALOG_VIEW_FUNCTION_LIST));
	if (NULL != functionList)
	{
		BeListViewAdapter* functionListAdapter = owner->uicFunctionListBox.GetRawAdapter();
		if (NULL != functionListAdapter)
		{
			SInt32 count = functionListAdapter->GetCount();
			int32 ix;
			for (ix = 0; ix < count; ix++)
			{
				const FuncInfo* funcInfo = static_cast<FuncInfo*>(functionListAdapter->GetItemData(ix));
				functionListAdapter->SetItemText(ix, nsl->LoadNativeString(funcInfo->funcNameId));
			}
		}
		functionList->Invalidate();
	}
	
	// CurrentKeyLabel
	BStringView* currentKeyLabel = dynamic_cast<BStringView*>(FindView(EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LABEL));
	if (NULL != currentKeyLabel)
	{
		currentKeyLabel->SetText(nsl->LoadNativeString(NSID_EDIT_KEYMAP_CURRENT_KEY));
	}

	// CurrentKeyList
	BListView* currentKeyList = dynamic_cast<BListView*>(FindView(EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LIST));
	if (NULL != currentKeyList)
	{
		BeListViewAdapter* currentKeyListAdapter = owner->uicCurrentKeyListBox.GetRawAdapter();
		if (NULL != currentKeyListAdapter)
		{
			SInt32 count = currentKeyListAdapter->GetCount();
			int32 ix;
			for (ix = 0; ix < count; ix++)
			{
				const KeyEventParameter* keyParam = static_cast<KeyEventParameter*>(currentKeyListAdapter->GetItemData(ix));
				MBCString keyName;
				owner->keyNameDB->GetKeyName(keyParam, keyName);
				currentKeyListAdapter->SetItemText(ix, keyName);
			}
		}
		currentKeyList->Invalidate();
	}
	
	// RemoveButton
	BButton* removeButton = dynamic_cast<BButton*>(FindView(EDITKEYMAP_DIALOG_VIEW_REMOVE_BUTTON));
	if (NULL != removeButton)
	{
		removeButton->SetLabel(nsl->LoadNativeString(NSID_EDIT_KEYMAP_REMOVE));
	}
	
	// KeyBox
	BBox* keyBox = dynamic_cast<BBox*>(FindView(EDITKEYMAP_DIALOG_VIEW_KEY_BOX));
	if (NULL != keyBox)
	{
		keyBox->SetLabel(nsl->LoadNativeString(NSID_EDIT_KEYMAP_GROUP_KEY));
	}
	
	// KeyLabel
	BStringView* keyLabel = dynamic_cast<BStringView*>(FindView(EDITKEYMAP_DIALOG_VIEW_KEY_LABEL));
	if (NULL != keyLabel)
	{
		keyLabel->SetText(nsl->LoadNativeString(NSID_EDIT_KEYMAP_KEY));
	}
	
	// AssignedFuncLabel
	BStringView* assignedFuncLabel = dynamic_cast<BStringView*>(FindView(EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_LABEL));
	if (NULL != assignedFuncLabel)
	{
		assignedFuncLabel->SetText(nsl->LoadNativeString(NSID_EDIT_KEYMAP_ASSIGNED_FUNCTION));
	}

	// AssignedFuncText
	owner->HandleUICEvent(CID_KeyInput, UICE_TextChanged, 0, NULL);
	
	// AssignButton
	BButton* assignButton = dynamic_cast<BButton*>(FindView(EDITKEYMAP_DIALOG_VIEW_ASSIGN_BUTTON));
	if (NULL != assignButton)
	{
		assignButton->SetLabel(nsl->LoadNativeString(NSID_EDIT_KEYMAP_ASSIGN));
	}
	
	// CancelButton
	BButton* cancelButton = dynamic_cast<BButton*>(FindView(EDITKEYMAP_DIALOG_VIEW_CANCEL));
	if (NULL != cancelButton)
	{
		cancelButton->SetLabel(nsl->LoadNativeString(NSID_EDIT_KEYMAP_CANCEL));
	}
	
	// OKButton
	BButton* okButton = dynamic_cast<BButton*>(FindView(EDITKEYMAP_DIALOG_VIEW_OK));
	if (NULL != okButton)
	{
		okButton->SetLabel(nsl->LoadNativeString(NSID_EDIT_KEYMAP_OK));
	}
}
#endif 

/**
 *	@brief	Initializes the object.
 *	@param[in]	owner	BeEditKeymapDlg object
 */
void BeEditKeymapDlg::DialogWindow::Init(BeEditKeymapDlg* owner)
{
	this->owner = owner;
	baseWindow::Init();
}

/**
 *	@brief	Initializes the dialog.
 */
void BeEditKeymapDlg::DialogWindow::initDialog()
{	
	createViews();
	moveToCenterOfScreen();

	owner->readyToShow();
}

/**
 *	@brief	Message handler.
 */
void BeEditKeymapDlg::DialogWindow::MessageReceived(
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
		
		case ID_EDITKEYMAP_FUNCTION_SELECTED:
			owner->HandleUICEvent(CID_FunctionListBox, UICE_SelectionChanged, 0, NULL);
			break;
		
		case ID_EDITKEYMAP_CURRENT_KEY_SELECTED:
			owner->HandleUICEvent(CID_CurrentKeysListBox, UICE_SelectionChanged, 0, NULL);
			break;
		
		case ID_EDITKEYMAP_KEYINPUT_CHANGED:
			owner->HandleUICEvent(CID_KeyInput, UICE_TextChanged, 0, NULL);
			break;
		
		case ID_EDITKEYMAP_ASSIGN:
			owner->HandleUICEvent(CID_AssignButton, UICE_ButtonClicked, 0, NULL);
			break;
		
		case ID_EDITKEYMAP_REMOVE:
			owner->HandleUICEvent(CID_RemoveButton, UICE_ButtonClicked, 0, NULL);
			break;
		
		case ID_EDITKEYMAP_NAME_MODIFIED:
			owner->HandleUICEvent(CID_NameTextEdit, UICE_TextChanged, 0, NULL);
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
