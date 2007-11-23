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
	@file		KeyMappings.h
	@brief		Definition of KeyMappings class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.10.10 created
*/

#ifndef _KEYMAPPINGS_H_
#define _KEYMAPPINGS_H_

#include "UTF8String.h"
#include "Enumerator.h"

class Path;
class NCDDocument;
class NCDElement;

/**
 *	@brief	The interface to read one key-mapping item.
 */
class KeyMappingItem
{
public:
						KeyMappingItem() {}
	virtual				~KeyMappingItem() {}
	
	/**
	 *	@brief	Returns a key-code value.
	 *	@param[out]	keyCode	a key-code value is returned.
	 *	@return true if the key-code value is available in this item.
	 */
	virtual bool		GetKeyCode(UTF8String& keyCode) const = 0;
	
	/**
	 *	@brief	Returns a modifiers value.
	 *	@param[out]	modifiers	a modifiers value is returned.
	 *	@return true if the modifiers value is available in this item.
	 */
	virtual bool		GetModifiers(UTF8String& modifiers) const = 0;

	/**
	 *	@brief	Returns a function value.
	 *	@param[out]	function	a function value is returned.
	 *	@return true if the function value is available in this item.
	 */
	virtual bool		GetFunction(UTF8String& function) const = 0;
};

/**
 *	@brief	This class represents an entire set of key mappings.
 */
class KeyMappings
{
public:
	typedef Enumerator<KeyMappingItem>	KeyMappingItemEnumerator;

	enum Version
	{
		Version_None		= 0,
		Version_1			= 1,
		
		Version_Current		= Version_1
	};

public:
						KeyMappings();
	virtual				~KeyMappings();

//	void				CreateNew();
	void				Load(const Path& xmlFilePath);
	void				Load(const Byte* buffer, SInt32 bufferSize);
//	void				Save(const Path& xmlFilePath);
	
	SInt32				GetVersion() const;
	void				SetVersion(SInt32 version);
	
	bool				GetPlatform(UTF8String& platform) const;
	void				SetPlatform(ConstUTF8Str platform);
	
	bool				GetTitle(UTF8String& title) const;
	void				SetTitle(ConstUTF8Str title);
	
	void				AddItem(ConstUTF8Str category, KeyMappingItem* item);
	void				RemoveItem(ConstUTF8Str category, KeyMappingItem* item);
	void				RemoveAllItem(ConstUTF8Str category);

	KeyMappingItemEnumerator*	NewItemEnumeratorFor(ConstUTF8Str category) const;

private:
	void				checkDocument(NCDDocument* document);
	void				checkApexElement(NCDElement* apexElement);
	NCDElement*			getApexElement() const;
	NCDElement*			findCategoryElement(ConstUTF8Str category) const;

private:
	NCDDocument*		loadedDocument;		///< loaded dom document
};

#endif // _KEYMAPPINGS_H_
