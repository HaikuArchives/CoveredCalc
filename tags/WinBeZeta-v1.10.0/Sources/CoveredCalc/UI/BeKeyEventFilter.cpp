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
	@file		BeKeyEventFilter.cpp
	@brief		Definition of BeKeyEventFilter class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.06.14 created
*/

#include "Prefix.h"
#include "BeKeyEventFilter.h"
#include <Message.h>
#include "KeyEventParameter.h"

/**
 *	@brief	Constructor.
 */
BeKeyEventFilter::BeKeyEventFilter()
	:	BMessageFilter(B_KEY_DOWN)
{
	handler = NULL;
}

/**
 *	@brief	Destructor.
 */
BeKeyEventFilter::~BeKeyEventFilter()
{
}

/**
 *	@brief	Examines an arriving message just before it's dispatched.
 */
filter_result BeKeyEventFilter::Filter(BMessage* message, BHandler** /* target */)
{
	if (NULL != handler)
	{
		int32 keyCode = 0;
		int32 modifiers = 0;
		message->FindInt32("key", &keyCode);
		message->FindInt32("modifiers", &modifiers);
		
		KeyEventParameter parameter;
		parameter.SetKeyCode(keyCode);
		parameter.SetModifiers(modifiers);
		if (!handler->HandleKeyEvent(&parameter))
		{
			return B_SKIP_MESSAGE;
		}
	}
	return B_DISPATCH_MESSAGE;
}
