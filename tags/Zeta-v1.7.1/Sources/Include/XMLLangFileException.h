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
	@file		XMLLangFileException.h
	@brief		Definition of XMLLangFileException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.12.21 created
*/

#ifndef _XMLLANGFILEEXCEPTION_H_
#define _XMLLANGFILEEXCEPTION_H_

#include "Exception.h"

#include "XMLParseException.h"
#include "UTF8String.h"

// ---------------------------------------------------------------------
//! Exception about XMLLangFile object.
// ---------------------------------------------------------------------
class XMLLangFileException : public XMLParseException
{
public:
						XMLLangFileException()
							: XMLParseException() { }
						XMLLangFileException(Exception* inner)
							: XMLParseException(inner) { }
						XMLLangFileException(ConstAStr message)
							: XMLParseException(message) { }
						XMLLangFileException(ConstAStr message, Exception* inner)
							: XMLParseException(message, inner) { }
						XMLLangFileException(SInt32 line, SInt32 column)
							: XMLParseException(line, column) { }
						XMLLangFileException(SInt32 line, SInt32 column, ConstAStr message)
							: XMLParseException(line, column, message) { }
						XMLLangFileException(SInt32 line, SInt32 column, ConstAStr message, Exception* inner)
							: XMLParseException(line, column, message, inner) { }
	virtual				~XMLLangFileException() { }
};

namespace XMLLangFileExceptions
{
	// ---------------------------------------------------------------------
	//! Thrown when the validation is failed.
	// ---------------------------------------------------------------------
	class ValidationFailed : public XMLLangFileException
	{
	public:
							ValidationFailed(SInt32 line, SInt32 column, ConstAStr message)
								: XMLLangFileException(line, column, message) { }
							ValidationFailed(ConstAStr message)
								: XMLLangFileException(message) { }
		virtual				~ValidationFailed() { }
	};
	
	/**
	 *	@brief	Thrown when a string is not defined in XML langage file.
	 */
	class StringNotDefined : public XMLLangFileException
	{
	public:
							StringNotDefined(ConstAStr name)
								: XMLLangFileException() { this->name = name; }
		virtual				~StringNotDefined() { }
		
		ConstAStr			GetName() { return name; }
		
	private:
		MBCString			name;
	};

	/**
	 *	@brief	Thrown when a dialog id is invalid.
	 */
	class InvalidDialogID : public XMLLangFileException
	{
	public:
							InvalidDialogID(SInt32 id)
								: XMLLangFileException() { this->id = id; }
		virtual				~InvalidDialogID() { }
		
		SInt32				GetID() { return id; }
		
	private:
		SInt32				id;
	};

	/**
	 *	@brief	Thrown when a string id is invalid.
	 */
	class InvalidStringID : public XMLLangFileException
	{
	public:
							InvalidStringID(SInt32 id)
								: XMLLangFileException() { this->id = id; }
		virtual				~InvalidStringID() { }
		
		SInt32				GetID() { return id; }
		
	private:
		SInt32				id;
	};
}

#endif // _XMLLANGFILEEXCEPTION_H_
