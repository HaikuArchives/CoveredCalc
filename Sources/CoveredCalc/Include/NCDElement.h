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
	@file		NCDElement.h
	@brief		Definition of NCDElement interface
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.01 created
*/

#ifndef _NCDELEMENT_H_
#define _NCDELEMENT_H_

#include "NCDNode.h"
#include "UTF8String.h"

class NCDAttr;

// ---------------------------------------------------------------------
//! This interface represents a single element in the Nanchatte DOM tree.
// ---------------------------------------------------------------------
class NCDElement : public NCDNode
{
public:
								NCDElement() { }
	virtual						~NCDElement() { }

	virtual void				getTagName(UTF8String& output) const = 0;

	virtual	void				getAttribute(ConstUTF8Str name, UTF8String& output) const = 0;
	virtual void				setAttribute(ConstUTF8Str name, ConstUTF8Str value) = 0;
	virtual void				removeAttribute(ConstUTF8Str name) = 0;

	virtual NCDAttr*			getAttributeNode(ConstUTF8Str name) const = 0;
	virtual NCDAttr*			setAttributeNode(NCDAttr* newAttr) = 0;
	virtual NCDAttr*			removeAttributeNode(NCDAttr* oldAttr) = 0;

//	NCDNodeList*				getElementsByTagName(UTF8Str name) const = 0;
};

#endif // _NCDELEMENT_H_
