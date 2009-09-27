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
	@file		LangSelectHelper.h
	@brief		Definition of LangSelectHelper class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.01.24 created
*/

#ifndef _LANGSELECTHELPER_H_
#define _LANGSELECTHELPER_H_

#include <vector>

class UICListBox;
class LangFileInfoCollection;
class Path;

/**
 *	@brief	A helper class for a language selection combobox in a dialog.
 */
class LangSelectHelper
{
#if defined(ZETA)
public:
	static ConstAStr LANGMENU_LOCALEKIT;
#endif // defined(ZETA)
	
public:
	enum CheckResult
	{
		Check_OK = 0,				///< Check OK.
		Check_INVALID_SELECTION,	///< Invalid language is selected.
		Check_GET_SELECTION			///< Failed to get user selection.
	};

public:
									LangSelectHelper();
	virtual							~LangSelectHelper();

	void							Init(UICListBox* langListComponent);
	void							SetToComponent(const LangFileInfoCollection* langFileInfos, const Path& currentLangFilePath
#if defined(ZETA)
													, bool isLocaleKitAvailable
#endif
									);
	CheckResult						GetFromComponent(Path& outLangFilePath
#if defined(ZETA)
													, bool& outIsLocaleKitAvailable
#endif
									);

private:
	class LangItemSortPredicate
	{
	public:
									LangItemSortPredicate(const LangFileInfoCollection* langFileInfos) { this->langFileInfos = langFileInfos; }
									LangItemSortPredicate(const LangItemSortPredicate& other) { this->langFileInfos = other.langFileInfos; }
									~LangItemSortPredicate() {}
		LangItemSortPredicate&		operator=(const LangItemSortPredicate& other) { this->langFileInfos = other.langFileInfos; return *this; }
		bool						operator()(SInt32 ix1, SInt32 ix2);
	private:
		const LangFileInfoCollection* langFileInfos;
	};

private:
	UICListBox*						langListComponent;			///< combobox component
	const LangFileInfoCollection*	langFileInfos;				///< language files
	std::vector<SInt32>				sortedIndexes;				///< indexes of lang files in display order.
};

#endif // _LANGSELECTHELPER_H_

