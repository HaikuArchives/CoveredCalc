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
	@file		BMPFileStore.h
	@brief		Definition of BMPFileStore class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.10.26 created
*/

#ifndef _BMPFILESTORE_H_
#define _BMPFILESTORE_H_

#include <map>
#include "DIBitmapStore.h"
#include "Path.h"

class DIBitmap;

/**
 *	@brief	This class loads BMP file and holds it.
 *	
 *	If specified BMP file is already loaded, this class don't load it again
 *	and returns the DIBitmap pointer loaded before.
 */
class BMPFileStore : public DIBitmapStore
{
public:
						BMPFileStore();
	virtual				~BMPFileStore();
	
	DIBitmap*			Load(const Path& fileName);
	virtual void		Clear();

private:
	typedef std::map<const Path, DIBitmap*, PathComparator>	PathDIBMap;
	PathDIBMap			bitmaps;
};

#endif // _BMPFILESTORE_H_
