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
	@file		WinAboutCurrentCoverDlg.h
	@brief		Definition of WinAboutCurrentCoverDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.05 created
*/

#ifndef _WINABOUTCURRENTCOVERDLG_H_
#define _WINABOUTCURRENTCOVERDLG_H_

#include "WinDialog.h"
#include "AboutCurrentCoverDlg.h"
#include "UICTextEditImpl.h"

// ---------------------------------------------------------------------
//! 現在のカバーについてダイアログ for Windows
// ---------------------------------------------------------------------
class WinAboutCurrentCoverDlg : public WinDialog, public AboutCurrentCoverDlg
{
public:
							WinAboutCurrentCoverDlg();
							~WinAboutCurrentCoverDlg();
protected:
	virtual LRESULT			wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual UICTextEdit*	getNameTextEdit() { return &uicNameTextEdit; }
	virtual UICTextEdit*	getDescriptionTextEdit() { return &uicDescriptionTextEdit; }
	virtual UICTextEdit*	getAboutTextEdit() { return &uicAboutTextEdit; }

private:
	LRESULT					onInitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT					onDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void					createControls();

private:
	UICTextEditImpl			uicNameTextEdit;
	UICTextEditImpl			uicDescriptionTextEdit;
	UICTextEditImpl			uicAboutTextEdit;
};

#endif // _WINABOUTCURRENTCOVERDLG_H_
