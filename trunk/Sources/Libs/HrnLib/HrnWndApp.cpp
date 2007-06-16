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

// HrnWndApp.cpp

#include "Prefix.h"
#include "HrnApp.h"
#include "HrnWnd.h"

// -------------------------------------------------------
// 関数名:	CHrnWnd::Create()
// 機能:	ウィンドウを作成する。
// 引数:	LPCTSTR		lpClassName	ウィンドウクラス名 (IN)
//		LPCTSTR		lpWindowName	ウィンドウ名 (IN)
//		DWORD		dwStyle		ウィンドウスタイル (IN)
//		int		x		ウィンドウの初期x座標 (IN)
//		int		y		ウィンドウの初期y座標 (IN)
//		int		nWidth		ウィンドウの幅( IN)
//		int		nHeight		ウィンドウの高さ (IN)
//		HWND		hWndParent	親ウィンドウ (IN)
//		HMENU		hMenu		メニューまたは子ウィンドウのID (IN)
// 戻り値:	
// 作成:	Hironori Ichimiya / 2000.11.19
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnWnd::Create( LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu )
{
	return Create( lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, CHrnAppBase::GetAppObject()->GetInstanceHandle() );
}

// -------------------------------------------------------
// 関数名:	CHrnWnd::CreateEx()
// 機能:	拡張スタイル付きでウィンドウを作成する。
// 引数:	DWORD		dwExStyle	拡張スタイル (IN)
//		LPCTSTR		lpClassName	ウィンドウクラス名 (IN)
//		LPCTSTR		lpWindowName	ウィンドウ名 (IN)
//		DWORD		dwStyle		ウィンドウスタイル (IN)
//		int		x		ウィンドウの初期x座標 (IN)
//		int		y		ウィンドウの初期y座標 (IN)
//		int		nWidth		ウィンドウの幅 (IN)
//		int		nHeight		ウィンドウの高さ (IN)
//		HWND		hWndParent	親ウィンドウ (IN)
//		HMENU		hMenu		メニューまたは子ウィンドウのID (IN)
// 戻り値:	
// 作成:	Hironori Ichimiya / 2000.11.19
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnWnd::CreateEx( DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu )
{
	return CreateEx( dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, CHrnAppBase::GetAppObject()->GetInstanceHandle() );
}

