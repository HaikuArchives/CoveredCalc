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
	@file		UIController.h
	@brief		Definition of UIController class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.28 created
*/

#ifndef _UICONTROLLER_H_
#define _UICONTROLLER_H_

class ColorCodedSkin;
class ColorCodedSkinAppearance;

// ---------------------------------------------------------------------
//! The interface which controls UI
// ---------------------------------------------------------------------
class UIController
{
public:
	enum TimerID
	{
		TimerID_MouseHover = 0,

		TimerID_Max
	};
public:
						UIController() { }
	virtual				~UIController() { }

	virtual ColorCodedSkinAppearance*	InitSkinAppearance() = 0;
	virtual void		DisposeSkinAppearance(ColorCodedSkinAppearance* appearance) = 0;

	virtual Point32		GetMousePosition() const = 0;
	virtual Point32		GetMouseScreenPosition() const = 0;
	virtual void		GetUIRect(Rect32& rect) const = 0;
	virtual void		MoveUI(const Point32& topLeft) = 0;
	virtual void		BeginMouseCapture() = 0;
	virtual void		EndMouseCapture() = 0;
	virtual void		CloseUI() = 0;
	virtual bool		IsUIActive() const = 0;
	virtual void		Show() = 0;
	virtual void		Hide() = 0;
	virtual void		Minimize() = 0;
	virtual bool		IsUIMinimized() const = 0;
	virtual void		Restore() = 0;
	virtual void		UpdateUI() = 0;
	virtual void		Wait(UInt32 milliseconds) = 0;
	virtual void		SetToolTipText(ConstAStr text) = 0;
	virtual void		SetAlwaysOnTopFlag(bool isFlagOn) = 0;
	virtual void		BeginTimer(UIController::TimerID timerId, UInt32 elapse) = 0;
	virtual void		EndTimer(UIController::TimerID timerId) = 0;
};

#endif // _UICONTROLLER_H_
