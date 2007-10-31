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
	@file		WinNormalAppearance.cpp
	@brief		Implementation of WinNormalAppearance class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.4.7 created
*/

#include "Prefix.h"
#include "WinNormalAppearance.h"
#include "DIBColorLookup.h"
#include "ColorCodedSkin.h"

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinNormalAppearance::WinNormalAppearance()
{
	hWnd = NULL;
	hWndRgn = NULL;
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinNormalAppearance::~WinNormalAppearance()
{
	Exit();
}

// ---------------------------------------------------------------------
//! 初期化します。
// ---------------------------------------------------------------------
void WinNormalAppearance::Init(
	HWND hWnd			//!< スキンが描画されるウィンドウ
)
{
	this->hWnd = hWnd;
	this->hWndRgn = NULL;
}

// ---------------------------------------------------------------------
//! 終了処理を行います。
// ---------------------------------------------------------------------
void WinNormalAppearance::Exit()
{
	// ウィンドウの切り取りを元に戻す
	if (NULL != hWndRgn)
	{
		::SetWindowRgn(hWnd, NULL, TRUE);
		::DeleteObject(hWndRgn);
		hWndRgn = NULL;
	}

	// 保持している WinDIBitmapDC を削除
	if (dibitmapDC.IsCreated())
	{
		dibitmapDC.Destroy();
	}
	
	hWnd = NULL;
}

/**
 *	@brief	ウィンドウプロシージャの処理をアピアランスクラスへ渡します。
 *	@return	処理したら true.
 */
bool WinNormalAppearance::RelayWndProc(
	HWND hWnd,				///< ウィンドウハンドル
	UINT uMsg,				///< メッセージ
	WPARAM /* wParam */,	///< メッセージの WPARAM
	LPARAM /* lParam */,	///< メッセージの LPARAM
	LRESULT* ret			///< 処理した場合、ウィンドウプロシージャの戻り値を格納します
)
{
	if (WM_PAINT == uMsg)
	{
		PAINTSTRUCT ps;
		HDC dc = NULL;

		RECT rect;
		::GetClientRect(hWnd, &rect);
		
		dc = ::BeginPaint(hWnd, &ps);
		if (NULL != dc)
		{
			try
			{	
				::BitBlt(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dibitmapDC, 0, 0, SRCCOPY);
			}
			catch (...)
			{
				::EndPaint(hWnd, &ps);
				throw;
			}
			::EndPaint(hWnd, &ps);
		}
		
		*ret = 0;
		return true;
	}

	return false;
}

/**
 *	@brief	外観を更新します。（無効領域をなくします）
 */
void WinNormalAppearance::UpdateAppearance()
{
	::UpdateWindow(hWnd);
}

// ---------------------------------------------------------------------
//! スキンのサイズを変更されたときに呼ばれます。
// ---------------------------------------------------------------------
void WinNormalAppearance::ChangeSkinSize(
	SInt32 width,		//!< 新しい幅
	SInt32 height		//!< 新しい高さ
)
{
	// 保持している WinDIBitmapDC を再作成
	if (dibitmapDC.IsCreated())
	{
		dibitmapDC.Destroy();
	}
	
	HDC hDC = ::GetDC(hWnd);
	try
	{
		dibitmapDC.Create(hDC, width, height);
	}
	catch (...)
	{
		::ReleaseDC(hWnd, hDC);
		throw;
	}
	::ReleaseDC(hWnd, hDC);	

	// ウィンドウのサイズを変更
	RECT winRect;
	::GetWindowRect(hWnd, &winRect);
	::MoveWindow(hWnd, winRect.left, winRect.top, width, height, FALSE);
}

// ---------------------------------------------------------------------
//! スキンの透明領域をウィンドウから切り取ります。
// ---------------------------------------------------------------------
void WinNormalAppearance::ClipSkinRegion(
	const DIBitmap* mapBitmap,		//!< 色マップ用ビットマップ
	ColorValue transparentColor		//!< 透明色
)
{
	// すでに切り取っていれば切り取りを元に戻す
	if (NULL != hWndRgn)
	{
		::SetWindowRgn(hWnd, NULL, TRUE);
		::DeleteObject(hWndRgn);
		hWndRgn = NULL;
	}
	
	hWndRgn = ::CreateRectRgn(0, 0, 0, 0);
	
	// 透明でない部分をリージョンに追加
	DIBColorLookup mapColorLookup(mapBitmap);
	SInt32 width = mapBitmap->GetWidth();
	SInt32 height = mapBitmap->GetHeight();
	HRGN rgn;
	SInt32 row;
	for (row=0; row<height; row++)
	{
		SInt32 contiguousPixels = 0;
		SInt32 col;
		mapColorLookup.InitLocation(0, row);
		for (col=0; col<width; col++)
		{
			if (mapColorLookup.LookupNextColor() != transparentColor)
			{
				contiguousPixels++;
			}
			else
			{
				if (0 != contiguousPixels)
				{
					rgn = ::CreateRectRgn(col - contiguousPixels, row, col, row + 1);
					::CombineRgn(hWndRgn, hWndRgn, rgn, RGN_OR);
					::DeleteObject(rgn);
					contiguousPixels = 0;
				}
			}
		}
		if (0 != contiguousPixels)
		{
			rgn = ::CreateRectRgn(col - contiguousPixels, row, col, row + 1);
			::CombineRgn(hWndRgn, hWndRgn, rgn, RGN_OR);
			::DeleteObject(rgn);
		}		
	}
	
	// ウィンドウを切り取り
	::SetWindowRgn(hWnd, hWndRgn, TRUE);
}

// ---------------------------------------------------------------------
//! スキンの切り取りをなくします。
// ---------------------------------------------------------------------
void WinNormalAppearance::UnclipSkinRegion()
{
	// すでに切り取っていれば切り取りを元に戻す
	if (NULL != hWndRgn)
	{
		::SetWindowRgn(hWnd, NULL, TRUE);
		::DeleteObject(hWndRgn);
		hWndRgn = NULL;
	}
}

// ---------------------------------------------------------------------
//! 色マップ上の指定された色に対応する部分を描画します。
// ---------------------------------------------------------------------
void WinNormalAppearance::DrawSkinByColor(
	const Point32& drawPoint,		//!< 描画先の左上座標
	const DIBitmap* mapBitmap,		//!< 色マップ用ビットマップ
	const DIBitmap* skinBitmap,		//!< 描画元スキン用ビットマップ
	ColorValue color,				//!< 色
	const Rect32& skinRect			//!< 描画元の描画する領域
)
{
	if (!dibitmapDC.IsCreated())
	{
		ASSERT(false);
		return;
	}

	DIBitmap* bitmap = dibitmapDC.GetDIB();
	DIBColorLookup mapColorLookup(mapBitmap);
	DIBColorLookup skinColorLookup(skinBitmap);
	
	SInt32 posY;
	for (posY=skinRect.top; posY<=skinRect.bottom; posY++)
	{
		Byte* line = bitmap->GetBitsLineAddress(posY - skinRect.top + drawPoint.y);
		line += 3 * drawPoint.x;
		skinColorLookup.InitLocation(skinRect.left, posY);
		mapColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			if (mapColorLookup.LookupNextColor() == color)
			{
				line[0] = skinColor.blueValue;
				line[1] = skinColor.greenValue;
				line[2] = skinColor.redValue;
			}
			line += 3;
		}
	}
	
	invalidateRect(drawPoint, skinRect);
}

