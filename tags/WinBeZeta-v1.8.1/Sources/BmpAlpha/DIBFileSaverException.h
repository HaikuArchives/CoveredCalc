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
	@file		DIBFileSaverException.h
	@brief		Definition of DIBFileSaverException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.5.5 created
*/

#ifndef _DIBFILESAVEREXCEPTION_H_
#define _DIBFILESAVEREXCEPTION_H_

#include "FileException.h"

// ---------------------------------------------------------------------
//! The exception object which is thrown by DIBFileSaver.
// ---------------------------------------------------------------------
class DIBFileSaverException : public FileException
{
public:
						DIBFileSaverException()											: FileException() { }
						DIBFileSaverException(const Path& fileName)						: FileException(fileName) { }
						DIBFileSaverException(Exception* inner)							: FileException(inner) { }
						DIBFileSaverException(const Path& fileName, Exception* inner)	: FileException(fileName, inner) { }

	virtual				~DIBFileSaverException() { }
};

namespace DIBFileSaverExceptions
{
	// ---------------------------------------------------------------------
	//! Thrown if the type of the DIBitmap object is unsupported.
	// ---------------------------------------------------------------------
	class UnsupportedDIB : public DIBFileSaverException
	{
	public:
							UnsupportedDIB()											: DIBFileSaverException() { }
							UnsupportedDIB(const Path& fileName)						: DIBFileSaverException(fileName) { }
							UnsupportedDIB(Exception* inner)							: DIBFileSaverException(inner) { }
							UnsupportedDIB(const Path& fileName, Exception* inner)		: DIBFileSaverException(fileName, inner) { }
		virtual				~UnsupportedDIB() { }
	};
}

#endif // _DIBFILESAVEREXCEPTION_H_
