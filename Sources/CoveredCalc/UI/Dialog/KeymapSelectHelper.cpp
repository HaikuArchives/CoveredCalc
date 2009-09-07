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
	@file		KeymapSelectHelper.cpp
	@brief		Implementation of KeymapSelectHelper class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.02.14 created
*/

#include "Prefix.h"
#include "KeymapSelectHelper.h"
#include <algorithm>
#include "CoveredCalcApp.h"
#include "VirtualPathNames.h"
#include "KeyMappings.h"
#include "UTF8Conv.h"
#include "Exception.h"
#include "UICSeparatorListBox.h"
#include "UICEventHandler.h"
#include "UICEventCode.h"
#if defined (BEOS)
#include <Directory.h>
#include <Entry.h>
#include <Path.h>
#endif // defubed(BEOS)

/**
 *	@brief	Constructor
 */
KeymapSelectHelper::KeymapSelectHelper()
{
	keymapListComponent = NULL;
	componentID = -1;
	uicEventHandler = NULL;
}

/**
 *	@brief	Destructor
 */
KeymapSelectHelper::~KeymapSelectHelper()
{
	unloadKeyMappingsInfos();
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	keymapListComponent	target keymap list component in the dialog.
 *	@param[in]	componentID			ID of the target keymap list component.
 *	@param[in]	eventHandler		UIComponent event handler.
 */
void KeymapSelectHelper::Init(UICSeparatorListBox* keymapListComponent, SInt32 componentID, UICEventHandler* uicEventHandler)
{
	this->keymapListComponent = keymapListComponent;
	this->componentID = componentID;
	this->uicEventHandler = uicEventHandler;
}

/**
 *	@brief	Compares objects in display order.
 *	@param[in]	first	object 1
 *	@param[in]	second	object 2
 *	@return		true if object1 < object2
 */
bool KeymapSelectHelper::KeymapItemSortPredicate::operator()(KeymapSelectHelper::KeyMappingsInfo* first, KeymapSelectHelper::KeyMappingsInfo* second)
{
	if (first->category > second->category)
	{
		return false;
	}
	else if (first->category < second->category)
	{
		return true;
	}
	else
	{
		return (0 > first->title.Compare(second->title));
	}
}

/**
 *	@brief	Reloads items and sets current selection.
 *	@param[in]	currentKeyMappingPath	virtual keymapping path for current selection.
 */
void KeymapSelectHelper::ReloadKeyMappingsInfos(const Path& currentKeyMappingPath)
{
	unloadKeyMappingsInfos();
	
	// load files in ${AppKeymaps} folder.
	Path virtualAppKeymaps(ALITERAL("${") VPATH_APP_KEYMAPS ALITERAL("}"));
	loadKeyMappingsInfosInFolder(virtualAppKeymaps, KMCategory_Application);
	
	// load files in ${UserKeymaps} folder.
	Path virtualUserKeymaps(ALITERAL("${") VPATH_USER_KEYMAPS ALITERAL("}"));
	loadKeyMappingsInfosInFolder(virtualUserKeymaps, KMCategory_User);
	
	// sort
	std::sort(keyMappingsInfos.begin(), keyMappingsInfos.end(), KeymapItemSortPredicate());

	// update list component.
	keymapListComponent->RemoveAllItem();
	KMCategory category = KMCategory_Invalid;
	SInt32 count = keyMappingsInfos.size();
	SInt32 index;
	SInt32 compSelectIndex = -1;
	for (index = 0; index < count; ++index)
	{
		const KeyMappingsInfo* info = keyMappingsInfos[index];
		SInt32 compIndex = keymapListComponent->AddItem(info->title, const_cast<KeyMappingsInfo*>(info));
		if (category != info->category)
		{
			if (compIndex > 0)
			{
				keymapListComponent->SetSeparator(compIndex - 1, true);
			}
			category = info->category;
		}
		
		if (0 == info->keyMapFilePath.Compare(currentKeyMappingPath))
		{
			compSelectIndex = compIndex;
		}
	}
	if (compSelectIndex < 0)
	{
		compSelectIndex = keymapListComponent->AddItem(ALITERAL("# invalid key-mapping #"), NULL);	// FIXME: native language?
		if (compSelectIndex > 0)
		{
			keymapListComponent->SetSeparator(compSelectIndex - 1, true);
		}
	}
	keymapListComponent->SetSelectedItem(compSelectIndex);
	if (NULL != uicEventHandler)
	{
		uicEventHandler->HandleUICEvent(componentID, UICE_SelectionChanged, 0, NULL);
	}
}

/**
 *	@breif	Loads informations about installed key-mapping files in specified folder.
 *	@param[in]	virtualFolderPath	folder (in virtual path)
 *	@param[in]	category	category of loading informations.
 */
void KeymapSelectHelper::loadKeyMappingsInfosInFolder(const Path& virtualFolderPath, KeymapSelectHelper::KMCategory category)
{
	Path folder = CoveredCalcApp::GetInstance()->ExpandVirtualPath(virtualFolderPath);

#if defined (WIN32)
	Path findPath = folder.Append(ALITERAL("*.cckxw"));
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile(findPath.GetPathString(), &findData);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				continue;
			}
			loadOneKeyMappingsInfo(folder.Append(findData.cFileName), virtualFolderPath.Append(findData.cFileName), category);
		}
		while (::FindNextFile(hFind, &findData));
		::FindClose(hFind);
	}
