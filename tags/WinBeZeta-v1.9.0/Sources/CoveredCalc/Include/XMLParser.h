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
	@file		XMLParser.h
	@brief		Definition of XMLParser class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.11 created
*/

#ifndef _XMLPARSER_H_
#define _XMLPARSER_H_

#include "expat.h"

class XMLEventHandler;
class XMLParseException;

// ---------------------------------------------------------------------
//! Parsing XML data.
/*!
	@note
	This is wrapper class for Expat, but only needed functions
	are implemented.
*/
// ---------------------------------------------------------------------
class XMLParser
{
public:
							XMLParser();
	virtual					~XMLParser();
	
	void					Create();
	void					Destroy();
	
	void					SetEventHandler(XMLEventHandler* handler);
	
	void					Parse(const char* s, SInt32 len, bool isFinal);
	void					ParseBuffer(SInt32 len, bool isFinal);
	void*					GetParseBuffer(SInt32 len);

	XML_Error				GetErrorCode();
	static	ConstAStr		GetErrorString(XML_Error code);
	ConstAStr				GetErrorString()			 { return GetErrorString(GetErrorCode()); }
	SInt32					GetCurrentLineNumber();
	SInt32					GetCurrentColumnNumber();						

private:
	void					bindHandler();
	void					throwNewXMLParseExcepton();

	static void XMLCALL		xmlStartElementHandler(void* userData, const XML_Char* name, const XML_Char** atts);
	static void XMLCALL		xmlEndElementHandler(void* userData, const XML_Char* name);
	static void XMLCALL		xmlCharacterDataHandler(void* userData, const XML_Char* s, int len);
	static void XMLCALL		xmlCommentHandler(void* userData, const XML_Char* data);

private:
	XML_Parser				parser;
	XMLEventHandler*		handler;
};


#endif // _XMLPARSER_H_
