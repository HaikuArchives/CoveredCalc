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
	@file		BeModalDialog.cpp
	@brief		Implemenetation of modal dialog related classes (for BeOS)
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.09.22 created
*/

#include "Prefix.h"
#include "BeModalDialog.h"

static const bigtime_t MODAL_POLLING_WAIT = 100000;

/**
 * @brief Constructor
 */
BeModalDialog::BeModalDialog()
{
	dialogWindow = NULL;
	dialogResult = DlgResult_Error;
}

/**
 * @brief Destructor
 */
BeModalDialog::~BeModalDialog()
{
	if (NULL != dialogWindow)
	{
		delete dialogWindow;
	}
}

/**
 * @brief Show dialog modally.
 * @param[in] parent parent window. if dialog has no parent, specify NULL.
 * @return dialog result.
 */
SInt32 BeModalDialog::DoModal(BWindow* parent)
{
	if (NULL == dialogWindow)
	{
		// dialog window must be created before this function.
		return DlgResult_Error;
	}
	
	sem_id semDialog = create_sem(0, getSemaphoreName());
	if (semDialog < B_NO_ERROR)
	{
		return DlgResult_Error;
	}
	
	try
	{
		dialogWindow->SetSemaphore(semDialog);
		if (NULL != parent)
		{
			dialogWindow->AddToSubset(parent);
		}
		dialogWindow->Show();
		
		status_t acquireResult;
		if (NULL == parent)
		{
			acquireResult = acquire_sem(semDialog);
		}
		else
		{
			do
			{
				acquireResult = acquire_sem_etc(semDialog, 1, B_RELATIVE_TIMEOUT, MODAL_POLLING_WAIT);
				parent->UpdateIfNeeded();
			}
			while (B_NO_ERROR != acquireResult);
		}
	}
	catch (...)
	{
		delete_sem(semDialog);
		dialogWindow = NULL;
		throw;
	}
	
	delete_sem(semDialog);
	dialogWindow = NULL;
	return dialogResult;
}

/**
 * @brief Constructor
 */
/**
 *	@brief	Constructor
 *	@param[in]	dialogID
 *	@param[in]	type		window type
 *	@param[in]	workspaces	displayed workspaces mask
 */
BeModalDialogWindow::BeModalDialogWindow(SInt32 dialogID, window_type type, uint32 flags, uint32 workspaces /* = B_CURRENT_WORKSPACE */)
	: BeDialog(dialogID, type, workspaces)
{
	semDialog = 0;
}

/**
 *	@brief	Constructor
 *	@param[in]	dialogID
 *	@param[in]	look		window look
 *	@param[in]	feel		window feel
 *	@param[in]	flags		window flags
 *	@param[in]	workspaces	displayed workspaces mask
 */
BeModalDialogWindow::BeModalDialogWindow(SInt32 dialogID, window_look look, window_feel feel, uint32 flags, uint32 workspaces /* = B_CURRENT_WORKSPACE */)
	: BeDialog(dialogID, look, feel, flags, workspaces)
{
	semDialog = 0;
}

/**
 * @brief Destructor
 */
BeModalDialogWindow::~BeModalDialogWindow()
{
}

/**
 * @brief Sets semaphore
 * @param[in] semDialog
 */
void BeModalDialogWindow::SetSemaphore(sem_id semDialog)
{
	this->semDialog = semDialog;
}

/**
 *	@brief Called when B_QUIT_REQUESTED message is received.
 */
bool BeModalDialogWindow::QuitRequested()
{
	if (semDialog >= 0)
	{
		release_sem(semDialog);
	}
	return BeDialog::QuitRequested();
}
