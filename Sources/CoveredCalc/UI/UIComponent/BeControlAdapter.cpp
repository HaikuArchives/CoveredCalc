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
	@file		BeControlAdapter.cpp
	@brief		Implementation of BeControlAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.19 created
*/

#include "Prefix.h"
#include "BeControlAdapter.h"
#include <Control.h>

/**
 *	@brief	Constructor
 */
BeControlAdapter::BeControlAdapter()
{
}

/**
 *	@brief	Destructor.
 */
BeControlAdapter::~BeControlAdapter()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	view	target view.
 */
void BeControlAdapter::Init(BControl* view)
{
	this->view = view;
}

/**
 *	@brief	gets target BControl object.
 */
BControl* BeControlAdapter::getControl()
{
	return static_cast<BControl*>(view);
}

/**
 *	@brief	Returns whether the control is enabled.
 *	@return true if enabled, otherwise disabled.
 */
bool BeControlAdapter::IsEnabled()
{
	return getControl()->IsEnabled();
}

/**
 *	@brief	Makes the control enabled or disabled.
 *	@param[in]	doEnable	true to enable the control, or false to disable the control.
 */
void BeControlAdapter::Enable(bool doEnable)
{
	getControl()->SetEnabled(doEnable);
}
