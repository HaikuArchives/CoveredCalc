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
	@file		MainUIManager.cpp
	@brief		Implementation of MainUIManager class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.27 created
*/

#include "Prefix.h"
#include "CoveredCalcApp.h"
#include "ColorCodedSkin.h"
#include "CoverManager.h"
#include "CoverDef.h"
#include "CoverInfo.h"
#include "CoverFontInfo.h"
#include "CoverButtonInfo.h"
#include "CoverMainWindowInfo.h"
#include "CoverDisplayInfo.h"
#include "MovingUITask.h"
#include "ButtonUITask.h"
#include "MouseHoverUITask.h"
#include "AppSettings.h"
#include "UIMessageProvider.h"
#include "ExceptionMessageUtils.h"
#include "MainUIManager.h"
#include "UTF8Conv.h"
#include "CoverToolTipInfo.h"
#include "BMPFileStore.h"
#include "UITaskClass.h"
#include "MainUIController.h"

////////////////////////////////////////
#define base	UIManager
////////////////////////////////////////

/// Index of state skins in generated main window skin object
enum SkinIndex
{
	SkinIndex_BackgroundActive = 0,		///< active background
	SkinIndex_BackgroundInactive,		///< inactive background
	SkinIndex_Pressed,					///< pressed
	SkinIndex_GrayedActive,				///< active grayed (disabled)
	SkinIndex_GrayedInactive,			///< inactive grayed (disabled)
	SkinIndex_CheckedActive,			///< active checked
	SkinIndex_CheckedInactive,			///< inactive checked
	SkinIndex_CheckedPressed,			///< checked and pressed
	SkinIndex_HotActive,				///< active hot
	SkinIndex_HotInactive,				///< inactive hot
	SkinIndex_CheckedHotActive,			///< active checked and hot
	SkinIndex_CheckedHotInactive,		///< inactive checked and hot
	
	SkinIndexCount						///< number of SkinIndex enum value.
};

/// Index of other images in generated main window skin object
enum OtherImageIndex
{
	OtherImageIndex_Font = 0,			///< font
	
	OtherImageIndexCount				///< number of OtherImageIndex enum value.
};

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
MainUIManager::MainUIManager()
{
	mainUIController = NULL;
	currentDigitForm = CalcCore::DigitForm_10;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
MainUIManager::~MainUIManager()
{
	clearMembers();
}

// ---------------------------------------------------------------------
//! Clear all members
// ---------------------------------------------------------------------
void MainUIManager::clearMembers()
{
	base::clearMembers();
}

// ---------------------------------------------------------------------
//! Initializes the object.
// ---------------------------------------------------------------------
/**
 *	@brief	Initializes the object.
 *	@param[in]	uiController		UI controller object
 *	@param[in]	mainUIController	main UI controller object
 *	@param[in	keyMappingManager	key-mapping manager
 */
void MainUIManager::Init(UIController* uiController, MainUIController* mainUIController, const KeyMappingManager* keyMappingManager)
{
	base::Init(uiController, keyMappingManager);
	this->mainUIController = mainUIController;
}

// ---------------------------------------------------------------------
//! Readies user interface.
// ---------------------------------------------------------------------
void MainUIManager::Create()
{
	clearMembers();

	// read current skin
	readSkin();
	
	// register CoverChangedEventHandler
	CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
	manager->RegisterCoverChangeEventHandler(this);
}

// ---------------------------------------------------------------------
//! Destroys user interface.
// ---------------------------------------------------------------------
void MainUIManager::Destroy()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();

	// save current location
	Rect32 uiRect;
	GetUIController()->GetUIRect(uiRect);
	const Point32& basePoint = getBasePoint();
	Point32 windowPos;
	windowPos.x = uiRect.left + basePoint.x;
	windowPos.y = uiRect.top + basePoint.y;
	AppSettings* appSettings = app->GetAppSettings();
	appSettings->SetLastMainWindowPos(windowPos);
	try
	{
		appSettings->Save();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBoxWithText(app, ex, IDS_EMSG_SAVE_SETTING);
		ex->Delete();		
	}

	// unregister CoverChangedEventHandler
	CoverManager* manager = app->GetCoverManager();
	manager->UnregisterCoverChangeEventHandler(this);
	
	base::Destroy();
}

// ---------------------------------------------------------------------
//! Called when cover definition is changed.
// ---------------------------------------------------------------------
void MainUIManager::CoverDefChanged()
{
	readSkin();
}

// ---------------------------------------------------------------------
//! Called when current cover number is changed.
// ---------------------------------------------------------------------
void MainUIManager::CurrentCoverChanged()
{
	readSkin();
}

