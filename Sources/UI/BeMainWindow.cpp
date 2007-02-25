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
	@file       BeMainWindow.cpp
	@brief      Implementation of BeMainWindow class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2004.02.18 created
*/

#include "Prefix.h"
#include <AppKit.h>
#include "BeMainView.h"
#include "BeMainWindow.h"
#include "CommandID.h"
#include <support/Autolock.h>

////////////////////////////////////////
#define base	BeSkinWindow
////////////////////////////////////////

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeMainWindow::BeMainWindow(
	BRect frame,			//!< bounds of frame
	const char* name,		//!< name of window
	uint32 workspace,		//!< workspace
	bool isAlwaysOnTop		///< whether window is always on top
)
	: BeSkinWindow(frame, name, workspace, isAlwaysOnTop)
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeMainWindow::~BeMainWindow()
{
}

// ---------------------------------------------------------------------
//! Shows about dialog.
// ---------------------------------------------------------------------
void BeMainWindow::ShowAboutDialog()
{
	BeSkinView* skinView = GetSkinView();
	if (NULL != skinView)
	{
		PostMessage(ID_ABOUT, skinView);
	}	
}

// ---------------------------------------------------------------------
//! Creates a new skin view and returns it
// ---------------------------------------------------------------------
BeSkinView* BeMainWindow::newSkinView()
{
	return new BeMainView(Bounds(), "Main View");
}

// ---------------------------------------------------------------------
//! Quit message looping of this window.
// ---------------------------------------------------------------------
void BeMainWindow::Quit()
{
	// quit the application
	be_app->PostMessage(B_QUIT_REQUESTED);
	
	base::Quit();
}

/** 
 *	@breif	Called when requested to quit.
 *	@return	true if continue quitting.
 */
bool BeMainWindow::QuitRequested()
{
	BeMainView* mainView = dynamic_cast<BeMainView*>(GetSkinView());
	if (NULL != mainView)
	{
		BeDeskbarPseudoWindow* deskbarWindow = mainView->GetDeskbarWindow();
		if (NULL != deskbarWindow)
		{
			deskbarWindow->PostMessage(B_QUIT_REQUESTED);
		}
	}
	
	return base::QuitRequested();
}

/**
 *	@brief	Called when workspace is changed.
 */
void BeMainWindow::WorkspacesChanged(
	uint32 oldWorkspaces,		///< old workspace
	uint32 newWorkspaces		///< new workspace
)
{
	BeMainView* mainView = dynamic_cast<BeMainView*>(GetSkinView());
	if (NULL != mainView)
	{
		BeDeskbarPseudoWindow* deskbarWindow = mainView->GetDeskbarWindow();
		if (NULL != deskbarWindow)
		{
			BAutolock locker(deskbarWindow);
			if (locker.IsLocked())
			{
				deskbarWindow->SetWorkspaces(newWorkspaces);
			}
		}
	}
	
	base::WorkspacesChanged(oldWorkspaces, newWorkspaces);
}

/**
 *	@brief	Called whdn window is activated or deactivated.
 */
void BeMainWindow::WindowActivated(
	bool active			///< whether window is activated or deactivated.
)
{
	base::WindowActivated(active);
	if (active)
	{
		BeMainView* mainView = dynamic_cast<BeMainView*>(GetSkinView());
		if (NULL != mainView)
		{
			mainView->MainWindowActivated();
		}
	}
}
