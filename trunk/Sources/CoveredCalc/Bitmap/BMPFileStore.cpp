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
	@file		BMPFileStore.cpp
	@brief		Implemantation of BMPFileStore class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.10.26 created
*/

#include "Prefix.h"
#include "BMPFileStore.h"
#include "DIBitmap.h"
#include "BMPFileLoader.h"

/**
 *	@brief	Constructor.
 */
BMPFileStore::BMPFileStore()
{
}

/**
 *	@brief	Destructor.
 */
BMPFileStore::~BMPFileStore()
{
	BMPFileStore::Clear();
}

/**
 *	@brief	Loads specified bitmap (BMP) file.
 *	@note	If specified BMP file is already loaded, 
 *			this function returns DIBitmap pointer which loaded before.
 *			So, same BMP file is loaded only once.
 */
DIBitmap* BMPFileStore::Load(const Path& fileName)
{
	PathDIBMap::const_iterator ite = bitmaps.find(fileName);
	if (ite != bitmaps.end())	// found ... already loaded
	{
		return (*ite).second;
	}
	
	DIBitmap* bitmap = new DIBitmap();
	try
	{
		BMPFileLoader loader;	
		loader.Load(bitmap, fileName);
		bitmaps[fileName] = bitmap;
	}
	catch (...)
	{
		if (NULL != bitmap)
		{
			delete bitmap;
		}
		throw;
	}
	return bitmap;
}

/**
 *	@brief	Deletes all loaded bitmaps.
 */
void BMPFileStore::Clear()
{
	PathDIBMap::iterator ite;
	for (ite = bitmaps.begin(); ite != bitmaps.end(); ite++)
	{
		DIBitmap* aBitmap = (*ite).second;
		delete aBitmap;
	}
}