// ---------------------------------------------------------------------
//! Reads current skin and reflect it to user interface
// ---------------------------------------------------------------------
void MainUIManager::readSkin()
{
	UIController* uiController = GetUIController();
	
	if (uiController->IsUIMinimized())
	{
		uiController->Restore();
	}
	uiController->Hide();
	
	try
	{
		const CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
		ASSERT(NULL != manager);
		SInt32 currentCover = manager->GetCurrentCover();
		const CoverMainWindowInfo* mainWindowInfo = manager->GetCoverDef()->GetCoverInfo(currentCover)->GetMainWindowInfo();
		ASSERT(NULL != mainWindowInfo);

		// create skin object
		ColorCodedSkin* skin = NULL;
		ColorCodedSkinAppearance* appearance = NULL;
		const Path& coverFilePath = manager->GetCoverDefFilePath();
		const Path coverFileParent = coverFilePath.GetParent();

		BMPFileStore* bmpFileStore = new BMPFileStore();
		DIBitmap* mapBitmap = NULL;
		ConstDIBitmapVector* stateSkins = NULL;
		ConstDIBitmapVector* otherImages = NULL;
		ColorAreaMap* colorAreas = NULL;
		try
		{
			Path imagePath;
			SInt32 index;
			
			// map bitmap
			imagePath = mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_ColorMap);
			ASSERT(!imagePath.IsEmpty());
			mapBitmap = bmpFileStore->Load(coverFileParent.Append(imagePath));
			
			// state image bitmaps
			stateSkins = new ConstDIBitmapVector();
			for (index = 0; index < SkinIndexCount; index++)
			{
				imagePath = getImageFilePath(mainWindowInfo, index);
				ASSERT(!imagePath.IsEmpty());
				stateSkins->push_back(bmpFileStore->Load(coverFileParent.Append(imagePath)));
			}

			// other images
			otherImages = new ConstDIBitmapVector();
			for (index = 0; index < OtherImageIndexCount; index++)
			{
				switch (index)
				{
				case OtherImageIndex_Font:
					imagePath = mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_Font);
					break;
				default:
					ASSERT(false);
					break;
				}

				ASSERT(!imagePath.IsEmpty());
				otherImages->push_back(bmpFileStore->Load(coverFileParent.Append(imagePath)));
			}

			// color area map			
			ColorAreaMap* colorAreas = new ColorAreaMap();
			SInt32 maxCount = mainWindowInfo->GetNumMapItems();
			for (index = 0; index < maxCount; index++)
			{
				const ColorValue& color = mainWindowInfo->GetMapItem(index);
				(*colorAreas)[color] = index;
			}

			// now create a skin object
			skin = new ColorCodedSkin();
			try
			{
				skin->Init(bmpFileStore, mapBitmap, stateSkins, otherImages, colorAreas);
			}
			catch (...)
			{
				delete skin;
				throw;
			}		
		}
		catch (...)
		{
			delete bmpFileStore;
			if (NULL != stateSkins)
				delete stateSkins;
			if (NULL != otherImages)
				delete otherImages;
			if (NULL != colorAreas)
				delete colorAreas;
			throw;
		}

		setSkinAppearance(NULL);
		appearance = uiController->InitSkinAppearance();
		skin->AttachAppearance(appearance);
		setSkinAppearance(appearance);
		setSkin(skin);

		// set base point and move user interface
		setBasePoint(mainWindowInfo->GetBasePoint());
		
		// clip transparent region of the skin
		SInt32 transparentIndex = mainWindowInfo->GetTransparentRegionIndex();
		if (-1 != transparentIndex)
		{
			ColorValue transparentColor = mainWindowInfo->GetMapItem(transparentIndex);
			skin->ClipSkinRegion(transparentColor);
		}
		else
		{
			skin->UnclipSkinRegion();
		}
	}
	catch (...)
	{
		uiController->Show();
		throw;
	}
	uiController->Show();
		
	// make the first time draw
	updateWholeAppearance();

	// initialize calc engine
	reinitCalcCore();
}

/**
 *	@brief	paints whole UI
 */
void MainUIManager::updateWholeAppearance()
{
	ColorCodedSkin* skin = GetSkin();
	if (NULL != skin)
	{
		bool isUIActive = GetUIController()->IsUIActive();

		SInt32 backgroundSkinIndex;
		SInt32 grayedSkinIndex;
		SInt32 checkedSkinIndex;
		if (isUIActive)
		{
			backgroundSkinIndex = SkinIndex_BackgroundActive;
			grayedSkinIndex = SkinIndex_GrayedActive;
			checkedSkinIndex = SkinIndex_CheckedActive;
		}
		else
		{
			backgroundSkinIndex = SkinIndex_BackgroundInactive;
			grayedSkinIndex = SkinIndex_GrayedInactive;
			checkedSkinIndex = SkinIndex_CheckedInactive;
		}

		// background
		skin->UpdateWholeAppearance(backgroundSkinIndex);

		// each non-normal area
		SInt32 commandId;
		for (commandId = 0; commandId < CoverMainWindowInfo::ButtonClass_Max; commandId++)
		{
			UInt32 state = GetCommandState(commandId);
			if (state & CommandState_Disabled)
			{
				SInt32 areaId = GetSkinAreaFromCommandId(commandId);
				if (ColorCodedSkin::Area_None != areaId)
				{
					skin->UpdatePartAppearance(areaId, grayedSkinIndex);
				}
			}
			else if (state & CommandState_Checked)
			{
				SInt32 areaId = GetSkinAreaFromCommandId(commandId);
				if (ColorCodedSkin::Area_None != areaId)
				{
					skin->UpdatePartAppearance(areaId, checkedSkinIndex);
				}
			}
		}
	}
	drawNumberStringOnDisplay(numberStringOnDisplay);
}

