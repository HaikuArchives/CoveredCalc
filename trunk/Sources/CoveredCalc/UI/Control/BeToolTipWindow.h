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
	@file		BeToolTipWindow.h
	@brief		Definition of BeToolTipWindow class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.10.05 created
*/

#ifndef _BETOOLTIPWINDOW_H_
#define _BETOOLTIPWINDOW_H_

#include <interface/Window.h>
#include <interface/View.h>

class BMessageRunner;
class BTextView;

/**
 *	@brief	Tool Tip control for BeOS
 */
class BeToolTipWindow : public BWindow
{
public:
	static const uint32	MSG_ACTIVATE;		///< activate tool tip
	static const uint32 MSG_DEACTIVATE;		///< deactivate tool tip
	static const uint32 MSG_SETTEXT;		///< set tool tip text (the text is stored in string form with name PARAM_TEXT)
	static const char*	PARAM_TEXT;			///< text parameter. used with MSG_SETTEXT.
	
public:
						BeToolTipWindow();
						~BeToolTipWindow();

	void				Init(BView* targetView, const char* text);

protected:
	virtual	void		MessageReceived(BMessage* message);

private:
	void				setText(const char* text);
	void				showToolTip();
	void				hideToolTip();
	void				timerEvent();
	void				activate();
	void				deactivate();

private:
	BView*				targetView;			///< tool tip target
	BMessageRunner*		timerRunner;		///< timer
	BTextView*			textView;			///< view which has tool tip text
	int32				timerCounter;		///< counter for timer event
	bool				isActive;			///< whether this control is activated
	bool				isDone;				///< true after tool tip was shown and was automatically hidden
};

/*
 *	@brief	Tool tip control's base view.
 */
class BeToolTipBaseView : public BView
{
public:
						BeToolTipBaseView(BRect frame, const char* name, uint32 resizingMode, uint32 flags);
	
protected:
	virtual	void		Draw(BRect updateRect);
};

#endif // _BETOOLTIPWINDOW_H_
