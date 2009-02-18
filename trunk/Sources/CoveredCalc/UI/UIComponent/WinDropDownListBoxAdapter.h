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
	@file		WinDropDownListBoxAdapter.h
	@brief		Definition of WinDropDownListBoxAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.01.08 created
*/

#ifndef _WINDROPDOWNLISTBOXADAPTER_H_
#define _WINDROPDOWNLISTBOXADAPTER_H_

#include "WinControlAdapter.h"
#include "MBCString.h"

/**
 *	@brief	Adapter class of drop-down list box (one form of the combo box) for Windows.
 */
class WinDropDownListBoxAdapter : public WinControlAdapter
{
public:
						WinDropDownListBoxAdapter();
	virtual				~WinDropDownListBoxAdapter();
	
	SInt32				GetCount();
	SInt32				AddItem(ConstAStr text, void* data);
	virtual void		RemoveItem(SInt32 index);
	virtual void		RemoveAllItem();
	
	void				GetItemText(SInt32 index, MBCString& text);
	void*				GetItemData(SInt32 index);
	void				SetItemData(SInt32 index, void* data);
	
	SInt32				GetSelectedItem();
	void				SetSelectedItem(SInt32 index);

	void				StopSelectionChangedNotification(bool doStop);
	bool				IsSelectionChangedNotificationStopped() { return stoppedSelectionChangedNotificationCount > 0; }

private:
	SInt32				stoppedSelectionChangedNotificationCount;
};

#endif // _WINDROPDOWNLISTBOXADAPTER_H_
