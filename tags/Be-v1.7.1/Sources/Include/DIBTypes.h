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
	@file		DIBTypes.h
	@brief		Definition of types for DIB.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.10.15 created
*/

#ifndef _DIBTYPES_H_
#define _DIBTYPES_H_

typedef struct tagRGBQUAD {
	UInt8	rgbBlue;
	UInt8	rgbGreen;
	UInt8	rgbRed;
	UInt8	rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFOHEADER{
	UInt32	biSize;
	SInt32	biWidth;
	SInt32	biHeight;
	UInt16	biPlanes;
	UInt16	biBitCount;
	UInt32	biCompression;
	UInt32	biSizeImage;
	SInt32	biXPelsPerMeter;
	SInt32	biYPelsPerMeter;
	UInt32	biClrUsed;
	UInt32	biClrImportant;
} BITMAPINFOHEADER;

/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER    bmiHeader;
	RGBQUAD             bmiColors[1];
} BITMAPINFO;

#pragma pack(2)
typedef struct tagBITMAPFILEHEADER {
	UInt16	bfType;
	UInt32	bfSize;
	UInt16	bfReserved1;
	UInt16	bfReserved2;
	UInt32	bfOffBits;
} BITMAPFILEHEADER;
#pragma pack()

#endif // _DIBTYPES_H_
