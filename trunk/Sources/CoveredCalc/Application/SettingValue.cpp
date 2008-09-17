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
	@file		SettingValue.cpp
	@brief		Implementation of SettingValue class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.08 created
*/

#include "Prefix.h"
#include <stdio.h>
#include "UTF8Conv.h"
#include "SettingValue.h"

static UTF8Char	BoolValue_True[]	= "true";
static UTF8Char	BoolValue_False[]	= "false";

// ---------------------------------------------------------------------
//! Constructor with no parameter
// ---------------------------------------------------------------------
SettingValue::SettingValue()
{
	this->value.Empty();
}

// ---------------------------------------------------------------------
//! Constructor with UTF-8 string
// ---------------------------------------------------------------------
SettingValue::SettingValue(ConstUTF8Str value)
{
	SetValue(value);
}

// ---------------------------------------------------------------------
//! Constructor with native multi-byte string
// ---------------------------------------------------------------------
SettingValue::SettingValue(ConstAStr value)
{
	SetValue(value);
}

// ---------------------------------------------------------------------
//! Constructor with signed 32-bit integer
// ---------------------------------------------------------------------
SettingValue::SettingValue(SInt32 value)
{
	SetValue(value);
}

// ---------------------------------------------------------------------
//! Constructor with unsigned 32-bit integer
// ---------------------------------------------------------------------
SettingValue::SettingValue(UInt32 value)
{
	SetValue(value);
}

// ---------------------------------------------------------------------
//! Constructor with bool
// ---------------------------------------------------------------------
SettingValue::SettingValue(bool value)
{
	SetValue(value);
}

// ---------------------------------------------------------------------
//! Constructor with path
// ---------------------------------------------------------------------
SettingValue::SettingValue(const Path& value)
{
	SetValue(value);
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
SettingValue::~SettingValue()
{
}

// ---------------------------------------------------------------------
//! Sets value from UTF-8 string.
// ---------------------------------------------------------------------
void SettingValue::SetValue(ConstUTF8Str value)
{
	this->value = value;
}

// ---------------------------------------------------------------------
//! Sets value from native multi-byte string
// ---------------------------------------------------------------------
void SettingValue::SetValue(ConstAStr value)
{
	UTF8Conv::FromMultiByte(this->value, value);
}

// ---------------------------------------------------------------------
//! Sets value from signed 32-bit integer
// ---------------------------------------------------------------------
void SettingValue::SetValue(SInt32 value)
{
	UTF8Char buf[16];
	snprintf(TypeConv::AsASCII(buf), sizeof(buf)/sizeof(UTF8Char), "%ld", value);
	buf[sizeof(buf)/sizeof(UTF8Char) - 1] = '\0';
	this->value = buf;
}

// ---------------------------------------------------------------------
//! Sets value from unsigned 32-bit integer
// ---------------------------------------------------------------------
void SettingValue::SetValue(UInt32 value)
{
	UTF8Char buf[16];
	snprintf(TypeConv::AsASCII(buf), sizeof(buf)/sizeof(UTF8Char), "%lu", value);
	buf[sizeof(buf)/sizeof(SChar) - 1] = '\0';
	this->value = buf;
}

// ---------------------------------------------------------------------
//! Sets value from bool
// ---------------------------------------------------------------------
void SettingValue::SetValue(bool value)
{
	if (value)
	{
		this->value = BoolValue_True;
	}
	else
	{
		this->value = BoolValue_False;
	}
}

// ---------------------------------------------------------------------
//! Sets value from path
// ---------------------------------------------------------------------
void SettingValue::SetValue(const Path& value)
{
	MBCString mbcString;
	value.GetSlashSeparatedPathString(mbcString);
	UTF8Conv::FromMultiByte(this->value, mbcString);
}

// ---------------------------------------------------------------------
//! Retrieves value as UTF-8 string
// ---------------------------------------------------------------------
void SettingValue::AsUTF8String(
	UTF8String& outString			//!< OUTPUT
) const
{
	outString = value;
}

// ---------------------------------------------------------------------
//! Retrieves value as native multi-byte string
// ---------------------------------------------------------------------
void SettingValue::AsMBCString(
	MBCString& outString			//!< OUTPUT
) const
{
	UTF8Conv::ToMultiByte(outString, value);
}

// ---------------------------------------------------------------------
//! Retrieves value as signed 32-bit integer
/*!
	@return signed 32-bit integer
*/
// ---------------------------------------------------------------------
SInt32 SettingValue::AsSInt32() const
{
	SInt32 retValue;
	
	if (1 != sscanf(TypeConv::AsASCII(value), "%ld", &retValue))
	{
		retValue = 0;
	}

	return retValue;
}

// ---------------------------------------------------------------------
//! Retrieves value as unsigned 32-bit integer
/*!
	@return unsigned 32-bit integer
*/
// ---------------------------------------------------------------------
UInt32 SettingValue::AsUInt32() const
{
	UInt32 retValue;
	
	if (1 != sscanf(TypeConv::AsASCII(value), "%lu", &retValue))
	{
		retValue = 0;
	}

	return retValue;
}

// ---------------------------------------------------------------------
//! Retrieves value as bool
/*!
	@return bool
*/
// ---------------------------------------------------------------------
bool SettingValue::AsBool() const
{
	if (0 == value.Compare(BoolValue_True))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ---------------------------------------------------------------------
//! Retrieves value as path
// ---------------------------------------------------------------------
void SettingValue::AsPath(
	Path& outPath			//!< OUTPUT
) const
{
	MBCString outMBCString;
	UTF8Conv::ToMultiByte(outMBCString, value);
	outPath.AssignFromSlashSeparated(outMBCString);
}
