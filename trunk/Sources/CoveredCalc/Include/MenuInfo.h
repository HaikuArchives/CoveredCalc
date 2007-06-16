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
	@file		MenuInfo.h
	@brief		Definition of MenuInfo interface
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.03.17 created
*/

#ifndef _MENUINFO_H_
#define _MENUINFO_H_

#include "MenuDef.h"

// ---------------------------------------------------------------------
//! The interface which has information about one menu.
// ---------------------------------------------------------------------
class MenuInfo
{
public:
	enum MenuItemStates
	{
		MenuItemState_Normal = 0x00,		//!< item is not in state which mentions specially
		MenuItemState_Disabled = 0x01,		//!< item is disabled
		MenuItemState_Checked = 0x02,		//!< item is checked
		MenuItemState_Radio = 0x04,			//!< item is radio-button behavior
	};

public:
						MenuInfo() { }
	virtual				~MenuInfo() { }
	
	virtual Point32		GetMenuPosition() = 0;		// in screen coordinates
	virtual SInt32		GetMenuID() = 0;
	virtual UInt32		GetMenuItemStates(SInt32 menuItemID) = 0;
};

#endif // _MENUINFO_H_
