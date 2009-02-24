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
	@file		EditKeymapDlg.cpp
	@brief		Implementation of EditKeymapDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.5.5 created
*/

#include "Prefix.h"
#include "EditKeymapDlg.h"
#include "MainWindowKeyFunc.h"
#include "MessageDef.h"
#include "KeyMappingManager.h"
#include "CoveredCalcApp.h"
#include "KeyNameDB.h"
#include "KeyMappings.h"
#include "UTF8Conv.h"
#include "UICTextEdit.h"
#include "UICListBox.h"
#include "UICButton.h"
#include "UICKeyInput.h"
#include "StringID.h"
#include "UICEventCode.h"

EditKeymapDlg::FuncInfo EditKeymapDlg::funcInfo[] =
{
	{	MainWindowKeyFunc::KeyFunc_Clear,					NSID_MAIN_WINDOW_KEYFUNC_CLEAR		},
	{	MainWindowKeyFunc::KeyFunc_BS,						NSID_MAIN_WINDOW_KEYFUNC_BS			},
	{	MainWindowKeyFunc::KeyFunc_0,						NSID_MAIN_WINDOW_KEYFUNC_0			},
	{	MainWindowKeyFunc::KeyFunc_1,						NSID_MAIN_WINDOW_KEYFUNC_1			},
	{	MainWindowKeyFunc::KeyFunc_2,						NSID_MAIN_WINDOW_KEYFUNC_2			},
	{	MainWindowKeyFunc::KeyFunc_3,						NSID_MAIN_WINDOW_KEYFUNC_3			},
	{	MainWindowKeyFunc::KeyFunc_4,						NSID_MAIN_WINDOW_KEYFUNC_4			},
	{	MainWindowKeyFunc::KeyFunc_5,						NSID_MAIN_WINDOW_KEYFUNC_5			},
	{	MainWindowKeyFunc::KeyFunc_6,						NSID_MAIN_WINDOW_KEYFUNC_6			},
	{	MainWindowKeyFunc::KeyFunc_7,						NSID_MAIN_WINDOW_KEYFUNC_7			},
	{	MainWindowKeyFunc::KeyFunc_8,						NSID_MAIN_WINDOW_KEYFUNC_8			},
	{	MainWindowKeyFunc::KeyFunc_9,						NSID_MAIN_WINDOW_KEYFUNC_9			},
	{	MainWindowKeyFunc::KeyFunc_A,						NSID_MAIN_WINDOW_KEYFUNC_A			},
	{	MainWindowKeyFunc::KeyFunc_B,						NSID_MAIN_WINDOW_KEYFUNC_B			},
	{	MainWindowKeyFunc::KeyFunc_C,						NSID_MAIN_WINDOW_KEYFUNC_C			},
	{	MainWindowKeyFunc::KeyFunc_D,						NSID_MAIN_WINDOW_KEYFUNC_D			},
	{	MainWindowKeyFunc::KeyFunc_E,						NSID_MAIN_WINDOW_KEYFUNC_E			},
	{	MainWindowKeyFunc::KeyFunc_F,						NSID_MAIN_WINDOW_KEYFUNC_F			},
	{	MainWindowKeyFunc::KeyFunc_Plus,					NSID_MAIN_WINDOW_KEYFUNC_PLUS		},
	{	MainWindowKeyFunc::KeyFunc_Minus,					NSID_MAIN_WINDOW_KEYFUNC_MINUS		},
	{	MainWindowKeyFunc::KeyFunc_Times,					NSID_MAIN_WINDOW_KEYFUNC_TIMES		},
	{	MainWindowKeyFunc::KeyFunc_Div,						NSID_MAIN_WINDOW_KEYFUNC_DIV			},
	{	MainWindowKeyFunc::KeyFunc_Equal,					NSID_MAIN_WINDOW_KEYFUNC_EQUAL		},
	{	MainWindowKeyFunc::KeyFunc_Point,					NSID_MAIN_WINDOW_KEYFUNC_POINT		},
	{	MainWindowKeyFunc::KeyFunc_Negate,					NSID_MAIN_WINDOW_KEYFUNC_NEGATE		},
	{	MainWindowKeyFunc::KeyFunc_Hex,						NSID_MAIN_WINDOW_KEYFUNC_HEX			},
	{	MainWindowKeyFunc::KeyFunc_Dec,						NSID_MAIN_WINDOW_KEYFUNC_DEC			},
	{	MainWindowKeyFunc::KeyFunc_Oct,						NSID_MAIN_WINDOW_KEYFUNC_OCT			},
	{	MainWindowKeyFunc::KeyFunc_Bin,						NSID_MAIN_WINDOW_KEYFUNC_BIN			},
	{	MainWindowKeyFunc::KeyFunc_Minimize,				NSID_MAIN_WINDOW_KEYFUNC_MINIMIZE	},
	{	MainWindowKeyFunc::KeyFunc_Close,					NSID_MAIN_WINDOW_KEYFUNC_CLOSE		},
	{	MainWindowKeyFunc::KeyFunc_ShowHideCoverBrowser,	NSID_MAIN_WINDOW_KEYFUNC_COVER_BROWSER },
	{	MainWindowKeyFunc::KeyFunc_ToggleAlwaysOnTop,		NSID_MAIN_WINDOW_KEYFUNC_ALWAYS_ON_TOP },
	{	MainWindowKeyFunc::KeyFunc_ToggleLockPos,			NSID_MAIN_WINDOW_KEYFUNC_LOCK_POS	},
	{	MainWindowKeyFunc::KeyFunc_ShowPreferencesDialog,	NSID_MAIN_WINDOW_KEYFUNC_PREFERENCES },
	{	MainWindowKeyFunc::KeyFunc_ShowCurrentCoverInfo,	NSID_MAIN_WINDOW_KEYFUNC_ABOUT_COVER },
	{	MainWindowKeyFunc::KeyFunc_About,					NSID_MAIN_WINDOW_KEYFUNC_ABOUT		},
	{	KeyFuncOperation::KeyFunc_None,						0									}		// terminator
};

