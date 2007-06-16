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
	@file		DIBitmap.cpp
	@brief		Implemantation of DIBitmap class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.1 created
*/

#include "Prefix.h"
#include <string.h>
#include "DIBitmap.h"
#include "MemoryException.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
DIBitmap::DIBitmap()
{
	bitmapInfo = NULL;
	bits = NULL;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
DIBitmap::~DIBitmap()
{
	Destroy();
}

// ---------------------------------------------------------------------
//! Creates a new 24bpp bitmap.
/*!
	@note All pixels in the created bitmap is initialized with black color.
*/
// ---------------------------------------------------------------------
void DIBitmap::Create(
	SInt32 sizeX,		//!< horizontal size
	SInt32 sizeY		//!< vertical size
)
{
	if (NULL != bitmapInfo)
	{
		FreeMemory(bitmapInfo);
		bitmapInfo = NULL;
	}
	if (NULL != bits)
	{
		FreeMemory(bits);
		bits = NULL;
	}

	// allocates buffers for the header
	bitmapInfo = reinterpret_cast<BITMAPINFO*>(AllocMemory(sizeof(BITMAPINFO)));
	if (NULL == bitmapInfo)
	{
		MemoryException::Throw();
	}

	// sets header information
	BITMAPINFOHEADER* biP = &(bitmapInfo->bmiHeader);

	biP->biSize = sizeof(BITMAPINFOHEADER);
	biP->biWidth = sizeX;
	biP->biHeight = sizeY;
	biP->biPlanes = 1;
	biP->biBitCount = 24;
	biP->biCompression = BI_RGB;
	biP->biSizeImage = 0;
	biP->biXPelsPerMeter = 0;
	biP->biYPelsPerMeter = 0;
	biP->biClrUsed = 0;
	biP->biClrImportant = 0;

	// allocates buffers for each pixel bits
	SInt32 bitsSize = sizeY * getStorageWidth();
	bits = reinterpret_cast<Byte*>(AllocMemory(bitsSize));
	if (NULL == bits)
	{
		MemoryException::Throw();
	}

	// initializes all pixels with black color
	memset( bits, 0, bitsSize );
}

// ---------------------------------------------------------------------
//! Frees the bitmap information.
// ---------------------------------------------------------------------
void DIBitmap::Destroy()
{
	if (NULL != bitmapInfo)
	{
		FreeMemory(bitmapInfo);
		bitmapInfo = NULL;
	}
	if (NULL != bits)
	{
		FreeMemory(bits);
		bits = NULL;
	}
}

// ---------------------------------------------------------------------
//! Attaches an existing BITMAPINFO and pixel bits to this object.
/*!
	@note
	The attached buffer will be freed by calling FreeMemory()
	member function. So the buffer should be allocated by
	AllocMemory() member function.
	You can call Detach() member function to free the buffer by yourself.
*/
// ---------------------------------------------------------------------
void DIBitmap::Attach(
	BITMAPINFO* bitmapInfo,		//!< BITMAPINFO structure
	Byte* bits					//!< pixel bits
)
{
	Destroy();

	this->bitmapInfo = bitmapInfo;
	this->bits = bits;
}

// ---------------------------------------------------------------------
//! Detaches the BITMAPINFO and pixel bits from this object.
/*!
	@note
	You can get the pointer to BITMAPINFO and pixel bits
	by calling GetInfoAddress() and GetBitsAddress() member
	function before calling this function.
	See also remarks of Attach() member function.
*/
// ---------------------------------------------------------------------
void DIBitmap::Detach()
{
	bitmapInfo = NULL;
	bits = NULL;
}
