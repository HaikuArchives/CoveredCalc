/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2009 CoveredCalc Project Contributors
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
	@file		UICSlider.h
	@brief		Definition of UICSlider interface.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.01.03 created
*/

#ifndef _UICSLIDER_H_
#define _UICSLIDER_H_

/**
 *	@brief	Slider component interface.
 */
class UICSlider
{
public:
						UICSlider() { }
	virtual				~UICSlider() { }
	
	virtual bool		IsEnabled() = 0;
	virtual void		Enable(bool isEnabled) = 0;
	
	virtual void		MakeFocus() = 0;
	
	virtual void		SetMaxValue(SInt32 value) = 0;
	virtual SInt32		GetMaxValue() = 0;
	virtual void		SetMinValue(SInt32 value) = 0;
	virtual SInt32		GetMinValue() = 0;
	virtual void		SetValue(SInt32 value) = 0;
	virtual SInt32		GetValue() = 0;
	virtual void		SetTickMarkCount(SInt32 count) = 0;
};

#endif // _UICSLIDER_H_
