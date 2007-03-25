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
	@file		ColorCodedSkin.cpp
	@brief		Implemantation of ColorCodedSkin class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.7 created
*/

#include "Prefix.h"
#include "ColorCodedSkin.h"
#include "DIBitmap.h"
#include "DIBitmapStore.h"
#include "DIBColorLookup.h"
#include "ColorCodedSkinAppearance.h"
#include "ColorCodedSkinException.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
ColorCodedSkin::ColorCodedSkin()
{
	dibStore = NULL;
	appearance = NULL;
	mapBitmap = NULL;
	stateSkins = NULL;
	otherImages = NULL;
	colorAreas = NULL;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
ColorCodedSkin::~ColorCodedSkin()
{
	destructAllMembers();
}

// ---------------------------------------------------------------------
//! Destructs the each member variables if needed.
// ---------------------------------------------------------------------
void ColorCodedSkin::destructAllMembers()
{
	// bitmaps
	if (NULL != this->dibStore)
	{
		delete this->dibStore;
		this->dibStore = NULL;
		
		this->mapBitmap = NULL;
		
		if (NULL != this->stateSkins)
		{
			delete this->stateSkins;
		}
		this->stateSkins = NULL;
		
		if (NULL != this->otherImages)
		{
			delete this->otherImages;
		}
		this->otherImages = NULL;
	}

	// colorAreas
	if (NULL != colorAreas)
	{
		delete colorAreas;
		colorAreas = NULL;
	}	
}

// ---------------------------------------------------------------------
//! Initializes the object.
/*!
	@throw ColorCodedSkinExceptions::BadInitParams parameter is invalid.
	@note
	The ColorCodedSkin object has responsibility to delete objects
	specified in following parameters: mapBitmap, stateSkins, otherImages and colorAreas.
	Caller must not delete them.
*/
// ---------------------------------------------------------------------
void ColorCodedSkin::Init(
	DIBitmapStore* dibStore,				///< an object which holds loaded bitmaps.
	const DIBitmap* mapBitmap,				//!< a color-coded bitmap which has information about the location of parts in the skin. 
	const ConstDIBitmapVector* stateSkins,	//!< a vector which has appearances bitmap.
	const ConstDIBitmapVector* otherImages,	//!< a vector which has other bitmap.
	const ColorAreaMap* colorAreas,			//!< a map object which maps color value to area ID.
											/*!< note that the negative area ID is reserved so ID in the map
													should be zero or positive value */
	ColorCodedSkinAppearance* appearance	//!< an object which implements ColorCodedSkinAppearance interface.
)
{
	checkInitParams(dibStore, mapBitmap, stateSkins, otherImages, colorAreas, appearance);
	destructAllMembers();

	this->dibStore = dibStore;	
	this->mapBitmap = mapBitmap;
	this->stateSkins = stateSkins;
	this->otherImages = otherImages;
	this->colorAreas = colorAreas;	
	this->appearance = appearance;
	
	makeAreaInfos();
	
	appearance->ChangeSkinSize(mapBitmap->GetWidth(), mapBitmap->GetHeight());
}

// ---------------------------------------------------------------------
//! Checks the parameters specified in Init().
/*!
	@throw ColorCodedSkinExceptions::BadInitParams parameter is invalid.
*/
// ---------------------------------------------------------------------
void ColorCodedSkin::checkInitParams(
	DIBitmapStore* /*dibStore*/,				///< an object which holds loaded bitmaps.
	const DIBitmap* mapBitmap,					//!< the color-coded bitmap which has information about the location of parts in the skin. 
	const ConstDIBitmapVector* stateSkins,		//!< the vector which has appearances bitmap.
	const ConstDIBitmapVector* /*otherImages*/,	//!< a vector which has other bitmap.
	const ColorAreaMap* colorAreas,				//!< the map object which maps color value to area ID.
												/*!< note that the negative area ID is reserved so ID in the map
													should be zero or positive value */
	ColorCodedSkinAppearance* /*appearance*/	//!< the object which implements ColorCodedSkinAppearance interface.
)
{
	SInt32 width  = mapBitmap->GetWidth();
	SInt32 height = mapBitmap->GetHeight();
	ConstDIBitmapVector::const_iterator skinIterator;
	for (skinIterator=stateSkins->begin(); skinIterator!=stateSkins->end(); skinIterator++)
	{
		const DIBitmap* bitmap = *skinIterator;
		if (bitmap->GetWidth() != width || bitmap->GetHeight() != height)
		{
			// mapBitmap and bitmap in stateSkins must have same width and height!
			throw new ColorCodedSkinExceptions::BadInitParams();
		}
	}
	
#ifdef DEBUG
	ColorAreaMap::const_iterator areaIterator;
	for (areaIterator=colorAreas->begin(); areaIterator!=colorAreas->end(); areaIterator++)
	{
		SInt32 area = areaIterator->second;
		if (0 > area)
		{
			// area ID should be zero or positive value!
			ASSERT(false);
			throw new ColorCodedSkinExceptions::BadInitParams();
		}
	}
#else	// Release
	colorAreas;	// avoid warning
#endif
}

// ---------------------------------------------------------------------
//! Creates the area information based on mapBitmap and colorAreas.
// ---------------------------------------------------------------------
void ColorCodedSkin::makeAreaInfos()
{
	ColorAreaMap::const_iterator iteColArea;

	areaInfos.clear();
	
	for (iteColArea=colorAreas->begin();iteColArea!=colorAreas->end();iteColArea++) {
		AreaInfo	info;
		info.color = iteColArea->first;
		info.minRect.left = info.minRect.top = info.minRect.right = info.minRect.bottom = -1;
		areaInfos[iteColArea->second] = info;
	}
	
	DIBColorLookup	lookup(mapBitmap);
	
	SInt32 width = mapBitmap->GetWidth();
	SInt32 height = mapBitmap->GetHeight();
	SInt32 posY;
	for (posY=0; posY<height; posY++)
	{
		lookup.InitLocation(0, posY);
		SInt32 posX;
		for (posX=0; posX<width; posX++)
		{
			ColorValue color = lookup.LookupNextColor();
			iteColArea = colorAreas->find(color);
			if (iteColArea != colorAreas->end())	// found
			{
				SInt32 areaID = iteColArea->second;
				AreaInfoMap::iterator iteAreaInfo = areaInfos.find(areaID);
				ASSERT(iteAreaInfo != areaInfos.end());
				if (iteAreaInfo != areaInfos.end())
				{
					Rect32& partRect = iteAreaInfo->second.minRect;
					if (-1 == partRect.left || partRect.left > posX)
					{
						partRect.left = posX;
					}
					if (-1 == partRect.top || partRect.top > posY)
					{
						partRect.top = posY;
					}
					if (partRect.right < posX)
					{
						partRect.right = posX;
					}
					if (partRect.bottom < posY)
					{
						partRect.bottom = posY;
					}
				}
			}
		}		
	}
}

// ---------------------------------------------------------------------
//! Calucuales the minimum rectangle including region of a specified area.
/*!
	@retval true successful.
	@retval false failed (area is not found).
*/
// ---------------------------------------------------------------------
bool ColorCodedSkin::GetAreaRect(
	SInt32 areaID,				//!< area id
	Rect32& areaRect			//!< OUTPUT. minimum rectangle
)
{
	AreaInfoMap::const_iterator iteAreaInfo = areaInfos.find(areaID);
	if (iteAreaInfo == areaInfos.end())
	{
		return false;
	}
	
	areaRect = iteAreaInfo->second.minRect;
	return true;
}

// ---------------------------------------------------------------------
//! Returns area ID at specified point.
/*!
	@return area ID, or Area_None if there is no area ID.
*/
// ---------------------------------------------------------------------
SInt32 ColorCodedSkin::GetArea(
	const Point32& pt			//!< the location
) const
{
	if (pt.x < 0 || pt.x >= mapBitmap->GetWidth() || pt.y < 0 || pt.y >= mapBitmap->GetHeight())
	{
		return Area_None;
	}

	DIBColorLookup	lookup(mapBitmap);
	ColorValue color = lookup.Lookup(pt.x, pt.y);
	ColorAreaMap::const_iterator iterator = colorAreas->find(color);
	if (iterator != colorAreas->end())
	{
		return iterator->second;
	}
	else
	{
		return Area_None;
	}
}

// ---------------------------------------------------------------------
//! Cuts out the transparent region of the skin.
// ---------------------------------------------------------------------
void ColorCodedSkin::ClipSkinRegion(
	ColorValue transparentColor		//!< transparent color
)
{
	if (NULL == appearance)
	{
		return;
	}
	
	appearance->ClipSkinRegion(mapBitmap, transparentColor);
}

// ---------------------------------------------------------------------
//! Restores the cut-outed transparent region.
// ---------------------------------------------------------------------
void ColorCodedSkin::UnclipSkinRegion()
{
	if (NULL == appearance)
	{
		return;
	}
	
	appearance->UnclipSkinRegion();
}

// ---------------------------------------------------------------------
//! Updates whole appearance using specified state of skin.
// ---------------------------------------------------------------------
void ColorCodedSkin::UpdateWholeAppearance(
	SInt32 skinNo				//!< specifies which state of bitmap to use.
)
{
	if (NULL == appearance)
	{
		return;
	}
	
	Point32 drawPoint = {0, 0};
	Rect32 skinRect = {0, 0, mapBitmap->GetWidth() - 1, mapBitmap->GetHeight() - 1 };
	appearance->CopySkin(drawPoint, (*stateSkins)[skinNo], skinRect);
}

// ---------------------------------------------------------------------
//! Updates whole appearance with transparent color using specified state of skin.
// ---------------------------------------------------------------------
void ColorCodedSkin::UpdateWholeAppearance(
	SInt32 skinNo,					//!< specifies which state of bitmap to use.
	ColorValue transparentColor		//!< transparent color
)
{
	if (NULL == appearance)
	{
		return;
	}
	
	Point32 drawPoint = {0, 0};
	Rect32 skinRect = {0, 0, mapBitmap->GetWidth() - 1, mapBitmap->GetHeight() - 1 };
	appearance->CopySkin(drawPoint, (*stateSkins)[skinNo], skinRect, transparentColor);
}

// ---------------------------------------------------------------------
//! Updates the part corresponding to the specified area, using specified state of skin.
// ---------------------------------------------------------------------
void ColorCodedSkin::UpdatePartAppearance(
	SInt32 areaID,					//!< area ID which specifies the part in the skin.
	SInt32 skinNo					//!< which state of bitmap to use.
)
{
	if (NULL == appearance)
	{
		return;
	}

	AreaInfoMap::const_iterator iteAreaInfo = areaInfos.find(areaID);
	ASSERT(iteAreaInfo != areaInfos.end());
	if (iteAreaInfo == areaInfos.end())
	{
		return;
	}
	
	const Rect32& skinRect = iteAreaInfo->second.minRect;
	Point32 drawPoint = { skinRect.left, skinRect.top };
	appearance->DrawSkinByColor(drawPoint, mapBitmap, (*stateSkins)[skinNo], iteAreaInfo->second.color, skinRect);
}

/**
 *	@brief	Updates the part correspinding to the spacified area, with blending two states of skin.
 *	@param	areaID	area ID which specifies the part in the skin.
 *	@param	skinNo1	which state of bitmap to use
 *	@param	skinNo2	which state of bitmap to use
 *	@param	ratio	ratio of blending bitmap of skinNo1 and bitmap of skinNo2.
 *					if ratio is 0, only a bitamap of skinNo1 is used,
 *					and if ratio is ColorCodedSkin::BlendRatio_Max, only a bitmap of skinNo2 is used.
 */
void ColorCodedSkin::UpdatePartAppearanceWithBlend(
	SInt32 areaID,
	SInt32 skinNo1,
	SInt32 skinNo2,
	UInt32 ratio
)
{
	if (ratio <= 0)
	{
		UpdatePartAppearance(areaID, skinNo2);
	}
	else if (ColorCodedSkin::BlendRatio_Max <= ratio)
	{
		UpdatePartAppearance(areaID, skinNo1);
	}
	else
	{
		if (NULL == appearance)
		{
			return;
		}

		AreaInfoMap::const_iterator iteAreaInfo = areaInfos.find(areaID);
		ASSERT(iteAreaInfo != areaInfos.end());
		if (iteAreaInfo == areaInfos.end())
		{
			return;
		}
		
		const Rect32& skinRect = iteAreaInfo->second.minRect;
		Point32 drawPoint = { skinRect.left, skinRect.top };
		appearance->DrawBlendSkinByColor(drawPoint, mapBitmap, (*stateSkins)[skinNo1], (*stateSkins)[skinNo2], iteAreaInfo->second.color, skinRect, ratio);
	}
}

// ---------------------------------------------------------------------
//! Draws specified image of skin.
// ---------------------------------------------------------------------
void ColorCodedSkin::DrawImage(
	const Point32& drawPt,		//!< top-left position of the appearance.
	SInt32 imageNo,				//!< which image of bitmap to use.
								/*!< image bitmap is specified in otherImages parameter at Init(). */
	const Rect32& imageRect		//!< rectangle in image
)
{
	if (NULL == appearance)
	{
		return;
	}
	
	appearance->CopySkin(drawPt, (*otherImages)[imageNo], imageRect);
}

// ---------------------------------------------------------------------
//! Draws specified state of skin with transparent color.
// ---------------------------------------------------------------------
void ColorCodedSkin::DrawImage(
	const Point32& drawPt,			//!< top-left position of the appearance.
	SInt32 imageNo,					//!< which image of bitmap to use.
									/*!< image bitmap is specified in otherImages parameter at Init(). */
	const Rect32& imageRect,		//!< rectangle in image
	ColorValue transparentColor		//!< transparent color
)
{
	if (NULL == appearance)
	{
		return;
	}
	
	appearance->CopySkin(drawPt, (*otherImages)[imageNo], imageRect, transparentColor);
}
