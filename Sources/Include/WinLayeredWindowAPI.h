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
	@file		WinLayeredWindowAPI.h
	@brief		Definition of WinLayeredWindowAPI class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.4.15 created
*/

#ifndef _WINLAYEREDWINDOWAPI_H_
#define _WINLAYEREDWINDOWAPI_H_

/**
 *	@brief	レイヤードウィンドウ API のラッパークラス
 */
class WinLayeredWindowAPI
{
public:
								WinLayeredWindowAPI();
	virtual						~WinLayeredWindowAPI();
	
	void						Initialize();
	void						Terminate();
	
	bool						IsSupported_UpdateLayeredWindow() const;
	BOOL						UpdateLayeredWindow(HWND hWnd, HDC hdcDst, POINT* pptDst, SIZE* psize, HDC hdcSrc, POINT* ppcSrc, COLORREF crKey, BLENDFUNCTION* pblend, DWORD dwFlags) const;

	static DWORD				cWS_EX_LAYERED;
	static DWORD				cULW_ALPHA;

private:
	typedef	BOOL (WINAPI *PFN_UpdateLayeredWindow)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);
	HMODULE						hDLLUser32;				///< User32.dll のモジュールハンドル
	PFN_UpdateLayeredWindow		pfnUpdateLayeredWindow;	///< UpdateLayeredWindow API のアドレス
};

#endif // _WINLAYEREDWINDOWAPI_H_
