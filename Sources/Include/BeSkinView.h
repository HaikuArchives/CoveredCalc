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
	@file       BeSkinView.h
	@brief      Definition of BeSkinView class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2004.02.17 created
*/

#ifndef _BESKINVIEW_H_
#define _BESKINVIEW_H_

#include <interface/View.h>
#include "UIController.h"
#include "BeToolTipWindow.h"

class UIManager;
class BeDialog;
class DialogInfo;
class BMenu;
class BMenuItem;
class BPopUpMenu;

// ---------------------------------------------------------------------
//! Skin view for BeOS
/*!
	The view classes which use skins are derived from this class.
*/
// ---------------------------------------------------------------------
class BeSkinView : public BView, public UIController
{
public:
							BeSkinView(BRect frame, const char* name);
	virtual 				~BeSkinView();

	virtual void			Init();
	virtual void			Quit();

	UIManager*				GetUIManager() { return getUIManager(); }

	// implementation of UIController interface
	virtual ColorCodedSkinAppearance*	InitSkinAppearance();
	virtual void			DisposeSkinAppearance(ColorCodedSkinAppearance* appearance);
	virtual Point32 		GetMousePosition() const { readyMousePosition(); return mousePosition; }
	virtual Point32 		GetMouseScreenPosition() const;
	virtual void			GetUIRect(Rect32& rect) const;
	virtual void			MoveUI(const Point32& topLeft);
	virtual void			BeginMouseCapture();
	virtual void			EndMouseCapture();
	virtual void			CloseUI();
	virtual bool			IsUIActive() const;
	virtual void			Show();
	virtual void			Hide();
	virtual void			Minimize();
	virtual bool			IsUIMinimized() const;
	virtual void			Restore();
	virtual void			ShowContextMenu(MenuInfo* menuInfo);
	virtual void			ShowDialog(DialogInfo* dialogInfo);
	virtual void			UpdateUI();
	virtual void			Wait(UInt32 milliseconds);
	virtual void			SetToolTipText(ConstAStr text);
	virtual void			SetAlwaysOnTopFlag(bool isFlagOn);
	virtual void			BeginTimer(UIController::TimerID timerId, UInt32 elapse);
	virtual void			EndTimer(UIController::TimerID timerId);

protected:
	virtual UIManager*		createUIManager() = 0;
	virtual void			deleteUIManager(UIManager* uiManager) = 0;

	virtual BPopUpMenu*		createContextMenu(SInt32 menuID);
	BMenuItem*				createMenuItem(ConstUTF8Str name, uint32 command, int8 shortcut, uint32 modifiers);
	BMenu*					createSubMenu(ConstUTF8Str name);

	UIManager*				getUIManager() { return uiManager; }

	virtual	void			Draw(BRect updateRect);
	virtual	void			MouseDown(BPoint where);
	virtual	void			MouseUp(BPoint where);
	virtual	void			MouseMoved(BPoint where, uint32 code, const BMessage *a_message);
	virtual	void			WindowActivated(bool state);
	virtual void			KeyDown(const char* bytes, int32 numBytes);
	virtual void			KeyUp(const char* bytes, int32 numBytes);

	virtual	void			MessageReceived(BMessage* message);

private:
	void					updateMenuItemStates(BMenu* menu, MenuInfo* menuInfo);
	char					parseMenuItemTrigger(const char* sourceLabel, BString& outLabel);

	uint32					timerIdToTimerCommand(UIController::TimerID timerId);
	UIController::TimerID	timerCommandToTimerId(uint32 timerCommand);
	void					timerCommandReceived(uint32 timerCommand);
	void					readyMousePosition() const;

private:
	Point32 				mousePosition;	//!< mouse location when some event has occured
	bool					mousePositionAvailable;
	UIManager*				uiManager;		//!< ui manager for this view
	SInt32					lastPressedMouseButton;		//!< mouse button which pressed at last MouseDown()
														//!< 0:none, 1:primary, 2:secondary
	BMessenger*				toolTipMessenger;	///< messenger for tool tip window.
	SInt32					mouseCaptureNestCount;	///< nest count of BeginMouseCapture.
	SInt32					timerNestCount[TimerID_Max];	///< nest count of BeginTimer.
	BMessageRunner*			timerRunners[TimerID_Max];	///< timers
};

#endif // _BESKINVIEW_H_
