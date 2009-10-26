/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2009 CoveredCalc Project Contributors
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
	@file		CoverFontInfo.h
	@brief		Definition of CoverFontInfo class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.01.05 created
*/

#ifndef _COVERFONTINFO_H_
#define _COVERFONTINFO_H_

#include "ColorValue.h"

// ---------------------------------------------------------------------
//! Information about font in the cover.
/*!
	This class holds the contents of one <font> tag in file "CoverDef.xml".
*/
// ---------------------------------------------------------------------
class CoverFontInfo
{
public:
	enum Type
	{
		Type_Number = 0,	//!< number only ('0' to '9', 'A' to 'F' and '.')
	};

public:
						CoverFontInfo();
	virtual				~CoverFontInfo();
	
	void				SetType(Type type)						{ this->type = type; }
	void				SetWidth(SInt32 width)					{ this->width = width; }
	void				SetHeight(SInt32 height)				{ this->height = height; }
	void				SetCharSpace(SInt32 charSpace)			{ this->charSpace = charSpace; }
	void				SetTransparentColor(ColorValue color);
	void				UnsetTransparentColor();

	Type				GetType() const							{ return type; }
	SInt32				GetWidth() const						{ return width; }
	SInt32				GetHeight() const						{ return height; }
	SInt32				GetCharSpace() const					{ return charSpace; }
	const ColorValue*	GetTransparentColor() const;
	
private:
	Type				type;				//!< type of information
	SInt32				width;				//!< width of one character in the font in pixel unit
	SInt32				height;				//!< height of one character in the font in pixel unit
	SInt32				charSpace;			//!< horizontal space between two characters
	ColorValue			transparentColor;	//!< transparent color
	bool				isTransparentColorSpecified;	//!< whether transparentColor is specified or not.
};

#endif // _COVERFONTINFO_H_
