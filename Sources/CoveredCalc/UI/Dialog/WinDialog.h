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
	@file		WinDialog.h
	@brief		Definition of WinDialog class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.03.22 created
*/

#ifndef _WINDIALOG_H_
#define _WINDIALOG_H_

#include "HrnDlg.h"
#include "MessageBoxProvider.h"
#include "DialogLayout.h"

// ---------------------------------------------------------------------
//! Windows 版ダイアログクラス
// ---------------------------------------------------------------------
class WinDialog : public CHrnDlg, virtual public MessageBoxProvider
{
public:
						WinDialog(SInt32 dialogID);

	int					DoModal( HWND hParent );
	BOOL				Create( HWND hParent );

	// MessageBoxProvider インタフェースの実装
	virtual Button		DoMessageBox(ConstAStr message, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);
	virtual Button		DoMessageBox(SInt32 messageId, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);


protected:
	virtual DWORD		getDialogStyle();
	virtual	DWORD		getDialogExStyle();

protected:
	DialogLayout*		getDialogLayout() { return &dialogLayout; }

protected:
	virtual LRESULT		wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void				readyDialogLayout();
	BYTE*				createDialogTemplate();
	LRESULT				onInitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	SInt32				dialogID;			///< dialog ID
	DialogLayout		dialogLayout;		///< dialog layout object.
};

#endif // _WINDIALOG_H_
