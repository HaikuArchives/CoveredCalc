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
	@file		WinBitmapDC.cpp
	@brief		Implementation of WinBitmapDC class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.10.11 created
*/

#include "Prefix.h"
#include "WinBitmapDC.h"
#include "WinBitmapDCException.h"

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
WinBitmapDC::WinBitmapDC()
{
	bitmap = NULL;
	oldBitmap = NULL;
	memDC = NULL;
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinBitmapDC::~WinBitmapDC()
{
	WinBitmapDC::Destroy();	// 派生先のを呼ばないように
}

// ---------------------------------------------------------------------
//! ビットマップDCを構築します。
/*!
	@throw WinBitmapDCException 関数が失敗したとき
*/
// ---------------------------------------------------------------------
void WinBitmapDC::Create(
	HDC dc,			//!< 作成したいビットマップDCと互換性のあるDC
	SInt32 width,	//!< ビットマップの幅
	SInt32 height	//!< ビットマップの高さ
)
{
	if (IsCreated())
	{
		throw new WinBitmapDCExceptions::AlreadyCreated();
	}
	
	HDC memDC;
	HBITMAP bitmap;
	
	// 指定されたDCと互換性のあるメモリDCを作成
	memDC = ::CreateCompatibleDC(dc);
	if (NULL == memDC)
	{
		throw new WinBitmapDCException();
	}
	
	try
	{
		// 指定されたDCと互換性のあるビットマップを作成
		bitmap = ::CreateCompatibleBitmap(dc, width, height);
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
		::DeleteDC(memDC);
		memDC = NULL;
		throw;
	}

	this->memDC = memDC;
	this->bitmap = bitmap;
}

// ---------------------------------------------------------------------
//! ビットマップDCを破棄します。
// ---------------------------------------------------------------------
void WinBitmapDC::Destroy()
{
	if (NULL != memDC)
	{
		if (NULL != oldBitmap)
		{
			::SelectObject(memDC, oldBitmap);
			oldBitmap = NULL;
		}
		::DeleteDC(memDC);
		memDC = NULL;
	}
	
	if (NULL != bitmap)
	{
		::DeleteObject(bitmap);
		bitmap = NULL;
	}
}

// ---------------------------------------------------------------------
//! HDCへのキャスト演算子
/*!
	@return デバイスコンテキストハンドル
	@note
	これによって取得したDCに別のビットマップを
	SelectObjectしたりしてはいけません。
*/
// ---------------------------------------------------------------------
WinBitmapDC::operator HDC() const
{
	return memDC;
}
