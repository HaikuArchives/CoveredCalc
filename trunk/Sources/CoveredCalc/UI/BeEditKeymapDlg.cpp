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
	@file		BeEditKeymapDlg.cpp
	@brief		Implementation of BeEditKeymapDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.06.01 created
*/

#include "Prefix.h"
#include "BeEditKeymapDlg.h"
#include <InterfaceKit.h>
#include "BeDialogDesign.h"
#include "CoveredCalcApp.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "CommandID.h"
#include "BeKeyInputEdit.h"
#if defined(ZETA)
#include <locale/Locale.h>
#include "BeDataStringItem.h"
#include "KeyNameDB.h"
#endif // defined(ZETA)

////////////////////////////////////////
#define baseWindow	BeDialog
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

static const char STR_EDIT_KEYBOARD[]		= "Edit Keyboard";
static const char STR_NAME[]				= "Name:";
static const char STR_FUNCTION_BOX[]		= "Function";
static const char STR_FUNCTION[]			= "Function:";
static const char STR_CURRENT_KEY[]			= "Current Key:";
static const char STR_REMOVE[]				= "Remove";
static const char STR_KEY_BOX[]				= "Key";
static const char STR_KEY[]					= "Key:";
static const char STR_ASSIGNED_FUNCTION[]	= "Assigned Function:";
static const char STR_ASSIGN[]				= "Assign";
static const char STR_CANCEL[]				= "Cancel";
static const char STR_OK[]					= "OK";

/**
 *	@brief	Constructor
 */
BeEditKeymapDlg::BeEditKeymapDlg(
	BeDialogDesign* dialogDesign	///< dialog design information
)
	: BeDialog(dialogDesign->GetFrame(BRect(0.0, 0.0, 432.0, 316.0)),
				LT(dialogDesign->GetTitle(STR_EDIT_KEYBOARD)),
				B_TITLED_WINDOW_LOOK,
				B_MODAL_SUBSET_WINDOW_FEEL,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_MINIMIZABLE)
{
	this->dialogDesign = dialogDesign;
	semDialog = 0;
	isDialogOK = false;
}

/**
 *	@brief	Destructor
 */
BeEditKeymapDlg::~BeEditKeymapDlg()
{
}

/**
 *	@brief	Creates child views.
 */
