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
	@file		BeDialogDesign.cpp
	@brief		Implementation of BeDialogDesign class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.09.23 created
*/

#include "Prefix.h"
#include "BeDialogDesign.h"
#include <InterfaceKit.h>
#include "UTF8String.h"
#include "NCDElement.h"
#include "UTF8Conv.h"
#include "UTF8Utils.h"
#include "DOMUtils.h"

BeDialogDesignItem BeDialogDesign::defaultDesignItem;

/**
 *	@brief	Constructor
 */
BeDialogDesignItem::BeDialogDesignItem()
{
	defFlags = 0;
	frame.Set(0, 0, 0, 0);
	divider = 0.0;
}

/**
 *	Initialize.
 *	@param[in]	dialogDesign	BeDialogDesign object.
 *	@param[in]	dialogItemElem	dialogItem element of XML language file.
 */
void BeDialogDesignItem::Load(BeDialogDesign* dialogDesign, const NCDElement* dialogItemElem)
{
	defFlags = 0;
	UTF8String nameUTF8;
	dialogItemElem->getAttribute(TypeConv::AsUTF8("name"), nameUTF8);
	MBCString nameMBCS;
	UTF8Conv::ToMultiByte(nameMBCS, nameUTF8);
	this->name = nameMBCS;
	
	NCDNode* childNode = dialogItemElem->getFirstChild();
	while (NULL != childNode)
	{
		if (NCDNode::ELEMENT_NODE == childNode->getNodeType())
		{
			NCDElement* childElem = static_cast<NCDElement*>(childNode);
			UTF8String name;
			childElem->getNodeName(name);
			if (0 == UTF8Utils::UTF8StrCmp(name, TypeConv::AsUTF8("rect")))
			{
				float value;
				UTF8String valueStr;
				childElem->getAttribute(TypeConv::AsUTF8("left"), valueStr);
				value = atof(TypeConv::AsASCII(valueStr));
				frame.left = value;
				childElem->getAttribute(TypeConv::AsUTF8("top"), valueStr);
				value = atof(TypeConv::AsASCII(valueStr));
				frame.top = value;
				childElem->getAttribute(TypeConv::AsUTF8("right"), valueStr);
				value = atof(TypeConv::AsASCII(valueStr));
				frame.right = value;
				childElem->getAttribute(TypeConv::AsUTF8("bottom"), valueStr);
				value = atof(TypeConv::AsASCII(valueStr));
				frame.bottom = value;
				dialogDesign->RescaleFrame(frame);
				defFlags |= DEFINITION_FRAME;
			}
			else if (0 == UTF8Utils::UTF8StrCmp(name, TypeConv::AsUTF8("label")))
			{
				UTF8String valueStr;
				DOMUtils::ReadTextNode(childElem, true, valueStr);
				UTF8Conv::ToMultiByte(label, valueStr);
				defFlags |= DEFINITION_LABEL;
			}
			else if (0 == UTF8Utils::UTF8StrCmp(name, TypeConv::AsUTF8("divider")))
			{
				UTF8String valueStr;
				childElem->getAttribute(TypeConv::AsUTF8("position"), valueStr);
				float value = atof(TypeConv::AsASCII(valueStr));
				BRect tempRect(0, 0, value, 0);
				dialogDesign->RescaleFrame(tempRect);
				divider = tempRect.right - tempRect.left;
				defFlags |= DEFINITION_DIVIDER;
			}
		}
		
		childNode = childNode->getNextSibling();
	}
}

/**
 *	@brief		Constructor
 */
BeDialogDesign::BeDialogDesign()
{
	hScale = vScale = 1.00f;
}

/**
 *	@brief	Destructor
 */
BeDialogDesign::~BeDialogDesign()
{
	clearItems();
}

/**
 *	Initialize.
 *	@param[in]	dialogElem	dialog element of XML language file.
 */
