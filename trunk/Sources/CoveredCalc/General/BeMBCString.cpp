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
	@file		BeMBCString.cpp
	@brief		Implementation of BeMBCString class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.2.14 created
*/

#include "Prefix.h"
#include <string.h>
#include "BeMBCString.h"

// ---------------------------------------------------------------------
//! Compares with the specified string.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 BeMBCString::Compare(ConstAStr string) const
{
	return strcmp(innerString.c_str(), string);
}

// ---------------------------------------------------------------------
//! Compares with the specified string.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 BeMBCString::Compare(const BeMBCString& other) const
{
	return strcmp(innerString.c_str(), other.innerString.c_str());
}

// ---------------------------------------------------------------------
//! Compares with the specified string, the comparison is done in small case.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 BeMBCString::CompareNoCase(ConstAStr string) const
{
	return strcasecmp(innerString.c_str(), string);
}

// ---------------------------------------------------------------------
//! Compares with the specified string, the comparison is done in small case.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 BeMBCString::CompareNoCase(const BeMBCString& other) const
{
	return strcasecmp(innerString.c_str(), other.innerString.c_str());
}

// ---------------------------------------------------------------------
//! Searches specified character in this string.
/*!
	@retval -1	not found.
	@retval >=0	index of found character.
*/
// ---------------------------------------------------------------------
SInt32 BeMBCString::Find(AChar character, SInt32 startIndex /* = 0 */) const
{
	const char* target = innerString.c_str() + startIndex;
	char* found = strchr(target, character);
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
SInt32 BeMBCString::Find(ConstAStr string, SInt32 startIndex /* = 0 */) const
{
	const char* target = innerString.c_str() + startIndex;
	char* found = strstr(target, string);
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
SInt32 BeMBCString::Find(const BeMBCString& other, SInt32 startIndex /* = 0 */) const
{
	const char* target = innerString.c_str() + startIndex;
	char* found = strstr(target, other.innerString.c_str());
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
SInt32 BeMBCString::RFind(AChar character) const
{
	const char* target = innerString.c_str();
	char* found = strrchr(target, character);
	if (NULL == found)
	{
		return -1;
	}
	else
	{
		return found - target;
	}	
}
