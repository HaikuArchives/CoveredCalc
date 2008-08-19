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
	@file		CalcCore.h
	@brief		Definition of CalcCore class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.6.25 created
*/

#ifndef _CALC_CORE_H_
#define _CALC_CORE_H_

class CalcDisplay;

#include "Exception.h"

/**
 * This class calculates inputed key data
 * and displays its result using CalcDisplay object
 * which was set to this object.
 *
 *	@note This class requires "C" locale. 
 */
class CalcCore
{
public:
	enum
	{
		MAX_VALUE_STR = 33		//!< maximum length of ValueStr (includes '\0')
	};

	/// Keys
	enum Key
	{
		Key_0 = 0,
		Key_1,
		Key_2,
		Key_3,
		Key_4,
		Key_5,
		Key_6,
		Key_7,
		Key_8,
		Key_9,
		Key_A,
		Key_B,
		Key_C,
		Key_D,
		Key_E,
		Key_F,
		Key_Point,
		Key_Clear,
		Key_BS,
		Key_Equal,
		Key_Plus,
		Key_Minus,
		Key_Times,
		Key_Div,
		Key_Negate,
		Key_DigitForm2,
		Key_DigitForm8,
		Key_DigitForm10,
		Key_DigitForm16,
	};

	/// operators
	enum Operator
	{
		Op_None = 0,
		Op_Equal,
		Op_Plus,
		Op_Minus,
		Op_Times,
		Op_Div,
	};

	/// forms of digit
	enum DigitForm
	{
		DigitForm_10 = 0,		//!< decimal
		DigitForm_16,			//!< hexadecimal
		DigitForm_8,			//!< octadecimal
		DigitForm_2				//!< binary
	};

	/// errors (which can be displayed)
	enum Error
	{
		Error_None = 0,			//!< no error.
		Error_DivideByZero,		//!< dividend was divided by zero.
		Error_Overflow,			//!< overflow error.
	};

protected:
	/// data in the storage
	union StorageNumber
	{
		Float64 decimal;
		UInt32 integer;
	};
	
public:
					CalcCore();
	virtual			~CalcCore();

	bool			Init(CalcDisplay* display, UInt32 maxDispLength);

	void			Put(Key theKey);

	UInt32			GetMaxDispLength() const
						{ return maxDispLength; }

private:
	void			clearAll();
	UInt32			getMaxInputLen() const;
	void			setLastOperator(Operator op);
	void			setDigitForm(DigitForm form);
	void			setError(Error error);
	StorageNumber	stringToValue(ConstAStr str, DigitForm form) const;
	UInt32			stringToValueUInt(ConstAStr str, SInt32 base) const;
	void			valueToString(AStr str, StorageNumber val, DigitForm form) const;
	void			valueToString10(AStr str, StorageNumber val) const;
	void			valueToStringUInt(AStr str, UInt32 val, SInt32 base) const;
	void			displayInputting();
	void			displayStorage();
	void			calculateCurrentInput();
	void			handleOperator(Operator op);
	void			inputFromNumber(StorageNumber number);
	void			handleNegate();
	void			checkOverFlow(StorageNumber value) const;
	void			handleDigitForm(DigitForm form);

private:
	CalcDisplay*	display;			//!< display to show results
	UInt32			maxDispLength;		//!< maximum length for display
	AChar			inputedString[MAX_VALUE_STR];	//!< string inputed by user
	UInt32			inputedLength;		//!< length of inputed string
	bool			isPointMode;		//!< whether inputting fractional portion
	bool			isErrorMode;		//!< whether error has occured
	DigitForm		currentDigitForm;	//!< current digit form

	StorageNumber	currentStorage;		//!< result of calculation
	StorageNumber	currentDisplayNum;	//!< the number which is shown in current display.
	Operator		lastOperator;		//!< last inputed operator
};

#endif // _CALC_CORE_H_
