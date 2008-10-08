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
	@file		MessageFormatter.cpp
	@brief		Implementtion of MessageFormatter functions
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.01.17 created
*/

#include "Prefix.h"
#include <stdio.h>
#include <map>
#include "MessageFormatter.h"
#include "MemoryException.h"

#define REPLACE_BUFFER_LEN		1024
#define RECORD_SEPARATOR		ALITERAL('\x1e')

struct FormatParamInfo
{
	MBCString format;		//!< format string
	SInt32 startIndex;		//!< index of the first character
	SInt32 length;			//!< length of parameter string
};

// ---------------------------------------------------------------------
//! Format message.
/*!
	@note
	Since the code RS (Record Separator; 0x1E) is used as a separator in this function,
	RS cannot be used in a format string or the strings which is result of replacing parameters.
	If RS is used, the operation cannot be predicted.
	
	format sample:
		- Format(output, "There is %1:s% on %0:s%.", "the table", "an apple"); // output is "There is an apple on the table."
		- Format(output, "Your score is %0:d%.", 12345); // output is "Your score is 12345."
		- Format(output, "now, %0:d% %% done.", 25); // output is "now, 25 % done."
*/
// ---------------------------------------------------------------------
void MessageFormatter::Format(
	MBCString& message,		//!< OUTPUT. formatted message is returned
	ConstAStr format,		//!< message format
	...						//!< parameters
)
{
	va_list args;
	va_start(args, format);

	try
	{
		FormatV(message, format, args);
	}
	catch (...)
	{
		va_end(args);
		throw;
	}
	va_end(args);
}

// ---------------------------------------------------------------------
//! Format message using variable number of arguments list
// ---------------------------------------------------------------------
void MessageFormatter::FormatV(
	MBCString& message,		//!< OUTPUT. formatted message is returned
	ConstAStr format,		//!< message format
	va_list args			//!< parameters
)
{
	MBCString strFormat = format;

	typedef std::map<SInt32, FormatParamInfo> ParamMap;
	ParamMap paramMap;

	// search parameter string
	SInt32 searchStart = 0;
	while (true)
	{
		FormatParamInfo paramInfo;
		paramInfo.startIndex = strFormat.Find(ALITERAL('%'), searchStart);
		if (-1 == paramInfo.startIndex)
		{
			break;
		}
		if (ALITERAL('%') == format[paramInfo.startIndex + 1])		// "%%" -> "%"
		{
			strFormat.Replace(paramInfo.startIndex, 1, ALITERAL(""));
			searchStart = paramInfo.startIndex + 1;
			continue;
		}
		
		SInt32 colon = strFormat.Find(ALITERAL(':'), paramInfo.startIndex + 1);
		if (-1 == colon)
		{
			throw new MessageFormatException(format);
		}
		MBCString strOrder = strFormat.SubString(paramInfo.startIndex + 1, colon - paramInfo.startIndex);
		SInt32 order = ttoi(strOrder);
		SInt32 endPoint = strFormat.Find(ALITERAL('%'), colon + 1);
		if (-1 == endPoint)
		{
			throw new MessageFormatException(format);
		}
		paramInfo.format = strFormat.SubString(colon + 1, endPoint - colon - 1);
		paramInfo.length = endPoint - paramInfo.startIndex + 1;
		paramMap[order] = paramInfo;

		searchStart = endPoint + 1;
	}
	
	// get what are the parameters replaces to.
	SInt32 index = 0;
	MBCString replaceFormat;
	while (true)
	{
		ParamMap::const_iterator findIte = paramMap.find(index);
		if (findIte == paramMap.end())
		{
			break;
		}
		
		const FormatParamInfo& paramInfo = findIte->second;
		replaceFormat += MBCString(ALITERAL("%")) + paramInfo.format + RECORD_SEPARATOR;
		index++;
	}

	AChar replaceTosFixedBuf[REPLACE_BUFFER_LEN];
	AChar* replaceTos = replaceTosFixedBuf;
	SInt32 bufSize = sizeof(replaceTosFixedBuf)/sizeof(AChar);
	AChar* dynBuf = NULL;
	
	try
	{
		while (true)
		{
			memset(replaceTos, 0, bufSize * sizeof(AChar));
			vsntprintf(replaceTos, bufSize, replaceFormat.CString(), args);
			if (replaceTos[bufSize - 1] != ALITERAL('\0'))
			{
				// Since the buffer is too small for result,
				// it doubles buffer size and retry.
				bufSize *= 2;
				AChar* newBuf = static_cast<AChar*>(realloc(dynBuf, bufSize * sizeof(AChar)));
				if (NULL == newBuf)
				{
					free(dynBuf);
					MemoryException::Throw();
				}
				replaceTos = dynBuf = newBuf;
			}
			else
			{
				break;
			}
		}

		AChar* oneReplaceTo = replaceTos;
		for (index=0; *oneReplaceTo != ALITERAL('\0'); index++)
		{
			SInt32 findSeparator = MBCString(oneReplaceTo).Find(RECORD_SEPARATOR);
			if (-1 != findSeparator)
			{
				oneReplaceTo[findSeparator] = ALITERAL('\0');
			}
		
			ParamMap::const_iterator findIte = paramMap.find(index);
			ASSERT(findIte != paramMap.end());	// checked above.
			
			const FormatParamInfo& paramInfo = findIte->second;
			strFormat.Replace(paramInfo.startIndex, paramInfo.length, oneReplaceTo);

			SInt32 oneReplaceToLen = tcslen(oneReplaceTo);
			SInt32 diffChars = oneReplaceToLen - paramInfo.length;
			ParamMap::iterator modIte;
			for (modIte=paramMap.begin(); modIte!=paramMap.end(); modIte++)
			{
				FormatParamInfo& modInfo = modIte->second;
				if (modInfo.startIndex > paramInfo.startIndex)
				{
					modInfo.startIndex += diffChars;
				}
			}
		
			oneReplaceTo += oneReplaceToLen + 1;
		}
	}
	catch (...)
	{
		free(dynBuf);
		throw;
	}
	free(dynBuf);
	
	message = strFormat;
}
