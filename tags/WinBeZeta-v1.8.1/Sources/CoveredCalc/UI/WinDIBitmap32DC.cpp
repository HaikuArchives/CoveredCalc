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
	@file		WinDIBitmap32DC.cpp
	@brief		Implementation of WinDIBitmap32DC class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.4.12 created
*/

#include "Prefix.h"
#include "WinDIBitmap32DC.h"
#include "MemoryException.h"

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinDIBitmap32DC::WinDIBitmap32DC()
{
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinDIBitmap32DC::~WinDIBitmap32DC()
{
}

/**
 *	@brief	BITMAPINFO を生成します。必ず malloc() でメモリ確保を行います。
 *	@return	生成した BITMAPINFO。
 */
BITMAPINFO* WinDIBitmap32DC::createBitmapInfo(
	SInt32 width,		//!< ビットマップの幅
	SInt32 height		//!< ビットマップの高さ
)
{
	BITMAPINFO* info = static_cast<BITMAPINFO*>(malloc(sizeof(BITMAPV4HEADER) + 3 * sizeof(DWORD)));
	if (NULL == info)
	{
		MemoryException::Throw();
	}

	::ZeroMemory(info, sizeof(BITMAPV4HEADER) + 3 * sizeof(DWORD));
	BITMAPV4HEADER* v4Header = reinterpret_cast<BITMAPV4HEADER*>(&(info->bmiHeader));
	v4Header->bV4Size = sizeof(BITMAPV4HEADER);
	v4Header->bV4Width = width;
	v4Header->bV4Height = height;
	v4Header->bV4Planes = 1;
	v4Header->bV4BitCount = 32;
	v4Header->bV4V4Compression = BI_BITFIELDS;
	v4Header->bV4SizeImage = 0;
	v4Header->bV4XPelsPerMeter = 0;
	v4Header->bV4YPelsPerMeter = 0;
	v4Header->bV4ClrUsed = 0;
	v4Header->bV4ClrImportant = 0;
	v4Header->bV4RedMask = 0x00FF0000;
	v4Header->bV4GreenMask = 0x0000FF00;
	v4Header->bV4BlueMask = 0x000000FF;
	v4Header->bV4AlphaMask = 0xFF000000;

	DWORD* colorMasks = reinterpret_cast<DWORD*>(reinterpret_cast<BYTE*>(v4Header) + v4Header->bV4Size);
	colorMasks[0] = v4Header->bV4RedMask;
	colorMasks[1] = v4Header->bV4GreenMask;
	colorMasks[2] = v4Header->bV4BlueMask;

	return info;
}
