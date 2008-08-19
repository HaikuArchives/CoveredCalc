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
	@file		BeControlAdapter.h
	@brief		Definition of BeControlAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.05.19 created
*/

#ifndef _BECONTROLADAPTER_H_
#define _BECONTROLADAPTER_H_

#include "BeViewAdapter.h"

class BControl;

/**
 *	@brief	Implementation of adapter class for BControl.
 */
class BeControlAdapter : public BeViewAdapter
{
public:
						BeControlAdapter();
	virtual				~BeControlAdapter();
	
	void				Init(BControl* view);
	
	bool				IsEnabled();
	void				Enable(bool doEnable);

protected:
	BControl*			getControl();
};

#endif // _BECONTROLADAPTER_H_
