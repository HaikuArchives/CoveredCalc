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
	@file		BeCCColumnListView.cpp
	@brief		Implementation of BeCCColumnListView class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.11.10 created
*/

#include "Prefix.h"
#include "BeCCColumnListView.h"

// ---------------------------------------------------------------------
//! Makes rgb_color from each member value.
// ---------------------------------------------------------------------
static rgb_color makeColor(uchar r, uchar g, uchar b, uchar a)
{
	rgb_color color;
	color.red = r;
	color.green = g;
	color.blue = b;
	color.alpha = a;
	return color;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeCCColumnListView::BeCCColumnListView(
	BRect rect,									///< view rect
	const char *name,							///< name
	uint32 resizingMode,						///< resizing mode
	uint32 drawFlags,							///< flags
	border_style border /* = B_NO_BORDER */,	///< border style
	bool showHorizontalScrollbar /* = true */	///< whether show horizontal scrollbar
)
	: BColumnListView(rect, name, resizingMode, drawFlags, border, showHorizontalScrollbar)
{
	// default color setting.
	SetColor(B_COLOR_BACKGROUND,			makeColor(255, 255, 255, 255));
	SetColor(B_COLOR_TEXT,					makeColor(0, 0, 0, 255));
	SetColor(B_COLOR_ROW_DIVIDER,			makeColor(255, 255, 255, 255));
//	SetColor(B_COLOR_SELECTION,				makeColor(0, 0, 0, 255));
	SetColor(B_COLOR_SELECTION,				makeColor(192, 192, 192, 255));
//	SetColor(B_COLOR_SELECTION_TEXT,		makeColor(255, 255, 255, 255));
	SetColor(B_COLOR_SELECTION_TEXT,		makeColor(0, 0, 0, 255));
//	SetColor(B_COLOR_NON_FOCUS_SELECTION,	makeColor(0, 0, 0, 255));	
	SetColor(B_COLOR_NON_FOCUS_SELECTION,	makeColor(192, 192, 192, 255));	
}
