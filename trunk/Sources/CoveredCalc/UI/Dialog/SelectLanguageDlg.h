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
	@file		SelectLanguageDlg.h
	@brief		Definition of SelectLanguageDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.9.13 created
*/

#ifndef _SELECTLANGUAGEDLG_H_
#define _SELECTLANGUAGEDLG_H_

#include "LangSelectHelper.h"
#include "UICEventHandler.h"
#include "Path.h"

class UICListBox;
class MessageBoxProvider;

/**
 *	@brief	This is a base class of select-language dialog class.
 */
class SelectLanguageDlg : public UICEventHandler
{
public:
									SelectLanguageDlg();
	virtual							~SelectLanguageDlg();

	void							SetRelativeLangFilePath(const Path& langFilePath) { this->langFilePath = langFilePath; }
	const Path&						GetRelativeLangFilePath() const { return langFilePath; }

#if defined(ZETA)
	void							SetIsLocaleKitAvailable(bool isLocaleKitAvailable) { this->isLocaleKitAvailable = isLocaleKitAvailable; }
	bool							IsLocaleKitAvailable() const { return isLocaleKitAvailable; }
#endif

	virtual void					HandleUICEvent(SInt32 componentID, int eventCode, SInt32 param1, void* param2);

protected:
	void							readyToShow();

protected:
	/**
	 *	@brief	Returns MessageBoxProvider object.
	 *	@return	MessageBoxProvider object.
	 */
	virtual MessageBoxProvider*		getMessageBoxProvider() = 0;

	enum ComponentID
	{
		CID_LanguageListBox,
		CID_OKButton,
		CID_CancelButton,
	};

	virtual UICListBox*				getLanguageListBox() = 0;

	/**
	 *	@brief	Close this dialog.
	 *	@param[in]	isOK	true if dialog is closing by OK button.
	 */
	virtual void					closeDialog(bool isOK) = 0;
private:
	void							handleOK();
	void							handleCancel();

private:
	LangSelectHelper				langSelectHelper;		///< "Language" component helper.
	Path							langFilePath;	///< language file path.
#if defined(ZETA)
	bool							isLocaleKitAvailable;	///< whether locale kit is used (ZETA only)
#endif
};

#endif // _SELECTLANGUAGEDLG_H_
