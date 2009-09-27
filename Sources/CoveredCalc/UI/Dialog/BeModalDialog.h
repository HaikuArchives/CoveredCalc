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
	@file		BeModalDialog.h
	@brief		Definition of modal dialog related classes (for BeOS)
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.09.22 created
*/

#ifndef _BEMODALDIALOG_H_
#define _BEMODALDIALOG_H_

#include "BeDialog.h"

class BeModalDialogWindow;

/**
 * @brief This class shows dialog and wait until the dialog is closed.
 */
class BeModalDialog
{
public:
	/**
	 * @brief Result of dialog
	 */
	enum DlgResult
	{
		DlgResult_Error = -1,	///< error
		DlgResult_Cancel = 0,	///< cancel
		DlgResult_OK = 1,		///< OK
	};

public:
							BeModalDialog();
	virtual					~BeModalDialog();
						
	SInt32					DoModal(BWindow* parent);

protected:
	virtual ConstAStr		getSemaphoreName() = 0;

protected:
	BeModalDialogWindow*	dialogWindow;	///< dialog
	SInt32					dialogResult;	///< return value of DoModal()
};

/**
 * @brief Modal dialog class which works with BeModalDialog
 */
class BeModalDialogWindow : public BeDialog
{
public:
						BeModalDialogWindow(SInt32 dialogID, window_type type, uint32 flags, uint32 workspaces = B_CURRENT_WORKSPACE);
						BeModalDialogWindow(SInt32 dialogID, window_look look, window_feel feel, uint32 flags, uint32 workspaces = B_CURRENT_WORKSPACE);
	virtual				~BeModalDialogWindow();

	void				SetSemaphore(sem_id semDialog);

protected:
	virtual bool		QuitRequested();
	
private:
	sem_id				semDialog;	///< semaphore which is used to wait dialog
};

#endif // _BEMODALDIALOG_H_
