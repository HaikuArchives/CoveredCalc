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
	@file		CoverDef.cpp
	@brief		Implemantation of CoverDef class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.18 created
*/

#include "Prefix.h"
#include <string.h>
#include "CoverDef.h"
#include "CoverDefValidateException.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverDef::CoverDef()
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverDef::~CoverDef()
{
	std::vector<CoverInfo*>::iterator ite;
	for (ite=coverInfos.begin(); ite!=coverInfos.end(); ite++)
	{
		delete *ite;
	}
}

// ---------------------------------------------------------------------
//! Add an information about a cover.
/*!
	@note
	CoverDef object has responsibility to delete coverInfo.
	Caller must not delete it.
*/
// ---------------------------------------------------------------------
void CoverDef::AddCoverInfo(
	CoverInfo* coverInfo		//!< inforamtion
)
{
	coverInfos.push_back(coverInfo);
}

// ---------------------------------------------------------------------
//! Checks validation of data.
/*!
	@note
	This function is called after making this object has finished.
*/
// ---------------------------------------------------------------------
void CoverDef::Validate()
{
	if (0 >= coverInfos.size())
	{
		throw new CoverDefValidateException(ALITERAL("The cover definition has no information about cover(s)."));
	}
}
