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
	@file		MainUIManager.h
	@brief		Definition of MainUIManager class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.27 created
*/

#ifndef _MAINUIMANAGER_H_
#define _MAINUIMANAGER_H_

#include "MBCString.h"
#include "UIManager.h"
#include "ButtonUITask.h"
#include "CalcCore.h"
#include "CalcDisplay.h"
#include "CoverChangeEventHandler.h"
#include "MainWindowKeyFunc.h"
#include "CoverMainWindowInfo.h"

class ColorCodedSkin;
class MainUIController;

// ---------------------------------------------------------------------
//! This class manages user interface of a main window
// ---------------------------------------------------------------------
class MainUIManager : public UIManager, public ButtonUITaskEventHandler, public CalcDisplay, public CoverChangeEventHandler
{
public:
								MainUIManager();
	virtual						~MainUIManager();

	void						Init(UIController* uiController, MainUIController* mainUIController, const KeyMappingManager* keyMappingManager);

	virtual void				Create();
	virtual void				Destroy();

	virtual void				MouseMove();
	virtual void				Button1Down();
	virtual void				Button2Down();
	virtual bool				KeyDown(const KeyEventParameter& parameter);
	virtual void				UIActivated();

	virtual SInt32				GetSkinAreaFromCommandId(SInt32 commandId);
	virtual UInt32				GetCommandState(SInt32 commandId);
	virtual void				ExecuteCommand(SInt32 commandId);


	CalcCore::DigitForm			GetCurrentDigitForm() const { return currentDigitForm; }
	
	// implementations of ButtonUITaskEventHandler interface
	virtual	void				ButtonClicked(SInt32 buttonCommand);

	// implementations of CalcDisplay interface
	virtual	void				DisplayValue(ConstAStr value);
	virtual void				DisplayOperator(CalcCore::Operator /*op*/) { }
	virtual	void				DisplayDigitForm(CalcCore::DigitForm form);
	virtual	void				DisplayError(CalcCore::Error error);

	// implementations of CoverChangeEventHandler interface
	virtual void				CoverDefChanged();
	virtual void				CurrentCoverChanged();

	MainUIController*			GetMainUIController() { return mainUIController; }
	const MainUIController*		GetMainUIController() const { return mainUIController; }

protected:
	virtual void				clearMembers();
	virtual void				readSkin();
	virtual void				changeLastCursorSkinArea(SInt32 area);

	CoverMainWindowInfo::ButtonClass getButtonFromKeyFunc(MainWindowKeyFunc::KeyFunc keyFunc);

private:
	void						reinitCalcCore();
	void						drawNumberStringOnDisplay(ConstAStr string);
	const CoverMainWindowInfo*	getCurrentMainWindowInfo() const;
	const Path&					getImageFilePath(const CoverMainWindowInfo* mainWindowInfo, SInt32 skinIndex);
	void						updateWholeAppearance();
	void						doCommandCoverBrowser();
	void						doCommandMainWindowAlwaysOnTop();
	void						doCommandMainWindowLockPos();
	void						doCommandPreferences();
	void						doCommandAbout();
	void						doCommandAboutCurrentCover();
	void						doCommandMainMinimize();
	void						doCommandClose();

private:
	MainUIController*			mainUIController;			///< main UI controller.
	CalcCore					calcCore;					//!< calc engine.
	CalcCore::DigitForm			currentDigitForm;			///< current digit form of engine.
	MBCString					numberStringOnDisplay;		//!< string on display
};

#endif // _MAINUIMANAGER_H_
