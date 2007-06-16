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
	@file		CalcDisplay.h
	@brief		Definition of CalcDisplay interface.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.6.25 created
*/

#ifndef _CALC_DISPLAY_H_
#define _CALC_DISPLAY_H_

#include "CalcCore.h"

// ---------------------------------------------------------------------
//! The interface for calc's display part.
// ---------------------------------------------------------------------
class CalcDisplay
{
public:
					CalcDisplay() { }
	virtual			~CalcDisplay() { }

	/**
	 *	@brief	Displays the calculated value.
	 */
	virtual	void	DisplayValue(
		ConstAStr value			///< data to display
	) = 0;
	
	/**
	 *	@brief	Displays the inputed operator.
	 */
	virtual	void	DisplayOperator(
		CalcCore::Operator op		///< inputed operator
	) = 0;
	
	/**
	 *	@brief	Displays digit form (radix)
	 */
	virtual void	DisplayDigitForm(
		CalcCore::DigitForm form	///< digit form
	) = 0;
	
	/**
	 *	@brief	Displays the error.
	 */
	virtual	void	DisplayError(
		CalcCore::Error error		///< error to display
	) = 0;
};

#endif // _CALC_DISPLAY_H_