const Path& MainUIManager::getImageFilePath(const CoverMainWindowInfo* mainWindowInfo, SInt32 skinIndex)
{
	const Path* imagePath = NULL;
	switch (skinIndex)
	{
	case SkinIndex_BackgroundActive:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_BackgroundActive));
		break;
	case SkinIndex_BackgroundInactive:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_BackgroundInactive));
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_BackgroundActive));
		}
		break;
	case SkinIndex_Pressed:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_Pressed));
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_BackgroundActive));
		}
		break;
	case SkinIndex_GrayedActive:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_GrayedActive));
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_BackgroundActive));
		}
		break;
	case SkinIndex_GrayedInactive:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_GrayedInactive));
		if (imagePath->IsEmpty())
		{
			imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_GrayedActive));
		}
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_BackgroundInactive));
		}
		break;
	case SkinIndex_CheckedActive:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedActive));
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_BackgroundActive));
		}
		break;
	case SkinIndex_CheckedInactive:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedInactive));
		if (imagePath->IsEmpty())
		{
			imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedActive));
		}
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_BackgroundInactive));
		}
		break;
	case SkinIndex_CheckedPressed:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedPressed));
		if (imagePath->IsEmpty())
		{
			imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedActive));
		}
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_Pressed));
		}
		break;
	case SkinIndex_HotActive:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_HotActive));
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_BackgroundActive));
		}
		break;
	case SkinIndex_HotInactive:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_HotInactive));
		if (imagePath->IsEmpty())
		{
			imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_HotActive));
		}
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_BackgroundInactive));
		}
		break;
	case SkinIndex_CheckedHotActive:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedHotActive));
		if (imagePath->IsEmpty())
		{
			imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedActive));
		}
		if (imagePath->IsEmpty())
		{
			imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_HotActive));
		}
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_BackgroundActive));
		}
		break;
	case SkinIndex_CheckedHotInactive:
		imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedHotInactive));
		if (imagePath->IsEmpty())
		{
			imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedHotActive));
		}
		if (imagePath->IsEmpty())
		{
			imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedInactive));
		}
		if (imagePath->IsEmpty())
		{
			imagePath = &(mainWindowInfo->GetImageFilePath(CoverMainWindowInfo::ImageClass_CheckedActive));
		}
		if (imagePath->IsEmpty())
		{
			imagePath = &(getImageFilePath(mainWindowInfo, SkinIndex_HotInactive));
		}
		break;
	default:
		ASSERT(false);
		break;
	}

	return *imagePath;
}



// ---------------------------------------------------------------------
//! Reinitializes calc's core object.
// ---------------------------------------------------------------------
void MainUIManager::reinitCalcCore()
{
	const CoverMainWindowInfo* mainWindowInfo = getCurrentMainWindowInfo();

	// get display length
	UInt32 maxDispLength = 0;
	const CoverFontInfo* fontInfo = mainWindowInfo->GetFontInfo();
	SInt32 displayMapIndex = mainWindowInfo->GetDisplayInfo()->GetMapIndex();
	Rect32 displayRect;
	ColorCodedSkin* skin = getSkin();
	skin->GetAreaRect(displayMapIndex, displayRect);
	if (displayRect.bottom - displayRect.top + 1 >= fontInfo->GetHeight())
	{
		SInt32 drawPointX;
		drawPointX = displayRect.right - fontInfo->GetWidth();
		while (drawPointX >= displayRect.left)
		{
			maxDispLength++;
			drawPointX -= fontInfo->GetWidth() + fontInfo->GetCharSpace();
		}
	}
	
	// reinit
	calcCore.Init(this, maxDispLength);
}


// ---------------------------------------------------------------------
//! Called when user interface is activated or deactivated.
// ---------------------------------------------------------------------
void MainUIManager::UIActivated()
{
	ColorCodedSkin* skin = getSkin();
	if (NULL != skin)
	{
		updateWholeAppearance();
	}

	base::UIActivated();	
}

/**
 *	@brief	Called when mouse cursor position is changed.
 */
