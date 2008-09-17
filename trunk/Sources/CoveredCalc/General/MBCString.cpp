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
	@file		MBCString.cpp
	@brief		Implementation of MBCString class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.1.17 created
*/

#include "Prefix.h"
#include "MBCString.h"

// ---------------------------------------------------------------------
//! Compares with the specified string.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 MBCString::Compare(ConstAStr string) const
{
#if defined (WIN32)
	return _tcscmp(innerString.c_str(), string);
#else
	return innerString.compare(string);
#endif
}

// ---------------------------------------------------------------------
//! Compares with the specified string.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 MBCString::Compare(const MBCString& other) const
{
#if defined (WIN32)
	return _tcscmp(innerString.c_str(), other.innerString.c_str());
#else
	return innerString.compare(other.innerString);
#endif
}

// ---------------------------------------------------------------------
//! Compares with the specified string, the comparison is done in small case.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 MBCString::CompareNoCase(ConstAStr string) const
{
#if defined (WIN32)
	return _tcsicmp(innerString.c_str(), string);
#else
	return strcasecmp(innerString.c_str(), string);
#endif
}

// ---------------------------------------------------------------------
//! Compares with the specified string, the comparison is done in small case.
/*!
	@retval <0	this string is less than specified string
	@retval 0	this string equals to specified string
	@retval >0	this string is greater than specified string
*/
// ---------------------------------------------------------------------
SInt32 MBCString::CompareNoCase(const MBCString& other) const
{
#if defined (WIN32)
	return _tcsicmp(innerString.c_str(), other.innerString.c_str());
#else
	return strcasecmp(innerString.c_str(), other.innerString.c_str());
#endif
}

// ---------------------------------------------------------------------
//! Searches specified character in this string.
/*!
	@retval -1	not found.
	@retval >=0	index of found character.
*/
// ---------------------------------------------------------------------
SInt32 MBCString::Find(AChar character, SInt32 startIndex /* = 0 */) const
{
#if defined (WIN32)
	const TCHAR* target = innerString.c_str() + startIndex;
	const TCHAR* found = _tcschr(target, character);
	if (NULL == found)
	{
		return -1;
	}
	else
	{
		return static_cast<SInt32>(found - target + startIndex);
	}
#else
	string::size_type ret = innerString.find(character, startIndex);
	if (string::npos == ret)
	{
		return -1;
	}
	else
	{
		return ret;
	}
#endif
}

// ---------------------------------------------------------------------
//! Searches specified string in this string.
/*!
	@retval -1	not found.
	@retval >=0	index of first character of found string.
*/
// ---------------------------------------------------------------------
SInt32 MBCString::Find(ConstAStr string, SInt32 startIndex /* = 0 */) const
{
#if defined (WIN32)
	const TCHAR* target = innerString.c_str() + startIndex;
	const TCHAR* found = _tcsstr(target, string);
	if (NULL == found)
	{
		return -1;
	}
	else
	{
		return static_cast<SInt32>(found - target + startIndex);
	}
#else
	string::size_type ret = innerString.find(string, startIndex);
	if (string::npos == ret)
	{
		return -1;
	}
	else
	{
		return ret;
	}
#endif
}

// ---------------------------------------------------------------------
//! Searches specified string in this string.
/*!
	@retval -1	not found.
	@retval >=0	index of first character of found string.
*/
// ---------------------------------------------------------------------
SInt32 MBCString::Find(const MBCString& other, SInt32 startIndex /* = 0 */) const
{
#if defined (WIN32)
	const TCHAR* target = innerString.c_str() + startIndex;
	const TCHAR* found = _tcsstr(target, other.innerString.c_str());
	if (NULL == found)
	{
		return -1;
	}
	else
	{
		return static_cast<SInt32>(found - target + startIndex);
	}
#else
	string::size_type ret = innerString.find(other.innerString, startIndex);
	if (string::npos == ret)
	{
		return -1;
	}
	else
	{
		return ret;
	}
#endif
}

// ---------------------------------------------------------------------
//! Searches last specified character in this string.
/*!
	@retval -1	not found.
	@retval >=0	index of found character.
*/
// ---------------------------------------------------------------------
SInt32 MBCString::RFind(AChar character) const
{
#if defined (WIN32)
	const TCHAR* target = innerString.c_str();
	const TCHAR* found = _tcsrchr(target, character);
	if (NULL == found)
	{
		return -1;
	}
	else
	{
		return static_cast<SInt32>(found - target);
	}	
#else
	string::size_type ret = innerString.rfind(character);
	if (string::npos == ret)
	{
		return -1;
	}
	else
	{
		return ret;
	}
#endif
}
