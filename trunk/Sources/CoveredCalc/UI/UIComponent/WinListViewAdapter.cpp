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
	@file		WinListViewAdapter.cpp
	@brief		Implementation of WinListViewAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.03.17 created
*/

#include "Prefix.h"
#include "WinListViewAdapter.h"

/**
 *	@brief	Constructor
 */
WinListViewAdapter::WinListViewAdapter()
{
	stoppedSelectionChangedNotificationCount = 0;
}

/**
 *	@brief	Destructor.
 */
WinListViewAdapter::~WinListViewAdapter()
{
}

/**
 *	@brief	Initialzes the object.
 *	@param[in]	hControl	handle to target control.
 */
void WinListViewAdapter::Init(HWND hControl)
{
	WinControlAdapter::Init(hControl);

	HWND hHeader = ListView_GetHeader(hControl);
	columnCount = Header_GetItemCount(hHeader);
}

/**
 *	@brief	Returns item count in the control.
 *	@return	item count.
 */
SInt32 WinListViewAdapter::GetCount()
{
	return ListView_GetItemCount(hControl);
}

/**
 *	@brief	Adds one item to the control.
 *	@param[in]	texts	item texts for each column.
 *	@param[in]	data	associated data of adding item.
 *	@return	the index of added item.
 */
SInt32 WinListViewAdapter::AddItem(ConstAStr* texts, void* data)
{
	LVITEM item;
	ZeroMemory(&item, sizeof(item));

	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.iItem = ListView_GetItemCount(hControl);
	item.lParam = reinterpret_cast<LPARAM>(data);
	item.pszText = const_cast<LPTSTR>(texts[0]);
	SInt32 itemIndex = ListView_InsertItem(hControl, &item);
	if (0 <= itemIndex)
	{	
		SInt32 colIndex;
		for (colIndex = 1; colIndex < columnCount; ++colIndex)
		{
			ListView_SetItemText(hControl, itemIndex, colIndex, const_cast<LPTSTR>(texts[colIndex]));
		}
	}
	return itemIndex;
}

/**
 *	@brief	Removes one item from the control.
 *	@param[in]	rowIndex	item index.
 */
void WinListViewAdapter::RemoveItem(SInt32 rowIndex)
{
	ListView_DeleteItem(hControl, rowIndex);
}

/**
 *	@brief	Removes all item from the control.
 */
void WinListViewAdapter::RemoveAllItem()
{
	ListView_DeleteAllItems(hControl);
}

/**
 *	@brief	Gets a string at specified index from the control.
 *	@param[in]	rowIndex		row index
 *	@param[in]	columnIndex		column index
 *	@param[out]	text	a string value is returned.
 */
void WinListViewAdapter::GetItemText(SInt32 rowIndex, SInt32 columnIndex, MBCString& text)
{
	AChar buf[256];	// fixed buffer for avoiding small memory allocation.
	AStr str = buf;

	LVITEM item;
	ZeroMemory(&item, sizeof(item));
	item.iSubItem = columnIndex;
	item.cchTextMax = 0;
	item.pszText = str;
	SInt32 length = SendMessage(hControl, LVM_GETITEMTEXT, rowIndex, reinterpret_cast<LPARAM>(&item));
	if (length + 1 > sizeof(buf) / sizeof(AChar))
	{
		str = new AChar[length + 1];
	}
	item.cchTextMax = length + 1;
	item.pszText = str;
	SendMessage(hControl, LVM_GETITEMTEXT, rowIndex, reinterpret_cast<LPARAM>(&item));
	text = str;
	if (str != buf)
	{
		delete[] str;
	}
}

/**
 *	@brief	Returns associated data of specified index.
 *	@param[in]	rowIndex	item index.
 *	@return	associated data.
 */
void* WinListViewAdapter::GetItemData(SInt32 rowIndex)
{
	LVITEM item;
	ZeroMemory(&item, sizeof(item));
	item.mask = LVIF_PARAM;
	item.iItem = rowIndex;
	ListView_GetItem(hControl, &item);
	return reinterpret_cast<void*>(item.lParam);
}

/**
 *	@brief	Sets associated data of specified index.
 *	@param[in]	rowIndex	item index.
 *	@param[in]	data	associated data.
 */
void WinListViewAdapter::SetItemData(SInt32 rowIndex, void* data)
{
	LVITEM item;
	ZeroMemory(&item, sizeof(item));
	item.mask = LVIF_PARAM;
	item.iItem = rowIndex;
	item.lParam = reinterpret_cast<LPARAM>(data);
	ListView_SetItem(hControl, &item);
}

/**
 *	@brief	Returns an index of selected item.
 *	@return	item index of selected item. or -1 if no item is selected.
 */
SInt32 WinListViewAdapter::GetSelectedItem()
{
	return ListView_GetNextItem(hControl, -1, LVIS_SELECTED);
}

/**
 *	@brief	Makes specified item selected.
 *	@param[in]	rowIndex	item index. if this is -1, no item is selected.
 */
void WinListViewAdapter::SetSelectedItem(SInt32 rowIndex)
{
	SInt32 selectedIndex = ListView_GetNextItem(hControl, -1, LVIS_SELECTED);
	while (0 <= selectedIndex)
	{
		ListView_SetItemState(hControl, selectedIndex, 0, LVIS_SELECTED);
		selectedIndex = ListView_GetNextItem(hControl, selectedIndex + 1, LVIS_SELECTED);
	}
	ListView_SetItemState(hControl, rowIndex, LVIS_SELECTED, LVIS_SELECTED);
}

/**
 *	@brief	Stops or restarts selection changed notification.
 *	@param[in]	doStop	true to stop, false to restart.
 */
void WinListViewAdapter::StopSelectionChangedNotification(bool doStop)
{
	if (doStop)
	{
		stoppedSelectionChangedNotificationCount++;
	}
	else
	{
		stoppedSelectionChangedNotificationCount--;
	}
}
