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
	@file		NCDDocument.h
	@brief		Definition of NCDDocument interface
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.01 created
*/

#ifndef _NCDDOCUMENT_H_
#define _NCDDOCUMENT_H_

#include "NCDNode.h"
class NCDElement;
class NCDText;
class NCDComment;
class NCDAttr;

// ---------------------------------------------------------------------
//! This interface represents the entire XML document in the Nanchatte DOM.
// ---------------------------------------------------------------------
class NCDDocument : public NCDNode
{
public:
										NCDDocument() { }
	virtual								~NCDDocument() { }

//	virtual NCDDocumentType*			getDoctype() const = 0;
//	virtual NCDImplementation*			getImplementation() const = 0;
	virtual NCDElement*					getDocumentElement() const = 0;

	virtual NCDElement*					createElement(ConstUTF8Str tagName) = 0;
//	virtual NCDDocumentFragmment*		createDocumentFragment() = 0;
	virtual NCDText*					createTextNode(ConstUTF8Str data) = 0;
	virtual NCDComment*					createComment(ConstUTF8Str data) = 0;
//	virtual NCDCDATASection*			createCDATASection(ConstUTF8Str data) = 0;
//	virtual NCDProcessingInstruction*	createProcessingInstruction(ConstUTF8Str target, ConstUTF8Str data) = 0;
	virtual NCDAttr*					createAttribute(ConstUTF8Str name) = 0;
//	virtual NCDEntityReference			createEntityReference(ConstUTF8Str name) = 0;

//	virtual NCDNodeList*				getElementsByTagName(ConstUTF8Str tagname) const = 0;
	
//	virtual	NCDElement*					getElementById(ConstUTF8Str elementId) const = 0;
};

#endif // _NCDDOCUMENT_H_
