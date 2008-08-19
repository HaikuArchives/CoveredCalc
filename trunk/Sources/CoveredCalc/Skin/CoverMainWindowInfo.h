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
	@file		CoverMainWindowInfo.h
	@brief		Definition of ColorMainWidowInfo class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.16 created
*/

#ifndef _COVERMAINWINDOWINFO_H_
#define _COVERMAINWINDOWINFO_H_

#include "Path.h"
#include "CoverWindowInfo.h"
#include "ColorValue.h"

class CoverButtonInfo;
class CoverFontInfo;
class CoverDisplayInfo;

// ---------------------------------------------------------------------
//! Information about the cover of main window.
/*!
	This class holds the contents of one <window class="Main"> tag
	in file "CoverDef.xml".
*/
// ---------------------------------------------------------------------
class CoverMainWindowInfo : public CoverWindowInfo
{
public:
	// ---------------------------------------------------------------------
	//! class attribute
	// ---------------------------------------------------------------------
	enum ImageClass
	{
		ImageClass_ColorMap = 0,
		ImageClass_BackgroundActive,
		ImageClass_BackgroundInactive,
		ImageClass_Pressed,
		ImageClass_GrayedActive,
		ImageClass_GrayedInactive,
		ImageClass_CheckedActive,
		ImageClass_CheckedInactive,
		ImageClass_CheckedPressed,
		ImageClass_HotActive,
		ImageClass_HotInactive,
		ImageClass_CheckedHotActive,
		ImageClass_CheckedHotInactive,
		ImageClass_Font,
		
		ImageClass_Max
	};
	
	// ---------------------------------------------------------------------
	//! button class
	// ---------------------------------------------------------------------
	enum ButtonClass
	{
		ButtonClass_None = -1,
	
		ButtonClass_0 = 0,
		ButtonClass_1,
		ButtonClass_2,
		ButtonClass_3,
		ButtonClass_4,
		ButtonClass_5,
		ButtonClass_6,
		ButtonClass_7,
		ButtonClass_8,
		ButtonClass_9,
		ButtonClass_A,
		ButtonClass_B,
		ButtonClass_C,
		ButtonClass_D,
		ButtonClass_E,
		ButtonClass_F,
		ButtonClass_Point,
		ButtonClass_Clear,
		ButtonClass_BS,
		ButtonClass_Equal,
		ButtonClass_Plus,
		ButtonClass_Minus,
		ButtonClass_Times,
		ButtonClass_Div,
		ButtonClass_Negate,
		ButtonClass_Hex,
		ButtonClass_Dec,
		ButtonClass_Oct,
		ButtonClass_Bin,
		ButtonClass_Minimize,
		ButtonClass_Close,
		
		ButtonClass_Max
	};
		
public:
							CoverMainWindowInfo();
	virtual					~CoverMainWindowInfo();

	void					SetImageFilePath(ImageClass imageClass, const Path& filePath);
	void					SetFontInfo(CoverFontInfo* info);
	void					SetButtonInfo(ButtonClass buttonClass, CoverButtonInfo* info);
	void					SetDisplayInfo(CoverDisplayInfo* info);

	virtual void			ResolveMapId();
	virtual void			Validate();
	
	const Path&				GetImageFilePath(ImageClass imageClass) const			{ return imageFilePath[imageClass]; }
	const CoverFontInfo*	GetFontInfo() const										{ return fontInfo; }
	const CoverButtonInfo*	GetButtonInfo(ButtonClass buttonClass) const			{ return buttonInfo[buttonClass]; }
	const CoverDisplayInfo*	GetDisplayInfo() const									{ return displayInfo; }

	ButtonClass				FindButton(SInt32 mapIndex) const;	

private:
	Path					imageFilePath[ImageClass_Max];	//!< full path of images
	CoverFontInfo*			fontInfo;						//!< information about font
	CoverDisplayInfo*		displayInfo;					//!< information about display
	CoverButtonInfo*		buttonInfo[ButtonClass_Max];	//!< index of mapItems for each button
};

#endif // _COVERMAINWINDOWINFO_H_
