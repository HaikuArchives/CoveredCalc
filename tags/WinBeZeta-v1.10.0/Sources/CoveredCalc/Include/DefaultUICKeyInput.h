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
	@file		DefaultUICKeyInput.h
	@brief		Definition of DefaultUICKeyInput class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.28 created
*/

#ifndef _DEFAULTUICKEYINPUT_H_
#define _DEFAULTUICKEYINPUT_H_

#include "UICKeyInput.h"

#if defined (WIN32)
#include "WinKeyInputAdapter.h"
#elif defined (BEOS)
#include "BeKeyInputAdapter.h"
#endif

/**
 *	@brief	Default implementation of UICButton interface.
 */
class DefaultUICKeyInput : public UICKeyInput
{
public:
										DefaultUICKeyInput() { }
	virtual								~DefaultUICKeyInput() { }
	
	virtual bool						IsEnabled() { return delegateObj.IsEnabled(); }
	virtual void						Enable(bool isEnabled) { delegateObj.Enable(isEnabled); }
	
	virtual void						MakeFocus() { delegateObj.MakeFocus(); }
	
	virtual void						SetKeyEventParameter(const KeyEventParameter& param)
													{ delegateObj.SetKeyEventParameter(param); }
	virtual const KeyEventParameter&	GetKeyEventParameter()
													{ return delegateObj.GetKeyEventParameter(); }

	virtual void						StopValueChangedNotification(bool doStop)
													{ return delegateObj.StopValueChangedNotification(doStop); }

#if defined (WIN32)
public:
	void								Init(WinKeyInputEdit* keyInputEdit) { delegateObj.Init(keyInputEdit); }
	WinKeyInputAdapter*					GetRawAdapter() { return &delegateObj; }
private:
	WinKeyInputAdapter					delegateObj;
#elif defined (BEOS)
public:
	void								Init(BeKeyInputEdit* view) { delegateObj.Init(view); }
	BeKeyInputAdapter*					GetRawAdapter() { return &delegateObj; }
private:
	BeKeyInputAdapter					delegateObj;
#endif
};

#endif // _DEFAULTUICKEYINPUT_H_
