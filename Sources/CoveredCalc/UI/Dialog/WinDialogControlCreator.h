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
	@file		WinDialogControlCreator.h
	@brief		Definition of WinDialogControlCreator class.
	@author 	ICHIMIYA Hironori (Hiron)
	@date		2008.11.08 created
*/

#ifndef _WINDIALOGCONTROLCREATOR_H_
#define _WINDIALOGCONTROLCREATOR_H_

class DialogLayout;
class MBCString;

/**
 *	@brief	This is a utility class which creates dialog controls.
 */
class WinDialogControlCreator
{
public:
						WinDialogControlCreator(HWND hDlg, const DialogLayout* layout);
	virtual				~WinDialogControlCreator();

	HWND				CreateGenericControl(LPCTSTR lpClassName, const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpWindowName, DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle);
	HWND				CreateStatic(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpStaticLabel, DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle);
	HWND				CreateButton(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpButtonLabel, DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle);
	HWND				CreateGroupBox(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpGroupLabel, DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle);
	HWND				CreateEdit(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpEditText, DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle);
	HWND				CreateListBox(const MBCString& rectLayoutName, SInt32 controlID, DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle);
	HWND				CreateComboBox(const MBCString& rectLayoutName, SInt32 controlID, DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle);
	HWND				CreateListView(const MBCString& rectLayoutName, SInt32 controlID, DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle);
	HWND				CreateTrackbar(const MBCString& rectLayoutName, SInt32 controlID, DWORD addStyle, DWORD removeStyle, DWORD addExStyle, DWORD removeExStyle);

#if 0
	HWND				CreateStatic(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpStaticLabel, bool isGroup, bool isTabStop = false, bool isVisible = true, DWORD dwStyle = 0, DWORD dwExStyle = 0);
	HWND				CreateButton(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpButtonLabel, bool isGroup, bool isTabStop = true, bool isVisible = true, DWORD dwStyle = BS_PUSHBUTTON, DWORD dwExStyle = 0);
	HWND				CreateGroupBox(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpGroupLabel, bool isGroup = true, bool isTabStop = false, bool isVisible = true, DWORD dwStyle = 0, DWORD dwExStyle = 0);
	HWND				CreateEdit(const MBCString& rectLayoutName, SInt32 controlID, LPCTSTR lpEditText, bool isGroup, bool isTabStop = true, bool isVisible = true, DWORD dwStyle = ES_AUTOHSCROLL, DWORD dwExStyle = WS_EX_CLIENTEDGE);
	HWND				CreateListBox(const MBCString& rectLayoutName, SInt32 controlID, bool isGroup, bool isTabStop = true, bool isVisible = true, DWORD dwStyle = LBS_NOTIFY | LBS_NOINTEGRALHEIGHT, DWORD dwExStyle = WS_EX_CLIENTEDGE);

	HWND				CreateComboBox(const MBCString& rectLayoutName, SInt32 controlID, bool isGroup, bool isTabStop = true, bool isVisible = true, DWORD dwStyle = CBS_SIMPLE, DWORD dwExStyle = 0);
	HWND				CreateListView(const MBCString& rectLayoutName, SInt32 controlID, bool isGroup, bool isTabStop = true, bool isVisible = true, DWORD dwStyle = 0, DWORD dwExStyle = WS_EX_CLIENTEDGE);
	HWND				CreateTrackbar(const MBCString& rectLayoutName, SInt32 controlID, bool isGroup, bool isTabStop = true, bool isVisible = true, DWORD dwStyle = 0, DWORD dwExStyle = 0);
#endif

private:
	HWND				hDlg;		///< window handle of dialog
	const DialogLayout*	layout;		///< DialogLayout object
	HFONT				hFont;		///< dialog font.
};

#endif _WINDIALOGCONTROLCREATOR_H_