void BeDialogDesign::Load(const NCDElement* dialogElem)
{
	clearItems();
	if (NULL != dialogElem)
	{
		NCDNode* childNode = dialogElem->getFirstChild();
		while (NULL != childNode)
		{
			if (NCDNode::ELEMENT_NODE == childNode->getNodeType())
			{
				NCDElement* childElem = static_cast<NCDElement*>(childNode);
				UTF8String name;
				childElem->getNodeName(name);
				if (0 == UTF8Utils::UTF8StrCmp(name, TypeConv::AsUTF8("dialogItem")))
				{
					BeDialogDesignItem* newItem = new BeDialogDesignItem();
					try
					{
						newItem->Load(this, childElem);
					}
					catch (...)
					{
						delete newItem;
						throw;
					}
					
					MBCString name = newItem->GetName();
					itemMap[name] = newItem;
				}
				else if (0 == UTF8Utils::UTF8StrCmp(name, TypeConv::AsUTF8("designFontInfo")))
				{
					calcScales(childElem);
				}
			}
			
			childNode = childNode->getNextSibling();
		}
	}
}

/**
 *	@brief	Clears BeDialogDesignItem objects.
 */
void BeDialogDesign::clearItems()
{
	hScale = vScale = 1.00f;
	NameToItemMap::iterator ite;
	for (ite = itemMap.begin(); ite != itemMap.end(); ite++)
	{
		delete (*ite).second;
	}
	itemMap.clear();
}

/**
 *	@brief	Rescale frame location according to current font size
 */
void BeDialogDesign::RescaleFrame(
	BRect& rect			///< frame rect
)
{
	float scale;

	if (hScale <= 0.99f || hScale >= 1.01f)
	{
		scale = hScale * 1.03f;	// increase the scale with 3% to avoid rounding problems
		rect.left	= rect.left		* scale + 0.5f;
		rect.right	= rect.right	* scale + 0.5f;
	}
	if (vScale <= 0.99f || vScale >= 1.01f)
	{
		scale = vScale * 1.03f;	// increase the scale with 3% to avoid rounding problems
		rect.top	= rect.top		* scale + 0.5f;
		rect.bottom	= rect.bottom	* scale + 0.5f;
	}
}

/**
 *	@brief		Calculates frame scales.
 *	@param[in]	designFontInfoElem	designFontInfo element of XML language file.
 */
void BeDialogDesign::calcScales(const NCDElement* designFontInfoElem)
{
	font_height fh;
	be_plain_font->GetHeight(&fh);
	float height = fh.ascent + fh.descent;
	float width = be_plain_font->StringWidth("abcdefghijklmnopqrstuvwxyz");
	
	UTF8String widthStr;
	designFontInfoElem->getAttribute(TypeConv::AsUTF8("width"), widthStr);
	float designFontWidth = atof(TypeConv::AsASCII(widthStr));
	if (0.0f != designFontWidth)
	{
		hScale = width / designFontWidth;
	}
	
	UTF8String heightStr;
	designFontInfoElem->getAttribute(TypeConv::AsUTF8("height"), heightStr);
	float designFontHeight = atof(TypeConv::AsASCII(heightStr));
	if (0.0f != designFontHeight)
	{
		vScale = height / designFontHeight;
	}
}

/**
 *	@brief	Finds dialog item from its name
 *	@return	dialog item. or NULL if not found.
 */
const BeDialogDesignItem* BeDialogDesign::FindItem(
	const char* name		///< name
) const
{
	MBCString nameStr = name;
	NameToItemMap::const_iterator ite = itemMap.find(nameStr);
	if (ite != itemMap.end())
	{
		return (*ite).second;
	}
	else
	{
		return &defaultDesignItem;
	}
}

/**
 *	@brief	Returns dialog title.
 */
const char* BeDialogDesign::GetTitle(const char* defaultTitle) const
{
	const BeDialogDesignItem* item = FindItem("");
	if (NULL == item)
	{
		return defaultTitle;
	}
	else
	{
		return item->GetLabel(defaultTitle);
	}
}

/**
 *	@brief	Returns dialog frame rectangle.
 */
const BRect& BeDialogDesign::GetFrame(const BRect& defaultRect) const
{
	const BeDialogDesignItem* item = FindItem("");
	if (NULL == item)
	{
		return defaultRect;
	}
	else
	{
		return item->GetFrame(defaultRect);
	}
}
