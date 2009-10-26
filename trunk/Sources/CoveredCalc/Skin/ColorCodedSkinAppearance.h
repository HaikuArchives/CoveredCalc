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
	@file		ColorCodedSkinAppearance.h
	@brief		Definition of ColorCodedSkinAppearance interface.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.17 created
*/

#ifndef _COLORCODEDSKINAPPEARANCE_H_
#define _COLORCODEDSKINAPPEARANCE_H_

#include "ColorValue.h"

class DIBitmap;

// ---------------------------------------------------------------------
//! This class manages skin appearance.
/*!
	@note
	This is interface, must be inherited.
*/
// ---------------------------------------------------------------------
class ColorCodedSkinAppearance
{
public:
						ColorCodedSkinAppearance()		{ }
	virtual				~ColorCodedSkinAppearance()		{ }
	
	virtual void		ChangeSkinSize(SInt32 width, SInt32 height) = 0;
	virtual void		ClipSkinRegion(const DIBitmap* mapBitmap, ColorValue transparentColor) = 0;
	virtual void		UnclipSkinRegion() = 0;
	virtual	void		UpdateMapArea(const Rect32& areaRect, const DIBitmap* mapBitmap, ColorValue color, const DIBitmap* skinBitmap) = 0;
	virtual void		UpdateMapAreaWithBlend(const Rect32& areaRect, const DIBitmap* mapBitmap, ColorValue color, const DIBitmap* skinBitmap1, const DIBitmap* skinBitmap2, UInt32 ratio) = 0;
	virtual	void		UpdateRect(const Rect32& skinRect, const DIBitmap* skinBitmap) = 0;
	virtual void		OverpaintImage(const Point32& drawPoint, const DIBitmap* sourceBitmap, const Rect32& sourceRect, const ColorValue* transparentColor) = 0;
};

#endif // _COLORCODEDSKINAPPEARANCE_H_
