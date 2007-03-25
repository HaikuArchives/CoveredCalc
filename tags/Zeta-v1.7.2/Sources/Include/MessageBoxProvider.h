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
	@file		MessageBoxProvider.h
	@brief		Definition of MessageBoxProvider interface
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.12 created
*/

#ifndef _MESSAGEBOXPROVIDER_H_
#define _MESSAGEBOXPROVIDER_H_

// ---------------------------------------------------------------------
//! This interface provides alert message box.
// ---------------------------------------------------------------------
class MessageBoxProvider
{
public:
	enum ButtonType
	{
		ButtonType_OK = 0,			//!< OK
		ButtonType_OKCancel,		//!< OK and Cancel
		ButtonType_YesNo,			//!< Yes and No
		ButtonType_YesNoCancel		//!< Yes, No and Cancel
	};
	
	enum AlertType
	{
		AlertType_Empty = 0,		//!< empty alert
		AlertType_Information,		//!< information alert
		AlertType_Idea,				//!< idea alert
		AlertType_Warning,			//!< warning alert
		AlertType_Stop,				//!< stop alert
	};
	
	enum Button
	{
		Button_None = -1,			//!< Nothing
		Button_OK = 0,				//!< OK
		Button_Cancel,				//!< Cancel
		Button_Yes,					//!< Yes
		Button_No,					//!< No
	};

public:
						MessageBoxProvider() { }
	virtual				~MessageBoxProvider() { }
	
	// ---------------------------------------------------------------------
	//! Shows a message box
	/*!
		@return the button which user selected.
	*/
	// ---------------------------------------------------------------------
	virtual Button		DoMessageBox(
							ConstAStr message,					//!< message text
							ButtonType buttonType,				//!< button type
							AlertType alertType,				//!< alert type
							Button defaultButton = Button_None	//!< default button
						) = 0;

	// ---------------------------------------------------------------------
	//! Shows a message box
	/*!
		@return the button which user selected.
	*/
	// ---------------------------------------------------------------------
	virtual Button		DoMessageBox(
							SInt32 messageId,					//!< message id of the text
							ButtonType buttonType,				//!< button type
							AlertType alertType,				//!< alert type
							Button defaultButton = Button_None	//!< default button
						) = 0;						
};

#endif // _MESSAGEBOXPROVIDER_H_
