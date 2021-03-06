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
	@file		FileException.cpp
	@brief		Implementation of FileException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.25 created
*/

#include "Prefix.h"
#include <string.h>
#include "FileException.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
FileException::FileException() : Exception()
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
FileException::FileException(
	const Path& fileName		//!< target filename
) : Exception()
{
	this->fileName = fileName;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
FileException::FileException(
	Exception* inner		//!< inner exception
) : Exception(inner)
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
FileException::FileException(
	const Path& fileName,		//!< target filename
	Exception* inner		//!< inner exception
) : Exception(inner)
{
	this->fileName = fileName;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
FileException::~FileException()
{
}

// ---------------------------------------------------------------------
//! Returns the target filename.
/*!
	@return Target filename or empty string if target is not found
*/
// ---------------------------------------------------------------------
const Path& FileException::GetFileName() const
{
	return fileName;
}
