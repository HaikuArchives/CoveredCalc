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
	@file       BeDialogManager.h
	@brief      Definition of BeDialogManager class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2008.12.26 created
*/

#ifndef _BEDIALOGMANAGER_H_
#define _BEDIALOGMANAGER_H_

class BMessenger;
class BeDialog;

/**
 *	@brief	Manages a shown dialog and helps showing it.
 */
class BeDialogManager
{
public:
						BeDialogManager();
	virtual				~BeDialogManager();
	
	void				ShowDialog();
	
protected:
	virtual BeDialog*	createNewDialog() = 0;

private:
	BMessenger*			dialogMessenger;
};

/**
 *	@brief	Generic dialog manager.
 */
class BeGenericDialogManager : public BeDialogManager
{
public:
						BeGenericDialogManager() {}
	virtual				~BeGenericDialogManager() {}

protected:
	virtual BeDialog*	createNewDialog();
	virtual BeDialog*	newDialogObject() = 0;
};

#endif // _BEDIALOGMANAGER_H_