void BeEditKeymapDlg::createViews()
{
	const BeDialogDesignItem* item;

	rgb_color viewColor = { 216, 216, 216, 255 };

	int32 borderSize = 2;
	BRect frameRect;

	// BaseView
	BView* baseView = new BView(Bounds(), EDITKEYMAP_DIALOG_VIEW_BASE_VIEW,
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(baseView);
	
	baseView->SetViewColor(viewColor);
	
	// NameText
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_NAME_NAME_TEXT);
	BTextControl* nameText = new BTextControl(item->GetFrame(BRect(12.0, 12.0, 420.0, 30.0)), EDITKEYMAP_DIALOG_VIEW_NAME_NAME_TEXT,
							LT(item->GetLabel(STR_NAME)), "", NULL);
	baseView->AddChild(nameText);
	
	nameText->SetDivider(item->GetDivider(50.0));
	nameText->SetModificationMessage(new BMessage(ID_EDITKEYMAP_NAME_MODIFIED));
	uicNameTextEdit.Init(nameText);
	
	// FunctionBox
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_FUNCTION_BOX);
	BBox* functionBox = new BBox(item->GetFrame(BRect(12.0, 42.0, 164.0, 268.0)), EDITKEYMAP_DIALOG_VIEW_FUNCTION_BOX);
	baseView->AddChild(functionBox);
	
	functionBox->SetLabel(LT(item->GetLabel(STR_FUNCTION_BOX)));
	
	// FunctionLabel
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_FUNCTION_LABEL);
	BStringView* functionLabel = new BStringView(item->GetFrame(BRect(12.0, 16.0, 140.0, 28.0)), EDITKEYMAP_DIALOG_VIEW_FUNCTION_LABEL,
							LT(item->GetLabel(STR_FUNCTION)));
	functionBox->AddChild(functionLabel);
	
	// FunctionList
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_FUNCTION_LIST);
	frameRect = item->GetFrame(BRect(12.0, 31.0, 140.0, 127.0));
	frameRect.OffsetBy(borderSize, borderSize);
	frameRect.right = frameRect.right - (B_V_SCROLL_BAR_WIDTH + borderSize * 2);
	frameRect.bottom = frameRect.bottom - (/* B_H_SCROLL_BAR_HEIGHT + */ borderSize * 2);
	BListView* functionList = new BListView(frameRect, EDITKEYMAP_DIALOG_VIEW_FUNCTION_LIST);
	BScrollView* functionListScroller = new BScrollView(EDITKEYMAP_DIALOG_VIEW_FUNCTION_SCROLLER, functionList,
							B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true, B_FANCY_BORDER);
	functionListScroller->SetViewColor(viewColor);
	functionBox->AddChild(functionListScroller);
	functionList->SetSelectionMessage(new BMessage(ID_EDITKEYMAP_FUNCTION_SELECTED));
	uicFunctionListBox.Init(functionList);
	
	// CurrentKeyLabel
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LABEL);
	BStringView* currentKeyLabel = new BStringView(item->GetFrame(BRect(12.0, 139.0, 140.0, 151.0)),
							EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LABEL, LT(item->GetLabel(STR_CURRENT_KEY)));
	functionBox->AddChild(currentKeyLabel);
	
	// CurrentKeyList
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LIST);
	frameRect = item->GetFrame(BRect(12.0, 154.0, 140.0, 214.0));
	frameRect.OffsetBy(borderSize, borderSize);
	frameRect.right = frameRect.right - (B_V_SCROLL_BAR_WIDTH + borderSize * 2);
	frameRect.bottom = frameRect.bottom - (/* B_H_SCROLL_BAR_HEIGHT + */ borderSize * 2);
	BListView* currentKeyList = new BListView(frameRect, EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LIST);
	BScrollView* currentKeyListScroller = new BScrollView(EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_SCROLLER, currentKeyList,
							B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true, B_FANCY_BORDER);
	currentKeyListScroller->SetViewColor(viewColor);
	functionBox->AddChild(currentKeyListScroller);
	currentKeyList->SetSelectionMessage(new BMessage(ID_EDITKEYMAP_CURRENT_KEY_SELECTED));
	uicCurrentKeyListBox.Init(currentKeyList);
	
	// RemoveButton
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_REMOVE_BUTTON);
	BButton* removeButton = new BButton(item->GetFrame(BRect(176.0, 196.0, 256.0, 220.0)), EDITKEYMAP_DIALOG_VIEW_REMOVE_BUTTON,
							LT(item->GetLabel(STR_REMOVE)), new BMessage(ID_EDITKEYMAP_REMOVE));
	baseView->AddChild(removeButton);
	uicRemoveButton.Init(removeButton);
	
	// KeyBox
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_KEY_BOX);
	BBox* keyBox = new BBox(item->GetFrame(BRect(268.0, 42.0, 420.0, 268.0)), EDITKEYMAP_DIALOG_VIEW_KEY_BOX);
	baseView->AddChild(keyBox);
	
	keyBox->SetLabel(LT(item->GetLabel(STR_KEY_BOX)));
	
	// KeyLabel
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_KEY_LABEL);
	BStringView* keyLabel = new BStringView(item->GetFrame(BRect(12.0, 16.0, 140.0, 28.0)),
							EDITKEYMAP_DIALOG_VIEW_KEY_LABEL, LT(item->GetLabel(STR_KEY)));
	keyBox->AddChild(keyLabel);
	
	// KeyInput
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_KEY_INPUT);
	BeKeyInputEdit* keyInput = new BeKeyInputEdit(item->GetFrame(BRect(10.0, 31.0, 140.0, 50.0)), EDITKEYMAP_DIALOG_VIEW_KEY_INPUT,
							"", "", NULL);
	keyBox->AddChild(keyInput);
	
	keyInput->Init(keyNameDB);
	keyInput->SetDivider(0.0);
	keyInput->SetValueChangeMessage(new BMessage(ID_EDITKEYMAP_KEYINPUT_CHANGED));
	uicKeyInput.Init(keyInput);
	
	// AssignedFuncLabel
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_LABEL);
	BStringView* assignedFuncLabel = new BStringView(item->GetFrame(BRect(12.0, 62.0, 140.0, 74.0)),
							EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_LABEL, LT(item->GetLabel(STR_ASSIGNED_FUNCTION)));
	keyBox->AddChild(assignedFuncLabel);
	
	// AssignedFuncText
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_TEXT);
	BTextControl* assignedFuncText = new BTextControl(item->GetFrame(BRect(10.0, 77.0, 140.0, 96.0)), EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_TEXT,
							"", "", NULL);
	keyBox->AddChild(assignedFuncText);
	
	assignedFuncText->SetDivider(0.0);
	assignedFuncText->TextView()->MakeEditable(false);
	uicAssignedFunctionTextEdit.Init(assignedFuncText);
	
	// AssignButton
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_ASSIGN_BUTTON);
	BButton* assignButton = new BButton(item->GetFrame(BRect(176.0, 73.0, 256.0, 97.0)), EDITKEYMAP_DIALOG_VIEW_ASSIGN_BUTTON,
							LT(item->GetLabel(STR_ASSIGN)), new BMessage(ID_EDITKEYMAP_ASSIGN));
	baseView->AddChild(assignButton);
	uicAssignButton.Init(assignButton);
	
	// CancelButton
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_CANCEL);
	BButton* cancelButton = new BButton(item->GetFrame(BRect(254.0, 280.0, 334.0, 304.0)), EDITKEYMAP_DIALOG_VIEW_CANCEL,
								LT(item->GetLabel(STR_CANCEL)), new BMessage(ID_DIALOG_CANCEL));
	baseView->AddChild(cancelButton);
	uicCancelButton.Init(cancelButton);

	// OKButton
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_OK);
	BButton* okButton = new BButton(item->GetFrame(BRect(340.0, 280.0, 420.0, 304.0)), EDITKEYMAP_DIALOG_VIEW_OK,
								LT(item->GetLabel(STR_OK)), new BMessage(ID_DIALOG_OK));
	baseView->AddChild(okButton);
	
	SetDefaultButton(okButton);
	uicOkButton.Init(okButton);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed by LocaleKit.
 */
