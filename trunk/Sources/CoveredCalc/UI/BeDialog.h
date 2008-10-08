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
	@file		BeDialog.h
	@brief		Definition of BeDialog class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.12.07 created
*/

#ifndef _BEDIALOG_H_
#define _BEDIALOG_H_

#include <Window.h>

// ---------------------------------------------------------------------
//! Dialog class on BeOS
// ---------------------------------------------------------------------
class BeDialog : public BWindow
{
public:
						BeDialog(BRect frame, const char* title, window_type type, uint32 flags, uint32 workspaces = B_CURRENT_WORKSPACE);
						BeDialog(BRect frame, const char* title, window_look look, window_feel feel, uint32 flags, uint32 workspaces = B_CURRENT_WORKSPACE);
	virtual				~BeDialog();
	
	void				Init();

protected:
	virtual void		initView(BView* view);
	void				moveToCenterOfScreen();
#if defined (ZETA)
	virtual void		languageChanged();
#endif

	virtual void		MessageReceived(BMessage *message);

private:
	void				onCommandActivate(BMessage* message);
};

#endif // _BEDIALOG_H_
