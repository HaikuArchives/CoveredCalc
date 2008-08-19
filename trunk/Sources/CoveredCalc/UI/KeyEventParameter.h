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
	@file		KeyEventParameter.h
	@brief		Definition of KeyEventParameter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.23 created
*/

#ifndef _KEYEVENTPARAMETER_H_
#define _KEYEVENTPARAMETER_H_

#if defined(BEOS)
#include <InterfaceDefs.h>
#endif

/**
 *	@brief	Keyboard event parameter.
 */
class KeyEventParameter
{
public:
	enum ModifierMask
	{
		ModifierMask_None	= 0x00,

#if defined(WIN32)
		ModifierMask_Shift		= 0x01,
		ModifierMask_Ctrl		= 0x02,
		ModifierMask_Alt		= 0x04,
		
		ModifierMask_AllMask	= ModifierMask_Shift | ModifierMask_Ctrl | ModifierMask_Alt
#elif defined(BEOS)
		ModifierMask_Shift		= B_SHIFT_KEY,
		ModifierMask_Command	= B_COMMAND_KEY,
		ModifierMask_Control	= B_CONTROL_KEY,
		ModifierMask_Option		= B_OPTION_KEY,

		ModifierMask_AllMask	= ModifierMask_Shift | ModifierMask_Command | ModifierMask_Control | ModifierMask_Option
#endif
	};

public:
#if defined(WIN32)
	typedef			DWORD	KeyCode;
#elif defined(BEOS)
	typedef			int32	KeyCode;
#endif

public:
					KeyEventParameter() { keyCode = 0; modifiers = 0; }
					KeyEventParameter(const KeyEventParameter& other)
						{
							this->keyCode = other.keyCode;
							this->modifiers = other.modifiers;
						}
	virtual			~KeyEventParameter() { }
	
	KeyCode			GetKeyCode() const
						{ return keyCode; }
	UInt32			GetModifiers() const
						{ return modifiers; }
	
	void			SetKeyCode(KeyCode keyCode)
						{ this->keyCode = keyCode; }
	void			SetModifiers(UInt32 modifiers)
						{ this->modifiers = modifiers; }
	
	bool			IsValid() const
						{ return keyCode != 0; }

	bool			IsSame(const KeyEventParameter& other) const
						{
							return (this->keyCode == other.keyCode &&
									(this->modifiers & ModifierMask_AllMask) == (other.modifiers & ModifierMask_AllMask));
						}

	KeyEventParameter& operator=(const KeyEventParameter& other)
						{
							if (this != &other)
							{
								this->keyCode = other.keyCode;
								this->modifiers = other.modifiers;
							}
							return *this;
						}
	
private:
	KeyCode			keyCode;			///< key code
	UInt32			modifiers;			///< modifiers
};

#endif // _KEYEVENTPARAMETER_H_
