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
	@file		UICSliderImpl.h
	@brief		Definition of UICSliderImpl class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.01.03 created
*/

#ifndef _UICSLIDERIMPL_H_
#define _UICSLIDERIMPL_H_

#include "UICSlider.h"

#if defined (WIN32)
#include "WinTrackbarAdapter.h"
#elif defined (BEOS)
#include "BeSliderAdapter.h"
#endif

/**
 *	@brief	List-box component interface.
 */
class UICSliderImpl : public UICSlider
{
public:
						UICSliderImpl() { }
	virtual				~UICSliderImpl() { }
	
	virtual bool		IsEnabled() { return delegateObj.IsEnabled(); }
	virtual void		Enable(bool isEnabled) { delegateObj.Enable(isEnabled); }
	
	virtual void		MakeFocus() { delegateObj.MakeFocus(); }
	
	virtual void		SetMaxValue(SInt32 value) { delegateObj.SetMaxValue(value); }
	virtual SInt32		GetMaxValue() { return delegateObj.GetMaxValue(); }
	virtual void		SetMinValue(SInt32 value) { delegateObj.SetMinValue(value); }
	virtual SInt32		GetMinValue() { return delegateObj.GetMinValue(); }
	virtual void		SetValue(SInt32 value) { delegateObj.SetValue(value); }
	virtual SInt32		GetValue() { return delegateObj.GetValue(); }
	virtual void		SetTickMarkCount(SInt32 count) { delegateObj.SetTickMarkCount(count); }

#if defined (WIN32)
public:
	void				Init(HWND hControl) { delegateObj.Init(hControl); }
	WinTrackbarAdapter*	GetRawAdapter() { return &delegateObj; }
private:
	WinTrackbarAdapter	delegateObj;
#elif defined (BEOS)
public:
	void				Init(BSlider* view) { delegateObj.Init(view); }
	BeSliderAdapter*	GetRawAdapter() { return &delegateObj; }
private:
	BeSliderAdapter		delegateObj;
#endif
};

#endif // _UICSLIDERIMPL_H_
