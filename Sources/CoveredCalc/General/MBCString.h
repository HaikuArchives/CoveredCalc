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
	@file		MBCString.h
	@brief		Definition of MBCString class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.1.15 created
*/

#ifndef _MBCSTRING_H_
#define _MBCSTRING_H_

#include <string>

// ---------------------------------------------------------------------
//! String class which supports multibyte code set.
// ---------------------------------------------------------------------
class MBCString
{
private:
#if defined(WIN32) && defined (_UNICODE)
	typedef std::wstring innerString_t;
#else
	typedef std::string innerString_t;
#endif

public:
						MBCString() : innerString() { }
						MBCString(AChar character) : innerString(1, character) { }
						MBCString(ConstAStr string) : innerString(string) { }
						MBCString(ConstAStr string, SInt32 length) : innerString(string, length) { }
						MBCString(const MBCString& other) : innerString(other.innerString) { }

	MBCString&		operator=(AChar character)
							{ innerString = character; return *this; }
	MBCString&		operator=(ConstAStr string)
							{ innerString = string; return *this; }
	MBCString&		operator=(const MBCString& other)
							{
								if (this != &other)
								{
									innerString = other.innerString;
								}
								return *this;
							}

	MBCString&		operator+=(AChar character)
							{ innerString += character; return *this; }
	MBCString&		operator+=(ConstAStr string)
							{ innerString += string; return *this; }
	MBCString&		operator+=(const MBCString& other)
							{ innerString += other.innerString; return *this; }

	friend MBCString	operator+(const MBCString& left, AChar character);
	friend MBCString operator+(const MBCString& left, ConstAStr string);
	friend MBCString	operator+(const MBCString& left, const MBCString& right);
	friend MBCString operator+(AChar character, const MBCString& right);
	friend MBCString operator+(ConstAStr string, const MBCString& right);

	void				Empty()
							{ innerString.erase(); }
	bool				IsEmpty() const
							{ return innerString.empty(); }

	SInt32				Compare(ConstAStr string) const;
	SInt32				Compare(const MBCString& other) const;
	SInt32				CompareNoCase(ConstAStr string) const;
	SInt32				CompareNoCase(const MBCString& other) const;

	SInt32				Length() const
							{ return static_cast<SInt32>(innerString.length()); }
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
	SInt32				Find(const MBCString& other, SInt32 startIndex = 0) const;
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
	MBCString&		Replace(SInt32 startIndex, SInt32 length, ConstAStr string)
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
	MBCString&		Replace(SInt32 startIndex, SInt32 length, const MBCString& string)
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
	MBCString		SubString(SInt32 startIndex) const
							{ return innerString.substr(startIndex); }
	MBCString		SubString(SInt32 startIndex, SInt32 length) const
							{ return innerString.substr(startIndex, length); }

private:
						MBCString(const innerString_t& string) : innerString(string) { }

private:
	innerString_t		innerString;	/// string object which takes charge of almost all operations.
};

inline bool operator==(const MBCString& left, const MBCString& right)
{
	return (0 == left.Compare(right));
}

inline bool operator==(const MBCString& left, ConstAStr right)
{
	return (0 == left.Compare(right));
}

inline bool operator==(ConstAStr left, const MBCString& right)
{
	return (0 == right.Compare(left));
}

inline bool operator!=(const MBCString& left, const MBCString& right)
{
	return (0 != left.Compare(right));
}

inline bool operator!=(const MBCString& left, ConstAStr right)
{
	return (0 != left.Compare(right));
}

inline bool operator!=(ConstAStr left, const MBCString& right)
{
	return (0 != right.Compare(left));
}

inline bool operator<(const MBCString& left, const MBCString& right)
{
	return (0 > left.Compare(right));
}

inline bool operator<(const MBCString& left, ConstAStr right)
{
	return (0 > left.Compare(right));
}

inline bool operator<(ConstAStr left, const MBCString& right)
{
	return (0 < right.Compare(left));
}

inline bool operator>(const MBCString& left, const MBCString& right)
{
	return (0 < left.Compare(right));
}

inline bool operator>(const MBCString& left, ConstAStr right)
{
	return (0 < left.Compare(right));
}

inline bool operator>(ConstAStr left, const MBCString& right)
{
	return (0 > right.Compare(left));
}

inline bool operator<=(const MBCString& left, const MBCString& right)
{
	return (0 >= left.Compare(right));
}

inline bool operator<=(const MBCString& left, ConstAStr right)
{
	return (0 >= left.Compare(right));
}

inline bool operator<=(ConstAStr left, const MBCString& right)
{
	return (0 <= right.Compare(left));
}

inline bool operator>=(const MBCString& left, const MBCString& right)
{
	return (0 <= left.Compare(right));
}

inline bool operator>=(const MBCString& left, ConstAStr right)
{
	return (0 <= left.Compare(right));
}

inline bool operator>=(ConstAStr left, const MBCString& right)
{
	return (0 >= right.Compare(left));
}

inline MBCString operator+(const MBCString& left, AChar character)
{
	return MBCString(left.innerString + character);
}

inline MBCString operator+(const MBCString& left, ConstAStr string)
{
	return MBCString(left.innerString + string);
}

inline MBCString operator+(const MBCString& left, const MBCString& right)
{
	return MBCString(left.innerString + right.innerString);
}

inline MBCString operator+(AChar character, const MBCString& right)
{
	return MBCString(character + right.innerString);
}

inline MBCString operator+(ConstAStr string, const MBCString& right)
{
	return MBCString(string + right.innerString);
}

#endif // _MBCSTRING_H_
