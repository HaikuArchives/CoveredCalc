/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2009 CoveredCalc Project Contributors
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
	@file		ColorCodedSkin.h
	@brief		Definition of ColorCodedSkin class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.7 created
*/

#ifndef _COLORCODEDSKIN_H_
#define _COLORCODEDSKIN_H_

#include <vector>
#include <map>
#include "ColorValue.h"
#include "DIBitmap.h"

class DIBitmapStore;
class ColorCodedSkinAppearance;

typedef std::vector<const DIBitmap*>						ConstDIBitmapVector;
typedef std::map<ColorValue, SInt32, ColorValueComparator>	ColorAreaMap;

/**
 * This class manages the skin in which the location of parts,
 * such as buttons, is represented by color-coded bitmap.
 */
class ColorCodedSkin
{
public:
	enum {
		Area_None = -1		//!< No area
	};

	enum {
		BlendRatio_Max = 255	///< blend ratio runs from 0 through this value.
	};

						ColorCodedSkin();
	virtual				~ColorCodedSkin();

	void				Init(DIBitmapStore* dibStore, const DIBitmap* mapBitmap, const ConstDIBitmapVector* stateSkins, const ConstDIBitmapVector* otherImages, const ColorAreaMap* colorAreas);
	void				AttachAppearance(ColorCodedSkinAppearance* appearance);

	SInt32				GetArea(const Point32& pt) const;
	SInt32				GetWidth() const { return mapBitmap->GetWidth(); }
	SInt32				GetHeight() const { return mapBitmap->GetHeight(); }

	void				ClipSkinRegion(ColorValue transparentColor);
	void				UnclipSkinRegion();
	void				UpdateWholeAppearance(SInt32 skinNo);
	void				UpdatePartAppearance(SInt32 areaID, SInt32 skinNo);
	void				UpdatePartAppearanceWithBlend(SInt32 areaID, SInt32 skinNo1, SInt32 skinNo2, UInt32 ratio);
	void				DrawImage(const Point32& drawPt, SInt32 imageNo, const Rect32& imageRect, const ColorValue* transparentColor);

	bool				GetAreaRect(SInt32 areaID, Rect32& areaRect);

private:
	struct AreaInfo
	{
		ColorValue	color;		//!< area color
		Rect32		minRect;	//!< minimum rectangle which encloses a part of area.
	};
	typedef std::map<SInt32, AreaInfo>	AreaInfoMap;

	void				destructAllMembers();
	void				makeAreaInfos();
	void				checkInitParams(DIBitmapStore* dibStore, const DIBitmap* mapBitmap, const ConstDIBitmapVector* stateSkins, const ConstDIBitmapVector* otherImages, const ColorAreaMap* colorAreas);

private:
	ColorCodedSkinAppearance*	appearance;
	DIBitmapStore*				dibStore;
	const DIBitmap*				mapBitmap;
	const ConstDIBitmapVector*	stateSkins;
	const ConstDIBitmapVector*	otherImages;
	const ColorAreaMap*			colorAreas;
	AreaInfoMap					areaInfos;
};

#endif // _COLORCODEDSKIN_H_