/**
 *	@brief	Constructor.
 */
EditKeymapDlg::EditKeymapDlg()
{
	isModified = false;
	isReadOnly = false;
	keyMappings = NULL;
	keyNameDB = NULL;
}

/**
 *	@brief	Destructor.
 */
EditKeymapDlg::~EditKeymapDlg()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	isReadOnly	true when read-only mode.
 *	@param[in]	keyNameDB
 */
void EditKeymapDlg::Init(bool isReadOnly, const KeyNameDB* keyNameDB)
{
	this->isReadOnly = isReadOnly;
	this->keyNameDB = keyNameDB;
}

void EditKeymapDlg::SetKeyMappings(KeyMappings* keyMappings)
{
	this->keyMappings = keyMappings;
	
	keyMappingManager.Clear();
	MainWindowKeyFunc keyFunc;
	keyMappingManager.Create(keyMappings, KeyMappings::STR_CATEGORY_MAIN_WINDOW, &keyFunc);
}

/**
 *	@brief	Called before the dialog is shown.
 */
void EditKeymapDlg::readyToShow()
{
	// keyboard name edit
	UICTextEdit* nameEdit = getNameTextEdit();
	if (NULL != nameEdit)
	{
		MBCString name;
		UTF8String utf8Name;
		keyMappings->GetTitle(utf8Name);
		UTF8Conv::ToMultiByte(name, utf8Name);
		nameEdit->StopModificationNotification(true);
		nameEdit->SetText(name.CString());
		nameEdit->StopModificationNotification(false);
		nameEdit->MakeEditable(!isReadOnly);
	}
	
	// assign button & remove button
	UICButton* assignButton = getAssignButton();
	if (NULL != assignButton)
	{
		assignButton->Enable(false);
	}
	UICButton* removeButton = getRemoveButton();
	if (NULL != removeButton)
	{
		removeButton->Enable(false);
	}
	
	// function listbox
	UICListBox* funcList = getFunctionListBox();
	if (NULL != funcList)
	{
		NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
		SInt32 index;
		for (index = 0; KeyFuncOperation::KeyFunc_None != funcInfo[index].keyFunc; index++)
		{
			MBCString funcName = stringLoader->LoadNativeString(funcInfo[index].funcNameId);
			funcList->AddItem(funcName.CString(), &funcInfo[index]);
		}
		funcList->SetSelectedItem(-1);
	}
	
	// grayout OK button when read-only mode.
	if (isReadOnly)
	{
		UICButton* okButton = getOKButton();
		if (NULL != okButton)
		{
			okButton->Enable(false);
		}
	}
	
	isModified = false;
}

