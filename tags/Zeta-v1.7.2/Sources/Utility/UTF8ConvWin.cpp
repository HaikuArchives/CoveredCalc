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
	@file		UTF8ConvWin.cpp
	@brief		Convert utility for UTF-8 for Windows.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.09 created
*/

#include "Prefix.h"
#include "MemoryException.h"
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
	WCHAR wideBuf[256];				// to avoid allocating from heap when needed buffer is small.
	LPWSTR wideStr = wideBuf;
	AStr result = NULL;
	
	try
	{
		int wideCharCount = ::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(utf8Str), -1, NULL, 0);
		if (wideCharCount > sizeof(wideBuf)/sizeof(WCHAR))
		{
			wideStr = static_cast<LPWSTR>(malloc(wideCharCount * sizeof(WCHAR)));
			if (NULL == wideStr)
			{
				MemoryException::Throw();
			}
		}
		::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(utf8Str), -1, wideStr, wideCharCount);

		int mbCharCount = ::WideCharToMultiByte(CP_OEMCP, 0, wideStr, wideCharCount, NULL, 0, NULL, NULL);
		result = static_cast<AStr>(malloc(mbCharCount));
		if (NULL == result)
		{
			MemoryException::Throw();
		}
		::WideCharToMultiByte(CP_OEMCP, 0, wideStr, wideCharCount, result, mbCharCount, NULL, NULL);
	}
	catch (...)
	{
		if (NULL != wideStr && wideStr != wideBuf)
		{
			free(wideStr);
		}
		if (NULL != result)
		{
			free(result);
		}
		throw;
	}

	if (NULL != wideStr && wideStr != wideBuf)
	{
		free(wideStr);
	}
	
	return result;
}

// ---------------------------------------------------------------------
//! Converts UTF-8 string to OS specific multi byte characters
// ---------------------------------------------------------------------
void UTF8Conv::ToMultiByte(
	MBCString& mbcString,		//!< OUTPUT. coverted string
	ConstUTF8Str utf8Str		//!< source UTF-8 string
)
{
	AStr result = ToMultiByte(utf8Str);
	mbcString = result;
	free(result);
}

// ---------------------------------------------------------------------
//! Converts UTF-8 string to OS specific multi byte characters in consideration of line ending character.
/*!
	This function suppose that the line ending character of input UTF-8 string is LF
	and the output multi byte string is OS specific char. i.e. CF-LF on Windows.
*/
// ---------------------------------------------------------------------
void UTF8Conv::ToMultiByteWithLineEnding(
	MBCString& mbcString,		//!< OUTPUT. coverted string
	ConstUTF8Str utf8Str		//!< source UTF-8 string
)
{
	// 必要なバッファのサイズを数える。
	int bufSize = 0;
	const UTF8Char* cur;
	for (cur=utf8Str; '\0'!=*cur; cur++)
	{
		if ('\x0a' == *cur)
		{
			bufSize++;		// CR が増える分
		}
		bufSize++;
	}
	bufSize++;		// '\0' の分
	
	// LF の数だけ増やして確保
	UTF8Str crlfUTF8Str = static_cast<UTF8Str>(malloc(bufSize));
	try
	{
		// LF → CR-LF にコンバート
		UTF8Char* bufCur = crlfUTF8Str;
		for (cur=utf8Str; '\0'!=*cur; cur++)
		{
			if ('\x0a' == *cur)
			{
				*bufCur = '\x0d';
				bufCur++;
			}
			*bufCur = *cur;
			bufCur++;
		}
		*bufCur = '\0';
		
		AStr result = ToMultiByte(crlfUTF8Str);
		mbcString = result;
		free(result);
	}
	catch (...)
	{
		free(crlfUTF8Str);
		throw;
	}
	free(crlfUTF8Str);	
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
	WCHAR wideBuf[256];				// to avoid allocating from heap when needed buffer is small.
	LPWSTR wideStr = wideBuf;
	UTF8Str result = NULL;
	
	try
	{
		int wideCharCount = ::MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, reinterpret_cast<LPCSTR>(mbcStr), -1, NULL, 0);
		if (wideCharCount > sizeof(wideBuf)/sizeof(WCHAR))
		{
			wideStr = static_cast<LPWSTR>(malloc(wideCharCount * sizeof(WCHAR)));
			if (NULL == wideStr)
			{
				MemoryException::Throw();
			}
		}
		::MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, reinterpret_cast<LPCSTR>(mbcStr), -1, wideStr, wideCharCount);

		int utf8CharCount = ::WideCharToMultiByte(CP_UTF8, 0, wideStr, wideCharCount, NULL, 0, NULL, NULL);
		result = static_cast<UTF8Str>(malloc(utf8CharCount));
		if (NULL == result)
		{
			MemoryException::Throw();
		}
		::WideCharToMultiByte(CP_UTF8, 0, wideStr, wideCharCount, reinterpret_cast<LPSTR>(result), utf8CharCount, NULL, NULL);
	}
	catch (...)
	{
		if (NULL != wideStr && wideStr != wideBuf)
		{
			free(wideStr);
		}
		if (NULL != result)
		{
			free(result);
		}
		throw;
	}

	if (NULL != wideStr && wideStr != wideBuf)
	{
		free(wideStr);
	}
	
	return result;
}

// ---------------------------------------------------------------------
//! Converts UTF-8 string to OS specific multi byte characters
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

