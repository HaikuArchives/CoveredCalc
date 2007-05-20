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
	@file		UIManager.h
	@brief		Implementation of UIManager class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.28 created
*/

#include "Prefix.h"
#include "UIManager.h"
#include "UITask.h"
#include "ColorCodedSkin.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
UIManager::UIManager()
{
	uiController = NULL;
	skin = NULL;
	appearance = NULL;
	basePoint.x = basePoint.y = 0;
	lastCursorSkinArea = ColorCodedSkin::Area_None;
}

// ---------------------------------------------------------------------
//! Initialize the object
// ---------------------------------------------------------------------
void UIManager::Init(
	UIController* uiController		// controller object
)
{
	this->uiController = uiController;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
UIManager::~UIManager()
{
	UIManager::clearMembers();
}

// ---------------------------------------------------------------------
//! Clear all members
// ---------------------------------------------------------------------
void UIManager::clearMembers()
{
	taskManager.EndAllTasks();

	if (NULL != skin)
	{
		delete skin;
		skin = NULL;
	}
	if (NULL != appearance)
	{
		uiController->DisposeSkinAppearance(appearance);
		appearance = NULL;
	}
}

// ---------------------------------------------------------------------
//! Destroys user interface.
// ---------------------------------------------------------------------
void UIManager::Destroy()
{
	clearMembers();
}

/**
 *	@brief		Starts specified task.
 *	@param[in]	task	a task which is starting.
 */
void UIManager::StartTask(UITask* task)
{
	taskManager.StartTask(task);
}

// ---------------------------------------------------------------------
//! Called when task ends
// ---------------------------------------------------------------------
void UIManager::EndTask(
	UITask* task			//!< the task which ends
)
{
	taskManager.EndTask(task);
}

// ---------------------------------------------------------------------
//! Sets a skin to the object
// ---------------------------------------------------------------------
void UIManager::setSkin(
	ColorCodedSkin* skin		//!< skin object
)
{
	if (NULL != this->skin)
	{
		delete this->skin;
	}
	this->skin = skin;
}

/**
 *	@brief	Sets an appearance of ui.
 */
void UIManager::setSkinAppearance(
	ColorCodedSkinAppearance* appearance	///< skin appearance
)
{
	if (NULL != this->appearance)
	{
		uiController->DisposeSkinAppearance(this->appearance);
	}
	this->appearance = appearance;
}

// ---------------------------------------------------------------------
//! Sets base point of the skin, and moves UI location if necessary
// ---------------------------------------------------------------------
void UIManager::setBasePoint(
	const Point32& basePoint	//!< new base point
)
{
	Point32 oldBasePoint = this->basePoint;
	this->basePoint = basePoint;

	if (NULL != uiController)
	{
		// moves top-left of UI so that the base point will not move.
		Rect32 uiRect;
		uiController->GetUIRect(uiRect);
		
		Point32 topLeft;
		topLeft.x = uiRect.left + oldBasePoint.x - basePoint.x;
		topLeft.y = uiRect.top + oldBasePoint.y - basePoint.y;
		uiController->MoveUI(topLeft);
	}
}

// ---------------------------------------------------------------------
//! Sets last skin area at cursor positon.
// ---------------------------------------------------------------------
void UIManager::setLastCursorSkinArea(
	SInt32 area					///< skin area
)
{
	if (lastCursorSkinArea != area)
	{
		changeLastCursorSkinArea(area);
	}
}

// ---------------------------------------------------------------------
//! Changes last skin area at cursor positon.
// ---------------------------------------------------------------------
void UIManager::changeLastCursorSkinArea(
	SInt32 area					///< skin area
)
{
	lastCursorSkinArea = area;
}

class MouseMoveCommand : public UITaskManager::Command
{
public:
	/**
	 *	@brief	Execute command with a task.
	 *	@retval	true	wants to continue enumerating.
	 *	@retval	false	wants to stop enumerating.
	 */
	virtual bool	DoIt(UITask* task)
	{
		task->MouseMove();
		return true;
	}
};
// ---------------------------------------------------------------------
//! Called when mouse moves
// ---------------------------------------------------------------------
void UIManager::MouseMove()
{
	MouseMoveCommand command;
	taskManager.DoForEach(&command);
	
	// update cursor skin area
	Point32 cursorPoint = GetUIController()->GetMousePosition();
	ColorCodedSkin* skin = getSkin();
	if (NULL != skin)
	{
		SInt32 skinArea = skin->GetArea(cursorPoint);
		setLastCursorSkinArea(skinArea);
	}
}

class UIActivatedCommand : public UITaskManager::Command
{
public:
	/**
	 *	@brief	Execute command with a task.
	 *	@retval	true	wants to continue enumerating.
	 *	@retval	false	wants to stop enumerating.
	 */
	virtual bool	DoIt(UITask* task)
	{
		task->UIActivated();
		return true;
	}
};
// ---------------------------------------------------------------------
//! Called when user interface is activated or deactivated.
// ---------------------------------------------------------------------
void UIManager::UIActivated()
{
	UIActivatedCommand command;
	taskManager.DoForEach(&command);
}

class Button1DownCommand : public UITaskManager::Command
{
public:
	/**
	 *	@brief	Execute command with a task.
	 *	@retval	true	wants to continue enumerating.
	 *	@retval	false	wants to stop enumerating.
	 */
	virtual bool	DoIt(UITask* task)
	{
		task->Button1Down();
		return true;
	}
};
// ---------------------------------------------------------------------
//! Called when mouse button 1 is pressed.
// ---------------------------------------------------------------------
void UIManager::Button1Down()
{
	Button1DownCommand command;
	taskManager.DoForEach(&command);
}

class Button1UpCommand : public UITaskManager::Command
{
public:
	/**
	 *	@brief	Execute command with a task.
	 *	@retval	true	wants to continue enumerating.
	 *	@retval	false	wants to stop enumerating.
	 */
	virtual bool	DoIt(UITask* task)
	{
		task->Button1Up();
		return true;
	}
};
// ---------------------------------------------------------------------
//! Called when mouse button 1 is released.
// ---------------------------------------------------------------------
void UIManager::Button1Up()
{
	Button1UpCommand command;
	taskManager.DoForEach(&command);
}

class Button2DownCommand : public UITaskManager::Command
{
public:
	/**
	 *	@brief	Execute command with a task.
	 *	@retval	true	wants to continue enumerating.
	 *	@retval	false	wants to stop enumerating.
	 */
	virtual bool	DoIt(UITask* task)
	{
		task->Button2Down();
		return true;
	}
};
// ---------------------------------------------------------------------
//! Called when mouse button 2 is pressed.
// ---------------------------------------------------------------------
void UIManager::Button2Down()
{
	Button2DownCommand command;
	taskManager.DoForEach(&command);
}

class Button2UpCommand : public UITaskManager::Command
{
public:
	/**
	 *	@brief	Execute command with a task.
	 *	@retval	true	wants to continue enumerating.
	 *	@retval	false	wants to stop enumerating.
	 */
	virtual bool	DoIt(UITask* task)
	{
		task->Button2Up();
		return true;
	}
};
// ---------------------------------------------------------------------
//! Called when mouse button 2 is released.
// ---------------------------------------------------------------------
void UIManager::Button2Up()
{
	Button2UpCommand command;
	taskManager.DoForEach(&command);
}

class KeyDownCommand : public UITaskManager::Command
{
public:
	KeyDownCommand(const KeyEventParameter* parameter)
	{
		this->parameter = parameter;
	}

	/**
	 *	@brief	Execute command with a task.
	 *	@retval	true	wants to continue enumerating.
	 *	@retval	false	wants to stop enumerating.
	 */
	virtual bool	DoIt(UITask* task)
	{
		task->KeyDown(*parameter);
		return true;
	}
private:
	const KeyEventParameter* parameter;
};
// ---------------------------------------------------------------------
//! Called when a key is pressed.
// ---------------------------------------------------------------------
void UIManager::KeyDown(
	const KeyEventParameter& parameter		//!< event parameter
)
{
	KeyDownCommand command(&parameter);
	taskManager.DoForEach(&command);
}

class KeyUpCommand : public UITaskManager::Command
{
public:
	KeyUpCommand(const KeyEventParameter* parameter)
	{
		this->parameter = parameter;
	}

	/**
	 *	@brief	Execute command with a task.
	 *	@retval	true	wants to continue enumerating.
	 *	@retval	false	wants to stop enumerating.
	 */
	virtual bool	DoIt(UITask* task)
	{
		task->KeyUp(*parameter);
		return true;
	}
private:
	const KeyEventParameter* parameter;
};
// ---------------------------------------------------------------------
//! Called when a key is released.
// ---------------------------------------------------------------------
void UIManager::KeyUp(
	const KeyEventParameter& parameter		//!< event parameter
)
{
	KeyUpCommand command(&parameter);
	taskManager.DoForEach(&command);
}

class TimerEventCommand : public UITaskManager::Command
{
public:
	TimerEventCommand(SInt32 timerId)
	{
		this->timerId = timerId;
	}

	/**
	 *	@brief	Execute command with a task.
	 *	@retval	true	wants to continue enumerating.
	 *	@retval	false	wants to stop enumerating.
	 */
	virtual bool	DoIt(UITask* task)
	{
		task->TimerEvent(timerId);
		return true;
	}
private:
	SInt32	timerId;
};
// ---------------------------------------------------------------------
//! Called when timer event occurred.
// ---------------------------------------------------------------------
void UIManager::TimerEvent(SInt32 timerId)
{
	TimerEventCommand command(timerId);
	taskManager.DoForEach(&command);
}
