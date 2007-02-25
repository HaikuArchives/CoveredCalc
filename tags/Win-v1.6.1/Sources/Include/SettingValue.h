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
	@file		SettingValue.h
	@brief		Definition of SettingValue class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.08 created
*/

#ifndef _SETTINGVALUE_H_
#define _SETTINGVALUE_H_

#include "UTF8String.h"
#include "MBCString.h"
#include "Path.h"

// ---------------------------------------------------------------------
//! This class represents a setting value which is written in a setting file.
// ---------------------------------------------------------------------
class SettingValue
{
public:
						SettingValue();
						SettingValue(ConstUTF8Str value);
						SettingValue(ConstAStr value);
						SettingValue(SInt32 value);
						SettingValue(UInt32 value);
						SettingValue(bool value);
						SettingValue(const Path& value);

	virtual				~SettingValue();

	void				SetValue(ConstUTF8Str value);
	void				SetValue(ConstAStr value);
	void				SetValue(SInt32 value);
	void				SetValue(UInt32 value);
	void				SetValue(bool value);
	void				SetValue(const Path& value);
	
	void				AsUTF8String(UTF8String& outString) const;
	void				AsMBCString(MBCString& outString) const;
	SInt32				AsSInt32() const;
	UInt32				AsUInt32() const;
	bool				AsBool() const;
	void				AsPath(Path& outPath) const;

private:
	// copy is not implemented
						SettingValue(const SettingValue& other);
	SettingValue&		operator=(const SettingValue& other);

private:
	UTF8String			value;
};

#endif // _SETTINGVALUE_H_
