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
	@file		BeMBCString.h
	@brief		Definition of BeMBCString class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.2.14 created
*/

#ifndef _BEMBCSTRING_H_
#define _BEMBCSTRING_H_

#include <string>

// ---------------------------------------------------------------------
//! String class which supports multibyte code set.
// ---------------------------------------------------------------------
class BeMBCString
{
public:
						BeMBCString() : innerString() { }
						BeMBCString(AChar character) : innerString(1, character) { }
						BeMBCString(ConstAStr string) : innerString(string) { }
						BeMBCString(ConstAStr string, SInt32 length) : innerString(string, length) { }
						BeMBCString(const BeMBCString& other) : innerString(other.innerString) { }

	BeMBCString&		operator=(AChar character)
							{ innerString = character; return *this; }
	BeMBCString&		operator=(ConstAStr string)
							{ innerString = string; return *this; }
	BeMBCString&		operator=(const BeMBCString& other)
							{
								if (this != &other)
								{
									innerString = other.innerString;
								}
								return *this;
							}

	BeMBCString&		operator+=(AChar character)
							{ innerString += character; return *this; }
	BeMBCString&		operator+=(ConstAStr string)
							{ innerString += string; return *this; }
	BeMBCString&		operator+=(const BeMBCString& other)
							{ innerString += other.innerString; return *this; }

	friend BeMBCString	operator+(const BeMBCString& left, AChar character);
	friend BeMBCString operator+(const BeMBCString& left, ConstAStr string);
	friend BeMBCString	operator+(const BeMBCString& left, const BeMBCString& right);
	friend BeMBCString operator+(AChar character, const BeMBCString& right);
	friend BeMBCString operator+(ConstAStr string, const BeMBCString& right);

	void				Empty()
							{ innerString = ""; }
	bool				IsEmpty() const
							{ return innerString.empty(); }

	SInt32				Compare(ConstAStr string) const;
	SInt32				Compare(const BeMBCString& other) const;
	SInt32				CompareNoCase(ConstAStr string) const;
	SInt32				CompareNoCase(const BeMBCString& other) const;

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
	SInt32				Find(const BeMBCString& other, SInt32 startIndex = 0) const;
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
	BeMBCString&		Replace(SInt32 startIndex, SInt32 length, ConstAStr string)
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
	BeMBCString&		Replace(SInt32 startIndex, SInt32 length, const BeMBCString& string)
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
	BeMBCString		SubString(SInt32 startIndex) const
							{ return innerString.substr(startIndex); }
	BeMBCString		SubString(SInt32 startIndex, SInt32 length) const
							{ return innerString.substr(startIndex, length); }

private:
						BeMBCString(const std::string& string) : innerString(string) { }

private:
	std::string			innerString;	//!< string object which takes charge of almost all operations.
};

inline bool operator==(const BeMBCString& left, const BeMBCString& right)
{
	return (0 == left.Compare(right));
}

inline bool operator==(const BeMBCString& left, ConstAStr right)
{
	return (0 == left.Compare(right));
}

inline bool operator==(ConstAStr left, const BeMBCString& right)
{
	return (0 == right.Compare(left));
}

inline bool operator!=(const BeMBCString& left, const BeMBCString& right)
{
	return (0 != left.Compare(right));
}

inline bool operator!=(const BeMBCString& left, ConstAStr right)
{
	return (0 != left.Compare(right));
}

inline bool operator!=(ConstAStr left, const BeMBCString& right)
{
	return (0 != right.Compare(left));
}

inline bool operator<(const BeMBCString& left, const BeMBCString& right)
{
	return (0 > left.Compare(right));
}

inline bool operator<(const BeMBCString& left, ConstAStr right)
{
	return (0 > left.Compare(right));
}

inline bool operator<(ConstAStr left, const BeMBCString& right)
{
	return (0 < right.Compare(left));
}

inline bool operator>(const BeMBCString& left, const BeMBCString& right)
{
	return (0 < left.Compare(right));
}

inline bool operator>(const BeMBCString& left, ConstAStr right)
{
	return (0 < left.Compare(right));
}

inline bool operator>(ConstAStr left, const BeMBCString& right)
{
	return (0 > right.Compare(left));
}

inline bool operator<=(const BeMBCString& left, const BeMBCString& right)
{
	return (0 >= left.Compare(right));
}

inline bool operator<=(const BeMBCString& left, ConstAStr right)
{
	return (0 >= left.Compare(right));
}

inline bool operator<=(ConstAStr left, const BeMBCString& right)
{
	return (0 <= right.Compare(left));
}

inline bool operator>=(const BeMBCString& left, const BeMBCString& right)
{
	return (0 <= left.Compare(right));
}

inline bool operator>=(const BeMBCString& left, ConstAStr right)
{
	return (0 <= left.Compare(right));
}

inline bool operator>=(ConstAStr left, const BeMBCString& right)
{
	return (0 >= right.Compare(left));
}

inline BeMBCString operator+(const BeMBCString& left, AChar character)
{
	return BeMBCString(left.innerString + character);
}

inline BeMBCString operator+(const BeMBCString& left, ConstAStr string)
{
	return BeMBCString(left.innerString + string);
}

inline BeMBCString operator+(const BeMBCString& left, const BeMBCString& right)
{
	return BeMBCString(left.innerString + right.innerString);
}

inline BeMBCString operator+(AChar character, const BeMBCString& right)
{
	return BeMBCString(character + right.innerString);
}

inline BeMBCString operator+(ConstAStr string, const BeMBCString& right)
{
	return BeMBCString(string + right.innerString);
}


#endif // _BEMBCSTRING_H_
