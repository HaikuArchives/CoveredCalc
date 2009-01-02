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
	@file       BeDialogManager.cpp
	@brief      Implementation of BeDialogManager class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2008.12.26 created
*/

#include "Prefix.h"
#include "BeDialogManager.h"
#include <Messenger.h>
#include "BeDialog.h"
#include "CommandID.h"

/**
 *	@brief	Constructor
 */
BeDialogManager::BeDialogManager()
{
	dialogMessenger = NULL;
}

/**
 *	@brief	Destructor
 */
BeDialogManager::~BeDialogManager()
{
	if (NULL != dialogMessenger)
	{
		delete dialogMessenger;
	}
}

/**
 *	@brief	Shows dialog.
 *	If dialog is already shown, activate it instead of showing new dialog.
 */
void BeDialogManager::ShowDialog()
{
	if (NULL != dialogMessenger && dialogMessenger->IsValid())
	{
		// already shown. activate it.
		dialogMessenger->SendMessage(ID_DIALOG_ACTIVATE);
		return;
	}
	
	if (NULL != dialogMessenger)
	{
		delete dialogMessenger;
		dialogMessenger = NULL;
	}
	
	BeDialog* dialog = createNewDialog();
	dialog->Show();
	dialogMessenger = new BMessenger(dialog);		
}

/**
 *	@brief	Creates a new dialog.
 *	@return	generated dialog.
 */
BeDialog* BeGenericDialogManager::createNewDialog()
{
	BeDialog* aDialog = newDialogObject();
	try
	{
		aDialog->Init();
	}
	catch (...)
	{
		delete aDialog;
		throw;
	}
	return aDialog;
}
