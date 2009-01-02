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
	@file		DialogLayout.cpp
	@brief		Implementation of DialogLayout class.
	@author 	ICHIMIYA Hironori (Hiron)
	@date		2008.10.29 created
*/

#include "Prefix.h"
#include "DialogLayout.h"
#include "DialogLayoutException.h"

/**
 *	@brief	Constructor
 */
DialogLayout::DialogLayout()
{
}

/**
 *	@brief	Destructor
 */
DialogLayout::~DialogLayout()
{
}

/**
 *	@brief	Adds layout item.
 *	@param[in]	name			name of the item
 *	@param[in]	offsetFrom		specify name of the other item when value parameter is offset from its value, otherwise specify an empty string.
 *	@param[in]	value			value
 */
void DialogLayout::SetLayoutItem(const MBCString& name, const MBCString& offsetFrom, DLValue value)
{
	DLItem item;
	item.offsetFrom = offsetFrom;
	item.value = value;
	item.computedValue = 0;
	item.state = ItemState_Uncomputed;
	
	itemMap[name] = item;
}

/**
 *	@brief	Retrieves layout item.
 *	@param[in]	name			name of the item
 *	@param[out]	outOffsetFrom	name of the other item is retuend when value parameter is offset from its value, otherwise an empty string is retuend.
 *	@param[out]	outValue		value is returned (this is not a computed value).
 */
bool DialogLayout::GetLayoutItem(const MBCString& name, MBCString& outOffsetFrom, DLValue& outValue) const
{
	DLItemMap::const_iterator itr = itemMap.find(name);
	if (itr == itemMap.end())
	{
		return false;
	}
	else
	{
		const DLItem& item = (*itr).second;
		outOffsetFrom = item.offsetFrom;
		outValue = item.value;
		return true;
	}
}

/**
 *	@brief	Deletes all layout items.
 */
void DialogLayout::ClearAllLayoutItems()
{
	itemMap.clear();
}

/**
 *	@brief	Compute all items.
 *	@throw	DialogLayoutExceptions::FailedToCompute	when failed.
 */
void DialogLayout::Compute()
{
	DLItemMap::iterator itr;
	for (itr = itemMap.begin(); itr != itemMap.end(); ++itr)
	{
		DLItem& item = (*itr).second;
		item.state = ItemState_Uncomputed;
	}
	
	for (itr = itemMap.begin(); itr != itemMap.end(); ++itr)
	{
		DLItem& item = (*itr).second;
		if (ItemState_Computed != item.state)
		{
			try
			{
				computeOneItem(item);
			}
			catch (DialogLayoutException* ex)
			{
				throw new DialogLayoutExceptions::FailedToCompute(ex, (*itr).first);
			}
		}
	}
}

/**
 *	@brief	Compute one item.
 *	@param[in]	item	item to be calculated.
 *	@throw	DialogLayoutException	when item has invalid reference or circular reference.
 */
void DialogLayout::computeOneItem(DLItem& item)
{
	if (ItemState_Computing == item.state)
	{
		// detect a circular reference!
		throw new DialogLayoutExceptions::CircularReference;
	}
	item.state = ItemState_Computing;
	
	if (item.offsetFrom.IsEmpty())
	{
		item.computedValue = item.value;
	}
	else
	{
		DLItemMap::iterator itr = itemMap.find(item.offsetFrom);
		if (itr == itemMap.end())
		{
			throw new DialogLayoutExceptions::ItemNotFound(item.offsetFrom);
		}
		DLItem& baseItem = (*itr).second;
		if (ItemState_Computed != baseItem.state)
		{
			try
			{
				computeOneItem(baseItem);
			}
			catch (DialogLayoutException* ex)
			{
				throw new DialogLayoutExceptions::FailedToCompute(ex, item.offsetFrom);
			}
		}
		item.computedValue = baseItem.computedValue + item.value;
	}
	
	item.state = ItemState_Computed;
}

/**
 *	@brief	Get computed value.
 *	@param[in]	name	name of the item.
 *	@return	computed value is retuend.
 *	@throw DialogLayoutExceptions::ItemNotFound	when specified item is not found.
 *	@throw DialogLayoutExceptions::ItemNotComputed	when specifid item value is not computed yet.
 */
DLValue DialogLayout::GetComputedValue(const MBCString& name) const
{
		DLItemMap::const_iterator itr = itemMap.find(name);
		if (itr == itemMap.end())
		{
			throw new DialogLayoutExceptions::ItemNotFound(name);
		}
		const DLItem& item = (*itr).second;
		if (ItemState_Computed != item.state)
		{
			throw new DialogLayoutExceptions::ItemNotComputed(name);
		}
		return item.computedValue;
}
