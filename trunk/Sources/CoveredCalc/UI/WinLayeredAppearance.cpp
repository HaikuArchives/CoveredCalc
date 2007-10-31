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
	@file		WinLayeredAppearance.cpp
	@brief		Implementation of WinLayeredAppearance class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.4.15 created
*/

#include "Prefix.h"
#include "WinLayeredAppearance.h"
#include "DIBColorLookup.h"
#include "ColorCodedSkin.h"
#include "MemoryException.h"
#include "WinCoveredCalcApp.h"

#define MAX_EDGE_SMOOTHING		10					///< エッジをぼかすレベルの最大値

// ---------------------------------------------------------------------
// ユーザ定義メッセージ
// ---------------------------------------------------------------------
#define UM_UPDATE_LAYERED		(WM_USER + 500)		///< レイヤードウィンドウの更新

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinLayeredAppearance::WinLayeredAppearance()
{
	hWnd = NULL;
	pixelWeights = NULL;
	width = 0;
	height = 0;
	isUpdateMessagePosted = false;
	ignoreClipping = false;
	edgeSmoothingLevel = 0;
	totalOpacity = 255;
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinLayeredAppearance::~WinLayeredAppearance()
{
	Exit();
}

// ---------------------------------------------------------------------
//! 初期化します。
// ---------------------------------------------------------------------
void WinLayeredAppearance::Init(
	HWND hWnd,					//!< スキンが描画されるウィンドウ
	bool ignoreClipping,		///< ウィンドウ領域のクリッピングを無視するかどうか (trueで無視)
	SInt32 edgeSmoothingLevel,	///< クリッピング時のエッジをぼかすレベル (0 ならぼかさない, 1以上でぼかす回数)
	SInt32 totalOpacity			///< 全体の不透明度 (1～255で指定)
)
{
	this->hWnd = hWnd;
	this->ignoreClipping = ignoreClipping;
	if (edgeSmoothingLevel > MAX_EDGE_SMOOTHING)
	{
		this->edgeSmoothingLevel = MAX_EDGE_SMOOTHING;
	}
	else
	{
		this->edgeSmoothingLevel = edgeSmoothingLevel;
	}
	if (totalOpacity < 1)
	{
		this->totalOpacity = 1;
	}
	else if (totalOpacity > 255)
	{
		this->totalOpacity = 255;
	}
	else
	{
		this->totalOpacity = static_cast<UInt8>(totalOpacity);
	}
	DWORD exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle | WinLayeredWindowAPI::cWS_EX_LAYERED);
}

// ---------------------------------------------------------------------
//! 終了処理を行います。
// ---------------------------------------------------------------------
void WinLayeredAppearance::Exit()
{
	if (pixelWeights != NULL)
	{
		free(pixelWeights);
		pixelWeights = NULL;
	}

	if (hWnd != NULL)
	{
		DWORD exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle & (~WinLayeredWindowAPI::cWS_EX_LAYERED));
		hWnd = NULL;
	}

	// 保持している WinDIBitmapDC を削除
	if (dibitmapDC.IsCreated())
	{
		dibitmapDC.Destroy();
	}
}

/**
 *	@brief	ウィンドウプロシージャの処理をアピアランスクラスへ渡します。
 *	@return	処理したら true.
 */
bool WinLayeredAppearance::RelayWndProc(
	HWND /*hWnd*/,				///< ウィンドウハンドル
	UINT uMsg,				///< メッセージ
	WPARAM /* wParam */,	///< メッセージの WPARAM
	LPARAM /* lParam */,	///< メッセージの LPARAM
	LRESULT* ret			///< 処理した場合、ウィンドウプロシージャの戻り値を格納します
)
{
	if (UM_UPDATE_LAYERED == uMsg)
	{
		if (isUpdateMessagePosted)
		{
			WinLayeredAppearance::UpdateAppearance();
		}
		
		*ret = 0;
		return true;
	}

	return false;
}

/**
 *	@brief	外観を更新します。（無効領域をなくします）
 */
void WinLayeredAppearance::UpdateAppearance()
{
	isUpdateMessagePosted = false;

	RECT rcWindow;
	::GetWindowRect(hWnd, &rcWindow);
	
	POINT windowTopLeft;
	windowTopLeft.x = rcWindow.left;
	windowTopLeft.y = rcWindow.top;
	
	SIZE windowSize;
	windowSize.cx = rcWindow.right - rcWindow.left;
	windowSize.cy = rcWindow.bottom - rcWindow.top;
	
	POINT surfaceTopLeft;
	surfaceTopLeft.x = surfaceTopLeft.y = 0;

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = totalOpacity;
	bf.AlphaFormat = AC_SRC_ALPHA;

	const WinLayeredWindowAPI* api = WinCoveredCalcApp::GetInstance()->GetLayeredWindowAPI();
	api->UpdateLayeredWindow(hWnd, NULL, &windowTopLeft, &windowSize, dibitmapDC, &surfaceTopLeft, 0, &bf, WinLayeredWindowAPI::cULW_ALPHA);
}

