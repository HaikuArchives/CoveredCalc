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
	@file		WinUIMessageProvider.h
	@brief		Definition of WinUIMessageProvider class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.01.12 created
*/

#include "Prefix.h"
#include "WinCoveredCalcApp.h"
#include "WinUIMessageProvider.h"
#include "MemoryException.h"
#include "MessageFormatter.h"

// ---------------------------------------------------------------------
//! Constructor.
// ---------------------------------------------------------------------
WinUIMessageProvider::WinUIMessageProvider()
{
}

// ---------------------------------------------------------------------
//! Initializes the object.
// ---------------------------------------------------------------------
void WinUIMessageProvider::Init()
{
}

// ---------------------------------------------------------------------
//! Destructor.
// ---------------------------------------------------------------------
WinUIMessageProvider::~WinUIMessageProvider()
{
}

// ---------------------------------------------------------------------
//! Retrieves message.
// ---------------------------------------------------------------------
void WinUIMessageProvider::GetMessage(
	SInt32 messageId,			//!< message id
	MBCString& message			//!< OUTPUT. message is returned
)
{
	GetMessage(WinCoveredCalcApp::GetInstance()->GetLangResHandle(), messageId, message);
}

// ---------------------------------------------------------------------
//! Retrieves message with instance handle.
// ---------------------------------------------------------------------
void WinUIMessageProvider::GetMessage(
	HINSTANCE hInstance,		//!< instance handle
	SInt32 messageId,			//!< message id
	MBCString& message			//!< OUTPUT. message is returned
)
{
	// ほとんどの場合、小さいサイズのバッファで間に合うはずなので、
	// 最初の 1 つは動的に確保しない。
	char fixedBuf[256];
	char* buf = fixedBuf;
	int bufSize = sizeof(fixedBuf)/sizeof(char);
	char* dynBuf = NULL;

	while (true)
	{	
		int retSize = ::LoadString(hInstance, messageId, buf, bufSize);
		if (bufSize - 1 == retSize)
		{
			// バッファが足りなかった可能性があるので倍確保してみる
			bufSize *= 2;
			char* newBuf = static_cast<char*>(realloc(dynBuf, bufSize));
			if (NULL == newBuf)
			{
				free(dynBuf);
				MemoryException::Throw();
			}
			
			buf = dynBuf = newBuf;
		}
		else
		{
			message = buf;
			free(dynBuf);
			return;
		}
	}
}

// ---------------------------------------------------------------------
//! Retrieves and format message.
// ---------------------------------------------------------------------
void WinUIMessageProvider::GetNFormatMessage(
	MBCString& message,				//!< OUTPUT. formatted message is returned
	SInt32 messageId,				//!< message id
	...								//!< parameters
)
{
	va_list args;
	va_start(args, messageId);
	try
	{
		MBCString format;
		GetMessage(messageId, format);

		MessageFormatter::FormatV(message, format, args);
	}
	catch (...)
	{
		va_end(args);
		throw;
	}
	va_end(args);
}
