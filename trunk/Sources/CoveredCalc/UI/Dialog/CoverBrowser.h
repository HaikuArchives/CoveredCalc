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
	@file		CoverBrowser.h
	@brief		Definition of CoverBrowser class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.11 created
*/

#ifndef _COVERBROWSER_H_
#define _COVERBROWSER_H_

#include "CoverListManager.h"
#include "UICEventHandler.h"

class UICMultiColumnList;

// ---------------------------------------------------------------------
//! Cover browser window
// ---------------------------------------------------------------------
class CoverBrowser : public UICEventHandler
{
public:
									CoverBrowser();
	virtual							~CoverBrowser();

	void							SetCoversFolderPath(const Path& path)
													{ this->coversFolderPath = path; }

	virtual	void					GetUIRect(Rect32& rect) const = 0;
	virtual void					HandleUICEvent(SInt32 componentID, int eventCode, SInt32 param1, void* param2);

protected:
	Point32							getInitialLocation();
	void							readyToShow();
	void							onDestroy();

	enum ComponentID
	{
		CID_CoverList,
		CID_ReloadButton,
		CID_ApplyButton,
		CID_CloseButton,
	};

	virtual UICMultiColumnList*		getCoverList() = 0;

private:
	void							doUpdateList();
	void							handleReloadButtonClicked();
	void							handleApplyButtonClicked();
	void							handleCloseButtonClicked();

private:
	CoverListManager				listManager;
	Path							coversFolderPath;				//!< 'Covers' folder path
};

#endif // _COVERBROWSER_H_
