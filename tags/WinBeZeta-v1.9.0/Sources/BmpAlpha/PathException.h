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
	@file		PathException.h
	@brief		Definition of PathExceptions namespace classes
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.2 created
*/

#ifndef _PATHEXCEPTION_H_
#define _PATHEXCEPTION_H_

#include "Exception.h"
#include "MBCString.h"

// ---------------------------------------------------------------------
//! The exception object about file operation.
// ---------------------------------------------------------------------
class PathException : public Exception
{
public:
						PathException() : Exception() { }
						PathException(Exception* inner) : Exception(inner) { }
	virtual				~PathException() { }
};

namespace PathExceptions
{
	// ---------------------------------------------------------------------
	//! Thrown when relative path is not exist
	// ---------------------------------------------------------------------
	class RelativePathNotExist : public PathException
	{
	public:
							RelativePathNotExist(ConstAStr base, ConstAStr target)
								{ this->base = base; this->target = target; }
		ConstAStr			GetBase() const { return base; }
		ConstAStr			GetTarget() const { return target; }
	
	private:
		MBCString			base;			//!< base path
		MBCString			target;			//!< target path
	};	
}

#endif // _PATHEXCEPTION_H_
