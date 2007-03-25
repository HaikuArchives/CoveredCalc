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
	@file		NCDAttrImpl.cpp
	@brief		Implementation of NCDAttrImpl class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.07.13 created
*/

#include "Prefix.h"
#include "NCDAttrImpl.h"
#include "NCDText.h"
#include "NCDDocument.h"
#include "NCDException.h"
#include "NCDDocumentImpl.h"

namespace NanchatteDOMImpl {

/**
 * Constructor
 */
NCDAttrImpl::NCDAttrImpl(
	NCDDocument* document,		///< owner document
	ConstUTF8Str nodeName		///< name of this node (attribute name)
)
{
	this->document = document;
	this->nodeName = nodeName;
	element = NULL;
	sourceLine = -1;
	sourceColumn = -1;
	treeNode.Init(this);
}

/**
 * Destructor
 */
NCDAttrImpl::~NCDAttrImpl()
{
}

/**
 * Retrieves the name of this node.
 */
void NCDAttrImpl::getNodeName(
	UTF8String& output			///< [out] the name of this node is returned.
) const
{
	output = nodeName;
}

/**
 * Retrieves the value of this node.
 */
void NCDAttrImpl::getNodeValue(
	UTF8String& output			///< [out] the value of this node is returned.
) const
{
	output.Empty();
	
	NCDNode* childNode = getFirstChild();
	while (NULL != childNode)
	{
		NCDNode::NodeType nodeType = childNode->getNodeType();
		if (NCDNode::TEXT_NODE == nodeType)
		{
			NCDText* textNode = static_cast<NCDText*>(childNode);
			UTF8String data;
			textNode->getData(data);
			output += data;
		}
		else if (NCDNode::ENTITY_REFERENCE_NODE == nodeType)
		{
			// this can't happen at present.
		}
		else
		{
			ASSERT(false);
		}
		
		childNode = childNode->getNextSibling();
	}
}

/**
 * Sets the value of this node.
 */
void NCDAttrImpl::setNodeValue(
	ConstUTF8Str value				///< [in] the new value of this node.
)
{
	// release all children
	while (hasChildNodes())
	{
		NCDNode* childNode = getFirstChild();
		removeChild(childNode);
		childNode->release();
	}

	NCDText* textNode = document->createTextNode(value);
	appendChild(textNode);
}

/**
 * Checks that the new child node is allowed.
 *
 * @retval	true	the new child node is allowed.
 * @retval	false	the new child node is not allowed.	
 */
bool NCDAttrImpl::checkNewChildNode(
	NCDNode* newChild				///< [in] new child node
)
{
	// check that the type of child node is allowed.
	NCDNode::NodeType nodeType = newChild->getNodeType();
	if (nodeType != NCDNode::TEXT_NODE &&
		nodeType != NCDNode::ENTITY_REFERENCE_NODE)
	{
		return false;
	}

	return true;
}

/**
 * Inserts the new child node before the existing child node.
 *
 * @return inserted node.
 * @throw	NCDException
 */
NCDNode* NCDAttrImpl::insertBefore(
	NCDNode* newChild,			///< [in] new child node.
	NCDNode* refChild			///< [in] existing child node.
)
{
	if (!checkNewChildNode(newChild))
	{
		throw NCDException(NCDException::HIERARCHY_REQUEST_ERR);		
	}

	return treeNode.InsertBefore(newChild, refChild);
}

/**
 * Replaces the existing child node with the new child node.
 *
 * @return old child node.
 * @throw	NCDException
 */
NCDNode* NCDAttrImpl::replaceChild(
	NCDNode* newChild,			///< [in] new child node.
	NCDNode* oldChild			///< [in] existing child node
)
{
	if (!checkNewChildNode(newChild))
	{
		throw NCDException(NCDException::HIERARCHY_REQUEST_ERR);		
	}

	return treeNode.ReplaceChild(newChild, oldChild);	
}

/**
 * Adds the new child node to the end of the list of children.
 *
 * @return added node
 * @throw	NCDException
 */
NCDNode* NCDAttrImpl::appendChild(
	NCDNode* newChild			///< [in] new child node.
)
{
	if (!checkNewChildNode(newChild))
	{
		throw NCDException(NCDException::HIERARCHY_REQUEST_ERR);		
	}

	return treeNode.AppendChild(newChild);
}

/**
 * Duplicates this node.
 *
 * @return duplicate node.
 */
NCDNode* NCDAttrImpl::cloneNode(
	bool /*deep*/		///< [in] if true, also copies children.
) const
{
	NCDAttrImpl* newAttr = static_cast<NCDAttrImpl*>(document->createAttribute(nodeName));
		
	// always copy children (its value)
	NCDNode* childNode = getFirstChild();
	while (NULL != childNode)
	{
		NCDNode* newChildNode = childNode->cloneNode(true);
		newAttr->appendChild(newChildNode);
		
		childNode = childNode->getNextSibling();
	}
	
	return newAttr;
}

/**
 * Release this node and resources associated with it, and also its children.
 */ 
void NCDAttrImpl::release()
{
	if (NULL != getOwnerElement())
	{
		throw new NCDException(NCDException::INVALID_ACCESS_ERR);
	}

	try
	{
		// release children
		while (hasChildNodes())
		{
			NCDNode* childNode = getFirstChild();
			removeChild(childNode);
			childNode->release();
		}
		
		// dispose this node.
		static_cast<NCDDocumentImpl*>(document)->disposeNode(this);
	}
	catch (NCDException* ex)
	{
		ASSERT(FALSE);
	
		// ignore
		ex->Delete();
	}
}

/**
 * Returns whether this attribute is explicitly given a value in the document.
 *
 * @retval	true	this attribute is explicitly given a value.
 * @retval	false	this attribute is default value.
 */ 
bool NCDAttrImpl::getSpecified() const
{
	return true;	// at present, default attribute is not supported.
}

}	// namespace NanchatteDOMImpl
