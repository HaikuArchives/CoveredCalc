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
	@file		BeKeyInputEdit.cpp
	@brief		Implementation of BeKeyInputEdit class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.06.15 created
*/

#include "Prefix.h"
#include "BeKeyInputEdit.h"
#include "MBCString.h"
#include "KeyNameDB.h"

/**
 *	@brief	Constructor.
 *	@note see BTextControl's constructor about parameters.
 */
BeKeyInputEdit::BeKeyInputEdit(
	BRect frame,
	const char* name,
	const char* label,
	const char* text,
	BMessage* message,
	uint32 resizingMode /* = B_FOLLOW_LEFT | B_FOLLOW_TOP */,
	uint32 flags /* = B_WILL_DRAW | B_NAVIGABLE */)
	: BTextControl(frame, name, label, text, message, resizingMode, flags)
{
	keyNameDB = NULL;
	valueChangeMessage = NULL;
}

/**
 *	@brief	Destructor.
 */
BeKeyInputEdit::~BeKeyInputEdit()
{
	if (NULL != valueChangeMessage)
	{
		delete valueChangeMessage;
	}
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	keyNameDB	key-name database
 */
void BeKeyInputEdit::Init(const KeyNameDB* keyNameDB)
{
	this->keyNameDB = keyNameDB;
}

/**
 *	@brief	Sets current value.
 *	@param[in]	param	a value to set to this control.
 */
void BeKeyInputEdit::SetKeyEventParameter(const KeyEventParameter& param)
{
	this->value = param;
	
	// create text of the value.
	MBCString valueText;
	keyNameDB->GetKeyName(&param, valueText);
	
	SetText(valueText);
	
	if (NULL != valueChangeMessage)
	{
		Invoke(valueChangeMessage);
	}
}

/**
 *	@brief	Handles key event.
 *	@param[in]	parameter	key event parameter.
 *	@retval	true	dispatch this message.
 *	@retval	false	don't dispatch this message.
 */
bool BeKeyInputEdit::HandleKeyEvent(const KeyEventParameter* parameter)
{
	SetKeyEventParameter(*parameter);
	return false;
}

/**
 *	@brief	Called when assigned to a window.
 */
void BeKeyInputEdit::AttachedToWindow()
{
	BTextControl::AttachedToWindow();

	BTextView* textView = TextView();
	textView->MakeEditable(false);

	BeKeyEventFilter* filter = new BeKeyEventFilter();
	filter->SetKeyEventHandler(this);
	textView->AddFilter(filter);
}
