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

#include "Prefix.h"
#include "WinDialogControlCreator.h"
#include "DialogLayout.h"
#include "MBCString.h"
#include "WinCoveredCalcApp.h"

/**
 *	@brief	Constructor
 *	@param[in]	hDlg		dialog window handle
 *	@param[in]	layout		DialogLayout object
 */
WinDialogControlCreator::WinDialogControlCreator(HWND hDlg, const DialogLayout* layout)
{
	this->hDlg = hDlg;
	this->layout = layout;
	this->hFont = GetWindowFont(hDlg);
}

/**
 *	@brief	Destructor
 */
WinDialogControlCreator::~WinDialogControlCreator()
{
}

/**
 *	@brief	Creates a generic control.
 *	@param[in]	lpClassName		name of window class
 *	@param[in]	rectLayoutName	layout name of window rectangle
 *	@param[in]	controlID		control ID
 *	@param[in]	lpWindowName	name of window (window title)
 *	@param[in]	addStyle		window styles which is added to defaults
 *	@param[in]	removeStyle		window styles which is removed from defaults
 *	@param[in]	addExStyle		extended window styles which is added to defaults
 *	@param[in]	removeExStyle	extended window styles which is removed from defaults
 *	@return		window handle of created control.
 */
HWND WinDialogControlCreator::CreateGenericControl(LPCTSTR lpClassName,
													const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpWindowName,
													DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle)
{
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	DWORD dwExStyle = 0;

	dwStyle |= addStyle;
	dwStyle &= ~removeStyle;
	dwExStyle |= addExStyle;
	dwExStyle &= ~removeExStyle;

	HINSTANCE hInstance = WinCoveredCalcApp::GetInstance()->GetInstanceHandle();

	POINT	topLeft;
	topLeft.x = layout->GetComputedValue(ALITERAL("Window.left"));
	topLeft.y = layout->GetComputedValue(ALITERAL("Window.top"));

	RECT rect;
	rect.left = layout->GetComputedValue(rectLayoutName + ALITERAL(".left")) - topLeft.x;
	rect.top = layout->GetComputedValue(rectLayoutName + ALITERAL(".top")) - topLeft.y;
	rect.right = layout->GetComputedValue(rectLayoutName + ALITERAL(".right")) - topLeft.x;
	rect.bottom = layout->GetComputedValue(rectLayoutName + ALITERAL(".bottom")) - topLeft.y;

	MapDialogRect(hDlg, &rect);

	HWND hWnd = ::CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle,
					rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					hDlg, reinterpret_cast<HMENU>(controlID), hInstance, NULL);
	if (NULL == hWnd)
	{
		return NULL;
	}

	SetWindowFont(hWnd, hFont, FALSE);

	return hWnd;
}

/**
 *	@brief	Creates a static control.
 *	@param[in]	rectLayoutName	layout name of window rectangle
 *	@param[in]	controlID		control ID
 *	@param[in]	lpStaticLabel	label of static
 *	@param[in]	addStyle		window styles which is added to defaults
 *	@param[in]	removeStyle		window styles which is removed from defaults
 *	@param[in]	addExStyle		extended window styles which is added to defaults
 *	@param[in]	removeExStyle	extended window styles which is removed from defaults
 *	@return		window handle of created control.
 */
HWND WinDialogControlCreator::CreateStatic(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpStaticLabel,
													DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle)
{
	addStyle |= (SS_LEFT) & ~removeStyle;

	return CreateGenericControl(WC_STATIC,
			rectLayoutName, controlID, lpStaticLabel,
			addStyle, removeStyle, addExStyle, removeExStyle);
}

/**
 *	@brief	Creates a button control.
 *	@param[in]	rectLayoutName	layout name of window rectangle
 *	@param[in]	controlID		control ID
 *	@param[in]	lpButtonLabel	label of button
 *	@param[in]	addStyle		window styles which is added to defaults
 *	@param[in]	removeStyle		window styles which is removed from defaults
 *	@param[in]	addExStyle		extended window styles which is added to defaults
 *	@param[in]	removeExStyle	extended window styles which is removed from defaults
 *	@return		window handle of created control.
 */
HWND WinDialogControlCreator::CreateButton(const MBCString &rectLayoutName, SInt32 controlID, LPCTSTR lpButtonLabel,
													DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle)
{
	addStyle |= (WS_TABSTOP | BS_PUSHBUTTON) & ~removeStyle;
	
	return CreateGenericControl(WC_BUTTON,
			rectLayoutName, controlID, lpButtonLabel,
			addStyle, removeStyle, addExStyle, removeExStyle);
}

/**
 *	@brief	Creates a group-box control.
 *	@param[in]	rectLayoutName	layout name of window rectangle
 *	@param[in]	controlID		control ID
 *	@param[in]	lpGroupLabel	label of group-box
 *	@param[in]	addStyle		window styles which is added to defaults
 *	@param[in]	removeStyle		window styles which is removed from defaults
 *	@param[in]	addExStyle		extended window styles which is added to defaults
 *	@param[in]	removeExStyle	extended window styles which is removed from defaults
 *	@return		window handle of created control.
 */
