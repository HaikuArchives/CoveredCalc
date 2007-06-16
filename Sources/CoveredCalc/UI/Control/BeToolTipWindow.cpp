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
	@file		BeToolTipWindow.cpp
	@brief		Implementation of BeToolTipWindow class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.10.05 created
*/

#include "Prefix.h"
#include "BeToolTipWindow.h"
#include <AppKit.h>
#include <InterfaceKit.h>
#include <support/Autolock.h>
#include <support/String.h>

#define TOOLTIP_BGCOLOR_R		255
#define TOOLTIP_BGCOLOR_G		255
#define TOOLTIP_BGCOLOR_B		225

#define TOOLTIP_TEXTCOLOR_R		0
#define TOOLTIP_TEXTCOLOR_G		0
#define TOOLTIP_TEXTCOLOR_B		0

#define TOOLTIP_BORDERCOLOR_R	0
#define TOOLTIP_BORDERCOLOR_G	0
#define TOOLTIP_BORDERCOLOR_B	0


#define TOOLTIP_H_MARGIN		2
#define TOOLTIP_V_MARGIN		1

#define TOOLTIP_OFFSET_X		16
#define TOOLTIP_OFFSET_Y		16

#define MSG_TIMER				'timr'
#define TIMER_ELAPSE			500000
#define SHOW_COUNTER			2
#define HIDE_COUNTER			10

const uint32 BeToolTipWindow::MSG_ACTIVATE = 'actv';
const uint32 BeToolTipWindow::MSG_DEACTIVATE = 'dact';
const uint32 BeToolTipWindow::MSG_SETTEXT = 'sttx';
const char* BeToolTipWindow::PARAM_TEXT = "text";

/**
 *	@brief	Constructor
 */
BeToolTipWindow::BeToolTipWindow()
	:	BWindow(BRect(-1, -1, -1, -1), "ToolTip",
				B_NO_BORDER_WINDOW_LOOK, B_FLOATING_ALL_WINDOW_FEEL, B_AVOID_FOCUS, B_ALL_WORKSPACES)
{
	targetView = NULL;
	timerRunner = NULL;
	textView = NULL;
	timerCounter = 0;
	isActive = false;
	isDone = false;
}

/**
 *	@brief	Destructor
 */
BeToolTipWindow::~BeToolTipWindow()
{
	delete timerRunner;
}

/**
 *	@brief	Initializes the object.
 */
