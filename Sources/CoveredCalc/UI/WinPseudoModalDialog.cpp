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
	@file		WinPseudoModalDialog.cpp
	@brief		Definition of WinPseudoModalDialog class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.07.23 created
*/

#include "Prefix.h"
#include "WinPseudoModalDialog.h"
#include "WinCoveredCalcApp.h"

/**
 *	@brief	Constructor
 *	@param[in]	uTemplateID	resource template ID.
 */
WinPseudoModalDialog::WinPseudoModalDialog(UINT uTemplateID)
	: WinDialog(uTemplateID)
{
	dialogResult = 0;
	isContinueModal = false;
}

/**
 *	@brief	Shows a modal dialog.
 *	@retrun dialog result.
 */
INT_PTR WinPseudoModalDialog::DoModal(HWND hParent)
{
	// disables parent window
	EnableWindow(hParent, FALSE);
	
	// creates modeless dialog
	if (!Create(hParent))
	{
		EnableWindow(hParent, TRUE);
		return -1;
	}
	
	// runs message loop
	isContinueModal = true;
	messageLoop();
	
	ShowWindow(m_hWnd, SW_HIDE);
	
	// enables parent window.
	EnableWindow(hParent, TRUE);
	
	DestroyWindow(m_hWnd);
	
	return dialogResult;
}

/**
 *	@brief	Closes this dialog.
 *	@param[in]	nResult	dialog result.
 *
 *	Call this function to close the modal dialog.
 *	Don't call Windows API version of EndDialog() directly.
 */
BOOL WinPseudoModalDialog::EndDialog(INT_PTR nResult)
{
	dialogResult = nResult;
	isContinueModal = false;
	PostMessage(m_hWnd, WM_NULL, 0, 0);
	
	return ::EndDialog(m_hWnd, nResult);
}

/**
 *	@brief	modal message loop
 */
void WinPseudoModalDialog::messageLoop()
{
	WinCoveredCalcApp* app = WinCoveredCalcApp::GetInstance();

	MSG	msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		bool doDispatch = app->CallMessageFilters(&msg);
		if (doDispatch)
		{
			if (!::IsDialogMessage(m_hWnd, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		
		if (!isContinueModal)
		{
			return;
		}
	}
}

LRESULT WinPseudoModalDialog::wndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			LRESULT lResult = WinDialog::wndProc(hWnd, uMsg, wParam, lParam);
			ShowWindow(m_hWnd, SW_SHOW);
			return lResult;
		}
		break;
	
	case WM_COMMAND:
		{
			WORD	wId = LOWORD( wParam );
			if ( IDCANCEL == wId || IDOK == wId )
			{
				EndDialog(static_cast<INT_PTR>(wId) );
				return TRUE;
			}
		}
		break;
	}
	
	return WinDialog::wndProc(hWnd, uMsg, wParam, lParam);
}
