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
	@file		ButtonUITask.cpp
	@brief		Implementation of ButtonUITask
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.29 created
*/

#include "Prefix.h"
#include "ButtonUITask.h"
#include "ColorCodedSkin.h"
#include "UIManager.h"
#include "UITaskClass.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
ButtonUITask::ButtonUITask()
{
	eventHandler = NULL;
	targetCommand = -1;
	targetSkinArea = ColorCodedSkin::Area_None;
	normalActiveImageIndex = 0;
	normalInactiveImageIndex = 0;
	pressedImageIndex = 0;
	currentImageIndex = -1;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
ButtonUITask::~ButtonUITask()
{
}

/**
 *	@brief	Returns a kind of this class.
 */
SInt32 ButtonUITask::GetClass()
{
	return UITaskClass::ButtonUITask;
}

// ---------------------------------------------------------------------
//! Sets target command
// ---------------------------------------------------------------------
void ButtonUITask::setTargetCommand(
	SInt32 targetCommand				//!< target command
)
{
	UIManager* uiManager = getUIManager();
	this->targetCommand = targetCommand;
	this->targetSkinArea = uiManager->GetSkinAreaFromCommandId(targetCommand);
}


// ---------------------------------------------------------------------
//! Sets image indexes in the skin
// ---------------------------------------------------------------------
void ButtonUITask::SetImageIndex(
	SInt32 normalActiveImageIndex,		///< skin index of normal active image
	SInt32 normalInactiveImageIndex,	///< skin index of normal inactive image
	SInt32 pressedImageIndex			///< skin index of pressed image
)
{
	this->normalActiveImageIndex = normalActiveImageIndex;
	this->normalInactiveImageIndex = normalInactiveImageIndex;
	this->pressedImageIndex = pressedImageIndex;
}

// ---------------------------------------------------------------------
//! Sets informations about initial state.
// ---------------------------------------------------------------------
void ButtonUITask::SetInitialInformation(
	SInt32 targetCommand				//!< target button
)
{
	setTargetCommand(targetCommand);
}

// ---------------------------------------------------------------------
//! Updates the appearance of the button
// ---------------------------------------------------------------------
void ButtonUITask::updateButtonAppearance(
	bool isPressed				//!< whether button is pressed
)
{
	if (targetSkinArea == ColorCodedSkin::Area_None)
	{
		return;
	}

	UIManager* uiManager = getUIManager();
	ColorCodedSkin* skin = uiManager->GetSkin();

	SInt32 imageIndex;

	if (isPressed)
	{
		imageIndex = pressedImageIndex;
	}
	else
	{
		imageIndex = uiManager->GetUIController()->IsUIActive() ? normalActiveImageIndex : normalInactiveImageIndex;
	}
	
	if (currentImageIndex != imageIndex)
	{
		currentImageIndex = imageIndex;
		skin->UpdatePartAppearance(targetSkinArea, imageIndex);
	}
}

// ---------------------------------------------------------------------
//! Starts task
// ---------------------------------------------------------------------
void ButtonUITask::StartTask()
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);

	updateButtonAppearance(true);
	uiManager->GetUIController()->BeginMouseCapture();
}

// ---------------------------------------------------------------------
//! Called when mouse cursor moves
// ---------------------------------------------------------------------
void ButtonUITask::MouseMove()
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);

	Point32 mousePoint = uiManager->GetUIController()->GetMousePosition();
	ColorCodedSkin* skin = uiManager->GetSkin();
	
	SInt32 mouseSkinArea = skin->GetArea(mousePoint);
	updateButtonAppearance(mouseSkinArea == targetSkinArea);
}

// ---------------------------------------------------------------------
//! Called when mouse button 1 is released
// ---------------------------------------------------------------------
void ButtonUITask::Button1Up()
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);

	Point32 mousePoint = uiManager->GetUIController()->GetMousePosition();
	ColorCodedSkin* skin = uiManager->GetSkin();

	uiManager->GetUIController()->EndMouseCapture();

	updateButtonAppearance(false);
	
	SInt32 area = skin->GetArea(mousePoint);		
	if (area == targetSkinArea)
	{
		if (NULL != eventHandler)
		{
			eventHandler->ButtonClicked(targetCommand);
		}
	}	
	
	uiManager->EndTask(this);
}

// ---------------------------------------------------------------------
//! Called when mouse button 2 is pressed
// ---------------------------------------------------------------------
void ButtonUITask::Button2Down()
{
	cancelTask();
}

// ---------------------------------------------------------------------
//! Called when mouse button 2 is released
// ---------------------------------------------------------------------
void ButtonUITask::Button2Up()
{
	cancelTask();
}

// ---------------------------------------------------------------------
//! Called when key is pressed.
// ---------------------------------------------------------------------
void ButtonUITask::KeyDown(
	const KeyEventParameter& /*parameter*/	//!< parameter
)
{
	cancelTask();
}

// ---------------------------------------------------------------------
//! Called when key is released.
// ---------------------------------------------------------------------
void ButtonUITask::KeyUp(
	const KeyEventParameter& /*parameter*/	//!< parameter
)
{
	cancelTask();
}

// ---------------------------------------------------------------------
//! Cancels push of the button.
// ---------------------------------------------------------------------
void ButtonUITask::cancelTask()
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);

	uiManager->GetUIController()->EndMouseCapture();

	updateButtonAppearance(false);
	
	uiManager->EndTask(this);
}

// ---------------------------------------------------------------------
//! Called when user interface is activated or deactivated.
// ---------------------------------------------------------------------
void ButtonUITask::UIActivated()
{
	// redraw button
	currentImageIndex = -1;
	MouseMove();
}
