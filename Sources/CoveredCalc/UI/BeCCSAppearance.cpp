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
	@file		BeCCSAppearance.cpp
	@brief		Implementation of BeCCSAppearance class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.10.27 created
*/

#include "Prefix.h"
#include <InterfaceKit.h>
#include <support/Autolock.h>
#include "BeCCSAppearance.h"
#include "DIBColorLookup.h"
#include "DIBitmap.h"
#include "ColorCodedSkin.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeCCSAppearance::BeCCSAppearance()
{
	view = NULL;
	bitmap = NULL;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeCCSAppearance::~BeCCSAppearance()
{
	Exit();
}

// ---------------------------------------------------------------------
//! Initializes the object
// ---------------------------------------------------------------------
void BeCCSAppearance::Init(
	BView* view			//!< the view invatedated when drawing
)
{
	this->view = view;
}

void BeCCSAppearance::Exit()
{
	if (NULL != bitmap)
	{
		delete bitmap;
		bitmap = NULL;
	}
	view = NULL;
}

// ---------------------------------------------------------------------
//! Called when size of skin is changed.
// ---------------------------------------------------------------------
void BeCCSAppearance::ChangeSkinSize(
	SInt32 width,		//!< new width
	SInt32 height		//!< new height
)
{
	// remake the offscreen bitmap.
	if (NULL != bitmap)
	{
		delete bitmap;
		bitmap = NULL;
	}
	
	BRect bounds(0, 0, width - 1, height - 1);
	bitmap = new BBitmap(bounds, B_RGB32);
	
	// resize window
	BWindow* window = view->Window();
	if (NULL != window)
	{
		BAutolock locker(window);
		window->ResizeTo(width - 1, height - 1);
	}
}

// ---------------------------------------------------------------------
//! Clips Skin.
// ---------------------------------------------------------------------
void BeCCSAppearance::ClipSkinRegion(
	const DIBitmap* mapBitmap,		//!< map bitmap.
	ColorValue transparentColor		//!< transparent color.
)
{
#if B_BEOS_VERSION >= 0x0510
	BWindow* window = view->Window();
	if (NULL != window)
	{
		BAutolock locker(window);
	
		view->BeginPicture(new BPicture());
	
		DIBColorLookup mapColorLookup(mapBitmap);
		SInt32 width = mapBitmap->GetWidth();
		SInt32 height = mapBitmap->GetHeight();
	
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
						view->FillRect(BRect(col - contiguousPixels, row, col - 1, row));
						contiguousPixels = 0;
					}
				}
			}
			if (0 != contiguousPixels)
			{
				view->FillRect(BRect(col - contiguousPixels, row, col - 1, row));
			}		
		}
		
		BPicture* picture = view->EndPicture();
		window->ClipWindowToPicture(picture, BPoint(0, 0), 0);
		delete picture;
	}
#endif
}

// ---------------------------------------------------------------------
//! Restores clipped skin.
// ---------------------------------------------------------------------
void BeCCSAppearance::UnclipSkinRegion()
{
#if B_BEOS_VERSION >= 0x0510
	BWindow* window = view->Window();
	if (NULL != window)
	{
		BAutolock locker(window);
		BRect wholeWindowRect = window->Bounds();

		view->BeginPicture(new BPicture());
		view->FillRect(wholeWindowRect);
		BPicture* picture = view->EndPicture();
		window->ClipWindowToPicture(picture, BPoint(0, 0), 0);
		delete picture;
	}	
#endif
}

// ---------------------------------------------------------------------
//! Updates the corresponding area of specified color in map.
// ---------------------------------------------------------------------
void BeCCSAppearance::UpdateMapArea(
	const Rect32& skinRect,			//!< rectangle which will be drawn.
	const DIBitmap* mapBitmap,		//!< color mapped bitmap.
	ColorValue color,				//!< the color in color map
	const DIBitmap* skinBitmap		//!< source skin bitmap.
)
{
	DIBColorLookup mapColorLookup(mapBitmap);
	DIBColorLookup skinColorLookup(skinBitmap);

	SInt32 posY;
	for (posY=skinRect.top; posY<=skinRect.bottom; posY++)
	{
		Byte* line = getBitsLineAddress(posY);
		line += 4 * skinRect.left;
		skinColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			if (mapColorLookup.Lookup(posX, posY) == color)
			{
				line[0] = skinColor.blueValue;
				line[1] = skinColor.greenValue;
				line[2] = skinColor.redValue;
				line[3] = 255;
			}
			line += 4;
		}
	}
	
	Point32 drawPoint = { skinRect.left, skinRect.top };
	invalidateRect(drawPoint, skinRect);
}

/**
 *	@brief	Updates the correspinding area of specified color in map,
 *			blending two skin bitmaps with specified blending ratio.
 */
