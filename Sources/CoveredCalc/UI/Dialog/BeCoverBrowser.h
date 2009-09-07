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
	@file		BeCoverBrowser.h
	@brief		Definition of BeCoverBrowser class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.09.12 created
*/


#ifndef _BECOVERBROWSER_H_
#define _BECOVERBROWSER_H_

#include "BeDialog.h"
#include "CoverBrowser.h"
#include "UICMultiColumnListImpl.h"

class BColumnListView;

// ---------------------------------------------------------------------
//! Cover browser window on BeOS
// ---------------------------------------------------------------------
class BeCoverBrowser : public BeDialog, public CoverBrowser
{
public:
									BeCoverBrowser();
	virtual							~BeCoverBrowser();

	virtual void					Quit();
	
	virtual void					GetUIRect(Rect32& rect) const;

protected:
	virtual void					MessageReceived(BMessage *message);
	virtual	bool					QuitRequested();
	
protected:
	virtual void					initDialog();
	void							createViews();

	virtual UICMultiColumnList*		getCoverList() { return &uicCoverList; }

#if defined (ZETA)
	virtual void					languageChanged();
#endif

private:
	void							moveToInitialLocation();

private:
	BColumnListView*				coverList;			///< cover list

	// adapters
	UICMultiColumnListImpl			uicCoverList;
};

#endif // _BECOVERBROWSER_H_
