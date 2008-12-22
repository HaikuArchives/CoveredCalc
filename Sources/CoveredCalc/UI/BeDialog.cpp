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
	@file		BeDialog.cpp
	@brief		Implementation of BeDialog class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.12.07 created
*/

#include "Prefix.h"
#include <InterfaceKit.h>
#include <support/Autolock.h>
#include "BeDialog.h"
#include "CommandID.h"
#include "CoveredCalcApp.h"
#include "BeDialogControlHelper.h"
#if defined(ZETA)
#include "BeCoveredCalcApp.h"
#endif // defined(ZETA)

////////////////////////////////////////
#define base	BWindow
////////////////////////////////////////

const MBCString BeDialog::ITEMNAME_WINDOW = ALITERAL("Window");

/**
 *	@brief	Constructor
 *	@param[in]	dialogID
 *	@param[in]	type		window type
 *	@param[in]	workspaces	displayed workspaces mask
 */
BeDialog::BeDialog(SInt32 dialogID, window_type type, uint32 flags, uint32 workspaces /* = B_CURRENT_WORKSPACE */)
	: BWindow(BRect(0, 0, 0, 0), ALITERAL(""), type, flags, workspaces)
{
	this->dialogID = dialogID;
}

/**
 *	@brief	Constructor
 *	@param[in]	dialogID
 *	@param[in]	look		window look
 *	@param[in]	feel		window feel
 *	@param[in]	flags		window flags
 *	@param[in]	workspaces	displayed workspaces mask
 */
BeDialog::BeDialog(SInt32 dialogID, window_look look, window_feel feel, uint32 flags, uint32 workspaces /* = B_CURRENT_WORKSPACE */)
	: BWindow(BRect(0, 0, 0, 0), ALITERAL(""), look, feel, flags, workspaces)
{
	this->dialogID = dialogID;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeDialog::~BeDialog()
{
}

// ---------------------------------------------------------------------
//! Initialize.
// ---------------------------------------------------------------------
void BeDialog::Init(
)
{
	CoveredCalcApp::GetInstance()->LoadDialogLayout(dialogID, dialogLayout);
	dialogLayout.Compute();

	BeDialogControlHelper dch(&dialogLayout);
	BRect windowRect = dch.GetItemRect(ALITERAL("Window"), MBCString());
	MoveTo(windowRect.left, windowRect.top);
	ResizeTo(windowRect.Width(), windowRect.Height());
	
	initDialog();
}

// ---------------------------------------------------------------------
//! Move the window to center of the screen.
// ---------------------------------------------------------------------
void BeDialog::moveToCenterOfScreen()
{
	BAutolock locker(this);

	BRect screenFrame;
	{
		BScreen screen;
		screenFrame = screen.Frame();
	}
	
	BRect windowFrame = Frame();
	
	float left = (screenFrame.Width() - windowFrame.Width()) / 2;
	float top = (screenFrame.Height() - windowFrame.Height()) / 2;
	MoveTo(left, top);
}

#if defined (ZETA)
/**
 *	@brief	Called when language setting is changed.
 */
void BeDialog::languageChanged()
{
}
#endif

// ---------------------------------------------------------------------
//! Message handler.
// ---------------------------------------------------------------------
void BeDialog::MessageReceived(
	BMessage *message		//!< received message.
)
{
	switch (message->what)
	{
	case ID_DIALOG_ACTIVATE:
		onCommandActivate(message);
		break;
#if defined (ZETA)
	case B_LANGUAGE_CHANGED:
		if (BeCoveredCalcApp::GetInstance()->IsLocaleKitAvailable())
		{
			languageChanged();
		}
		break;
#endif
	default:
		base::MessageReceived(message);
		break;
	}
}

// ---------------------------------------------------------------------
//! Called when Activate command message is dispatched.
// ---------------------------------------------------------------------
void BeDialog::onCommandActivate(
	BMessage* message		//!< received message.
)
{
	if (IsMinimized())
		Minimize(false);
	if (!IsActive())
		Activate(true);
}
