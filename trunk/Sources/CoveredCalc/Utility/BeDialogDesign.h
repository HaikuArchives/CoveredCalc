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
	@file		BeDialogDesign.h
	@brief		Definition of BeDialogDesign class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.09.23 created
*/

#ifndef _BEDIALOGDESIGN_H_
#define _BEDIALOGDESIGN_H_

#include <map>
#include <support/String.h>
#include <interface/Rect.h>
#include <support/List.h>
#include "MBCString.h"

class NCDElement;
class BeDialogDesign;

/**
 *	@brief	This class represents a dialog item (view information) in BeDialogDesign
 */
class BeDialogDesignItem
{
public:
								BeDialogDesignItem();
	virtual						~BeDialogDesignItem() {}

	void						Load(BeDialogDesign* dialogDesign, const NCDElement* dialogItemElem);

	const char*					GetName() const
										{ return name; }
	const BRect&				GetFrame(const BRect& defaultFrame) const
										{ return (defFlags & DEFINITION_FRAME) ? frame : defaultFrame; }
	const char*					GetLabel(const char* defaultLabel) const
										{ return (defFlags & DEFINITION_LABEL) ? label.CString() : defaultLabel; }
	float						GetDivider(float defaultDivider) const
										{ return (defFlags & DEFINITION_DIVIDER) ? divider : defaultDivider; }

private:
	enum DefinitionFlags
	{
		DEFINITION_FRAME	= 0x01,		///< frame is defined.
		DEFINITION_LABEL	= 0x02,		///< label is defined.
		DEFINITION_DIVIDER	= 0x04,		///< divider is defined.
	};

private:
	UInt32						defFlags;		///< combination of DefinitionFlags
	MBCString					name;			///< view name
	BRect						frame;			///< view frame rect
	MBCString					label;			///< label for this item
	float						divider;		///< divider x-coordinate
};

/**
 *	@brief	This class represents dialog design resource ('DLGD') data.
 */
class BeDialogDesign
{
public:
								BeDialogDesign();
	virtual						~BeDialogDesign();

	void						Load(const NCDElement* dialogElem);

	const char*					GetTitle(const char* defaultTitle) const;
	const BRect&				GetFrame(const BRect& defaultRect) const;

	const BeDialogDesignItem*	FindItem(const char* name) const;
	void						RescaleFrame(BRect& rect);

private:
	void						clearItems();
	void						calcScales(const NCDElement* designFontInfoElem);
	
private:
	typedef std::map<MBCString, const BeDialogDesignItem*>	NameToItemMap;

private:
	static BeDialogDesignItem	defaultDesignItem;	///< default design item (non-defined item)
	NameToItemMap				itemMap;			///< map to retrieve a BeDialogDesignItem object from name.

	float						hScale;				///< horizontal scale of frame
	float						vScale;				///< vertical scale of frame
};

#endif // _BEDIALOGDESIGN_H_
