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
	@file		XMLParser.cpp
	@brief		Implementation of XMLParser class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.13 created
*/

#include "Prefix.h"
#include "TypeConv.h"
#include "XMLParser.h"
#include "XMLParseException.h"
#include "XMLEventHandler.h"
#include "MemoryException.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParser::XMLParser()
{
	parser = NULL;
	handler = NULL;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
XMLParser::~XMLParser()
{
	Destroy();
}

// ---------------------------------------------------------------------
//! Creates a new parser.
/*!
	@throw XMLParseException failed
*/
// ---------------------------------------------------------------------
void XMLParser::Create()
{
	ASSERT(NULL == parser);

	parser = ::XML_ParserCreate(NULL);
	bindHandler();
	if (NULL != handler)
	{
		handler->BoundToParser(this);
	}	
}

// ---------------------------------------------------------------------
//! Registers our callback functions to the parser.
// ---------------------------------------------------------------------
void XMLParser::bindHandler()
{
	ASSERT(NULL != parser);

	::XML_SetElementHandler(parser, xmlStartElementHandler, xmlEndElementHandler);
	::XML_SetCharacterDataHandler(parser, xmlCharacterDataHandler);
	::XML_SetCommentHandler(parser, xmlCommentHandler);

	::XML_SetUserData(parser, handler);
}

// ---------------------------------------------------------------------
//! Destroys the parser.
// ---------------------------------------------------------------------
void XMLParser::Destroy()
{
	if (NULL != parser)
	{
		::XML_ParserFree(parser);
		parser = NULL;
	}
}

// ---------------------------------------------------------------------
//! Sets event handler.
// ---------------------------------------------------------------------
void XMLParser::SetEventHandler(
	XMLEventHandler* handler		//!< event handler object
)
{
	this->handler = handler;
	
	if (NULL != parser)
	{
		::XML_SetUserData(parser, handler);
		handler->BoundToParser(this);
	}	
}

// ---------------------------------------------------------------------
//! Do parsing.
/*!
	@throw XMLParseException when failed
*/
// ---------------------------------------------------------------------
void XMLParser::Parse(
	const char* s,			//!< buffer contains part (or all) of document.
	SInt32 len,				//!< length of the buffer.
	bool isFinal			//!< specified whether this is last piece of document.
)
{
	ASSERT(NULL != parser);

	if (XML_STATUS_OK != ::XML_Parse(parser, s, len, isFinal))
	{
		throwNewXMLParseExcepton();
	}
}

// ---------------------------------------------------------------------
//! Do parsing internal buffer.
/*!
	Do parsing internal buffer.
	GetParseBuffer() function should be called prior to this call.

	@throw XMLParseException when failed
*/
// ---------------------------------------------------------------------
void XMLParser::ParseBuffer(
	SInt32 len,			//!< length of the buffer.
	bool isFinal		//!< specified whether this is last piece of document.
)
{
	ASSERT(NULL != parser);

	if (XML_STATUS_OK != ::XML_ParseBuffer(parser, len, isFinal))
	{
		throwNewXMLParseExcepton();
	}
}

// ---------------------------------------------------------------------
//! Creates new XMLParseException according to the error information.
/*!
	@return new XMLParseException object.
*/
// ---------------------------------------------------------------------
void XMLParser::throwNewXMLParseExcepton()
{
	if (NULL != parser)
	{
		SInt32 line = GetCurrentLineNumber();
		SInt32 column = GetCurrentColumnNumber();
		ConstAStr message = GetErrorString();
		throw new XMLParseException(line, column, message);
	}
	else
	{
		throw new XMLParseException();
	}
}

// ---------------------------------------------------------------------
//! Obtains the buffer which is used in ParseBuffer() function.
/*!
	@return the buffer.
	@throw MemoryException when failed to allocate buffer
*/
// ---------------------------------------------------------------------
void* XMLParser::GetParseBuffer(
	SInt32 len			//!< length of the buffer.
)
{
	ASSERT(NULL != parser);

	void* buffer = ::XML_GetBuffer(parser, len);
	if (NULL == buffer)
	{
		MemoryException::Throw();
		return NULL;
	}
	else
	{
		return buffer;
	}
}

// ---------------------------------------------------------------------
//! Returns Expat error code.
/*!
	@return error code.
*/
// ---------------------------------------------------------------------
XML_Error XMLParser::GetErrorCode()
{
	ASSERT(NULL != parser);

	return ::XML_GetErrorCode(parser);
}

// ---------------------------------------------------------------------
//! Returns a string describing the error corresponding to code.
/*!
	@return error string.
*/
// ---------------------------------------------------------------------
ConstAStr XMLParser::GetErrorString(
	XML_Error code			//!< error code
)
{
	return ::XML_ErrorString(code);
}

// ---------------------------------------------------------------------
//! Returns the current line number.
/*!
	@return current line number
*/
// ---------------------------------------------------------------------
SInt32 XMLParser::GetCurrentLineNumber()
{
	ASSERT(NULL != parser);
	return ::XML_GetCurrentLineNumber(parser);
}

// ---------------------------------------------------------------------
//! Returns the current column number in the current line.
/*!
	@return current column number
*/
// ---------------------------------------------------------------------
SInt32 XMLParser::GetCurrentColumnNumber()
{
	ASSERT(NULL != parser);
	return ::XML_GetCurrentColumnNumber(parser);
}

// ---------------------------------------------------------------------
//! Handler for start tags.
// ---------------------------------------------------------------------
void XMLCALL XMLParser::xmlStartElementHandler(
	void* userData,				//!< pointer to XMLEventHandler object
	const XML_Char* name,		//!< name of start tag
	const XML_Char** atts		//!< attributes
)
{
	XMLEventHandler* handler = static_cast<XMLEventHandler*>(userData);
	if (NULL != handler)
	{
		handler->StartElement(TypeConv::AsUTF8(name), reinterpret_cast<ConstUTF8Str*>(atts));
	}
}

// ---------------------------------------------------------------------
//! Handler for end tags.
// ---------------------------------------------------------------------
void XMLCALL XMLParser::xmlEndElementHandler(
	void* userData,				//!< pointer to XMLEventHandler object
	const XML_Char* name		//!< name of end tag
)
{
	XMLEventHandler* handler = static_cast<XMLEventHandler*>(userData);
	if (NULL != handler)
	{
		handler->EndElement(TypeConv::AsUTF8(name));
	}
}

// ---------------------------------------------------------------------
//! Handler for text.
// ---------------------------------------------------------------------
void XMLCALL XMLParser::xmlCharacterDataHandler(
	void* userData,				//!< pointer to XMLEventHandler object
	const XML_Char* s,			//!< the text. this is not null-terminated.
	int len						//!< length of the text.
)
{
	XMLEventHandler* handler = static_cast<XMLEventHandler*>(userData);
	if (NULL != handler)
	{
		handler->CharacterData(reinterpret_cast<const UTF8Char*>(s), len);
	}
}

/**
 * Handler for comment.
 */
void XMLCALL XMLParser::xmlCommentHandler(
	void* userData,				///< pointer to XMLEventHandler object
	const XML_Char* data		///< comment text
)
{
	XMLEventHandler* handler = static_cast<XMLEventHandler*>(userData);
	if (NULL != handler)
	{
		handler->Comment(TypeConv::AsUTF8(data));
	}
}
