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
	@file		BePreferencesDlg.h
	@brief		Definition of BePreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.02.27 created
*/

#ifndef _BEPREFERENCESDLG_H_
#define _BEPREFERENCESDLG_H_

#include "BeDialog.h"
#include "PreferencesDlg.h"
#include <List.h>

class BMenu;

/**
 *	@brief	Preferences dialog on BeOS.
 */
class BePreferencesDlg : public BeDialog, public PreferencesDlg
{
public:
								BePreferencesDlg();
	virtual						~BePreferencesDlg();
	
protected:
	virtual void				initDialog();
	virtual void				MessageReceived(BMessage *message);
#if defined (ZETA)
	virtual void				languageChanged();
#endif

	virtual MessageBoxProvider*	getMessageBoxProvider();
	virtual bool				showEditKeyMapDialog(bool isReadOnly, KeyMappings& keyMappings);
	
	virtual void				setLanguage(const LangFileInfoCollection& langFileInfos, const Path& currentLangFilePath
#if defined(ZETA)
										, bool isLocaleKitAvailable
#endif
								);
	virtual	bool				getLanguage(Path& langFilePath
#if defined(ZETA)
										, bool& isLocaleKitAvailable
#endif
								);

	virtual void				setKeyMapping(const KeyMappingsInfoPtrVector& keyMappingsInfos, const Path& currentKeyMappingPath);
	virtual const PreferencesDlg::KeyMappingsInfo*	getKeyMapping(bool doErrorProcessing);
	virtual void				enableEditKeyMapping(bool isEnabled);
	virtual void				enableDuplicateKeyMapping(bool isEnabled);
	virtual void				enableDeleteKeyMapping(bool isEnabled);

private:
	void						createViews();

private:
	BMenu*						langMenu;			///< language popup menu
	BList						langMenuItemInfo;	///< informations of each language popup menu item
	BMenu*						keyMappingMenu;		///< key-mapping popup menu
};

#endif // _BEPREFERENCESDLG_H_
