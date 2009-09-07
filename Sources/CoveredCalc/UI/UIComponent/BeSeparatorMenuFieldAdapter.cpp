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
	@file		BeSeparatorMenuFieldAdapter.cpp
	@brief		Implementation of BeSeparatorMenuFieldAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.03.07 created
*/

#include "Prefix.h"
#include "BeSeparatorMenuFieldAdapter.h"
#include <MenuField.h>
#include <MenuItem.h>

////////////////////////////////////////
#define base		BeMenuFieldAdapter
////////////////////////////////////////

/**
 *	@brief	Constructor
 */
BeSeparatorMenuFieldAdapter::BeSeparatorMenuFieldAdapter()
{
}

/**
 *	@brief	Destructor.
 */
BeSeparatorMenuFieldAdapter::~BeSeparatorMenuFieldAdapter()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	menuField	target menu field.
 *	@param[in]	selectedCommand	command number posted when menu is selected.
 */
void BeSeparatorMenuFieldAdapter::Init(BMenuField* menuField, uint32 selectedCommand)
{
	base::Init(menuField, selectedCommand);
	menuItemVector.clear();
}

/**
 *	@brief	Returns item count in the control.
 *	@return	item count.
 */
SInt32 BeSeparatorMenuFieldAdapter::GetCount()
{
	return menuItemVector.size();
}

/**
 *	@brief	Adds one item to the control.
 *	@param[in]	text	item text.
 *	@param[in]	data	associated data of adding item.
 *	@return	the index of added item.
 */
SInt32 BeSeparatorMenuFieldAdapter::AddItem(ConstAStr text, void* data)
{
	SInt32 aix = base::AddItem(text, data);
	
	MenuItem menuItem;
	menuItem.rawIndex = aix;
	menuItem.hasSeparator = false;
	menuItemVector.push_back(menuItem);
	return menuItemVector.size() - 1;
}

/**
 *	@brief	Removes one item from the control.
 *	@param[in]	index	item index.
 */
void BeSeparatorMenuFieldAdapter::RemoveItem(SInt32 index)
{
	SInt32 dix = 1;
	const MenuItem& item = menuItemVector[index];
	if (item.hasSeparator)
	{
		base::RemoveItem(item.rawIndex + 1);
		++dix;
	}
	base::RemoveItem(item.rawIndex);
	menuItemVector.erase(menuItemVector.begin() + index);
	SInt32 ix;
	SInt32 count = menuItemVector.size();
	for (ix = index; ix < count; ++ix)
	{
		menuItemVector[ix].rawIndex -= dix;
	}
}

/**
 *	@brief	Removes all item from the control.
 */
void BeSeparatorMenuFieldAdapter::RemoveAllItem()
{
	menuItemVector.clear();
	base::RemoveAllItem();
}

/**
 *	@brief	Set or reset a separator at specified index.
 *	@param[in]	index	separator index. (0 is the bottom of first item, 1 is the bottom of second, ...)
 *	@param[in]	isSet	true if set separator, or reset (remove) it.
 */
void BeSeparatorMenuFieldAdapter::SetSeparator(SInt32 index, bool isSet)
{
	SInt32 dix;
	MenuItem& item = menuItemVector[index];
	if (isSet)
	{
		if (item.hasSeparator)
			return;
		
		BMenuField* menuField = getMenuField();
		BMenu* menu = menuField->Menu();
		menu->AddItem(new BSeparatorItem, item.rawIndex + 1);
		dix = 1;
	}
	else
	{
		if (!item.hasSeparator)
			return;
		
		base::RemoveItem(item.rawIndex + 1);
		dix = -1;
	}

	SInt32 ix;
	SInt32 count = menuItemVector.size();
	for (ix = index + 1; ix < count; ++ix)
	{
		menuItemVector[ix].rawIndex += dix;
	}
}

/**
 *	@brief	Returns true if separator exists at specified index.
 *	@param[in]	index	index.
 *	@return	true if exists.
 */
bool BeSeparatorMenuFieldAdapter::HasSeparator(SInt32 index)
{
	return menuItemVector[index].hasSeparator;
}

/**
 *	@brief	Gets a string at specified index from the control.
 *	@param[in]	index	index
 *	@param[out]	text	a string value is returned.
 */
void BeSeparatorMenuFieldAdapter::GetItemText(SInt32 index, MBCString& text)
{
	base::GetItemText(menuItemVector[index].rawIndex, text);
}

/**
 *	@brief	Returns associated data of specified index.
 *	@param[in]	index	item index.
 *	@return	associated data.
 */
void* BeSeparatorMenuFieldAdapter::GetItemData(SInt32 index)
{
	return base::GetItemData(menuItemVector[index].rawIndex);
}

/**
 *	@brief	Sets a string at specified index from the control.
 *	@param[in]	index	index
 *	@param[out]	text	a string value to set.
 */
void BeSeparatorMenuFieldAdapter::SetItemText(SInt32 index, ConstAStr text)
{
	base::SetItemText(menuItemVector[index].rawIndex, text);
}

/**
 *	@brief	Sets associated data of specified index.
 *	@param[in]	index	item index.
 *	@param[in]	data	associated data.
 */
void BeSeparatorMenuFieldAdapter::SetItemData(SInt32 index, void* data)
{
	base::SetItemData(menuItemVector[index].rawIndex, data);
}

/**
 *	@brief	Returns an index of selected item.
 *	@return	item index of selected item. or -1 if no item is selected.
 */
SInt32 BeSeparatorMenuFieldAdapter::GetSelectedItem()
{
	SInt32 rawSelected = base::GetSelectedItem();
	if (-1 == rawSelected)
		return rawSelected;
	
	SInt32 index;
	SInt32 count = menuItemVector.size();
	for (index = 0; index < count; ++index)
	{
		if (rawSelected == menuItemVector[index].rawIndex)
			return index;
	}
	return -1;
}

/**
 *	@brief	Makes specified item selected.
 *	@param[in]	index	item index. if this is -1, no item is selected.
 */
void BeSeparatorMenuFieldAdapter::SetSelectedItem(SInt32 index)
{
	base::SetSelectedItem(menuItemVector[index].rawIndex);
}