void MainUIManager::MouseMove()
{
	base::MouseMove();

	bool doHoverTask;
#if defined(BEOS)
	doHoverTask = GetUIController()->IsUIActive();
#else
	doHoverTask = true;
#endif

	if (doHoverTask)
	{
		UITaskManager* uiTaskManager = getTaskManager();
		UITask* task = uiTaskManager->FindOneTask(UITaskClass::ButtonUITask);
		if (NULL == task)
		{
			task = uiTaskManager->FindOneTask(UITaskClass::MovingUITask);
		}
		if (NULL == task)
		{
			Point32 startPoint = GetUIController()->GetMousePosition();

			// hit test for button item
			ColorCodedSkin* skin = getSkin();
			SInt32 skinArea = skin->GetArea(startPoint);
			if (ColorCodedSkin::Area_None != skinArea)
			{
				// checks if hover task is already exist.
				bool isExist = false;
				UITaskVector hoverTasks;
				getTaskManager()->FindAllTask(UITaskClass::MouseHoverUITask, hoverTasks);
				UITaskVector::const_iterator ite;
				for (ite = hoverTasks.begin(); ite != hoverTasks.end(); ite++)
				{
					MouseHoverUITask* task = dynamic_cast<MouseHoverUITask*>(*ite);
					if (task->GetTargetArea() == skinArea)
					{
						isExist = true;
						break;
					}
				}

				if (!isExist)
				{
					const CoverMainWindowInfo* mainWindowInfo = getCurrentMainWindowInfo();
					CoverMainWindowInfo::ButtonClass command = mainWindowInfo->FindButton(skinArea);
					if (CoverMainWindowInfo::ButtonClass_None != command)
					{
						UInt32 commandState = GetCommandState(command);
						if (!(commandState & CommandState_Disabled))
						{
							SInt32 activeImageIndex = SkinIndex_BackgroundActive;
							SInt32 inactiveImageIndex = SkinIndex_BackgroundInactive;
							SInt32 hotActiveImageIndex = SkinIndex_HotActive;
							SInt32 hotInactiveImageIndex = SkinIndex_HotInactive;
							if (commandState & CommandState_Checked)
							{
								activeImageIndex = SkinIndex_CheckedActive;
								inactiveImageIndex = SkinIndex_CheckedInactive;
								hotActiveImageIndex = SkinIndex_CheckedHotActive;
								hotInactiveImageIndex = SkinIndex_CheckedHotInactive;
							}

							MouseHoverUITask* newTask = new MouseHoverUITask();
							newTask->Init(this);
							newTask->SetImageIndex(activeImageIndex, inactiveImageIndex, hotActiveImageIndex, hotInactiveImageIndex);
							newTask->SetInitialInformation(command, mainWindowInfo->GetHottingSteps());
							StartTask(newTask);
						}
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------
//! Called when mouse button 1 is pressed.
// ---------------------------------------------------------------------
void MainUIManager::Button1Down()
{
	base::Button1Down();

	UITaskManager* uiTaskManager = getTaskManager();
	UITask* task = uiTaskManager->FindOneTask(UITaskClass::ButtonUITask);
	if (NULL == task)
	{
		task = uiTaskManager->FindOneTask(UITaskClass::MovingUITask);
	}
	if (NULL == task)
	{
		Point32 startPoint = GetUIController()->GetMousePosition();
		
		UITask* newTask = NULL;
		
		// hit test for button item
		ColorCodedSkin* skin = getSkin();
		SInt32 skinArea = skin->GetArea(startPoint);
		if (ColorCodedSkin::Area_None != skinArea)
		{
			const CoverMainWindowInfo* mainWindowInfo = getCurrentMainWindowInfo();
			CoverMainWindowInfo::ButtonClass command = mainWindowInfo->FindButton(skinArea);
			if (CoverMainWindowInfo::ButtonClass_None != command)
			{
				UInt32 commandState = GetCommandState(command);
				if (!(commandState & CommandState_Disabled))
				{
					// cancels hover task of this button.
					UITaskVector hoverTasks;
					getTaskManager()->FindAllTask(UITaskClass::MouseHoverUITask, hoverTasks);
					UITaskVector::iterator ite;
					for (ite = hoverTasks.begin(); ite != hoverTasks.end(); ite++)
					{
						MouseHoverUITask* task = dynamic_cast<MouseHoverUITask*>(*ite);
						if (task->GetTargetArea() == skinArea)
						{
							task->CancelTask();
						}
					}

					ButtonUITask* buttonUITask = new ButtonUITask();
					try
					{
						buttonUITask->Init(this);
						buttonUITask->SetEventHandler(this);

						SInt32 activeImageIndex = SkinIndex_BackgroundActive;
						SInt32 inactiveImageIndex = SkinIndex_BackgroundInactive;
						SInt32 pressedImageIndex = SkinIndex_Pressed;
						if (commandState & CommandState_Checked)
						{
							activeImageIndex = SkinIndex_CheckedActive;
							inactiveImageIndex = SkinIndex_CheckedInactive;
							pressedImageIndex = SkinIndex_CheckedPressed;
						}

						buttonUITask->SetImageIndex(	activeImageIndex,
														inactiveImageIndex,
														pressedImageIndex);
						buttonUITask->SetInitialInformation(command);
					}
					catch (...)
					{
						delete buttonUITask;
						throw;
					}
					newTask = buttonUITask;
				}
			}
		}

		if (NULL == newTask &&
				!CoveredCalcApp::GetInstance()->GetAppSettings()->IsMainWindowLocked())
		{
			// moving task
			MovingUITask* movingUITask = new MovingUITask();
			try
			{
				movingUITask->Init(this);		
			}
			catch (...)
			{
				delete movingUITask;
				throw;
			}
			newTask = movingUITask;
		}
		
		if (NULL != newTask)
		{
			StartTask(newTask);
		}
	}
}

// ---------------------------------------------------------------------
//! Converts key event parameter to button class
// ---------------------------------------------------------------------
CoverMainWindowInfo::ButtonClass
MainUIManager::getButtonFromKeyFunc(
	MainWindowKeyFunc::KeyFunc keyFunc
)
{
#define HANDLE_KEYFUNC(_keyFunc, _buttonClass)			\
		case _keyFunc:									\
			buttonClass = _buttonClass;					\
			break;

	CoverMainWindowInfo::ButtonClass buttonClass;

	switch (keyFunc)
	{
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_0,						CoverMainWindowInfo::ButtonClass_0)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_1,						CoverMainWindowInfo::ButtonClass_1)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_2,						CoverMainWindowInfo::ButtonClass_2)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_3,						CoverMainWindowInfo::ButtonClass_3)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_4,						CoverMainWindowInfo::ButtonClass_4)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_5,						CoverMainWindowInfo::ButtonClass_5)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_6,						CoverMainWindowInfo::ButtonClass_6)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_7,						CoverMainWindowInfo::ButtonClass_7)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_8,						CoverMainWindowInfo::ButtonClass_8)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_9,						CoverMainWindowInfo::ButtonClass_9)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_A,						CoverMainWindowInfo::ButtonClass_A)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_B,						CoverMainWindowInfo::ButtonClass_B)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_C,						CoverMainWindowInfo::ButtonClass_C)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_D,						CoverMainWindowInfo::ButtonClass_D)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_E,						CoverMainWindowInfo::ButtonClass_E)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_F,						CoverMainWindowInfo::ButtonClass_F)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Point,					CoverMainWindowInfo::ButtonClass_Point)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Clear,					CoverMainWindowInfo::ButtonClass_Clear)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_BS,						CoverMainWindowInfo::ButtonClass_BS)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Equal,					CoverMainWindowInfo::ButtonClass_Equal)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Plus,						CoverMainWindowInfo::ButtonClass_Plus)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Minus,					CoverMainWindowInfo::ButtonClass_Minus)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Times,					CoverMainWindowInfo::ButtonClass_Times)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Div,						CoverMainWindowInfo::ButtonClass_Div)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Negate,					CoverMainWindowInfo::ButtonClass_Negate)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Hex,						CoverMainWindowInfo::ButtonClass_Hex)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Dec,						CoverMainWindowInfo::ButtonClass_Dec)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Oct,						CoverMainWindowInfo::ButtonClass_Oct)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Bin,						CoverMainWindowInfo::ButtonClass_Bin)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Minimize,					CoverMainWindowInfo::ButtonClass_Minimize)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_Close,					CoverMainWindowInfo::ButtonClass_Close)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_ShowHideCoverBrowser,		CoverMainWindowInfo::ButtonClass_ShowHideCoverBrowser)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_ToggleAlwaysOnTop,		CoverMainWindowInfo::ButtonClass_ToggleAlwaysOnTop)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_ToggleLockPos,			CoverMainWindowInfo::ButtonClass_ToggleLockPos)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_ShowPreferencesDialog,	CoverMainWindowInfo::ButtonClass_ShowPreferencesDialog)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_ShowCurrentCoverInfo,		CoverMainWindowInfo::ButtonClass_ShowCurrentCoverInfo)
	HANDLE_KEYFUNC(MainWindowKeyFunc::KeyFunc_About,					CoverMainWindowInfo::ButtonClass_About)

	default:
		buttonClass = CoverMainWindowInfo::ButtonClass_None;
		break;		
	}

	return buttonClass;

