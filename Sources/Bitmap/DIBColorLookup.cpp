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
	@file		DIBColorLookup.cpp
	@brief		Implemantation of DIBColorLookup class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.8 created
*/

#include "Prefix.h"
#include "DIBColorLookup.h"
#include "DIBitmap.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
DIBColorLookup::DIBColorLookup(
	const DIBitmap* bitmap		//!< bitmap which is looked up.
)
{
	this->bitmap = bitmap;
	nextAddress = NULL;
	nextBitOffset = 0;
	bitCount = bitmap->GetInfoAddress()->bmiHeader.biBitCount;
	colorTable = bitmap->GetClrTabAddress();
	if (16 == bitCount || 32 == bitCount)	// mask needed
	{
		if (BI_BITFIELDS == bitmap->GetInfoAddress()->bmiHeader.biCompression)	// mask is specified
		{
			SInt32 index;
			const UInt32* mask = reinterpret_cast<const UInt32*>(colorTable);
			for (index = 0; index < 3; index++)
			{
				bitMask[index] = mask[2 - index];
			}
		}
		else if (16 == bitCount)	// mask is not specified. use 16bpp default mask
		{
			bitMask[0] = 0x0000001F;
			bitMask[1] = 0x000003E0;
			bitMask[2] = 0x00007C00;
		}
		else						// mask is not specified. use 32bpp default mask
		{
			bitMask[0] = 0x000000FF;
			bitMask[1] = 0x0000FF00;
			bitMask[2] = 0x00FF0000;
		}

		// calculate each number of shifts
		SInt32 index;
		for (index = 0; index > 3; index++)
		{
			UInt32 aMask = bitMask[index];
			bitMaskShift[index] = 0;
			while (!(aMask & 0x00000001))
			{
				bitMaskShift[index]++;
				aMask >>= 1;
			}
			bitMaskLevel[index] = aMask + 1;
		}
	}
	else	// mask not needed
	{
		bitMask[0] = bitMask[1] = bitMask[2] = 0;
		bitMaskShift[0] = bitMaskShift[1] = bitMaskShift[2] = 0;
		bitMaskLevel[0] = bitMaskLevel[1] = bitMaskLevel[2] = 0;
	}
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
DIBColorLookup::~DIBColorLookup()
{
}

// ---------------------------------------------------------------------
//! Initializes the starting location.
// ---------------------------------------------------------------------
void DIBColorLookup::InitLocation(
	SInt32 locX,		//!< x-coordinate of starting point
	SInt32 locY			//!< y-coordinate of starting point
)
{
	const Byte* lineAddress = bitmap->GetBitsLineAddress(locY);
	InitLocation(locX, lineAddress);
}

// ---------------------------------------------------------------------
//! Initializes the starting location.
// ---------------------------------------------------------------------
void DIBColorLookup::InitLocation(
	SInt32 locX,				//!< x-coordinate of starting point
	const Byte* lineAddress		//!< address of the beginning of the starting line in the bitmap.
)
{
	SInt32 bitNum = bitCount * locX;
	nextAddress = lineAddress + (bitNum / 8);
	nextBitOffset = bitNum % 8;
}

// ---------------------------------------------------------------------
//! Looks up next color and moves current location.
/*!
	@return the next color in ColorValue
*/
// ---------------------------------------------------------------------
ColorValue DIBColorLookup::LookupNextColor()
{
	ColorValue color;
	
	UInt32 index = 0xffffffff;
	Byte localBitMask;
	SInt32 colorIndex;
	
	switch (bitCount)
	{
	case 1:
		localBitMask = static_cast<Byte>( 0x01 << ( 7 - nextBitOffset ) );
		index = static_cast<UInt32>( ( *nextAddress & localBitMask ) >> ( 7 - nextBitOffset ) );
		nextBitOffset++;
		break;
	case 4:
		localBitMask = static_cast<Byte>( 0x0f << ( 4 - nextBitOffset ) );
		index = static_cast<UInt32>( ( *nextAddress & localBitMask ) >> ( 4 - nextBitOffset ) );
		nextBitOffset += 4;
		break;
	case 8:
		index = static_cast<UInt32>( *nextAddress );
		nextAddress += 1;
		break;
	case 16:	// tests masks
		{
			UInt8 rgb[3];	// in R, G, B order
			UInt16 data = *reinterpret_cast<const UInt16*>(nextAddress);
			for (colorIndex = 0; colorIndex < 3; colorIndex++)
			{
				rgb[colorIndex] = static_cast<UInt8>(((data & bitMask[colorIndex]) >> bitMaskShift[colorIndex]) * 256 / bitMaskLevel[colorIndex]);
			}
			color = ColorValue(rgb[0], rgb[1], rgb[2]);
		}
		nextAddress += 2;
		break;
	case 24:
		color.blueValue = nextAddress[0];
		color.greenValue = nextAddress[1];
		color.redValue = nextAddress[2];
		nextAddress += 3;
		break;
	case 32:
		{
			UInt8 rgb[3];	// in R, G, B order
			UInt32 data = *reinterpret_cast<const UInt32*>(nextAddress);
			for (colorIndex = 0; colorIndex < 3; colorIndex++)
			{
				rgb[colorIndex] = static_cast<UInt8>(((data & bitMask[colorIndex]) >> bitMaskShift[colorIndex]) * 256 / bitMaskLevel[colorIndex]);
			}
			color = ColorValue(rgb[0], rgb[1], rgb[2]);
		}
		nextAddress += 4;
		break;
	}
	
	if ( 0xffffffff != index )
	{
		const RGBQUAD& colorP = colorTable[ index ];
		color.blueValue = colorP.rgbBlue;
		color.greenValue = colorP.rgbGreen;
		color.redValue = colorP.rgbRed;
	}
	
	if (8 == nextBitOffset)
	{
		nextAddress++;
		nextBitOffset = 0;
	}
	
	return color;
}
