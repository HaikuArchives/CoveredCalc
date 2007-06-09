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
	@file       BeSkinView.cpp
	@brief      Implementation of BeSkinView class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2004.02.17 created
*/

#include "Prefix.h"
#include <AppKit.h>
#include <InterfaceKit.h>
#include <support/Autolock.h>
#include <support/String.h>
#include "UIManager.h"
#include "ColorCodedSkin.h"
#include "MenuInfo.h"
#include "UIControllerException.h"
#include "BeSkinView.h"
#include "DialogInfo.h"
#include "BeDialog.h"
#include "BeCoveredCalcApp.h"
#include "BeCCSAppearance.h"
#include "MemoryException.h"

#define MSG_TIMER_MOUSEHOVER	'tmmh'

////////////////////////////////////////
#define base	BView
////////////////////////////////////////

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeSkinView::BeSkinView(
	BRect frame,			//!< frame rect
	const char* name		//!< view name
)		: BView(frame, name, B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	mousePosition.x = 0;
	mousePosition.y = 0;
	mousePositionAvailable = false;
	uiManager = NULL;
	lastPressedMouseButton = 0;
	toolTipMessenger = NULL;
	mouseCaptureNestCount = 0;
	SInt32 index;
	for (index = 0; index < TimerID_Max; index++)
	{
		timerNestCount[index] = 0;
		timerRunners[index] = NULL;
	}
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeSkinView::~BeSkinView()
{
	toolTipMessenger->SendMessage(B_QUIT_REQUESTED);

	if (NULL != toolTipMessenger)
	{
		delete toolTipMessenger;
	}
	SInt32 index;
	for (index = 0; index < TimerID_Max; index++)
	{
		if (NULL != timerRunners[index])
		{
			delete timerRunners[index];
		}
	}
}

// ---------------------------------------------------------------------
//! Initializes the object
// ---------------------------------------------------------------------
void BeSkinView::Init()
{
	BeToolTipWindow* toolTipWindow = new BeToolTipWindow();
	toolTipWindow->Init(this, "");
	toolTipMessenger = new BMessenger(toolTipWindow);

	if (NULL != uiManager)
	{
		deleteUIManager(uiManager);
	}
	
	uiManager = createUIManager();
}

// ---------------------------------------------------------------------
//! Called by BeSkinWindow::Quit()
// ---------------------------------------------------------------------
void BeSkinView::Quit()
{
	if (NULL != uiManager)
	{
		deleteUIManager(uiManager);
		uiManager = NULL;
	}
}

/**
 *	@brief	Initializes ColorCodedSkinAppearance and then return it.
 *	@return	initialized ColorCodedSkinAppearance object.
 */
ColorCodedSkinAppearance* BeSkinView::InitSkinAppearance()
{
	BeCCSAppearance* appearance = new BeCCSAppearance();
	if (NULL == appearance)
	{
		MemoryException::Throw();
	}
	appearance->Init(this);
	return appearance;
}

/**
 *	@brief	Disposes unused ColorCodedSkinAppearance.
 *	@param[in]	appearance	ColorCodedSkinAppearance object to dispose.
 */
void BeSkinView::DisposeSkinAppearance(ColorCodedSkinAppearance *appearance)
{
	delete appearance;
}

/**
 *	@brief	If mouse position is not available, this function retrives current mouse position.]
 */
void BeSkinView::readyMousePosition() const
{
	if (!mousePositionAvailable)
	{
		BWindow* window = Window();
		BAutolock locker(window);
	
		BPoint where;
		uint32 buttons;
		
		BeSkinView* nonConstThis = const_cast<BeSkinView*>(this);
		nonConstThis->GetMouse(&where, &buttons, false);
		nonConstThis->mousePosition.x = static_cast<SInt32>(where.x);
		nonConstThis->mousePosition.y = static_cast<SInt32>(where.y);
		nonConstThis->mousePositionAvailable = true;
	}
}

// ---------------------------------------------------------------------
//! Retrives mouse location in screen coordinates.
// ---------------------------------------------------------------------
Point32 BeSkinView::GetMouseScreenPosition() const
{
	BAutolock locker(Window());
	
	readyMousePosition();

	BPoint mousePoint(mousePosition.x, mousePosition.y);
	ConvertToScreen(&mousePoint);
	Point32 retPoint;
	retPoint.x = static_cast<SInt32>(mousePoint.x);
	retPoint.y = static_cast<SInt32>(mousePoint.y);
	return retPoint;
}

// ---------------------------------------------------------------------
//! Retrives rectangele of UI in screen coordinates.
// ---------------------------------------------------------------------
void BeSkinView::GetUIRect(
	Rect32& rect				//!< OUTPUT. UI rectangle is returned.
) const
{
	BAutolock locker(Window());

	BRect bounds = Bounds();
	ConvertToScreen(&bounds);
	rect.left = static_cast<SInt32>(bounds.left);
	rect.top = static_cast<SInt32>(bounds.top);
	rect.right = static_cast<SInt32>(bounds.right + 1);
	rect.bottom = static_cast<SInt32>(bounds.bottom + 1);
}

// ---------------------------------------------------------------------
//! Moves UI location
// ---------------------------------------------------------------------
void BeSkinView::MoveUI(
	const Point32& topLeft		//!< top left of UI
)
{
	BWindow* window = Window();
	BAutolock locker(window);
	
	// TODO:完全に画面外に出てしまうときは少し画面内に残すようにしてそれ以上移動しないようにする
	
	window->MoveTo(static_cast<float>(topLeft.x), static_cast<float>(topLeft.y));
}

// ---------------------------------------------------------------------
//! Called when it starts dragging.
// ---------------------------------------------------------------------
void BeSkinView::BeginMouseCapture()
{
	if (0 == mouseCaptureNestCount)
		SetEventMask(EventMask() | B_POINTER_EVENTS);
	
	mouseCaptureNestCount++;
}

// ---------------------------------------------------------------------
//! Called when it ends dragging.
// ---------------------------------------------------------------------
void BeSkinView::EndMouseCapture()
{
	if (mouseCaptureNestCount > 0)
	{
		mouseCaptureNestCount--;
		
		if (0 == mouseCaptureNestCount)
		{
			SetEventMask(EventMask() & ~B_POINTER_EVENTS);
		}
	}
}

// ---------------------------------------------------------------------
//! Closes UI window.
// ---------------------------------------------------------------------
void BeSkinView::CloseUI()
{
	BWindow* window = Window();
	BAutolock locker(window);

	window->PostMessage(B_QUIT_REQUESTED);
}

// ---------------------------------------------------------------------
//! Returns whether window is active.
/*!
	@retval true window is active.
	@retval false windows is not active.
*/
// ---------------------------------------------------------------------
bool BeSkinView::IsUIActive() const
{
	BWindow* window = Window();
	BAutolock locker(window);

	return window->IsActive();
}

// ---------------------------------------------------------------------
//! Shows UI
// ---------------------------------------------------------------------
void BeSkinView::Show()
{
	BWindow* window = Window();
	BAutolock locker(window);

	return window->Show();
}

// ---------------------------------------------------------------------
//! Hides UI
// ---------------------------------------------------------------------
void BeSkinView::Hide()
{
	BWindow* window = Window();
	BAutolock locker(window);

	return window->Hide();
}

// ---------------------------------------------------------------------
//! Minimizes UI
// ---------------------------------------------------------------------
void BeSkinView::Minimize()
{
	BWindow* window = Window();
	BAutolock locker(window);
	
	window->Minimize(true);
}

// ---------------------------------------------------------------------
//! Returns whether window is minimized.
/*!
	@retval true window is minimized.
	@retval false windows is not minimized.
*/
// ---------------------------------------------------------------------
bool BeSkinView::IsUIMinimized() const
{
	BWindow* window = Window();
	BAutolock locker(window);
	
	return window->IsMinimized();
}

// ---------------------------------------------------------------------
//! Restores minimized UI
// ---------------------------------------------------------------------
void BeSkinView::Restore()
{
	BWindow* window = Window();
	BAutolock locker(window);
	
	window->Minimize(false);
}

// ---------------------------------------------------------------------
//! Shows context menu
// ---------------------------------------------------------------------
void BeSkinView::ShowContextMenu(
	MenuInfo *menuInfo			//!< information about the menu.
)
{
	BWindow* window = Window();
	BAutolock locker(window);

	// Create popup menu.
	BPopUpMenu* popupMenu = createContextMenu(menuInfo->GetMenuID());

	// Update states
	updateMenuItemStates(popupMenu, menuInfo);

	// Show popup menu.
	Point32 point32 = menuInfo->GetMenuPosition();
	BPoint	point(point32.x, point32.y);
	BRect rect(point, point);
	rect.InsetBy(-5, -5);
	BMenuItem* selectedMenuItem = popupMenu->Go(point, false, true, rect, false);
	if (NULL != selectedMenuItem && NULL != selectedMenuItem->Message())
	{
		BMessenger messenger(this);
		messenger.SendMessage(selectedMenuItem->Message());
	}
	delete popupMenu;
}

/**
 *	@brief	Creates context menu.
 *	@param[in]	menuID	menu id.
 *	@return		context menu.
 */
BPopUpMenu* BeSkinView::createContextMenu(SInt32 menuID)
{
	throw new UIControllerExceptions::FailedToShowContextMenu(menuID);
}

/**
 *	@brief	Reads items array in menu design resource.
 *	@return	trigger character
 */
char BeSkinView::parseMenuItemTrigger(
	const char* sourceLabel,	///< source label string
	BString& outLabel			///< OUTPUT. label string without '&' prefix character.
)
{
	char trigger = 0;
	outLabel = sourceLabel;
	int32 find = 0;
	
	while (true)
	{
		find = outLabel.FindFirst('~', find);
		if (B_ERROR == find)
		{
			break;
		}
		outLabel.Remove(find, 1);
		if (outLabel[find] != '~')
		{
			if (0 == trigger)
			{
				trigger = tolower(outLabel[find]);
			}
		}
		else
		{
			find++;
		}
	}
	
	return trigger;
}

/// helper function to create menu item.
BMenuItem* BeSkinView::createMenuItem(ConstUTF8Str name, uint32 command, int8 shortcut, uint32 modifiers)
{
	const BeXMLLangFile* langFile = BeCoveredCalcApp::GetInstance()->GetLangFile();
	MBCString sourceLabel;
	langFile->LoadString(name, sourceLabel);
	BString label;
	char trigger = parseMenuItemTrigger(sourceLabel, label);
	BMenuItem* newItem = new BMenuItem(label.String(), new BMessage(command), shortcut, modifiers);
	newItem->SetTrigger(trigger);
	return newItem;
}

/// helper function to create sub menu.
BMenu* BeSkinView::createSubMenu(ConstUTF8Str name)
{
	const BeXMLLangFile* langFile = BeCoveredCalcApp::GetInstance()->GetLangFile();
	MBCString sourceLabel;
	langFile->LoadString(name, sourceLabel);
	BString label;
	parseMenuItemTrigger(sourceLabel, label);
	return new BMenu(label.String(), B_ITEMS_IN_COLUMN);
}

// ---------------------------------------------------------------------
//! Updates the state of each menu item in specified menu.
/*!
	@note if there is submenu in the menu, update items in it.
*/
// ---------------------------------------------------------------------
void BeSkinView::updateMenuItemStates(
	BMenu* menu,					//!< menu
	MenuInfo* menuInfo				//!< information about menu
)
{
	int32 count = menu->CountItems();
	int32 index;
	for (index=0; index<count; index++)
	{
		BMenuItem* menuItem = menu->ItemAt(index);
		if (NULL == menuItem)
		{
			continue;
		}
		BMenu* subMenu = menuItem->Submenu();
		if (NULL != subMenu)
		{
			updateMenuItemStates(subMenu, menuInfo);
		}
		else
		{
			BMessage* itemMessage = menuItem->Message();
			if (NULL != itemMessage)
			{
				bool isEnabled = true;
				bool isChecked = false;
				UInt32 itemStates = menuInfo->GetMenuItemStates(static_cast<SInt32>(itemMessage->what));
				if (itemStates & MenuInfo::MenuItemState_Checked)
				{
					isChecked = true;
				}
				if (itemStates & MenuInfo::MenuItemState_Disabled)
				{
					isEnabled = false;
				}
				menuItem->SetEnabled(isEnabled);
				menuItem->SetMarked(isChecked);
			}
		}
	}
}

// ---------------------------------------------------------------------
//! Shows dialog
// ---------------------------------------------------------------------
void BeSkinView::ShowDialog(
	DialogInfo* dialogInfo		//!< information about the dialog.
)
{
	throw new UIControllerExceptions::FailedToShowDialog(dialogInfo->GetDialogID());
}

// ---------------------------------------------------------------------
//! Updates UI immediately
// ---------------------------------------------------------------------
void BeSkinView::UpdateUI()
{
	BWindow* window = Window();
	BAutolock locker(window);

	window->UpdateIfNeeded();
	Sync();
}

// ---------------------------------------------------------------------
//! Waits specified period
// ---------------------------------------------------------------------
void BeSkinView::Wait(
	UInt32 milliseconds			//!< number of milliseconds.
)
{
	snooze(static_cast<bigtime_t>(milliseconds) * 1000);
}

/**
 *	@brief	Sets tool tip text.
 */
void BeSkinView::SetToolTipText(
	ConstAStr text				///< new tool tip text
)
{
	BMessage deactivateMessage(BeToolTipWindow::MSG_DEACTIVATE);
	toolTipMessenger->SendMessage(&deactivateMessage, static_cast<BHandler*>(NULL), 1000000);

	BMessage setTextMessage(BeToolTipWindow::MSG_SETTEXT);
	setTextMessage.AddString(BeToolTipWindow::PARAM_TEXT, text);
	toolTipMessenger->SendMessage(&setTextMessage, static_cast<BHandler*>(NULL), 1000000);

	if ('\0' != text[0])
	{
		BMessage activateMessage(BeToolTipWindow::MSG_ACTIVATE);
		toolTipMessenger->SendMessage(&activateMessage, static_cast<BHandler*>(NULL), 1000000);		
	}
}

/**
 *	@brief	Set or reset the flag which means the window is always on top or not.
 */
void BeSkinView::SetAlwaysOnTopFlag(
	bool isFlagOn			///< true if on top
)
{
	BWindow* window = Window();
	BAutolock locker(window);
	if (locker.IsLocked())
	{
		if (isFlagOn)
		{
			window->SetFeel(B_FLOATING_ALL_WINDOW_FEEL);
		}
		else
		{
			window->SetFeel(B_NORMAL_WINDOW_FEEL);
		}
	}
}

/**
 *	@brief	Converts from UIController's timer ID to message command.
 *	@param	timerId	UIController's timer ID
 *	@return	timer message command.
 */
uint32 BeSkinView::timerIdToTimerCommand(UIController::TimerID timerId)
{
	switch (timerId)
	{
	case TimerID_MouseHover:
		return MSG_TIMER_MOUSEHOVER;
	default:
		ASSERT(false);
		return 0;
	}
}

/**
 *	@brief	Converts from timer message comand to UIController's timer ID.
 *	@param	timerCommand	timer message command.
 *	@return	UIController's timer ID.
 */
UIController::TimerID BeSkinView::timerCommandToTimerId(uint32 timerCommand)
{
	switch (timerCommand)
	{
	case MSG_TIMER_MOUSEHOVER:
		return TimerID_MouseHover;
	default:
		ASSERT(false);
		return TimerID_Max;
	}
}

/**
 *	@brief	Begins specified timer.
 *	@param	timerId	kind of timer.
 *	@param	elapse	timer intervals (msec)
 */
void BeSkinView::BeginTimer(UIController::TimerID timerId, UInt32 elapse)
{
	BWindow* window = Window();
	BAutolock locker(window);

	if (timerNestCount[timerId] == 0)
	{
		ASSERT(NULL == timerRunners[timerId]);
		timerRunners[timerId] = new BMessageRunner(BMessenger(this), new BMessage(timerIdToTimerCommand(timerId)), elapse * 1000);
	}
	else
	{
		ASSERT(NULL != timerRunners[timerId]);
		bigtime_t interval = 0;
		int32 count;
		timerRunners[timerId]->GetInfo(&interval, &count);
		if (interval != elapse * 1000)
		{
			timerRunners[timerId]->SetInterval(elapse * 1000);
		}
	}
	timerNestCount[timerId]++;
}

/**
 *	@brief	Ends specified timer.
 *	@param	timerId	kind of timer.
 */
void BeSkinView::EndTimer(UIController::TimerID timerId)
{
	BWindow* window = Window();
	BAutolock locker(window);

	if (timerNestCount[timerId] > 0)
	{
		timerNestCount[timerId]--;
		if (timerNestCount[timerId] == 0)
		{
			delete timerRunners[timerId];
			timerRunners[timerId] = NULL;
		}
	}
}

// ---------------------------------------------------------------------
//! Draws the update rectangle of the view.
// ---------------------------------------------------------------------
void BeSkinView::Draw(
	BRect updateRect	//!< update rectangle
)
{
	BeCCSAppearance* appearance = static_cast<BeCCSAppearance*>(uiManager->GetSkinAppearance());
	const BBitmap* bitmap = appearance->GetBitmap();

	ColorCodedSkin* skin = uiManager->GetSkin();
	BRect skinRect( 0, 0, skin->GetWidth() - 1, skin->GetHeight() - 1);
	BRect drawRect = skinRect & updateRect;
	DrawBitmap(bitmap, drawRect, drawRect);
}

// ---------------------------------------------------------------------
//! Called when user presses a mouse button.
// ---------------------------------------------------------------------
void BeSkinView::MouseDown(
	BPoint where			//!< location of the cursor
)
{
	uint32 buttons = 0;
	BMessage* message = Window()->CurrentMessage();
	if (NULL != message)
	{
		int32 value = 0;
		if (B_OK == message->FindInt32("buttons", &value))
		{
			buttons = value;
		}
	}
	if (0 == (buttons & (B_PRIMARY_MOUSE_BUTTON | B_SECONDARY_MOUSE_BUTTON)))
	{
		return;
	}

	mousePosition.x = static_cast<SInt32>(where.x);
	mousePosition.y = static_cast<SInt32>(where.y);
	mousePositionAvailable = true;

	BWindow* window = Window();
	if (!window->IsActive())
	{
		window->Activate();
	}

	// deactivate tool tip
	BMessage deactivateMessage(BeToolTipWindow::MSG_DEACTIVATE);
	toolTipMessenger->SendMessage(&deactivateMessage, static_cast<BHandler*>(NULL), 1000000);
	
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		if (buttons & B_PRIMARY_MOUSE_BUTTON)
		{
			uiManager->Button1Down();
			lastPressedMouseButton = 1;
		}
		else if (buttons & B_SECONDARY_MOUSE_BUTTON)
		{
			uiManager->Button2Down();
			lastPressedMouseButton = 2;
		}
	}
}