class UpdateCurrentKeysListBox : public KeyMappingManager::KeyVisitor
{
public:
	UpdateCurrentKeysListBox(UICListBox* currentKeysList, const KeyNameDB* keyNameDB)
	{
		this->currentKeysList = currentKeysList;
		this->keyNameDB = keyNameDB;
	}
	
	/**
	 * @retval	true	continue iteration
	 * @retval	false	stop iteration
	 */
	virtual bool Visit(const KeyEventParameter& parameter)
	{
		MBCString keyName;
		keyNameDB->GetKeyName(&parameter, keyName);
		currentKeysList->AddItem(keyName.CString(), const_cast<KeyEventParameter*>(&parameter));
		
		return true;
	}

private:
	UICListBox*			currentKeysList;
	const KeyNameDB*	keyNameDB;
};

/**
 *	@brief	Handles UIComponent event.
 *	@param[in]	componentID	component ID
 *	@param[in]	eventCode	event code
 *	@param[in]	param1		parameter 1
 *	@param[in]	param2		parameter 2
 */
void EditKeymapDlg::HandleUICEvent(SInt32 componentID, int eventCode, SInt32 /* param1 */, void* /* param2 */)
{
#define HANDLE_EVENT(cid, evt, handler)		\
	if (cid == componentID && evt == eventCode)	handler

	HANDLE_EVENT(CID_NameTextEdit,			UICE_TextChanged,		handleNameTextEditChanged());
	HANDLE_EVENT(CID_FunctionListBox,		UICE_SelectionChanged,	handleFunctionListBoxSelectionChanged());
	HANDLE_EVENT(CID_CurrentKeysListBox,	UICE_SelectionChanged,	handleCurrentKeysListBoxSelectionChanged());
	HANDLE_EVENT(CID_KeyInput,				UICE_TextChanged,		handleKeyInputChanged());
	HANDLE_EVENT(CID_AssignButton,			UICE_ButtonClicked,		handleAssignButtonClicked());
	HANDLE_EVENT(CID_RemoveButton,			UICE_ButtonClicked,		handleRemoveButtonClicked());
	HANDLE_EVENT(CID_OKButton,				UICE_ButtonClicked,		handleOK());
	HANDLE_EVENT(CID_CancelButton,			UICE_ButtonClicked,		handleCancel());

#undef HANDLE_EVENT
}

/**
 *	@brief	Called when the value of name text-edit is changed.
 */
void EditKeymapDlg::handleNameTextEditChanged()
{
	isModified = true;
}

/**
 *	@brief	Called when the selection of function listbox is changed.
 */
void EditKeymapDlg::handleFunctionListBoxSelectionChanged()
{
	// enable/disable assign button
	updateAssignButtonState();

	// shows current keys which the selected function is assigned to.
	UICListBox* funcList = getFunctionListBox();
	if (NULL != funcList)
	{
		UICListBox* currentKeysList = getCurrentKeysListBox();
		if (NULL != currentKeysList	)
		{
			currentKeysList->StopSelectionChangedNotification(true);
			currentKeysList->RemoveAllItem();
			SInt32 funcIndex = funcList->GetSelectedItem();
			if (0 <= funcIndex)
			{
				UpdateCurrentKeysListBox updater(currentKeysList, keyNameDB);
				const FuncInfo* funcInfo = static_cast<FuncInfo*>(funcList->GetItemData(funcIndex));
				keyMappingManager.ForEachKey(funcInfo->keyFunc, &updater);
			}
			currentKeysList->StopSelectionChangedNotification(false);
			handleCurrentKeysListBoxSelectionChanged();
		}
	}
}

