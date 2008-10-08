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
	@file       BeMainView.cpp
	@brief      Implementation of BeMainView class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2004.02.18 created
*/

#include "Prefix.h"
#include <AppKit.h>
#include <InterfaceKit.h>
#include "MainUIManager.h"
#include "BeMainView.h"
#include "BeCoveredCalcApp.h"
#include "DialogID.h"
#include "CommandID.h"
#include "BeAboutDialog.h"
#include "UIControllerException.h"
#include "BeAboutCurrentCoverDialog.h"
#include "BeDialogDesign.h"
#include "BePreferencesDlg.h"
#include <support/Autolock.h>

////////////////////////////////////////
#define base	BeSkinView
////////////////////////////////////////

#include <Alert.h>

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeMainView::BeMainView(
	BRect frame,			//!< frame rect
	const char* name		//!< view name
)
	: BeSkinView(frame, name)
{
	aboutDialogMessenger = NULL;
	aboutCCDialogMessenger = NULL;
	prefDialogMessenger = NULL;
	deskbarWindow = NULL;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeMainView::~BeMainView()
{
	if (NULL != aboutDialogMessenger)
		delete aboutDialogMessenger;
	
	if (NULL != aboutCCDialogMessenger)
		delete aboutCCDialogMessenger;
}

/**
 *	@brief	Initialized the object
 */
void BeMainView::Init()
{
	BWindow* mainWindow = Window();

	deskbarWindow = new BeDeskbarPseudoWindow();
	deskbarWindow->Init(mainWindow);

	if (B_FLOATING_ALL_WINDOW_FEEL == mainWindow->Feel())
	{
		BAutolock locker(deskbarWindow);
		if (locker.IsLocked())
		{
			deskbarWindow->Show();
		}
	}

	base::Init();
}

// ---------------------------------------------------------------------
//! Creates a new UIManager object
/*!
	@return UIManager object
*/
// ---------------------------------------------------------------------
UIManager* BeMainView::createUIManager()
{
	MainUIManager* uiManager = new MainUIManager();
	try
	{
		uiManager->Init(this, this, CoveredCalcApp::GetInstance()->GetKeyMappingManagerForMainWindow());
		uiManager->Create();
	}
	catch (...)
	{
		delete uiManager;
		throw;
	}
	return uiManager;
}

// ---------------------------------------------------------------------
//! Deletes a UIManager object
// ---------------------------------------------------------------------
void BeMainView::deleteUIManager(
	UIManager* uiManager			//!< UI manager object
)
{
	if (NULL != uiManager)
	{
		MainUIManager* mainUIManager = dynamic_cast<MainUIManager*>(uiManager);
		ASSERT(NULL != mainUIManager);
		if (NULL != mainUIManager)
		{
			mainUIManager->Destroy();
		}

		delete uiManager;
	}	
}

/**
 *	@brief	Shows main ui context menu.
 *	@param[in]	menuPos	position of the menu (in screen coordinates).
 */
void BeMainView::ShowMainUIContextMenu(Point32 menuPos)
{
	BMenu* subMenu;
	BPopUpMenu* popupMenu = new BPopUpMenu("context menu", false, false, B_ITEMS_IN_COLUMN);
	popupMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_COVER_BROWSER"), ID_COVER_BROWSER, 0, 0));
	popupMenu->AddSeparatorItem();
	subMenu = createSubMenu(TypeConv::AsUTF8("ID_RADIX"));
	popupMenu->AddItem(subMenu);
		subMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_RADIX_HEX"), ID_RADIX_HEX, 0, 0));
		subMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_RADIX_DECIMAL"), ID_RADIX_DECIMAL, 0, 0));
		subMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_RADIX_OCTAL"), ID_RADIX_OCTAL, 0, 0));
		subMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_RADIX_BINARY"), ID_RADIX_BINARY, 0, 0));
	popupMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_MAIN_ALWAYS_ON_TOP"), ID_MAIN_ALWAYS_ON_TOP, 0, 0));
	popupMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_MAIN_LOCK_POS"), ID_MAIN_LOCK_POS, 0, 0));
	popupMenu->AddSeparatorItem();
	popupMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_PREFERENCES"), ID_PREFERENCES, 0, 0));
	popupMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_MAIN_ABOUT_COVER"), ID_MAIN_ABOUT_COVER, 0, 0));
	popupMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_ABOUT"), ID_ABOUT, 0, 0));
	popupMenu->AddSeparatorItem();
	popupMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_MAIN_MINIMIZE"), ID_MAIN_MINIMIZE, 0, 0));
	popupMenu->AddItem(createMenuItem(TypeConv::AsUTF8("ID_MAIN_CLOSE"), ID_MAIN_CLOSE, 0, 0));

	try
	{
		showContextMenu(popupMenu, menuPos);
	}
	catch (...)
	{
		delete popupMenu;
		throw;
	}
	delete popupMenu;
}

/**
 *	@brief	Returns menu item's command.
 *	@param[in] menuID	menu ID.
 *	@return	command id is returned.
 */
SInt32 BeMainView::getMenuCommand(uint32 menuCommand)
{
	SInt32 commandID;

#define HANDLE_MENU_ID(menu, buttonClass)		\
	case menu:									\
		commandID = buttonClass;				\
		break;

	switch (menuCommand)
	{
		HANDLE_MENU_ID(ID_COVER_BROWSER, CoverMainWindowInfo::ButtonClass_ShowHideCoverBrowser)
		HANDLE_MENU_ID(ID_RADIX_HEX, CoverMainWindowInfo::ButtonClass_Hex)
		HANDLE_MENU_ID(ID_RADIX_DECIMAL, CoverMainWindowInfo::ButtonClass_Dec)
		HANDLE_MENU_ID(ID_RADIX_OCTAL, CoverMainWindowInfo::ButtonClass_Oct)
		HANDLE_MENU_ID(ID_RADIX_BINARY, CoverMainWindowInfo::ButtonClass_Bin)
		HANDLE_MENU_ID(ID_MAIN_ALWAYS_ON_TOP, CoverMainWindowInfo::ButtonClass_ToggleAlwaysOnTop)
		HANDLE_MENU_ID(ID_MAIN_LOCK_POS, CoverMainWindowInfo::ButtonClass_ToggleLockPos)
		HANDLE_MENU_ID(ID_PREFERENCES, CoverMainWindowInfo::ButtonClass_ShowPreferencesDialog)
		HANDLE_MENU_ID(ID_MAIN_ABOUT_COVER, CoverMainWindowInfo::ButtonClass_ShowCurrentCoverInfo)
		HANDLE_MENU_ID(ID_ABOUT, CoverMainWindowInfo::ButtonClass_About)
		HANDLE_MENU_ID(ID_MAIN_MINIMIZE, CoverMainWindowInfo::ButtonClass_Minimize)
		HANDLE_MENU_ID(ID_MAIN_CLOSE, CoverMainWindowInfo::ButtonClass_Close)
	
	default:
		commandID = UIManager::Command_None;
		break;
	}
	
	return commandID;
}

/**
 *	@brief	Shows "About" dialog.
 */
void BeMainView::ShowAboutDialog()
{
	if (NULL != aboutDialogMessenger && aboutDialogMessenger->IsValid())
	{
		aboutDialogMessenger->SendMessage(ID_DIALOG_ACTIVATE);
		return;
	}
	
	if (NULL != aboutDialogMessenger)
	{
		delete aboutDialogMessenger;
		aboutDialogMessenger = NULL;
	}

	const BeXMLLangFile* langFile = BeCoveredCalcApp::GetInstance()->GetLangFile();
	if (NULL == langFile)
	{
		throw new UIControllerExceptions::FailedToShowDialog(IDD_ABOUT);
	}
	
	BeDialogDesign* dialogDesign = langFile->LoadDialogDesign(IDD_ABOUT);
	try
	{
		BeAboutDialog* aboutDialog = new BeAboutDialog(dialogDesign);
		dialogDesign = NULL;
		aboutDialog->Init();
		aboutDialog->Show();
		aboutDialogMessenger = new BMessenger(aboutDialog);
	}
	catch (...)
	{
		if (NULL != dialogDesign)
			delete dialogDesign;
		throw;
	}
}

/**
 *	@brief	Shows "About Current Cover" dialog.
 */
void BeMainView::ShowAboutCurrentCoverDialog()
{
	if (NULL != aboutCCDialogMessenger && aboutCCDialogMessenger->IsValid())
	{
		aboutCCDialogMessenger->SendMessage(ID_DIALOG_ACTIVATE);
		return;
	}

	if (NULL != aboutCCDialogMessenger)
	{
		delete aboutCCDialogMessenger;
		aboutCCDialogMessenger = NULL;
	}

	const BeXMLLangFile* langFile = BeCoveredCalcApp::GetInstance()->GetLangFile();
	if (NULL == langFile)
	{
		throw new UIControllerExceptions::FailedToShowDialog(IDD_ABOUT_COVER);
	}
	
	BeDialogDesign* dialogDesign = langFile->LoadDialogDesign(IDD_ABOUT_COVER);
	try
	{
		BeAboutCurrentCoverDialog* aboutCCDialog = new BeAboutCurrentCoverDialog(dialogDesign);
		dialogDesign = NULL;
		aboutCCDialog->Init();
		aboutCCDialog->Show();
		aboutCCDialogMessenger = new BMessenger(aboutCCDialog);
	}
	catch (...)
	{
		if (NULL != dialogDesign)
			delete dialogDesign;
		throw;
	}
}

/**
 *	@brief	Shows "Preferences" dialog.
 */
void BeMainView::ShowPreferencesDialog()
{
	if (NULL != prefDialogMessenger && prefDialogMessenger->IsValid())
	{
		prefDialogMessenger->SendMessage(ID_DIALOG_ACTIVATE);
		return;
	}

	if (NULL != prefDialogMessenger)
	{
		delete prefDialogMessenger;
		prefDialogMessenger = NULL;
	}
	
	const BeXMLLangFile* langFile = BeCoveredCalcApp::GetInstance()->GetLangFile();
	if (NULL == langFile)
	{
		throw new UIControllerExceptions::FailedToShowDialog(IDD_PREFERENCES);
	}
	
	BeDialogDesign* dialogDesign = langFile->LoadDialogDesign(IDD_PREFERENCES);
	try
	{
		BePreferencesDlg* prefDialog = new BePreferencesDlg(dialogDesign);
		dialogDesign = NULL;
		prefDialog->Init();
		prefDialog->Show();
		prefDialogMessenger = new BMessenger(prefDialog);		
	}
	catch (...)
	{
		if (NULL != dialogDesign)
			delete dialogDesign;
		throw;
	}
}

/**
 *	@brief	Minimizes UI
 */
void BeMainView::Minimize()
{
	if (CoveredCalcApp::GetInstance()->GetAppSettings()->IsMainWindowAlwaysOnTop())
	{
		BWindow* window = Window();
		BAutolock locker(window);
		if (locker.IsLocked())
		{
			BAutolock locker2(deskbarWindow);
			if (locker2.IsLocked() && !deskbarWindow->IsHidden())
			{
				deskbarWindow->Hide();
			}
			window->SetFeel(B_NORMAL_WINDOW_FEEL);
		}
	}
	base::Minimize();
}

/**
 *	@brief	Set or reset the flag which means the window is always on top or not.
 */
void BeMainView::SetAlwaysOnTopFlag(
	bool isFlagOn			///< true if on top
)
{
	base::SetAlwaysOnTopFlag(isFlagOn);
	BAutolock locker(deskbarWindow);
	if (locker.IsLocked())
	{
		if (isFlagOn && deskbarWindow->IsHidden())
		{
			deskbarWindow->Show();
		}
		else if (!isFlagOn && !deskbarWindow->IsHidden())
		{
			deskbarWindow->Hide();
		}
	}
}

/**
 *	@brief	Called when main window is activated.
 */
void BeMainView::MainWindowActivated()
{
	if (CoveredCalcApp::GetInstance()->GetAppSettings()->IsMainWindowAlwaysOnTop())
	{
		BWindow* window = Window();
		BAutolock locker(window);
		if (locker.IsLocked() && B_FLOATING_ALL_WINDOW_FEEL != window->Feel())
		{
			window->SetFeel(B_FLOATING_ALL_WINDOW_FEEL);
		}
	
		BAutolock locker2(deskbarWindow);
		if (locker2.IsLocked() && deskbarWindow->IsHidden())
		{
			deskbarWindow->Show();
		}	
	}
}
