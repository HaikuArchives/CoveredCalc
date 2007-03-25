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
	@file		AppSettings.h
	@brief		Definition of AppSettings class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.08 created
*/

#ifndef _APPSETTINGS_H_
#define _APPSETTINGS_H_

#include "Path.h"
#include "SettingValue.h"
#include "File.h"

class NCDDocument;

#include "MBCString.h"

// ---------------------------------------------------------------------
//! This class holds application setting informations
// ---------------------------------------------------------------------
class AppSettings
{
public:
						AppSettings();
	virtual				~AppSettings();

	void				Load(const Path& settingFileName);
	void				LoadDefault();
	void				Save();
	void				SaveAs(const Path& settingFileName);

	const Point32&		GetLastMainWindowPos() const
							{ return lastMainWindowPos; }
	void				SetLastMainWindowPos(const Point32& lastMainWindowPos)
							{ this->lastMainWindowPos = lastMainWindowPos; }
	const Point32&		GetLastCoverBrowserPos() const
							{ return lastCoverBrowserPos; }
	void				SetLastCoverBrowserPos(const Point32& lastCoverBrowserPos)
							{ this->lastCoverBrowserPos = lastCoverBrowserPos; }
	bool				IsCoverBrowserVisible()
							{ return isCoverBrowserVisible; }
	void				SetCoverBrowserVisible(bool isVisible)
							{ this->isCoverBrowserVisible = isVisible; }
	const Path&			GetLastCoverDef() const
							{ return lastCoverDef; }
	void				SetLastCoverDef(const Path& lastCoverDef)
							{ this->lastCoverDef = lastCoverDef; }
	SInt32				GetLastCoverNo() const
							{ return lastCoverNo; }
	void				SetLastCoverNo(SInt32 lastCoverNo)
							{ this->lastCoverNo = lastCoverNo; }
	UInt32				GetKeyAnimationWait() const
							{ return 100; }			// currently, this is constant.
	const Path&			GetBaseFolder() const
							{ return baseFolder; }
	void				SetBaseFolder(const Path& baseFolder)
							{ this->baseFolder = baseFolder; }
	bool				IsMainWindowAlwaysOnTop() const
							{ return isMainWindowAlwaysOnTop; }
	void				SetMainWindowAlwaysOnTop(bool isFlagOn)
							{ this->isMainWindowAlwaysOnTop = isFlagOn; }
	bool				IsMainWindowLocked() const
							{ return isMainWindowLocked; }
	void				SetMainWindowLocked(bool isLocked)
							{ this->isMainWindowLocked = isLocked; }
	const Path&			GetLanguageFilePath() const
							{ return langFilePath; }
	void				SetLanguageFilePath(const Path& langFilePath)
							{ this->langFilePath = langFilePath; }
#if defined(ZETA)
	bool				IsLocaleKitAvailable() const
							{ return isLocaleKitAvailable; }
	void				SetLocaleKitAvailable(bool isLocaleKitAvailable)
							{ this->isLocaleKitAvailable = isLocaleKitAvailable; }
#endif	// defined(ZETA)

public:
	static const Path	Value_LangFileBuiltIn;		///< this value is for built-in language file.

private:
	void				loadDefaultToMember();
	bool				getSettingValueFromDOM(ConstUTF8Str key, SettingValue& outValue);
	void				setSettingValueToDOM(ConstUTF8Str key, const SettingValue& value);
	void				syncMembersWithDOM(bool isStoreToDOM);

private:
	Path				settingFileName;			//!< file in which this setting is written
	NCDDocument*		domDocument;				//!< DOM document tree of the setting
	
	Path				baseFolder;					//!< base folder. if this is empty, application's installed folder should be used.
	Path				lastCoverDef;				//!< last cover's definition file in relative path from base folder
	SInt32				lastCoverNo;				//!< last cover number
	Point32				lastMainWindowPos;			//!< top left point of last main window position in screen coordinates
	Point32				lastCoverBrowserPos;		//!< top left point of last cover browser position in screen coordinates
	bool				isCoverBrowserVisible;		//!< whether cover browser is visible
	bool				isMainWindowAlwaysOnTop;	///< whether main window is always on top
	bool				isMainWindowLocked;			///< whether main window is locked
	Path				langFilePath;				///< language file path.
#if defined(ZETA)
	bool				isLocaleKitAvailable;		///< whether uses ZETA's Locale Kit.
#endif	// defined(ZETA)
};

#endif // _APPSETTINGS_H_
