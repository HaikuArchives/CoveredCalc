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
	@file		CoverManager.h
	@brief		Definition of CoverManager class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.16 created
*/

#ifndef _COVERMANAGER_H_
#define _COVERMANAGER_H_

#include <list>
#include "Path.h"
#include "ColorCodedSkin.h"

class CoverDef;
class ColorCodedSkin;
class DIBitmap;
class ColorCodedSkinAppearance;
class CoverMainWindowInfo;
class SkinBuilder;
class CoverChangeEventHandler;

// ---------------------------------------------------------------------
//! Loads CoverDef from "CoverDef.xml" and generates ColorCodedSkin.
/*!
	This class loads the XML file "CoverDef.xml",
	and generates a CoverDef object and a ColorCodedSkin object.
*/
// ---------------------------------------------------------------------
class CoverManager
{
public:
								CoverManager();
	virtual						~CoverManager();
		
	void						Load(const Path& filePath, SInt32 coverNo);
	
	const Path&					GetCoverDefFilePath() const { return coverFilePath; }
	const CoverDef*				GetCoverDef() const { return coverDef; }

	void						SetCurrentCover(SInt32 currentCover);
	SInt32						GetCurrentCover() const { return currentCover; }

	void						RegisterCoverChangeEventHandler(CoverChangeEventHandler* handler);
	void						UnregisterCoverChangeEventHandler(CoverChangeEventHandler* handler);

private:
	typedef std::list<CoverChangeEventHandler*> CoverChangeEventHandlerList;

private:
	CoverDef*					coverDef;					//!< generated CoverDef object
	Path						coverFilePath;				//!< path of loaded cover file
	SInt32						currentCover;				//!< current cover no.
	CoverChangeEventHandlerList	coverChangeEventHandlers;	//!< CoverChangeEventHandler handlers
};

#endif // _COVERMANAGER_H_