#undef HANDLE_KEYFUNC
}

// ---------------------------------------------------------------------
//! Converts command id (on this UI) to area id on the skin.
/*!
	@return area id on the skin
*/
// ---------------------------------------------------------------------
SInt32 MainUIManager::GetSkinAreaFromCommandId(
	SInt32 commandId								//!< command id on this UI
)
{
	SInt32 retValue = ColorCodedSkin::Area_None;
	
	if (commandId != CoverMainWindowInfo::ButtonClass_None)
	{
		const CoverButtonInfo* buttonInfo = getCurrentMainWindowInfo()->GetButtonInfo(static_cast<CoverMainWindowInfo::ButtonClass>(commandId));
		if (NULL != buttonInfo)
		{
			retValue = buttonInfo->GetMapIndex();
		}
	}
	
	return retValue;
}

/**
 *	@brief		Returns state of specified command.
 *	@param[in]	commandId	command ID
 *	@return		command state. it is a combination of UIManager::CommandState_xxx.
 */
UInt32 MainUIManager::GetCommandState(SInt32 commandId)
{
	UInt32 state = UIManager::CommandState_None;

	switch (commandId)
	{
	case CoverMainWindowInfo::ButtonClass_F:
	case CoverMainWindowInfo::ButtonClass_E:
	case CoverMainWindowInfo::ButtonClass_D:
	case CoverMainWindowInfo::ButtonClass_C:
	case CoverMainWindowInfo::ButtonClass_B:
	case CoverMainWindowInfo::ButtonClass_A:
		if (CalcCore::DigitForm_16 != currentDigitForm)
		{
			state |= UIManager::CommandState_Disabled; 
		}
		break;
	case CoverMainWindowInfo::ButtonClass_9:
	case CoverMainWindowInfo::ButtonClass_8:
		if (CalcCore::DigitForm_16 != currentDigitForm
				&& CalcCore::DigitForm_10 != currentDigitForm)
		{
			state |= UIManager::CommandState_Disabled; 
		}
		break;
	case CoverMainWindowInfo::ButtonClass_7:
	case CoverMainWindowInfo::ButtonClass_6:
	case CoverMainWindowInfo::ButtonClass_5:
	case CoverMainWindowInfo::ButtonClass_4:
	case CoverMainWindowInfo::ButtonClass_3:
	case CoverMainWindowInfo::ButtonClass_2:
		if (CalcCore::DigitForm_16 != currentDigitForm
				&& CalcCore::DigitForm_10 != currentDigitForm
				&& CalcCore::DigitForm_8 != currentDigitForm)
		{
			state |= UIManager::CommandState_Disabled; 
		}
		break;
	case CoverMainWindowInfo::ButtonClass_Hex:
		state |= UIManager::CommandState_Radio;
		if (CalcCore::DigitForm_16 == currentDigitForm)
		{
			state |= UIManager::CommandState_Checked;
		}
		break;
	case CoverMainWindowInfo::ButtonClass_Dec:
		state |= UIManager::CommandState_Radio;
		if (CalcCore::DigitForm_10 == currentDigitForm)
		{
			state |= UIManager::CommandState_Checked;
		}
		break;
	case CoverMainWindowInfo::ButtonClass_Oct:
		state |= UIManager::CommandState_Radio;
		if (CalcCore::DigitForm_8 == currentDigitForm)
		{
			state |= UIManager::CommandState_Checked;
		}
		break;
	case CoverMainWindowInfo::ButtonClass_Bin:
		state |= UIManager::CommandState_Radio;
		if (CalcCore::DigitForm_2 == currentDigitForm)
		{
			state |= UIManager::CommandState_Checked;
		}
		break;
	case CoverMainWindowInfo::ButtonClass_ShowHideCoverBrowser:
		if (CoveredCalcApp::GetInstance()->IsCoverBrowserVisible())
		{
			state |= UIManager::CommandState_Checked;
		}	
		break;
	case CoverMainWindowInfo::ButtonClass_ToggleAlwaysOnTop:
		if (CoveredCalcApp::GetInstance()->GetAppSettings()->IsMainWindowAlwaysOnTop())
		{
			state |= UIManager::CommandState_Checked;
		}
		break;
	case CoverMainWindowInfo::ButtonClass_ToggleLockPos:
		if (CoveredCalcApp::GetInstance()->GetAppSettings()->IsMainWindowLocked())
		{
			state |= UIManager::CommandState_Checked;
		}
		break;
	}

	return state;
}

