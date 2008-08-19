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
	@file		BeTextControlAdapter.h
	@brief		Definition of BeTextControlAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.19 created
*/

#ifndef _BETEXTCONTROLADAPTER_H_
#define _BETEXTCONTROLADAPTER_H_

#include "BeControlAdapter.h"
#include "MBCString.h"

class BTextControl;

/**
 *	@brief	Adapter class of TextEdit for BeOS.
 */
class BeTextControlAdapter : public BeControlAdapter
{
public:
						BeTextControlAdapter();
	virtual				~BeTextControlAdapter();
	
	void				Init(BTextControl* view);

	bool				IsEditable();
	void				MakeEditable(bool isEditable);
	
	void				SetText(ConstAStr text);
	void				GetText(MBCString& text);

	void				StopModificationNotification(bool doStop);

protected:
	BTextControl*		getTextControl();

private:
	uint32				stoppedModificationCommand;
	SInt32				stoppedModificationNotificationCount;
};

#endif // _BETEXTCONTROLADAPTER_H_
