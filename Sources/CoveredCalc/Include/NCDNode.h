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
	@file		NCDNode.h
	@brief		Definition of NCDNode interface
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.01 created
*/

#ifndef _NCDNODE_H_
#define _NCDNODE_H_

#include "UTF8String.h"

class NCDNamedNodeMap;
class NCDDocument;

// ---------------------------------------------------------------------
//! This interface represents a single node in the Nanchatte DOM tree.
// ---------------------------------------------------------------------
class NCDNode
{
public:

	//! NodeType
	enum NodeType
	{
		ELEMENT_NODE				= 1,
		ATTRIBUTE_NODE				= 2,
		TEXT_NODE					= 3,
		CDATA_SECTION_NODE			= 4,
		ENTITY_REFERENCE_NODE		= 5,
		ENTITY_NODE					= 6,
		PROCESSING_INSTRUCTION_NODE	= 7,
		COMMENT_NODE				= 8,
		DOCUMENT_NODE				= 9,
		DOCUMENT_TYPE_NODE			= 10,
		DOCUMENT_FRAGMENT_NODE		= 11,
		NOTATION_NODE				= 12,
	};

								NCDNode() { }
	virtual						~NCDNode() { }

	virtual void				getNodeName(UTF8String& output) const = 0;
	virtual void				getNodeValue(UTF8String& output) const = 0;
	virtual void				setNodeValue(ConstUTF8Str value) = 0;
	virtual NodeType			getNodeType() const = 0;
	
	virtual NCDNode*			getParentNode() const = 0;
//	virtual NCDNodeList*		getChildNodes() const = 0;
	virtual NCDNode*			getFirstChild() const = 0;
	virtual NCDNode*			getLastChild() const = 0;
	virtual NCDNode*			getPreviousSibling() const = 0;
	virtual NCDNode*			getNextSibling() const = 0;

	virtual NCDNamedNodeMap*	getAttributes() const = 0;

	virtual NCDDocument*		getOwnerDocument() const = 0;

	virtual NCDNode*			insertBefore(NCDNode* newChild, NCDNode* refChild) = 0;
	virtual NCDNode*			replaceChild(NCDNode* newChild, NCDNode* oldChild) = 0;
	virtual NCDNode*			removeChild(NCDNode* oldChild) = 0;
	virtual NCDNode*			appendChild(NCDNode* newChild) = 0;
	virtual bool				hasChildNodes() const = 0;
	virtual NCDNode*			cloneNode(bool deep) const = 0;

//	void						normalize() = 0;

	// like Xerces-C++
	virtual void				release() = 0;
	
	// original feature
	virtual void				setSourceLine(SInt32 line) = 0;
	virtual void				setSourceColumn(SInt32 column) = 0;
	virtual SInt32				getSourceLine() const = 0;
	virtual SInt32				getSourceColumn() const = 0;
};

#endif // _NCDNODE_H_
