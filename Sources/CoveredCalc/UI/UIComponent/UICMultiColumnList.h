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
	@file		UICMultiCulumnList.h
	@brief		Definition of UICMultiCulumnList interface.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.03.17 created
*/

#ifndef _UICMULTICOLUMNLIST_H_
#define _UICMULTICOLUMNLIST_H_

/**
 *	@brief	Multi column list-box component interface.
 */
class UICMultiColumnList
{
public:
						UICMultiColumnList() { }
	virtual				~UICMultiColumnList() { }
	
	virtual bool		IsEnabled() = 0;
	virtual void		Enable(bool isEnabled) = 0;
	
	virtual void		MakeFocus() = 0;
	
	virtual SInt32		GetCount() = 0;
	virtual SInt32		AddItem(ConstAStr* texts, void* data) = 0;
	virtual void		RemoveItem(SInt32 rowIndex) = 0;
	virtual void		RemoveAllItem() = 0;
	
	virtual void		GetItemText(SInt32 rowIndex, SInt32 columnIndex, MBCString& text) = 0;
	virtual void*		GetItemData(SInt32 rowIndex) = 0;
	virtual void		SetItemData(SInt32 rowIndex, void* data) = 0;
	
	virtual SInt32		GetSelectedItem() = 0;
	virtual void		SetSelectedItem(SInt32 rowIndex) = 0;

	virtual void		StopSelectionChangedNotification(bool doStop) = 0;
};

#endif // _UICMULTICOLUMNLIST_H_
