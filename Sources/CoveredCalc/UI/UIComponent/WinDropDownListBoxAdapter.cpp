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
	@file		WinDropDownListBoxAdapter.cpp
	@brief		Implementation of WinDropDownListBoxAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.01.08 created
*/

#include "Prefix.h"
#include "WinDropDownListBoxAdapter.h"

/**
 *	@brief	Constructor
 */
WinDropDownListBoxAdapter::WinDropDownListBoxAdapter()
{
	stoppedSelectionChangedNotificationCount = 0;
}

/**
 *	@brief	Destructor.
 */
WinDropDownListBoxAdapter::~WinDropDownListBoxAdapter()
{
}

/**
 *	@brief	Returns item count in the control.
 *	@return	item count.
 */
SInt32 WinDropDownListBoxAdapter::GetCount()
{
	return ComboBox_GetCount(hControl);
}

/**
 *	@brief	Adds one item to the control.
 *	@param[in]	text	item text.
 *	@param[in]	data	associated data of adding item.
 *	@return	the index of added item.
 */
SInt32 WinDropDownListBoxAdapter::AddItem(ConstAStr text, void* data)
{
	SInt32 index = ComboBox_AddString(hControl, text);
	if (CB_ERR != index)
	{
		ComboBox_SetItemData(hControl, index, data);
	}
	return index;
}

/**
 *	@brief	Removes one item from the control.
 *	@param[in]	index	item index.
 */
void WinDropDownListBoxAdapter::RemoveItem(SInt32 index)
{
	ComboBox_DeleteString(hControl, index);
}

/**
 *	@brief	Removes all item from the control.
 */
void WinDropDownListBoxAdapter::RemoveAllItem()
{
	ComboBox_ResetContent(hControl);
}

/**
 *	@brief	Gets a string at specified index from the control.
 *	@param[in]	index	index
 *	@param[out]	text	a string value is returned.
 */
void WinDropDownListBoxAdapter::GetItemText(SInt32 index, MBCString& text)
{
	AChar buf[256];	// fixed buffer for avoiding small memory allocation.
	AStr str = buf;
	SInt32 length = ComboBox_GetLBTextLen(hControl, index);
	if (CB_ERR == length)
	{
		text.Empty();
		return;
	}
	if (length + 1 > sizeof(buf) / sizeof(AChar))
	{
		str = new AChar[length + 1];
	}
	if (CB_ERR != ComboBox_GetLBText(hControl, index, str))
	{
		text = str;
	}
	else
	{
		text.Empty();
	}
	if (str != buf)
	{
		delete[] str;
	}
}

/**
 *	@brief	Returns associated data of specified index.
 *	@param[in]	index	item index.
 *	@return	associated data.
 */
void* WinDropDownListBoxAdapter::GetItemData(SInt32 index)
{
	return reinterpret_cast<void*>(ComboBox_GetItemData(hControl, index));
}

/**
 *	@brief	Sets associated data of specified index.
 *	@param[in]	index	item index.
 *	@param[in]	data	associated data.
 */
void WinDropDownListBoxAdapter::SetItemData(SInt32 index, void* data)
{
	ComboBox_SetItemData(hControl, index, data);
}

/**
 *	@brief	Returns an index of selected item.
 *	@return	item index of selected item. or -1 if no item is selected.
 */
SInt32 WinDropDownListBoxAdapter::GetSelectedItem()
{
	return ComboBox_GetCurSel(hControl);
}

/**
 *	@brief	Makes specified item selected.
 *	@param[in]	index	item index. if this is -1, no item is selected.
 */
void WinDropDownListBoxAdapter::SetSelectedItem(SInt32 index)
{
	ComboBox_SetCurSel(hControl, index);
}

/**
 *	@brief	Stops or restarts selection changed notification.
 *	@param[in]	doStop	true to stop, false to restart.
 */
void WinDropDownListBoxAdapter::StopSelectionChangedNotification(bool doStop)
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
