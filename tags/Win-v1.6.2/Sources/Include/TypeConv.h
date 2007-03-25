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
	@file		TypeConv.h
	@brief		Definition of Type Conversion.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.6.28 created
*/

#ifndef _TYPECONV_H_
#define _TYPECONV_H_

// ---------------------------------------------------------------------
//! Type Conversions
// ---------------------------------------------------------------------
namespace TypeConv
{
	// -----------------------------------------------------------------
	// AChar/AStr to UTF8Char/UTF8Str
	// -----------------------------------------------------------------
	inline UTF8Char AsUTF8(AChar character)
	{
		return static_cast<UTF8Char>(character);
	}

	inline UTF8Str AsUTF8(AStr string)
	{
		return reinterpret_cast<UTF8Str>(string);
	}

	inline ConstUTF8Str AsUTF8(ConstAStr string)
	{
		return reinterpret_cast<ConstUTF8Str>(string);
	}

	// -----------------------------------------------------------------
	// UTF8Char/UTF8Str to AChar/AStr 
	// -----------------------------------------------------------------
	inline AChar AsASCII(UTF8Char character)
	{
		return static_cast<AChar>(character);
	}
		
	inline AStr AsASCII(UTF8Str string)
	{
		return reinterpret_cast<AStr>(string);
	}
	
	inline ConstAStr AsASCII(ConstUTF8Str string)
	{
		return reinterpret_cast<ConstAStr>(string);
	}
}

#endif // _TYPECONV_H_
