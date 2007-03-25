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
	@file		ColorCodedSkinException.h
	@brief		Definition of ColorCodedSkinException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.18 created
*/

#ifndef _COLORCODEDSKINEXCEPTION_H_
#define _COLORCODEDSKINEXCEPTION_H_

#include "Exception.h"

// ---------------------------------------------------------------------
//! The exception object which is thrown by ColorCodedSkin.
// ---------------------------------------------------------------------
class ColorCodedSkinException : public Exception
{
public:
						ColorCodedSkinException() : Exception() { }
						ColorCodedSkinException(Exception* inner) : Exception(inner) { }
	virtual				~ColorCodedSkinException() { }	
};

namespace ColorCodedSkinExceptions
{
	// ---------------------------------------------------------------------
	//! Thrown if the parameter of ColorCodedSkin::Init() is bad.
	// ---------------------------------------------------------------------
	class BadInitParams : public ColorCodedSkinException
	{
	public:
							BadInitParams() : ColorCodedSkinException() { }
							BadInitParams(Exception* inner) : ColorCodedSkinException(inner) { }
		virtual				~BadInitParams() { }
	};
}

#endif // _COLORCODEDSKINEXCEPTION_H_