/**
 *	@brief	Executes a command.
 *	@param[in]	commandId	command ID
 */
void MainUIManager::ExecuteCommand(SInt32 commandId)
{
	CoverMainWindowInfo::ButtonClass button = static_cast<CoverMainWindowInfo::ButtonClass>(commandId);
	ASSERT(CoverMainWindowInfo::ButtonClass_None != button);

#define HANDLE_CALCKEY(buttonClass, calcKey)				\
		case buttonClass:									\
			calcCore.Put(calcKey);							\
			break;

	switch (button)
	{
	case CoverMainWindowInfo::ButtonClass_Close:
		doCommandClose();
		break;
	
	case CoverMainWindowInfo::ButtonClass_Minimize:
		doCommandMainMinimize();
		break;

	case CoverMainWindowInfo::ButtonClass_ShowHideCoverBrowser:
		doCommandCoverBrowser();
		break;

	case CoverMainWindowInfo::ButtonClass_ToggleAlwaysOnTop:
		doCommandMainWindowAlwaysOnTop();
		break;

	case CoverMainWindowInfo::ButtonClass_ToggleLockPos:
		doCommandMainWindowLockPos();
		break;

	case CoverMainWindowInfo::ButtonClass_ShowPreferencesDialog:
		doCommandPreferences();
		break;

	case CoverMainWindowInfo::ButtonClass_ShowCurrentCoverInfo:
		doCommandAboutCurrentCover();
		break;

	case CoverMainWindowInfo::ButtonClass_About:
		doCommandAbout();
		break;

	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_0,		CalcCore::Key_0)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_1,		CalcCore::Key_1)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_2,		CalcCore::Key_2)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_3,		CalcCore::Key_3)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_4,		CalcCore::Key_4)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_5,		CalcCore::Key_5)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_6,		CalcCore::Key_6)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_7,		CalcCore::Key_7)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_8,		CalcCore::Key_8)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_9,		CalcCore::Key_9)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_A,		CalcCore::Key_A)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_B,		CalcCore::Key_B)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_C,		CalcCore::Key_C)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_D,		CalcCore::Key_D)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_E,		CalcCore::Key_E)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_F,		CalcCore::Key_F)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Point,	CalcCore::Key_Point)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Clear,	CalcCore::Key_Clear)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_BS,		CalcCore::Key_BS)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Equal,	CalcCore::Key_Equal)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Plus,	CalcCore::Key_Plus)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Minus,	CalcCore::Key_Minus)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Times,	CalcCore::Key_Times)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Div,	CalcCore::Key_Div)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Negate, CalcCore::Key_Negate)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Hex,	CalcCore::Key_DigitForm16)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Dec,	CalcCore::Key_DigitForm10)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Oct,	CalcCore::Key_DigitForm8)
	HANDLE_CALCKEY(CoverMainWindowInfo::ButtonClass_Bin,	CalcCore::Key_DigitForm2)

	default:
		break;
	}

#undef HANDLE_CALCKEY
}

/**
 *	@brief	Called when a key is pressed.
 *	@return	returns false to skip other behavior assigned to this key-event.
 */
