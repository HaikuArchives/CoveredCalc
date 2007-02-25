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
	@file		WinCCSAppearance.h
	@brief		Definition of WinCCSAppearance class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.10.13 created
*/

#ifndef _WINCCSAPPEARANCE_H_
#define _WINCCSAPPEARANCE_H_

#include "ColorCodedSkinAppearance.h"
#include "WinDIBitmapDC.h"

// ---------------------------------------------------------------------
//! Windows �p�� ColorCodedSkinApperance �����N���X
// ---------------------------------------------------------------------
class WinCCSAppearance : public ColorCodedSkinAppearance
{
public:
							WinCCSAppearance();
	virtual					~WinCCSAppearance();

	void					Init(HWND hWnd);
	void					Exit();
	
	const WinDIBitmapDC*	GetDIBitmapDC() const { return &dibitmapDC; }

	virtual void			ChangeSkinSize(SInt32 width, SInt32 height);
	virtual void			ClipSkinRegion(const DIBitmap* mapBitmap, ColorValue transparentColor);
	virtual void			UnclipSkinRegion();
	virtual	void			DrawSkinByColor(const Point32& drawPoint, const DIBitmap* mapBitmap, const DIBitmap* skinBitmap, ColorValue color, const Rect32& skinRect);
	virtual void			DrawBlendSkinByColor(const Point32& drawPoint, const DIBitmap* mapBitmap, const DIBitmap* skinBitmap1, const DIBitmap* skinBitmap2, ColorValue color, const Rect32& skinRect, UInt32 ratio);
	virtual	void			CopySkin(const Point32& drawPoint, const DIBitmap* skinBitmap, const Rect32& skinRect);
	virtual	void			CopySkin(const Point32& drawPoint, const DIBitmap* skinBitmap, const Rect32& skinRect, ColorValue transparentColor);

private:
	void					invalidateRect(const Point32& drawPoint, const Rect32& skinRect);

private:
	HWND					hWnd;
	WinDIBitmapDC			dibitmapDC;
	HRGN					hWndRgn;
};

#endif // _WINCCSAPPEARANCE_H_