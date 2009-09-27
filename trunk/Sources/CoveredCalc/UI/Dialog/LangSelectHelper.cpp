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
	@file		LangSelectHelper.cpp
	@brief		Implementation of LangSelectHelper class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.01.24 created
*/

#include "Prefix.h"
#include "LangSelectHelper.h"
#include "LangFileInfo.h"
#include <algorithm>
#include "CoveredCalcApp.h"
#include "UICListBox.h"
#if defined(ZETA)
#include <locale/Locale.h>
#endif // defined(ZETA)

#define INVALID_ITEM		(-1)
#if defined(ZETA)
#define ZETA_LOCALE_KIT		(-2)
#endif

#if defined(ZETA)
ConstAStr LangSelectHelper::LANGMENU_LOCALEKIT = ALITERAL("Use ZETA's Locale Kit");
#endif // defined(ZETA)

/**
 *	@brief	Constructor
 */
LangSelectHelper::LangSelectHelper()
{
	langListComponent = NULL;
	langFileInfos = NULL;
}

/**
 *	@brief	Destructor
 */
LangSelectHelper::~LangSelectHelper()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	langListComponent	target language list component in the dialog.
 */
void LangSelectHelper::Init(UICListBox* langListComponent)
{
	this->langListComponent = langListComponent;
}

/**
 *	@brief	Compares indexes in display order.
 *	@param[in]	ix1	index 1
 *	@param[in]	ix2	index 2
 *	@return		true if ix1 < ix2
 */
bool LangSelectHelper::LangItemSortPredicate::operator()(SInt32 ix1, SInt32 ix2)
{
	const LangFileInfo& info1 = langFileInfos->GetAt(ix1);
	const LangFileInfo& info2 = langFileInfos->GetAt(ix2);

	const MBCString& name1 = info1.GetLanguageName();
	const MBCString& name2 = info2.GetLanguageName();
	
	return (name1.Compare(name2) < 0);

}

/**
 *	@brief	Sets the content of language component and update current selection.
 *	@param[in]	langFileInfos			collection of language popup menu item
 *	@param[in]	currentLangFilePath		current selection
 *	@param[in]	isLocaleKitAvailable	whether locale kit is used (ZETA only)
 */
void LangSelectHelper::SetToComponent(const LangFileInfoCollection* langFileInfos, const Path& currentLangFilePath
#if defined(ZETA)
								, bool isLocaleKitAvailable
#endif
)
{
	this->langFileInfos = langFileInfos;
	sortedIndexes.clear();
	SInt32 selectedIndex = INVALID_ITEM;
	SInt32 count = langFileInfos->GetCount();
	SInt32 index;
	for (index = 0; index < count; index++)
	{
		const LangFileInfo& info = langFileInfos->GetAt(index);
		sortedIndexes.push_back(index);
		
		if (0 == info.GetPath().Compare(currentLangFilePath))
		{
			selectedIndex = index;
		}
	}

	// sort in display order
	LangItemSortPredicate pred(langFileInfos);
	std::sort(sortedIndexes.begin(), sortedIndexes.end(), pred);

#if defined(ZETA)
	sortedIndexes.insert(sortedIndexes.begin(), ZETA_LOCALE_KIT);
	if (currentLangFilePath.IsEmpty() && isLocaleKitAvailable)
	{
		selectedIndex = ZETA_LOCALE_KIT;
	}
#endif
	if (INVALID_ITEM == selectedIndex)
	{
		sortedIndexes.push_back(INVALID_ITEM);
	}

	// set to the component
	langListComponent->RemoveAllItem();
	SInt32 selectedCompIndex = -1;
	std::vector<SInt32>::const_iterator itr;
	for (itr = sortedIndexes.begin(); itr != sortedIndexes.end(); ++itr)
	{
		SInt32 itemIndex = *itr;
		MBCString itemLabel;
		if (INVALID_ITEM == itemIndex)
		{
			// FIXME: do we have a native string?
			itemLabel = ALITERAL("# invalid language #");
		}
#if defined(ZETA)
		else if (ZETA_LOCALE_KIT == itemIndex)
		{
			itemLabel = _T(LANGMENU_LOCALEKIT);
		}
#endif
		else
		{
			itemLabel = langFileInfos->GetAt(itemIndex).GetLanguageName();
		}
		SInt32 compIndex = langListComponent->AddItem(itemLabel, reinterpret_cast<void*>(itemIndex));
		if (selectedIndex == itemIndex)
		{
			selectedCompIndex = compIndex;
		}
	}
	if (selectedCompIndex >= 0)
	{
		langListComponent->SetSelectedItem(selectedCompIndex);
	}
}

/**
 *	@brief	Retrieves current language from the component.
 *	@param[out]	outLangFilePath			language file path
 *	@param[out]	outIsLocaleKitAvailable	whether locale kit is used.
 *	@return	check result.
 *	@note	The output values are available when Check_OK is returned.
 */
LangSelectHelper::CheckResult LangSelectHelper::GetFromComponent(Path& outLangFilePath
#if defined(ZETA)
								, bool& outIsLocaleKitAvailable
#endif
)
{
#if defined(ZETA)
	outIsLocaleKitAvailable = false;
#endif

	SInt32 selectedCompIndex = langListComponent->GetSelectedItem();
	if (selectedCompIndex < 0)
	{
		return Check_GET_SELECTION;
	}
	SInt32 selectedIndex = reinterpret_cast<SInt32>(langListComponent->GetItemData(selectedCompIndex));
	if (INVALID_ITEM == selectedIndex)
	{
		return Check_INVALID_SELECTION;
	}
#if defined(ZETA)
	else if (ZETA_LOCALE_KIT == selectedIndex)
	{
		outLangFilePath = Path();
		outIsLocaleKitAvailable = true;
		return Check_OK;
	}
#endif
	else
	{
		const LangFileInfo& langFileInfo = langFileInfos->GetAt(selectedIndex);
		outLangFilePath = langFileInfo.GetPath();
		return Check_OK;
	}
}
