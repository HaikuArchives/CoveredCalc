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
	@file		PathUtils.h
	@brief		Path utilities.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.17 created
*/

#ifndef _PATHUTILS_H_
#define _PATHUTILS_H_

#include "MBCString.h"

//! Path utilities
namespace PathUtils
{
	MBCString			AddSeparator(ConstAStr path);
	MBCString			RemoveSeparator(ConstAStr path);
	MBCString			Append(ConstAStr path, ConstAStr more);
	MBCString			RemoveFileSpec(ConstAStr path);
	MBCString			GetParentDirectory(ConstAStr path);
	MBCString			GetFileTitle(ConstAStr path);
	bool				IsRoot(ConstAStr path);
	SInt32				ComparePath(ConstAStr left, ConstAStr right);
	MBCString			MakeRelativePath(ConstAStr base, ConstAStr target);
};

#endif // _PATHUTILS_H_
