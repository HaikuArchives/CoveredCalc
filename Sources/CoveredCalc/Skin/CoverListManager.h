/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2007 CoveredCalc Project Contributors
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
	@file		CoverListManager.h
	@brief		Definition of CoverListManager class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.03.07 created
*/

#ifndef _COVERLISTMANAGER_H_
#define _COVERLISTMANAGER_H_

#include <vector>
#include "Path.h"

class CoverListItem;

typedef std::vector<CoverListItem*>	CoverListVector;

// ---------------------------------------------------------------------
//! This class manages a list of available covers.
// ---------------------------------------------------------------------
class CoverListManager
{
public:
								CoverListManager();
	virtual						~CoverListManager();

	void						MakeList(const Path& coverPath);

	CoverListVector*			GetItems()				{ return &items; }
	const CoverListVector*		GetItems() const		{ return &items; }

private:
	void						removeAllItems();
	void						enumCoverDef(const Path& coverPath);
	void						loadCoverDef(SInt32 index);

private:
	CoverListVector				items;
};

#endif // _COVERLISTMANAGER_H_
