/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2008 CoveredCalc Project Contributors
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
	@file		DialogLayoutException.h
	@brief		Definition of DialogLayoutException and  its derived classes.
	@author 	ICHIMIYA Hironori (Hiron)
	@date		2008.10.29 created
*/

#ifndef _DIALOGLAYOUTEXCEPTION_H_
#define _DIALOGLAYOUTEXCEPTION_H_

#include "MBCString.h"
#include "Exception.h"

/**
 *	@brief	exception about DialogLayout
 */
class DialogLayoutException : public Exception
{
public:
							DialogLayoutException() { }
							DialogLayoutException(const MBCString& anItemName) : itemName(anItemName) { }
							DialogLayoutException(Exception* inner) : Exception(inner) { }
							DialogLayoutException(Exception* inner, const MBCString& anItemName) : Exception(inner), itemName(anItemName) { }
	virtual					~DialogLayoutException() { }
	
	const MBCString&		GetItemName() const { return itemName; }

private:
	MBCString				itemName;
};

namespace DialogLayoutExceptions
{
	/// Thrown when layout item is not defined.
	class ItemNotFound : public DialogLayoutException
	{
	public:
							ItemNotFound(const MBCString& anItemName) : DialogLayoutException(anItemName) { }
		virtual				~ItemNotFound() { }
	};
	
	/// Thrown when a circular reference is detected.
	class CircularReference : public DialogLayoutException
	{
	public:
							CircularReference() { }
		virtual				~CircularReference() { }
	};
	
	/// Thrown when it could not compute dialog layout.
	class FailedToCompute : public DialogLayoutException
	{
	public:
							FailedToCompute(Exception* inner, const MBCString& anItemName) : DialogLayoutException(inner, anItemName) { }
		virtual				~FailedToCompute() { }
	};
	
	/// Thrown when item value is not computed yet.
	class ItemNotComputed : public DialogLayoutException
	{
	public:
							ItemNotComputed(const MBCString& anItemName) : DialogLayoutException(anItemName) { }
		virtual				~ItemNotComputed() { }
	};
}

#endif // _DIALOGLAYOUTEXCEPTION_H_
