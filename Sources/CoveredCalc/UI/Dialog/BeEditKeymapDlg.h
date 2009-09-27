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
	@file		BeEditKeymapDlg.h
	@brief		Definition of BeEditKeymapDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.06.01 created
*/

#ifndef _BEEDITKEYMAPDLG_H_
#define _BEEDITKEYMAPDLG_H_

#include "BeModalDialog.h"
#include "EditKeymapDlg.h"
#include "UICTextEditImpl.h"
#include "UICListBoxImpl.h"
#include "UICButtonImpl.h"
#include "UICKeyInputImpl.h"

/**
 *	@brief	Edit-Keymapping dialog on BeOS.
 */
class BeEditKeymapDlg : public BeModalDialog, public EditKeymapDlg
{
public:
								BeEditKeymapDlg();
	virtual						~BeEditKeymapDlg();
	
	void						Init(bool isReadOnly, const KeyNameDB* keyNameDB);
	
protected:
	virtual ConstAStr			getSemaphoreName();
	virtual MessageBoxProvider*	getMessageBoxProvider();

	virtual UICTextEdit*		getNameTextEdit()				{ return &uicNameTextEdit; }
	virtual UICListBox*			getFunctionListBox()			{ return &uicFunctionListBox; }
	virtual UICListBox*			getCurrentKeysListBox()			{ return &uicCurrentKeyListBox; }
	virtual	UICKeyInput*		getKeyInput()					{ return &uicKeyInput; }
	virtual UICTextEdit*		getAssignedFunctionTextEdit()	{ return &uicAssignedFunctionTextEdit; }
	virtual UICButton*			getAssignButton()				{ return &uicAssignButton; }
	virtual UICButton*			getRemoveButton()				{ return &uicRemoveButton; }
	virtual UICButton*			getOKButton()					{ return &uicOkButton; }
	virtual UICButton*			getCancelButton()				{ return &uicCancelButton; }

	virtual void				closeDialog(bool isOK);

private:
	// adapters
	UICTextEditControlImpl		uicNameTextEdit;
	UICListBoxImpl				uicFunctionListBox;
	UICListBoxImpl				uicCurrentKeyListBox;
	UICKeyInputImpl				uicKeyInput;
	UICTextEditControlImpl		uicAssignedFunctionTextEdit;
	UICButtonImpl				uicAssignButton;
	UICButtonImpl				uicRemoveButton;
	UICButtonImpl				uicOkButton;
	UICButtonImpl				uicCancelButton;
	
private:
	class DialogWindow : public BeModalDialogWindow
	{
	public:
								DialogWindow();
		virtual					~DialogWindow();
		
		void					Init(BeEditKeymapDlg* owner);
		
	protected:
		virtual void			initDialog();
		virtual void			MessageReceived(BMessage *message);
#if defined (ZETA)
		virtual void			languageChanged();
#endif
		
	private:
		void					createViews();
	
	private:
		BeEditKeymapDlg*		owner;
	};
	friend class DialogWindow;
};

#endif // _BEEDITKEYMAPDLG_H_