/**
 *	@brief	Called when the selection of current keys listbox is changed.
 */
void EditKeymapDlg::handleCurrentKeysListBoxSelectionChanged()
{
	UICListBox* currentKeysList = getCurrentKeysListBox();
	if (NULL != currentKeysList)
	{
		SInt32 keyIndex = currentKeysList->GetSelectedItem();
		
		// enable/disable remove button
		if (!isReadOnly)
		{
			UICButton* removeButton = getRemoveButton();
			if (NULL != removeButton)
			{
				removeButton->Enable(0 <= keyIndex);
			}
		}
		
		// set selected key to the key-input control
		if (0 <= keyIndex)
		{
			UICKeyInput* keyInput = getKeyInput();
			if (NULL != keyInput)
			{
				const KeyEventParameter* keyParam = static_cast<KeyEventParameter*>(currentKeysList->GetItemData(keyIndex));
				keyInput->StopValueChangedNotification(true);
				keyInput->SetKeyEventParameter(*keyParam);
				keyInput->StopValueChangedNotification(false);
				handleKeyInputChanged();
			}
		}
	}
}

/**
 *	@brief	Called when the value of key-input control is changed.
 */
void EditKeymapDlg::handleKeyInputChanged()
{
	// enable/disable assign button
	updateAssignButtonState();

	UICKeyInput* keyInput = getKeyInput();
	if (NULL != keyInput)
	{
		// set assigned function to text edit.
		UICTextEdit* assignedFuncEdit = getAssignedFunctionTextEdit();
		if (NULL != assignedFuncEdit)
		{
			const KeyEventParameter& keyParam = keyInput->GetKeyEventParameter();
			SInt32 func = keyMappingManager.GetFunction(keyParam);
			if (KeyFuncOperation::KeyFunc_None == func)
			{
				assignedFuncEdit->SetText(ALITERAL(""));
			}
			else
			{
				bool found = false;
				SInt32 index;
				for (index = 0; KeyFuncOperation::KeyFunc_None != funcInfo[index].keyFunc; index++)
				{
					if (funcInfo[index].keyFunc == func)
					{
						NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
						MBCString funcName = stringLoader->LoadNativeString(funcInfo[index].funcNameId);
						assignedFuncEdit->SetText(funcName.CString());
						found = true;
						break;
					}
				}
				if (!found)
				{
					assignedFuncEdit->SetText(ALITERAL(""));
				}
			}
		}
	}
}

/**
 *	@brief	Updates enable/disable state of assign button.
 */
void EditKeymapDlg::updateAssignButtonState()
{
	if (isReadOnly)
	{
		return;
	}

	UICButton* assignButton = getAssignButton();
	if (NULL == assignButton)
	{
		return;
	}

	bool isFuncOK = false;
	bool isKeyOK = false;

	UICListBox* funcList = getFunctionListBox();
	if (NULL != funcList)
	{
		if (funcList->GetSelectedItem() >= 0)
		{
			isFuncOK = true;
		}
	}
	
	UICKeyInput* keyInput = getKeyInput();
	if (NULL != keyInput)
	{
		const KeyEventParameter& keyParam = keyInput->GetKeyEventParameter();
		if (keyParam.IsValid())
		{
			isKeyOK = true;
		}
	}
	
	assignButton->Enable(isFuncOK && isKeyOK);
}

/**
 *	@brief	Called when assign button is clicked.
 */
