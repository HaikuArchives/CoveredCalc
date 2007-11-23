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
	@file		WinPreferencesDlg.h
	@brief		Definition of WinPreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.1.14 created
*/

#ifndef _WINPREFERENCESDLG_H_
#define _WINPREFERENCESDLG_H_

#include "WinDialog.h"
#include "PreferencesDlg.h"

/**
 *	@brief	設定ダイアログのクラスです。(Windows 版)
 */
class WinPreferencesDlg : public WinDialog, public PreferencesDlg
{
public:
									WinPreferencesDlg();
	virtual							~WinPreferencesDlg();
	
protected:
	virtual LRESULT					wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void					setLanguage(const LangFileInfoCollection& langFileInfos, const Path& currentLangFilePath);
	virtual	bool					getLanguage(Path& langFilePath);
	virtual void					setKeyMapping(const KeyMappingsInfoPtrVector& keyMappingsInfos, const Path& currentKeyMappingPath);
	virtual bool					getKeyMapping(Path& keyMappingPath);
	virtual void					setOpacity(SInt32 opacity);
	virtual SInt32					getOpacity();
	virtual void					enableOpacity(bool isEnabled);
	virtual void					setEdgeSmoothing(SInt32 edgeSmoothing);
	virtual SInt32					getEdgeSmoothing();
	virtual void					enableEdgeSmoothing(bool isEnabled);

private:
	LRESULT							onInitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT							onCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	const LangFileInfoCollection*	langComboInfos;		///< 言語コンボボックスに格納されたアイテムの情報
};

#endif // _WINPREFERENCESDLG_H_