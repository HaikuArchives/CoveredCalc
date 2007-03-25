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
	@file		DOMParseEventHandler.h
	@brief		Implementation of DOMParseEventHandler class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.07.14 created
*/

#include "Prefix.h"
#include "DOMParseEventHandler.h"
#include "NCDDocument.h"
#include "NCDDocumentFactory.h"
#include "NCDElement.h"
#include "NCDText.h"
#include "NCDComment.h"
#include "NCDAttr.h"
#include "XMLParser.h"

/**
 * Constructor
 */
DOMParseEventHandler::DOMParseEventHandler()
{
	document = NULL;
	currentNode = NULL;
	xmlParser = NULL;
}

/*
 * Destructor
 */
DOMParseEventHandler::~DOMParseEventHandler()
{
	if (NULL != document)
	{
		document->release();
	}
}

/**
 * Detaches generated document.
 *
 * @return generated document
 */
NCDDocument* DOMParseEventHandler::DetachDocument()
{
	NCDDocument* ret = document;
	document = NULL;
	return ret;
}

/**
 * Called when handler is bound to the parser
 */
void DOMParseEventHandler::BoundToParser(
	XMLParser* parser		//!< parser to which the handler is bound
)
{
	if (NULL != document)
	{
		document->release();
	}
	document = NCDDocumentFactory::CreateDocument();
	currentNode = document;
	xmlParser = parser;
}

/**
 * The handler for a start tag.
 */
void DOMParseEventHandler::StartElement(
	ConstUTF8Str name,			//!< name of tag
	ConstUTF8Str* attrs			//!< array of attributes. consists of pairs (name followed by value) and terminated by NULL.
)
{
	SInt32 line = -1;
	SInt32 column = -1;
	if (NULL != xmlParser)
	{
		line = xmlParser->GetCurrentLineNumber();
		column = xmlParser->GetCurrentColumnNumber();
	}
	NCDElement* element = document->createElement(name);
	element->setSourceLine(line);
	element->setSourceColumn(column);
	try
	{
		for (; *attrs; attrs+=2)
		{
			element->setAttribute(*attrs, *(attrs + 1));
			NCDAttr* attrNode = element->getAttributeNode(*attrs);
			if (NULL != attrNode)
			{
				attrNode->setSourceLine(line);
				attrNode->setSourceColumn(column);
			}
		}
		currentNode->appendChild(element);
	}
	catch (...)
	{
		element->release();
		throw;
	}

	currentNode = element;
}

/**
 * The handler for an end tag.
 */
void DOMParseEventHandler::EndElement(
	ConstUTF8Str /*name*/		//!< name of tag
)
{
	ASSERT(NCDNode::ELEMENT_NODE == currentNode->getNodeType());
	currentNode = currentNode->getParentNode();
}

/**
 * The handler for a text.
 */
void DOMParseEventHandler::CharacterData(
	const UTF8Char* data,			//!< text which is not nul-terminated.
	SInt32 length					//!< length of data
)
{
	UTF8Char	stackBuffer[1024];		// このサイズ以内の長さならバッファをヒープにとらない。
	UTF8Char*	buffer = stackBuffer;
	
	if (length >= static_cast<SInt32>(sizeof(stackBuffer)/sizeof(UTF8Char)))
	{
		buffer = static_cast<UTF8Char*>(malloc(length + 1));
	}
	
	try
	{
		memcpy(buffer, data, length);
		buffer[length] = TypeConv::AsUTF8('\0');

		NCDNode* lastChild = currentNode->getLastChild();
		if (NULL != lastChild && NCDNode::TEXT_NODE == lastChild->getNodeType())
		{
			NCDText* textNode = static_cast<NCDText*>(lastChild);
			textNode->appendData(buffer);
		}
		else
		{
			SInt32 line = -1;
			SInt32 column = -1;
			if (NULL != xmlParser)
			{
				line = xmlParser->GetCurrentLineNumber();
				column = xmlParser->GetCurrentColumnNumber();
			}			
			NCDText* textNode = document->createTextNode(buffer);
			textNode->setSourceLine(line);
			textNode->setSourceColumn(column);
			try
			{
				currentNode->appendChild(textNode);
			}
			catch (...)
			{
				textNode->release();
				throw;
			}
		}
	}
	catch (...)
	{
		if (buffer != stackBuffer)
		{
			free(buffer);
		}
		throw;
	}	
	if (buffer != stackBuffer)
	{
		free(buffer);
	}
}

/**
 * The handler for a comment.
 */
void DOMParseEventHandler::Comment(
	ConstUTF8Str data				//!< comment string
)
{
	SInt32 line = -1;
	SInt32 column = -1;
	if (NULL != xmlParser)
	{
		line = xmlParser->GetCurrentLineNumber();
		column = xmlParser->GetCurrentColumnNumber();
	}			
	NCDComment* commentNode = document->createComment(data);
	commentNode->setSourceLine(line);
	commentNode->setSourceColumn(column);
	try
	{
		currentNode->appendChild(commentNode);
	}
	catch (...)
	{
		commentNode->release();
		throw;
	}
}
