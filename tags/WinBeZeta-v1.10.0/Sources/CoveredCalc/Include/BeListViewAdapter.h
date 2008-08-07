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
	@file		BeListViewAdapter.h
	@brief		Definition of BeListViewAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.06.07 created
*/

#ifndef _BELISTVIEWADAPTER_H_
#define _BELISTVIEWADAPTER_H_

#include "BeViewAdapter.h"
#include "MBCString.h"

class BListView;

/**
 *	@brief	Adapter class of ListBox for BeOS.
 */
class BeListViewAdapter : public BeViewAdapter
{
public:
						BeListViewAdapter();
	virtual				~BeListViewAdapter();
	
	void				Init(BListView* listView);
	
	bool				IsEnabled();
	void				Enable(bool doEnable);

	SInt32				GetCount();
	SInt32				AddItem(ConstAStr text, void* data);
	void				RemoveItem(SInt32 index);
	void				RemoveAllItem();
	
	void				GetItemText(SInt32 index, MBCString& text);
	void*				GetItemData(SInt32 index);
	void				SetItemText(SInt32 index, ConstAStr text);
	void				SetItemData(SInt32 index, void* data);
	
	SInt32				GetSelectedItem();
	void				SetSelectedItem(SInt32 index);

	void				StopSelectionChangedNotification(bool doStop);

protected:
	BListView*			getListView();

private:
	bool				isEnabled;
	uint32				stoppedSelectionChangedCommand;
	SInt32				stoppedSelectionChangedNotificationCount;
};

#endif // _BELISTVIEWADAPTER_H_
