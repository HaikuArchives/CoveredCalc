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
	@file		FileConstants.h
	@brief		Definition of constatnt for File (or its base) class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.01.15 created
*/

#ifndef _FILECONSTANTS_H_
#define _FILECONSTANTS_H_

namespace FileConstants
{
	enum OpenMode {
		OpenMode_ReadOnly = 1,
		OpenMode_WriteOnly,
		OpenMode_ReadWrite
	};

	enum OpenOption {
		OpenOption_None = 0x00,			//!< no option
		OpenOption_Create = 0x01,		//!< create if not exists
		OpenOption_FailIfExists = 0x02,	//!< fail if exists
		OpenOption_Erase = 0x04			//!< erase the data if already exists
	};

	enum SeekOrigin {
		SeekOrigin_Begin = 1,
		SeekOrigin_Current,
		SeekOrigin_End
	};	
};

#endif // _FILECONSTANTS_H_
