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
	@file		NCDTextImpl.h
	@brief		Definition of NCDTextImpl class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.07.13 created
*/

#ifndef _NCDTEXTIMPL_H_
#define _NCDTEXTIMPL_H_

#include "NCDText.h"
#include "NCDTreeNode.h"

namespace NanchatteDOMImpl {

/**
 * This class implements NCDText interface.
 */
class NCDTextImpl : public NCDText, public NCDTreeNodeAccessor
{
public:
								NCDTextImpl(NCDDocument* document);
	virtual						~NCDTextImpl();

	// implementation of NCDNode interface
	virtual void				getNodeName(UTF8String& output) const;
	virtual void				getNodeValue(UTF8String& output) const;
	virtual void				setNodeValue(ConstUTF8Str value);
	virtual NodeType			getNodeType() const { return NCDNode::TEXT_NODE; }
	
	virtual NCDNode*			getParentNode() const { return treeNode.GetParentNode(); }
	virtual NCDNode*			getFirstChild() const { return treeNode.GetFirstChildNode(); }
	virtual NCDNode*			getLastChild() const { return treeNode.GetLastChildNode(); }
	virtual NCDNode*			getPreviousSibling() const { return treeNode.GetPrevSiblingNode(); }
	virtual NCDNode*			getNextSibling() const { return treeNode.GetNextSiblingNode(); }

	virtual NCDNamedNodeMap*	getAttributes() const { return NULL; }

	virtual NCDDocument*		getOwnerDocument() const { return document; }

	virtual NCDNode*			insertBefore(NCDNode* newChild, NCDNode* refChild);
	virtual NCDNode*			replaceChild(NCDNode* newChild, NCDNode* oldChild);
	virtual NCDNode*			removeChild(NCDNode* oldChild) { return treeNode.RemoveChild(oldChild); }
	virtual NCDNode*			appendChild(NCDNode* newChild);
	virtual bool				hasChildNodes() const { return treeNode.HasChildNodes(); }
	virtual NCDNode*			cloneNode(bool deep) const;

	virtual void				release();

	virtual void				setSourceLine(SInt32 line) { sourceLine = line; }
	virtual void				setSourceColumn(SInt32 column) { sourceColumn = column; }
	virtual SInt32				getSourceLine() const { return sourceLine; }
	virtual SInt32				getSourceColumn() const { return sourceColumn; }

	// implementation of NCDCharacterData interface
	virtual	void				getData(UTF8String& output) const { getNodeValue(output); }
	virtual void				setData(ConstUTF8Str value) { setNodeValue(value); }

//	virtual	void				substringData(UInt32 offset, UInt32 count, UTF8String& output) const = 0;
	virtual	void				appendData(ConstUTF8Str arg);
//	virtual void				insertData(UInt32 offset, ConstUTF8Str arg) = 0;
//	virtual void				deleteData(UInt32 offset, UInt32 count) = 0;
//	virtual void				replaceData(UInt32 offset, UInt32 count, ConstUTF8Str arg) = 0;

	// implementation of NCDText interface
//	virtual	NCDText*			splitText(UInt32 offset) = 0;

	// implementation of NCDTreeNodeAccessor interface
	virtual NCDTreeNode*		GetNCDTreeNode() { return &treeNode; }

private:
	NCDTreeNode					treeNode;		///< tree node operation delegate
	NCDDocument*				document;		///< owner document
	UTF8String					value;			///< node value
	SInt32						sourceLine;		///< line number in source document
	SInt32						sourceColumn;	///< column number in source document
};

}	// namespace NanchatteDOMImpl

#endif // _NCDTEXTIMPL_H_
