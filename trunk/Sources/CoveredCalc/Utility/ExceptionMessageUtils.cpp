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
	@file		ExceptionMessageUtils.cpp
	@brief		Utility functions for message of the exception.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.12 created
*/

#include "Prefix.h"
#include "Exception.h"
#include "ExceptionMessageGenerator.h"
#include "UIMessageProvider.h"
#include "CoveredCalcApp.h"
#include "ExceptionMessageUtils.h"

// ---------------------------------------------------------------------
//! Shows a message box about specified exception
/*!
	@return a button which user selected.
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button
ExceptionMessageUtils::DoExceptionMessageBox(
	MessageBoxProvider* messageBoxProvider,														//!< provider for message box
	Exception* ex,																				//!< exception
	MessageBoxProvider::ButtonType buttonType /* = MessageBoxProvider::ButtonType_OK */,		//!< button type
	MessageBoxProvider::AlertType alertType /* = MessageBoxProvider::AlertType_Warning */,		//!< alert type
	MessageBoxProvider::Button defaultButton /* = MessageBoxProvider::Button_None */			//!< default button
)
{
	MBCString message;
	ExceptionMessageGenerator* generator = CoveredCalcApp::GetInstance()->GetExceptionMessageGenerator();
	ASSERT(NULL != generator);
	generator->GetMessageString(ex, message);
	
	return messageBoxProvider->DoMessageBox(message, buttonType, alertType, defaultButton);
}

// ---------------------------------------------------------------------
//! Shows a message box about specified exception
/*!
	@return a button which user selected.
	@note
	Currently, a text shown in the message box is just same as messageText parameter.
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button
ExceptionMessageUtils::DoExceptionMessageBoxWithText(
	MessageBoxProvider* messageBoxProvider,														//!< provider for message box
	Exception* /*ex*/,																				//!< exception
	ConstAStr messageText,																		//!< text shown in message box
	MessageBoxProvider::ButtonType buttonType /* = MessageBoxProvider::ButtonType_OK */,		//!< button type
	MessageBoxProvider::AlertType alertType /* = MessageBoxProvider::AlertType_Warning */,		//!< alert type
	MessageBoxProvider::Button defaultButton /* = MessageBoxProvider::Button_None */			//!< default button
)
{
	return messageBoxProvider->DoMessageBox(messageText, buttonType, alertType, defaultButton);
}

// ---------------------------------------------------------------------
//! Shows a message box about specified exception
/*!
	@return a button which user selected.
	@note
	Currently, a text shown in the message box is just same as messageTextId parameter.
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button
ExceptionMessageUtils::DoExceptionMessageBoxWithText(
	MessageBoxProvider* messageBoxProvider,														//!< provider for message box
	Exception* /*ex*/,																				//!< exception
	SInt32 messageTextId,																		//!< id of the text shown in message box
	MessageBoxProvider::ButtonType buttonType /* = MessageBoxProvider::ButtonType_OK */,		//!< button type
	MessageBoxProvider::AlertType alertType /* = MessageBoxProvider::AlertType_Warning */,		//!< alert type
	MessageBoxProvider::Button defaultButton /* = MessageBoxProvider::Button_None */			//!< default button
)
{
	return messageBoxProvider->DoMessageBox(messageTextId, buttonType, alertType, defaultButton);
}
