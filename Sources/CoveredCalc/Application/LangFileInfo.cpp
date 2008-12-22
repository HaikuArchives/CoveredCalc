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
	@file		LangFileInfo.cpp
	@brief		Implementation of LangFileInfo class and LangFileInfoCollection class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.01.08 created
*/

#include "Prefix.h"
#include "LangFileInfo.h"
#include "Exception.h"
#include "AppSettings.h"
#include "XMLLangFile.h"
#if defined (BEOS)
#include <StorageKit.h>
#endif

/**
 *	@brief	Constructor
 */
LangFileInfoCollection::LangFileInfoCollection()
{
	defaultLangFileIndex = -1;
}

/**
 *	@brief	Destructor
 */
LangFileInfoCollection::~LangFileInfoCollection()
{
	removeAllInfos();
}

/**
 *	@brief	Reads informations about language files in the specified folder.
 *
 *	[Windows] This function loads the information of Japanese resource
 *	which is not exist as a language file but is included in default resource
 *	of CoveredCalc.exe.
 */
void LangFileInfoCollection::Load(
	const Path& langFileFolder			///< language files folder.
)
{
	removeAllInfos();

#if defined (WIN32)
	// add information of language files.
	Path findPath = langFileFolder.Append(ALITERAL("*.cclxw"));
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile(findPath.GetPathString(), &findData);
	if (NULL != hFind)
	{
		do
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				continue;
			}

			Path fileName(findData.cFileName);
			Path langFilePath = langFileFolder.Append(fileName);
			try
			{
				XMLLangFile langFile;
				langFile.Load(langFilePath);
				MBCString languageName;
				langFile.GetLanguageName(languageName);

				LangFileInfo* newInfo = new LangFileInfo();
				newInfo->SetPath(langFilePath);
				newInfo->SetLanguageName(languageName);
				langFileInfos.push_back(newInfo);

				langFile.Unload();

				if (0 == fileName.Compare(Path(ALITERAL("enUS.cclxw"))))
				{
					defaultLangFileIndex = langFileInfos.size() - 1;
				}
			}
			catch (Exception* ex)
			{
				// ignore
				ex->Delete();
			}
		}
		while (::FindNextFile(hFind, &findData));
		::FindClose(hFind);
	}
#elif defined (BEOS)
	// add information of language files.
	BDirectory findDir(langFileFolder.GetPathString());
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
				if (6 < length && 0 == strcmp(filename + length - 6, ALITERAL(".cclxb")))
				{
					BPath bpath;
					entry.GetPath(&bpath);
					Path langFilePath(bpath.Path());
					try
					{
						XMLLangFile langFile;
						langFile.Load(langFilePath);
						MBCString languageName;
						langFile.GetLanguageName(languageName);
						
						LangFileInfo* newInfo = new LangFileInfo();
						newInfo->SetPath(langFilePath);
						newInfo->SetLanguageName(languageName);
						langFileInfos.push_back(newInfo);

						langFile.Unload();

						if (0 == strcmp(filename, "enUS.cclxb"))
						{
							defaultLangFileIndex = langFileInfos.size() - 1;
						}
					}
					catch (Exception* ex)
					{
						// ignore
						ex->Delete();
					}
				}
			}
		}
	}
#endif
}

/**
 *	@brief	Removes all informations in this object.
 */
void LangFileInfoCollection::removeAllInfos()
{
	LangFileInfoPtrVector::iterator iter;
	for (iter=langFileInfos.begin(); iter!=langFileInfos.end(); iter++)
	{
		LangFileInfo* info = *iter;
		delete info;
	}
	langFileInfos.clear();
	defaultLangFileIndex = -1;
}
