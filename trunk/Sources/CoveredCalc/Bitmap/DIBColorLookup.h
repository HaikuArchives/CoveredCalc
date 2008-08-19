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
	@file		DIBColorLookup.h
	@brief		Definition of DIBColorLookup class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.8 created
*/

#ifndef _DIBCOLORLOOKUP_H_
#define _DIBCOLORLOOKUP_H_

#include "ColorValue.h"

class DIBitmap;

// ---------------------------------------------------------------------
//! The helper class for looking-up color of the pixel in the DIB.
// ---------------------------------------------------------------------
class DIBColorLookup
{
public:
						DIBColorLookup(const DIBitmap* bitmap);
	virtual				~DIBColorLookup();
	
	void				InitLocation(SInt32 locX, SInt32 locY);
	void				InitLocation(SInt32 locX, const Byte* lineAddress);

	ColorValue			LookupNextColor();
	ColorValue			Lookup(SInt32 locX, SInt32 locY);

private:
	const DIBitmap*		bitmap;				///< target bitmap
	SInt32				bitCount;			///< bitmap's bit count
	const RGBQUAD*		colorTable;			///< pointer to color table
	UInt32				bitMask[4];			///< R-G-B-A color masks
	SInt32				bitMaskShift[4];	///< number of shifts to right, after AND operation to bitMask
	UInt32				bitMaskLevel[4];	
	const Byte*			nextAddress;		///< pointer to next pixel information
	SInt32				nextBitOffset;		///< next offset of the bits in a pixel information
	bool				isAlphaAvailable;	///< whether alpha channel of the bitmap is available
};

// ---------------------------------------------------------------------
//! The helper class for looking-up color of the pixel in the DIB.
/*!
	@return color value
	@note
	Note that this function is slow if you call many times.
	You should call InitLocation() once and then call LookupNextColor()
	several times to improve runtime speed.
*/
// ---------------------------------------------------------------------
inline ColorValue DIBColorLookup::Lookup(
	SInt32 locX,		//!< x-coordinate
	SInt32 locY			//!< y-coordinate
)
{
	InitLocation(locX, locY);
	return LookupNextColor();
}

#endif // _DIBCOLORLOOKUP_H_
