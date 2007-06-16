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
	@file		NCDNamedNodeMap.h
	@brief		Definition of NCDNamedNodeMap interface
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.01 created
*/

#ifndef _NCDNAMEDNODEMAP_H_
#define _NCDNAMEDNODEMAP_H_

class NCDNode;

// ---------------------------------------------------------------------
//! This interface represents collections of nodes that can be accessed by name in the Nanchatte DOM.
// ---------------------------------------------------------------------
class NCDNamedNodeMap
{
public:
								NCDNamedNodeMap() { }
	virtual						~NCDNamedNodeMap() { }

	virtual NCDNode*			getNamedItem(ConstUTF8Str name) const = 0;
	virtual NCDNode*			setNamedItem(NCDNode* arg) = 0;
	virtual NCDNode*			removeNamedItem(ConstUTF8Str name) = 0;

	virtual NCDNode*			item(UInt32 index) const = 0;
	virtual UInt32				getLength() const = 0;
};

#endif // _NCDNAMEDNODEMAP_H_
