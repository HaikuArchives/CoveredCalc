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
	@file		UIControllerException.h
	@brief		Definition of UIControllerException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.20 created
*/

#ifndef _UICONTROLLEREXCEPTION_H_
#define _UICONTROLLEREXCEPTION_H_

#include "Exception.h"

// ---------------------------------------------------------------------
//! Exception about UIController interface
// ---------------------------------------------------------------------
class UIControllerException : public Exception
{
public:
						UIControllerException() : Exception() { }
						UIControllerException(Exception* inner) : Exception(inner) { }
	virtual				~UIControllerException() { }
};

namespace UIControllerExceptions
{
	// ---------------------------------------------------------------------
	//! Thrown when showing context menu is failed.
	// ---------------------------------------------------------------------
	class FailedToShowContextMenu : public UIControllerException
	{
	public:
							FailedToShowContextMenu(SInt32 menuId) : UIControllerException() { this->menuId = menuId; }
		virtual				~FailedToShowContextMenu() { }
		
		SInt32				GetMenuID() { return menuId; }
	
	private:
		SInt32				menuId;
	};

	// ---------------------------------------------------------------------
	//! Thrown when showing dialog is failed.
	// ---------------------------------------------------------------------
	class FailedToShowDialog : public UIControllerException
	{
	public:
							FailedToShowDialog(SInt32 dialogId) : UIControllerException() { this->dialogId = dialogId; }
		virtual				~FailedToShowDialog() { }
		
		SInt32				GetDialogID() { return dialogId; }
	
	private:
		SInt32				dialogId;
	};
}

#endif // _UICONTROLLEREXCEPTION_H_
