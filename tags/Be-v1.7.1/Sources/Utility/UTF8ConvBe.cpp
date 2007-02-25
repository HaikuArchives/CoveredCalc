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
	@file		UTF8ConvBe.cpp
	@brief		Convert utility for UTF-8 for BeOS.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.22 created
*/

#include "Prefix.h"
#include "MemoryException.h"
#include "TypeConv.h"
#include "UTF8Conv.h"

// ---------------------------------------------------------------------
//! Converts UTF-8 string to OS specific multi byte characters
/*!
	@return converted string
	@note
	The returned buffer is allocated by malloc().
	Caller must free it by free().	
*/
// ---------------------------------------------------------------------
AStr UTF8Conv::ToMultiByte(
	ConstUTF8Str utf8Str		//!< source UTF-8 string
)
{
	AStr retPtr = strdup(TypeConv::AsASCII(utf8Str));
	if (NULL == retPtr)
	{
		MemoryException::Throw();
	}
	
	return retPtr;
}

// ---------------------------------------------------------------------
//! Converts UTF-8 string to OS specific multi byte characters
// ---------------------------------------------------------------------
void UTF8Conv::ToMultiByte(
	MBCString& mbcString,		//!< OUTPUT. coverted string
	ConstUTF8Str utf8Str		//!< source UTF-8 string
)
{
	mbcString = TypeConv::AsASCII(utf8Str);
}

// ---------------------------------------------------------------------
//! Converts UTF-8 string to OS specific multi byte characters in consideration of line ending character.
/*!
	This function suppose that the line ending character of input UTF-8 string is LF
	and the output multi byte string is OS specific char. i.e. also LF on BeOS.
*/
// ---------------------------------------------------------------------
void UTF8Conv::ToMultiByteWithLineEnding(
	MBCString& mbcString,		//!< OUTPUT. coverted string
	ConstUTF8Str utf8Str		//!< source UTF-8 string
)
{
	ToMultiByte(mbcString, utf8Str);
}

// ---------------------------------------------------------------------
//! Converts OS specific multi byte characters to UTF-8 string
/*!
	@return converted string
	@note
	The returned buffer is allocated by malloc().
	Caller must free it by free().	
*/
// ---------------------------------------------------------------------
UTF8Str UTF8Conv::FromMultiByte(
	ConstAStr mbcStr			//!< source multi byte string
)
{
	UTF8Str retPtr = TypeConv::AsUTF8(strdup(mbcStr));
	if (NULL == retPtr)
	{
		MemoryException::Throw();
	}
	
	return retPtr;
}

// ---------------------------------------------------------------------
//! Converts OS specific multi byte characters to UTF-8 string
// ---------------------------------------------------------------------
void UTF8Conv::FromMultiByte(
	UTF8String& utf8String,		//!< OUTPUT. coverted string
	ConstAStr mbcStr			//!< source multi byte string
)
{
	UTF8Str result = FromMultiByte(mbcStr);
	utf8String = result;
	free(result);
}

// ---------------------------------------------------------------------
//! Converts OS specific multi byte characters to UTF-8 string in consideration of line ending character.
/*!
	This function suppose that the line ending character of input multi byte string is LF on BeOS
	and the output UTF-8 string is also LF.
*/
// ---------------------------------------------------------------------
void UTF8Conv::FromMultiByteWithLineEnding(
	UTF8String& utf8String,		//!< OUTPUT. coverted string
	ConstAStr mbcStr			//!< source multi byte string
)
{
	FromMultiByte(utf8String, mbcStr);
}
