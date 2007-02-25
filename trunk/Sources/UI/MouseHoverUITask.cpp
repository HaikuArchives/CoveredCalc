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
	@file		MouseHoverUITask.cpp
	@brief		Implementation of MouseHoverUITask
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.11.24 created
*/

#include "Prefix.h"
#include "MouseHoverUITask.h"
#include "UITaskClass.h"
#include "UIManager.h"
#include "UIController.h"
#include "ColorCodedSkin.h"

#define HOVER_TIMER_ELAPSE			50		///< timer elapse (msec)

/**
 *	@brief	Constructor
 */
MouseHoverUITask::MouseHoverUITask()
{
	normalActiveImageIndex = 0;
	normalInactiveImageIndex = 0;
	hotActiveImageIndex = 0;
	hotInactiveImageIndex = 0;
	targetSkinArea = ColorCodedSkin::Area_None;
	stepCount = 0;
	currentStep = 0;
}

/**
 *	@brief	Destructor
 */
MouseHoverUITask::~MouseHoverUITask()
{
}

/**
 *	@brief	Returns a kind of this class.
 */
SInt32 MouseHoverUITask::GetClass()
{
	return UITaskClass::MouseHoverUITask;
}

// ---------------------------------------------------------------------
//! Sets image indexes in the skin
// ---------------------------------------------------------------------
void MouseHoverUITask::SetImageIndex(
	SInt32 normalActiveImageIndex,		///< skin index of normal active image
	SInt32 normalInactiveImageIndex,	///< skin index of normal inactive image
	SInt32 hotActiveImageIndex,			///< skin index of hot active image
	SInt32 hotInactiveImageIndex		///< skin index of hot inactive image
)
{
	this->normalActiveImageIndex = normalActiveImageIndex;
	this->normalInactiveImageIndex = normalInactiveImageIndex;
	this->hotActiveImageIndex = hotActiveImageIndex;
	this->hotInactiveImageIndex = hotInactiveImageIndex;
}

/**
 *	@brief	Sets informations about initial state.
 *	@param	commandID	target command id.
 *	@param	stepCount	how many steps to become complete hot image.
 */
void MouseHoverUITask::SetInitialInformation(SInt32 commandID, SInt32 stepCount)
{
	this->targetCommandID = commandID;
	this->targetSkinArea = getUIManager()->GetSkinAreaFromCommandId(commandID);
	this->stepCount = stepCount;
}

/**
 *	@brief	Updates the appearance of the area.
 */
void MouseHoverUITask::updateAppearance()
{
	if (targetSkinArea == ColorCodedSkin::Area_None)
	{
		return;
	}

	UIManager* uiManager = getUIManager();
	ColorCodedSkin* skin = uiManager->GetSkin();

	SInt32 normalImageIndex = normalActiveImageIndex;
	SInt32 hotImageIndex = hotActiveImageIndex;
	if (!uiManager->GetUIController()->IsUIActive())
	{
		normalImageIndex = normalInactiveImageIndex;
		hotImageIndex = hotInactiveImageIndex;
	}

	skin->UpdatePartAppearanceWithBlend(targetSkinArea, hotImageIndex, normalImageIndex, currentStep * ColorCodedSkin::BlendRatio_Max / stepCount);
	uiManager->GetUIController()->UpdateUI();
}

// ---------------------------------------------------------------------
//! Cancels this task.
// ---------------------------------------------------------------------
void MouseHoverUITask::CancelTask()
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);

	uiManager->GetUIController()->EndTimer(UIController::TimerID_MouseHover);
	uiManager->EndTask(this);
}

/**
 *	@brief	Starts task.
 */
void MouseHoverUITask::StartTask()
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);

	currentStep = 1;
	isBeingHot = true;
	updateAppearance();
	uiManager->GetUIController()->BeginTimer(UIController::TimerID_MouseHover, HOVER_TIMER_ELAPSE);
}

/**
 *	@brief	Called when mouse cursor position is changed.
 */
void MouseHoverUITask::MouseMove()
{
	if (!changeStatusAccordingToMouse())
	{
		CancelTask();
	}
}

/**
 *	@brief	Called when user interface is being activated or being deactivated.
 */
void MouseHoverUITask::UIActivated()
{
	updateAppearance();
}

/**
 *	@brief	Called when timer event occurred.
 *	@param	timerId		ID of the timer.
 */
void MouseHoverUITask::TimerEvent(SInt32 timerId)
{
	if (timerId != UIController::TimerID_MouseHover)
	{
		return;
	}

	UInt32 commandState = getUIManager()->GetCommandState(targetCommandID);
	if (commandState & UIManager::CommandState_Disabled & commandState)
	{
		CancelTask();
		return;
	}

	if (!changeStatusAccordingToMouse())
	{
		CancelTask();
		return;
	}

	if (isBeingHot)
	{
		if (currentStep < stepCount)
		{
			currentStep++;
			updateAppearance();
		}
	}
	else
	{
		if (currentStep > 0)
		{
			currentStep--;
			updateAppearance();
			if (currentStep == 0)
			{
				CancelTask();
			}
		}
	}
}

/**
 *	@brief	Changes the status of this task according to mouse position.
 *	@return	false if this task shoule be canceled.
 */
bool MouseHoverUITask::changeStatusAccordingToMouse()
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);

	Point32 mousePoint = uiManager->GetUIController()->GetMousePosition();
	ColorCodedSkin* skin = uiManager->GetSkin();
	
	SInt32 mouseSkinArea = skin->GetArea(mousePoint);
	bool isInArea = (mouseSkinArea == targetSkinArea);
	bool isHoverable;
#if defined(BEOS)
	isHoverable = uiManager->GetUIController()->IsUIActive();
#else
	isHoverable = true;
#endif
	if (isBeingHot && (!isInArea || !isHoverable))
	{
		isBeingHot = false;
		if (currentStep > 0)
		{
			currentStep--;
			updateAppearance();
			if (currentStep == 0)
			{
				return false;
			}
		}
	}
	else if (!isBeingHot && isInArea && isHoverable)
	{
		isBeingHot = true;
		if (currentStep < stepCount)
		{
			currentStep++;
			updateAppearance();
		}
	}	
	return true;
}
