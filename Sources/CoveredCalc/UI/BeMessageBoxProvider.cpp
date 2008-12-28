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
	@file		BeMessageBoxProvider.cpp
	@brief		Implemenetation of BeMessageBoxProvider class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.20 created
*/

#include "Prefix.h"
#include <interface/Alert.h>
#include "MBCString.h"
#include "CoveredCalcApp.h"
#include "BeMessageBoxProvider.h"
#include "StringID.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeMessageBoxProvider::BeMessageBoxProvider()
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeMessageBoxProvider::~BeMessageBoxProvider()
{
}

// ---------------------------------------------------------------------
//! Shows a message box
/*!
	@return the button which user selected.
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button BeMessageBoxProvider::DoMessageBox(
	ConstAStr message,							//!< message text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	const char* button0Label = NULL;
	const char* button1Label = NULL;
	const char* button2Label = NULL;
	char button0Shortcut = '\0';
	char button1Shortcut = '\0';
	char button2Shortcut = '\0';
	int32 defaultButtonIndex = 0;
	button_spacing spacing = B_EVEN_SPACING;
	switch (buttonType)
	{
	case ButtonType_OK:
		button0Label = "OK";
		defaultButtonIndex = 0;
		break;
	case ButtonType_OKCancel:
		button0Label = "Cancel";	button0Shortcut = B_ESCAPE;
		button1Label = "OK";
		if (Button_OK == defaultButton)
		{
			defaultButtonIndex = 1;
		}
		else
		{
			defaultButtonIndex = 0;
		}
		break;
	case ButtonType_YesNo:
		button0Label = "No";		button0Shortcut = 'n';
		button1Label = "Yes";		button1Shortcut = 'y';
		if (Button_Yes == defaultButton)
		{
			defaultButtonIndex = 1;
		}
		else
		{
			defaultButtonIndex = 0;
		}
		break;
	case ButtonType_YesNoCancel:
		button0Label = "Cancel";	button0Shortcut = B_ESCAPE;
		button1Label = "No";		button1Shortcut = 'n';
		button2Label = "Yes";		button2Shortcut = 'y';
		spacing = B_OFFSET_SPACING;
		if (Button_Yes == defaultButton)
		{
			defaultButtonIndex = 2;
		}
		else if (Button_No == defaultButton)
		{
			defaultButtonIndex = 1;
		}
		else
		{
			defaultButtonIndex = 0;
		}
		break;
	default:
		ASSERT(false);
	}
	
	alert_type type = B_WARNING_ALERT;
	switch (alertType)
	{
	case AlertType_Empty:
		type = B_EMPTY_ALERT;
		break;
	case AlertType_Information:
		type = B_INFO_ALERT;
		break;
	case AlertType_Idea:
		type = B_IDEA_ALERT;
		break;
	case AlertType_Warning:
		type = B_WARNING_ALERT;
		break;
	case AlertType_Stop:
		type = B_STOP_ALERT;
		break;
	default:
		ASSERT(false);
	}

	MBCString title = CoveredCalcApp::GetInstance()->LoadNativeString(NSID_APPNAME);

	BAlert* alert = new BAlert(title, message,
							button0Label, button1Label, button2Label,
							B_WIDTH_AS_USUAL, spacing, type);
	if ('\0' != button0Shortcut)
	{
		alert->SetShortcut(0, button0Shortcut);
	}
	if ('\0' != button1Shortcut)
	{
		alert->SetShortcut(1, button1Shortcut);
	}
	if ('\0' != button2Shortcut)
	{
		alert->SetShortcut(2, button2Shortcut);
	}
	alert->SetDefaultButton(alert->ButtonAt(defaultButtonIndex));
	int32 result = alert->Go();
	
	switch (buttonType)
	{
	case ButtonType_OK:
		return Button_OK;
		break;
	case ButtonType_OKCancel:
		switch (result)
		{
		case 0:
			return Button_Cancel;
			break;
		case 1:
			return Button_OK;
			break;
		default:
			return Button_Cancel;
			break;
		}
		break;
	case ButtonType_YesNo:
		switch (result)
		{
		case 0:
			return Button_No;
			break;
		case 1:
			return Button_Yes;
			break;
		default:
			return Button_No;
			break;
		}
		break;
	case ButtonType_YesNoCancel:
		switch (result)
		{
		case 0:
			return Button_Cancel;
			break;
		case 1:
			return Button_No;
			break;
		case 2:
			return Button_Yes;
			break;
		default:
			return Button_Cancel;
			break;
		}
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
MessageBoxProvider::Button BeMessageBoxProvider::DoMessageBox(
	SInt32 messageId,							//!< message id of the text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	MBCString message = CoveredCalcApp::GetInstance()->LoadNativeString(messageId);		
	return DoMessageBox(message, buttonType, alertType, defaultButton);
}
