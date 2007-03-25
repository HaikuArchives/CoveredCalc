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
	@file		CoverDefParseException.h
	@brief		Definition of CoverDefParseException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.23 created
*/

#ifndef _COVERDEFPARSEEXCEPTION_H_
#define _COVERDEFPARSEEXCEPTION_H_

#include "XMLParseException.h"
#include "UTF8String.h"

class CoverDefValidateException;

// ---------------------------------------------------------------------
//! The exception object about parsing "CoverDef.xml".
// ---------------------------------------------------------------------
class CoverDefParseException : public XMLParseException
{
public:
						CoverDefParseException()
							: XMLParseException() { }
						CoverDefParseException(Exception* inner)
							: XMLParseException(inner) { }
						CoverDefParseException(ConstAStr message)
							: XMLParseException(message) { }
						CoverDefParseException(ConstAStr message, Exception* inner)
							: XMLParseException(message, inner) { }
						CoverDefParseException(SInt32 line, SInt32 column)
							: XMLParseException(line, column) { }
						CoverDefParseException(SInt32 line, SInt32 column, ConstAStr message)
							: XMLParseException(line, column, message) { }
						CoverDefParseException(SInt32 line, SInt32 column, ConstAStr message, Exception* inner)
							: XMLParseException(line, column, message, inner) { }
	virtual				~CoverDefParseException() { }
};

namespace CoverDefParseExceptions
{
	// ---------------------------------------------------------------------
	//! Thrown when unknown id is appeared.
	// ---------------------------------------------------------------------
	class UnknownID : public CoverDefParseException
	{
	public:
							UnknownID(ConstUTF8Str id);
		virtual				~UnknownID();
		
		ConstUTF8Str		GetID() const { return id; }
	
	private:
		UTF8String			id;
	};
	
	// ---------------------------------------------------------------------
	//! Thrown when unknown class attribute is appeared.
	// ---------------------------------------------------------------------
	class UnknownClass : public CoverDefParseException
	{
	public:
							UnknownClass(ConstUTF8Str classAttr);
		virtual				~UnknownClass();
		
		ConstUTF8Str		GetClass() const { return classAttr; }
	
	private:
		UTF8String			classAttr;
	};

	// ---------------------------------------------------------------------
	//! Thrown when unknown type attribute is appeared.
	// ---------------------------------------------------------------------
	class UnknownType : public CoverDefParseException
	{
	public:
							UnknownType(ConstUTF8Str typeAttr);
		virtual				~UnknownType();
		
		ConstUTF8Str		GetType() const { return typeAttr; }
	
	private:
		UTF8String			typeAttr;
	};
	
	// ---------------------------------------------------------------------
	//! Thrown when the validation is failed.
	// ---------------------------------------------------------------------
	class ValidationFailed : public CoverDefParseException
	{
	public:
							ValidationFailed(CoverDefValidateException* inner);
							ValidationFailed(SInt32 line, SInt32 column, CoverDefValidateException* inner);
		virtual				~ValidationFailed() { }
	};
	
	// ---------------------------------------------------------------------
	//! Thrown when parsing color string is failed.
	// ---------------------------------------------------------------------
	class ColorStringNotUnderstood : public CoverDefParseException
	{
	public:
							ColorStringNotUnderstood(ConstUTF8Str colorString);
		virtual				~ColorStringNotUnderstood();
		
		ConstUTF8Str		GetColorString() const { return colorString; }
	
	private:
		UTF8String			colorString;
	};
	
	// ---------------------------------------------------------------------
	//! Thrown when the version of CoverDef is unsupported.
	// ---------------------------------------------------------------------
	class UnsupportedCoverDefVersion : public CoverDefParseException
	{
	public:
							UnsupportedCoverDefVersion(ConstUTF8Str versionString);
		virtual				~UnsupportedCoverDefVersion();
		
		ConstUTF8Str		GetVersionString() const { return versionString; }
	
	private:
		UTF8String			versionString;		
	};
	
	// ---------------------------------------------------------------------
	//! Thrown when the document element is not "CoverDef"
	// ---------------------------------------------------------------------
	class InvalidDocumentElement : public CoverDefParseException
	{
	public:
							InvalidDocumentElement()
								: CoverDefParseException() { }
							InvalidDocumentElement(SInt32 line, SInt32 column)
								: CoverDefParseException(line, column) { }
		virtual				~InvalidDocumentElement() { }
	};
}

#endif // _COVERDEFPARSEEXCEPTION_H_
