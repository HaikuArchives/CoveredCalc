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
	@file		PreferencesDlg.h
	@brief		Definition of PreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.1.15 created
*/

#ifndef _PREFERENCESDLG_H_
#define _PREFERENCESDLG_H_

#include <vector>
#include "Path.h"
#include "LangSelectHelper.h"
#include "KeymapSelectHelper.h"

class LangFileInfoCollection;
class MessageBoxProvider;
class KeyMappings;
class UICSlider;
class UICListBox;
class UICSeparatorListBox;
class UICButton;

/**
 *	@brief	This is a base class of preferences dialog class.
 */
class PreferencesDlg
{
public:
									PreferencesDlg();
	virtual							~PreferencesDlg();
	
protected:
	void							loadToDialog();
	bool							saveFromDialog();
	
	void							processKeyMappingSelectionChanged();
	void							doEditKeyMapping();
	void							doDuplicateKeyMapping();
	void							doDeleteKeyMapping();
	
	Path							createUniqueUserKeyMappingFile(const Path& folderPath);

protected:
	/**
	 *	@brief	Returns MessageBoxProvider object.
	 *	@return	MessageBoxProvider object.
	 */
	virtual MessageBoxProvider*		getMessageBoxProvider() = 0;

#if defined (WIN32)
	virtual UICSlider*				getOpacitySlider() = 0;
	virtual UICSlider*				getEdgeSmoothingSlider() = 0;
#endif
	virtual UICListBox*				getLanguageListBox() = 0;
	virtual UICSeparatorListBox*	getKeyMapListBox() = 0;
	virtual UICButton*				getEditKeyMapButton() = 0;
	virtual UICButton*				getDuplicateKeyMapButton() = 0;
	virtual UICButton*				getDeleteKeyMapButton() = 0;

	/**
	 *	@brief	Shows "Edit Keyboard" dialog and wait until it is closed.
	 *	@param[in]		isReadOnly	true when read-only mode.
	 *	@param[in,out]	keyMappings	Key-mappings definition.
	 *	@retval	true	user closes the dialog by OK button.
	 *	@retval	false	user closes the dialog by Cancel button.
	 */
	virtual bool				showEditKeyMapDialog(bool isReadOnly, KeyMappings& keyMappings) = 0;

private:
	LangSelectHelper			langSelectHelper;		///< "Language" component helper.
	KeymapSelectHelper			keymapSelectHelper;		///< "Keymap" component helper.
};

#endif // _PREFERENCESDLG_H_
