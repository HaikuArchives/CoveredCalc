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
	@file		NCDElementImpl.cpp
	@brief		Implementation of NCDElementImpl class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.05.16 created
*/

#include "Prefix.h"
#include "NCDElementImpl.h"
#include "NCDException.h"
#include "NCDAttr.h"
#include "NCDDocumentImpl.h"
#include "UTF8String.h"

namespace NanchatteDOMImpl {

/**
 * Constructor
 */
NCDElementImpl::NCDElementImpl(
	NCDDocument* document,		///< owner document
	ConstUTF8Str nodeName		///< name of this node (tag name)
)
{
	this->document = document;
	this->nodeName = nodeName;
	sourceLine = -1;
	sourceColumn = -1;
	treeNode.Init(this);
	attributes.Init(this);
}

/**
 * Destructor
 */
NCDElementImpl::~NCDElementImpl()
{
}

/**
 * Retrieves the name of this node.
 */
void NCDElementImpl::getNodeName(
	UTF8String& output			///< [out] the name of this node is returned.
) const
{
	output = nodeName;
}

/**
 * Retrieves the value of this node.
 */
void NCDElementImpl::getNodeValue(
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
void NCDElementImpl::setNodeValue(
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
bool NCDElementImpl::checkNewChildNode(
	NCDNode* newChild				///< [in] new child node
)
{
	// check that the type of child node is allowed.
	NCDNode::NodeType nodeType = newChild->getNodeType();
	if (nodeType != NCDNode::ELEMENT_NODE &&
		nodeType != NCDNode::TEXT_NODE &&
		nodeType != NCDNode::COMMENT_NODE &&
		nodeType != NCDNode::PROCESSING_INSTRUCTION_NODE &&
		nodeType != NCDNode::CDATA_SECTION_NODE &&
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
NCDNode* NCDElementImpl::insertBefore(
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
NCDNode* NCDElementImpl::replaceChild(
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
NCDNode* NCDElementImpl::appendChild(NCDNode* newChild)
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
NCDNode* NCDElementImpl::cloneNode(
	bool deep		///< [in] if true, also copies children.
) const
{
	NCDElementImpl* newElement = static_cast<NCDElementImpl*>(document->createElement(nodeName));
	
	// copy attributes
	UInt32 attrCount = attributes.getLength();
	UInt32 attrIndex;
	for (attrIndex=0; attrIndex<attrCount; attrIndex++)
	{
		NCDAttr* attr = attributes.attrItem(attrIndex);
		NCDAttr* newAttr = static_cast<NCDAttr*>(attr->cloneNode(true));
		newElement->attributes.setAttrItem(newAttr);
	}
	
	// copy children
	if (deep)
	{
		NCDNode* childNode = getFirstChild();
		while (NULL != childNode)
		{
			NCDNode* newChildNode = childNode->cloneNode(deep);
			newElement->appendChild(newChildNode);
			
			childNode = childNode->getNextSibling();
		}
	}
	
	return newElement;
}

/**
 * Release this node and resources associated with it, and also its children.
 */ 
void NCDElementImpl::release()
{
	if (NULL != getParentNode())
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
 * Retrieves the name of the element.
 */
void NCDElementImpl::getTagName(
	UTF8String& output				///< [out] the name is retrieved.
) const
{
	output = nodeName;
}

/**
 * Retrieves the value of the specified attribute.
 */
void NCDElementImpl::getAttribute(
	ConstUTF8Str name,					///< [in] attribute name
	UTF8String& output				///< [out] value is retrieved
) const
{
	NCDAttr* attribute = attributes.getAttrItem(name);
	if (NULL == attribute)
	{
		output.Empty();
	}
	else
	{
		attribute->getValue(output);
	}
}

/**
 * Sets the value of the specified attribute.
 */
void NCDElementImpl::setAttribute(
	ConstUTF8Str name,					///< [in] attribute name
	ConstUTF8Str value					///< [in] attribute value
)
{
	NCDAttr* attribute = attributes.getAttrItem(name);
	if (NULL == attribute)
	{
		attribute = document->createAttribute(name);
		attribute->setNodeValue(value);
		attributes.setNamedItem(attribute);
	}
	else
	{
		attribute->setNodeValue(value);
	}
}

/**
 * Removes an attribute.
 */
void NCDElementImpl::removeAttribute(
	ConstUTF8Str name					///< [in] attribute name
)
{
	NCDNode* removed = attributes.removeNamedItem(name);
	if (NULL != removed)
	{
		removed->release();
	}
}

/**
 * Retrieves the node of the specified attribute.
 *
 * @return attribute node.
 */
NCDAttr* NCDElementImpl::getAttributeNode(
	ConstUTF8Str name					///< [in] attribute name
) const
{
	return attributes.getAttrItem(name);
}

/**
 * Set a new attribute node.
 *
 * @return if an existing attirbute is replaced this function returns it. otherwise, NULL.
 */
NCDAttr* NCDElementImpl::setAttributeNode(
	NCDAttr* newAttr				///< [in] attribute node
)
{
	UTF8String name;
	newAttr->getName(name);

#if 0
	NCDAttr* oldAttr = attributes.getAttrItem(name);
	if (NULL == oldAttr)
	{
		attributes.setNamedItem(newAttr);
		return NULL;
	}
	else
	{
		if (newAttr == oldAttr)		// same object
		{
			return NULL;			// no effect
		}
		else
		{
			attributes.removeNamedItem(name);
			attributes.setNamedItem(newAttr);
			return oldAttr;
		}
	}
#else
	return attributes.setAttrItem(newAttr);
#endif
}

/**
 * Removes the specified attribute node.
 *
 * @return removed attribute node.
 */
NCDAttr* NCDElementImpl::removeAttributeNode(
	NCDAttr* oldAttr				///< [in] attribtue
)
{
	UTF8String name;
	oldAttr->getName(name);

	if (attributes.getNamedItem(name) != oldAttr)
	{
		throw new NCDException(NCDException::NOT_FOUND_ERR);
	}
	
	return attributes.removeAttrItem(name);
}

}	// namespace NanchatteDOMImpl
