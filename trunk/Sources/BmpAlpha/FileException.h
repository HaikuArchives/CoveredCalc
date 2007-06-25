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
	@file		FileException.h
	@brief		Definition of FileException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.25 created
*/

#ifndef _FILEEXCEPTION_H_
#define _FILEEXCEPTION_H_

#include "Path.h"
#include "Exception.h"

// ---------------------------------------------------------------------
//! The exception object about file operation.
// ---------------------------------------------------------------------
class FileException : public Exception
{
public:
						FileException();
						FileException(const Path& fileName);
						FileException(Exception* inner);
						FileException(const Path& fileName, Exception* inner);
	virtual				~FileException();
	
	virtual const Path&	GetFileName() const;

private:
	Path				fileName;
};

namespace FileExceptions
{
	class FileNotFound : public FileException
	{
	public:
							FileNotFound() : FileException() { }
							FileNotFound(const Path& fileName) : FileException(fileName) { }
							FileNotFound(Exception* inner) : FileException(inner) { }
							FileNotFound(const Path& fileName, Exception* inner) : FileException(fileName, inner) { }
		virtual				~FileNotFound() { }
	};
	
	class FileAlreadyExists : public FileException
	{
	public:
							FileAlreadyExists() : FileException() { }
							FileAlreadyExists(const Path& fileName) : FileException(fileName) { }
							FileAlreadyExists(Exception* inner) : FileException(inner) { }
							FileAlreadyExists(const Path& fileName, Exception* inner) : FileException(fileName, inner) { }
		virtual				~FileAlreadyExists() { }
	};
	
	class SharingViolation : public FileException
	{
	public:
							SharingViolation() : FileException() { }
							SharingViolation(const Path& fileName) : FileException(fileName) { }
							SharingViolation(Exception* inner) : FileException(inner) { }
							SharingViolation(const Path& fileName, Exception* inner) : FileException(fileName, inner) { }
		virtual				~SharingViolation() { }
	};
	
	class AccessDenied : public FileException
	{
	public:
							AccessDenied() : FileException() { }
							AccessDenied(const Path& fileName) : FileException(fileName) { }
							AccessDenied(Exception* inner) : FileException(inner) { }
							AccessDenied(const Path& fileName, Exception* inner) : FileException(fileName, inner) { }
		virtual				~AccessDenied() { }
	};
	
	class DeviceFull : public FileException
	{
	public:
							DeviceFull() : FileException() { }
							DeviceFull(const Path& fileName) : FileException(fileName) { }
							DeviceFull(Exception* inner) : FileException(inner) { }
							DeviceFull(const Path& fileName, Exception* inner) : FileException(fileName, inner) { }
		virtual				~DeviceFull() { }
	};
	
	class IOError : public FileException
	{
	public:
							IOError() : FileException() { }
							IOError(const Path&fileName) : FileException(fileName) { }
							IOError(Exception* inner) : FileException(inner) { }
							IOError(const Path& fileName, Exception* inner) : FileException(fileName, inner) { }
		virtual				~IOError() { }
	};
}

#endif // _FILEEXCEPTION_H_
