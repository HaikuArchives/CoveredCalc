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
	@file		MouseHoverUITask.h
	@brief		Definition of MouseHoverUITask
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.11.24 created
*/

#ifndef _MOUSEHOVERUITASK_H_
#define _MOUSEHOVERUITASK_H_

#include "UITask.h"

/**
 *	@brief
 */
class MouseHoverUITask : public UITaskEmptyImpl
{
public:
									MouseHoverUITask();
	virtual							~MouseHoverUITask();

	void							SetImageIndex(SInt32 normalActiveImageIndex, SInt32 normalInactiveImageIndex, SInt32 hotActiveImageIndex, SInt32 hotInactiveImageIndex);
	void							SetInitialInformation(SInt32 commandId, SInt32 stepCount);

	void							CancelTask();
	SInt32							GetTargetArea() const	{ return targetSkinArea; }

	virtual SInt32					GetClass();

	virtual void					StartTask();
	virtual void					MouseMove();
	virtual void					UIActivated();
	virtual	void					TimerEvent(SInt32 timerId);

private:
	bool							changeStatusAccordingToMouse();
	void							updateAppearance();

private:
	SInt32							normalActiveImageIndex;		///< skin index of normal active image
	SInt32							normalInactiveImageIndex;	///< skin index of normal inactive image
	SInt32							hotActiveImageIndex;		///< skin index of hot active image
	SInt32							hotInactiveImageIndex;		///< skin index of hot inactive image
	SInt32							targetSkinArea;				///< target skin area id
	SInt32							targetCommandID;			///< target command id
	SInt32							stepCount;					///< how many steps to become complete hot image
	SInt32							currentStep;				///< current steps
	bool							isBeingHot;					///< true if begin hot
};

#endif // _MOUSEHOVERUITASK_H_
