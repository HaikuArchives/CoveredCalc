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
	@file		DialogLayout.h
	@brief		Definition of DialogLayout class.
	@author 	ICHIMIYA Hironori (Hiron)
	@date		2008.10.28 created
*/

#ifndef _DIALOGLAYOUT_H_
#define _DIALOGLAYOUT_H_

#include <map>
#include "MBCString.h"
#include "Exception.h"

typedef int		DLValue;

/**
 * @brief This class holds informations about dialog control layouts.
 */
class DialogLayout
{
public:
							DialogLayout();
	virtual					~DialogLayout();
	
	void					SetLayoutItem(const MBCString& name, const MBCString& offsetFrom, DLValue value);
	bool					GetLayoutItem(const MBCString& name, MBCString& outOffsetFrom, DLValue& outValue) const;
	void					ClearAllLayoutItems();
	
	void					Compute();
	
	DLValue					GetComputedValue(const MBCString& name) const;

private:
	enum ItemState
	{
		ItemState_Uncomputed = 0,
		ItemState_Computing,
		ItemState_Computed
	};

	struct DLItem
	{
		MBCString			offsetFrom;			///< if value is relative, name of the base value is specified.
		DLValue				value;				///< value
		DLValue				computedValue;		///< computed value
		ItemState			state;				///< item state
	};

	typedef std::map<MBCString, DLItem> DLItemMap;

private:
	void					computeOneItem(DLItem& item);	

private:
	DLItemMap				itemMap;
};

#endif // _DIALOGLAYOUT_H_