// ---------------------------------------------------------------------
//! Called when user releases a mouse button.
// ---------------------------------------------------------------------
void BeSkinView::MouseUp(
	BPoint where			//!< location of the cursor
)
{
	if (0 == lastPressedMouseButton)
	{
		return;
	}
	
	mousePosition.x = static_cast<SInt32>(where.x);
	mousePosition.y = static_cast<SInt32>(where.y);
	mousePositionAvailable = true;
	
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		if (1 == lastPressedMouseButton)
		{
			uiManager->Button1Up();
		}
		else if (2 == lastPressedMouseButton)
		{
			uiManager->Button2Up();
		}
	}
	
	lastPressedMouseButton = 0;
}

// ---------------------------------------------------------------------
//! Called when user moves the mouse cursor.
// ---------------------------------------------------------------------
void BeSkinView::MouseMoved(
	BPoint where,				//!< location of the cursor
	uint32 code,				//!< enter or exit view, inside or outside view
	const BMessage *a_message	//!< message if user is dragging
)
{
	mousePosition.x = static_cast<SInt32>(where.x);
	mousePosition.y = static_cast<SInt32>(where.y);
	mousePositionAvailable = true;

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->MouseMove();		
	}
}

// ---------------------------------------------------------------------
//! Called when window is activated or deactivated.
// ---------------------------------------------------------------------
void BeSkinView::WindowActivated(
	bool state			//!< true if windows has become active, false if window no longer is the active.
)
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->UIActivated();
	}
}

