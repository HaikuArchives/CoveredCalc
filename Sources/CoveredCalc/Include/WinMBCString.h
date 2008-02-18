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
	@file		WinMBCString.h
	@brief		Definition of WinMBCString class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.1.16 created
*/

#ifndef _WINMBCSTRING_H_
#define _WINMBCSTRING_H_

#include <string>

// ---------------------------------------------------------------------
//! String class which supports multibyte code set.
// ---------------------------------------------------------------------
class WinMBCString
{
public:
						WinMBCString() : innerString() { }
						WinMBCString(AChar character) : innerString(1, character) { }
						WinMBCString(ConstAStr string) : innerString(string) { }
						WinMBCString(ConstAStr string, SInt32 length) : innerString(string, length) { }
						WinMBCString(const WinMBCString& other) : innerString(other.innerString) { }

	WinMBCString&		operator=(AChar character)
							{ innerString = character; return *this; }
	WinMBCString&		operator=(ConstAStr string)
							{ innerString = string; return *this; }
	WinMBCString&		operator=(const WinMBCString& other)
							{
								if (this != &other)
								{
									innerString = other.innerString;
								}
								return *this;
							}

	WinMBCString&		operator+=(AChar character)
							{ innerString += character; return *this; }
	WinMBCString&		operator+=(ConstAStr string)
							{ innerString += string; return *this; }
	WinMBCString&		operator+=(const WinMBCString& other)
							{ innerString += other.innerString; return *this; }

	friend WinMBCString	operator+(const WinMBCString& left, AChar character);
	friend WinMBCString operator+(const WinMBCString& left, ConstAStr string);
	friend WinMBCString	operator+(const WinMBCString& left, const WinMBCString& right);
	friend WinMBCString operator+(AChar character, const WinMBCString& right);
	friend WinMBCString operator+(ConstAStr string, const WinMBCString& right);

	void				Empty()
							{ innerString.clear(); }
	bool				IsEmpty() const
							{ return innerString.empty(); }

	SInt32				Compare(ConstAStr string) const;
	SInt32				Compare(const WinMBCString& other) const;
	SInt32				CompareNoCase(ConstAStr string) const;
	SInt32				CompareNoCase(const WinMBCString& other) const;

	SInt32				Length() const
							{ return innerString.length(); }
	// ---------------------------------------------------------------------
	/*!
		@note
		The byte in the middle of a multi-byte character may be returned.
	*/
	// ---------------------------------------------------------------------
	AChar				GetAt(SInt32 index) const
							{ return innerString[index]; }
	AChar				operator[](SInt32 index) const
							{ return innerString[index]; }
	AChar&				operator[](SInt32 index)
							{ return innerString[index]; }
	
	ConstAStr			CString() const
							{ return innerString.c_str(); }
						operator ConstAStr() const
							{ return CString(); }
	
	SInt32				Find(AChar character, SInt32 startIndex = 0) const;
	SInt32				Find(ConstAStr string, SInt32 startIndex = 0) const;
	SInt32				Find(const WinMBCString& other, SInt32 startIndex = 0) const;
	SInt32				RFind(AChar character) const;

	// ---------------------------------------------------------------------
	//! Replaces substring in the string at specified position with specified string.
	/*!
		@note
		If startIndex is not specified appropriately,
		a multi-byte character may be divided.
		In this case, an invalid string is generated
		and subsequent operation cannot be predicted.
	*/
	// ---------------------------------------------------------------------
	WinMBCString&		Replace(SInt32 startIndex, SInt32 length, ConstAStr string)
							{ innerString.replace(startIndex, length, string); return *this; }
	// ---------------------------------------------------------------------
	//! Replaces substring in the string at specified position with specified string.
	/*!
		@note
		If startIndex is not specified appropriately,
		a multi-byte character may be divided.
		In this case, an invalid string is generated
		and subsequent operation cannot be predicted.
	*/
	// ---------------------------------------------------------------------
	WinMBCString&		Replace(SInt32 startIndex, SInt32 length, const WinMBCString& string)
							{ innerString.replace(startIndex, length, string); return *this; }

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
	WinMBCString		SubString(SInt32 startIndex) const
							{ return innerString.substr(startIndex); }
	WinMBCString		SubString(SInt32 startIndex, SInt32 length) const
							{ return innerString.substr(startIndex, length); }

private:
						WinMBCString(const std::string& string) : innerString(string) { }

private:
	std::string			innerString;	//!< string object which takes charge of almost all operations.
};

inline bool operator==(const WinMBCString& left, const WinMBCString& right)
{
	return (0 == left.Compare(right));
}

inline bool operator==(const WinMBCString& left, ConstAStr right)
{
	return (0 == left.Compare(right));
}

inline bool operator==(ConstAStr left, const WinMBCString& right)
{
	return (0 == right.Compare(left));
}

inline bool operator!=(const WinMBCString& left, const WinMBCString& right)
{
	return (0 != left.Compare(right));
}

inline bool operator!=(const WinMBCString& left, ConstAStr right)
{
	return (0 != left.Compare(right));
}

inline bool operator!=(ConstAStr left, const WinMBCString& right)
{
	return (0 != right.Compare(left));
}

inline bool operator<(const WinMBCString& left, const WinMBCString& right)
{
	return (0 > left.Compare(right));
}

inline bool operator<(const WinMBCString& left, ConstAStr right)
{
	return (0 > left.Compare(right));
}

inline bool operator<(ConstAStr left, const WinMBCString& right)
{
	return (0 < right.Compare(left));
}

inline bool operator>(const WinMBCString& left, const WinMBCString& right)
{
	return (0 < left.Compare(right));
}

inline bool operator>(const WinMBCString& left, ConstAStr right)
{
	return (0 < left.Compare(right));
}

inline bool operator>(ConstAStr left, const WinMBCString& right)
{
	return (0 > right.Compare(left));
}

inline bool operator<=(const WinMBCString& left, const WinMBCString& right)
{
	return (0 >= left.Compare(right));
}

inline bool operator<=(const WinMBCString& left, ConstAStr right)
{
	return (0 >= left.Compare(right));
}

inline bool operator<=(ConstAStr left, const WinMBCString& right)
{
	return (0 <= right.Compare(left));
}

inline bool operator>=(const WinMBCString& left, const WinMBCString& right)
{
	return (0 <= left.Compare(right));
}

inline bool operator>=(const WinMBCString& left, ConstAStr right)
{
	return (0 <= left.Compare(right));
}

inline bool operator>=(ConstAStr left, const WinMBCString& right)
{
	return (0 >= right.Compare(left));
}

inline WinMBCString operator+(const WinMBCString& left, AChar character)
{
	return WinMBCString(left.innerString + character);
}

inline WinMBCString operator+(const WinMBCString& left, ConstAStr string)
{
	return WinMBCString(left.innerString + string);
}

inline WinMBCString operator+(const WinMBCString& left, const WinMBCString& right)
{
	return WinMBCString(left.innerString + right.innerString);
}

inline WinMBCString operator+(AChar character, const WinMBCString& right)
{
	return WinMBCString(character + right.innerString);
}

inline WinMBCString operator+(ConstAStr string, const WinMBCString& right)
{
	return WinMBCString(string + right.innerString);
}

#endif // _WINMBCSTRING_H_