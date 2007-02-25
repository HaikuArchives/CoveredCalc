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
	@file		DIBitmap.h
	@brief		Definition of DIBitmap class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.1 created
*/

#ifndef _DIBITMAP_H_
#define _DIBITMAP_H_ 

#include <malloc.h>

// ---------------------------------------------------------------------
//! This class represents Windows DIB (Device Independent Bitmap).
// ---------------------------------------------------------------------
class DIBitmap
{
public:
						DIBitmap();
	virtual				~DIBitmap();

	void				Create(SInt32 sizeX, SInt32 sizeY);
	void				Destroy();
	bool				IsReady() const { return (bitmapInfo != NULL) ? true : false; }

	void				Attach(BITMAPINFO* bitmapInfo, Byte* bits);
	void				Detach();

	BITMAPINFO*			GetInfoAddress() { return bitmapInfo; }
	Byte*				GetBitsAddress() { return bits; }
	Byte*				GetBitsLineAddress(SInt32 line) { return bits + ( GetHeight() - line - 1 ) * getStorageWidth(); }
	RGBQUAD*			GetClrTabAddress() { return (RGBQUAD*)( ((Byte*)bitmapInfo) + sizeof( BITMAPINFOHEADER ) ); }

	const BITMAPINFO*	GetInfoAddress() const { return bitmapInfo; }
	const Byte*			GetBitsAddress() const { return bits; }
	const Byte*			GetBitsLineAddress(SInt32 line) const { return bits + ( GetHeight() - line - 1 ) * getStorageWidth(); }
	const RGBQUAD*		GetClrTabAddress() const { return (RGBQUAD*)( ((Byte*)bitmapInfo) + sizeof( BITMAPINFOHEADER ) ); }

	SInt32				GetWidth() const { return getRealWidth(); }
	SInt32				GetHeight() const { SInt32 ret = getRealHeight(); return (ret < 0 ) ? ( -ret ) : ( ret ); }
	SInt32				GetBitsSize() const { return getStorageWidth() * GetHeight(); }

	static	void*		AllocMemory(size_t size) { return malloc(size); }
	static	void		FreeMemory(void* memory) { free(memory); }

	// ---------------------------------------------------------------------
	//! Smart pointer utility for DIBitmap::FreeMemory().
	// ---------------------------------------------------------------------
	class AutoPtr
	{
	public:
							AutoPtr() { ptr = NULL; }
							AutoPtr(void* ptr) { this->ptr = ptr; }
		virtual				~AutoPtr() { Reset(); }
		
		void*				Get() const { return ptr; }
		void*				Release() { void* ret = ptr; ptr = NULL; return ret; }	
		void				Reset() { if (NULL != ptr) { DIBitmap::FreeMemory(ptr); ptr = NULL; } }

		void*				operator=(void* ptr) { Reset(); this->ptr = ptr; return ptr; }
		
	private:
		void*				ptr;
	};

private:
	SInt32				getStorageWidth() const;
	SInt32				getRealWidth() const { return bitmapInfo->bmiHeader.biWidth; }
	SInt32				getRealHeight() const { return bitmapInfo->bmiHeader.biHeight; }

private:
	BITMAPINFO*			bitmapInfo;
	Byte*				bits;
};

// ---------------------------------------------------------------------
//! Returns the count of bytes per one line in the bits array
/*!
	@return Count of bytes per one line
*/
// ---------------------------------------------------------------------
inline SInt32 DIBitmap::getStorageWidth() const
{
	int noPaddingWidth = ( bitmapInfo->bmiHeader.biWidth * bitmapInfo->bmiHeader.biBitCount + 7 ) / 8;
	return ( noPaddingWidth + 3 ) & ~3;
}

#endif // _DIBITMAP_H_
