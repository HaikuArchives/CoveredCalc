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
	@file		NCDCharacterData.h
	@brief		Definition of NCDCharacterData interface
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.01 created
*/

#ifndef _NCDCHARACTERDATA_H_
#define _NCDCHARACTERDATA_H_

#include "NCDNode.h"
#include "UTF8String.h"

// ---------------------------------------------------------------------
/*!
	This interface extends NCDNode with a set of methods for
	accessing character data in the Nanchatte DOM.
*/
// ---------------------------------------------------------------------
class NCDCharacterData : public NCDNode
{
public:
								NCDCharacterData() { }
	virtual						~NCDCharacterData() { }

	virtual	void				getData(UTF8String& output) const = 0;
	virtual void				setData(ConstUTF8Str value) = 0;

//	virtual	void				substringData(UInt32 offset, UInt32 count, UTF8String& output) const = 0;
	virtual	void				appendData(ConstUTF8Str arg) = 0;
//	virtual void				insertData(UInt32 offset, ConstUTF8Str arg) = 0;
//	virtual void				deleteData(UInt32 offset, UInt32 count) = 0;
//	virtual void				replaceData(UInt32 offset, UInt32 count, ConstUTF8Str arg) = 0;
};

#endif // _NCDCHARACTERDATA_H_
