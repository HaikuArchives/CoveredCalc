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
	@file		KeyFuncOperation.h
	@brief		Definition of KeyFuncOperation interface.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.10.13 created
*/

#ifndef _KEYFUNCOPERATION_H_
#define _KEYFUNCOPERATION_H_

class UTF8String;

/**
 *	@brief	This interface provides key-function related operation.
 */
class KeyFuncOperation
{
public:
	enum KeyFunc
	{
		KeyFunc_None = -1
	};

						KeyFuncOperation() {}
	virtual				~KeyFuncOperation() {}
	
	/**
	 *	@brief	Converts a function name string to a keyfunc ID.
	 *	@param[in]	function name
	 *	@return		keyfunc ID.
	 */
	virtual	SInt32		FuncNameToKeyFunc(ConstUTF8Str funcName) const = 0;
	
	/**
	 *	@brief	Converts a keyfunc ID to a function name string.
	 *	@param[in]	keyFunc		keyfunc ID.
	 *	@param[out]	funcName	function name is returned.
	 */
	virtual void		KeyFuncToFuncName(SInt32 keyFunc, UTF8String& funcName) const = 0;
};

#endif // _KEYFUNCOPERATION_H_
