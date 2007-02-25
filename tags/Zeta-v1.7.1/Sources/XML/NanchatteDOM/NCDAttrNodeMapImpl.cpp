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
	@file		NCDAttrNodeMapImpl.cpp
	@brief		Implementation of NCDAttrNodeMapImpl class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.05.20 created
*/

#include "Prefix.h"
#include "NCDAttrNodeMapImpl.h"
#include "NCDAttr.h"
#include "NCDException.h"
#include "NCDElement.h"
#include "NCDAttrImpl.h"

namespace NanchatteDOMImpl {

/**
 * Constructor
 */
NCDAttrNodeMapImpl::NCDAttrNodeMapImpl()
{
	ownerElement = NULL;
}

/**
 * Destructor
 */
NCDAttrNodeMapImpl::~NCDAttrNodeMapImpl()
{
	AttrNodeVector::iterator ite;
	for (ite=attrNodes.begin(); ite!=attrNodes.end(); ite++)
	{
		NCDAttrImpl* attrNode = static_cast<NCDAttrImpl*>(*ite);
		attrNode->setOwnerElement(NULL);
		attrNode->release();
	}
}

/**
 * Initializes this object.
 */
void NCDAttrNodeMapImpl::Init(
	NCDElement* ownerElement		///< [in] owner element node.
)
{
	ASSERT(NULL != ownerElement);
	this->ownerElement = ownerElement;
}

/**
 * Retrieves a node by specified name.
 *
 * @return node. or NULL if node is not found.
 */
NCDNode* NCDAttrNodeMapImpl::getNamedItem(
	ConstUTF8Str name				///< [in] node name
) const
{
	return getAttrItem(name);
}

/**
 * Sets a new node.
 *
 * @return if an existing attirbute is replaced this function returns it. otherwise, NULL.
 */ 
NCDNode* NCDAttrNodeMapImpl::setNamedItem(
	NCDNode* arg					///< [in] node object.
)
{
	if (arg->getNodeType() != NCDNode::ATTRIBUTE_NODE)
	{
		throw new NCDException(NCDException::HIERARCHY_REQUEST_ERR);
	}
	
	return setAttrItem(static_cast<NCDAttr*>(arg));
}

/**
 * Removes a node by specified name.
 *
 * @return removed node
 */
NCDNode* NCDAttrNodeMapImpl::removeNamedItem(
	ConstUTF8Str name			///< [in] node name.
)
{
	return removeAttrItem(name);
}

/**
 * Returns the index-th item in the map.
 *
 * @return index-th item in the map, or NULL if index is not valid.
 */
NCDNode* NCDAttrNodeMapImpl::item(
	UInt32 index				///< [in] index
) const
{
	return attrItem(index);
}

/**
 * Returns the number of nodes in the map.
 *
 * @return number of nodes
 */
UInt32 NCDAttrNodeMapImpl::getLength() const
{
	return attrNodes.size();
}

/**
 * Retrieves an attribute node by specified name.
 *
 * @return node. or NULL if node is not found.
 */
NCDAttr* NCDAttrNodeMapImpl::getAttrItem(
	ConstUTF8Str name
) const
{
	AttrNodeVector::const_iterator ite;
	for (ite=attrNodes.begin(); ite!=attrNodes.end(); ite++)
	{
		NCDAttr* attrNode = *ite;
		UTF8String nodeName;
		attrNode->getName(nodeName);
		if (0 == nodeName.Compare(name))
		{
			return attrNode;
		}
	}
	return NULL;
}

/**
 * Sets a new attribute node.
 *
 * @return if an existing attirbute is replaced this function returns it. otherwise, NULL.
 */ 
NCDAttr* NCDAttrNodeMapImpl::setAttrItem(
	NCDAttr* arg						///< [in] node object
)
{
	ASSERT(NULL != ownerElement);			
	if (arg->getOwnerDocument() != ownerElement->getOwnerDocument())
	{
		throw new NCDException(NCDException::WRONG_DOCUMENT_ERR);
	}
	if (arg->getOwnerElement() != NULL)
	{
		throw new NCDException(NCDException::INUSE_ATTRIBUTE_ERR);
	}
	
	UTF8String argName;
	arg->getName(argName);
	
	NCDAttr* oldNode = NULL;
	
	AttrNodeVector::iterator ite;
	for (ite=attrNodes.begin(); ite!=attrNodes.end(); ite++)
	{
		NCDAttr* attrNode = *ite;
		UTF8String nodeName;
		attrNode->getName(nodeName);
		if (0 == nodeName.Compare(argName))
		{
			oldNode = attrNode;
			break;
		}
	}
	if (oldNode == arg)
	{
		return NULL;		// no effect
	}
	if (NULL != oldNode)
	{
		attrNodes.erase(ite);
		NCDAttrImpl* oldNodeImpl = static_cast<NCDAttrImpl*>(oldNode);
		oldNodeImpl->setOwnerElement(NULL);
	}
	attrNodes.push_back(arg);
	NCDAttrImpl* argImpl = static_cast<NCDAttrImpl*>(arg);
	argImpl->setOwnerElement(ownerElement);	
	
	return oldNode;
}

/**
 * Removes an attribute node by specified name.
 *
 * @return removed node
 */
NCDAttr* NCDAttrNodeMapImpl::removeAttrItem(
	ConstUTF8Str name				///< [in] node name
)
{
	AttrNodeVector::iterator ite;
	for (ite=attrNodes.begin(); ite!=attrNodes.end(); ite++)
	{
		NCDAttr* attrNode = *ite;
		UTF8String nodeName;
		attrNode->getName(nodeName);
		if (0 == nodeName.Compare(name))
		{
			attrNodes.erase(ite);
			NCDAttrImpl* attrNodeImpl = static_cast<NCDAttrImpl*>(attrNode);
			attrNodeImpl->setOwnerElement(NULL);
			return attrNode;
		}
	}
	
	return NULL;
}

/**
 * Returns the index-th attribute item in the map.
 *
 * @return index-th item in the map, or NULL if index is not valid.
 */
NCDAttr* NCDAttrNodeMapImpl::attrItem(
	UInt32 index					///< [in] index
) const
{
	if (attrNodes.size() <= index)
	{
		return NULL;
	}
	else
	{
		return attrNodes[index];
	}
}

}	// namespace NanchatteDOMImpl
