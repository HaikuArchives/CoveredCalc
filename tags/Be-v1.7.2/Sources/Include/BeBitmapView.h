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
	@file		BeBitmapView.h
	@brief		Definition of BeBitmapView class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.12.19 created
*/

#ifndef _BEBITMAPVIEW_H_
#define _BEBITMAPVIEW_H_

#include <interface/View.h>

class BBitmap;

// ---------------------------------------------------------------------
//! The view which draws a specified bitmap.
// ---------------------------------------------------------------------
class BeBitmapView : public BView
{
public:
								BeBitmapView(BRect frame, const char* name, uint32 resizingMode, uint32 flags);
	virtual						~BeBitmapView();

	void						SetBitmap(BBitmap* bitmap);
	BBitmap*					Bitmap() const;
	
protected:
	virtual void				Draw(BRect updateRect);
	
private:
	BBitmap*					bitmap;			//!< bitmap to draw on the view

};

#endif // _BEBITMAPVIEW_H_