// ---------------------------------------------------------------------
//! スキンのサイズを変更されたときに呼ばれます。
// ---------------------------------------------------------------------
void WinLayeredAppearance::ChangeSkinSize(
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
	
	this->width = width;
	this->height = height;
}

// ---------------------------------------------------------------------
//! スキンの透明領域をウィンドウから切り取ります。
// ---------------------------------------------------------------------
void WinLayeredAppearance::ClipSkinRegion(
	const DIBitmap* mapBitmap,		//!< 色マップ用ビットマップ
	ColorValue transparentColor		//!< 透明色
)
{
	if (ignoreClipping)
	{
		return;
	}

	if (NULL != pixelWeights)
	{
		free(pixelWeights);
		pixelWeights = NULL;
	}

	pixelWeights = static_cast<UInt8*>(malloc(width * height));
	if (NULL == pixelWeights)
	{
		MemoryException::Throw();
	}
	memset(pixelWeights, 0xff, width * height * sizeof(UInt8));

	// いったん 0xff で初期化
	memset(pixelWeights, 0xff, width * height * sizeof(UInt8));

	// 透明部分を weight 0 にする
	DIBColorLookup mapColorLookup(mapBitmap);
	SInt32 row;
	for (row=0; row<height; row++)
	{
		SInt32 col;
		mapColorLookup.InitLocation(0, row);
		for (col=0; col<width; col++)
		{
			if (mapColorLookup.LookupNextColor() == transparentColor)
			{
				pixelWeights[row * width + col] = 0;
			}
		}
	}

	// ぼかす
	int count;
	for (count = 0; count < edgeSmoothingLevel; count++)
	{
		blurEdgeWeight();
	}
}

static UInt8 getWeight(UInt8* weights, SInt32 width, SInt32 height, SInt32 posX, SInt32 posY)
{
	if (posX < 0 || posX >= width || posY < 0 || posY >= height)
	{
		// 領域からはみ出る場合は重み 0
		return 0;
	}
	else
	{
		return weights[posY * width + posX];
	}
}

/**
 *	@brief	切り取りの端の重みをぼかします。
 */
void WinLayeredAppearance::blurEdgeWeight()
{
	// 各点とその 8 近傍の重みの平均をとることでぼかします。
	const SInt32 BLUR_RANGE	= 1;

	UInt8* tempWeights = static_cast<UInt8*>(malloc(width * height));
	if (NULL == tempWeights)
	{
		MemoryException::Throw();
	}
	try
	{
		UInt32 total;
		UInt8 oldest;

		// 横方向に平均をとる
		SInt32 posY, posX;
		for (posY = 0; posY < height; posY++)
		{
			oldest = 0;
			total = 0;
			for (posX = 0; posX < BLUR_RANGE; posX++)
			{
				total += getWeight(pixelWeights, width, height, posX, posY);
			}
			for (posX = 0; posX < width; posX++)
			{
				total += getWeight(pixelWeights, width, height, posX + BLUR_RANGE, posY) - oldest;
				oldest = getWeight(pixelWeights, width, height, posX - BLUR_RANGE, posY);
				tempWeights[posY * width + posX] = static_cast<UInt8>((total + BLUR_RANGE) / (2 * BLUR_RANGE + 1));	// 割る数の半分を先に足しておいて四捨五入をエミュレート				
			}
		}

		// 縦方向に平均をとる
		for (posX = 0; posX < width; posX++)
		{
			oldest = 0;
			total = 0;
			for (posY = 0; posY < BLUR_RANGE; posY++)
			{
				total += getWeight(tempWeights, width, height, posX, posY);
			}
			for (posY = 0; posY < height; posY++)
			{
				total += getWeight(tempWeights, width, height, posX, posY + BLUR_RANGE) - oldest;
				oldest = getWeight(tempWeights, width, height, posX, posY - BLUR_RANGE);
				// 最初から切り抜かれていた部分 (アルファ値0) は切り抜かれたままになるように
				if (pixelWeights[posY * width + posX] != 0)
				{
					pixelWeights[posY * width + posX] = static_cast<UInt8>((total + BLUR_RANGE) / (2 * BLUR_RANGE + 1));	// 割る数の半分を先に足しておいて四捨五入をエミュレート
				}
			}
		}
	}
	catch (...)
	{
		free(tempWeights);
		throw;
	}
	free(tempWeights);
}


