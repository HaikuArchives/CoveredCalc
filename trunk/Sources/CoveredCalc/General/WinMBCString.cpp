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
	@file		WinMBCString.cpp
	@brief		Implementation of WinMBCString class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.1.17 created
*/

#include "Prefix.h"
#include <mbstring.h>
#include "WinMBCString.h"

// ---------------------------------------------------------------------
//! Compares with the specified string.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 WinMBCString::Compare(ConstAStr string) const
{
	return _mbscmp(reinterpret_cast<const unsigned char*>(innerString.c_str()), reinterpret_cast<const unsigned char*>(string));
}

// ---------------------------------------------------------------------
//! Compares with the specified string.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 WinMBCString::Compare(const WinMBCString& other) const
{
	return _mbscmp(reinterpret_cast<const unsigned char*>(innerString.c_str()), reinterpret_cast<const unsigned char*>(other.innerString.c_str()));
}

// ---------------------------------------------------------------------
//! Compares with the specified string, the comparison is done in small case.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 WinMBCString::CompareNoCase(ConstAStr string) const
{
	return _mbsicmp(reinterpret_cast<const unsigned char*>(innerString.c_str()), reinterpret_cast<const unsigned char*>(string));
}

// ---------------------------------------------------------------------
//! Compares with the specified string, the comparison is done in small case.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 WinMBCString::CompareNoCase(const WinMBCString& other) const
{
	return _mbsicmp(reinterpret_cast<const unsigned char*>(innerString.c_str()), reinterpret_cast<const unsigned char*>(other.innerString.c_str()));
}

// ---------------------------------------------------------------------
//! Searches specified character in this string.
/*!
	@retval -1	not found.
	@retval >=0	index of found character.
*/
// ---------------------------------------------------------------------
SInt32 WinMBCString::Find(AChar character, SInt32 startIndex /* = 0 */) const
{
	const unsigned char* target = reinterpret_cast<const unsigned char*>(innerString.c_str() + startIndex);
	const unsigned char* found = _mbschr(target, static_cast<unsigned char>(character));
	if (NULL == found)
	{
		return -1;
	}
	else
	{
		return found - target + startIndex;
	}
}

// ---------------------------------------------------------------------
//! Searches specified string in this string.
/*!
	@retval -1	not found.
	@retval >=0	index of first character of found string.
*/
// ---------------------------------------------------------------------
SInt32 WinMBCString::Find(ConstAStr string, SInt32 startIndex /* = 0 */) const
{
	const unsigned char* target = reinterpret_cast<const unsigned char*>(innerString.c_str() + startIndex);
	const unsigned char* found = _mbsstr(target, reinterpret_cast<const unsigned char*>(string));
	if (NULL == found)
	{
		return -1;
	}
	else
	{
		return found - target + startIndex;
	}
}

// ---------------------------------------------------------------------
//! Searches specified string in this string.
/*!
	@retval -1	not found.
	@retval >=0	index of first character of found string.
*/
// ---------------------------------------------------------------------
SInt32 WinMBCString::Find(const WinMBCString& other, SInt32 startIndex /* = 0 */) const
{
	const unsigned char* target = reinterpret_cast<const unsigned char*>(innerString.c_str() + startIndex);
	const unsigned char* found = _mbsstr(target, reinterpret_cast<const unsigned char*>(other.innerString.c_str()));
	if (NULL == found)
	{
		return -1;
	}
	else
	{
		return found - target + startIndex;
	}
}

// ---------------------------------------------------------------------
//! Searches last specified character in this string.
/*!
	@retval -1	not found.
	@retval >=0	index of found character.
*/
// ---------------------------------------------------------------------
SInt32 WinMBCString::RFind(AChar character) const
{
	const unsigned char* target = reinterpret_cast<const unsigned char*>(innerString.c_str());
	const unsigned char* found = _mbsrchr(target, static_cast<unsigned char>(character));
	if (NULL == found)
	{
		return -1;
	}
	else
	{
		return found - target;
	}	
}
