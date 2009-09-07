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
	@file		UICMultiCulumnListImpl.h
	@brief		Definition of UICMultiCulumnListImpl class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.03.17 created
*/

#ifndef _UICMULTICOLUMNLISTIMPL_H_
#define _UICMULTICOLUMNLISTIMPL_H_

#include "UICMultiColumnList.h"
#if defined (WIN32)
#include "WinListViewAdapter.h"
#elif defined (BEOS)
#include "BeColumnListViewAdapter.h"
#endif

/**
 *	@brief	Multi column list-box component class.
 */
class UICMultiColumnListImpl : public UICMultiColumnList
{
public:
						UICMultiColumnListImpl() { }
	virtual				~UICMultiColumnListImpl() { }
	
	virtual bool		IsEnabled() { return delegateObj.IsEnabled(); }
	virtual void		Enable(bool isEnabled) { delegateObj.Enable(isEnabled); }
	
	virtual void		MakeFocus() { delegateObj.MakeFocus(); }
	
	virtual SInt32		GetCount() { return delegateObj.GetCount(); }
	virtual SInt32		AddItem(ConstAStr* texts, void* data) { return delegateObj.AddItem(texts, data); }
	virtual void		RemoveItem(SInt32 rowIndex) { delegateObj.RemoveItem(rowIndex); }
	virtual void		RemoveAllItem() { delegateObj.RemoveAllItem(); }
	
	virtual void		GetItemText(SInt32 rowIndex, SInt32 columnIndex, MBCString& text)
							{ delegateObj.GetItemText(rowIndex, columnIndex, text); }
	virtual void*		GetItemData(SInt32 rowIndex) { return delegateObj.GetItemData(rowIndex); }
	virtual void		SetItemData(SInt32 rowIndex, void* data) { delegateObj.SetItemData(rowIndex, data); }
	
	virtual SInt32		GetSelectedItem() { return delegateObj.GetSelectedItem(); }
	virtual void		SetSelectedItem(SInt32 rowIndex) { delegateObj.SetSelectedItem(rowIndex); }

	virtual void		StopSelectionChangedNotification(bool doStop)
							{ delegateObj.StopSelectionChangedNotification(doStop); }

#if defined (WIN32)
public:
	void						Init(HWND hControl) { delegateObj.Init(hControl); }
	WinListViewAdapter	*		GetRawAdapter() { return &delegateObj; }
private:
	WinListViewAdapter			delegateObj;
#elif defined (BEOS)
public:
	void						Init(BColumnListView* view) { delegateObj.Init(view); }
	BeColumnListViewAdapter*	GetRawAdapter() { return &delegateObj; }
private:
	BeColumnListViewAdapter		delegateObj;
#endif
};

#endif // _UICMULTICOLUMNLISTIMPL_H_
