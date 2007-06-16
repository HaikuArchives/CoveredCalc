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
	@file		MovingUITask.h
	@brief		Definition of MovingUITask
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.29 created
*/

#ifndef _MOVINGUITASK_H_
#define _MOVINGUITASK_H_

#include "UITask.h"

// ---------------------------------------------------------------------
//! The task which moves UI.
// ---------------------------------------------------------------------
class MovingUITask : public UITaskEmptyImpl
{
public:
						MovingUITask();
	virtual				~MovingUITask();

	SInt32				GetClass();

	virtual void		StartTask();

	virtual void		MouseMove();
	virtual void		Button1Up();
	virtual void		Button2Down();
	virtual void		Button2Up();

private:
	void				endTask();

private:
	Point32				startPoint;			//!< starting point
	Point32				startTopLeft;		//!< UI's top left point when this task starts
};

#endif // _MOVINGUITASK_H_
