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

/**
	@file		LangFileException.h
	@brief		Definition of LangFileException
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.1.9 created
*/

#ifndef _LANGFILEEXCEPTION_H_
#define _LANGFILEEXCEPTION_H_

#include "Path.h"
#include "Exception.h"
#include "MBCString.h"

/**
 *	@brief	This exception is thrown when processing a language file.
 */
class LangFileException : public Exception
{
public:
						LangFileException() : Exception() {}
						LangFileException(Exception* inner) : Exception(inner) {}
	virtual				~LangFileException() {}
};

namespace LangFileExceptions
{
	/**
	 *	@brief	Thrown when failed to load a language file.
	 */
	class LoadFailed : public LangFileException
	{
	public:
							LoadFailed(const Path& fileName) : LangFileException() { this->fileName = fileName; }
							LoadFailed(const Path& fileName, AStr message) : LangFileException() { this->fileName = fileName; this->message = message; }
		virtual				~LoadFailed() { }

		virtual	ConstAStr	GetErrorMessage() const
								{ return "Failed to load a language file."; }

		const Path&			GetFileName() const	{ return fileName; }
		ConstAStr			GetMessage() const { return message; }

	private:
		Path				fileName;
		MBCString			message;
	};
}

#endif // _LANGFILEEXCEPTION_H_
