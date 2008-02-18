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
	@file		NCDDocumentImpl.h
	@brief		Definition of NCDDocumentImpl class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.04 created
*/

#ifndef _NCDDOCUMENTIMPL_H_
#define _NCDDOCUMENTIMPL_H_

#include "NCDDocument.h"
#include "NCDTreeNode.h"
#include <set>

namespace NanchatteDOMImpl {

// ---------------------------------------------------------------------
//! This class is an implementation of NCDDocument interface.
// ---------------------------------------------------------------------
class NCDDocumentImpl : public NCDDocument, public NCDTreeNodeAccessor
{
public:
										NCDDocumentImpl();
	virtual								~NCDDocumentImpl();

	// implementation of NCDNode interface
	virtual void						getNodeName(UTF8String& output) const;
	virtual void						getNodeValue(UTF8String& output) const;
	virtual void						setNodeValue(ConstUTF8Str value);
	virtual NodeType					getNodeType() const { return NCDNode::DOCUMENT_NODE; }

	virtual NCDNode*					getParentNode() const { return treeNode.GetParentNode(); }
	virtual NCDNode*					getFirstChild() const { return treeNode.GetFirstChildNode(); }
	virtual NCDNode*					getLastChild() const { return treeNode.GetLastChildNode(); }
	virtual NCDNode*					getPreviousSibling() const { return treeNode.GetPrevSiblingNode(); }
	virtual NCDNode*					getNextSibling() const { return treeNode.GetNextSiblingNode(); }

	virtual NCDNamedNodeMap*			getAttributes() const { return NULL; }

	virtual NCDDocument*				getOwnerDocument() const { return NULL; }

	virtual NCDNode*					insertBefore(NCDNode* newChild, NCDNode* refChild);
	virtual NCDNode*					replaceChild(NCDNode* newChild, NCDNode* oldChild);
	virtual NCDNode*					removeChild(NCDNode* oldChild) { return treeNode.RemoveChild(oldChild); }
	virtual NCDNode*					appendChild(NCDNode* newChild);
	virtual bool						hasChildNodes() const { return treeNode.HasChildNodes(); }
	virtual NCDNode*					cloneNode(bool deep) const;

	virtual void						release();

	virtual void						setSourceLine(SInt32 line) { sourceLine = line; }
	virtual void						setSourceColumn(SInt32 column) { sourceColumn = column; }
	virtual SInt32						getSourceLine() const { return sourceLine; }
	virtual SInt32						getSourceColumn() const { return sourceColumn; }

	// implementation of NCDDocument interface
	virtual NCDElement*					getDocumentElement() const;
	virtual NCDElement*					createElement(ConstUTF8Str tagName);
	virtual NCDText*					createTextNode(ConstUTF8Str data);
	virtual NCDComment*					createComment(ConstUTF8Str data);
	virtual NCDAttr*					createAttribute(ConstUTF8Str name);
//	virtual	NCDElement*					getElementById(ConstUTF8Str elementId) const;

	// implementation of NCDTreeNodeAccessor interface
	virtual NCDTreeNode*				GetNCDTreeNode() { return &treeNode; }

	// original implementation
	void								disposeNode(NCDNode* node);

private:
	bool								checkNewChildNode(NCDNode* newChild, bool doSecondCheck);

private:
	typedef	std::set<NCDNode*>			NCDNodeSet;

private:
	NCDTreeNode							treeNode;		///< tree node operation delegate
	NCDNodeSet							ownedNodes;		///< nodes which created by this document
	SInt32								sourceLine;		///< line number in source document
	SInt32								sourceColumn;	///< column number in source document
};

}	// namespace NanchatteDOMImpl

#endif // _NCDDOCUMENTIMPL_H_
