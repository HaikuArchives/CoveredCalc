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
	@file		BMPFileSaver.cpp
	@brief		Implemantation of BMPFileSaver class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.5.5 created
*/

#include "Prefix.h"
#include "BMPFileSaver.h"
#include "DIBitmap.h"
#include "DIBFileSaverException.h"
#include "File.h"

/**
 *	@brief	Constructor
 */
BMPFileSaver::BMPFileSaver()
{
}

/**
 *	@brief	Destructor
 */
BMPFileSaver::~BMPFileSaver()
{
}

/**
 *	@brief	Saves the bitmap data to specified file.
 *	@param[in]	bitmap		bitmap data.
 *	@param[in]	fileName	a file that the data will be saved to.
 *	@exception DIBFileSaverException when this function fails.
 */
void BMPFileSaver::Save(DIBitmap* bitmap, const Path& fileName)
{
	try
	{
		const BITMAPINFO* info = bitmap->GetInfoAddress();
		const Byte* bits = bitmap->GetBitsAddress();
	
		// check DIB type.
		if (BI_RLE8 == info->bmiHeader.biCompression ||
				BI_RLE4 == info->bmiHeader.biCompression ||
				BI_JPEG == info->bmiHeader.biCompression ||
				BI_PNG == info->bmiHeader.biCompression)
		{
			throw new DIBFileSaverExceptions::UnsupportedDIB(fileName);
		}
	
		// calculate color palette size
		SInt32 paletteSize;
		if ((16 == info->bmiHeader.biBitCount || 32 == info->bmiHeader.biBitCount) &&
				BI_BITFIELDS == info->bmiHeader.biCompression)
		{
			paletteSize = 3;
		}
		else
		{
			paletteSize = info->bmiHeader.biClrUsed;
			if (0 == paletteSize)
			{
				switch (info->bmiHeader.biBitCount)
				{
				case 1:
					paletteSize = 2;
					break;
				case 4:
					paletteSize = 16;
					break;
				case 8:
					paletteSize = 256;
					break;
				}
			}
		}
		paletteSize *= 4;
		
		// calculate bits size.
		SInt32 bitsSize = bitmap->GetBitsSize();

		// save		
		File	dataFile;
		dataFile.Open(fileName, FileConstants::OpenMode_WriteOnly, FileConstants::OpenOption_Create | FileConstants::OpenOption_Erase);
		
		// --- file header
		BITMAPFILEHEADER fileHeader;
		fileHeader.bfType = 0x4d42;		// 0x4d42 ... 'BM' in little endian
		fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + info->bmiHeader.biSize + paletteSize + bitsSize;
		fileHeader.bfReserved1 = 0;
		fileHeader.bfReserved2 = 0;
		fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + info->bmiHeader.biSize + paletteSize;
		dataFile.Write(&fileHeader, sizeof(BITMAPFILEHEADER));
		
		// --- info header + color palette
		dataFile.Write(info, info->bmiHeader.biSize + paletteSize);
		
		// --- image bits
		dataFile.Write(bits, bitsSize);
		
		dataFile.Close();
	}
	catch (DIBFileSaverException*)
	{
		throw;
	}
	catch (FileException* ex)
	{
		throw new DIBFileSaverException(fileName, ex);
	}
}
