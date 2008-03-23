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
	@file		WinComboListSeparators.h
	@brief		Definition of WinComboListSeparators class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.2.24 created
*/

#ifndef _WINCOMBOLISTSEPARATORS_H_
#define _WINCOMBOLISTSEPARATORS_H_

#include <vector>
#include "HrnWnd.h"

/**
 *	@brief	This class draws separator line on the listbox of specified combobox.
 */
class WinComboListSeparators : public CHrnWnd
{
public:
						WinComboListSeparators();
	virtual				~WinComboListSeparators();
	
	BOOL				Attach(HWND hComboBox);
	void				Detach();

	void				AddSeparatorAt(SInt32 index);
	void				RemoveSeparatorAt(SInt32 index);
	void				ClearAllSeparators();

protected:
	virtual LRESULT		wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	class ListBox : public CHrnWnd
	{
	public:
		void							SetContainer(const WinComboListSeparators* container);
		void							DrawSeparators(HDC hDC);

	protected:
		virtual LRESULT					wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		const WinComboListSeparators*	container;
	};
	friend class ListBox;

	ListBox				listBox;
	std::vector<SInt32>	separatorIndexes;
};

#endif // _WINCOMBOLISTSEPARATORS_H_
