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
	@file		BeTextViewAdapter.cpp
	@brief		Implementation of BeTextViewAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.12.31 created
*/

#include "Prefix.h"
#include "BeTextViewAdapter.h"
#include <TextView.h>

/**
 *	@brief	Constructor
 */
BeTextViewAdapter::BeTextViewAdapter()
{
}

/**
 *	@brief	Destructor.
 */
BeTextViewAdapter::~BeTextViewAdapter()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	view	target view.
 */
void BeTextViewAdapter::Init(BTextView* view)
{
	BeViewAdapter::Init(view);
}

/**
 *	@brief	gets target BTextControl object.
 */
BTextView* BeTextViewAdapter::getTextView()
{
	return static_cast<BTextView*>(view);
}

/**
 *	@brief	Returns whether the control is enabled.
 *	@return true if enabled, otherwise disabled.
 */
bool BeTextViewAdapter::IsEnabled()
{
	return true;
}

/**
 *	@brief	Makes the control enabled or disabled.
 *	@param[in]	doEnable	true to enable the control, or false to disable the control.
 */
void BeTextViewAdapter::Enable(bool doEnable)
{
	// do nothing.
	// BTextView cannot be disabled.
}

/**
 *	@brief	Returns whether text control is editable.
 */
bool BeTextViewAdapter::IsEditable()
{
	return getTextView()->IsEditable();
}

/**
 *	@brief	Sets text control editable or uneditable.
 */
void BeTextViewAdapter::MakeEditable(bool isEditable)
{
	getTextView()->MakeEditable(isEditable);
}

/**
 *	@brief	Sets a string to the control.
 *	@param[in]	text	a string to be used as the new value of TextEdit.
 */
void BeTextViewAdapter::SetText(ConstAStr text)
{
	getTextView()->SetText(text);
}

/**
 *	@brief	Gets a string from the control.
 *	@param[out]	text	a string value is returned.
 */
void BeTextViewAdapter::GetText(MBCString& text)
{
	text = getTextView()->Text();
}

/**
 *	@brief	Stops or restarts modification notification.
 *	@param[in]	doStop	true to stop, false to restart.
 */
void BeTextViewAdapter::StopModificationNotification(bool doStop)
{
	// because the modification notification is not available on BTextView,
	// simply ignores this method.
}