// ---------------------------------------------------------------------
//! スキンの切り取りをなくします。
// ---------------------------------------------------------------------
void WinLayeredAppearance::UnclipSkinRegion()
{
	if (NULL != pixelWeights)
	{
		free(pixelWeights);
		pixelWeights = NULL;
	}
}

// ---------------------------------------------------------------------
//! 色マップ上の指定された色に対応する部分を描画します。
// ---------------------------------------------------------------------
void WinLayeredAppearance::DrawSkinByColor(
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
		line += 4 * drawPoint.x;
		skinColorLookup.InitLocation(skinRect.left, posY);
		mapColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			if (mapColorLookup.LookupNextColor() == color)
			{
				UInt32 alpha = static_cast<UInt32>(skinColor.alphaValue);
				if (NULL != pixelWeights)
				{
					alpha = alpha * pixelWeights[width * (posY - skinRect.top + drawPoint.y) + (posX - skinRect.left + drawPoint.x)] / 255;
				}
				line[0] = static_cast<Byte>(static_cast<UInt32>(skinColor.blueValue) * alpha / 255);
				line[1] = static_cast<Byte>(static_cast<UInt32>(skinColor.greenValue) * alpha / 255);
				line[2] = static_cast<Byte>(static_cast<UInt32>(skinColor.redValue) * alpha / 255);
				line[3] = static_cast<Byte>(alpha);
			}
			line += 4;
		}
	}
	
	updateLayeredWindow();
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
void WinLayeredAppearance::DrawBlendSkinByColor(
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
		line += 4 * drawPoint.x;
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
				UInt32 red = (skinColor1.redValue * ratio + skinColor2.redValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max;
				UInt32 green = (skinColor1.greenValue * ratio + skinColor2.greenValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max;
				UInt32 blue = (skinColor1.blueValue * ratio + skinColor2.blueValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max;
				UInt32 alpha = (skinColor1.alphaValue * ratio + skinColor2.alphaValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max;
				if (NULL != pixelWeights)
				{
					alpha = alpha * pixelWeights[width * (posY - skinRect.top + drawPoint.y) + (posX - skinRect.left + drawPoint.x)] / 255;
				}
				line[0] = static_cast<Byte>(blue * alpha / 255);
				line[1] = static_cast<Byte>(green * alpha / 255);
				line[2] = static_cast<Byte>(red * alpha / 255);
				line[3] = static_cast<Byte>(alpha);
			}
			line += 4;
		}
	}
	
	updateLayeredWindow();
}


// ---------------------------------------------------------------------
//! 指定された矩形のスキンを描画します。
// ---------------------------------------------------------------------
void WinLayeredAppearance::CopySkin(
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
		line += 4 * drawPoint.x;
		skinColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			UInt32 alpha = static_cast<UInt32>(skinColor.alphaValue);
			if (NULL != pixelWeights)
			{
				alpha = alpha * pixelWeights[width * (posY - skinRect.top + drawPoint.y) + (posX - skinRect.left + drawPoint.x)] / 255;
			}
			line[0] = static_cast<Byte>(static_cast<UInt32>(skinColor.blueValue) * alpha / 255);
			line[1] = static_cast<Byte>(static_cast<UInt32>(skinColor.greenValue) * alpha / 255);
			line[2] = static_cast<Byte>(static_cast<UInt32>(skinColor.redValue) * alpha / 255);
			line[3] = static_cast<Byte>(alpha);
			line += 4;
		}
	}

	updateLayeredWindow();
}

// ---------------------------------------------------------------------
//! 指定された矩形のスキンを描画します。ただし透明色に指定された色のピクセルはコピーしません。
// ---------------------------------------------------------------------
void WinLayeredAppearance::CopySkin(
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
		line += 4 * drawPoint.x;
		skinColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			if (skinColor != transparentColor)
			{
				UInt32 alpha = static_cast<UInt32>(skinColor.alphaValue);
				if (NULL != pixelWeights)
				{
					alpha = alpha * pixelWeights[width * (posY - skinRect.top + drawPoint.y) + (posX - skinRect.left + drawPoint.x)] / 255;
				}
				line[0] = static_cast<Byte>(static_cast<UInt32>(skinColor.blueValue) * alpha / 255);
				line[1] = static_cast<Byte>(static_cast<UInt32>(skinColor.greenValue) * alpha / 255);
				line[2] = static_cast<Byte>(static_cast<UInt32>(skinColor.redValue) * alpha / 255);
				line[3] = static_cast<Byte>(alpha);
			}
			line += 4;
		}
	}

	updateLayeredWindow();
}

/**
 *	@brief	レイヤードウィンドウを更新します。
 */
void WinLayeredAppearance::updateLayeredWindow()
{
	if (!isUpdateMessagePosted)
	{
		::PostMessage(hWnd, UM_UPDATE_LAYERED, 0, 0);
		isUpdateMessagePosted = true;
	}
}
