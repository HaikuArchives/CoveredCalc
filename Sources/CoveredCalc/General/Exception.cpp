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
	@file		Exception.cpp
	@brief		Implemantation of Exception class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.8.24 created
*/

#include "Prefix.h"
#include "Exception.h"

//! default error message
static ConstAStr TheDefaultErrorMessage = ALITERAL("Error has occured.");

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
Exception::Exception()
{
	innerException = NULL;
}

// ---------------------------------------------------------------------
//! Constructor with inner exception
// ---------------------------------------------------------------------
Exception::Exception(
	Exception* inner	//!< inner exception
)
{
	innerException = inner;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
Exception::~Exception()
{
	if (NULL != innerException)
	{
		innerException->Delete();
	}
}

// ---------------------------------------------------------------------
//! Returns the errror message of this exception.
/*!
	@return error message.
	@note
	The returned pointer can be temporary buffer.
	Caller must not save this pointer.
*/
// ---------------------------------------------------------------------
ConstAStr Exception::GetErrorMessage() const
{
	return TheDefaultErrorMessage;
}

// ---------------------------------------------------------------------
//! Delete this object.
/*!
	@return error message.
	@note
	Do not use delete operator to delete this object. Use this method instead.
	After calling this method, caller must not access this class anymore.
*/
// ---------------------------------------------------------------------
void Exception::Delete()
{
	delete this;
}
