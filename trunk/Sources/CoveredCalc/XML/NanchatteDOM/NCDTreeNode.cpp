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
	@file		NCDTreeNode.h
	@brief		Implementation of NCDTreeNode class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.04 created
*/

#include "Prefix.h"
#include "NCDTreeNode.h"
#include "NCDNode.h"
#include "NCDException.h"
#include "NCDDocument.h"

namespace NanchatteDOMImpl {

// ---------------------------------------------------------------------
//!	Constructor
// ---------------------------------------------------------------------
NCDTreeNode::NCDTreeNode()
{
	thisNode = NULL;
	parentNode = NULL;
	firstChildNode = NULL;
	lastChildNode = NULL;
	prevSiblingNode = NULL;
	nextSiblingNode = NULL;
}

// ---------------------------------------------------------------------
//!	Destructor
// ---------------------------------------------------------------------
NCDTreeNode::~NCDTreeNode()
{
}

// ---------------------------------------------------------------------
//! Initializes object.
// ---------------------------------------------------------------------
void NCDTreeNode::Init(
	NCDNode* thisNode		//!< this node
)
{
	this->thisNode = thisNode;
}

// ---------------------------------------------------------------------
//! Retrieves NCDTreeNode object from NCDNode object (which should be derived from NCDTreeNodeAccessor)
// ---------------------------------------------------------------------
NCDTreeNode* NCDTreeNode::getTreeNode(NCDNode* node)
{
	NCDTreeNode* ret = NULL;

	if (NULL != node)
	{
		NCDTreeNodeAccessor* accessor = dynamic_cast<NCDTreeNodeAccessor*>(node);
		ASSERT(NULL != accessor);
		if (NULL != accessor)
		{
			ret = accessor->GetNCDTreeNode();
		}
	}
	return ret;
}

// ---------------------------------------------------------------------
//! Inserts the child node before the specified node.
/*!
	@return inserted node.
*/
// ---------------------------------------------------------------------
NCDNode* NCDTreeNode::InsertBefore(
	NCDNode* newChild,			//!< new child node.
	NCDNode* refChild			//!< new child node is inserted before this node.
)
{
	if (newChild == refChild)
	{
		return newChild;
	}

	// check newChild is created from same document.
	if (thisNode->getNodeType() == NCDNode::DOCUMENT_NODE)
	{
		if (newChild->getOwnerDocument() != static_cast<NCDDocument*>(thisNode))
		{
			throw new NCDException(NCDException::WRONG_DOCUMENT_ERR);
		}
	}
	else if (newChild->getOwnerDocument() != thisNode->getOwnerDocument())
	{
		throw new NCDException(NCDException::WRONG_DOCUMENT_ERR);
	}

	// check refChild is a child of this node.
	if (NULL != refChild && refChild->getParentNode() != thisNode)
	{
		throw new NCDException(NCDException::NOT_FOUND_ERR);
	}

	// if newChild is in the tree, first remove it.
	if (NULL != newChild->getParentNode())
	{
		newChild->getParentNode()->removeChild(newChild);
	}

	NCDTreeNode* newTreeChild = getTreeNode(newChild);

	if (NULL == refChild)
	{
		// insert as first child
		newTreeChild->parentNode = thisNode;
		newTreeChild->nextSiblingNode = NULL;
		newTreeChild->prevSiblingNode = lastChildNode;
		
		if (NULL == lastChildNode)
		{
			// there was no child.
			firstChildNode = lastChildNode = newChild;
		}
		else
		{
			// there was some child(ren).
			NCDTreeNode* lastTreeChild = getTreeNode(lastChildNode);
			ASSERT(NULL == lastTreeChild->nextSiblingNode);
			lastTreeChild->nextSiblingNode = newChild;
			lastChildNode = newChild;
		}
	}
	else
	{
		// insert before refChild.
		NCDNode* refPrevChild = refChild->getPreviousSibling();
		if (NULL == refPrevChild)
		{
			// refChild was first child.
			newTreeChild->parentNode = thisNode;
			newTreeChild->nextSiblingNode = refChild;
			newTreeChild->prevSiblingNode = NULL;

			NCDTreeNode* refTreeChild = getTreeNode(refChild);
			ASSERT(NULL == refTreeChild->prevSiblingNode);
			refTreeChild->prevSiblingNode = newChild;
			firstChildNode = newChild;
		}
		else
		{
			newTreeChild->parentNode = thisNode;
			newTreeChild->nextSiblingNode = refChild;
			newTreeChild->prevSiblingNode = refPrevChild;

			NCDTreeNode* refPrevTreeChild = getTreeNode(refPrevChild);
			refPrevTreeChild->nextSiblingNode = newChild;
			
			NCDTreeNode* refTreeChild = getTreeNode(refChild);
			refTreeChild->prevSiblingNode = newChild;
		}
	}
	
	return newChild;
}

// ---------------------------------------------------------------------
//! Replaces the oldChild with newChild in the list of children.
/*!
	@return oldChild.
*/
// ---------------------------------------------------------------------
NCDNode* NCDTreeNode::ReplaceChild(
	NCDNode* newChild,
	NCDNode* oldChild
)
{
	// check newChild is created from same document.
	NCDNode* thisDocument = (NCDNode::DOCUMENT_NODE == thisNode->getNodeType()) ? thisNode : thisNode->getOwnerDocument();
	if (newChild->getOwnerDocument() != thisDocument)
	{
		throw new NCDException(NCDException::WRONG_DOCUMENT_ERR);
	}

	// check oldChild is a child of this node.
	if (oldChild->getParentNode() != thisNode)
	{
		throw new NCDException(NCDException::NOT_FOUND_ERR);
	}

	NCDNode* nextChild = oldChild->getNextSibling();
	RemoveChild(oldChild);
	InsertBefore(newChild, nextChild);
	return oldChild;
}

// ---------------------------------------------------------------------
//! Removes child node from the list of children.
/*!
	@return oldChild.
*/
// ---------------------------------------------------------------------
NCDNode* NCDTreeNode::RemoveChild(NCDNode* oldChild)
{
	// check oldChild is a child of this node.
	if (oldChild->getParentNode() != thisNode)
	{
		throw new NCDException(NCDException::NOT_FOUND_ERR);
	}

	NCDNode* prevChild = oldChild->getPreviousSibling();
	NCDNode* nextChild = oldChild->getNextSibling();
	
	if (NULL == prevChild)
	{
		// oldChild was the first child.
		if (NULL == nextChild)
		{
			// oldChild was the only child.
			firstChildNode = lastChildNode = NULL;
		}
		else
		{
			NCDTreeNode* nextTreeChild = getTreeNode(nextChild);
			nextTreeChild->prevSiblingNode = NULL;			
			firstChildNode = nextChild;
		}		
	}
	else if (NULL == nextChild)
	{
		// oldChild was the last child.
		NCDTreeNode* prevTreeChild = getTreeNode(prevChild);
		prevTreeChild->nextSiblingNode = NULL;
		lastChildNode = prevChild;
	}
	else
	{
		NCDTreeNode* nextTreeChild = getTreeNode(nextChild);
		nextTreeChild->prevSiblingNode = prevChild;
		NCDTreeNode* prevTreeChild = getTreeNode(prevChild);
		prevTreeChild->nextSiblingNode = nextChild;
	}

	NCDTreeNode* oldTreeChild = getTreeNode(oldChild);
	oldTreeChild->parentNode = NULL;
	oldTreeChild->nextSiblingNode = NULL;
	oldTreeChild->prevSiblingNode = NULL;	
	return oldChild;
}

// ---------------------------------------------------------------------
//! Adds the node to the end of the list of children of this node.
/*!
	@return added node.
*/
// ---------------------------------------------------------------------
NCDNode* NCDTreeNode::AppendChild(
	NCDNode* newChild			//!< new child node
)
{
	return InsertBefore(newChild, NULL);
}

}	// namespace NanchatteDOMImpl
