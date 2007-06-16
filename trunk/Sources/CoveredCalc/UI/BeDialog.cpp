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
#if defined(ZETA)
#include "BeCoveredCalcApp.h"
#endif // defined(ZETA)

////////////////////////////////////////
#define base	BWindow
////////////////////////////////////////

/**
 *	@brief	Constructor
 */
BeDialog::BeDialog(
	BRect frame,									///< window's content area rectangle given in screen coordinates
	const char* title,								///< window title
	window_type type,								///< window type
	uint32 flags,									///< window flags
	uint32 workspaces /* = B_CURRENT_WORKSPACE */	///< displayed workspaces mask
) 
	: BWindow(frame, title, type, flags, workspaces)
{
}

/**
 *	@brief	Constructor
 */
BeDialog::BeDialog(
	BRect frame,									///< window's content area rectangle given in screen coordinates
	const char *title,								///< window title
	window_look look, 								///< window look
	window_feel feel, 								///< window feel
	uint32 flags, 									///< window flags
	uint32 workspaces /* = B_CURRENT_WORKSPACE */	///< displayed workspaces mask
)
	: BWindow(frame, title, look, feel, flags, workspaces)
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeDialog::~BeDialog()
{
}

// ---------------------------------------------------------------------
//! Initialize view.
// ---------------------------------------------------------------------
void BeDialog::initView(
	BView* view					///< this view is initialized.
)
{
	// replace font.
	view->SetFont(be_plain_font);
	
	// initialize child views.
	int32 childCount = view->CountChildren();
	int childIndex;
	for (childIndex=0; childIndex<childCount; childIndex++)
	{
		initView(view->ChildAt(childIndex));
	}	
}

// ---------------------------------------------------------------------
//! Initialize.
// ---------------------------------------------------------------------
void BeDialog::Init(
)
{
	// initialize each view.
	int32 childCount = CountChildren();
	int childIndex;
	for (childIndex=0; childIndex<childCount; childIndex++)
	{
		initView(ChildAt(childIndex));
	}
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
