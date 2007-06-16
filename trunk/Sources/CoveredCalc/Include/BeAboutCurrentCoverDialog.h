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
	@file		BeAboutCurrentCoverDialog.h
	@brief		Definition of BeAboutCurrentCoverDialog class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.24 created
*/

#ifndef _BEABOUTCURRENTCOVERDIALOG_H_
#define _BEABOUTCURRENTCOVERDIALOG_H_

#include "BeDialog.h"
#include "AboutCurrentCoverDlg.h"

class BeDialogDesign;

// ---------------------------------------------------------------------
//! About current cover dialog on BeOS.
// ---------------------------------------------------------------------
class BeAboutCurrentCoverDialog : public BeDialog, public AboutCurrentCoverDlg
{
public:
								BeAboutCurrentCoverDialog(BeDialogDesign* dialogDesign);
	virtual						~BeAboutCurrentCoverDialog();
	
	virtual void				Init();

protected:
	virtual void				MessageReceived(BMessage *message);
	virtual bool				QuitRequested();

	virtual	void				setDataToDialog(const CoverDef* coverDef);
#if defined (ZETA)
	virtual void				languageChanged();
#endif

private:
	void						createViews();
	void						setDataToDialogInThisThread(const CoverDef* coverDef);

private:
	BeDialogDesign*				dialogDesign;
};

#endif // _BEABOUTCURRENTCOVERDIALOG_H_
