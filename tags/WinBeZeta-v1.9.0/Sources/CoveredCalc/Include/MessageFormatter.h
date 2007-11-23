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
	@file		MessageFormatter.h
	@brief		Definition of MessageFormatter functions
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.01.17 created
*/

#ifndef _MESSAGEFORMATTER_H_
#define _MESSAGEFORMATTER_H_

#include <stdio.h>
#include "MBCString.h"
#include "Exception.h"

// ---------------------------------------------------------------------
//! Thrown when MessageFormatter::Format() is failed.
// ---------------------------------------------------------------------
class MessageFormatException : public Exception
{
public:
						MessageFormatException(ConstAStr _format) : format(_format) { }
	virtual				~MessageFormatException() { }
	
	virtual	ConstAStr	GetErrorMessage() const
							{ return "Message format error."; }
	virtual ConstAStr	GetErrorFormat() const
							{ return format; }

private:
	MBCString			format;
};

namespace MessageFormatter
{
	void Format(MBCString& message, ConstAStr format, ...);
	void FormatV(MBCString& message, ConstAStr format, va_list args);
}

#endif // _MESSAGEFORMATTER_H_
