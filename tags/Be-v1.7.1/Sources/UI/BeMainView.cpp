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
	@file       BeMainView.cpp
	@brief      Implementation of BeMainView class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2004.02.18 created
*/

#include "Prefix.h"
#include <AppKit.h>
#include <InterfaceKit.h>
#include "MainUIManager.h"
#include "MenuInfo.h"
#include "BeMainView.h"
#include "BeCoveredCalcApp.h"
#include "DialogInfo.h"
#include "BeAboutDialog.h"
#include "UIControllerException.h"
#include "BeAboutCurrentCoverDialog.h"
#include "BeDialogDesign.h"
#include "BePreferencesDlg.h"
#include <support/Autolock.h>
#include "ExceptionMessageUtils.h"

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
		uiManager->Init(this);
		uiManager->Create(GetAppearance());
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
 *	@brief	Creates context menu.
 *	@param[in]	menuID	menu id.
 *	@return		context menu.
 */
BPopUpMenu* BeMainView::createContextMenu(SInt32 menuID)
{
	const BeXMLLangFile* langFile = BeCoveredCalcApp::GetInstance()->GetLangFile();
	switch (menuID)
	{
	case IDM_MAIN_CONTEXT:
		return createMainContextMenu(langFile);
		break;
		
	default:
		return base::createContextMenu(menuID);
		break;
	}
}

/**
 *	@brief	Creates main context menu.
 *	@param[in]	language file.
 *	@return		context menu.
 */
BPopUpMenu* BeMainView::createMainContextMenu(const BeXMLLangFile* langFile)
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
	return popupMenu;
}

// ---------------------------------------------------------------------
//! Shows dialog
// ---------------------------------------------------------------------
void BeMainView::ShowDialog(
	DialogInfo* dialogInfo		//!< information about the dialog.
)
{
	SInt32 dialogID = dialogInfo->GetDialogID();
	const BeXMLLangFile* langFile = BeCoveredCalcApp::GetInstance()->GetLangFile();
	if (NULL == langFile)
	{
		throw new UIControllerExceptions::FailedToShowDialog(dialogID);
	}

	BeDialogDesign* dialogDesign = NULL;

	try
	{
		switch (dialogID)
		{
		case IDD_ABOUT:
			{
				if (NULL != aboutDialogMessenger && aboutDialogMessenger->IsValid())
				{
					aboutDialogMessenger->SendMessage(ID_DIALOG_ACTIVATE);
				}
				else
				{
					dialogDesign = langFile->LoadDialogDesign(dialogID);
					BeAboutDialog* aboutDialog = new BeAboutDialog(dialogDesign);
					dialogDesign = NULL;
					aboutDialog->Init();
					aboutDialog->Show();
					if (NULL != aboutDialogMessenger)
						delete aboutDialogMessenger;
					aboutDialogMessenger = new BMessenger(aboutDialog);
				}
			}
			break;
		case IDD_ABOUT_COVER:
			{
				if (NULL != aboutCCDialogMessenger && aboutCCDialogMessenger->IsValid())
				{
					aboutCCDialogMessenger->SendMessage(ID_DIALOG_ACTIVATE);
				}
				else
				{
					dialogDesign = langFile->LoadDialogDesign(dialogID);
					BeAboutCurrentCoverDialog* aboutCCDialog = new BeAboutCurrentCoverDialog(dialogDesign);
					dialogDesign = NULL;
					aboutCCDialog->Init();
					aboutCCDialog->Show();
					if (NULL != aboutCCDialogMessenger)
						delete aboutCCDialogMessenger;
					aboutCCDialogMessenger = new BMessenger(aboutCCDialog);
				}
			}
			break;
		case IDD_PREFERENCES:
			{
				if (NULL != prefDialogMessenger && prefDialogMessenger->IsValid())
				{
					prefDialogMessenger->SendMessage(ID_DIALOG_ACTIVATE);
				}
				else
				{
					dialogDesign = langFile->LoadDialogDesign(dialogID);
					BePreferencesDlg* prefDialog = new BePreferencesDlg(dialogDesign);
					dialogDesign = NULL;
					prefDialog->Init();
					prefDialog->Show();
					if (NULL != prefDialogMessenger)
						delete prefDialogMessenger;
					prefDialogMessenger = new BMessenger(prefDialog);
				}
			}
			break;			
		default:
			base::ShowDialog(dialogInfo);
			break;
		}
	}
	catch (...)
	{
		if (NULL != dialogDesign)
			delete dialogDesign;
		
		throw;
	}
	
	if (NULL != dialogDesign)
	{
		delete dialogDesign;
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

// ---------------------------------------------------------------------
//! Message handling
// ---------------------------------------------------------------------
void BeMainView::MessageReceived(
	BMessage* msg				//!< received message
)
{
	UIManager* uiManager = getUIManager();
	MainUIManager* mainUIManager = dynamic_cast<MainUIManager*>(uiManager);
	if (mainUIManager == NULL)
	{
		base::MessageReceived(msg);
		return;
	}

	try
	{
		switch (msg->what)
		{
		case ID_COVER_BROWSER:
			mainUIManager->DoFuncCoverBrowser();
			break;
		case ID_RADIX_HEX:
			mainUIManager->DoFuncChangeRadix(CalcCore::DigitForm_16);
			break;
		case ID_RADIX_DECIMAL:
			mainUIManager->DoFuncChangeRadix(CalcCore::DigitForm_10);
			break;
		case ID_RADIX_OCTAL:
			mainUIManager->DoFuncChangeRadix(CalcCore::DigitForm_8);
			break;
		case ID_RADIX_BINARY:
			mainUIManager->DoFuncChangeRadix(CalcCore::DigitForm_2);
			break;	
		case ID_MAIN_ALWAYS_ON_TOP:
			mainUIManager->DoFuncMainWindowAlwaysOnTop();
			break;
		case ID_MAIN_LOCK_POS:
			mainUIManager->DoFuncMainWindowLockPos();
			break;
		case ID_PREFERENCES:
			mainUIManager->DoFuncPreferences();
			break;
		case ID_ABOUT:
			mainUIManager->DoFuncAbout();
			break;
		case ID_MAIN_ABOUT_COVER:
			mainUIManager->DoFuncAboutCurrentCover();
			break;
		case ID_MAIN_MINIMIZE:
			mainUIManager->DoFuncMainMinimize();
			break;
		case ID_MAIN_CLOSE:
			mainUIManager->DoFuncClose();
			break;
		default:
			base::MessageReceived(msg);
			break;
		}
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
		ex->Delete();		
	}	
}	
