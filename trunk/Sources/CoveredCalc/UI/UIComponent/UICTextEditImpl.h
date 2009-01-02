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
	@file		UICTextEditImpl.h
	@brief		Definition of UICTextEditImpl class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.28 created
*/

#ifndef _UICTEXTEDITIMPL_H_
#define _UICTEXTEDITIMPL_H_

#include "UICTextEdit.h"

#if defined (WIN32)
#include "WinTextControlAdapter.h"
#elif defined (BEOS)
#include "BeTextControlAdapter.h"
#include "BeTextViewAdapter.h"
#endif

/**
 *	@brief	 implementation of UICTextEditImpl interface.
 */
#if defined (BEOS)
template <class DELEGATE_T, class INIT_PARAM_T>
#endif
class UICTextEditImpl : public UICTextEdit
{
public:
								UICTextEditImpl() { }
	virtual						~UICTextEditImpl() { }
	
	virtual bool				IsEnabled() { return delegateObj.IsEnabled(); }
	virtual void				Enable(bool isEnabled) { delegateObj.Enable(isEnabled); }

	virtual bool				IsEditable() { return delegateObj.IsEditable(); }
	virtual void				MakeEditable(bool isEditable) { delegateObj.MakeEditable(isEditable); }
	
	virtual void				MakeFocus() { delegateObj.MakeFocus(); }
	
	virtual void				SetText(ConstAStr text) { delegateObj.SetText(text); }
	virtual void				GetText(MBCString& text) { delegateObj.GetText(text); }

	virtual void				StopModificationNotification(bool doStop) { delegateObj.StopModificationNotification(doStop); }

#if defined (WIN32)
public:
	void						Init(HWND hControl) { delegateObj.Init(hControl); }
	WinTextControlAdapter*		GetRawAdapter() { return &delegateObj; }
private:
	WinTextControlAdapter		delegateObj;
#elif defined (BEOS)
public:
	void						Init(INIT_PARAM_T view) { delegateObj.Init(view); }
	DELEGATE_T*					GetRawAdapter() { return &delegateObj; }
private:
	DELEGATE_T					delegateObj;
#endif
};

#if defined (BEOS)
typedef UICTextEditImpl<BeTextControlAdapter, BTextControl*>	UICTextEditControlImpl;
typedef UICTextEditImpl<BeTextViewAdapter, BTextView*>			UICTextEditViewImpl;
#endif

#endif // _UICTEXTEDITIMPL_H_
