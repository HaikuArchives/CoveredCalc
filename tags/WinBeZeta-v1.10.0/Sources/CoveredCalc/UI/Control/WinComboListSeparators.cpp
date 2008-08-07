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
	@file		WinComboListSeparators.cpp
	@brief		Implementation of WinComboListSeparators class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.2.24 created
*/

#include "Prefix.h"
#include "WinComboListSeparators.h"

/**
 *	@brief	Constructor
 */
WinComboListSeparators::WinComboListSeparators()
{
}

/**
 *	@brief	Destructor
 */
WinComboListSeparators::~WinComboListSeparators()
{
}

/**
 *	@brief	Attaches to target combobox.
 *	@param[in]	hComboBox	window handle of target combobox.
 *	@return	TRUE if succeeded.
 */
BOOL WinComboListSeparators::Attach(HWND hComboBox)
{
	return Subclass(hComboBox);
}

/**
 *	@brief	Detaches from target combobox.
 */
void WinComboListSeparators::Detach()
{
	if (NULL != listBox.m_hWnd)
	{
		listBox.UnSubclass();
	}
	UnSubclass();
}

/**
 *	@brief	Adds a separator.
 *	@param[in]	index	separator index. (0 is the bottom of first item, 1 is the bottom of second, ...)
 */
void WinComboListSeparators::AddSeparatorAt(SInt32 index)
{
	std::vector<SInt32>::iterator it;
	for (it = separatorIndexes.begin(); it != separatorIndexes.end(); it++)
	{
		if (*it == index)
		{
			// already exists
			return;
		}
	}
	
	separatorIndexes.push_back(index);
}

/**
 *	@brief	Removes a separator.
 *	@param[in]	index	separator index. (see AddSeparatorAt())
 */
void WinComboListSeparators::RemoveSeparatorAt(SInt32 index)
{
	std::vector<SInt32>::iterator it;
	for (it = separatorIndexes.begin(); it != separatorIndexes.end(); it++)
	{
		if (*it == index)
		{
			separatorIndexes.erase(it);
			break;
		}
	}
}

/**
 *	@brief	Removes all separators.
 */
void WinComboListSeparators::ClearAllSeparators()
{
	separatorIndexes.clear();
}

/**
 *	@brief	Windows procedure of a combobox.
 */
LRESULT WinComboListSeparators::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CTLCOLORLISTBOX:
		{
			HWND hListBox = reinterpret_cast<HWND>(lParam);
			if (NULL == listBox.m_hWnd)
			{
				listBox.Subclass(hListBox);
				listBox.SetContainer(this);
			}
			HDC hDC = reinterpret_cast<HDC>(wParam);
			listBox.DrawSeparators(hDC);
		}
		break;
	case WM_DESTROY:
		{
			listBox.UnSubclass();
		}
		break;
	}
	return CHrnWnd::wndProc(hWnd, uMsg, wParam, lParam);

}

void WinComboListSeparators::ListBox::SetContainer(const WinComboListSeparators* container)
{
	this->container = container;
}

/**
 *	@brief	Windows procedure of a listbox of the combobox.
 */
LRESULT WinComboListSeparators::ListBox::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PRINTCLIENT:
		{
			LRESULT result = CHrnWnd::wndProc(hWnd, uMsg, wParam, lParam);
			
			HDC hDC = reinterpret_cast<HDC>(wParam);
			DrawSeparators(hDC);

			return result;
		}
		break;
	default:
		return CHrnWnd::wndProc(hWnd, uMsg, wParam, lParam);
	}
}

/**
 *	@brief	Draws separators on listbox.
 *	@param[in]	hDC	device context.
 */
void WinComboListSeparators::ListBox::DrawSeparators(HDC hDC)
{
	std::vector<SInt32>::const_iterator it;
	for (it = container->separatorIndexes.begin(); it != container->separatorIndexes.end(); it++)
	{
		SInt32 ix = *it;

		RECT rc;
		ListBox_GetItemRect(m_hWnd, ix, &rc);
		rc.top = rc.bottom - 1;
		
		COLORREF oldColor = SetBkColor(hDC, RGB(0,0,0));
		ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
		SetBkColor(hDC, oldColor);
	}
}
