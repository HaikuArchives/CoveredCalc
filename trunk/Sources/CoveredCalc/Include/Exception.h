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
	@file		Exception.h
	@brief		Definition of Exception class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.8.24 created
*/

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

// ---------------------------------------------------------------------
//! The basic exception object.
// ---------------------------------------------------------------------
class Exception
{
public:
						Exception();
						Exception(Exception* inner);
	virtual				~Exception();
	
	virtual	ConstAStr	GetErrorMessage() const;
			Exception*	GetInnerException() const;
	virtual	void		Delete();

private:
	Exception*			innerException;
};

// ---------------------------------------------------------------------
//! Returns an inner exception if exist
/*!
	@return Inner exception if it is exist in this object, otherwise NULL
*/
// ---------------------------------------------------------------------
inline Exception* Exception::GetInnerException() const
{
	return innerException;
}

#endif // _EXCEPTION_H_
