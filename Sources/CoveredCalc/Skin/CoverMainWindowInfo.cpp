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
	@file		CoverMainWindowInfo.cpp
	@brief		Implementation of CoverMainWindowInfo class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.16 created
*/

#include "Prefix.h"
#include <string.h>
#include "CoverButtonInfo.h"
#include "CoverFontInfo.h"
#include "CoverDisplayInfo.h"
#include "CoverMainWindowInfo.h"
#include "CoverDefParseException.h"
#include "CoverDefValidateException.h"

////////////////////////////////////////
#define base	CoverWindowInfo
////////////////////////////////////////

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverMainWindowInfo::CoverMainWindowInfo()
{
	fontInfo = NULL;
	displayInfo = NULL;
	memset(buttonInfo, 0, sizeof(buttonInfo));
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverMainWindowInfo::~CoverMainWindowInfo()
{
	SInt32 index;
	
	if (NULL != displayInfo)
	{
		delete displayInfo;
	}
	
	if (NULL != fontInfo)
	{
		delete fontInfo;
	}

	for (index=0; index<ButtonClass_Max; index++)
	{
		if (NULL != buttonInfo[index])
		{
			delete buttonInfo[index];
		}
	}
}

// ---------------------------------------------------------------------
//! Sets specified string as a path of an image file.
// ---------------------------------------------------------------------
void CoverMainWindowInfo::SetImageFilePath(
	ImageClass imageClass,		//!< specifies which image file
	const Path& filePath		//!< path of the image file
)
{
	ASSERT(0 <= imageClass && imageClass < ImageClass_Max);

	imageFilePath[imageClass] = filePath;
}

// ---------------------------------------------------------------------
//! Sets an information about a button.
/*!
	@note
	CoverMainWindowInfo object has responsibility to delete
	the object specified at info parameter.
	Caller must not delete it.
*/
// ---------------------------------------------------------------------
void CoverMainWindowInfo::SetButtonInfo(
	ButtonClass buttonClass,		//!< specifies which button
	CoverButtonInfo* info			//!< button information

)
{
	ASSERT(0 <= buttonClass && buttonClass < ButtonClass_Max);

	if (NULL != buttonInfo[buttonClass])
	{
		delete buttonInfo[buttonClass];
	}

	buttonInfo[buttonClass] = info;
}

// ---------------------------------------------------------------------
//! Sets an information about font.
/*!
	@note
	CoverMainWindowInfo object has responsibility to delete
	the object specified at info parameter.
	Caller must not delete it.
*/
// ---------------------------------------------------------------------
void CoverMainWindowInfo::SetFontInfo(
	CoverFontInfo* info			//!< font information
)
{
	if (NULL != fontInfo)
	{
		delete fontInfo;
	}
	fontInfo = info;
}

// ---------------------------------------------------------------------
//! Sets an information about a display.
/*!
	@note
	CoverMainWindowInfo object has responsibility to delete
	the object specified at info parameter.
	Caller must not delete it.
*/
// ---------------------------------------------------------------------
void CoverMainWindowInfo::SetDisplayInfo(
	CoverDisplayInfo* info			//!< display information
)
{
	if (NULL != displayInfo)
	{
		delete displayInfo;
	}
	displayInfo = info;
}

// ---------------------------------------------------------------------
//! Checks validation of data.
/*!
	@note
	This function is called after making this object has finished.
*/
// ---------------------------------------------------------------------
void CoverMainWindowInfo::Validate()
{
	// image path
	if (imageFilePath[ImageClass_ColorMap].IsEmpty())
	{
		throw new CoverDefValidateException(ALITERAL("Map image is required."));
	}
	if (imageFilePath[ImageClass_BackgroundActive].IsEmpty())
	{
		throw new CoverDefValidateException(ALITERAL("\"BackgroundActive\" image is required."));
	}
	
	// display information
	if (NULL == displayInfo)
	{
		throw new CoverDefValidateException(ALITERAL("Definition of display is required."));
	}
	
	// font
	if (NULL == fontInfo)
	{
		throw new CoverDefValidateException(ALITERAL("Definition of font is required."));
	}
}

// ---------------------------------------------------------------------
//! Fixes color index of the item from map ID
/*!
	@note
	This function may throw CoverDefParseException or
	its derived class object when failed.
*/
// ---------------------------------------------------------------------
void CoverMainWindowInfo::ResolveMapId()
{
	base::ResolveMapId();
	
	// display information
	if (NULL != displayInfo)
	{
		ConstUTF8Str mapId = displayInfo->GetMapID();
		SInt32 mapIndex = findId(mapId);
		if (-1 == mapIndex)
		{
			throw new CoverDefParseExceptions::UnknownID(mapId);
		}
		displayInfo->SetMapIndex(mapIndex);
	}
	
	// button information
	SInt32 index;
	for (index=0; index<ButtonClass_Max; index++)
	{
		if (NULL != buttonInfo[index])
		{
			ConstUTF8Str buttonID = buttonInfo[index]->GetMapID();
			SInt32 buttonIndex = findId(buttonID);
			if (-1 == buttonIndex)
			{
				throw new CoverDefParseExceptions::UnknownID(buttonID);
			}
			buttonInfo[index]->SetMapIndex(buttonIndex);
		}
	}
}

// ---------------------------------------------------------------------
//! Searches the button from map index.
/*!
	@return ButtonClass which suits condition.
			if there is no button suitable, ButtonClass_None is returned.
*/
// ---------------------------------------------------------------------
CoverMainWindowInfo::ButtonClass CoverMainWindowInfo::FindButton(
	SInt32 mapIndex		///< map index
) const
{
	// find button
	SInt32 index;
	for (index=0; index<ButtonClass_Max; index++)
	{
		if (NULL != buttonInfo[index])
		{
			if (mapIndex == buttonInfo[index]->GetMapIndex())
			{
				return static_cast<CoverMainWindowInfo::ButtonClass>(index);
			}
		}
	}
	
	return ButtonClass_None;
}
