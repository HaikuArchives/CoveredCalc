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
	@file		WinSeparatorDDListBoxAdapter.cpp
	@brief		Implementation of WinSeparatorDDListBoxAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.01.08 created
*/

#include "Prefix.h"
#include "WinSeparatorDDListBoxAdapter.h"
#include "WinComboListSeparators.h"

#define	base	WinDropDownListBoxAdapter

/**
 *	@brief	Constructor
 */
WinSeparatorDDListBoxAdapter::WinSeparatorDDListBoxAdapter()
{
	separators = NULL;
}

/**
 *	@brief	Destructor.
 */
WinSeparatorDDListBoxAdapter::~WinSeparatorDDListBoxAdapter()
{
}

/**
 *	@brief	Initialzes the object.
 *	@param[in]	hControl	handle to target control.
 *	@param[in]	separators	separator info object.
 */
void WinSeparatorDDListBoxAdapter::Init(HWND hControl, WinComboListSeparators* separators)
{
	base::Init(hControl);
	this->separators = separators;
}

/**
 *	@brief	Removes one item from the control.
 *	@param[in]	index	item index.
 */
void WinSeparatorDDListBoxAdapter::RemoveItem(SInt32 index)
{
	base::RemoveItem(index);
	separators->RemoveSeparatorAt(index);
	separators->ShiftSeparatorIndex(index, -1);
}

/**
 *	@brief	Removes all item from the control.
 */
void WinSeparatorDDListBoxAdapter::RemoveAllItem()
{
	base::RemoveAllItem();
	separators->ClearAllSeparators();
}

/**
 *	@brief	Set or reset a separator at specified index.
 *	@param[in]	index	separator index. (0 is the bottom of first item, 1 is the bottom of second, ...)
 *	@param[in]	isSet	true if set separator, or reset (remove) it.
 */
void WinSeparatorDDListBoxAdapter::SetSeparator(SInt32 index, bool isSet)
{
	if (isSet)
	{
		separators->AddSeparatorAt(index);
	}
	else
	{
		separators->RemoveSeparatorAt(index);
	}
}

/**
 *	@brief	Returns true if separator exists at specified index.
 *	@param[in]	index	index.
 *	@return	true if exists.
 */
bool WinSeparatorDDListBoxAdapter::HasSeparator(SInt32 index)
{
	return separators->HasSeparatorAt(index);
}
