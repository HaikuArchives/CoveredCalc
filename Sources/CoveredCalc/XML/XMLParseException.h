/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2008 CoveredCalc Project Contributors
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
	@file		XMLParseException.h
	@brief		Definition of XMLParseException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.14 created
*/

#ifndef _XMLPARSEEXCEPTION_H_
#define _XMLPARSEEXCEPTION_H_

#include "MBCString.h"
#include "UTF8String.h"
#include "Exception.h"

// ---------------------------------------------------------------------
//! The exception object about parsing xml.
// ---------------------------------------------------------------------
class XMLParseException : public Exception
{
public:
						XMLParseException();
						XMLParseException(Exception* inner);
						XMLParseException(ConstAStr message);
						XMLParseException(ConstAStr message, Exception* inner);
						XMLParseException(SInt32 line, SInt32 column);
						XMLParseException(SInt32 line, SInt32 column, Exception* inner);
						XMLParseException(SInt32 line, SInt32 column, ConstAStr message);
						XMLParseException(SInt32 line, SInt32 column, ConstAStr message, Exception* inner);
	virtual				~XMLParseException();

	void				SetLine(SInt32 line)		{ this->line = line; }
	void				SetColumn(SInt32 column)	{ this->column = column; }

	virtual SInt32		GetLine() const				{ return line; }
	virtual SInt32		GetColumn() const			{ return column; }
	virtual	ConstAStr	GetErrorMessage() const;

protected:
	ConstAStr			getErrorMessage() const		{ return message; }
	void				setErrorMessage(ConstUTF8Str message);

private:
	SInt32				line;
	SInt32				column;
	MBCString			message;
	static	const AChar	emptyMessage[1];
};

namespace XMLParseExceptions
{
	// ---------------------------------------------------------------------
	//! Thrown when end tag which has no start tag, is appeared.
	// ---------------------------------------------------------------------
	class NoMatchStartTag : public XMLParseException
	{
	public:
								NoMatchStartTag(SInt32 line, SInt32 column, ConstUTF8Str name);
								NoMatchStartTag(SInt32 line, SInt32 column, ConstUTF8Str name, Exception* inner);
		virtual					~NoMatchStartTag();

		virtual ConstUTF8Str	GetName() const { return name; }

	private:
		void					init(ConstUTF8Str name);
		void					makeErrorMessage();
		
	private:
		UTF8String				name;
	};	

	// ---------------------------------------------------------------------
	//! Thrown when the tag cannot be in current parent entity.
	// ---------------------------------------------------------------------
	class InvalidParentEntity : public XMLParseException
	{
	public:
								InvalidParentEntity(SInt32 line, SInt32 column, ConstUTF8Str name, ConstUTF8Str parentName);
								InvalidParentEntity(SInt32 line, SInt32 column, ConstUTF8Str name, ConstUTF8Str parentName, Exception* inner);
		virtual					~InvalidParentEntity();
		
		virtual ConstUTF8Str	GetName() const { return name; }
		virtual ConstUTF8Str	GetParentName() const { return parentName; }
	
	private:
		void					makeErrorMessage();
	
	private:
		UTF8String				name;
		UTF8String				parentName;
	};	
	
	// ---------------------------------------------------------------------
	//! Thrown when the attribute which is not omissible is omitted.
	// ---------------------------------------------------------------------
	class MissingAttribute : public XMLParseException
	{
	public:
								MissingAttribute(SInt32 line, SInt32 column, ConstUTF8Str tagName, ConstUTF8Str attrName);
		virtual					~MissingAttribute();
		
		virtual ConstUTF8Str	GetTagName() const { return tagName; }
		virtual ConstUTF8Str	GetAttributeName() const { return attrName; }

	private:
		void					makeErrorMessage();
	
	private:
		UTF8String				tagName;
		UTF8String				attrName;
	};
	
	// ---------------------------------------------------------------------
	//! Thrown when an unknown tag is appeared.
	// ---------------------------------------------------------------------
	class UnknownTag : public XMLParseException
	{
	public:
								UnknownTag(SInt32 line, SInt32 column, ConstUTF8Str name);
		virtual					~UnknownTag();

		virtual ConstUTF8Str	GetName() const { return name; }

	private:
		void					init(ConstUTF8Str name);
		void					makeErrorMessage();

	private:
		UTF8String				name;
	};
}

#endif // _XMLPARSEEXCEPTION_H_
