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
	@file		BeSeparatorMenuFieldAdapter.h
	@brief		Definition of BeSeparatorMenuFieldAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.03.07 created
*/

#ifndef _BESEPARATORMENUFIELDADAPTER_H_
#define _BESEPARATORMENUFIELDADAPTER_H_

#include "BeMenuFieldAdapter.h"
#include <vector>

/**
 *	@brief	Adapter class of ListBox for BeOS.
 */
class BeSeparatorMenuFieldAdapter : public BeMenuFieldAdapter
{
public:
						BeSeparatorMenuFieldAdapter();
	virtual				~BeSeparatorMenuFieldAdapter();
	
	virtual void		Init(BMenuField* menuField, uint32 selectedCommand);
	
	virtual SInt32		GetCount();
	virtual SInt32		AddItem(ConstAStr text, void* data);
	virtual void		RemoveItem(SInt32 index);
	virtual void		RemoveAllItem();

	void				SetSeparator(SInt32 index, bool isSet);
	bool				HasSeparator(SInt32 index);

	virtual void		GetItemText(SInt32 index, MBCString& text);
	virtual void*		GetItemData(SInt32 index);
	virtual void		SetItemText(SInt32 index, ConstAStr text);
	virtual void		SetItemData(SInt32 index, void* data);
	
	virtual SInt32		GetSelectedItem();
	virtual void		SetSelectedItem(SInt32 index);

private:
	struct MenuItem
	{
		SInt32	rawIndex;		///< actual menu item index
		bool	hasSeparator;	///< true if this item has separator
	};
	typedef std::vector<MenuItem> MenuItemVector;
	MenuItemVector		menuItemVector;	///< menu items infos
};

#endif // _BESEPARATORMENUFIELDADAPTER_H_
