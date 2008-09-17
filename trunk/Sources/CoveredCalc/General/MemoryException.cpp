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
	@file		MemoryException.cpp
	@brief		Implemantation of MemopryException class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.20 created
*/

#include "Prefix.h"
#include "MemoryException.h"

//! error message for MemoryException.
static ConstAStr TheMemoryErrorMessage = ALITERAL("Out of memory.");

// ---------------------------------------------------------------------
//! The only instance of memory exception
/*!
	The only instance of memory exception prepared in advance to avoid
	memory error when throwing this object, because this exception will
	be thrown at out of memory.
*/
// ---------------------------------------------------------------------
MemoryException MemoryException::theOnlyInstance;

// ---------------------------------------------------------------------
//! Returns the error message of this exception.
/*!
	@return error message.
	@note
	The returned pointer can be temporary buffer.
	Caller must not save this pointer.
*/
// ---------------------------------------------------------------------
ConstAStr MemoryException::GetErrorMessage() const
{
	return TheMemoryErrorMessage;
}

// ---------------------------------------------------------------------
//! Delete this object.
/*!
	Do nothing because there is the only one instance
	and it will be reused next time.
*/
// ---------------------------------------------------------------------
void MemoryException::Delete()
{
	// do nothing.
}
