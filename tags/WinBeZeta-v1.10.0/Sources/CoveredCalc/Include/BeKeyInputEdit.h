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
	@file		BeKeyInputEdit.h
	@brief		Definition of BeKeyInputEdit class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.06.14 created
*/

#ifndef _BEKEYINPUTEDIT_H_
#define _BEKEYINPUTEDIT_H_

#include <TextControl.h>
#include "BeKeyEventFilter.h"
#include "KeyEventParameter.h"

class KeyNameDB;

/**
 *	@brief	Key-input control for BeOS.
 */
class BeKeyInputEdit : public BTextControl, public BeKeyEventHandler
{
public:
								BeKeyInputEdit(BRect frame, const char* name, const char* label,
									const char* text, BMessage* message,
									uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, uint32 flags = B_WILL_DRAW | B_NAVIGABLE);
	virtual						~BeKeyInputEdit();
	
	void						Init(const KeyNameDB* keyNameDB);

	const KeyEventParameter&	GetKeyEventParameter() const
									{ return value; }
	void						SetKeyEventParameter(const KeyEventParameter& param);

	virtual void				SetValueChangeMessage(BMessage* message)
									{ this->valueChangeMessage = message; }
	BMessage*					ValueChangeMessage() const
									{ return this->valueChangeMessage; }

	// implementation of BeKeyEventHandler
	virtual bool				HandleKeyEvent(const KeyEventParameter* parameter);
	
	// override BTextControl's function
	virtual void				AttachedToWindow();
	
private:
	const KeyNameDB*			keyNameDB;			///< key-name database.
	KeyEventParameter			value;				///< control value.
	BMessage*					valueChangeMessage;	///< value change message.
};

#endif // _BEKEYINPUTEDIT_H_