void BeToolTipWindow::Init(
	BView* targetView,		///< tool tip target
	const char* text		///< tool tip text
)
{
	this->targetView = targetView;

	// create timer
	timerRunner = new BMessageRunner(BMessenger(this), new BMessage(MSG_TIMER), TIMER_ELAPSE);

	// create baseView
	BView* baseView = new BeToolTipBaseView(Bounds(), "baseView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS);
	baseView->SetViewColor(TOOLTIP_BGCOLOR_R, TOOLTIP_BGCOLOR_G, TOOLTIP_BGCOLOR_B);
	AddChild(baseView);
	
	// create a text part
	BRect textFrameRect = baseView->Frame();
	textFrameRect.OffsetTo(0, 0);
	textFrameRect.InsetBy(TOOLTIP_H_MARGIN, TOOLTIP_V_MARGIN);
	BRect textRect = textFrameRect;
	textRect.OffsetTo(0, 0);
	textView = new BTextView(textFrameRect, "textView", textRect,
			B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS);
	textView->SetWordWrap(false);
	rgb_color textColor = { TOOLTIP_TEXTCOLOR_R, TOOLTIP_TEXTCOLOR_G, TOOLTIP_TEXTCOLOR_B, 0xff };
	textView->SetViewColor(TOOLTIP_BGCOLOR_R, TOOLTIP_BGCOLOR_G, TOOLTIP_BGCOLOR_B);
	textView->SetFontAndColor(be_plain_font, B_FONT_ALL, &textColor);
	baseView->AddChild(textView);
	
	// set Text
	setText(text);
	
	// begin message loop in hidden state.
	Hide();
	Show();
}

/**
 *	@brief	Sets tool tip text and resize window
 */
void BeToolTipWindow::setText(
	const char* text		///< tool tip text
)
{
	if (NULL == textView)
		return;
	
	// set text
	textView->SetText(text, strlen(text));

	// resize window
	float width = 0.0;
	int32 line;
	int32 lineCount = textView->CountLines();
	for (line=0; line<lineCount; line++)
	{
		float lineWidth = textView->LineWidth(line);
		if (lineWidth > width)
		{
			width = lineWidth;
		}
	}
	float height = textView->TextHeight(0, lineCount - 1);
	ResizeTo(width + TOOLTIP_H_MARGIN * 2, height + TOOLTIP_V_MARGIN * 2);
	textView->SetTextRect(BRect(0, 0, width, height));
}

/**
 *	@brief	Shows tool tip.
 */
void BeToolTipWindow::showToolTip()
{
	if (!isActive)
		return;

	BPoint cursor;
	uint32 buttons;
	textView->GetMouse(&cursor, &buttons, false);
	textView->ConvertToScreen(&cursor);
	MoveTo(cursor.x + TOOLTIP_OFFSET_X, cursor.y + TOOLTIP_OFFSET_Y);
	if (IsHidden())
	{
		Show();
		timerCounter = 0;
	}
}

/**
 *	@brief	Hides tool tip.
 */
void BeToolTipWindow::hideToolTip()
{
	if (!IsHidden())
	{
		Hide();
		timerCounter = 0;
	}
}

/**
 *	@brief	Called when timer event has occured.
 */
void BeToolTipWindow::timerEvent()
{
	if (!isActive)
		return;
		
	BPoint cursor;
	uint32 buttons;
	textView->GetMouse(&cursor, &buttons, false);
	textView->ConvertToScreen(&cursor);

	bool isActive = false;
	BRect targetRect(0, 0, 0, 0);
	BRegion targetClipRegion;
	{
		BAutolock locker(targetView->Window());
		if (locker.IsLocked())
		{
			isActive = targetView->Window()->IsActive();
			if (isActive)
			{
				targetRect = targetView->ConvertToScreen(targetView->Bounds());
				targetView->GetClippingRegion(&targetClipRegion);
			}
		}
	}
	if (!isActive || !targetRect.Contains(cursor))
	{
		isDone = false;
		timerCounter = 0;
		if (!IsHidden())
		{
			hideToolTip();
		}
	}
	else
	{
		if (!isDone)
		{
			timerCounter++;
			if (IsHidden())
			{
				if (timerCounter >= SHOW_COUNTER)
				{
					showToolTip();
				}
			}
			else
			{
				if (timerCounter >= HIDE_COUNTER)
				{
					hideToolTip();
					isDone = true;
				}
			}
		}	
	}
}

/**
 *	@brief	Activates this tool tip.
 */
void BeToolTipWindow::activate()
{
	isActive = true;
	timerCounter = 0;
	isDone = false;
}

/**
 *	@brief	Deactivates this tool tip.
 */
void BeToolTipWindow::deactivate()
{
	isActive = false;
	hideToolTip();
}

/*
 *	@brief	Message handler
 */
void BeToolTipWindow::MessageReceived(
	BMessage* message			///< received message
)
{
	switch (message->what)
	{
	case MSG_TIMER:
		{
			BAutolock locker(this);
			timerEvent();
		}
		break;
	
	case MSG_ACTIVATE:
		{
			BAutolock locker(this);
			activate();
		}
		break;
	
	case MSG_DEACTIVATE:
		{
			BAutolock locker(this);
			deactivate();
		}
		break;

	case MSG_SETTEXT:
		{
			BAutolock locker(this);
			BString newText;
			if (B_OK == message->FindString(PARAM_TEXT, &newText))
			{
				setText(newText.String());
			}
		}

	default:
		BWindow::MessageReceived(message);
		break;
	}
}


/////////////////////////////////////////////////// BeToolTipBaseView

/**
 *	@brief	Constructor
 */
BeToolTipBaseView::BeToolTipBaseView(
	BRect frame,
	const char* name,
	uint32 resizingMode,
	uint32 flags
)
	: BView(frame, name, resizingMode, flags)
{	
}

/*
 *	@brief	Draws the view area
 */
void BeToolTipBaseView::Draw(
	BRect updateRect
)
{
	BView::Draw(updateRect);
	
	SetPenSize(1);
	SetHighColor(TOOLTIP_BORDERCOLOR_R, TOOLTIP_BORDERCOLOR_G, TOOLTIP_BORDERCOLOR_B, 255);
	StrokeRect(Bounds(), B_SOLID_HIGH);
}
