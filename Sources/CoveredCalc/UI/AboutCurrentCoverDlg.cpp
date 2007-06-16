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
	@file		AboutCurrentCoverDlg.cpp
	@brief		Implementation of AboutCurrentCoverDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.02 created
*/

#include "Prefix.h"
#include "AboutCurrentCoverDlg.h"
#include "CoverManager.h"
#include "CoveredCalcApp.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
AboutCurrentCoverDlg::AboutCurrentCoverDlg()
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
AboutCurrentCoverDlg::~AboutCurrentCoverDlg()
{
}

// ---------------------------------------------------------------------
//! Initializes the dialog.
// ---------------------------------------------------------------------
void AboutCurrentCoverDlg::initialize()
{
	CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
	ASSERT(NULL != manager);
	manager->RegisterCoverChangeEventHandler(this);

	update();
}

// ---------------------------------------------------------------------
//! Terminates the dialog.
// ---------------------------------------------------------------------
void AboutCurrentCoverDlg::terminate()
{
	CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
	ASSERT(NULL != manager);
	manager->UnregisterCoverChangeEventHandler(this);
}

// ---------------------------------------------------------------------
//! Updates data in the dialog.
// ---------------------------------------------------------------------
void AboutCurrentCoverDlg::update()
{
	CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
	ASSERT(NULL != manager);
	const CoverDef* coverDef = manager->GetCoverDef();
	setDataToDialog(coverDef);	
}

// ---------------------------------------------------------------------
//! Called when current CoverDef is changed.
// ---------------------------------------------------------------------
void AboutCurrentCoverDlg::CoverDefChanged()
{
	update();
}

// ---------------------------------------------------------------------
//! Called when current cover number is changed.
// ---------------------------------------------------------------------
void AboutCurrentCoverDlg::CurrentCoverChanged()
{
	// do nothing.
}
