/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2007 CoveredCalc Project Contributors
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
	@file		WinMessageBoxProvider.cpp
	@brief		Definition of WinMessageBoxProvider class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.12 created
*/

#include "Prefix.h"
#include "UIMessageProvider.h"
#include "CoveredCalcApp.h"
#include "WinMessageBoxProvider.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
WinMessageBoxProvider::WinMessageBoxProvider(
	HWND hWnd			//!< handle of owner window
)
{
	this->hWnd = hWnd;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
WinMessageBoxProvider::~WinMessageBoxProvider()
{
}

// ---------------------------------------------------------------------
//! Shows a message box
/*!
	@return the button which user selected.
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button WinMessageBoxProvider::DoMessageBox(
	ConstAStr message,							//!< message text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	UINT uType = 0;
	switch (buttonType)
	{
	case ButtonType_OK:
		uType |= MB_OK;
		break;
	case ButtonType_OKCancel:
		uType |= MB_OKCANCEL;
		break;
	case ButtonType_YesNo:
		uType |= MB_YESNO;
		break;
	case ButtonType_YesNoCancel:
		uType |= MB_YESNOCANCEL;
		break;
	default:
		ASSERT(false);
	}
	
	switch (alertType)
	{
	case AlertType_Information:
	case AlertType_Idea:
		uType |= MB_ICONINFORMATION;
		break;
	case AlertType_Warning:
		uType |= MB_ICONEXCLAMATION;
		break;
	case AlertType_Stop:
		uType |= MB_ICONSTOP;
		break;
	}
	
	switch (defaultButton)
	{
	case Button_OK:
		if (ButtonType_OK == buttonType || ButtonType_OKCancel == buttonType)
		{
			uType |= MB_DEFBUTTON1;
		}
		break;
	case Button_Cancel:
		if (ButtonType_OKCancel == buttonType)
		{
			uType |= MB_DEFBUTTON2;
		}
		else if (ButtonType_YesNoCancel == buttonType)
		{
			uType |= MB_DEFBUTTON3;
		}
		break;
	case Button_Yes:
		if (ButtonType_YesNo == buttonType || ButtonType_YesNoCancel == buttonType)
		{
			uType |= MB_DEFBUTTON1;
		}
		break;
	case Button_No:
		if (ButtonType_YesNo == buttonType || ButtonType_YesNoCancel == buttonType)
		{
			uType |= MB_DEFBUTTON2;
		}
		break;
	}
	
	MBCString title;
	CoveredCalcApp::GetInstance()->GetMessageProvider()->GetMessage(IDS_APPNAME, title);
	
	int result = ::MessageBox(hWnd, message, title, uType);

	switch (result)
	{
	case IDOK:
		return Button_OK;
		break;
	case IDCANCEL:
		return Button_Cancel;
		break;
	case IDYES:
		return Button_Yes;
		break;
	case IDNO:
		return Button_No;
		break;
	default:
		ASSERT(false);
		return Button_None;
	}	
}

// ---------------------------------------------------------------------
//! Shows a message box
/*!
	@return the button which user selected.
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button WinMessageBoxProvider::DoMessageBox(
	SInt32 messageId,							//!< message id of the text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	MBCString message;
	CoveredCalcApp::GetInstance()->GetMessageProvider()->GetMessage(messageId, message);
	
	return DoMessageBox(message, buttonType, alertType, defaultButton);
}
