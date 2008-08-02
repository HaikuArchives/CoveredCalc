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
	@file		EditKeymapDlg.h
	@brief		Definition of EditKeymapDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.5.5 created
*/

#ifndef _EDITKEYMAPDLG_H_
#define _EDITKEYMAPDLG_H_

#include "MBCString.h"
#include "KeyMappingManager.h"

class KeyNameDB;
class KeyMappings;
class MessageBoxProvider;

class UICTextEdit;
class UICListBox;
class UICButton;
class UICKeyInput;

/**
 *	@brief	This is a base class of Edit-Keymapping dialog class.
 */
class EditKeymapDlg
{
public:
								EditKeymapDlg();
	virtual						~EditKeymapDlg();

	void						Init(bool isReadOnly, const KeyNameDB* keyNameDB);

	void						SetKeyMappings(KeyMappings* keyMappings);
	KeyMappings*				GetKeyMappings() { return keyMappings; }

protected:
	void						readyToShow();
	void						processNameTextEditChanged();
	void						processFunctionListBoxSelectionChanged();
	void						processCurrentKeysListBoxSelectionChanged();
	void						processKeyInputChanged();
	void						processAssignButtonClicked();
	void						processRemoveButtonClicked();
	bool						processOK();
	bool						processCancel();

protected:
	/**
	 *	@brief	Returns MessageBoxProvider object.
	 *	@return	MessageBoxProvider object.
	 */
	virtual MessageBoxProvider*	getMessageBoxProvider() = 0;

	virtual UICTextEdit*		getNameTextEdit() = 0;
	virtual UICListBox*			getFunctionListBox() = 0;
	virtual UICListBox*			getCurrentKeysListBox() = 0;
	virtual	UICKeyInput*		getKeyInput() = 0;
	virtual UICTextEdit*		getAssignedFunctionTextEdit() = 0;
	virtual UICButton*			getAssignButton() = 0;
	virtual UICButton*			getRemoveButton() = 0;
	virtual UICButton*			getOKButton() = 0;
	virtual UICButton*			getCancelButton() = 0;

private:
	void						updateAssignButtonState();

protected:
	struct FuncInfo
	{
		SInt32	keyFunc;
		SInt32	funcNameId;
	};
	static FuncInfo				funcInfo[];

protected:
	bool						isModified;			///< true if key-mapping is changed after the dialog was shown.
	bool						isReadOnly;			///< true if key-mapping is readonly.
	KeyMappings*				keyMappings;		///< target key-mappings
	const KeyNameDB*			keyNameDB;

	KeyMappingManager			keyMappingManager;
};

#endif // _EDITKEYMAPDLG_H_
