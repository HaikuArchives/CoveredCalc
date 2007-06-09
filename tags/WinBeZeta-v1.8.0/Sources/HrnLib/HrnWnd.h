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

// HrnWnd.h: CHrnWnd クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HRNWND_H__1E16F9DF_8EB0_4DFC_8A73_9A5D4C5EB9FD__INCLUDED_)
#define AFX_HRNWND_H__1E16F9DF_8EB0_4DFC_8A73_9A5D4C5EB9FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHrnWnd;

// -------------------------------------------------------
// クラス名:	CHrnWndMap
// 機能:	ウィンドウハンドルとCHrnWndオブジェクトを
//		結びつけるデータ管理クラス
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
class CHrnWndMap
{
public:
	CHrnWndMap();
	virtual ~CHrnWndMap();

	BOOL AddHrnWndObject( HWND hWnd, CHrnWnd* pWnd );
	CHrnWnd* RemoveHrnWndObject( HWND hWnd );
	CHrnWnd* GetHrnWndObject( HWND hWnd );

protected:
	enum
	{
		HASH_SIZE = 256,	// ハッシュテーブルのサイズ
	};
	struct MAPDATA
	{
		HWND		hWnd;
		CHrnWnd*	pWnd;
		MAPDATA*	pNextData;
	};
	
	int hashWndHandle( HWND hWnd );
	MAPDATA* newMapData( HWND hWnd, CHrnWnd* pWnd );
	void deleteMapData( MAPDATA* pMapData );

protected:
	MAPDATA*	m_mapHash[ HASH_SIZE ];
};

// -------------------------------------------------------
// 関数名:	CHrnWndMap::hashWndHandle()
// 機能:	ハッシュテーブルのインデックスを求める。
// 引数:	HWND	hWnd (IN)
// 戻り値:	インデックス ( 0 ～ HASH_SIZE - 1 )
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
inline int CHrnWndMap::hashWndHandle( HWND hWnd )
{
	// NT/2000 で Spy++ してみたら、ウィンドウハンドルは 2 の倍数になっていた。
	// ちなみに、Win98 の場合は 4 の倍数。
	// 下の方法だと、Win98 のときはハッシュテーブルの半分しか使われなくて
	// 非効率といえば非効率。だが、連続してウィンドウを作ると、
	// ウィンドウハンドルは順に(NT/2000なら2ずつ、98なら4ずつ)インクリメント
	// されることを考えると、連続してウィンドウを作ったときに
	// NT/2000 で必ず2回に1回ぶつかる(=遅くなる)よりは、マシと考える。
	// だって、一気に256個もウィンドウ作らないだろうから、
	// 結局、2回に1回ぶつかるのは偏っていることになるので。
	return ( (DWORD)hWnd / 2 ) % HASH_SIZE;
}

// -------------------------------------------------------
// クラス名:	CHrnWnd
// 機能:	Windowsのウィンドウのクラス
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
class CHrnWnd  
{
public:
	CHrnWnd();
	virtual ~CHrnWnd();

	BOOL Create( LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	BOOL CreateEx( DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );

	BOOL Create( LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu );
	BOOL CreateEx( DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu );

	static ATOM RegisterClass( UINT style, int cbClsExtra, int cbWndExtra, HINSTANCE hInstance, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm );

	BOOL Subclass( HWND hWnd );
	void UnSubclass();


public:
	HWND	m_hWnd;
	WNDPROC	m_orgWndProc;


public:
	virtual BOOL OnRefrectNotify( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult );	

protected:
	virtual LRESULT wndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:
	static CHrnWndMap	m_gHrnWndMap;		// ウィンドウハンドルとオブジェクトを結びつけるマッパー


protected:
	static LRESULT CALLBACK cbHrnWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};

#endif // !defined(AFX_HRNWND_H__1E16F9DF_8EB0_4DFC_8A73_9A5D4C5EB9FD__INCLUDED_)
