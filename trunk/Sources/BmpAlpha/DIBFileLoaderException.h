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
	@file		DIBFileLoaderException.h
	@brief		Definition of DIBFileLoaderException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.27 created
*/

#ifndef _DIBFILELOADEREXCEPTION_H_
#define _DIBFILELOADEREXCEPTION_H_

#include "FileException.h"

// ---------------------------------------------------------------------
//! The exception object which is thrown by DIBFileLoader.
// ---------------------------------------------------------------------
class DIBFileLoaderException : public FileException
{
public:
						DIBFileLoaderException()										: FileException() { }
						DIBFileLoaderException(const Path& fileName)					: FileException(fileName) { }
						DIBFileLoaderException(Exception* inner)						: FileException(inner) { }
						DIBFileLoaderException(const Path& fileName, Exception* inner)	: FileException(fileName, inner) { }

	virtual				~DIBFileLoaderException() { }
};

namespace DIBFileLoaderExceptions
{
	// ---------------------------------------------------------------------
	//! Thrown if the data file format is unknown.
	// ---------------------------------------------------------------------
	class UnknownFileFormat : public DIBFileLoaderException
	{
	public:
							UnknownFileFormat()											: DIBFileLoaderException() { }
							UnknownFileFormat(const Path& fileName)						: DIBFileLoaderException(fileName) { }
							UnknownFileFormat(Exception* inner)							: DIBFileLoaderException(inner) { }
							UnknownFileFormat(const Path& fileName, Exception* inner)	: DIBFileLoaderException(fileName, inner) { }
		virtual				~UnknownFileFormat() { }
	};

	// ---------------------------------------------------------------------
	//! Thrown if the data file is broken.
	// ---------------------------------------------------------------------
	class BrokenFile : public DIBFileLoaderException
	{
	public:
							BrokenFile()											: DIBFileLoaderException() { }
							BrokenFile(const Path& fileName)						: DIBFileLoaderException(fileName) { }
							BrokenFile(Exception* inner)							: DIBFileLoaderException(inner) { }
							BrokenFile(const Path& fileName, Exception* inner)		: DIBFileLoaderException(fileName, inner) { }
		virtual				~BrokenFile() { }
	};
}

#endif // _DIBFILELOADEREXCEPTION_H_