/**
 *	@brief	色マップ上の指定された色に対応する部分を
 *			2 つのスキン用ビットマップで混合した色で描画します。
 *	@param	drawPoint	描画先の左上座標
 *	@param	mapBitmap	色マップ用ビットマップ
 *	@param	skinBitmap1	描画元スキン用ビットマップ1
 *	@param	skinBitmap2	描画元スキン用ビットマップ2
 *	@param	color		色
 *	@param	skinRect	描画元の描画する領域
 *	@para,	ratio		混合率 (0～ColorCodedSkin::BlendRatio_Max)
 */
void WinNormalAppearance::DrawBlendSkinByColor(
	const Point32& drawPoint,
	const DIBitmap* mapBitmap,
	const DIBitmap* skinBitmap1,
	const DIBitmap* skinBitmap2,
	ColorValue color,
	const Rect32& skinRect,
	UInt32 ratio
)
{
	if (!dibitmapDC.IsCreated())
	{
		ASSERT(false);
		return;
	}

	DIBitmap* bitmap = dibitmapDC.GetDIB();
	DIBColorLookup mapColorLookup(mapBitmap);
	DIBColorLookup skinColorLookup1(skinBitmap1);
	DIBColorLookup skinColorLookup2(skinBitmap2);
	
	SInt32 posY;
	for (posY=skinRect.top; posY<=skinRect.bottom; posY++)
	{
		Byte* line = bitmap->GetBitsLineAddress(posY - skinRect.top + drawPoint.y);
		line += 3 * drawPoint.x;
		skinColorLookup1.InitLocation(skinRect.left, posY);
		skinColorLookup2.InitLocation(skinRect.left, posY);
		mapColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor1 = skinColorLookup1.LookupNextColor();
			ColorValue skinColor2 = skinColorLookup2.LookupNextColor();
			if (mapColorLookup.LookupNextColor() == color)
			{
				line[0] = static_cast<Byte>((skinColor1.blueValue * ratio + skinColor2.blueValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max);
				line[1] = static_cast<Byte>((skinColor1.greenValue * ratio + skinColor2.greenValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max);
				line[2] = static_cast<Byte>((skinColor1.redValue * ratio + skinColor2.redValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max);
			}
			line += 3;
		}
	}
	
	invalidateRect(drawPoint, skinRect);
}


// ---------------------------------------------------------------------
//! 指定された矩形のスキンを描画します。
// ---------------------------------------------------------------------
void WinNormalAppearance::CopySkin(
	const Point32& drawPoint,		//!< 描画先の左上座標
	const DIBitmap* skinBitmap,		//!< 描画元スキン用ビットマップ
	const Rect32& skinRect			//!< 描画元の描画する領域
)
{
	if (!dibitmapDC.IsCreated())
	{
		ASSERT(false);
		return;
	}

	DIBitmap* bitmap = dibitmapDC.GetDIB();
	DIBColorLookup skinColorLookup(skinBitmap);
	
	SInt32 posY;
	for (posY=skinRect.top; posY<=skinRect.bottom; posY++)
	{
		Byte* line = bitmap->GetBitsLineAddress(posY - skinRect.top + drawPoint.y);
		line += 3 * drawPoint.x;
		skinColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			line[0] = skinColor.blueValue;
			line[1] = skinColor.greenValue;
			line[2] = skinColor.redValue;
			line += 3;
		}
	}

	invalidateRect(drawPoint, skinRect);
}

// ---------------------------------------------------------------------
//! 指定された矩形のスキンを描画します。ただし透明色に指定された色のピクセルはコピーしません。
// ---------------------------------------------------------------------
void WinNormalAppearance::CopySkin(
	const Point32& drawPoint,		//!< 描画先の左上座標
	const DIBitmap* skinBitmap,		//!< 描画元スキン用ビットマップ
	const Rect32& skinRect,			//!< 描画元の描画する領域
	ColorValue transparentColor		//!< 透明色
)
{
	if (!dibitmapDC.IsCreated())
	{
		ASSERT(false);
		return;
	}

	DIBitmap* bitmap = dibitmapDC.GetDIB();
	DIBColorLookup skinColorLookup(skinBitmap);
	
	SInt32 posY;
	for (posY=skinRect.top; posY<=skinRect.bottom; posY++)
	{
		Byte* line = bitmap->GetBitsLineAddress(posY - skinRect.top + drawPoint.y);
		line += 3 * drawPoint.x;
		skinColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			if (skinColor != transparentColor)
			{
				line[0] = skinColor.blueValue;
				line[1] = skinColor.greenValue;
				line[2] = skinColor.redValue;
			}
			line += 3;
		}
	}

	invalidateRect(drawPoint, skinRect);
}

// ---------------------------------------------------------------------
//! 指定された矩形を無効化します。
// ---------------------------------------------------------------------
void WinNormalAppearance::invalidateRect(
	const Point32& drawPoint,		//!< 描画先の左上座標
	const Rect32& skinRect			//!< 描画元の描画する領域
)
{
	RECT invRect;
	invRect.left = drawPoint.x;
	invRect.top = drawPoint.y;
	invRect.right = drawPoint.x + skinRect.right - skinRect.left + 1;	// Windows 座標系はピクセル間に座標があるので +1 する。
	invRect.bottom = drawPoint.y + skinRect.bottom - skinRect.top + 1;	//  0  1  2		... ピクセル座標
																		// |■|■|■|
																		// 0  1  2  3   ... Windows 座標
	::InvalidateRect(hWnd, &invRect, TRUE);
}
