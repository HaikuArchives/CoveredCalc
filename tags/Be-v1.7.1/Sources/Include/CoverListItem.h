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
	@file		CoverListItem.h
	@brief		Definition of CoverListItem class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.03.07 created
*/

#ifndef _COVERLISTITEM_H_
#define _COVERLISTITEM_H_

#include "Path.h"
#include "UTF8String.h"

// ---------------------------------------------------------------------
//! An item in cover list
// ---------------------------------------------------------------------
class CoverListItem
{
public:
						CoverListItem() { }
	virtual				~CoverListItem() { }

	const Path&			GetPath() const { return path; }
	ConstUTF8Str		GetTitle() const { return title.CString(); }
	ConstUTF8Str		GetDescription() const { return description.CString(); }

	void				SetPath(const Path& path)
							{ this->path = path; }
	void				SetTitle(ConstUTF8Str title)
							{ this->title = title; }
	void				SetDescription(ConstUTF8Str description)
							{ this->description = description; }
					
private:
	Path				path;			//!< path of CoverDef.xml
	UTF8String			title;			//!< title of the cover.
	UTF8String			description;	//!< description of the cover
};

#endif // _COVERLISTITEM_H_
