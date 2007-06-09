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
	@file		CoverDefParseException.cpp
	@brief		Implementation of CoverDefParseException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.02 created
*/

#include "Prefix.h"
#include "CoverDefParseException.h"
#include "CoverDefValidateException.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::UnknownID::UnknownID(ConstUTF8Str id)
{
	this->id = id;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::UnknownID::~UnknownID()
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::UnknownClass::UnknownClass(ConstUTF8Str classAttr)
{
	this->classAttr = classAttr;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::UnknownClass::~UnknownClass()
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::UnknownType::UnknownType(ConstUTF8Str typeAttr)
{
	this->typeAttr = typeAttr;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::UnknownType::~UnknownType()
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::ValidationFailed::ValidationFailed(
	CoverDefValidateException* inner		//!< CoverDefValidateException object
)
	: CoverDefParseException(inner->GetErrorMessage(), inner)
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::ValidationFailed::ValidationFailed(
	SInt32 line,						//!< the line which error has occured.
	SInt32 column,						//!< the column in the line which error has occured.
	CoverDefValidateException* inner	//!< CoverDefValidateException object
)
	: CoverDefParseException(line, column, inner->GetErrorMessage(), inner)
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::ColorStringNotUnderstood::ColorStringNotUnderstood(
	ConstUTF8Str colorString		//!< color string which is not understood
)
{
	this->colorString = colorString;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::ColorStringNotUnderstood::~ColorStringNotUnderstood()
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::UnsupportedCoverDefVersion::UnsupportedCoverDefVersion(
	ConstUTF8Str versionString		//!< version string which is unsupported
)
{
	this->versionString = versionString;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverDefParseExceptions::UnsupportedCoverDefVersion::~UnsupportedCoverDefVersion()
{
}