void BeCCSAppearance::UpdateMapAreaWithBlend(
	const Rect32& skinRect,			///< rectangle which will be drawn
	const DIBitmap* mapBitmap,		///< color mapped bitmap.
	ColorValue color,				///< the color in color map
	const DIBitmap* skinBitmap1,	///< source skin bitmap #1
	const DIBitmap* skinBitmap2,	///< source skin bitmap #2
	UInt32 ratio					///< blending ratio (from 0 to ColorCodedSkin::BlendRatio_Max)
)
{
	DIBColorLookup mapColorLookup(mapBitmap);
	DIBColorLookup skinColorLookup1(skinBitmap1);
	DIBColorLookup skinColorLookup2(skinBitmap2);

	SInt32 posY;
	for (posY=skinRect.top; posY<=skinRect.bottom; posY++)
	{
		Byte* line = getBitsLineAddress(posY);
		line += 4 * skinRect.left;
		skinColorLookup1.InitLocation(skinRect.left, posY);
		skinColorLookup2.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor1 = skinColorLookup1.LookupNextColor();
			ColorValue skinColor2 = skinColorLookup2.LookupNextColor();
			if (mapColorLookup.Lookup(posX, posY) == color)
			{
				line[0] = static_cast<Byte>((skinColor1.blueValue * ratio + skinColor2.blueValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max);
				line[1] = static_cast<Byte>((skinColor1.greenValue * ratio + skinColor2.greenValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max);
				line[2] = static_cast<Byte>((skinColor1.redValue * ratio + skinColor2.redValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max);
				line[3] = 255;
			}
			line += 4;
		}
	}
	
	Point32 drawPoint = { skinRect.left, skinRect.top };
	invalidateRect(drawPoint, skinRect);
}

// ---------------------------------------------------------------------
//! Draws the corresponding part of specified rectangle.
// ---------------------------------------------------------------------
void BeCCSAppearance::UpdateRect(
	const Rect32& skinRect,			//!< rectangle to draw in.
	const DIBitmap* skinBitmap		//!< source skin bitmap.
)
{
	DIBColorLookup skinColorLookup(skinBitmap);
	
	SInt32 posY;
	for (posY = skinRect.top; posY <= skinRect.bottom; ++posY)
	{
		Byte* line = getBitsLineAddress(posY);
		line += 4 * skinRect.left;
		skinColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX = skinRect.left; posX <= skinRect.right; ++posX)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			line[0] = skinColor.blueValue;
			line[1] = skinColor.greenValue;
			line[2] = skinColor.redValue;
			line[3] = 255;
			line += 4;
		}
	}

	Point32 drawPoint = { skinRect.left, skinRect.top };
	invalidateRect(drawPoint, skinRect);
}

/**
 * @brief Overpaint a rectangle of specified bitmap to specified position.
 * @param[in] drawPoint upperleft position of the appearance
 * @param[in] sourceBitmap source bitmap image
 * @param[in] sourceRect rectangle of source bitmap
 * @param[in] transparentColor this color in source bitmap is not painted.
 *              specify NULL when not to use it.
 */
void BeCCSAppearance::OverpaintImage(
	const Point32& drawPoint,
	const DIBitmap* sourceBitmap,
	const Rect32& sourceRect,
	const ColorValue* transparentColor
)
{
	DIBColorLookup skinColorLookup(sourceBitmap);
	
	SInt32 posY;
	for (posY = sourceRect.top; posY <= sourceRect.bottom; ++posY)
	{
		Byte* line = getBitsLineAddress(posY - sourceRect.top + drawPoint.y);
		line += 4 * drawPoint.x;
		skinColorLookup.InitLocation(sourceRect.left, posY);
		SInt32 posX;
		for (posX = sourceRect.left; posX <= sourceRect.right; posX++)
		{
			ColorValue sourceColor = skinColorLookup.LookupNextColor();
			if (NULL == transparentColor || sourceColor != *transparentColor)
			{
				UInt32 alpha = static_cast<UInt32>(sourceColor.alphaValue);
				line[0] = static_cast<Byte>(((255 - alpha) * line[0] + alpha * sourceColor.blueValue) / 255);
				line[1] = static_cast<Byte>(((255 - alpha) * line[1] + alpha * sourceColor.greenValue) / 255);
				line[2] = static_cast<Byte>(((255 - alpha) * line[2] + alpha * sourceColor.redValue) / 255);
				line[3] = 255;
			}
			line += 4;
		}
	}

	invalidateRect(drawPoint, sourceRect);
}

// ---------------------------------------------------------------------
//! Invalidates the view at specified recatangle.
// ---------------------------------------------------------------------
void BeCCSAppearance::invalidateRect(
	const Point32& drawPoint,		//!< upper-left coordinates of target.
	const Rect32& skinRect			//!< source skin rectangle which will be drawn.
)
{
	BRect invRect( drawPoint.x,
		drawPoint.y,
		drawPoint.x + skinRect.right - skinRect.left,
		drawPoint.y + skinRect.bottom - skinRect.top);

	BWindow* window = view->Window();
	BAutolock locker(window);	
		
	view->Invalidate(invRect);
}

// ---------------------------------------------------------------------
//! Returns the address of the first pixel at specified line in the bitmap.
/*!
	@return address of the first pixel at specified line.
*/
// ---------------------------------------------------------------------
Byte* BeCCSAppearance::getBitsLineAddress(
	SInt32 line		//!< line to get address.
)
{
	return reinterpret_cast<Byte*>(bitmap->Bits()) + line * bitmap->BytesPerRow();
}
