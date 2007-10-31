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

/**
 *	@brief	Keyboard event parameter class for BeOS.
 */
class BeKeyEventParameter
{
public:
					BeKeyEventParameter();
	virtual			~BeKeyEventParameter() { }
	
	int32			GetKeyCode() const
						{ return keyCode; }
	int32			GetModifiers() const
						{ return modifiers; }
	
	void			SetKeyCode(int32 keyCode)
						{ this->keyCode = keyCode; }
	void			SetModifiers(int32 modifiers)
						{ this->modifiers = modifiers; }
	
private:
	int32			keyCode;			///< key code
	int32			modifiers;			///< modifiers
};

inline BeKeyEventParameter::BeKeyEventParameter()
{
	keyCode = 0;
	modifiers = 0;
}

#endif // _BEKEYEVENTPARAMETER_H_