#elif defined (BEOS)
	BDirectory findDir(folder.GetPathString());
	if (B_OK == findDir.InitCheck())
	{
		BEntry entry;
		char filename[B_FILE_NAME_LENGTH];

		findDir.Rewind();
		while (B_NO_ERROR == findDir.GetNextEntry(&entry))
		{
			if (!entry.IsDirectory())
			{
				entry.GetName(filename);
				SInt32 length = strlen(filename);
				if (6 < length && 0 == strcmp(filename + length - 6, ALITERAL(".cckxb")))
				{
					BPath bpath;
					entry.GetPath(&bpath);
					Path keymapFilePath(bpath.Path());
					loadOneKeyMappingsInfo(keymapFilePath, virtualFolderPath.Append(filename), category);
				}
			}
		}
	}
#endif	
}

/**
 *	@brief	Loads and validates specified key-mapping file, and append it to keyMappingsInfos if it is valid.
 *	@param[in]	realKeymapFilePath		path of a key-mapping file (in real path)
 *	@param[in]	virtualKeymapFilePath	path of a key-mapping file (in virtual path)
 *	@param[in]	category	category of loading key-mapping.
 */
void KeymapSelectHelper::loadOneKeyMappingsInfo(const Path& realKeymapFilePath, const Path& virtualKeymapFilePath, KeymapSelectHelper::KMCategory category)
{
	try
	{
		KeyMappings keyMappings;
		keyMappings.Load(realKeymapFilePath);
		UTF8String platform;
		if (!keyMappings.GetPlatform(platform) || !CoveredCalcApp::GetInstance()->CheckPlatform(platform))
		{
			return;
		}
		
		UTF8String utf8Title;
		keyMappings.GetTitle(utf8Title);
		
		KeyMappingsInfo* info = new KeyMappingsInfo;
		info->category = category;
		UTF8Conv::ToMultiByte(info->title, utf8Title);
		info->keyMapFilePath = virtualKeymapFilePath;
		keyMappingsInfos.push_back(info);
	}
	catch (Exception* ex)
	{
		// ignore
		ex->Delete();
	}			
}

/**
 *	@brief	Unload informations about installed key-mapping files.
 */
void KeymapSelectHelper::unloadKeyMappingsInfos()
{
	KeyMappingsInfoPtrVector::iterator ite;
	for (ite = keyMappingsInfos.begin(); ite != keyMappingsInfos.end(); ++ite)
	{
		KeyMappingsInfo* info = *ite;
		delete info;
	}
	keyMappingsInfos.clear();
}

/**
 *	@brief	Delete specified keymap item from list.
 *	@param[in]	pointer to keymap information.
 */
void KeymapSelectHelper::DeleteFromList(const KeymapSelectHelper::KeyMappingsInfo* info)
{
	KeyMappingsInfo* deleteInfo = NULL;
	UInt32 infosIndex;
	for (infosIndex = 0; infosIndex < keyMappingsInfos.size(); infosIndex++)
	{
		if (keyMappingsInfos[infosIndex] == info)
		{
			deleteInfo = keyMappingsInfos[infosIndex];
			break;
		}
	}
	if (NULL == deleteInfo)
	{
		return;
	}

	keymapListComponent->RemoveItem(infosIndex);
	keyMappingsInfos.erase(keyMappingsInfos.begin() + infosIndex);
	if (keyMappingsInfos.size() <= infosIndex)
	{
		infosIndex = keyMappingsInfos.size() - 1;
	}
	keymapListComponent->SetSelectedItem(infosIndex);
	if (NULL != uicEventHandler)
	{
		uicEventHandler->HandleUICEvent(componentID, UICE_SelectionChanged, 0, NULL);
	}
	delete deleteInfo;
}

/**
 *	@brief	Select specified keymap item.
 *	@param[in]	currentKeyMappingPath	virtual keymapping path for current selection.
 */
void KeymapSelectHelper::SetCurrentKeymap(const Path& currentKeyMappingPath)
{
	SInt32 invalidIndex = -1;
	SInt32 compSelectIndex = -1;
	SInt32 count = keymapListComponent->GetCount();
	SInt32 index;
	for (index = 0; index < count; ++index)
	{
		const KeyMappingsInfo* info = static_cast<KeyMappingsInfo*>(keymapListComponent->GetItemData(index));
		if (NULL == info)
		{
			invalidIndex = index;
		}
		else
		{
			if (0 == info->keyMapFilePath.Compare(currentKeyMappingPath))
			{
				compSelectIndex = index;
				break;
			}
		}
	}
	if (compSelectIndex < 0)
	{
		if (invalidIndex < 0)
		{
			invalidIndex = keymapListComponent->AddItem(ALITERAL("# invalid key-mapping #"), NULL);	// FIXME: native language?
			if (invalidIndex > 0)
			{
				keymapListComponent->SetSeparator(invalidIndex - 1, true);
			}
		}
		compSelectIndex = invalidIndex;
	}
	keymapListComponent->SetSelectedItem(compSelectIndex);
	if (NULL != uicEventHandler)
	{
		uicEventHandler->HandleUICEvent(componentID, UICE_SelectionChanged, 0, NULL);
	}
}

/**
 *	@brief	Retrives current keymap from the component.
 *	@param[out]	outInfo		pointer to selected keymap information.
 *	@return	check result.
 *	@note	The output value are available when Check_OK is returned.
 */
KeymapSelectHelper::CheckResult	KeymapSelectHelper::GetCurrentKeymap(const KeyMappingsInfo*& outInfo)
{
	SInt32 compSelectIndex = keymapListComponent->GetSelectedItem();
	if (compSelectIndex < 0)
	{
		return Check_GET_SELECTION;
	}
	const KeyMappingsInfo* info = static_cast<KeyMappingsInfo*>(keymapListComponent->GetItemData(compSelectIndex));
	if (NULL == info)
	{
		return Check_INVALID_SELECTION;
	}

	outInfo = info;
	return Check_OK;
}
