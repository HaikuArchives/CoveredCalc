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
	@file       BeSkinWindow.cpp
	@brief      Implementation of BeSkinWindow class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2004.02.18 created
*/

#include "Prefix.h"
#include "BeSkinView.h"
#include "BeSkinWindow.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcApp.h"

////////////////////////////////////////
#define base	BWindow
////////////////////////////////////////

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeSkinWindow::BeSkinWindow(
	BRect frame,			//!< bounds of frame
	const char* name,		//!< name of window
	uint32 workspace,		//!< workspace
	bool isAlwaysOnTop		///< whether window is always on top
)
	: BWindow(frame, name, B_NO_BORDER_WINDOW_LOOK,
		(isAlwaysOnTop) ? B_FLOATING_ALL_WINDOW_FEEL : B_NORMAL_WINDOW_FEEL,
		B_NOT_RESIZABLE, workspace)
{
	skinView = NULL;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeSkinWindow::~BeSkinWindow()
{
}

// ---------------------------------------------------------------------
//! Initializes the object
// ---------------------------------------------------------------------
void BeSkinWindow::Init()
{
	skinView = newSkinView();
	AddChild(skinView);
	try
	{
		skinView->Init();
	}
	catch (Exception*)
	{
		RemoveChild(skinView);
		delete skinView;
		skinView = NULL;
		throw;
	}
}

// ---------------------------------------------------------------------
//! Quit message looping of this window.
// ---------------------------------------------------------------------
void BeSkinWindow::Quit()
{
	if (NULL != skinView)
	{
		skinView->Quit();
	}
	
	base::Quit();
}

// ---------------------------------------------------------------------
//! Called when window is activated or deactivated.
// ---------------------------------------------------------------------
void BeSkinWindow::WindowActivated(
	bool state			//!< true if windows has become active, false if window no longer is the active.
)
{
	if (state && NULL != skinView)
	{
		skinView->MakeFocus(true);
	}
	
	base::WindowActivated(state);
}

// ---------------------------------------------------------------------
//! Message handler.
// ---------------------------------------------------------------------
void BeSkinWindow::MessageReceived(
	BMessage *message		//!< received message.
)
{
	try
	{
		base::MessageReceived(message);
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
		ex->Delete();		
	}	
}
