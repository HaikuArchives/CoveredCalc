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
	@file		DebugSupport.h
	@brief		Support macros, functions and so on for debugging.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.6.27 created
*/

#ifndef _DEBUGSUPPORT_H_
#define _DEBUGSUPPORT_H_

#if defined(WIN32)	// ------ for Windows ------------------------------

// defines DEBUG if _DEBUG is defined
#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

// for easy finding of memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// defines ASSERT()
#ifndef ASSERT
#define ASSERT(x) _ASSERT(x)
#endif

#elif defined(BEOS)	// ------ for BeOS ---------------------------------

#include <support/Debug.h>

#endif

#endif // _DEBUGSUPPORT_H_
