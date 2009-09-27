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
	@file		WinSelectLanguageDlg.h
	@brief		Definition of WinSelectLanguageDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.9.13 created
*/

#ifndef _WINSELECTLANGUAGEDLG_H_
#define _WINSELECTLANGUAGEDLG_H_

#include "WinDialog.h"
#include "SelectLanguageDlg.h"
#include "UICListBoxDDImpl.h"

/**
 *	@brief "select-language" dialog (for Windows)
 */
class WinSelectLanguageDlg : public WinDialog, public SelectLanguageDlg
{
public:
									WinSelectLanguageDlg();
	virtual							~WinSelectLanguageDlg();
	
protected:
	virtual LRESULT					wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual MessageBoxProvider*		getMessageBoxProvider() { return this; }

	virtual UICListBox*				getLanguageListBox() { return &uicLanguageListBox; }
	virtual void					closeDialog(bool isOK);

private:
	LRESULT							onInitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT							onCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void							createControls();

private:
	// adapters
	UICListBoxDDImpl				uicLanguageListBox;
};

#endif // _WINSELECTLANGUAGEDLG_H_
