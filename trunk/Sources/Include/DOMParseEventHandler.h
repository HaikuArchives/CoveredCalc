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
	@brief		Definition of DOMParseEventHandler class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.07.14 created
*/

#ifndef _DOMPARSEEVENTHANDLER_H_
#define _DOMPARSEEVENTHANDLER_H_

#include "XMLEventHandler.h"

class NCDDocument;
class NCDNode;

/// This XMLEventHandler generates nanchatte DOM tree. 
class DOMParseEventHandler : public XMLEventHandler
{
public:
								DOMParseEventHandler();
	virtual						~DOMParseEventHandler();
	
	NCDDocument*				DetachDocument();
	
	// implementation of XMLEventHandler interface
	virtual void				BoundToParser(XMLParser* parser);
	
	virtual void				StartElement(ConstUTF8Str name, ConstUTF8Str* attrs);
	virtual void				EndElement(ConstUTF8Str name);
	virtual void				CharacterData(const UTF8Char* data, SInt32 length);
	virtual void				Comment(ConstUTF8Str data);

private:
	NCDDocument*				document;				///< generated document
	NCDNode*					currentNode;			///< current node when parsing
	XMLParser*					xmlParser;				///< parser object
};

#endif // _DOMPARSEEVENTHANDLER_H_
