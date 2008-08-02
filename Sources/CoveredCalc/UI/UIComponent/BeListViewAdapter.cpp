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
	@file		BeListViewAdapter.cpp
	@brief		Implementation of BeListViewAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.19 created
*/

#include "Prefix.h"
#include "BeListViewAdapter.h"
#include <ListView.h>
#include "BeDataStringItem.h"
#include "CommandID.h"

/**
 *	@brief	Constructor
 */
BeListViewAdapter::BeListViewAdapter()
{
	isEnabled = true;
	stoppedSelectionChangedCommand = ID_NULL;
	stoppedSelectionChangedNotificationCount = 0;
}

/**
 *	@brief	Destructor.
 */
BeListViewAdapter::~BeListViewAdapter()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	view	target listview.
 */
void BeListViewAdapter::Init(BListView* view)
{
	BeViewAdapter::Init(view);
}

/**
 *	@brief	gets target BListView object.
 */
BListView* BeListViewAdapter::getListView()
{
	return static_cast<BListView*>(view);
}

/**
 *	@brief	Returns whether the control is enabled.
 *	@return true if enabled, otherwise disabled.
 */
bool BeListViewAdapter::IsEnabled()
{
	return isEnabled;
}

/**
 *	@brief	Makes the control enabled or disabled.
 *	@param[in]	doEnable	true to enable the control, or false to disable the control.
 */
void BeListViewAdapter::Enable(bool doEnable)
{
	BListView* listView = getListView();
	int32 count = listView->CountItems();
	int32 index;
	for (index = 0; index < count; index++)
	{
		BListItem* listItem = listView->ItemAt(index);
		listItem->SetEnabled(doEnable);
	}
	listView->LockLooper();
	listView->Invalidate();
	listView->UnlockLooper();
	
	isEnabled = doEnable;
}

/**
 *	@brief	Returns item count in the control.
 *	@return	item count.
 */
SInt32 BeListViewAdapter::GetCount()
{
	BListView* listView = getListView();
	return listView->CountItems();
}

typedef BeDataStringItem<void*> BeGenericDataStringItem;

/**
 *	@brief	Adds one item to the control.
 *	@param[in]	text	item text.
 *	@param[in]	data	associated data of adding item.
 *	@return	the index of added item.
 */
SInt32 BeListViewAdapter::AddItem(ConstAStr text, void* data)
{
	BeGenericDataStringItem* listItem = new BeGenericDataStringItem(text);
	listItem->SetItemData(data);
	
	BListView* listView = getListView();
	listView->AddItem(listItem);
	return listView->CountItems() - 1;
}

/**
 *	@brief	Removes one item from the control.
 *	@param[in]	index	item index.
 */
void BeListViewAdapter::RemoveItem(SInt32 index)
{
	BListView* listView = getListView();
	BListItem* listItem = listView->RemoveItem(index);
	if (NULL != listItem)
	{
		delete listItem;
	}
}

/**
 *	@brief	Removes all item from the control.
 */
void BeListViewAdapter::RemoveAllItem()
{
	BListView* listView = getListView();
	int32 count = listView->CountItems();
	int32 index;
	for (index = count - 1; index >= 0; index--)
	{
		BListItem* listItem = listView->RemoveItem(index);
		if (NULL != listItem)
		{
			delete listItem;
		}
	}
}

/**
 *	@brief	Gets a string at specified index from the control.
 *	@param[in]	index	index
 *	@param[out]	text	a string value is returned.
 */
void BeListViewAdapter::GetItemText(SInt32 index, MBCString& text)
{
	BListView* listView = getListView();
	BStringItem* listItem = dynamic_cast<BStringItem*>(listView->ItemAt(index));
	if (NULL != listItem)
	{
		text = listItem->Text();
	}
	else
	{
		text.Empty();
	}
}

/**
 *	@brief	Returns associated data of specified index.
 *	@param[in]	index	item index.
 *	@return	associated data.
 */
void* BeListViewAdapter::GetItemData(SInt32 index)
{
	BListView* listView = getListView();
	BeGenericDataStringItem* listItem = dynamic_cast<BeGenericDataStringItem*>(listView->ItemAt(index));
	if (NULL != listItem)
	{
		return listItem->GetItemData();
	}
	else
	{
		return NULL;
	}
}

/**
 *	@brief	Sets a string at specified index from the control.
 *	@param[in]	index	index
 *	@param[out]	text	a string value to set.
 */
void BeListViewAdapter::SetItemText(SInt32 index, ConstAStr text)
{
	BListView* listView = getListView();
	BeGenericDataStringItem* listItem = dynamic_cast<BeGenericDataStringItem*>(listView->ItemAt(index));
	if (NULL != listItem)
	{
		listItem->SetText(text);
	}
}

/**
 *	@brief	Sets associated data of specified index.
 *	@param[in]	index	item index.
 *	@param[in]	data	associated data.
 */
void BeListViewAdapter::SetItemData(SInt32 index, void* data)
{
	BListView* listView = getListView();
	BeGenericDataStringItem* listItem = dynamic_cast<BeGenericDataStringItem*>(listView->ItemAt(index));
	if (NULL != listItem)
	{
		listItem->SetItemData(data);
	}
}

/**
 *	@brief	Returns an index of selected item.
 *	@return	item index of selected item. or -1 if no item is selected.
 */
SInt32 BeListViewAdapter::GetSelectedItem()
{
	BListView* listView = getListView();
	int32 ret = listView->CurrentSelection(0);
	if (ret < 0)
	{
		return -1;
	}
	else
	{
		return ret;
	}
}

/**
 *	@brief	Makes specified item selected.
 *	@param[in]	index	item index. if this is -1, no item is selected.
 */
void BeListViewAdapter::SetSelectedItem(SInt32 index)
{
	BListView* listView = getListView();
	int32 current = listView->CurrentSelection(0);
	if (current != index)
	{
		listView->DeselectAll();
		listView->Select(index);
	}
}

/**
 *	@brief	Stops or restarts selection changed notification.
 *	@param[in]	doStop	true to stop, false to restart.
 */
void BeListViewAdapter::StopSelectionChangedNotification(bool doStop)
{
	BListView* listView = getListView();
	if (doStop)
	{
		if (0 == stoppedSelectionChangedNotificationCount)
		{
			BMessage* message = listView->SelectionMessage();
			stoppedSelectionChangedCommand = message->what;
			message->what = ID_NULL;
		}
		stoppedSelectionChangedNotificationCount++;
	}
	else
	{
		stoppedSelectionChangedNotificationCount--;
		if (0 == stoppedSelectionChangedNotificationCount)
		{
			BMessage* message = listView->SelectionMessage();
			message->what = stoppedSelectionChangedCommand;
			stoppedSelectionChangedCommand = ID_NULL;
		}
	}
}
