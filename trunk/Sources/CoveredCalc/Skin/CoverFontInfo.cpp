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
	@file		CoverFontInfo.cpp
	@brief		Implemantation of CoverFontInfo class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.01.05 created
*/

#include "Prefix.h"
#include "CoverFontInfo.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverFontInfo::CoverFontInfo() : transparentColor(0, 0, 0)
{
	type = Type_Number;
	width = 0;
	height = 0;
	charSpace = 0;
	isTransparentColorSpecified = false;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverFontInfo::~CoverFontInfo()
{
}

/**
 * @brief Sets transparent color.
 * @param[in] color transparent color
 */
void CoverFontInfo::SetTransparentColor(ColorValue color)
{
	this->transparentColor = color;
	this->isTransparentColorSpecified = true;
}

/**
 * @brief Unsets transparent color.
 */
void CoverFontInfo::UnsetTransparentColor()
{
	this->isTransparentColorSpecified = false;
}

/**
 * @brief Retrieves transaparent color.
 * @return pointer to transparent color, or NULL if not specified.
 */
const ColorValue* CoverFontInfo::GetTransparentColor() const
{
	if (isTransparentColorSpecified)
	{
		return &transparentColor;
	}
	else
	{
		return NULL;
	}
}	
