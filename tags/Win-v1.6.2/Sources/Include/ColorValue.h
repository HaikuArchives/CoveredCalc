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
	@file		ColorValue.h
	@brief		Definition of ColorValue structure.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.7 created
*/

#ifndef _COLORVALUE_H_
#define _COLORVALUE_H_

// ---------------------------------------------------------------------
//! The value of a color.
// ---------------------------------------------------------------------
class ColorValue
{
public:
	ColorValue()
	{
		redValue = greenValue = blueValue = 0;
	}

	ColorValue(UInt8 red, UInt8 green, UInt8 blue)
	{
		redValue = red;
		greenValue = green;
		blueValue = blue;
	}

	ColorValue& operator=(const ColorValue& other)
	{
		if (this != &other)
		{
			redValue = other.redValue;
			greenValue = other.greenValue;
			blueValue = other.blueValue;
		}
		return *this;
	}

	bool operator==(const ColorValue& other) const
	{
		return (this->redValue == other.redValue
				&& this->greenValue == other.greenValue
				&& this->blueValue == other.blueValue)		? true : false;
	}

	bool operator!=(const ColorValue& other) const
	{
		return (*this == other) ? false : true;
	}

public:
	UInt8	redValue;
	UInt8	greenValue;
	UInt8	blueValue;
};

// ---------------------------------------------------------------------
// NAME:		ColorValueComparator
// DESCRIPTION:	The comparetor for two ColorValue.
// NOTE:
// ---------------------------------------------------------------------
class ColorValueComparator
{
public:
	bool operator()(const ColorValue& left, const ColorValue& right) const
	{
		return (asUInt32(left) < asUInt32(right)) ? true : false;
	}
	
private:
	UInt32 asUInt32(const ColorValue& value) const
	{
		return (static_cast<UInt32>(value.redValue) | (static_cast<UInt32>(value.blueValue) << 8) | (static_cast<UInt32>(value.greenValue) << 16));
	}
};

#endif // _COLORVALUE_H_
