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
	@file		WinLayeredWindowAPI.cpp
	@brief		Implementation of WinLayeredWindowAPI class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.4.15 created
*/

#define WINVER			0x0500
#define _WIN32_WINNT	0x0500
#include <windows.h>
#include <tchar.h>
#include "WinLayeredWindowAPI.h"

DWORD WinLayeredWindowAPI::cWS_EX_LAYERED = WS_EX_LAYERED;
DWORD WinLayeredWindowAPI::cULW_ALPHA = ULW_ALPHA;

/**
 *	@brief	コンストラクタ
 */
WinLayeredWindowAPI::WinLayeredWindowAPI()
{
	hDLLUser32 = NULL;
	pfnUpdateLayeredWindow = NULL;
}

/**
 *	@brief	デストラクタ
 */
WinLayeredWindowAPI::~WinLayeredWindowAPI()
{
	Terminate();
}

/**
 *	@brief	初期化処理を行います。
 */
void WinLayeredWindowAPI::Initialize()
{
	Terminate();
	
	hDLLUser32 = ::LoadLibrary(_T("user32.dll"));
	if (NULL != hDLLUser32)
	{
		pfnUpdateLayeredWindow = reinterpret_cast<PFN_UpdateLayeredWindow>(GetProcAddress(hDLLUser32, "UpdateLayeredWindow"));
	}
}

/**
 *	@brief	終了処理を行います。
 */
void WinLayeredWindowAPI::Terminate()
{
	pfnUpdateLayeredWindow = NULL;
	if (NULL != hDLLUser32)
	{
		::FreeLibrary(hDLLUser32);
		hDLLUser32 = NULL;
	}
}

/**
 *	@brief	UpdateLayeredWindow() 関数がサポートされているかどうかを返します。
 *	@retval	true	サポートされています。
 *	@retval	false	サポートされていません。
 */
bool WinLayeredWindowAPI::IsSupported_UpdateLayeredWindow() const
{
	return (NULL != pfnUpdateLayeredWindow) ? true : false;
}

/**
 *	@brief	UpdateLayeredWindow() 関数を呼び出します。
 */
BOOL WinLayeredWindowAPI::UpdateLayeredWindow(
	HWND hWnd,
	HDC hdcDst,
	POINT* pptDst,
	SIZE* psize,
	HDC hdcSrc,
	POINT* ppcSrc,
	COLORREF crKey,
	BLENDFUNCTION* pblend,
	DWORD dwFlags
) const
{
	if (NULL == pfnUpdateLayeredWindow)
	{
		return FALSE;
	}
	return pfnUpdateLayeredWindow(hWnd, hdcDst, pptDst, psize, hdcSrc, ppcSrc, crKey, pblend, dwFlags);
}
