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
	@file		BeColumnListViewAdapter.h
	@brief		Definition of BeColumnListViewAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.03.21 created
*/

#ifndef _BECOLUMNLISTVIEWADAPTER_H_
#define _BECOLUMNLISTVIEWADAPTER_H_

#include "BeViewAdapter.h"
#include "MBCString.h"

class BColumnListView;

/**
 *	@brief	Implementation of adapter class for BView.
 */
class BeColumnListViewAdapter : public BeViewAdapter
{
public:
						BeColumnListViewAdapter();
	virtual				~BeColumnListViewAdapter();
	
	void				Init(BColumnListView* columnListView);
	
	bool				IsEnabled();
	void				Enable(bool isEnabled);
	
	SInt32				GetCount();
	SInt32				AddItem(ConstAStr* texts, void* data);
	void				RemoveItem(SInt32 rowIndex);
	void				RemoveAllItem();
	
	void				GetItemText(SInt32 rowIndex, SInt32 columnIndex, MBCString& text);
	void*				GetItemData(SInt32 rowIndex);
	void				SetItemData(SInt32 rowIndex, void* data);
	
	SInt32				GetSelectedItem();
	void				SetSelectedItem(SInt32 rowIndex);

	void				StopSelectionChangedNotification(bool doStop);

private:
	BColumnListView*	getColumnListView();

private:
	SInt32				columnCount;
	uint32				stoppedSelectionChangedCommand;
	SInt32				stoppedSelectionChangedNotificationCount;
};

#endif // _BECOLUMNLISTVIEWADAPTER_H_