bool MainUIManager::KeyDown(const KeyEventParameter& parameter)
{
	bool ret = base::KeyDown(parameter);
	
	SInt32 function = getKeyMappingManager()->GetFunction(parameter);
	CoverMainWindowInfo::ButtonClass command = getButtonFromKeyFunc(static_cast<MainWindowKeyFunc::KeyFunc>(function));
	if (CoverMainWindowInfo::ButtonClass_None != command)
	{
		UInt32 commandState = GetCommandState(command);
		if (!(commandState & CommandState_Disabled))
		{
			ColorCodedSkin* skin = getSkin();
			UIController* uiController = getUIController();
			SInt32 skinArea = GetSkinAreaFromCommandId(command);
			if (ColorCodedSkin::Area_None != skinArea)
			{
				// cancels hover task of this key.
				UITaskVector hoverTasks;
				getTaskManager()->FindAllTask(UITaskClass::MouseHoverUITask, hoverTasks);
				UITaskVector::iterator ite;
				for (ite = hoverTasks.begin(); ite != hoverTasks.end(); ite++)
				{
					MouseHoverUITask* task = dynamic_cast<MouseHoverUITask*>(*ite);
					if (task->GetTargetArea() == skinArea)
					{
						task->CancelTask();
					}
				}

				// update appearance
				SInt32 pressedImageIndex;
				SInt32 restoreImageIndex;
				if (commandState & CommandState_Checked)
				{
					pressedImageIndex = SkinIndex_CheckedPressed;
					if (uiController->IsUIActive())
						restoreImageIndex = SkinIndex_CheckedActive;
					else
						restoreImageIndex = SkinIndex_CheckedInactive;
				}
				else
				{
					pressedImageIndex = SkinIndex_Pressed;
					if (uiController->IsUIActive())
						restoreImageIndex = SkinIndex_BackgroundActive;
					else
						restoreImageIndex = SkinIndex_BackgroundInactive;
				}

				skin->UpdatePartAppearance(skinArea, pressedImageIndex);
				uiController->UpdateUI();
				uiController->Wait(CoveredCalcApp::GetInstance()->GetAppSettings()->GetKeyAnimationWait());
				skin->UpdatePartAppearance(skinArea, restoreImageIndex);
			}
			ButtonClicked(command);
		}
	}
	else
	{
		// no button matches this key, but the function of this key can exsits.
	}

	if (ret)
	{
		ret = KeyFuncOperation::KeyFunc_None == function;
	}
	
	return ret;
}

// ---------------------------------------------------------------------
//! Called when mouse button 2 is pressed.
// ---------------------------------------------------------------------
void MainUIManager::Button2Down()
{
	base::Button2Down();
	
	UIController* uiController = GetUIController();
	Point32 position = uiController->GetMouseScreenPosition();
	GetMainUIController()->ShowMainUIContextMenu(position);
}

// ---------------------------------------------------------------------
//! Called when user execute "Cover Browser" command.
// ---------------------------------------------------------------------
void MainUIManager::doCommandCoverBrowser()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();
	bool isShown = app->IsCoverBrowserVisible();
	app->ShowCoverBrowser(!isShown);
	updateWholeAppearance();
}

/**
 *	@brief	Called when user execute "Always on top" command.
 */
void MainUIManager::doCommandMainWindowAlwaysOnTop()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();
	AppSettings* appSettings = app->GetAppSettings();
	bool isFlagOn = appSettings->IsMainWindowAlwaysOnTop();
	GetUIController()->SetAlwaysOnTopFlag(!isFlagOn);
	appSettings->SetMainWindowAlwaysOnTop(!isFlagOn);
	updateWholeAppearance();
}

/**
 *	@brief	Called when user execute "Lock position" command.
 */
void MainUIManager::doCommandMainWindowLockPos()
{
	CoveredCalcApp* app = CoveredCalcApp::GetInstance();
	AppSettings* appSettings = app->GetAppSettings();
	bool isLocked = appSettings->IsMainWindowLocked();
	appSettings->SetMainWindowLocked(!isLocked);
	updateWholeAppearance();
}

/**
 *	@brief	Called when user execute "Preferences" command.
 */
void MainUIManager::doCommandPreferences()
{
	mainUIController->ShowPreferencesDialog();
}

// ---------------------------------------------------------------------
//! Called when user execute "About" command.
// ---------------------------------------------------------------------
void MainUIManager::doCommandAbout()
{
	mainUIController->ShowAboutDialog();
}

// ---------------------------------------------------------------------
//! Called when user execute "About current cover" command.
// ---------------------------------------------------------------------
void MainUIManager::doCommandAboutCurrentCover()
{
	mainUIController->ShowAboutCurrentCoverDialog();
}

// ---------------------------------------------------------------------
//! Called when user execute "Minimize" command.
// ---------------------------------------------------------------------
void MainUIManager::doCommandMainMinimize()
{
	getUIController()->Minimize();
}

// ---------------------------------------------------------------------
//! Called when user execute "Close" command.
// ---------------------------------------------------------------------
void MainUIManager::doCommandClose()
{
	GetUIController()->CloseUI();
}

// ---------------------------------------------------------------------
//! Called when button item is clicked
// ---------------------------------------------------------------------
void MainUIManager::ButtonClicked(
	SInt32 buttonCommand			//!< command of clicked button
)
{
	ExecuteCommand(buttonCommand);
}

// ---------------------------------------------------------------------
//! Called when calculated value is displayed
// ---------------------------------------------------------------------
void MainUIManager::DisplayValue(
	ConstAStr value			//!< data to display
)
{
	drawNumberStringOnDisplay(value);
	numberStringOnDisplay = value;
}

/**
 *	@brief	helper function for converting DigitForm to command id
 */
CoverMainWindowInfo::ButtonClass digitFormToCommandId(CalcCore::DigitForm form)
{
	switch (form)
	{
	case CalcCore::DigitForm_16:
		return CoverMainWindowInfo::ButtonClass_Hex;
	case CalcCore::DigitForm_10:
		return CoverMainWindowInfo::ButtonClass_Dec;
	case CalcCore::DigitForm_8:
		return CoverMainWindowInfo::ButtonClass_Oct;
	case CalcCore::DigitForm_2:
		return CoverMainWindowInfo::ButtonClass_Bin;
	default:
		ASSERT(false);
		return CoverMainWindowInfo::ButtonClass_None;
	}
}

