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
	@file		CoverDef.h
	@brief		Definition of CoverDef class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.16 created
*/

#ifndef _COVERDEF_H_
#define _COVERDEF_H_

#include <vector>
#include "CoverInfo.h"
#include "UTF8String.h"

// ---------------------------------------------------------------------
//! Information about definition of the cover(s).
/*!
	This class holds the contents of the file "CoverDef.xml".
*/
// ---------------------------------------------------------------------
class CoverDef
{
public:
							CoverDef();
	virtual					~CoverDef();

	void					SetAbout(ConstUTF8Str about)		{ this->about = about; }
	void					SetTitle(ConstUTF8Str title)		{ this->title = title; }
	void					SetDescription(ConstUTF8Str description)
																{ this->description = description; }
	void					AddCoverInfo(CoverInfo* coverInfo);

	void					Validate();
	
	ConstUTF8Str			GetAbout() const					{ return about; }
	ConstUTF8Str			GetTitle() const					{ return title; }
	ConstUTF8Str			GetDescription() const				{ return description; }
	const CoverInfo*		GetCoverInfo(SInt32 index) const	{ return coverInfos[index]; }
	SInt32					GetNumCoverInfos() const			{ return static_cast<SInt32>(coverInfos.size()); }

private:
	UTF8String				about;				//!< message about the cover
	UTF8String				title;				//!< title of the cover
	UTF8String				description;		//!< description of the cover
	std::vector<CoverInfo*>	coverInfos;			//!< array of CoverInfo objects
};

#endif // _COVERDEF_H_
