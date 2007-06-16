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
	@file		BeKeyEventParameter.h
	@brief		Definition of BeKeyEventParameter class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.08.29 created
*/

#ifndef _BEKEYEVENTPARAMETER_H_
#define _BEKEYEVENTPARAMETER_H_

// ---------------------------------------------------------------------
//! キーボードイベントパラメータ for BeOS
/*!
	@note	このクラスは渡されたポインタの内容をコピーせず、
			そのまま保持します。
*/
// ---------------------------------------------------------------------
class BeKeyEventParameter
{
public:
					BeKeyEventParameter();
	virtual			~BeKeyEventParameter() { }
	
	const char*		GetBytes() const
						{ return bytes; }
	int32			GetNumBytes() const
						{ return numBytes; }
	
	void			SetBytes(const char* bytes, int32 numBytes)
						{ this->bytes = bytes; this->numBytes = numBytes; }

private:
	const char*		bytes;				//!< ユーザが押したキーにマップされる文字
	int32			numBytes;			//!< bytes のバイト数
};

inline BeKeyEventParameter::BeKeyEventParameter()
{
	bytes = NULL;
	numBytes = 0;
}

#endif // _BEKEYEVENTPARAMETER_H_
