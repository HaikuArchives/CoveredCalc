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
	@file		BeSelectLanguageDlg.h
	@brief		Definition of BeSelectLanguageDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.09.19 created
*/

#ifndef _BESELECTLANGUAGEDLG_H_
#define _BESELECTLANGUAGEDLG_H_

#include "BeModalDialog.h"
#include "SelectLanguageDlg.h"
#include "UICListBoxDDImpl.h"

class BMenu;

/**
 *	@brief	Preferences dialog on BeOS.
 */
class BeSelectLanguageDlg : public BeModalDialog, public SelectLanguageDlg
{
public:
									BeSelectLanguageDlg();
	virtual							~BeSelectLanguageDlg();

	void							Init();
	
protected:
	virtual ConstAStr				getSemaphoreName();
	virtual MessageBoxProvider*		getMessageBoxProvider();
	
	virtual UICListBox*				getLanguageListBox() { return &uicLanguageListBox; }
	
	virtual void					closeDialog(bool isOK);
	
private:
	// adapters
	UICListBoxDDImpl				uicLanguageListBox;

private:
	class DialogWindow : public BeModalDialogWindow
	{
	public:
									DialogWindow();
		virtual						~DialogWindow();
		
		void						Init(BeSelectLanguageDlg* owner);

	protected:
		virtual void				initDialog();
		virtual void				MessageReceived(BMessage *message);
#if defined (ZETA)
		virtual void				languageChanged();
#endif

	private:
		void						createViews();
	
	private:
		BeSelectLanguageDlg*		owner;
		BMenu*						langMenu;			///< language popup menu
	};
	friend class DialogWindow;
};

#endif // _BESELECTLANGUAGEDLG_H_
