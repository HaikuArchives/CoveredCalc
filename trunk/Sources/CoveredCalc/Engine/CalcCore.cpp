/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2010 CoveredCalc Project Contributors
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
	@file		CalcCore.cpp
	@brief		Implemantation of CalcCore class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.6.27 created
*/

#include "Prefix.h"
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "CalcDisplay.h"
#include "CalcCore.h"

/**
 * The exception which has CalcCore error.
 *
 * @note This class is internally used by CalcCore class.
 */
class CalcCoreException : public Exception
{
public:
					CalcCoreException(CalcCore::Error error) : Exception() { this->error = error; }
	CalcCore::Error	GetCalcCoreError() const { return this->error; }
	
private:
	CalcCore::Error error;
};

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CalcCore::CalcCore()
{
	maxDispLength = 32;
	display = NULL;
	currentDigitForm = DigitForm_10;
	clearAll();
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CalcCore::~CalcCore()
{
}

// ---------------------------------------------------------------------
//! Initializes the object.
// ---------------------------------------------------------------------
bool CalcCore::Init(
	CalcDisplay* display,		//!< display object
	UInt32 maxDispLength		//!< maximum length for display
)
{
	this->display = display;
	if (maxDispLength < 2)
	{
		maxDispLength = 2;
	}
	else if (maxDispLength > 32)
	{
		maxDispLength = 32;
	}
	this->maxDispLength = maxDispLength;
	
	setDigitForm(DigitForm_10);
	clearAll();

	displayInputting();
	return true;
}

// ---------------------------------------------------------------------
//! Clears all data.
// ---------------------------------------------------------------------
void CalcCore::clearAll()
{
	inputedString[0] = '\0';
	inputedLength = 0;
	isPointMode = false;

	if (DigitForm_10 == currentDigitForm)
	{
		currentStorage.decimal = 0.0;
		currentDisplayNum.decimal = 0.0;
	}
	else
	{
		currentStorage.integer = 0;
		currentDisplayNum.integer = 0;
	}
	setLastOperator(Op_None);
	setError(Error_None);
}

// ---------------------------------------------------------------------
//! Gets the maximum length of input data in current state.
/*!
	@return maximum length of input data
*/
// ---------------------------------------------------------------------
UInt32 CalcCore::getMaxInputLen() const
{
	UInt32 result = 0;

	switch(currentDigitForm)
	{
	case DigitForm_10:
		// The value of decimal form is represented by Float64.
		// It has 15-16 significant digits.
		// 1 of 15 digits is for rounding, so the calc can accept 14 digits.
		if (isPointMode)
		{
			result = 15;	// 14 + length of '.'
		}
		else
		{
			result = 14;
			return min(result, maxDispLength - 1);	// Since '.' is added automatically
		}
		break;

	case DigitForm_16:
		result = 8;
		break;

	case DigitForm_8:
		result = 11;
		break;

	case DigitForm_2:
		result = 32;
		break;
	
	default:
		ASSERT(false);
	}

	return min(result, maxDispLength);
}

// ---------------------------------------------------------------------
//! Converts from specified string to numeric value.
/*!
	@return numeric value
*/
// ---------------------------------------------------------------------
CalcCore::StorageNumber CalcCore::stringToValue(
	ConstAStr str,		//!< string to convert
	DigitForm form		//!< digit form which is used for conversion
) const
{
	StorageNumber val;
	
	switch(form)
	{
	case DigitForm_10:
		if (1 != stscanf(str, ALITERAL("%lf"), &(val.decimal)))
		{
			val.decimal = 0.0;
		}
		break;
	
	case DigitForm_16:
		val.integer = stringToValueUInt(str, 16);
		break;

	case DigitForm_8:
		val.integer = stringToValueUInt(str, 8);
		break;
		
	case DigitForm_2:		
		val.integer = stringToValueUInt(str, 2);
		break;

	default:
		ASSERT(false);
		memset(&val, 0, sizeof(val));
		break;
		}

	return val;
}

/**
 *	@brief	Converts from specified string to unsigned long interger
 *	@return	converted integer.
 */
UInt32 CalcCore::stringToValueUInt(
	ConstAStr str,		///< string to convert.
	SInt32 base			///< base number.
) const
{
	AStr end = NULL;
#if defined (WIN32)
	UInt32 ret = _tcstoul(str, &end, base);
#else
	UInt32 ret = strtoul(str, &end, base);
#endif
	if (end == str)
	{
		ret = 0;
	}
	return ret;
}


// ---------------------------------------------------------------------
//! Converts from specified numeric value to string.
/*!
	@note The length of the output buffer must be >= 33 bytes.
*/
// ---------------------------------------------------------------------
void CalcCore::valueToString(
	AStr str,			//!< the result of conversion is returned (output)
	StorageNumber val,	//!< value to convert
	DigitForm form		//!< digit form which is used for conversion
) const
{
	switch(form)
	{
	case DigitForm_10:
		valueToString10(str, val);
		break;
	case DigitForm_16:
		valueToStringUInt(str, val.integer, 16);
		break;
	case DigitForm_8:
		valueToStringUInt(str, val.integer, 8);
		break;
	case DigitForm_2:
		valueToStringUInt(str, val.integer, 2);
		break;
	default:
		ASSERT(false);
	}
}

// ---------------------------------------------------------------------
//! Converts from specified numeric value to decimal string.
/*!
	@note The length of the output buffer must be >= 33 bytes.
*/
// ---------------------------------------------------------------------
void CalcCore::valueToString10(
	AStr str,				//!< the result of conversion is returned (output)
	StorageNumber val		//!< value to convert
) const
{
	AChar	format[9];
	AChar	buf[30];
	SInt32	intNum;			// length of the integer part
	SInt32	frcNum;			// length of the fractional part
	SInt32	dispNum;
	
	// The value of decimal form is represented by Float64.
	// It has 15-16 significant digits.
	// 1 of 15 digits is for rounding, so the calc can accept 14 digits.
	// 14 minus the minimum length of the integer part (=1) equals maximum length of the fractional part (=13) 
	sntprintf( buf, sizeof(buf)/sizeof(AChar), ALITERAL("%.13f"), val.decimal );
	buf[sizeof(buf)/sizeof(AChar)-1] = ALITERAL('\0');
	// trim right 0s
	for(frcNum=static_cast<SInt32>(tcslen(buf))-1; ALITERAL('0')==buf[frcNum]; frcNum--);
	buf[frcNum+1] = ALITERAL('\0');

	for(intNum=0; '.'!=buf[intNum]; intNum++);		// count the length of the integer part
	frcNum = static_cast<SInt32>(tcslen( &buf[intNum+1] ));				// count the length of the fractional part
#if 0	// Bug-12 minus sign may not be displayed.
		// why did I check this before?
	if ('-' == buf[0])
	{
		intNum--;
	}
#endif

	// decide the length of fractional part for displaying
	dispNum = intNum + 1 + frcNum;
	if (min(15, maxDispLength) < static_cast<UInt32>(dispNum))
	{
		frcNum -= dispNum - min(15, maxDispLength);
	}

	// sprintf again for auto rounding
	if (0 >= frcNum)
	{
		sntprintf(format, sizeof(format)/sizeof(AChar), ALITERAL("%%%u.0f."), static_cast<unsigned>(maxDispLength) - 1);
	}
	else
	{
		sntprintf(format, sizeof(format)/sizeof(AChar), ALITERAL("%%%u.%df"), static_cast<unsigned>(maxDispLength), static_cast<int>(frcNum) );
	}
	format[sizeof(format)/sizeof(AChar)-1] = ALITERAL('\0');
	sntprintf(str, 33, format, val);
	str[32] = ALITERAL('\0');
	if (0 < frcNum)
	{
		// trim right 0s
		SInt32 count = 0;
		count = 0;
		SInt32 length = static_cast<SInt32>(tcslen(str));
		SInt32 index;
		for (index=length-1; ALITERAL('0')==str[index]; index--)
			count++;
		
		if (count > 0)
		{
			memmove(str + count, str, (length - count) * sizeof(AChar));
			for (index=0; index<count; index++)
				str[index] = ALITERAL(' ');
		}
	}
}

/**
 *	@brief	 Converts from specified integer value to string.
 *	@note The length of the output buffer must be >= 33 bytes.
 */
void CalcCore::valueToStringUInt(
	AStr str,			///< the result of conversion is returned (output)
	UInt32 val,			///< integer value to convert
	SInt32 base			///< base number
) const
{
	UInt32 ix;
	for (ix = 0; ix < maxDispLength; ++ix)
	{
		str[ix] = ALITERAL(' ');
	}
	AChar* curPos = str + maxDispLength;
	*curPos = ALITERAL('\0');
	curPos--;
	
	if (val == 0)
	{
		*curPos = ALITERAL('0');
		return;
	}
	
	while (val != 0)
	{
		if (curPos < str)
		{
			ASSERT(false);
			return;
		}
		UInt32 digit = val % base;
		val /= base;
		if (digit > 9)
		{
			*curPos = static_cast<AChar>(ALITERAL('A') + digit - 10);
		}
		else
		{
			*curPos = static_cast<AChar>(ALITERAL('0') + digit);
		}
		curPos--;
	}
}

// ---------------------------------------------------------------------
//! Sets the last operator.
// ---------------------------------------------------------------------
void CalcCore::setLastOperator(
	Operator op			//!< the last operator
)
{
	lastOperator = op;
	if (NULL != display)
	{
		Operator dispOperator = op;
		if (Op_Equal == dispOperator)
		{
			dispOperator = Op_None;
		}
		display->DisplayOperator(dispOperator);
	}
}

/**
 *	@brief	Sets current digit form.
 */
void CalcCore::setDigitForm(
	DigitForm form		///< digit form
)
{
	currentDigitForm = form;
	if (NULL != display)
	{
		display->DisplayDigitForm(form);
	}
}

// ---------------------------------------------------------------------
//! Sets the error
/*!
	@note If inputed error is Error_None, it clears last error.
*/
// ---------------------------------------------------------------------
void CalcCore::setError(
	Error error		//!< the error.
)
{
	if (Error_None == error)
	{
		isErrorMode = false;
	}
	else
	{
		isErrorMode = true;
	}

	if (NULL != display)
	{
		display->DisplayError(error);
	}	
}

/**
 *	@brief	Throws Overflow exception if value is overflow.
 */
void CalcCore::checkOverFlow(StorageNumber value) const
{
	if (DigitForm_10 == currentDigitForm)
	{
		double overflowThP = pow(10.0, static_cast<SInt32>(min(14, maxDispLength - 1)));
		double overflowThN = pow(10.0, static_cast<SInt32>(min(14, maxDispLength - 2)));
		
		if (value.decimal >= overflowThP
				|| value.decimal <= -overflowThN)
		{
			throw new CalcCoreException(CalcCore::Error_Overflow);
		}		
	}
	else
	{
		UInt32 uiValue = value.integer;
		UInt32 digits = 0;
		while (uiValue != 0)
		{
			switch (currentDigitForm)
			{
			case DigitForm_16:
				uiValue /= 16;
				break;
			case DigitForm_8:
				uiValue /= 8;
				break;
			case DigitForm_2:
				uiValue /= 2;
				break;
			default:
				ASSERT(false);
				break;
			}
			digits++;
			if (digits > maxDispLength)
			{
				throw new CalcCoreException(CalcCore::Error_Overflow);
			}
		}
	}
}

// ---------------------------------------------------------------------
//! Calculates using current inputed data.
// ---------------------------------------------------------------------
void CalcCore::calculateCurrentInput()
{
	if (Op_Equal != lastOperator)
	{
		StorageNumber inputedValue = currentDisplayNum;
		if (DigitForm_10 == currentDigitForm)
		{
			switch (lastOperator)
			{
			case Op_None:
				currentStorage.decimal = inputedValue.decimal;
				break;
			case Op_Plus:
				currentStorage.decimal += inputedValue.decimal;
				break;
			case Op_Minus:
				currentStorage.decimal -= inputedValue.decimal;
				break;
			case Op_Times:
				currentStorage.decimal *= inputedValue.decimal;
				break;
			case Op_Div:
				if (0.0 == inputedValue.decimal)
				{
					throw new CalcCoreException(CalcCore::Error_DivideByZero);
				}
				currentStorage.decimal /= inputedValue.decimal;
				break;
			default:
				ASSERT(false);
				break;
			}
		}
		else
		{
			switch (lastOperator)
			{
			case Op_None:
				currentStorage.integer = inputedValue.integer;
				break;
			case Op_Plus:
				currentStorage.integer += inputedValue.integer;
				break;
			case Op_Minus:
				currentStorage.integer -= inputedValue.integer;
				break;
			case Op_Times:
				currentStorage.integer *= inputedValue.integer;
				break;
			case Op_Div:
				if (0 == inputedValue.integer)
				{
					throw new CalcCoreException(CalcCore::Error_DivideByZero);
				}
				currentStorage.integer /= inputedValue.integer;
				break;
			default:
				ASSERT(false);
				break;
			}
		}
	}
	
	checkOverFlow(currentStorage);
	inputedString[0] = ALITERAL('\0');
	inputedLength = 0;
	isPointMode = false;
}

/**
 * Trims left space of the string
 */
static void TrimLeftSpace(AStr string)
{
	AChar* curPos = string;
	while (*curPos == ' ')
	{
		curPos++;
	}
	if (curPos != string)
	{
		memmove(string, curPos, (tcslen(curPos) + 1) * sizeof(AChar));
	}	
}

// ---------------------------------------------------------------------
//! Inputs a calc's key.
// ---------------------------------------------------------------------
void CalcCore::Put(
	Key theKey		//!< the inputed key.
)
{
	// if error was occured, this object only accepts 'Clear' key.
	if (isErrorMode && Key_Clear != theKey)
	{
		return;
	}
	
	try
	{
		switch(theKey)
		{
		case Key_0:
		case Key_1:
		case Key_2:
		case Key_3:
		case Key_4:
		case Key_5:
		case Key_6:
		case Key_7:
		case Key_8:
		case Key_9:
		case Key_A:
		case Key_B:
		case Key_C:
		case Key_D:
		case Key_E:
		case Key_F:
			{
				UInt32 value = theKey - Key_0;
				if ((currentDigitForm == DigitForm_2 && value > 1) ||
					(currentDigitForm == DigitForm_8 && value > 7) ||
					(currentDigitForm == DigitForm_10 && value > 9))
				{
					// invalid key in this form.
					break;
				}
				
				if ( getMaxInputLen() <= inputedLength )
				{
					break;
				}
				if (Op_Equal == lastOperator)
				{
					lastOperator = Op_None;
				}
				if (1 == inputedLength && '0' == inputedString[0])
				{
					inputedLength--;
				}
				inputedString[inputedLength++] = static_cast<AChar>(value + ((value > 9) ? ALITERAL('A') - 10 : ALITERAL('0')));
				inputedString[inputedLength] = ALITERAL('\0');
				displayInputting();
			}
			break;
		case Key_Point:
			if (currentDigitForm != DigitForm_10 || isPointMode || getMaxInputLen() <= inputedLength)
			{
				break;
			}
			if (Op_Equal == lastOperator)
			{
				lastOperator = Op_None;
			}
			if (0 == inputedLength)
			{
				inputedString[inputedLength++] = ALITERAL('0');
			}
			inputedString[inputedLength++] = ALITERAL('.');
			inputedString[inputedLength] = ALITERAL('\0');
			isPointMode = true;
			displayInputting();
			break;		
		case Key_Clear:
			clearAll();
			displayInputting();
			break;
		case Key_BS:
			if (Op_Equal == lastOperator)
			{
				lastOperator = Op_None;
			}
			if (0 == inputedLength)
			{
				// handle special case : the current displayed value is regarded as inputed data.
				inputFromNumber(currentDisplayNum);
			}
			if ('.' == inputedString[inputedLength - 1])
			{
				isPointMode = false;
			}		
			inputedString[--inputedLength] = ALITERAL('\0');
			displayInputting();
			break;
		case Key_Equal:
			calculateCurrentInput();
			setLastOperator(Op_Equal);
			displayStorage();
			break;
		case Key_Plus:
			handleOperator(Op_Plus);
			break;
		case Key_Minus:
			handleOperator(Op_Minus);
			break;
		case Key_Times:
			handleOperator(Op_Times);
			break;
		case Key_Div:
			handleOperator(Op_Div);
			break;
		case Key_Negate:
			handleNegate();
			break;
		case Key_DigitForm2:
			handleDigitForm(DigitForm_2);
			break;
		case Key_DigitForm8:
			handleDigitForm(DigitForm_8);
			break;
		case Key_DigitForm10:
			handleDigitForm(DigitForm_10);
			break;
		case Key_DigitForm16:
			handleDigitForm(DigitForm_16);
			break;
		default:
			ASSERT(false);	// not implemented yet
			break;
		}
	}
	catch (CalcCoreException* ex)
	{
		setError(ex->GetCalcCoreError());
		ex->Delete();
	}
}

/**
 * Handles a negate key
 */
void CalcCore::handleNegate()
{
	if (Op_Equal == lastOperator)
	{
		lastOperator = Op_None;
	}
	if (0 == inputedLength)
	{
		// handle special case : the current displayed value is regarded as inputed data.
		inputFromNumber(currentDisplayNum);
	}
	
	StorageNumber tempNumber = stringToValue(inputedString, currentDigitForm);
	if (DigitForm_10 == currentDigitForm)
	{
		tempNumber.decimal = -tempNumber.decimal;
	}
	else
	{
		tempNumber.integer = -static_cast<SInt32>(tempNumber.integer);
	}
	inputFromNumber(tempNumber);
	
	if (DigitForm_10 == currentDigitForm)
	{
		// if result length is too long for display... (because of the minus sign character is added)
		if (inputedLength > getMaxInputLen())
		{
			if (isPointMode)
			{
				// truncate the last fractional part
				// Note: if value is rounded off, the length of integer part can be increase. it's no point.
				//       (for example, 99.9999 can become 100.999.)
				//       so here, just truncate the last fractional part.
				inputedString[inputedLength - 1] = ALITERAL('\0');
				inputedLength--;
				while ('0' == inputedString[inputedLength - 1])		// example: if 2.506 becomes 2.50, then modify it to 2.5.
				{
					inputedString[inputedLength - 1] = ALITERAL('\0');
					inputedLength--;
				}
				if ('.' == inputedString[inputedLength - 1])
				{
					inputedString[inputedLength - 1] = ALITERAL('\0');
					inputedLength--;
					isPointMode = false;
				}
			}
			else
			{
				// error (overflow)
				throw new CalcCoreException(CalcCore::Error_Overflow);
			}
		}

		// handle special case: -0 is modified to 0.
		if (inputedLength == 2 && inputedString[0] == ALITERAL('-') && inputedString[1] == ALITERAL('0'))	// handle special case: modify -0 to 0
		{
			inputedString[0] = ALITERAL('0');
			inputedString[1] = ALITERAL('\0');
			inputedLength = 1;
		}
	}
	else
	{
		checkOverFlow(tempNumber);
	}
	
	displayInputting();
}

/**
 *	@brief	Handles a operator.
 */
void CalcCore::handleOperator(
	Operator op		///< inputted operator
)
{
	if (inputedLength > 0)
	{
		calculateCurrentInput();
	}
	setLastOperator(op);
	displayStorage();
}

/**
 *	@brief	Handles changing digit form.
 */
void CalcCore::handleDigitForm(
	CalcCore::DigitForm form	///< digit form
)
{
	if (inputedLength > 0)
	{
		calculateCurrentInput();
	}
	setLastOperator(Op_None);
	
	if (currentDigitForm == DigitForm_10 && form != DigitForm_10)
	{
		SInt32 integer = static_cast<SInt32>(currentStorage.decimal);
		currentStorage.integer = integer;
	}
	else if (currentDigitForm != DigitForm_10 && form == DigitForm_10)
	{
		Float64 decimal = static_cast<SInt32>(currentStorage.integer);
		currentStorage.decimal = decimal;
	}
	
	setDigitForm(form);

	checkOverFlow(currentStorage);
	displayStorage();
}

// ---------------------------------------------------------------------
//! Displays inputting value
// ---------------------------------------------------------------------
void CalcCore::displayInputting()
{
	if (NULL == display)
	{
		return;
	}

	AChar valueStr[CalcCore::MAX_VALUE_STR];
	SInt32 index, index2;

	// padding left spaces
	SInt32 ctMax = maxDispLength - inputedLength;
	if (0 == inputedLength)
	{
		ctMax--;		// displaying 0 if nothing is inputed.
	}
	if (DigitForm_10 == currentDigitForm)
	{
		if (!isPointMode)
		{
			ctMax--;		// to end with period (.) if inputed value is integer
		}
	}
	for( index=0; index<ctMax; index++ )
	{
		valueStr[index] = ALITERAL(' ');
	}

	// set the value to display
	ctMax = inputedLength;
	for( index2=0; index2<ctMax; index2++ )
	{
		valueStr[index++] = inputedString[index2];
	}
	if (0 == inputedLength)
	{
		valueStr[index++] = ALITERAL('0');
	}
	// add the period if decimal form mode and value is integer
	if ( DigitForm_10 == currentDigitForm)
	{
		if (!isPointMode)
		{
			valueStr[index++] = ALITERAL('.');
		}
	}
	valueStr[index] = ALITERAL('\0');
	
	display->DisplayValue(valueStr);
	currentDisplayNum = stringToValue(inputedString, currentDigitForm);
}

// ---------------------------------------------------------------------
//! Displays the value in storage
// ---------------------------------------------------------------------
void CalcCore::displayStorage()
{
	if (NULL == display)
	{
		return;
	}
	
	AChar valueStr[CalcCore::MAX_VALUE_STR];
	this->valueToString(valueStr, currentStorage, currentDigitForm);
	display->DisplayValue(valueStr);
	
	// once the storage data is displayed, it should be updated to really displayed value.
	currentStorage = this->stringToValue(valueStr, currentDigitForm);
	currentDisplayNum = currentStorage;
}

// ---------------------------------------------------------------------
//! The current displayed value is regarded as specified number.
// ---------------------------------------------------------------------
void CalcCore::inputFromNumber(
	StorageNumber number		///< this value is used to inputed data.
)
{
	valueToString(inputedString, number, currentDigitForm);
	TrimLeftSpace(inputedString);
	inputedLength = tcslen(inputedString);
	if (ALITERAL('.') == inputedString[inputedLength - 1])
	{
		inputedString[inputedLength - 1] = ALITERAL('\0');
		inputedLength--;
		isPointMode = false;					
	}
	else
	{
		isPointMode = true;
	}
}
