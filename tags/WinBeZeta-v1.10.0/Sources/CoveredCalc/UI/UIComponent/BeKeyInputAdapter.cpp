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
	@file		BeKeyInputAdapter.cpp
	@brief		Implementation of BeKeyInputAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.06.15 created
*/

#include "Prefix.h"
#include "BeKeyInputAdapter.h"
#include "BeKeyInputEdit.h"
#include "CommandID.h"

/**
 *	@brief	Constructor
 */
BeKeyInputAdapter::BeKeyInputAdapter()
{
	stoppedValueChangedCommand = ID_NULL;
	stoppedValueChangedNotificationCount = 0;
}

/**
 *	@brief	Destructor.
 */
BeKeyInputAdapter::~BeKeyInputAdapter()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	view	target view.
 */
void BeKeyInputAdapter::Init(BeKeyInputEdit* view)
{
	BeControlAdapter::Init(view);
}

/**
 *	@brief	gets target BTextControl object.
 */
BeKeyInputEdit* BeKeyInputAdapter::getKeyInputEdit()
{
	return static_cast<BeKeyInputEdit*>(view);
}

/**
 *	@brief	Sets a keyboard parameter to the control.
 *	@param[in]	param	a keyboard parameter to be used as the new value of KeyInput.
 */
void BeKeyInputAdapter::SetKeyEventParameter(const KeyEventParameter& param)
{
	getKeyInputEdit()->SetKeyEventParameter(param);
}

/**
 *	@brief	Gets a keyboard parameter from the control.
 *	@return	a keyboard parameter value.
 */
const KeyEventParameter& BeKeyInputAdapter::GetKeyEventParameter()
{
	return getKeyInputEdit()->GetKeyEventParameter();
}

/**
 *	@brief	Stops or restarts value changed notification.
 *	@param[in]	doStop	true to stop, false to restart.
 */
void BeKeyInputAdapter::StopValueChangedNotification(bool doStop)
{
	BeKeyInputEdit* keyInput = getKeyInputEdit();
	if (doStop)
	{
		if (0 == stoppedValueChangedNotificationCount)
		{
			BMessage* message = keyInput->ValueChangeMessage();
			stoppedValueChangedCommand = message->what;
			message->what = ID_NULL;
		}
		stoppedValueChangedNotificationCount++;
	}
	else
	{
		stoppedValueChangedNotificationCount--;
		if (0 == stoppedValueChangedNotificationCount)
		{
			BMessage* message = keyInput->ValueChangeMessage();
			message->what = stoppedValueChangedCommand;
			stoppedValueChangedCommand = ID_NULL;
		}
	}
}
