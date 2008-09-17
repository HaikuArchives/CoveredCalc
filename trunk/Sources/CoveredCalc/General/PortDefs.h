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
	@file		PortDefs.h
	@brief		Definition for easy porting.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.6.28 created
*/

#ifndef _PORTDEFS_H_
#define _PORTDEFS_H_

// ---------------------------------------------------------------------
// Generic types
// ---------------------------------------------------------------------
typedef signed char			SInt8;		//!< signed 8bit integer
typedef unsigned char		UInt8;		//!< unsigned 8bit integer

typedef signed short		SInt16;		//!< signed 16bit integer
typedef unsigned short		UInt16;		//!< unsigned 16bit integer

typedef signed long			SInt32;		//!< signed 32bit integer
typedef unsigned long		UInt32;		//!< unsigned 32bit integer

typedef float				Float32;	//!< 32bit IEEE float
typedef double				Float64;	//!< 64bit IEEE float

typedef unsigned char		Byte;		//!< for byte sequence buffer

typedef char				SChar;		//!< single byte character
typedef SChar*				SStr;		//!< string which consists of single byte characters
typedef const SChar*		ConstSStr;	//!< read-only string consists of single byte characters

#if defined (WIN32)
typedef TCHAR				AChar;		//!< native encode character
#else
typedef char				AChar;		//!< native encode character
#endif
typedef AChar*				AStr;		//!< string which consists of native encode characters
typedef const AChar*		ConstAStr;	//!< read-only string consists of native encode characters

typedef unsigned char		UTF8Char;	//!< UTF-8 character
typedef UTF8Char*			UTF8Str;	//!< string which consists of UTF-8 characeters
typedef const UTF8Char*		ConstUTF8Str;	//!< read-only string consists of UTF-8 characeters

// ---------------------------------------------------------------------
// Types for coordinates
// ---------------------------------------------------------------------
//! point which consists of SInt32 coordinates
typedef struct {
	SInt32	x;
	SInt32	y;
} Point32;

//! rectangle which consists of SInt32 coordinates
typedef struct {
	SInt32	left;
	SInt32	top;
	SInt32	right;
	SInt32	bottom;
} Rect32;

// ---------------------------------------------------------------------
// Macros
// ---------------------------------------------------------------------
#if defined (WIN32)
#define strcasecmp		_stricmp
#define snprintf		_snprintf
#define vsnprintf		_vsnprintf

#define stscanf			_stscanf
#define tcslen			_tcslen
#define tcschr			_tcschr
#define ttoi			_ttoi
#define sntprintf		_sntprintf
#define vsntprintf		_vsntprintf
#endif

#if defined (BEOS)
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 

#define stscanf			sscanf
#define tcslen			strlen
#define tcschr			strchr
#define ttoi			atoi
#define sntprintf		snprintf
#define vsntprintf		vsnprintf
#endif

#if defined (WIN32)
#define ALITERAL(x)	_T(x)
#else
#define ALITERAL(x)	x
#endif

#endif // _PORTDEFS_H_
