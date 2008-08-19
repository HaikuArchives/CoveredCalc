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
	@file		BeKeyEventFilter.h
	@brief		Definition of BeKeyEventFilter class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.06.14 created
*/

#ifndef _BEKEYEVENTFILTER_H_
#define _BEKEYEVENTFILTER_H_

#include <MessageFilter.h>

class KeyEventParameter;

/**
 *	@brief	 Interface of handling key events.
 */
class BeKeyEventHandler
{
public:
							BeKeyEventHandler() { }
	virtual					~BeKeyEventHandler() { }
	
	/**
	 *	@brief	Handles key event.
	 *	@param[in]	parameter	key event parameter.
	 *	@retval	true	dispatch this message.
	 *	@retval	false	don't dispatch this message.
	 */
	virtual bool			HandleKeyEvent(const KeyEventParameter* parameter) = 0;
};

/**
 *	@brief	Key event message filter.
 */
class BeKeyEventFilter : public BMessageFilter
{
public:
							BeKeyEventFilter();
	virtual					~BeKeyEventFilter();
	
	void					SetKeyEventHandler(BeKeyEventHandler* handler)
								{ this->handler = handler; }

	virtual	filter_result	Filter(BMessage* message, BHandler** target);
	
private:
	BeKeyEventHandler*		handler;
};

#endif // _BEKEYEVENTFILTER_H_
