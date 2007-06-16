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
	@file		MovingUITask.cpp
	@brief		Implementation of MovingUITask
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.29 created
*/

#include "Prefix.h"
#include <memory.h>
#include "MovingUITask.h"
#include "UIManager.h"
#include "UITaskClass.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
MovingUITask::MovingUITask()
{
	memset(&startPoint, 0, sizeof(startPoint));
	memset(&startTopLeft, 0, sizeof(startTopLeft));
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
MovingUITask::~MovingUITask()
{
}

/**
 *	@brief	Returns a kind of this class.
 */
SInt32 MovingUITask::GetClass()
{
	return UITaskClass::MovingUITask;
}

// ---------------------------------------------------------------------
//! Starts task
// ---------------------------------------------------------------------
void MovingUITask::StartTask()
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);

	Rect32 uiRect;
	startPoint = uiManager->GetUIController()->GetMouseScreenPosition();
	uiManager->GetUIController()->GetUIRect(uiRect);
	startTopLeft.x = uiRect.left;
	startTopLeft.y = uiRect.top;
	
	uiManager->GetUIController()->BeginMouseCapture();
}

// ---------------------------------------------------------------------
//! Called when mouse cursor moves
// ---------------------------------------------------------------------
void MovingUITask::MouseMove()
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);

	Point32 currentPoint = uiManager->GetUIController()->GetMouseScreenPosition();
	Point32 topLeft;
	topLeft.x = startTopLeft.x + currentPoint.x - startPoint.x;
	topLeft.y = startTopLeft.y + currentPoint.y - startPoint.y;
	uiManager->GetUIController()->MoveUI(topLeft);
}

// ---------------------------------------------------------------------
//! Called when mouse button 1 is released
// ---------------------------------------------------------------------
void MovingUITask::Button1Up()
{
	endTask();
}

// ---------------------------------------------------------------------
//! Called when mouse button 2 is pressed
// ---------------------------------------------------------------------
void MovingUITask::Button2Down()
{
	endTask();
}

// ---------------------------------------------------------------------
//! Called when mouse button 2 is released
// ---------------------------------------------------------------------
void MovingUITask::Button2Up()
{
	endTask();
}

// ---------------------------------------------------------------------
//! Ends this task.
// ---------------------------------------------------------------------
void MovingUITask::endTask()
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);

	uiManager->GetUIController()->EndMouseCapture();
	uiManager->EndTask(this);	
}
