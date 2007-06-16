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
	@file		NCDTextImpl.cpp
	@brief		Implementation of NCDTextImpl class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.07.13 created
*/

#include "Prefix.h"
#include "NCDTextImpl.h"
#include "NCDException.h"
#include "NCDDocumentImpl.h"

namespace NanchatteDOMImpl {

/**
 * Constructor
 */
NCDTextImpl::NCDTextImpl(
	NCDDocument* document		///< owner document
)
{
	this->document = document;
	sourceLine = -1;
	sourceColumn = -1;
	treeNode.Init(this);
	value.Empty();
}

/**
 * Destructor
 */
NCDTextImpl::~NCDTextImpl()
{
}

/**
 * Retrieves the name of this node.
 */
void NCDTextImpl::getNodeName(
	UTF8String& output			///< [out] the name of this node is returned.
) const
{
	output = TypeConv::AsUTF8("#text");
}

/**
 * Retrieves the value of this node.
 */
void NCDTextImpl::getNodeValue(
	UTF8String& output			///< [out] the value of this node is returned.
) const
{
	output = value;
}

/**
 * Sets the value of this node.
 */
void NCDTextImpl::setNodeValue(
	ConstUTF8Str value				///< [in] the new value of this node.
)
{
	this->value = value;
}

/**
 * Inserts the new child node before the existing child node.
 *
 * @return inserted node.
 * @throw	NCDException
 */
NCDNode* NCDTextImpl::insertBefore(
	NCDNode* /*newChild*/,			///< [in] new child node.
	NCDNode* /*refChild*/			///< [in] existing child node.
)
{
	throw NCDException(NCDException::HIERARCHY_REQUEST_ERR);
}

/**
 * Replaces the existing child node with the new child node.
 *
 * @return old child node.
 * @throw	NCDException
 */
NCDNode* NCDTextImpl::replaceChild(
	NCDNode* /*newChild*/,			///< [in] new child node.
	NCDNode* /*oldChild*/			///< [in] existing child node
)
{
	throw NCDException(NCDException::HIERARCHY_REQUEST_ERR);
}

/**
 * Adds the new child node to the end of the list of children.
 *
 * @return added node
 * @throw	NCDException
 */
NCDNode* NCDTextImpl::appendChild(
	NCDNode* /*newChild*/			///< [in] new child node.
)
{
	throw NCDException(NCDException::HIERARCHY_REQUEST_ERR);
}

/**
 * Duplicates this node.
 *
 * @return duplicate node.
 */
NCDNode* NCDTextImpl::cloneNode(
	bool /*deep*/		///< [in] if true, also copies children.
) const
{
	NCDTextImpl* newAttr = static_cast<NCDTextImpl*>(document->createTextNode(value));
	return newAttr;
}

/**
 * Release this node and resources associated with it, and also its children.
 */ 
void NCDTextImpl::release()
{
	if (NULL != getParentNode())
	{
		throw new NCDException(NCDException::INVALID_ACCESS_ERR);
	}

	try
	{
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
 * Append the string to the end of the character data of this node.
 */
void NCDTextImpl::appendData(
	ConstUTF8Str arg			///< string to append
)
{
	value += arg;
}

}	// namespace NanchatteDOMImpl
