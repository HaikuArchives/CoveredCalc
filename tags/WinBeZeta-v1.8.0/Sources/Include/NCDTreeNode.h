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
	@file		NCDTreeNode.h
	@brief		Definition of NCDTreeNode class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.04 created
*/

#ifndef _NCDSIMPLETREENODE_H_
#define _NCDSIMPLETREENODE_H_

class NCDNode;

namespace NanchatteDOMImpl {

// ---------------------------------------------------------------------
/*!
	This class manages tree node operation.	

	Each NCDNode-derived class delegates tree node operations to this class.
*/
// ---------------------------------------------------------------------
class NCDTreeNode
{
public:
								NCDTreeNode();
	virtual						~NCDTreeNode();

	void						Init(NCDNode* thisNode);

	NCDNode*					GetParentNode() const { return parentNode; }
	NCDNode*					GetFirstChildNode() const { return firstChildNode; }
	NCDNode*					GetLastChildNode() const { return lastChildNode; }
	NCDNode*					GetPrevSiblingNode() const { return prevSiblingNode; }
	NCDNode*					GetNextSiblingNode() const { return nextSiblingNode; }

	NCDNode*					InsertBefore(NCDNode* newChild, NCDNode* refChild);
	NCDNode*					ReplaceChild(NCDNode* newChild, NCDNode* oldChild);
	NCDNode*					RemoveChild(NCDNode* oldChild);
	NCDNode*					AppendChild(NCDNode* newChild);
	bool						HasChildNodes() const { return (NULL != firstChildNode); }

private:
	NCDTreeNode*				getTreeNode(NCDNode* node);

private:
	NCDNode*					thisNode;			///< this node
	NCDNode*					parentNode;			///< parent node
	NCDNode*					firstChildNode;		///< first child node
	NCDNode*					lastChildNode;		///< last child node
	NCDNode*					prevSiblingNode;	///< previous sibling node
	NCDNode*					nextSiblingNode;	///< next sibling node
};

// ---------------------------------------------------------------------
//!	Each NCDNode-derived class implements this interface.
// ---------------------------------------------------------------------
class NCDTreeNodeAccessor
{
public:
								NCDTreeNodeAccessor() { }
	virtual						~NCDTreeNodeAccessor() { }

	virtual NCDTreeNode*		GetNCDTreeNode() = 0;
};

}	// namespace NanchatteDOMImpl

#endif // _NCDSIMPLETREENODE_H_
