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
	@file		UTF8String.h
	@brief		Definition of UTF8String class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.1.25 created
*/

#ifndef _UTF8STRING_H_
#define _UTF8STRING_H_

#include <string>
#include "TypeConv.h"

// ---------------------------------------------------------------------
//! String class which deal with UTF8 string
// ---------------------------------------------------------------------
class UTF8String
{
public:
						UTF8String() : innerString() { }
						UTF8String(UTF8Char character) : innerString(1, character) { }
						UTF8String(ConstUTF8Str string) : innerString(TypeConv::AsASCII(string)) { }
						UTF8String(ConstUTF8Str string, SInt32 length) : innerString(TypeConv::AsASCII(string), length) { }
						UTF8String(const UTF8String& other) : innerString(other.innerString) { }

	UTF8String&		operator=(UTF8Char character)
							{ innerString = character; return *this; }
	UTF8String&		operator=(ConstUTF8Str string)
							{ innerString = TypeConv::AsASCII(string); return *this; }
	UTF8String&		operator=(const UTF8String& other)
							{
								if (this != &other)
								{
									innerString = other.innerString;
								}
								return *this;
							}

	UTF8String&		operator+=(UTF8Char character)
							{ innerString += character; return *this; }
	UTF8String&		operator+=(ConstUTF8Str string)
							{ innerString += TypeConv::AsASCII(string); return *this; }
	UTF8String&		operator+=(const UTF8String& other)
							{ innerString += other.innerString; return *this; }

	friend UTF8String	operator+(const UTF8String& left, UTF8Char character);
	friend UTF8String	operator+(const UTF8String& left, ConstUTF8Str string);
	friend UTF8String	operator+(const UTF8String& left, const UTF8String& right);
	friend UTF8String	operator+(UTF8Char character, const UTF8String& right);
	friend UTF8String	operator+(ConstUTF8Str string, const UTF8String& right);

	void				Empty()
							{ innerString = ""; }
	bool				IsEmpty() const
							{ return innerString.empty(); }

	SInt32				Compare(ConstUTF8Str string) const
							{ return innerString.compare(TypeConv::AsASCII(string)); }
	SInt32				Compare(const UTF8String& other) const
							{ return innerString.compare(other.innerString); }

	SInt32				Length() const
							{ return innerString.length(); }
							
	// ---------------------------------------------------------------------
	/*!
		@note
		The byte in the middle of a multi-byte character may be returned.
	*/
	// ---------------------------------------------------------------------
	UTF8Char			operator[](SInt32 index) const
							{ return innerString[index]; }
	
	ConstUTF8Str		CString() const
							{ return TypeConv::AsUTF8(innerString.c_str()); }
						operator ConstUTF8Str() const
							{ return CString(); }
	
	// ---------------------------------------------------------------------
	//! Copies substring.
	/*!
		@note
		If startIndex is not specified appropriately,
		a multi-byte character may be divided.
		In this case, an invalid string is generated
		and subsequent operation cannot be predicted.
	*/
	// ---------------------------------------------------------------------
	UTF8String			SubString(SInt32 startIndex) const
							{ return innerString.substr(startIndex); }
	UTF8String			SubString(SInt32 startIndex, SInt32 length) const
							{ return innerString.substr(startIndex, length); }

private:
						UTF8String(const std::string& string) : innerString(string) { }

private:
	std::string			innerString;	//!< string object which takes charge of almost all operations.
};

inline bool operator==(const UTF8String& left, const UTF8String& right)
{
	return (0 == left.Compare(right));
}

inline bool operator==(const UTF8String& left, ConstUTF8Str right)
{
	return (0 == left.Compare(right));
}

inline bool operator==(ConstUTF8Str left, const UTF8String& right)
{
	return (0 == right.Compare(left));
}

inline bool operator!=(const UTF8String& left, const UTF8String& right)
{
	return (0 != left.Compare(right));
}

inline bool operator!=(const UTF8String& left, ConstUTF8Str right)
{
	return (0 != left.Compare(right));
}

inline bool operator!=(ConstUTF8Str left, const UTF8String& right)
{
	return (0 != right.Compare(left));
}

inline bool operator<(const UTF8String& left, const UTF8String& right)
{
	return (0 > left.Compare(right));
}

inline bool operator<(const UTF8String& left, ConstUTF8Str right)
{
	return (0 > left.Compare(right));
}

inline bool operator<(ConstUTF8Str left, const UTF8String& right)
{
	return (0 < right.Compare(left));
}

inline bool operator>(const UTF8String& left, const UTF8String& right)
{
	return (0 < left.Compare(right));
}

inline bool operator>(const UTF8String& left, ConstUTF8Str right)
{
	return (0 < left.Compare(right));
}

inline bool operator>(ConstUTF8Str left, const UTF8String& right)
{
	return (0 > right.Compare(left));
}

inline bool operator<=(const UTF8String& left, const UTF8String& right)
{
	return (0 >= left.Compare(right));
}

inline bool operator<=(const UTF8String& left, ConstUTF8Str right)
{
	return (0 >= left.Compare(right));
}

inline bool operator<=(ConstUTF8Str left, const UTF8String& right)
{
	return (0 <= right.Compare(left));
}

inline bool operator>=(const UTF8String& left, const UTF8String& right)
{
	return (0 <= left.Compare(right));
}

inline bool operator>=(const UTF8String& left, ConstUTF8Str right)
{
	return (0 <= left.Compare(right));
}

inline bool operator>=(ConstUTF8Str left, const UTF8String& right)
{
	return (0 >= right.Compare(left));
}

inline UTF8String operator+(const UTF8String& left, UTF8Char character)
{
	return UTF8String(left.innerString + character);
}

inline UTF8String operator+(const UTF8String& left, ConstUTF8Str string)
{
	return UTF8String(left.innerString + string);
}

inline UTF8String operator+(const UTF8String& left, const UTF8String& right)
{
	return UTF8String(left.innerString + right.innerString);
}

inline UTF8String operator+(UTF8Char character, const UTF8String& right)
{
	return UTF8String(character + right.innerString);
}

inline UTF8String operator+(ConstUTF8Str string, const UTF8String& right)
{
	return UTF8String(string + right.innerString);
}

#endif // _UTF8STRING_H_
