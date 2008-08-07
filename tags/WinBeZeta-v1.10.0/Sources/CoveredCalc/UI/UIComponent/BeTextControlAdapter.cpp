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
	@file		BeTextControlAdapter.cpp
	@brief		Implementation of BeTextControlAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.19 created
*/

#include "Prefix.h"
#include "BeTextControlAdapter.h"
#include <TextControl.h>
#include "CommandID.h"

/**
 *	@brief	Constructor
 */
BeTextControlAdapter::BeTextControlAdapter()
{
	stoppedModificationCommand = ID_NULL;
	stoppedModificationNotificationCount = 0;
}

/**
 *	@brief	Destructor.
 */
BeTextControlAdapter::~BeTextControlAdapter()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	view	target view.
 */
void BeTextControlAdapter::Init(BTextControl* view)
{
	BeControlAdapter::Init(view);
}

/**
 *	@brief	gets target BTextControl object.
 */
BTextControl* BeTextControlAdapter::getTextControl()
{
	return static_cast<BTextControl*>(view);
}

/**
 *	@brief	Returns whether text control is editable.
 */
bool BeTextControlAdapter::IsEditable()
{
	return getTextControl()->TextView()->IsEditable();
}

/**
 *	@brief	Sets text control editable or uneditable.
 */
void BeTextControlAdapter::MakeEditable(bool isEditable)
{
	getTextControl()->TextView()->MakeEditable(isEditable);
}

/**
 *	@brief	Sets a string to the control.
 *	@param[in]	text	a string to be used as the new value of TextEdit.
 */
void BeTextControlAdapter::SetText(ConstAStr text)
{
	getTextControl()->SetText(text);
}

/**
 *	@brief	Gets a string from the control.
 *	@param[out]	text	a string value is returned.
 */
void BeTextControlAdapter::GetText(MBCString& text)
{
	text = getTextControl()->Text();
}

/**
 *	@brief	Stops or restarts modification notification.
 *	@param[in]	doStop	true to stop, false to restart.
 */
void BeTextControlAdapter::StopModificationNotification(bool doStop)
{
	BTextControl* textControl = getTextControl();
	if (doStop)
	{
		if (0 == stoppedModificationNotificationCount)
		{
			BMessage* message = textControl->ModificationMessage();
			stoppedModificationCommand = message->what;
			message->what = ID_NULL;
		}
		stoppedModificationNotificationCount++;
	}
	else
	{
		stoppedModificationNotificationCount--;
		if (0 == stoppedModificationNotificationCount)
		{
			BMessage* message = textControl->ModificationMessage();
			message->what = stoppedModificationCommand;
			stoppedModificationCommand = ID_NULL;
		}
	}
}