// ---------------------------------------------------------------------
//! Called when key is down.
// ---------------------------------------------------------------------
void BeSkinView::KeyDown(
	const char* bytes,	//!< character which mapped to the key.
	int32 numBytes		//!< length of parameter bytes.
)
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		KeyEventParameter parameter;
		parameter.SetBytes(bytes, numBytes);
		uiManager->KeyDown(parameter);
	}
}

// ---------------------------------------------------------------------
//! Called when key is down.
// ---------------------------------------------------------------------
void BeSkinView::KeyUp(
	const char* bytes,	//!< character which mapped to the key.
	int32 numBytes		//!< length of parameter bytes.
)
{
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		KeyEventParameter parameter;
		parameter.SetBytes(bytes, numBytes);
		uiManager->KeyUp(parameter);
	}
}

/**
 *	@brief	message handler.
 */
void BeSkinView::MessageReceived(BMessage *message)
{
	mousePositionAvailable = false;
	
	switch (message->what)
	{
	case MSG_TIMER_MOUSEHOVER:
		timerCommandReceived(message->what);
		break;
	default:
		base::MessageReceived(message);
		break;
	}
}

/**
 *	@brief	timer command handler.
 */
void BeSkinView::timerCommandReceived(uint32 timerCommand)
{
	UIController::TimerID timerId = timerCommandToTimerId(timerCommand);
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->TimerEvent(timerId);
	}
}
