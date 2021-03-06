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
	@file		UICListBoxImpl.h
	@brief		Definition of UICListBoxImpl class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.28 created
*/

#ifndef _UICLISTBOXIMPL_H_
#define _UICLISTBOXIMPL_H_

#include "UICListBox.h"

#if defined (WIN32)
#include "WinListBoxAdapter.h"
#elif defined (BEOS)
#include "BeListViewAdapter.h"
#endif

/**
 *	@brief	List-box component interface.
 */
class UICListBoxImpl : public UICListBox
{
public:
						UICListBoxImpl() { }
	virtual				~UICListBoxImpl() { }
	
	virtual bool		IsEnabled() { return delegateObj.IsEnabled(); }
	virtual void		Enable(bool isEnabled) { delegateObj.Enable(isEnabled); }
	
	virtual void		MakeFocus() { delegateObj.MakeFocus(); }
	
	virtual SInt32		GetCount() { return delegateObj.GetCount(); }
	virtual SInt32		AddItem(ConstAStr text, void* data) { return delegateObj.AddItem(text, data); }
	virtual void		RemoveItem(SInt32 index) { delegateObj.RemoveItem(index); }
	virtual void		RemoveAllItem() { delegateObj.RemoveAllItem(); }
	
	virtual void		GetItemText(SInt32 index, MBCString& text) { delegateObj.GetItemText(index, text); }
	virtual void*		GetItemData(SInt32 index) { return delegateObj.GetItemData(index); }
	virtual void		SetItemData(SInt32 index, void* data) { delegateObj.SetItemData(index, data); }
	
	virtual SInt32		GetSelectedItem() { return delegateObj.GetSelectedItem(); }
	virtual void		SetSelectedItem(SInt32 index) { delegateObj.SetSelectedItem(index); }

	virtual void		StopSelectionChangedNotification(bool doStop) { delegateObj.StopSelectionChangedNotification(doStop); }

#if defined (WIN32)
public:
	void				Init(HWND hControl) { delegateObj.Init(hControl); }
	WinListBoxAdapter*	GetRawAdapter() { return &delegateObj; }
private:
	WinListBoxAdapter	delegateObj;
#elif defined (BEOS)
public:
	void				Init(BListView* view) { delegateObj.Init(view); }
	BeListViewAdapter*	GetRawAdapter() { return &delegateObj; }
private:
	BeListViewAdapter	delegateObj;
#endif
};

#endif // _UICLISTBOXIMPL_H_
