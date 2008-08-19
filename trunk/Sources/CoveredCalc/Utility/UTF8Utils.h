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
	@file		UTF8Utils.h
	@brief		Utility functions for UTF-8.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.26 created
*/

#ifndef _UTF8UTILS_H_
#define _UTF8UTILS_H_

#include "TypeConv.h"
#include <string.h>

namespace UTF8Utils
{	
	// ---------------------------------------------------------------------
	//! UTF-8 version of strcmp()
	/*!
		@return reseult
		@note The string is compared as ASCII character.
	*/
	// ---------------------------------------------------------------------
	inline SInt32 UTF8StrCmp(
		ConstUTF8Str string1,		//!< one string
		ConstUTF8Str string2		//!< the other string
	)
	{
		return strcmp(TypeConv::AsASCII(string1), TypeConv::AsASCII(string2));
	}
	
	// ---------------------------------------------------------------------
	//! UTF-8 version of strcmpi()
	/*!
		@return reseult
		@note The string is compared as ASCII character.
	*/
	// ---------------------------------------------------------------------
	inline SInt32 UTF8StrCmpI(
		ConstUTF8Str string1,	//!< one string
		ConstUTF8Str string2	//!< the other string
	)
	{
		return strcasecmp(TypeConv::AsASCII(string1), TypeConv::AsASCII(string2));
	}
}

#endif // _UTF8UTILS_H_
