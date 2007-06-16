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
	@file		WinDIBitmapDC.cpp
	@brief		Implementation of WinDIBitmapDC class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.10.12 created
*/

#include "Prefix.h"
#include "WinDIBitmapDC.h"
#include "WinBitmapDCException.h"
#include "MemoryException.h"

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinDIBitmapDC::WinDIBitmapDC()
{
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinDIBitmapDC::~WinDIBitmapDC()
{
	WinDIBitmapDC::Destroy();		// 派生先のを呼ばないように
}

// ---------------------------------------------------------------------
//! DIBとしてアクセスできるビットマップDCを構築します。
/*!
	@throw WinBitmapDCException 失敗したとき
*/
// ---------------------------------------------------------------------
void WinDIBitmapDC::Create(
	HDC dc,				//!< 作成したいビットマップDCと互換性のあるDC
	SInt32 width,		//!< ビットマップの幅
	SInt32 height		//!< ビットマップの高さ
)
{
	if (IsCreated())
	{
		throw new WinBitmapDCExceptions::AlreadyCreated();
	}
	
	HDC memDC;
	HBITMAP bitmap;
	BITMAPINFO* info = NULL;
	Byte* bits = NULL;
	
	// 指定されたDCと互換性のあるメモリDCを作成
	memDC = ::CreateCompatibleDC(dc);
	if (NULL == memDC)
	{
		throw new WinBitmapDCException();
	}

	try
	{
		// 指定されたDCと互換のあるDIBを作成
		info = createBitmapInfo(width, height);
		try
		{
			bitmap = ::CreateDIBSection(dc, info, DIB_RGB_COLORS, reinterpret_cast<LPVOID*>(&bits), NULL, 0);
			if (NULL == bitmap)
			{
				throw new WinBitmapDCException();
			}
			
			try
			{
				// メモリDC にビットマップを結びつけておく
				this->oldBitmap = static_cast<HBITMAP>(::SelectObject(memDC, bitmap));
				if (NULL == this->oldBitmap)
				{
					throw new WinBitmapDCException();
				}
			}
			catch (...)
			{
				::DeleteObject(bitmap);
				bitmap = NULL;
				throw;			
			}
		}
		catch (...)
		{
			free(info);
			info = NULL;
			throw;
		}
	}
	catch (...)
	{
		::DeleteDC(memDC);
		memDC = NULL;
		throw;		
	}

	// DIB部分をAttach
	dib.Attach(info, bits);
	info = NULL;
	bits = NULL;

	this->memDC = memDC;
	this->bitmap = bitmap;	
}

/**
 *	@brief	BITMAPINFO を生成します。必ず malloc() でメモリ確保を行います。
 *	@return	生成した BITMAPINFO。
 */
BITMAPINFO* WinDIBitmapDC::createBitmapInfo(
	SInt32 width,		//!< ビットマップの幅
	SInt32 height		//!< ビットマップの高さ
)
{
	BITMAPINFO*	info = static_cast<BITMAPINFO*>(malloc(sizeof(BITMAPINFO)));
	if (NULL == info)
	{
		MemoryException::Throw();
	}

	::ZeroMemory(info, sizeof(BITMAPINFO));
	info->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	info->bmiHeader.biWidth = width;
	info->bmiHeader.biHeight = height;
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biBitCount = 24;	// 24bpp固定
	info->bmiHeader.biCompression = BI_RGB;
	info->bmiHeader.biSizeImage = 0;
	info->bmiHeader.biXPelsPerMeter = 0;
	info->bmiHeader.biYPelsPerMeter = 0;
	info->bmiHeader.biClrUsed = 0;
	info->bmiHeader.biClrImportant = 0;		

	return info;
}

// ---------------------------------------------------------------------
//! DIBとしてアクセスできるビットマップDCを破棄します。
// ---------------------------------------------------------------------
void WinDIBitmapDC::Destroy()
{
	BITMAPINFO* info = dib.GetInfoAddress();
	dib.Detach();
	
	if (NULL != info)
	{
		free(info);
	}
	
	WinBitmapDC::Destroy();
}
