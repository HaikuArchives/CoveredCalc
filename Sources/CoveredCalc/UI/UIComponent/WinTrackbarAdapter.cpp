/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2009 CoveredCalc Project Contributors
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
	@file		WinTrackbarAdapter.cpp
	@brief		Implementation of WinTrackbarAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.01.03 created
*/

#include "Prefix.h"
#include "WinTrackbarAdapter.h"

/**
 *	@brief	Constructor
 */
WinTrackbarAdapter::WinTrackbarAdapter()
{
}

/**
 *	@brief	Destructor.
 */
WinTrackbarAdapter::~WinTrackbarAdapter()
{
}

/**
 *	@brief	Sets maximum value of slider.
 *	@param[in]	value	maximum value.
 */
void WinTrackbarAdapter::SetMaxValue(SInt32 value)
{
	SendMessage(hControl, TBM_SETRANGEMAX, TRUE, value);
}

/**
 *	@brief	Returns maximum value of slider.
 *	@return maximum value.
 */
SInt32 WinTrackbarAdapter::GetMaxValue()
{
	return SendMessage(hControl, TBM_GETRANGEMAX, 0, 0);
}

/**
 *	@brief	Sets minimum value of slider.
 *	@param[in]	value	minimum value.
 */
void WinTrackbarAdapter::SetMinValue(SInt32 value)
{
	SendMessage(hControl, TBM_SETRANGEMIN, TRUE, value);
}

/**
 *	@brief	Returns minimum value of slider.
 *	@return	minimum value.
 */
SInt32 WinTrackbarAdapter::GetMinValue()
{
	return SendMessage(hControl, TBM_GETRANGEMIN, 0, 0);
}

void WinTrackbarAdapter::SetValue(SInt32 value)
{
	SendMessage(hControl, TBM_SETPOS, TRUE, value);
}

SInt32 WinTrackbarAdapter::GetValue()
{
	return SendMessage(hControl, TBM_GETPOS, 0, 0);
}

void WinTrackbarAdapter::SetTickMarkCount(SInt32 count)
{
	SendMessage(hControl, TBM_SETTICFREQ, (GetMaxValue() - GetMinValue()) / (count - 1), 0);
}
