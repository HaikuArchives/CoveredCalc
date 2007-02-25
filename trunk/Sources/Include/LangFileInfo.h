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
	@file		LangFileInfo.h
	@brief		Definition of LangFileInfo class and LangFileInfoCollection class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.01.08 created
*/

#ifndef _LANGFILEINFO_H_
#define _LANGFILEINFO_H_

#include "Path.h"
#include "MBCString.h"
#include <vector>

/**
 *	@brief	This class holds informations about a language file.
 */
class LangFileInfo
{
public:
								LangFileInfo() {
#if defined(WIN32)
									langID = 0;
#endif
								}
	virtual						~LangFileInfo() {}

	void						SetPath(const Path& path)				{ this->path = path; }
	void						SetLanguageName(ConstAStr languageName)	{ this->languageName = languageName; }
#if defined(WIN32)
	void						SetLangID(LANGID langID)				{ this->langID = langID; }
#endif

	const Path&					GetPath() const			{ return path; }
	const MBCString&			GetLanguageName() const	{ return languageName; }
#if defined(WIN32)
	LANGID						GetLangID() const		{ return langID; }
#endif

private:
	Path						path;			///< a path of the language file.
	MBCString					languageName;	///< a name of the language.
#if defined(WIN32)
	LANGID						langID;			///< a language identifier of this language file.
#endif
};

/**
 *	@brief	A collection of language file informations.
 */
class LangFileInfoCollection
{
public:
								LangFileInfoCollection();
	virtual						~LangFileInfoCollection();
	
	void						Load(const Path& langFileFolder);
	
	SInt32						GetCount() const			{ return langFileInfos.size(); }
	const LangFileInfo&			GetAt(SInt32 index) const	{ return *langFileInfos[index]; }

private:
	void						removeAllInfos();

private:
	typedef std::vector<LangFileInfo*> LangFileInfoPtrVector;
	LangFileInfoPtrVector		langFileInfos;	///< informations about language file.
};

#endif // _LANGFILEINFO_H_
