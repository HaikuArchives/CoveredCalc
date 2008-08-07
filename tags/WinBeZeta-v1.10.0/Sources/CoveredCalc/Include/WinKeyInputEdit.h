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
	@file		WinKeyInputEdit.h
	@brief		Definition of WinKeyInputEdit class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.5.13 created
*/

#ifndef _WINKEYINPUTEDIT_H_
#define _WINKEYINPUTEDIT_H_

#include "HrnWnd.h"
#include "KeyEventParameter.h"
#include "WinMessageFilter.h"

class KeyNameDB;

/**
 *	@brief	Key-input control for Windows.
 */
class WinKeyInputEdit : public CHrnWnd, public WinMessageFilter
{
public:
								WinKeyInputEdit();
	virtual						~WinKeyInputEdit();

	void						Init(const KeyNameDB* keyNameDB);
	void						Attach(HWND hWnd);
	void						Detach();

	const KeyEventParameter&	GetKeyEventParameter() const
									{ return value; }
	void						SetKeyEventParameter(const KeyEventParameter& param);

	// implementation of WinMessageFilter interface
	virtual bool				FilterMessage(MSG* msg);

protected:
	virtual LRESULT				wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LRESULT						onGetDlgCode(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT						onKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT						onChar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT						onContextMenu(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	const KeyNameDB*			keyNameDB;		///< key-name database.
	KeyEventParameter			value;			///< control value.
	HIMC						hImcOrg;		///< original IME context
};
#endif // _WINKEYINPUTEDIT_H_
