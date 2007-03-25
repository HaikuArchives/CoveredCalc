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
	@file       BeDeskbarPseudoWindow.cpp
	@brief      Implementation of BeDeskbarPseudoWindow class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2005.10.29 created
*/

#include "Prefix.h"
#include "BeDeskbarPseudoWindow.h"
#include <support/Autolock.h>

/**
 *	@brief	Constructor
 */
BeDeskbarPseudoWindow::BeDeskbarPseudoWindow()
	:	BWindow(BRect(-10000, -10000, -10000, -10000), NULL, B_NO_BORDER_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_CURRENT_WORKSPACE)
{
	realWindow = NULL;
}

/**
 *	@brief	Destructor
 */
BeDeskbarPseudoWindow::~BeDeskbarPseudoWindow()
{
}

/**
 *	@brief	Initialize
 */
void BeDeskbarPseudoWindow::Init(
	BWindow* realWindow		///< real target window
)
{
	this->realWindow = realWindow;
	
	SetTitle(realWindow->Title());

	// start looping in hidden state.	
	Hide();
	Show();
}

/**
 *	@brief	Called when window is activated or deactivated.
 */
void BeDeskbarPseudoWindow::WindowActivated(
	bool active				///< active or deavtive
)
{
	if (active)
	{
		Activate(false);
		BAutolock locker(realWindow);
		realWindow->Activate(true);
	}
}
