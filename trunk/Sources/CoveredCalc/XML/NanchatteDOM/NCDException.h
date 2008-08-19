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
	@file		NCDException.h
	@brief		Definition of NCDException class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.01 created
*/

#ifndef _NCDEXCEPTION_H_
#define _NCDEXCEPTION_H_

#include "Exception.h"

// ---------------------------------------------------------------------
//! This exception is thrown in Nanchatte DOM operations.
// ---------------------------------------------------------------------
class NCDException : public Exception
{
public:
	enum ExceptionCode
	{
		INDEX_SIZE_ERR					= 1,
		DOMSTRING_SIZE_ERR				= 2,
		HIERARCHY_REQUEST_ERR			= 3,
		WRONG_DOCUMENT_ERR				= 4,
		INVALID_CHARACTER_ERR			= 5,
		NO_DATA_ALLOWED_ERR				= 6,
		NO_MODIFICATION_ALLOWED_ERR		= 7,
		NOT_FOUND_ERR					= 8,
		NOT_SUPPORTED_ERR				= 9,
		INUSE_ATTRIBUTE_ERR				= 10,
		INVALID_STATE_ERR				= 11,
		SYNTAX_ERR						= 12,
		INVALID_MODIFICATION_ERR		= 13,
		NAMESPACE_ERR					= 14,
		INVALID_ACCESS_ERR				= 15,
		VALIDATION_ERR					= 16,
		TYPE_MISMATCH_ERR				= 17,
	};

								NCDException(ExceptionCode code) : Exception() { this->code = code; }
	virtual						~NCDException() { }

	ExceptionCode				getCode() const { return code; }

	ExceptionCode				code;
};

#endif // _NCDEXCEPTION_H_