void BeEditKeymapDlg::languageChanged()
{
	UIMessageProvider* messageProvider = CoveredCalcApp::GetInstance()->GetMessageProvider();
	const BeDialogDesignItem* item;
	
	// dialog title
	SetTitle(LT(dialogDesign->GetTitle(STR_EDIT_KEYBOARD)));
	
	// NameText
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_NAME_NAME_TEXT);
	BTextControl* nameText = dynamic_cast<BTextControl*>(FindView(EDITKEYMAP_DIALOG_VIEW_NAME_NAME_TEXT));
	if (NULL != nameText)
	{
		nameText->SetLabel(LT(item->GetLabel(STR_NAME)));
	}
	
	// FunctionBox
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_FUNCTION_BOX);
	BBox* functionBox = dynamic_cast<BBox*>(FindView(EDITKEYMAP_DIALOG_VIEW_FUNCTION_BOX));
	if (NULL != functionBox)
	{
		functionBox->SetLabel(LT(item->GetLabel(STR_FUNCTION_BOX)));
	}

	// FunctionLabel
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_FUNCTION_LABEL);
	BStringView* functionLabel = dynamic_cast<BStringView*>(FindView(EDITKEYMAP_DIALOG_VIEW_FUNCTION_LABEL));
	if (NULL != functionLabel)
	{
		functionLabel->SetText(LT(item->GetLabel(STR_FUNCTION)));
	}
	
	// FunctionList
	BListView* functionList = dynamic_cast<BListView*>(FindView(EDITKEYMAP_DIALOG_VIEW_FUNCTION_LIST));
	if (NULL != functionList)
	{
		BeListViewAdapter* functionListAdapter = uicFunctionListBox.GetRawAdapter();
		if (NULL != functionListAdapter)
		{
			SInt32 count = functionListAdapter->GetCount();
			int32 ix;
			for (ix = 0; ix < count; ix++)
			{
				const FuncInfo* funcInfo = static_cast<FuncInfo*>(functionListAdapter->GetItemData(ix));
				MBCString funcName;
				messageProvider->GetMessage(funcInfo->funcNameId, funcName);
				functionListAdapter->SetItemText(ix, funcName.CString());
			}
		}
		functionList->Invalidate();
	}
	
	// CurrentKeyLabel
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LABEL);
	BStringView* currentKeyLabel = dynamic_cast<BStringView*>(FindView(EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LABEL));
	if (NULL != currentKeyLabel)
	{
		currentKeyLabel->SetText(LT(item->GetLabel(STR_CURRENT_KEY)));
	}

	// CurrentKeyList
	BListView* currentKeyList = dynamic_cast<BListView*>(FindView(EDITKEYMAP_DIALOG_VIEW_CURRENT_KEY_LIST));
	if (NULL != currentKeyList)
	{
		BeListViewAdapter* currentKeyListAdapter = uicCurrentKeyListBox.GetRawAdapter();
		if (NULL != currentKeyListAdapter)
		{
			SInt32 count = currentKeyListAdapter->GetCount();
			int32 ix;
			for (ix = 0; ix < count; ix++)
			{
				const KeyEventParameter* keyParam = static_cast<KeyEventParameter*>(currentKeyListAdapter->GetItemData(ix));
				MBCString keyName;
				keyNameDB->GetKeyName(keyParam, keyName);
				currentKeyListAdapter->SetItemText(ix, keyName.CString());
			}
		}
		currentKeyList->Invalidate();
	}
	
	// RemoveButton
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_REMOVE_BUTTON);
	BButton* removeButton = dynamic_cast<BButton*>(FindView(EDITKEYMAP_DIALOG_VIEW_REMOVE_BUTTON));
	if (NULL != removeButton)
	{
		removeButton->SetLabel(LT(item->GetLabel(STR_REMOVE)));
	}
	
	// KeyBox
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_KEY_BOX);
	BBox* keyBox = dynamic_cast<BBox*>(FindView(EDITKEYMAP_DIALOG_VIEW_KEY_BOX));
	if (NULL != keyBox)
	{
		keyBox->SetLabel(LT(item->GetLabel(STR_KEY_BOX)));
	}
	
	// KeyLabel
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_KEY_LABEL);
	BStringView* keyLabel = dynamic_cast<BStringView*>(FindView(EDITKEYMAP_DIALOG_VIEW_KEY_LABEL));
	if (NULL != keyLabel)
	{
		keyLabel->SetText(LT(item->GetLabel(STR_KEY)));
	}
	
	// AssignedFuncLabel
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_LABEL);
	BStringView* assignedFuncLabel = dynamic_cast<BStringView*>(FindView(EDITKEYMAP_DIALOG_VIEW_ASSIGNED_FUNC_LABEL));
	if (NULL != assignedFuncLabel)
	{
		assignedFuncLabel->SetText(LT(item->GetLabel(STR_ASSIGNED_FUNCTION)));
	}

	// AssignedFuncText
	processKeyInputChanged();
	
	// AssignButton
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_ASSIGN_BUTTON);
	BButton* assignButton = dynamic_cast<BButton*>(FindView(EDITKEYMAP_DIALOG_VIEW_ASSIGN_BUTTON));
	if (NULL != assignButton)
	{
		assignButton->SetLabel(LT(item->GetLabel(STR_ASSIGN)));
	}
	
	// CancelButton
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_CANCEL);
	BButton* cancelButton = dynamic_cast<BButton*>(FindView(EDITKEYMAP_DIALOG_VIEW_CANCEL));
	if (NULL != cancelButton)
	{
		cancelButton->SetLabel(LT(item->GetLabel(STR_CANCEL)));
	}
	
	// OKButton
	item = dialogDesign->FindItem(EDITKEYMAP_DIALOG_VIEW_OK);
	BButton* okButton = dynamic_cast<BButton*>(FindView(EDITKEYMAP_DIALOG_VIEW_OK));
	if (NULL != okButton)
	{
		okButton->SetLabel(LT(item->GetLabel(STR_OK)));
	}
}
#endif 

