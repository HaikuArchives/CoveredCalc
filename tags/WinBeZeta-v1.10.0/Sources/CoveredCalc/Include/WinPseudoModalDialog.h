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
	@file		WinPseudoModalDialog.h
	@brief		Definition of WinPseudoModalDialog class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.07.23 created
*/

#ifndef _WINPSEUDOMODALDIALOG_H_
#define _WINPSEUDOMODALDIALOG_H_

#include "WinDialog.h"

/**
 *	@brief	Pseudo modal dialog class.
 *
 *	This class actually creates a modeless dialog in DoModal() function,
 *	but runs own message loop in it and behaves as if it is a modal dialog.
 *	In its own message loop, the retrieved message is filtered
 *	by WinCoveredCalcApp::CallMessageFilters().
 *	That is why this class is introduced.
 */
class WinPseudoModalDialog : public WinDialog
{
public:
						WinPseudoModalDialog(UINT uTemplateID);

	INT_PTR				DoModal(HWND hParent);
	BOOL				EndDialog(INT_PTR nResult);

protected:
	virtual LRESULT		wndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void		messageLoop();

private:
	INT_PTR				dialogResult;		///< return value of DoModal.
	bool				isContinueModal;	///< dialog is modal while this value is true.
};

#endif // _WINPSEUDOMODALDIALOG_H_
