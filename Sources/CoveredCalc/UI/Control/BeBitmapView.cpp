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
	@file		BeBitmapView.cpp
	@brief		Implementation of BeBitmapView class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.12.19 created
*/

#include "Prefix.h"
#include <InterfaceKit.h>
#include "BeBitmapView.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeBitmapView::BeBitmapView(
	BRect frame,			//!< rectangle
	const char* name,		//!< name
	uint32 resizingMode,	//!< resizing mode
	uint32 flags			//!< flags
)
	: BView(frame, name, resizingMode, flags)
{
	bitmap = NULL;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeBitmapView::~BeBitmapView()
{
	if (NULL != bitmap)
		delete bitmap;
}

// ---------------------------------------------------------------------
//! Set a bitmap which is drawn on this view.
/*!
	@note	This class have responsibility to delete the passed bitmap object.
			Caller must not delete it.
*/
// ---------------------------------------------------------------------
void BeBitmapView::SetBitmap(
	BBitmap* bitmap		//!< bitmap object
)
{
	if (NULL != this->bitmap)
		delete this->bitmap;
	
	this->bitmap = bitmap;
	Invalidate();	
}

// ---------------------------------------------------------------------
//! Returns bitmap.
/*!
	@note	Caller must not delete returned bitmap object.
*/
// ---------------------------------------------------------------------
BBitmap* BeBitmapView::Bitmap() const
{
	return bitmap;
}

// ---------------------------------------------------------------------
//! Drawing function.
// ---------------------------------------------------------------------
void BeBitmapView::Draw(
	BRect /*updateRect*/		//!< update rectangle.
)
{
	if (NULL != bitmap)
	{
		DrawBitmap(bitmap, BPoint(0, 0));
	}
}
