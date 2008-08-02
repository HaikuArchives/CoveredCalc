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
	@file		WinMessageFilter.h
	@brief		Definition of WinMessageFilter interface.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.7.10 created
*/

#ifndef _WINMESSAGEFILTER_H_
#define _WINMESSAGEFILTER_H_

/**
 *	@brief	An interface for filters which processes windows message before dispatching.
 */
class WinMessageFilter
{
public:
						WinMessageFilter() {}
						~WinMessageFilter() {}

	/**
	 *	@brief	Filters message.
	 *	@param[in,out] msg	message.
	 *	@return false to skip this message. true to dispatch this message.
	 */
	virtual bool		FilterMessage(MSG* msg) = 0;
};

#endif // _WINMESSAGEFILTER_H_
