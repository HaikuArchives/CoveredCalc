/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2008 CoveredCalc Project Contributors
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
	@file		CoverListManager.cpp
	@brief		Definition of CoverListManager class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.03.07 created
*/

#include "Prefix.h"
#include <memory>
#include "CoverListItem.h"
#include "CoverListManager.h"
#include "Exception.h"
#include "CoverDef.h"
#include "CoverDefParser.h"

#if defined (BEOS)
#include <StorageKit.h>
#endif


// ---------------------------------------------------------------------
//! Constructor.
// ---------------------------------------------------------------------
CoverListManager::CoverListManager()
{
}

// ---------------------------------------------------------------------
//! Destructor.
// ---------------------------------------------------------------------
CoverListManager::~CoverListManager()
{
	removeAllItems();
}

// ---------------------------------------------------------------------
//! Removes all items in the list.
// ---------------------------------------------------------------------
void CoverListManager::removeAllItems()
{
	CoverListVector::iterator ite;
	for (ite=items.begin(); ite!=items.end(); ite++)
	{
		delete *ite;
	}
	items.clear();
}

// ---------------------------------------------------------------------
//! Makes cover list
// ---------------------------------------------------------------------
void CoverListManager::MakeList(
	const Path& coverPath			//!< "Covers" folder path
)
{
	removeAllItems();
	enumCoverDef(coverPath);
	
	SInt32 maxCount = items.size();
	SInt32 index;
	for(index=0; index<maxCount; index++)
	{
		bool success = true;
		try
		{
			loadCoverDef(index);
		}
		catch (Exception* ex)
		{
			ex->Delete();
			success = false;
		}
		catch (...)
		{
			success = false;
		}
		
		if (!success)
		{
			// the cover failed in loading is deleted from a list.
			delete items[index];
			items.erase(items.begin() + index);
			maxCount--;
			index--;		
		}
	}
}

// ---------------------------------------------------------------------
//! Enumerates CoverDef.xml in "Covers" folder
// ---------------------------------------------------------------------
void CoverListManager::enumCoverDef(
	const Path& coverPath
)
{
#if defined (WIN32)
	Path findPath = coverPath.Append(ALITERAL("*.*"));
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile(findPath.GetPathString(), &findData);
	if (NULL != hFind)
	{
		do
		{
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				continue;
			}
			if (0 == lstrcmp(ALITERAL("."), findData.cFileName) || 0 == lstrcmp(ALITERAL(".."), findData.cFileName))
			{
				continue;
			}
			
			Path coverDefPath = coverPath.Append(findData.cFileName).Append(ALITERAL("CoverDef.xml"));
			DWORD attributes = ::GetFileAttributes(coverDefPath.GetPathString());
			if (0xffffffff != attributes && (!(attributes & FILE_ATTRIBUTE_DIRECTORY)))
			{
				CoverListItem* listItem = new CoverListItem();
				listItem->SetPath(coverDefPath);
				items.push_back(listItem);
			}
		}
		while (::FindNextFile(hFind, &findData));
		::FindClose(hFind);
	}

#elif defined (BEOS)
	BDirectory findDir(coverPath.GetPathString());
	if (B_OK == findDir.InitCheck())
	{
		BEntry entry;
		
		findDir.Rewind();
		while (B_NO_ERROR == findDir.GetNextEntry(&entry))
		{
			if (!entry.IsDirectory())
			{
				continue;
			}
			
			BDirectory coverDir(&entry);
			BEntry coverDefEntry;
			
			if (B_OK == coverDir.FindEntry("CoverDef.xml", &coverDefEntry))
			{
				if (!coverDefEntry.IsDirectory())
				{
					BPath bpath;
					coverDefEntry.GetPath(&bpath);
					Path coverDefPath(bpath.Path());
					CoverListItem* listItem = new CoverListItem();
					listItem->SetPath(coverDefPath);
					items.push_back(listItem);
				}
			}			
		}
	}
#else
	ASSERT(false);
#endif
}

// ---------------------------------------------------------------------
//! Loads an information about the cover in specified index.
// ---------------------------------------------------------------------
void CoverListManager::loadCoverDef(
	SInt32 index
)
{
	CoverListItem* listItem = items[index];

	// read "CoverDef.xml"
	CoverDefParser parser;
	parser.Parse(listItem->GetPath());
	std::auto_ptr<CoverDef> coverDef(parser.DetachCoverDef());
	listItem->SetTitle(coverDef->GetTitle());
	listItem->SetDescription(coverDef->GetDescription());
}
