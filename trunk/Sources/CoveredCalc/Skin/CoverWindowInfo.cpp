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
	@file		CoverWindowInfo.cpp
	@brief		Implementation of ColorWidowInfo class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.01 created
*/

#include "Prefix.h"
#include "CoverWindowInfo.h"
#include "CoverToolTipInfo.h"
#include "CoverDefParseException.h"
#include "UTF8Utils.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverWindowInfo::CoverWindowInfo()
{
	basePoint.x = 0;
	basePoint.y = 0;
	transparentRegionIndex = -1;
	isTransparentRegionUsedAgainstAlpha = true;
	edgeSmoothingLevel = -1;
	hottingSteps = 1;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverWindowInfo::~CoverWindowInfo()
{
	MapItemPtrVector::iterator mapIte;
	for (mapIte=mapItems.begin(); mapIte!=mapItems.end(); mapIte++)
	{
		delete *mapIte;
	}
	
	CoverToolTipInfoPtrVector::iterator tooltipIte;
	for (tooltipIte=toolTipInfos.begin(); tooltipIte!=toolTipInfos.end(); tooltipIte++)
	{
		delete *tooltipIte;
	}
}

// ---------------------------------------------------------------------
//! Adds new item in colored map.
// ---------------------------------------------------------------------
void CoverWindowInfo::AddMapItem(
	const ColorValue& color,		//!< color in map.
	ConstUTF8Str id					//!< item id
)
{
	MapItem* item = new MapItem(color, id);
	mapItems.push_back(item);
}

// ---------------------------------------------------------------------
//! Adds items in other MapItemPtrVector object.
/*!
	@note
	The items in vector are not copied.
	So if some items in original vector are deleted,
	corresponding items (item's pointers) in this object is invalid.
*/
// ---------------------------------------------------------------------
void CoverWindowInfo::AppendMapItems(
	const MapItemPtrVector& mapItems		//!< MapItemPtrVector object which has added item
)
{
	MapItemPtrVector::const_iterator ite;
	for (ite=mapItems.begin(); ite!=mapItems.end(); ite++)
	{
		this->mapItems.push_back(*ite);
	}	
}

// ---------------------------------------------------------------------
//! Adds new tool tip information.
/*!
	@note
	The CoverWindowInfo object has responsibility to delete
	CoverToolTipInfo object which specified in info.
	Caller must not delete it.
*/
// ---------------------------------------------------------------------
void CoverWindowInfo::AddToolTipInfo(
	CoverToolTipInfo* info		//!< tool tip information
)
{
	toolTipInfos.push_back(info);
}

// ---------------------------------------------------------------------
//! Sets transparent region ID in colored map.
// ---------------------------------------------------------------------
void CoverWindowInfo::SetTransparentRegionID(
	ConstUTF8Str id			//!< transparent region ID
)
{
	transparentRegionID = id;
}

// ---------------------------------------------------------------------
//! Fixes color index of the item from map ID
/*!
	@throw CoverDefParseException when failed
*/
// ---------------------------------------------------------------------
void CoverWindowInfo::ResolveMapId()
{
	// transparent region
	if (!transparentRegionID.IsEmpty())
	{
		transparentRegionIndex = findId(transparentRegionID);
		if (-1 == transparentRegionIndex)
		{
			throw new CoverDefParseExceptions::UnknownID(transparentRegionID);
		}
	}

	// tool tip information
	CoverToolTipInfoPtrVector::iterator tooltipIte;
	for (tooltipIte=toolTipInfos.begin(); tooltipIte!=toolTipInfos.end(); tooltipIte++)
	{
		CoverToolTipInfo* info = *tooltipIte;
		ConstUTF8Str buttonID = info->GetMapID();
		SInt32 buttonIndex = findId(buttonID);
		if (-1 == buttonIndex)
		{
			throw new CoverDefParseExceptions::UnknownID(buttonID);
		}
		info->SetMapIndex(buttonIndex);
	}
}

// ---------------------------------------------------------------------
//! Searches item which id matches specified id and returns the index of the item.
/*!
	@return index. if item was not found, the return value is -1. 
*/
// ---------------------------------------------------------------------
SInt32 CoverWindowInfo::findId(
	ConstUTF8Str id			//!< item id
) const
{
	SInt32 itemCount = static_cast<SInt32>(mapItems.size());
	SInt32 index;
	for (index=0; index<itemCount; index++)
	{
		if (0 == UTF8Utils::UTF8StrCmp(mapItems[index]->GetId(), id))
		{
			return index;
		}
	}
	
	return -1;
}

/**
 *	@brief	Searches the tool tip info from map index.
 *	@return	CoverToolTipInfo object if found, otherwise NULL.
 */
 const CoverToolTipInfo* CoverWindowInfo::FindToolTipInfo(
	SInt32 mapIndex					///< map index
) const
{
	CoverToolTipInfoPtrVector::const_iterator ite;
	for (ite=toolTipInfos.begin(); ite!=toolTipInfos.end(); ite++)
	{
		const CoverToolTipInfo* toolTipInfo = *ite;
		if (NULL != toolTipInfo && toolTipInfo->GetMapIndex() == mapIndex)
		{
			return toolTipInfo;
		}
	}
	
	return NULL;
}

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverWindowInfo::MapItem::MapItem(
	const ColorValue& color,		//!< color in map.
	ConstUTF8Str id					//!< item id
)
{
	this->color = color;
	this->id =id;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverWindowInfo::MapItem::~MapItem()
{
}
