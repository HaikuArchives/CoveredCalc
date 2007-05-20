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
	@file		CoverWindowInfo.h
	@brief		Definition of ColorWidowInfo class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.30 created
*/

#ifndef _COVERWINDOWINFO_H_
#define _COVERWINDOWINFO_H_

#include <vector>
#include "ColorValue.h"
#include "UTF8String.h"

class CoverToolTipInfo;

// ---------------------------------------------------------------------
//! Information about the cover of the window.
/*!
	This class holds the contents of one <window> tag in file "CoverDef.xml".
*/
// ---------------------------------------------------------------------
class CoverWindowInfo
{
public:
	// ---------------------------------------------------------------------
	//! The item in colored map.
	// ---------------------------------------------------------------------
	class MapItem
	{
	public:
							MapItem(const ColorValue& color, ConstUTF8Str id);
							~MapItem();
		const ColorValue&	GetColor() const { return color; }
		ConstUTF8Str		GetId() const { return id; }
	
	private:
		ColorValue	color;
		UTF8String	id;
	};
	
	typedef std::vector<MapItem*>	MapItemPtrVector;

public:
								CoverWindowInfo();
	virtual						~CoverWindowInfo();

	void						SetBasePoint(const Point32& point) { basePoint = point; }
	void						AddMapItem(const ColorValue& color, ConstUTF8Str id);
	void						AppendMapItems(const MapItemPtrVector& mapItems);
	void						AddToolTipInfo(CoverToolTipInfo* info);
	void						SetTransparentRegionID(ConstUTF8Str id);
	void						SetTransparentRegionUsedAgainstAlpha(bool isUsed) { this->isTransparentRegionUsedAgainstAlpha = isUsed; }
	void						SetEdgeSmoothingLevel(SInt32 level) { this->edgeSmoothingLevel = level; }
	void						SetHottingSteps(SInt32 hottingSteps) { this->hottingSteps = hottingSteps; }

	virtual void				ResolveMapId();
	virtual void				Validate() { }

	const Point32&				GetBasePoint() const { return basePoint; }
	SInt32						GetNumMapItems() const	{ return static_cast<SInt32>(mapItems.size()); }
	const ColorValue&			GetMapItem(SInt32 index) const { return mapItems[index]->GetColor(); }
	SInt32						GetTransparentRegionIndex() const { return transparentRegionIndex; }
	bool						IsTransparentRegionUsedAgainstAlpha() const { return isTransparentRegionUsedAgainstAlpha; }
	SInt32						GetEdgeSmoothingLevel() const { return edgeSmoothingLevel; }
	const CoverToolTipInfo*		FindToolTipInfo(SInt32 mapIndex) const;
	SInt32						GetHottingSteps() const { return hottingSteps; }

protected:
	SInt32						findId(ConstUTF8Str id) const;

private:
	typedef std::vector<CoverToolTipInfo*>	CoverToolTipInfoPtrVector;
	
private:
	Point32						basePoint;				//!< the point which doesn't move when cover is changed
	MapItemPtrVector			mapItems;				//!< items in colored map
	CoverToolTipInfoPtrVector	toolTipInfos;			//!< tool tip
	UTF8String					transparentRegionID;	//!< id of mapItems for transparent region
	SInt32						transparentRegionIndex;	//!< index of mapItems for transparent region	
	bool						isTransparentRegionUsedAgainstAlpha;	///< whether transmarent region is used against alpha channel.
	SInt32						edgeSmoothingLevel;		///< edge smoothing level. negative value means "auto".
	SInt32						hottingSteps;			///< how many steps a button becomes completely "hot" state.
};

#endif // _COVERWINDOWINFO_H_
