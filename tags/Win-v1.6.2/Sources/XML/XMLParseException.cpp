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
	@file		XMLParseException.cpp
	@brief		Implementation of XMLParseException and its derived classes.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.14 created
*/

#include "Prefix.h"
#include <stdio.h>
#include <string.h>
#include "XMLParseException.h"

// ---------------------------------------------------------------------
//! buffer for empty message
// ---------------------------------------------------------------------
const AChar XMLParseException::emptyMessage[1] = {'\0'};

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseException::XMLParseException() : Exception()
{
	this->line = -1;
	this->column = -1;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseException::XMLParseException(
	Exception* inner		//!< inner exception
) : Exception(inner)
{
	this->line = -1;
	this->column = -1;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseException::XMLParseException(
	ConstAStr message		//!< error message
) : Exception()
{
	this->line = -1;
	this->column = -1;
	this->message = message;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseException::XMLParseException(
	ConstAStr message,		//!< error message
	Exception* inner		//!< inner exception
) : Exception(inner)
{
	this->line = -1;
	this->column = -1;
	this->message = message;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseException::XMLParseException(
	SInt32 line,		//!< current line at parsing
	SInt32 column		//!< current column in current line at parsing
) : Exception()
{
	this->line = line;
	this->column = column;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseException::XMLParseException(
	SInt32 line,		//!< current line at parsing
	SInt32 column,		//!< current column in current line at parsing
	Exception* inner	//!< inner exception
) : Exception(inner)
{
	this->line = line;
	this->column = column;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseException::XMLParseException(
	SInt32 line,			//!< current line at parsing
	SInt32 column,			//!< current column in current line at parsing
	ConstAStr message		//!< error message
) : Exception()
{
	this->line = line;
	this->column = column;
	this->message = message;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseException::XMLParseException(
	SInt32 line,			//!< current line at parsing
	SInt32 column,			//!< current column in current line at parsing
	ConstAStr message,		//!< error message
	Exception* inner		//!< inner exception
) : Exception(inner)
{
	this->line = line;
	this->column = column;
	this->message = message;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
XMLParseException::~XMLParseException()
{
}

// ---------------------------------------------------------------------
//! Returns the error message.
/*!
	@return Error message
*/
// ---------------------------------------------------------------------
ConstAStr XMLParseException::GetErrorMessage() const
{
	if (!message.IsEmpty())
	{
		return message;
	}
	else
	{
		return emptyMessage;
	}
}

// ---------------------------------------------------------------------
//! Sets the error message.
/*!
	@return Error message
*/
// ---------------------------------------------------------------------
void XMLParseException::setErrorMessage(ConstAStr message)
{
	this->message = message;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseExceptions::NoMatchStartTag::NoMatchStartTag(
	SInt32 line,			//!< current line at parsing
	SInt32 column,			//!< current column in current line at parsing
	ConstUTF8Str name		//!< name of error tag
)
	: XMLParseException(line, column)
{
	init(name);
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseExceptions::NoMatchStartTag::NoMatchStartTag(
	SInt32 line,			//!< current line at parsing
	SInt32 column,			//!< current column in current line at parsing
	ConstUTF8Str name,		//!< name of error tag
	Exception* inner		//!< inner exception
)
	: XMLParseException(line, column, inner)
{
	init(name);
}

// ---------------------------------------------------------------------
//! Initialize
// ---------------------------------------------------------------------
void XMLParseExceptions::NoMatchStartTag::init(
	ConstUTF8Str name		//!< name of error tag
)
{
	this->name = name;

	makeErrorMessage();
}

// ---------------------------------------------------------------------
//! Creates error message and sets it to the object.
// ---------------------------------------------------------------------
void XMLParseExceptions::NoMatchStartTag::makeErrorMessage()
{
	AChar formatMessage[128];
	snprintf(formatMessage, sizeof(formatMessage)/sizeof(AChar) - 1, "Start tag of </%s> is not found.", TypeConv::AsASCII(name));
	formatMessage[sizeof(formatMessage)/sizeof(AChar) - 1] = '\0';
	setErrorMessage(formatMessage);	
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
XMLParseExceptions::NoMatchStartTag::~NoMatchStartTag()
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseExceptions::InvalidParentEntity::InvalidParentEntity(
	SInt32 line,				//!< current line at parsing
	SInt32 column,				//!< current column in current line at parsing
	ConstUTF8Str name,			//!< name of error tag
	ConstUTF8Str parentName		//!< name of parent tag
)
	: XMLParseException(line, column)
{
	this->name = name;
	if (parentName)
	{
		this->parentName = parentName;
	}

	makeErrorMessage();
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseExceptions::InvalidParentEntity::InvalidParentEntity(
	SInt32 line,				//!< current line at parsing
	SInt32 column,				//!< current column in current line at parsing
	ConstUTF8Str name,			//!< name of error tag
	ConstUTF8Str parentName,	//!< name of parent tag
	Exception* inner			//!< inner exception
)
	: XMLParseException(line, column, inner)
{
	this->name = name;
	if (parentName)
	{
		this->parentName = parentName;
	}
	
	makeErrorMessage();
}

// ---------------------------------------------------------------------
//! Creates error message and sets it to the object.
// ---------------------------------------------------------------------
void XMLParseExceptions::InvalidParentEntity::makeErrorMessage()
{
	AChar formatMessage[128];
	if (parentName.IsEmpty())
	{
		snprintf(formatMessage, sizeof(formatMessage)/sizeof(AChar) - 1, "The tag <%s> cannot be root entity.", TypeConv::AsASCII(name));
		formatMessage[sizeof(formatMessage)/sizeof(AChar) - 1] = '\0';
	}
	else
	{
		snprintf(formatMessage, sizeof(formatMessage)/sizeof(AChar) - 1, "The tag <%s> cannot be in the tag <%s>.", TypeConv::AsASCII(name), TypeConv::AsASCII(parentName));
		formatMessage[sizeof(formatMessage)/sizeof(AChar) - 1] = '\0';
	}
	setErrorMessage(formatMessage);
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
XMLParseExceptions::InvalidParentEntity::~InvalidParentEntity()
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseExceptions::MissingAttribute::MissingAttribute(
	SInt32 line,			//!< current line at parsing
	SInt32 column,			//!< current column in current line at parsing
	ConstUTF8Str tagName,	//!< name of error tag
	ConstUTF8Str attrName	//!< name of error attribute
)
	: XMLParseException(line, column)
{
	this->tagName = tagName;
	this->attrName = attrName;

	makeErrorMessage();
}

// ---------------------------------------------------------------------
//! Creates error message and sets it to the object.
// ---------------------------------------------------------------------
void XMLParseExceptions::MissingAttribute::makeErrorMessage()
{
	AChar formatMessage[128];
	snprintf(formatMessage, sizeof(formatMessage)/sizeof(AChar) - 1, "The tag <%s> must have \"%s\" attribute.", TypeConv::AsASCII(tagName), TypeConv::AsASCII(attrName));
	formatMessage[sizeof(formatMessage)/sizeof(AChar) - 1] = '\0';
	setErrorMessage(formatMessage);
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
XMLParseExceptions::MissingAttribute::~MissingAttribute()
{
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
XMLParseExceptions::UnknownTag::UnknownTag(
	SInt32 line,			//!< current line at parsing
	SInt32 column,			//!< current column in current line at parsing
	ConstUTF8Str name		//!< name of error tag
)
	: XMLParseException(line, column)
{
	init(name);
}

// ---------------------------------------------------------------------
//! Initialize
// ---------------------------------------------------------------------
void XMLParseExceptions::UnknownTag::init(
	ConstUTF8Str name			//!< name of error tag
)
{
	this->name = name;

	makeErrorMessage();
}

// ---------------------------------------------------------------------
//! Creates error message and sets it to the object.
// ---------------------------------------------------------------------
void XMLParseExceptions::UnknownTag::makeErrorMessage()
{
	AChar formatMessage[128];
	snprintf(formatMessage, sizeof(formatMessage)/sizeof(AChar) - 1, "The tag \"%s\" is unknown.", TypeConv::AsASCII(name));
	formatMessage[sizeof(formatMessage)/sizeof(AChar) - 1] = '\0';
	setErrorMessage(formatMessage);	
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
XMLParseExceptions::UnknownTag::~UnknownTag()
{
}
