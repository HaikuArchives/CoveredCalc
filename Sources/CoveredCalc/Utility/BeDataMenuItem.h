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
	@file		BeDataMenuItem.h
	@brief		Definition of BeDataMenuItem class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.11.17 created
*/

#ifndef _BEDATAMENUITEM_H_
#define _BEDATAMENUITEM_H_

#include <MenuItem.h>

/**
 * @brief	BMenuItem with an item data.
 * @note	The type DATATYPE must be a pointer type.
 */
template<class DATATYPE>
class BeDataMenuItem : public BMenuItem
{
public:
						BeDataMenuItem(const char* label, BMessage* message, char shortcut = 0, uint32 modifiers = 0)
							:	BMenuItem(label, message, shortcut, modifiers), data(NULL) {}

	void				SetItemData(DATATYPE data) { this->data = data; }
	DATATYPE			GetItemData() const { return this->data; }

private:
	DATATYPE			data;		///< item data
};

#endif // _BEDATAMENUITEM_H_
