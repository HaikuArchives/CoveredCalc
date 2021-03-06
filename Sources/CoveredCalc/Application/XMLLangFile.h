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

/**
	@file		XMLLangFile.h
	@brief		Definition of XMLLangFile class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.12.20 created
*/

#ifndef _XMLLANGFILE_H_
#define _XMLLANGFILE_H_

class NCDDocument;
class NCDElement;
class Path;
class DialogLayout;

#include <map>
#include <vector>
#include "UTF8String.h"
#include "MBCString.h"
#if defined (WIN32)
#include "DialogFont.h"
#endif

/**
 *	@brief	Language file written in XML.
 */
class XMLLangFile
{
public:
#if defined (WIN32)
	typedef std::vector<DialogFont>	DialogFontVector;
#endif

public:
								XMLLangFile();
	virtual						~XMLLangFile();

	void						Load(const Path& langFilePath);
	void						Unload();

	bool						IsReady() const { return isReady; }

	const NCDElement*			GetDialogElement(ConstUTF8Str name) const;
	const NCDElement*			GetStringElement(ConstUTF8Str name) const;

	void						GetLanguageName(MBCString& langName) const;
	void						GetLanguageCode(MBCString& langCode) const;
	void						GetBaseDefSource(MBCString& fileName) const;

	bool						LoadString(ConstUTF8Str name, MBCString& message) const;
	void						LoadDialogLayout(ConstUTF8Str name, DialogLayout& layout) const;
#if defined (WIN32)
	void						LoadDialogFont(DialogFontVector& outDialogFonts) const;
#endif

#if defined (WIN32)
	static MBCString			ConvertAccessMnemonic(const MBCString& srcString);
#endif

private:
	void						validateFirst();
	void						makeTables();
	void						validateSecond();
	bool						checkVersion(ConstUTF8Str version);

private:
	typedef std::map<UTF8String, const NCDElement*>	NameToElementMap;

private:
	bool						isReady;			///< true if language file is loaded.
	NCDDocument*				document;			///< DOM tree.
	NameToElementMap			dialogElementMap;	///< map to retrieve a dialog element from name.
	NameToElementMap			stringElementMap;	///< map to retrieve a string element from name.
	MBCString					langName;			///< language name
	MBCString					langCode;			///< language code
	MBCString					baseDefSource;		///< baseDef source
};

#endif // _XMLLANGFILE_H_