/**
 *	@brief	Initializes the object.
 *	@param[in]	parent		parent window, which is blocked while this dialog is shown.
 *	@param[in]	semDialog	this semaphore is released when dialog is closed.
 *	@param[in]	isReadOnly	true when read-only mode.
 *	@param[in]	keyNameDB
 */
void BeEditKeymapDlg::Init(BWindow* parent, sem_id semDialog, bool isReadOnly, const KeyNameDB* keyNameDB)
{
	base::Init(isReadOnly, keyNameDB);
	
	this->semDialog = semDialog;

	AddToSubset(parent);
	
	createViews();
	baseWindow::Init();
	moveToCenterOfScreen();
	
	readyToShow();
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
 *	@brief	Message handler.
 */
void BeEditKeymapDlg::MessageReceived(
	BMessage *message		//!< received message.
)
{
	try
	{
		switch (message->what)
		{
		case ID_DIALOG_OK:
			if (processOK())
			{
				isDialogOK = true;
				PostMessage(B_QUIT_REQUESTED);
			}
			break;
			
		case ID_DIALOG_CANCEL:
			if (processCancel())
			{
				isDialogOK = false;
				PostMessage(B_QUIT_REQUESTED);
			}
			break;
		
		case ID_EDITKEYMAP_FUNCTION_SELECTED:
			processFunctionListBoxSelectionChanged();
			break;
		
		case ID_EDITKEYMAP_CURRENT_KEY_SELECTED:
			processCurrentKeysListBoxSelectionChanged();
			break;
		
		case ID_EDITKEYMAP_KEYINPUT_CHANGED:
			processKeyInputChanged();
			break;
		
		case ID_EDITKEYMAP_ASSIGN:
			processAssignButtonClicked();
			break;
		
		case ID_EDITKEYMAP_REMOVE:
			processRemoveButtonClicked();
			break;
		
		case ID_EDITKEYMAP_NAME_MODIFIED:
			processNameTextEditChanged();
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

/**
 *	@brief	Called when B_QUIT_REQUESTED message is received.
 */
bool BeEditKeymapDlg::QuitRequested()
{
	if (semDialog >= 0)
	{
		release_sem(semDialog);
	}
	return baseWindow::QuitRequested();
}
