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
	@file		WinDIBitmapDC.h
	@brief		Definition of WinDIBitmapDC class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.10.12 created
*/

#ifndef _WINDIBITMAPDC_H_
#define _WINDIBITMAPDC_H_

#include "WinBitmapDC.h"
#include "DIBitmap.h"

// ---------------------------------------------------------------------
//! DIBとしてもアクセスできる WinBitmapDC クラス
// ---------------------------------------------------------------------
class WinDIBitmapDC : public WinBitmapDC
{
public:
						WinDIBitmapDC();
	virtual				~WinDIBitmapDC();
	
	virtual	void		Create(HDC dc, SInt32 width, SInt32 height);
	virtual	void		Destroy();

	const DIBitmap*		GetDIB() const	{ return &dib; }
	DIBitmap*			GetDIB()		{ return &dib; }

protected:
	virtual BITMAPINFO*	createBitmapInfo(SInt32 width, SInt32 height);

protected:
	DIBitmap			dib;
};

#endif // _WINDIBITMAPDC_H_