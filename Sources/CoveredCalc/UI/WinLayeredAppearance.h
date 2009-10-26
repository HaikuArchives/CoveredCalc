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
	@file		WinLayeredAppearance.h
	@brief		Definition of WinLayeredAppearance class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.4.12 created
*/

#ifndef _WINLAYEREDAPPEARANCE_H_
#define _WINLAYEREDAPPEARANCE_H_

#include "WinCCSAppearance.h"
#include "WinDIBitmap32DC.h"

// ---------------------------------------------------------------------
//! Windows 用 ColorCodedSkinApperance のレイヤードウィンドウ用実装クラス
// ---------------------------------------------------------------------
class WinLayeredAppearance : public WinCCSAppearance
{
public:
							WinLayeredAppearance();
	virtual					~WinLayeredAppearance();

	void					Init(HWND hWnd, bool ignoreClipping, SInt32 edgeSmoothingLevel, SInt32 totalOpacity);
	void					Exit();
	
	virtual bool			RelayWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* ret);
	virtual void			UpdateAppearance();

	virtual void			ChangeSkinSize(SInt32 width, SInt32 height);
	virtual void			ClipSkinRegion(const DIBitmap* mapBitmap, ColorValue transparentColor);
	virtual void			UnclipSkinRegion();
	virtual	void			UpdateMapArea(const Rect32& areaRect, const DIBitmap* mapBitmap, ColorValue color, const DIBitmap* skinBitmap);
	virtual void			UpdateMapAreaWithBlend(const Rect32& areaRect, const DIBitmap* mapBitmap, ColorValue color, const DIBitmap* skinBitmap1, const DIBitmap* skinBitmap2, UInt32 ratio);
	virtual	void			UpdateRect(const Rect32& skinRect, const DIBitmap* skinBitmap);
	virtual void			OverpaintImage(const Point32& drawPoint, const DIBitmap* sourceBitmap, const Rect32& sourceRect, const ColorValue* transparentColor);

private:
	void					blurEdgeWeight();
	void					updateLayeredWindow();

private:
	HWND					hWnd;
	WinDIBitmap32DC			dibitmapDC;
	UInt8*					pixelWeights;
	SInt32					width;
	SInt32					height;
	bool					isUpdateMessagePosted;	///< アップデート用メッセージをポストしたかどうか
	bool					ignoreClipping;			///< スキン領域のクリッピングを無視するかどうか (trueで無視)
	SInt32					edgeSmoothingLevel;		///< クリッピング時のエッジをぼかすレベル (0 ならぼかさない, 1以上でぼかす回数)
	UInt8					totalOpacity;			///< 全体の不透明度
};

#endif // _WINLAYEREDAPPEARANCE_H_

