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
	@file		BeDialogControlHelper.cpp
	@brief		Implementation of BeDialogControlHelper class.
	@author 	ICHIMIYA Hironori (Hiron)
	@date		2008.11.29 created
*/

#include "Prefix.h"
#include <cmath>
#include <Font.h>
#include "BeDialogControlHelper.h"
#include "DialogLayout.h"

/**
 *	@brief	Constructor
 *	@param[in]	layout	dialog layout object (specify computed one)
 */
BeDialogControlHelper::BeDialogControlHelper(const DialogLayout* layout)
{
	this->layout = layout;

	font_height fh;
	be_plain_font->GetHeight(&fh);
	fontAverageHeight = static_cast<int>(std::floor(fh.ascent + fh.descent));
	
	float width = be_plain_font->StringWidth("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	fontAverageWidth = static_cast<int>(std::floor(width / 52));
}

/**
 *	@brief	Destructor
 */
BeDialogControlHelper::~BeDialogControlHelper()
{
}

/**
 *	@brief	Converts DLU (dialog unit) to pixel,
 *	@param[in]	isHorizontal	true for x-coordinates, or false for y-coordinates.
 *	@param[in]	dlu				DLU value.
 *	@return pixel value.
 */
float BeDialogControlHelper::ConvertDLUToPixel(bool isHorizontal, float dlu)
{
	if (isHorizontal)
	{
		return static_cast<int>(dlu) * fontAverageWidth / 4;
	}
	else
	{
		return static_cast<int>(dlu) * fontAverageHeight / 8;
	}
}

/**
 *	@brief	Retrieves item position
 *	@param[in]	isHorizontal	true for x-coordinates, or false for y-coordinates.
 *	@param[in]	layoutName	name of item pos in dialog layout.
 *	@param[in]	baseLayoutName	if this is not empty, returned item position is offset from this item pos.
 *	@return	item position.
 */
float BeDialogControlHelper::GetItemPos(bool isHorizontal, const MBCString& layoutName, const MBCString& baseLayoutName)
{
	float dlu = layout->GetComputedValue(layoutName);
	if (!baseLayoutName.IsEmpty())
	{
		dlu -= layout->GetComputedValue(baseLayoutName);
	}
	return ConvertDLUToPixel(isHorizontal, dlu);
}

/**
 *	@brief	Retrieves item rectangle.
 *	@param[in]	layoutName	name of rect item. this name does not include postfix such as ".left".
 *	@param[in]	baseLayoutName	name of base rect item.
 *	@return	item rect from top-left of the base item specified in baseLayoutName.
 */
BRect BeDialogControlHelper::GetItemRect(const MBCString& layoutName, const MBCString& baseLayoutName)
{
	MBCString baseLeft, baseTop;
	if (!baseLayoutName.IsEmpty())
	{
		baseLeft = baseLayoutName + ALITERAL(".left");
		baseTop = baseLayoutName + ALITERAL(".top");
	}
		
	float left = GetItemPos(true, layoutName + ALITERAL(".left"), baseLeft);
	float right = GetItemPos(true, layoutName + ALITERAL(".right"), baseLeft);
	float top = GetItemPos(false, layoutName + ALITERAL(".top"), baseTop);
	float bottom = GetItemPos(false, layoutName + ALITERAL(".bottom"), baseTop);
	return BRect(left, top, right, bottom);
}
