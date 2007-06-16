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
	@file		XMLEventHandler.h
	@brief		Definition of XMLEventHandler interface.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.15 created
*/

#ifndef _XMLEVENTHANDLER_H_
#define _XMLEVENTHANDLER_H_

class XMLParser;

// ---------------------------------------------------------------------
//! Interface of event handlers which called by XMLParser.
// ---------------------------------------------------------------------
class XMLEventHandler
{
public:
						XMLEventHandler() { }
	virtual				~XMLEventHandler() { }

	virtual void		BoundToParser(XMLParser* parser) = 0;
	
	virtual void		StartElement(ConstUTF8Str name, ConstUTF8Str* attrs) = 0;
	virtual void		EndElement(ConstUTF8Str name) = 0;
	virtual void		CharacterData(const UTF8Char* data, SInt32 length) = 0;
	virtual void		Comment(ConstUTF8Str data) = 0;
};

#endif // _XMLEVENTHANDLER_H_
