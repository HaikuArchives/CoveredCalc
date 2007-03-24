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
	@file		BMPFileLoader.cpp
	@brief		Implemantation of BMPFileLoader class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.27 created
*/

#include "Prefix.h"
#include "BMPFileLoader.h"
#include "DIBitmap.h"
#include "DIBFileLoaderException.h"
#include "MemoryException.h"
#include "File.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BMPFileLoader::BMPFileLoader()
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BMPFileLoader::~BMPFileLoader()
{
}

// ---------------------------------------------------------------------
//! Loads the bitmap data from specified file to bitmap object.
/*!
	@exception DIBFileLoaderException when this function fails.
*/
// ---------------------------------------------------------------------
void BMPFileLoader::Load(
	DIBitmap* bitmap,		//!< the bitmap object to which the bitmap data is loaded.
	const Path& fileName	//!< filename of .BMP file to load.
)
{
	try
	{
		File	dataFile;
	
		dataFile.Open(fileName, FileConstants::OpenMode_ReadOnly, FileConstants::OpenOption_None);
		
		// load file header and check it is BMP file.
		BITMAPFILEHEADER	fileHeader;
		if (sizeof(BITMAPFILEHEADER) > dataFile.Read(&fileHeader, sizeof(BITMAPFILEHEADER)))
		{
			throw new DIBFileLoaderExceptions::UnknownFileFormat(fileName);
		}
		if (0x4d42 != fileHeader.bfType)	// 0x4d42 ... 'BM' in little endian
		{
			throw new DIBFileLoaderExceptions::UnknownFileFormat(fileName);		
		}
		
		// load bitmap info header and color table
		// then check that it is Windows DIB file.
		// PM DIB is not acceppted.
		UInt32 bitsSize = fileHeader.bfSize - fileHeader.bfOffBits;
		UInt32 bitmapInfoSize = fileHeader.bfOffBits - sizeof( fileHeader );
			// allocate memory for header and color table
		DIBitmap::AutoPtr infoSaver = DIBitmap::AllocMemory(bitmapInfoSize);
		BITMAPINFO* info = static_cast<BITMAPINFO*>(infoSaver.Get());
		if (NULL == info)
		{
			MemoryException::Throw();		
		}
		if (bitmapInfoSize > dataFile.Read(info, bitmapInfoSize))
		{
			throw new DIBFileLoaderExceptions::UnknownFileFormat(fileName);		
		}
		if (sizeof(BITMAPINFOHEADER) > info->bmiHeader.biSize)
		{
			throw new DIBFileLoaderExceptions::UnknownFileFormat(fileName);		
		}
		if (BI_RLE8 == info->bmiHeader.biCompression ||
				BI_RLE4 == info->bmiHeader.biCompression)
		{
			// RLE compression is not supported.
			throw new DIBFileLoaderExceptions::UnknownFileFormat(fileName);	
		}
				
		// allocate memory for bit data.
		DIBitmap::AutoPtr bitsSaver = DIBitmap::AllocMemory(bitsSize);
		Byte* bits = static_cast<Byte*>(bitsSaver.Get());
		if (NULL == bits)
		{
			MemoryException::Throw();
		}
		
		// load bit data.
		if (bitsSize > dataFile.Read(bits, bitsSize))
		{
			throw new DIBFileLoaderExceptions::BrokenFile(fileName);
		}
		
		// store to DIBitmap object.
		infoSaver.Release();
		bitsSaver.Release();
		bitmap->Attach(info, bits);
		
		dataFile.Close();
	}
	catch (DIBFileLoaderException*)
	{
		throw;
	}
	catch (FileException* ex)
	{
		throw new DIBFileLoaderException(fileName, ex);
	}
}
