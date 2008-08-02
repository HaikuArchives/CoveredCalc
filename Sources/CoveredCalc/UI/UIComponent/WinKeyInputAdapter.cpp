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
	@file		WinKeyInputAdapter.cpp
	@brief		Implementation of WinKeyInputAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.25 created
*/

#include "Prefix.h"
#include "WinKeyInputAdapter.h"
#include "WinKeyInputEdit.h"

/**
 *	@brief	Constructor
 */
WinKeyInputAdapter::WinKeyInputAdapter()
{
	keyInputEdit = NULL;
	stoppedValueChangedNotificationCount = 0;
}

/**
 *	@brief	Destructor.
 */
WinKeyInputAdapter::~WinKeyInputAdapter()
{
}

/**
 *	@brief	Initialzes the object.
 *	@param[in]	keyInputEdit	target KeyInput control.
 */
void WinKeyInputAdapter::Init(WinKeyInputEdit* keyInputEdit)
{
	WinControlAdapter::Init(keyInputEdit->m_hWnd);
	this->keyInputEdit = keyInputEdit;
}

/**
 *	@brief	Sets a keyboard parameter to the control.
 *	@param[in]	param	a keyboard parameter to be used as the new value of KeyInput.
 */
void WinKeyInputAdapter::SetKeyEventParameter(const KeyEventParameter& param)
{
	keyInputEdit->SetKeyEventParameter(param);
}

/**
 *	@brief	Gets a keyboard parameter from the control.
 *	@return	a keyboard parameter value.
 */
const KeyEventParameter& WinKeyInputAdapter::GetKeyEventParameter()
{
	return keyInputEdit->GetKeyEventParameter();
}

/**
 *	@brief	Stops or restarts value changed notification.
 *	@param[in]	doStop	true to stop, false to restart.
 */
void WinKeyInputAdapter::StopValueChangedNotification(bool doStop)
{
	if (doStop)
	{
		stoppedValueChangedNotificationCount++;
	}
	else
	{
		stoppedValueChangedNotificationCount--;
	}
}