HWND WinDialogControlCreator::CreateGroupBox(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpGroupLabel,
													DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle)
{
	addStyle |= (BS_GROUPBOX) & ~removeStyle;
	removeStyle |= (WS_TABSTOP | BS_PUSHBUTTON) & ~addStyle;

	return CreateButton(rectLayoutName, controlID, lpGroupLabel,
			addStyle, removeStyle, addExStyle, removeExStyle);
}

/**
 *	@brief	Creates a edit control.
 *	@param[in]	rectLayoutName	layout name of window rectangle
 *	@param[in]	controlID		control ID
 *	@param[in]	lpEditText		content string of edit
 *	@param[in]	addStyle		window styles which is added to defaults
 *	@param[in]	removeStyle		window styles which is removed from defaults
 *	@param[in]	addExStyle		extended window styles which is added to defaults
 *	@param[in]	removeExStyle	extended window styles which is removed from defaults
 *	@return		window handle of created control.
 */
HWND WinDialogControlCreator::CreateEdit(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpEditText,
													DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle)
{
	addStyle |= (WS_TABSTOP | ES_AUTOHSCROLL) & ~removeStyle;
	addExStyle |= (WS_EX_CLIENTEDGE) & ~removeExStyle;

	return CreateGenericControl(WC_EDIT,
			rectLayoutName, controlID, lpEditText,
			addStyle, removeStyle, addExStyle, removeExStyle);
}

/**
 *	@brief	Creates a listbox control.
 *	@param[in]	rectLayoutName	layout name of window rectangle
 *	@param[in]	controlID		control ID
 *	@param[in]	addStyle		window styles which is added to defaults
 *	@param[in]	removeStyle		window styles which is removed from defaults
 *	@param[in]	addExStyle		extended window styles which is added to defaults
 *	@param[in]	removeExStyle	extended window styles which is removed from defaults
 *	@return		window handle of created control.
 */
HWND WinDialogControlCreator::CreateListBox(const MBCString& rectLayoutName, SInt32 controlID,
													DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle)
{
	addStyle |= (WS_TABSTOP | LBS_NOINTEGRALHEIGHT) & ~removeStyle;
	addExStyle |= (WS_EX_CLIENTEDGE) & ~removeExStyle;

	return CreateGenericControl(WC_LISTBOX,
			rectLayoutName, controlID, ALITERAL(""),
			addStyle, removeStyle, addExStyle, removeExStyle);
}

/**
 *	@brief	Creates a combo box control.
 *	@param[in]	rectLayoutName	layout name of window rectangle
 *	@param[in]	controlID		control ID
 *	@param[in]	addStyle		window styles which is added to defaults
 *	@param[in]	removeStyle		window styles which is removed from defaults
 *	@param[in]	addExStyle		extended window styles which is added to defaults
 *	@param[in]	removeExStyle	extended window styles which is removed from defaults
 *	@return		window handle of created control.
 */
HWND WinDialogControlCreator::CreateComboBox(const MBCString& rectLayoutName, SInt32 controlID,
													DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle)
{
	addStyle |= (WS_TABSTOP) & ~removeStyle;

	return CreateGenericControl(WC_COMBOBOX,
			rectLayoutName, controlID, ALITERAL(""),
			addStyle, removeStyle, addExStyle, removeExStyle);
}

/**
 *	@brief	Creates a list-view control.
 *	@param[in]	rectLayoutName	layout name of window rectangle
 *	@param[in]	controlID		control ID
 *	@param[in]	addStyle		window styles which is added to defaults
 *	@param[in]	removeStyle		window styles which is removed from defaults
 *	@param[in]	addExStyle		extended window styles which is added to defaults
 *	@param[in]	removeExStyle	extended window styles which is removed from defaults
 *	@return		window handle of created control.
 */
HWND WinDialogControlCreator::CreateListView(const MBCString& rectLayoutName, SInt32 controlID,
													DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle)
{
	addStyle |= (WS_TABSTOP) & ~removeStyle;
	addExStyle |= (WS_EX_CLIENTEDGE) & ~removeExStyle;

	return CreateGenericControl(WC_LISTVIEW,
			rectLayoutName, controlID, ALITERAL(""),
			addStyle, removeStyle, addExStyle, removeExStyle);
}

/**
 *	@brief	Creates a trackbar control.
 *	@param[in]	rectLayoutName	layout name of window rectangle
 *	@param[in]	controlID		control ID
 *	@param[in]	addStyle		window styles which is added to defaults
 *	@param[in]	removeStyle		window styles which is removed from defaults
 *	@param[in]	addExStyle		extended window styles which is added to defaults
 *	@param[in]	removeExStyle	extended window styles which is removed from defaults
 *	@return		window handle of created control.
 */
HWND WinDialogControlCreator::CreateTrackbar(const MBCString& rectLayoutName, SInt32 controlID,
													DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle)
{
	addStyle |= (WS_TABSTOP) & ~removeStyle;

	return CreateGenericControl(TRACKBAR_CLASS,
			rectLayoutName, controlID, ALITERAL(""),
			addStyle, removeStyle, addExStyle, removeExStyle);
}
