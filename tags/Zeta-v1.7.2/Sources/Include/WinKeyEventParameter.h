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
	@file		WinKeyEventParameter.h
	@brief		WinKeyEventParameter クラスの定義
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.23 created
*/

#ifndef _WINKEYEVENTPARAMETER_H_
#define _WINKEYEVENTPARAMETER_H_

// ---------------------------------------------------------------------
//! キーボードイベントのパラメータ for Windows
// ---------------------------------------------------------------------
class WinKeyEventParameter
{
public:
	enum ModifierMask
	{
		ModifierMask_None	= 0x00,
		
		ModifierMask_Shift	= 0x01,
		ModifierMask_Ctrl	= 0x02,
		ModifierMask_Alt	= 0x04,
	};

public:
						WinKeyEventParameter();
	virtual				~WinKeyEventParameter() { }
	
	DWORD				GetVirtualKeyCode() const
							{ return virtualKeyCode; }
	DWORD				GetFlags() const
							{ return flags; }
	bool				IsShiftPressed() const
							{ return isModifierPressed(ModifierMask_Shift); }
	bool				IsCtrlPressed() const
							{ return isModifierPressed(ModifierMask_Ctrl); }
	bool				IsAltPressed() const
							{ return isModifierPressed(ModifierMask_Alt); }
	UInt32				GetModifierMask() const
							{ return modifiers; }
	
	void				SetVirtualKeyCode(DWORD virtualKeyCode)
							{ this->virtualKeyCode = virtualKeyCode; }
	void				SetFlags(DWORD flags)
							{ this->flags = flags; }
	void				SetShiftPressed(bool isPressed)
							{ setModifierPressed(ModifierMask_Shift, isPressed); }
	void				SetCtrlPressed(bool isPressed)
							{ setModifierPressed(ModifierMask_Ctrl, isPressed); }
	void				SetAltPressed(bool isPressed)
							{ setModifierPressed(ModifierMask_Alt, isPressed); }

private:
	bool				isModifierPressed(DWORD modifierMask) const
							{ return (modifiers & modifierMask) ? true : false; }
	void				setModifierPressed(DWORD modifierMask, bool isPressed);
	
private:
	DWORD				virtualKeyCode;
	DWORD				flags;
	UInt32				modifiers;
};

inline WinKeyEventParameter::WinKeyEventParameter()
{
	virtualKeyCode = 0;
	flags = 0;
	modifiers = 0;
}

inline 	void WinKeyEventParameter::setModifierPressed(
	DWORD modifierMask,
	bool isPressed
)
{
	if (isPressed)
	{
		modifiers |= modifierMask;
	}
	else
	{
		modifiers &= ~modifierMask;
	}
}

#endif // _WINKEYEVENTPARAMETER_H_