/**
 *	@brief	Displays digit form (radix)
 */
void MainUIManager::DisplayDigitForm(
	CalcCore::DigitForm form	///< digit form
)
{
	if (currentDigitForm != form)
	{
		currentDigitForm = form;
		updateWholeAppearance();
	}
}

// ---------------------------------------------------------------------
//! Displays the error.
// ---------------------------------------------------------------------
void MainUIManager::DisplayError(
	CalcCore::Error /*error*/	//!< error to display
)
{
	AChar value[CalcCore::MAX_VALUE_STR];
	UInt32 maxLength = calcCore.GetMaxDispLength();
	ASSERT(maxLength < CalcCore::MAX_VALUE_STR);
	UInt32 index;
	for (index=0; index<maxLength; index++)
	{
		value[index] = '-';
	}
	value[index] = '\0';

	drawNumberStringOnDisplay(value);
	numberStringOnDisplay = value;	
}

// ---------------------------------------------------------------------
//! Draws number string
// ---------------------------------------------------------------------
void MainUIManager::drawNumberStringOnDisplay(
	ConstAStr string			//!< string to draw
)
{
	const CoverMainWindowInfo* mainWindowInfo = getCurrentMainWindowInfo();
	const CoverFontInfo* fontInfo = mainWindowInfo->GetFontInfo();

	SInt32 displayMapIndex = mainWindowInfo->GetDisplayInfo()->GetMapIndex();
	Rect32 displayRect;
	ColorCodedSkin* skin = getSkin();
	skin->GetAreaRect(displayMapIndex, displayRect);

	// clear display
	SInt32 skinIndex;
	if (GetUIController()->IsUIActive())
	{
		skinIndex = SkinIndex_BackgroundActive;
	}
	else
	{
		skinIndex = SkinIndex_BackgroundInactive;
	}
	skin->UpdatePartAppearance(displayMapIndex, skinIndex);

	// draw characters
	if (displayRect.bottom - displayRect.top + 1 >= fontInfo->GetHeight())
	{
		const AChar* curChar = string + tcslen(string) - 1;
		Point32 drawPoint;
		drawPoint.y = displayRect.top;
		drawPoint.x = displayRect.right - fontInfo->GetWidth();
		while (curChar >= string && drawPoint.x >= displayRect.left)
		{
			switch (fontInfo->GetType())
			{
			case CoverFontInfo::Type_Number:
				{
					// The font on bitmap is located in order of the following: "0123456789ABCDEF.-"
					Rect32 imageRect;
					if ('.' == *curChar)
					{
						imageRect.left = fontInfo->GetWidth() * 16;
					}
					else if ('-' == *curChar)
					{
						imageRect.left = fontInfo->GetWidth() * 17;
					}
					else if ('0' <= *curChar && *curChar <= '9')
					{
						imageRect.left = fontInfo->GetWidth() * (*curChar - '0');
					}
					else if ('a' <= *curChar && *curChar <= 'f')
					{
						imageRect.left = fontInfo->GetWidth() * ((*curChar - 'a') + 10);
					}
					else if ('A' <= *curChar && *curChar <= 'F')
					{
						imageRect.left = fontInfo->GetWidth() * ((*curChar - 'A') + 10);
					}
					else
					{
						imageRect.left = -1;
					}
					
					if (-1 != imageRect.left)
					{
						imageRect.top = 0;
						imageRect.bottom = fontInfo->GetHeight() - 1;
						imageRect.right = imageRect.left + fontInfo->GetWidth() - 1;
						skin->DrawImage(drawPoint, OtherImageIndex_Font, imageRect, fontInfo->GetTransparentColor());
					}
				}
				break;
			}
			
			drawPoint.x -= fontInfo->GetWidth() + fontInfo->GetCharSpace();
			curChar--;
		}
	}
}

// ---------------------------------------------------------------------
//! Returns main window information about current cover.
/*!
	@return pointer to CoverMainWindowInfo object.
*/
// ---------------------------------------------------------------------
const CoverMainWindowInfo* MainUIManager::getCurrentMainWindowInfo() const
{
	const CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
	return manager->GetCoverDef()->GetCoverInfo(manager->GetCurrentCover())->GetMainWindowInfo();
}

/**
 *	@brief	changes last skin area at the cursor position
 */
void MainUIManager::changeLastCursorSkinArea(
	SInt32 area					///< new skin area
)
{
	const CoverMainWindowInfo* mainWindowInfo = getCurrentMainWindowInfo();
	if (NULL != mainWindowInfo)
	{
		const CoverToolTipInfo* toolTipInfo = mainWindowInfo->FindToolTipInfo(area);
		if (NULL != toolTipInfo)
		{
			MBCString mbToolTipText;
			UTF8Conv::ToMultiByteWithLineEnding(mbToolTipText, toolTipInfo->GetToolTipText());
			GetUIController()->SetToolTipText(mbToolTipText);
		}
		else
		{
			GetUIController()->SetToolTipText(ALITERAL(""));
		}
	}

	base::changeLastCursorSkinArea(area);
}