void EditKeymapDlg::handleAssignButtonClicked()
{
	UICListBox* funcList = getFunctionListBox();
	UICKeyInput* keyInput = getKeyInput();
	if (NULL == funcList || NULL == keyInput)
	{
		return;
	}
	
	SInt32 funcIndex = funcList->GetSelectedItem();
	if (funcIndex < 0)
	{
		// function is not selected.
		return;
	}
	const FuncInfo* funcInfo = static_cast<FuncInfo*>(funcList->GetItemData(funcIndex));
	
	const KeyEventParameter& keyParam = keyInput->GetKeyEventParameter();
	if (!keyParam.IsValid())
	{
		// key parameter is not valid.
		return;
	}
	
	SInt32 oldFunc = keyMappingManager.GetFunction(keyParam);
	if (oldFunc == funcInfo->keyFunc)
	{
		// already assigned.
		return;
	}
	
	if (KeyFuncOperation::KeyFunc_None != oldFunc)
	{
		keyMappingManager.RemoveKey(oldFunc, keyParam);
	}
	keyMappingManager.AddKey(funcInfo->keyFunc, keyParam);
	isModified = true;
	
	keyInput->StopValueChangedNotification(true);
	handleFunctionListBoxSelectionChanged();
	keyInput->StopValueChangedNotification(false);
	handleKeyInputChanged();
}

/**
 *	@brief	Called when remove button is clicked.
 */
void EditKeymapDlg::handleRemoveButtonClicked()
{
	UICListBox* funcList = getFunctionListBox();
	UICListBox* currentKeysList = getCurrentKeysListBox();
	if (NULL == funcList || NULL == currentKeysList)
	{
		return;
	}
	
	SInt32 funcIndex = funcList->GetSelectedItem();
	if (funcIndex < 0)
	{
		// function is not selected.
		return;
	}
	const FuncInfo* funcInfo = static_cast<FuncInfo*>(funcList->GetItemData(funcIndex));

	SInt32 keyIndex = currentKeysList->GetSelectedItem();
	if (keyIndex < 0)
	{
		// key is not selected.
		return;
	}
	const KeyEventParameter* keyParam = static_cast<KeyEventParameter*>(currentKeysList->GetItemData(keyIndex));

	keyMappingManager.RemoveKey(funcInfo->keyFunc, *keyParam);
	isModified = true;
	
	UICKeyInput* keyInput = getKeyInput();
	if (NULL != keyInput)
	{
		keyInput->StopValueChangedNotification(true);
	}
	handleFunctionListBoxSelectionChanged();
	if (NULL != keyInput)
	{
		keyInput->StopValueChangedNotification(false);
	}
	handleKeyInputChanged();
}

/**
 *	@brief	Called when dialog is closing by Cancel button.
 */
void EditKeymapDlg::handleCancel()
{
	if (isModified)
	{
		MessageBoxProvider::Button button = getMessageBoxProvider()->DoMessageBox(
			NSID_QMSG_EDITED_KEYMAP_DISCARDING,
			MessageBoxProvider::ButtonType_YesNo,
			MessageBoxProvider::AlertType_Warning,
			MessageBoxProvider::Button_No);

		if (MessageBoxProvider::Button_Yes == button)
		{
			closeDialog(false);
		}
	}
	else
	{
		closeDialog(false);
	}
}

/**
 *	@brief	Called when dialog is closing by OK button.
 */
void EditKeymapDlg::handleOK()
{
	MBCString name;
	UICTextEdit* nameEdit = getNameTextEdit();
	if (NULL != nameEdit)
	{
		nameEdit->GetText(name);
	}
	if (name.IsEmpty())
	{
		getMessageBoxProvider()->DoMessageBox(
			NSID_EMSG_EMPTY_KEYMAP_NAME,
			MessageBoxProvider::ButtonType_OK,
			MessageBoxProvider::AlertType_Warning);
		if (NULL != nameEdit)
		{
			nameEdit->MakeFocus();
		}
		return;
	}
	
	// update keyMappings by keyMappingManager.
	if (NULL != keyMappings)
	{
		UTF8String utf8Title;
		UTF8Conv::FromMultiByte(utf8Title, name.CString());
		keyMappings->SetTitle(utf8Title.CString());
	
		MainWindowKeyFunc keyFunc;
		keyMappingManager.WriteOut(keyMappings, KeyMappings::STR_CATEGORY_MAIN_WINDOW, &keyFunc);
	}
	
	closeDialog(true);
}
