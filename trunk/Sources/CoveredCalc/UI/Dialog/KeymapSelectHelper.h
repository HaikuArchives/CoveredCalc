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
	@file		KeymapSelectHelper.h
	@brief		Definition of KeymapSelectHelper class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.02.13 created
*/

#ifndef _KEYMAPSELECTHELPER_H_
#define _KEYMAPSELECTHELPER_H_

#include <vector>
#include "MBCString.h"
#include "Path.h"

class UICSeparatorListBox;
class KeyMappingsInfo;

/**
 *	@brief	A helper class for a keymap selection combobox in a dialog.
 */
class KeymapSelectHelper
{
public:
	enum CheckResult
	{
		Check_OK = 0,				///< Check OK.
		Check_INVALID_SELECTION,	///< Invalid keymap is selected.
		Check_GET_SELECTION			///< Failed to get user selection.
	};
	enum KMCategory
	{
		KMCategory_Invalid		= -1,	///< invalid category
		KMCategory_Application	= 0,	///< application defined key-mappings
		KMCategory_User			= 1		///< user defined key-mappings
	};
	struct KeyMappingsInfo
	{
		KMCategory	category;			///< key-mapping category
		MBCString	title;				///< title.
		Path		keyMapFilePath;		///< key-mapping file path. (virtual path)
	};

public:
									KeymapSelectHelper();
	virtual							~KeymapSelectHelper();

	void							Init(UICSeparatorListBox* keymapListComponent);
	void							ReloadKeyMappingsInfos(const Path& currentKeyMappingPath);
	void							DeleteFromList(const KeyMappingsInfo* info);
	void							SetCurrentKeymap(const Path& currentKeyMappingPath);
	CheckResult						GetCurrentKeymap(const KeyMappingsInfo*& outInfo);

private:
	typedef std::vector<KeyMappingsInfo*>	KeyMappingsInfoPtrVector;

private:
	void							loadKeyMappingsInfosInFolder(const Path& virtualFolderPath, KMCategory category);
	void							loadOneKeyMappingsInfo(const Path& realKeymapFilePath, const Path& virtualKeymapFilePath, KMCategory category);
	void							unloadKeyMappingsInfos();


	class KeymapItemSortPredicate
	{
	public:
		bool						operator()(KeymapSelectHelper::KeyMappingsInfo* first, KeymapSelectHelper::KeyMappingsInfo* second);
	};

private:
	UICSeparatorListBox*			keymapListComponent;	///< combobox component
	KeyMappingsInfoPtrVector		keyMappingsInfos;		///< installed key-mappings.
};

#endif // _KEYMAPSELECTHELPER_H_

