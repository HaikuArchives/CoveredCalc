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
	@file		WaitingUIKeeper.h
	@brief		Definition of WaitingUIKeeper interface
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.03.14 created
*/

#ifndef _WAITINGUIKEEPER_H_
#define _WAITINGUIKEEPER_H_

#include "WaitingUI.h"

// ---------------------------------------------------------------------
//! Utility class which callsWaitingUI::EndWaitingUI when the instance of this class is deleted.
// ---------------------------------------------------------------------
class WaitingUIKeeper
{
public:
						WaitingUIKeeper(WaitingUI* waitingUI)
							{ this->waitingUI = waitingUI; }
						~WaitingUIKeeper()
							{ End(); }

	void				Start()
						{
							if (NULL != waitingUI)
							{
								waitingUI->BeginWaitingUI();
							}
						}
	
	void				End()
						{
							if (NULL != waitingUI)
							{
								waitingUI->EndWaitingUI();
								waitingUI = NULL;
							}
						}

private:
	WaitingUI*			waitingUI;
};



#endif // _WAITINGUIKEEPER_H_
