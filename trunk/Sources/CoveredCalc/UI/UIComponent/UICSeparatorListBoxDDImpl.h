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
	@file		UICSeparatorListBoxDDImpl.h
	@brief		Definition of UICSeparatorListBoxDDImpl class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.01.08 created
*/

#ifndef _UICSEPARATORLISTBOXDDIMPL_H_
#define _UICSEPARATORLISTBOXDDIMPL_H_

#include "UICSeparatorListBox.h"

#if defined (WIN32)
#include "WinSeparatorDDListBoxAdapter.h"
#elif defined (BEOS)
#include "FIXME.h"
#endif

/**
 *	@brief	drop-down list box component interface implementation.
 */
class UICSeparatorListBoxDDImpl : public UICSeparatorListBox
{
public:
								UICSeparatorListBoxDDImpl() { }
	virtual						~UICSeparatorListBoxDDImpl() { }
	
	virtual bool				IsEnabled() { return delegateObj.IsEnabled(); }
	virtual void				Enable(bool isEnabled) { delegateObj.Enable(isEnabled); }
	
	virtual void				MakeFocus() { delegateObj.MakeFocus(); }
	
	virtual SInt32				GetCount() { return delegateObj.GetCount(); }
	virtual SInt32				AddItem(ConstAStr text, void* data) { return delegateObj.AddItem(text, data); }
	virtual void				RemoveItem(SInt32 index) { delegateObj.RemoveItem(index); }
	virtual void				RemoveAllItem() { delegateObj.RemoveAllItem(); }
	
	virtual void				SetSeparator(SInt32 index, bool isSet) { delegateObj.SetSeparator(index, isSet); }
	virtual bool				HasSeparator(SInt32 index) { return delegateObj.HasSeparator(index); }
	
	virtual void				GetItemText(SInt32 index, MBCString& text) { delegateObj.GetItemText(index, text); }
	virtual void*				GetItemData(SInt32 index) { return delegateObj.GetItemData(index); }
	virtual void				SetItemData(SInt32 index, void* data) { delegateObj.SetItemData(index, data); }
	
	virtual SInt32				GetSelectedItem() { return delegateObj.GetSelectedItem(); }
	virtual void				SetSelectedItem(SInt32 index) { delegateObj.SetSelectedItem(index); }

	virtual void				StopSelectionChangedNotification(bool doStop) { delegateObj.StopSelectionChangedNotification(doStop); }

#if defined (WIN32)
public:
	void						Init(HWND hControl, WinComboListSeparators* separators) { delegateObj.Init(hControl, separators); }
	WinSeparatorDDListBoxAdapter*	GetRawAdapter() { return &delegateObj; }
private:
	WinSeparatorDDListBoxAdapter	delegateObj;
#elif defined (BEOS)
//FIXME:
public:
	void						Init(BListView* view) { delegateObj.Init(view); }
	BeListViewAdapter*			GetRawAdapter() { return &delegateObj; }
private:
	BeListViewAdapter			delegateObj;
#endif
};

#endif // _UICSEPARATORLISTBOXDDIMPL_H_
