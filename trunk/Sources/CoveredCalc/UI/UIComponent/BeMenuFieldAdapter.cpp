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
	@file		BeMenuFieldAdapter.cpp
	@brief		Implementation of BeMenuFieldAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.03.01 created
*/

#include "Prefix.h"
#include "BeMenuFieldAdapter.h"
#include <MenuField.h>
#include "BeDataMenuItem.h"
#include "CommandID.h"

/**
 *	@brief	Constructor
 */
BeMenuFieldAdapter::BeMenuFieldAdapter()
{
	selectedCommand = ID_NULL;
	stoppedSelectionChangedNotificationCount = 0;
}

/**
 *	@brief	Destructor.
 */
BeMenuFieldAdapter::~BeMenuFieldAdapter()
{
	if (NULL != getMenuField())
	{
		RemoveAllItem();
	}
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	menuField	target menu field.
 *	@param[in]	selectedCommand	command number posted when menu is selected.
 */
void BeMenuFieldAdapter::Init(BMenuField* menuField, uint32 selectedCommand)
{
	BeViewAdapter::Init(menuField);
	this->selectedCommand = selectedCommand;
}

/**
 *	@brief	gets target BMenuField object.
 */
BMenuField* BeMenuFieldAdapter::getMenuField()
{
	return static_cast<BMenuField*>(view);
}

/**
 *	@brief	Returns whether the control is enabled.
 *	@return true if enabled, otherwise disabled.
 */
bool BeMenuFieldAdapter::IsEnabled()
{
	BMenuField* menuField = getMenuField();
	return menuField->IsEnabled();
}

/**
 *	@brief	Makes the control enabled or disabled.
 *	@param[in]	doEnable	true to enable the control, or false to disable the control.
 */
void BeMenuFieldAdapter::Enable(bool doEnable)
{
	BMenuField* menuField = getMenuField();
	menuField->SetEnabled(doEnable);
}

/**
 *	@brief	Returns item count in the control.
 *	@return	item count.
 */
SInt32 BeMenuFieldAdapter::GetCount()
{
	BMenuField* menuField = getMenuField();
	BMenu* menu = menuField->Menu();
	return menu->CountItems();
}

typedef BeDataMenuItem<void*> BeGenericDataMenuItem;

/**
 *	@brief	Adds one item to the control.
 *	@param[in]	text	item text.
 *	@param[in]	data	associated data of adding item.
 *	@return	the index of added item.
 */
SInt32 BeMenuFieldAdapter::AddItem(ConstAStr text, void* data)
{
	BeGenericDataMenuItem* menuItem = new BeGenericDataMenuItem(text, new BMessage(selectedCommand));
	menuItem->SetItemData(data);
	
	BMenuField* menuField = getMenuField();
	BMenu* menu = menuField->Menu();
	menu->AddItem(menuItem);
	return menu->CountItems() - 1;
}

/**
 *	@brief	Removes one item from the control.
 *	@param[in]	index	item index.
 */
void BeMenuFieldAdapter::RemoveItem(SInt32 index)
{
	BMenuField* menuField = getMenuField();
	BMenu* menu = menuField->Menu();
	BMenuItem* menuItem = menu->RemoveItem(index);
	if (NULL != menuItem)
	{
		delete menuItem;
	}
}

/**
 *	@brief	Removes all item from the control.
 */
void BeMenuFieldAdapter::RemoveAllItem()
{
	BMenuField* menuField = getMenuField();
	BMenu* menu = menuField->Menu();
	int32 count = menu->CountItems();
	int32 index;
	for (index = count - 1; index >= 0; index--)
	{
		BMenuItem* menuItem = menu->RemoveItem(index);
		if (NULL != menuItem)
		{
			delete menuItem;
		}
	}
}

/**
 *	@brief	Gets a string at specified index from the control.
 *	@param[in]	index	index
 *	@param[out]	text	a string value is returned.
 */
void BeMenuFieldAdapter::GetItemText(SInt32 index, MBCString& text)
{
	BMenuField* menuField = getMenuField();
	BMenu* menu = menuField->Menu();
	BMenuItem* menuItem = menu->ItemAt(index);
	if (NULL != menuItem)
	{
		text = menuItem->Label();
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
void* BeMenuFieldAdapter::GetItemData(SInt32 index)
{
	BMenuField* menuField = getMenuField();
	BMenu* menu = menuField->Menu();
	BeGenericDataMenuItem* menuItem = dynamic_cast<BeGenericDataMenuItem*>(menu->ItemAt(index));
	if (NULL != menuItem)
	{
		return menuItem->GetItemData();
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
void BeMenuFieldAdapter::SetItemText(SInt32 index, ConstAStr text)
{
	BMenuField* menuField = getMenuField();
	BMenu* menu = menuField->Menu();
	BeGenericDataMenuItem* menuItem = dynamic_cast<BeGenericDataMenuItem*>(menu->ItemAt(index));
	if (NULL != menuItem)
	{
		menuItem->SetLabel(text);
	}
}

/**
 *	@brief	Sets associated data of specified index.
 *	@param[in]	index	item index.
 *	@param[in]	data	associated data.
 */
void BeMenuFieldAdapter::SetItemData(SInt32 index, void* data)
{
	BMenuField* menuField = getMenuField();
	BMenu* menu = menuField->Menu();
	BeGenericDataMenuItem* menuItem = dynamic_cast<BeGenericDataMenuItem*>(menu->ItemAt(index));
	if (NULL != menuItem)
	{
		menuItem->SetItemData(data);
	}
}

/**
 *	@brief	Returns an index of selected item.
 *	@return	item index of selected item. or -1 if no item is selected.
 */
SInt32 BeMenuFieldAdapter::GetSelectedItem()
{
	BMenuField* menuField = getMenuField();
	BMenu* menu = menuField->Menu();
	int32 index, numItems;
	numItems = menu->CountItems();
	for (index = 0; index < numItems; ++index)
	{
		BMenuItem* menuItem = menu->ItemAt(index);
		if (menuItem->IsMarked())
		{
			return index;
		}
	}
	return -1;
}

/**
 *	@brief	Makes specified item selected.
 *	@param[in]	index	item index. if this is -1, no item is selected.
 */
void BeMenuFieldAdapter::SetSelectedItem(SInt32 index)
{
	BMenuField* menuField = getMenuField();
	BMenu* menu = menuField->Menu();
	BMenuItem* menuItem = menu->ItemAt(index);
	menuItem->SetMarked(true);
}

/**
 *	@brief	Stops or restarts selection changed notification.
 *	@param[in]	doStop	true to stop, false to restart.
 */
void BeMenuFieldAdapter::StopSelectionChangedNotification(bool doStop)
{
	bool doChangeCommand = false;
	uint32 prevCommand, nextCommand;
	if (doStop)
	{
		if (0 == stoppedSelectionChangedNotificationCount)
		{
			doChangeCommand = true;
			prevCommand = selectedCommand;
			nextCommand = ID_NULL;
		}
		stoppedSelectionChangedNotificationCount++;
	}
	else
	{
		stoppedSelectionChangedNotificationCount--;
		if (0 == stoppedSelectionChangedNotificationCount)
		{
			doChangeCommand = true;
			prevCommand = ID_NULL;
			nextCommand = selectedCommand;
		}
	}

	if (doChangeCommand)
	{
		BMenuField* menuField = getMenuField();
		BMenu* menu = menuField->Menu();
		int32 index, numItems;
		numItems = menu->CountItems();
		for (index = 0; index < numItems; ++index)
		{
			BMenuItem* menuItem = menu->ItemAt(index);
			BMessage* message = menuItem->Message();
			if (NULL != message && prevCommand == message->what)
			{
				message->what = nextCommand;
			}
		}
	}
}
