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
	@file		BeUIMessageProvider.cpp
	@brief		Implementation of BeUIMessageProvider class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.16 created
*/

#include "Prefix.h"
#include "MessageFormatter.h"
#include "BeUIMessageProvider.h"
#include "BeCoveredCalcApp.h"
#include "BeXMLLangFile.h"
#include "XMLLangFileException.h"

static const char STR_STRING_NOT_DEFINED[] = "[[The string \"%0:s%\" is not defined in the language file.]]";

// ---------------------------------------------------------------------
//! Constructor.
// ---------------------------------------------------------------------
BeUIMessageProvider::BeUIMessageProvider()
{
}

// ---------------------------------------------------------------------
//! Initializes the object.
// ---------------------------------------------------------------------
void BeUIMessageProvider::Init()
{
}

// ---------------------------------------------------------------------
//! Destructor.
// ---------------------------------------------------------------------
BeUIMessageProvider::~BeUIMessageProvider()
{
}

/**
 *	@brief		Retrieves message without using language file.
 *	@param[in]	messageId	message id
 *	@param[out]	message		message is returned.
 *	@return		true if message is retrieved.
 */
bool BeUIMessageProvider::getMessageWithoutLangFile(SInt32 messageId, MBCString& message)
{
	switch (messageId)
	{
	case IDS_EMSG_LOAD_COMMANDLINE_LANGFILE:
		message = "Failed to load the language file specified as a command line parameter.\nThe parameter is ignored.";
		break;
	case IDS_EMSG_LOAD_SETTING_LANGFILE:
		message = "Failed to load the language file specified in your settings.";
		break;
	case IDS_EMSG_LOAD_DEFAULT_LANGFILE:
		message = "Failed to load the default language file.";
		break;
		
	default:
		return false;
		break;
	}
	
	return true;
}

/**
 *	@brief	Inner function to get message.
 *	@param[in]	messageId	message id
 *	@param[out]	message		messsage is returned.	
 */
void BeUIMessageProvider::getMessage(SInt32 messageId, MBCString& message)
{
	// 言語ファイルを読み込めないときのメッセージなど、
	// 一部のメッセージは言語ファイルを使わずに内部定義のメッセージを使う。
	if (getMessageWithoutLangFile(messageId, message))
	{
		return;
	}

	const BeXMLLangFile* langFile = BeCoveredCalcApp::GetInstance()->GetLangFile();
	langFile->LoadStringFromID(messageId, message);
}

/**
 *	@brief	Makes an error message which is shown when the string is not defined.
 *	@param[in]	name	string name.
 *	@param[out]	message	message is returned.
 */
void BeUIMessageProvider::getStringNotDefinedMessage(ConstAStr name, MBCString& message)
{
	MBCString errorFormat;
	try
	{
		getMessage(IDS_EMSG_LANG_STRING_NOT_DEFINED, errorFormat);
	}
	catch (XMLLangFileExceptions::StringNotDefined* ex2)
	{
		ex2->Delete();
		errorFormat = STR_STRING_NOT_DEFINED;
	}
	MessageFormatter::Format(message, errorFormat, name);
}

/**
 *	@brief	Retrieves message.
 *	@return	true when succeeded.
 */
bool BeUIMessageProvider::GetMessage(
	SInt32 messageId,			//!< message id
	MBCString& message			//!< OUTPUT. message is returned
)
{
	try
	{
		getMessage(messageId, message);
	}
	catch (XMLLangFileExceptions::StringNotDefined* ex)
	{
		getStringNotDefinedMessage(ex->GetName(), message);
		ex->Delete();
		return false;
	}
	return true;
}

/**
 *	@brief	Retrieves and format message.
 *	@return	true when succeeded.
 */
bool BeUIMessageProvider::GetNFormatMessage(
	MBCString& message,				//!< OUTPUT. formatted message is returned
	SInt32 messageId,				//!< message id
	...								//!< parameters
)
{
	bool ret = false;
	va_list args;
	va_start(args, messageId);
	try
	{
		try
		{
			MBCString format;
			getMessage(messageId, format);
	
			MessageFormatter::FormatV(message, format, args);
			ret = true;
		}
		catch (XMLLangFileExceptions::StringNotDefined* ex)
		{
			getStringNotDefinedMessage(ex->GetName(), message);
			ex->Delete();
		}
	}
	catch (...)
	{
		va_end(args);
		throw;
	}
	va_end(args);
	
	return ret;
}
