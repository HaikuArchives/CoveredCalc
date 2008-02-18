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
	@brief		Implementation of NCDDocumentImpl class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.04 created
*/

#include "Prefix.h"
#include "NCDDocumentImpl.h"
#include "NCDException.h"
#include "NCDElement.h"
#include "TypeConv.h"
#include "NCDElementImpl.h"
#include "NCDAttrImpl.h"
#include "NCDTextImpl.h"
#include "NCDCommentImpl.h"

namespace NanchatteDOMImpl {

/**
 * Constructor
 */
NCDDocumentImpl::NCDDocumentImpl()
{
	sourceLine = -1;
	sourceColumn = -1;
	treeNode.Init(this);
}

/**
 * Destructor
 */
NCDDocumentImpl::~NCDDocumentImpl()
{
}

/**
 * Retrieves the name of this node.
 */
void NCDDocumentImpl::getNodeName(
	UTF8String& output			///< [out] the name of this node is returned.
) const
{
	output = TypeConv::AsUTF8("#document");
}

/**
 * Retrieves the value of this node.
 */
void NCDDocumentImpl::getNodeValue(
	UTF8String& output			///< [out] the value of this node is returned.
) const
{
	output.Empty();
}

/**
 * Sets the value of this node.
 *
 * @throw NCDException always throws. (NO_MODIFICATION_ALLOWED ERR)
 */
void NCDDocumentImpl::setNodeValue(
	ConstUTF8Str /*value*/				///< [in] the new value of this node.
)
{
	throw new NCDException(NCDException::NO_MODIFICATION_ALLOWED_ERR);
}

/**
 * Checks that the new child node is allowed.
 *
 * @retval	true	the new child node is allowed.
 * @retval	false	the new child node is not allowed.	
 */
bool NCDDocumentImpl::checkNewChildNode(
	NCDNode* newChild,				///< [in] new child node
	bool doSecondCheck				///< [in] if true, it checks that newChild is not a second node.
)
{
	// check that the type of child node is allowed.
	NCDNode::NodeType nodeType = newChild->getNodeType();
	if (nodeType != NCDNode::ELEMENT_NODE &&
		nodeType != NCDNode::PROCESSING_INSTRUCTION_NODE &&
		nodeType != NCDNode::COMMENT_NODE &&
		nodeType != NCDNode::DOCUMENT_TYPE_NODE)
	{
		return false;
	}

	// check that newChild is not a second Element or DocumentType node.
	if (doSecondCheck)
	{
		if (nodeType == NCDNode::ELEMENT_NODE || nodeType == NCDNode::DOCUMENT_TYPE_NODE)
		{
			if (treeNode.HasChildNodes())
			{
				if (treeNode.GetFirstChildNode() != newChild)
				{
					return false;
				}
			}
		}
	}

	return true;
}

/**
 * Inserts the new child node before the existing child node.
 *
 * @return inserted node.
 * @throw	NCDException
 */
NCDNode* NCDDocumentImpl::insertBefore(
	NCDNode* newChild,			///< [in] new child node.
	NCDNode* refChild			///< [in] existing child node.
)
{
	if (!checkNewChildNode(newChild, true))
	{
		throw new NCDException(NCDException::HIERARCHY_REQUEST_ERR);		
	}

	return treeNode.InsertBefore(newChild, refChild);
}

/**
 * Replaces the existing child node with the new child node.
 *
 * @return old child node.
 * @throw	NCDException
 */
NCDNode* NCDDocumentImpl::replaceChild(
	NCDNode* newChild,			///< [in] new child node.
	NCDNode* oldChild			///< [in] existing child node
)
{
	if (!checkNewChildNode(newChild, false))
	{
		throw new NCDException(NCDException::HIERARCHY_REQUEST_ERR);		
	}

	return treeNode.ReplaceChild(newChild, oldChild);	
}

/**
 * Adds the new child node to the end of the list of children.
 *
 * @return added node
 * @throw	NCDException
 */
NCDNode* NCDDocumentImpl::appendChild(NCDNode* newChild)
{
	if (!checkNewChildNode(newChild, true))
	{
		throw new NCDException(NCDException::HIERARCHY_REQUEST_ERR);		
	}

	return treeNode.AppendChild(newChild);
}

/**
 * Duplicates this node. --- but, cloning NCDDocument is not supported in this implementation.
 *
 * @return duplicate node.
 */
NCDNode* NCDDocumentImpl::cloneNode(
	bool /*deep*/		///< [in] if true, also copies children.
) const
{
	throw new NCDException(NCDException::NOT_SUPPORTED_ERR);
}

/**
 * Release this node and resources associated with it, and also its children.
 *
 * This function disposes all data which associated with this document.
 * For example, the element which was created by this document's createElement() but
 * it has no parent (ie, does not exist in node-tree), is also disposed.  
 */ 
void NCDDocumentImpl::release()
{
	try
	{
		// first, dispose document (root) element in node-tree and its children.
		NCDNode* documentElement = getDocumentElement();
		if (NULL != documentElement)
		{
			removeChild(documentElement);
			documentElement->release();
		}
		
		// next, dispose left node.
		while (ownedNodes.size() > 0)
		{
			NCDNodeSet::iterator ite = ownedNodes.begin();
			ASSERT(ite != ownedNodes.end());	// because ownedNodes.size() > 0.
			NCDNode* node = *ite;

			if (node->getNodeType() == NCDNode::ATTRIBUTE_NODE)
			{
				NCDAttr* attrNode = static_cast<NCDAttr*>(node);
				NCDNode* ownerElement = attrNode->getOwnerElement();
				if (NULL != ownerElement)
				{
					// must dispose owner element node before this attribute node.
					node = ownerElement;
				}
			}

			// must dispose parent node before this node.
			NCDNode* parentNode = node->getParentNode();
			while (NULL != parentNode)
			{
				node = parentNode;
				parentNode = node->getParentNode();
			}
			
			node->release();
		}
		
		delete this;
	}
	catch (NCDException* ex)
	{
		ASSERT(FALSE);
	
		// ignore
		ex->Delete();
	}
}

/**
 * Returns document element node.
 *
 * @return document element node.
 */
NCDElement* NCDDocumentImpl::getDocumentElement() const
{
	return dynamic_cast<NCDElement*>(treeNode.GetFirstChildNode());
}

/**
 * Creates an element of the type specified.
 *
 * @return created element object.
 */
NCDElement* NCDDocumentImpl::createElement(
	ConstUTF8Str tagName			///< [in] name of the element
)
{
	NCDElementImpl* element = new NCDElementImpl(this, tagName);
	
	// MEMO: if it supports DOM specification that default attribute is automatically created,
	// the creation of default attribute is done here.
	
	
	ownedNodes.insert(element);
	return element;
}

/**
 * Creates an text node given the specified string.
 *
 * @return created text node object.
 */
NCDText* NCDDocumentImpl::createTextNode(
	ConstUTF8Str data		///< [in] the data for new node.
)
{
	NCDTextImpl* text = new NCDTextImpl(this);
	text->setData(data);
	ownedNodes.insert(text);
	return text;
}

/**
 * Creates an comment node given the specified string.
 *
 * @return created comment object.
 */
NCDComment* NCDDocumentImpl::createComment(
	ConstUTF8Str data		///< [in] the data for new node.
)
{
	NCDCommentImpl* comment = new NCDCommentImpl(this);
	comment->setData(data);
	ownedNodes.insert(comment);
	return comment;
}

/**
 * Creates an attribute of the given name.
 *
 * @return created attribute object.
 */
NCDAttr* NCDDocumentImpl::createAttribute(
	ConstUTF8Str name				///< [in] name of the attribute
)
{
	NCDAttrImpl* attr = new NCDAttrImpl(this, name);
	ownedNodes.insert(attr);
	return attr;
}

/**
 * Disposes a node which was created by this document.
 *
 * @note Don't access to the node any more after it was disposed.
 */
void NCDDocumentImpl::disposeNode(
	NCDNode* node					///< [in] the node which will be disposed
)
{
	ownedNodes.erase(node);
	delete node;
}

}	// namespace NanchatteDOMImpl
