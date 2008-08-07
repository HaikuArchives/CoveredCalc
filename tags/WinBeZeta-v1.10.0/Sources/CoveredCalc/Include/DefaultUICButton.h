/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2008 CoveredCalc Project Contributors
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
	@file		DefaultUICButton.h
	@brief		Definition of DefaultUICButton class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.28 created
*/

#ifndef _DEFAULTUICBUTTON_H_
#define _DEFAULTUICBUTTON_H_

#include "UICButton.h"

#if defined (WIN32)
#include "WinTextControlAdapter.h"
#elif defined (BEOS)
#include "BeLabeledControlAdapter.h"
#endif

/**
 *	@brief	Default implementation of UICButton interface.
 */
class DefaultUICButton : public UICButton
{
public:
								DefaultUICButton() { }
	virtual						~DefaultUICButton() { }
	
	virtual bool				IsEnabled() { return delegateObj.IsEnabled(); }
	virtual void				Enable(bool isEnabled) { delegateObj.Enable(isEnabled); }
	
	virtual void				MakeFocus() { delegateObj.MakeFocus(); }
	
	virtual void				SetText(ConstAStr text) { delegateObj.SetText(text); }
	virtual void				GetText(MBCString& text) { delegateObj.GetText(text); }

#if defined (WIN32)
public:
	void						Init(HWND hControl) { delegateObj.Init(hControl); }
	WinTextControlAdapter*		GetRawAdapter() { return &delegateObj; }
private:
	WinTextControlAdapter		delegateObj;
#elif defined (BEOS)
public:
	void						Init(BControl* view) { delegateObj.Init(view); }
	BeLabeledControlAdapter*	GetRawAdapter() { return &delegateObj; }
private:
	BeLabeledControlAdapter		delegateObj;
#endif
};

#endif // _DEFAULTUICBUTTON_H_
