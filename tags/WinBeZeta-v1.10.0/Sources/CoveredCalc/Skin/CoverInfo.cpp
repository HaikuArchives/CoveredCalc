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
	@file		CoverInfo.cpp
	@brief		Implementation of CoverInfo class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.18 created
*/

#include "Prefix.h"
#include "CoverMainWindowInfo.h"
#include "CoverInfo.h"
#include "CoverDefValidateException.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverInfo::CoverInfo()
{
	mainWindowInfo = NULL;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverInfo::~CoverInfo()
{
	if (NULL != mainWindowInfo)
	{
		delete mainWindowInfo;
	}
}

// ---------------------------------------------------------------------
//! Sets an information about the main window.
/*!
	@note
	CoverInfo object has responsibility to delete the object
	specified at mainWindowInfo parameter.
	Caller must not delete it.	
*/
// ---------------------------------------------------------------------
void CoverInfo::SetMainWindowInfo(
	CoverMainWindowInfo* mainWindowInfo		//!< information
)
{
	if (NULL != this->mainWindowInfo)
	{
		delete this->mainWindowInfo;
	}
	this->mainWindowInfo = mainWindowInfo;
}

// ---------------------------------------------------------------------
//! Checks validation of data.
/*!
	@note
	This function is called after making this object has finished.
*/
// ---------------------------------------------------------------------
void CoverInfo::Validate()
{
	if (NULL == mainWindowInfo)
	{
		throw new CoverDefValidateException("The cover has no information about main window.");
	}
}
