/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2007 CoveredCalc Project Contributors
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
	@file		ButtonUITask.h
	@brief		Definition of ButtonUITask
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.29 created
*/

#ifndef _BUTTONUITASK_H_
#define _BUTTONUITASK_H_

#include "UITask.h"

// ---------------------------------------------------------------------
//! The event handler interface for ButtonUITask
// ---------------------------------------------------------------------
class ButtonUITaskEventHandler
{
public:
						ButtonUITaskEventHandler() { }
	virtual				~ButtonUITaskEventHandler() { }
	
	virtual void		ButtonClicked(SInt32 buttonCommand) = 0;
};

// ---------------------------------------------------------------------
//! The task which manages pressing button on UI
// ---------------------------------------------------------------------
class ButtonUITask : public UITaskEmptyImpl
{
public:
									ButtonUITask();
	virtual							~ButtonUITask();

	void							SetImageIndex(SInt32 normalActiveImageIndex, SInt32 normalInActiveImageIndex, SInt32 pressedImageIndex);
	void							SetEventHandler(ButtonUITaskEventHandler* eventHandler) { this->eventHandler = eventHandler; }
	void							SetInitialInformation(SInt32 targetCommand);

	SInt32							GetClass();

	virtual void					StartTask();

	virtual void					MouseMove();
	virtual void					Button1Up();
	virtual void					Button2Down();
	virtual void					Button2Up();
	virtual void					KeyDown(const KeyEventParameter& parameter);
	virtual void					KeyUp(const KeyEventParameter& parameter);
	virtual void					UIActivated();

private:
	void							setTargetCommand(SInt32 targetCommand);
	void							updateButtonAppearance(bool isPressed);
	void							cancelTask();
	
private:
	ButtonUITaskEventHandler*		eventHandler;
	SInt32							normalActiveImageIndex;
	SInt32							normalInactiveImageIndex;
	SInt32							pressedImageIndex;
	SInt32							targetSkinArea;
	SInt32							targetCommand;
	SInt32							currentImageIndex;
};

#endif // _BUTTONUITASK_H_
