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
	@file		UITask.h
	@brief		Definition of UITask and UITaskEmptyImpl class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.28 created
*/

#ifndef _UITASK_H_
#define _UITASK_H_

#include "KeyEventParameter.h"

class UIManager;

// ---------------------------------------------------------------------
//! The task interface of UI operation
// ---------------------------------------------------------------------
class UITask
{
public:
						UITask() { }
	virtual				~UITask() { }

	virtual	SInt32		GetClass() = 0;

	virtual void		Init(UIManager* uiManager) = 0;

	virtual void		StartTask() = 0;

	virtual void		MouseMove() = 0;
	virtual void		Button1Down() = 0;
	virtual void		Button1Up() = 0;
	virtual void		Button2Down() = 0;
	virtual void		Button2Up() = 0;
	virtual void		UIActivated() = 0;
	virtual void		KeyDown(const KeyEventParameter& parameter) = 0;
	virtual void		KeyUp(const KeyEventParameter& parameter) = 0;
	virtual	void		TimerEvent(SInt32 timerId) = 0;
};

// ---------------------------------------------------------------------
//! Implementation of UITask which performs nothing.
/*!
	If this class is inherited,
	it is not necessary to handle an unnecessary event.
*/
// ---------------------------------------------------------------------
class UITaskEmptyImpl : public UITask
{
public:
						UITaskEmptyImpl() { uiManager = NULL; }
	virtual				~UITaskEmptyImpl() { }
	
	virtual void		Init(UIManager* uiManager) { this->uiManager = uiManager; }

	virtual void		StartTask() { }

	virtual void		MouseMove() { }
	virtual void		Button1Down() { }
	virtual void		Button1Up() { }
	virtual void		Button2Down() { }
	virtual void		Button2Up() { }
	virtual void		UIActivated() { }
	virtual void		KeyDown(const KeyEventParameter& /*parameter*/) { }
	virtual void		KeyUp(const KeyEventParameter& /*parameter*/) { }
	virtual	void		TimerEvent(SInt32 /*timerId*/) { }

protected:
	UIManager*			getUIManager() { return uiManager; }

private:
	UIManager*			uiManager;
};

#endif // _UITASK_H_
