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
	@file		CoverToolTipInfo.h
	@brief		Definition of CoverToolTipInfo class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.12 created
*/

#ifndef _COVERTOOLTIPINFO_H_
#define _COVERTOOLTIPINFO_H_

#include "UTF8String.h"

// ---------------------------------------------------------------------
//! Information about a tool tip in the cover.
/*!
	This class holds the contents of one <tooltip> tag in file "CoverDef.xml".
*/
// ---------------------------------------------------------------------
class CoverToolTipInfo
{
public:
						CoverToolTipInfo();
	virtual				~CoverToolTipInfo();
	
	void				SetMapID(ConstUTF8Str id)				{ this->mapID = id; }
	void				SetMapIndex(SInt32 mapIndex)			{ this->mapIndex = mapIndex; }
	void				SetToolTipText(ConstUTF8Str text)		{ this->text = text; }
	
	ConstUTF8Str		GetMapID() const						{ return mapID; }
	SInt32				GetMapIndex() const						{ return mapIndex; }
	ConstUTF8Str		GetToolTipText() const					{ return text; }
	
private:
	UTF8String			mapID;					//!< id of mapItems for this button
	SInt32				mapIndex;				//!< index of mapItems for this button
	UTF8String			text;					//!< tool tip text
};

#endif // _COVERTOOLTIPINFO_H_
