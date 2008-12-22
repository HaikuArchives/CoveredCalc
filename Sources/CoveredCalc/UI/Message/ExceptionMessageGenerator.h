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
	@file		ExceptionMessageGenerator.h
	@brief		Definition of ExceptionMessageGenerator class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.01.09 created
*/

#ifndef _EXCEPTIONMESSAGEGENERATOR_H_
#define _EXCEPTIONMESSAGEGENERATOR_H_

#include "MBCString.h"

class NativeStringLoader;
class Exception;

// ---------------------------------------------------------------------
//! This class creates a message, which is displayed in UI, about the exception.
// ---------------------------------------------------------------------
class ExceptionMessageGenerator
{
public:
								ExceptionMessageGenerator();
	virtual						~ExceptionMessageGenerator();
	
	void						Init(NativeStringLoader* stringLoader);
	void						GetMessageString(Exception* ex, MBCString& message) const;

private:
	void						processException(Exception* ex, MBCString& message) const;
	bool						processMemoryException(Exception* ex, MBCString& message) const;
	bool						processXMLParseException(Exception* ex, MBCString& message) const;
	bool						processCoverDefParseException(Exception* ex, MBCString& message) const;
	bool						processFileException(Exception* ex, MBCString& message) const;
	bool						processDIBFileLoaderException(Exception* ex, MBCString& message) const;
	bool						processUIControllerException(Exception* ex, MBCString& message) const;
	bool						processColorCodedSkinException(Exception* ex, MBCString& message) const;
	bool						processPathException(Exception* ex, MBCString& message) const;
	bool						processXMLLangFileException(Exception* ex, MBCString& message) const;
	bool						processDialogLayoutException(Exception* ex, MBCString& message) const;

private:
	NativeStringLoader*			stringLoader;
};

#endif // _EXCEPTIONMESSAGEGENERATOR_H_
