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
	@file		WinControlAdapter.cpp
	@brief		Implementation of WinControlAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.19 created
*/

#include "Prefix.h"
#include "WinControlAdapter.h"

/**
 *	@brief	Constructor
 */
WinControlAdapter::WinControlAdapter()
{
}

/**
 *	@brief	Destructor.
 */
WinControlAdapter::~WinControlAdapter()
{
}

/**
 *	@brief	Initialzes the object.
 *	@param[in]	hControl	handle to target control.
 */
void WinControlAdapter::Init(HWND hControl)
{
	this->hControl = hControl;
}

/**
 *	@brief	Returns whether the control is enabled.
 *	@return true if enabled, otherwise disabled.
 */
bool WinControlAdapter::IsEnabled()
{
	return IsWindowEnabled(hControl) ? true : false;
}

/**
 *	@brief	Makes the control enabled or disabled.
 *	@param[in]	doEnable	true to enable the control, or false to disable the control.
 */
void WinControlAdapter::Enable(bool isEnabled)
{
	EnableWindow(hControl, (isEnabled) ? TRUE : FALSE);
}

/**
 *	@brief	Makes this control focused.
 */
void WinControlAdapter::MakeFocus()
{
	SetFocus(hControl);
}
