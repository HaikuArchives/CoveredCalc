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
	@file		WinSeparatorDDListBoxAdapter.h
	@brief		Definition of WinSeparatorDDListBoxAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.01.08 created
*/

#ifndef _WINSEPARATORDDLISTBOXADAPTER_H_
#define _WINSEPARATORDDLISTBOXADAPTER_H_

#include "WinDropDownListBoxAdapter.h"
#include "MBCString.h"

class WinComboListSeparators;

/**
 *	@brief	Adapter class of drop-down list box (one form of the combo box) which supports separators for Windows.
 */
class WinSeparatorDDListBoxAdapter : public WinDropDownListBoxAdapter
{
public:
							WinSeparatorDDListBoxAdapter();
	virtual					~WinSeparatorDDListBoxAdapter();
	
	void					Init(HWND hControl, WinComboListSeparators* separators);
	
	virtual void			RemoveItem(SInt32 index);
	virtual void			RemoveAllItem();
	
	void					SetSeparator(SInt32 index, bool isSet);
	bool					HasSeparator(SInt32 index);
	
private:
	WinComboListSeparators* separators;	///< separator info object.
};

#endif // _WINSEPARATORDDLISTBOXADAPTER_H_
