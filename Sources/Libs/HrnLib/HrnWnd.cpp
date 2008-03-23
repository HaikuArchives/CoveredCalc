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

// HrnWnd.cpp: CHrnWnd クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "Prefix.h"
#include <crtdbg.h>
#include <tchar.h>
#include <windowsx.h>
#include "HrnException.h"
#include "HrnWnd.h"

// ウィンドウハンドルとCHrnWndオブジェクトを結びつけるマッパー
CHrnWndMap CHrnWnd::m_gHrnWndMap;

// -------------------------------------------------------
// 関数名:	CHrnWndMap::CHrnWndMap()
// 機能:	コンストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
CHrnWndMap::CHrnWndMap()
{
	::ZeroMemory( &m_mapHash, sizeof( m_mapHash ) );
}

// -------------------------------------------------------
// 関数名:	CHrnWndMap::~CHrnWndMap()
// 機能:	デストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
CHrnWndMap::~CHrnWndMap()
{
	// とりあえず、残ってるマップデータがあれば消す
	int nIdx;
	for( nIdx=0; nIdx<HASH_SIZE; nIdx++ )
	{
		MAPDATA*	pNow = m_mapHash[ nIdx ];
		while( NULL != pNow )
		{
			MAPDATA*	pPrev = pNow;
			pNow = pNow->pNextData;
			delete pPrev;
		}
	}
}

// -------------------------------------------------------
// 関数名:	CHrnWndMap::AddHrnWndObject()
// 機能:	CHrnWnd オブジェクトとウィンドウハンドルを結びつける
// 引数:	HWND		hWnd	ウィンドウハンドル (IN)
//		CHrnWnd*	pWnd	CHrnWnd オブジェクト (IN)
// 戻り値:	TRUE	成功
//		FALSE	失敗
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnWndMap::AddHrnWndObject( HWND hWnd, CHrnWnd* pWnd )
{
	// 新しいマップデータを生成
	MAPDATA*	pNewData = newMapData( hWnd, pWnd );
	if ( NULL == pNewData )
	{
		return FALSE;
	}

	// マップデータを入れるハッシュテーブルの場所を決める
	int	nHashIdx = hashWndHandle( hWnd );

	// 入れる場所が決まったら、そこへマップデータを挿入
	pNewData->pNextData = m_mapHash[ nHashIdx ];
	m_mapHash[ nHashIdx ] = pNewData;

	return TRUE;
}

// -------------------------------------------------------
// 関数名:	CHrnWndMap::RemoveHrnWndObject()
// 機能:	ウィンドウハンドルにマッピングされた CHrnWnd オブジェクトを切り離す
// 引数:	HWND	hWnd	ウィンドウハンドル (IN)
// 戻り値:	NULL	もともとマッピングされていない
//		!NULL	マッピングされていた CHrnWnd オブジェクト
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
CHrnWnd* CHrnWndMap::RemoveHrnWndObject( HWND hWnd )
{
	// マップデータの存在するハッシュテーブルの場所を求める
	int	nHashIdx = hashWndHandle( hWnd );

	// そのテーブルから線形リストをたどる
	MAPDATA*	pPrev = NULL;
	MAPDATA*	pNow = m_mapHash[ nHashIdx ];
	while( NULL != pNow )
	{
		if ( hWnd == pNow->hWnd )
		{
			// 見つけた!

			if ( NULL != pPrev )
			{
				// pNow は先頭ではないところ
				pPrev->pNextData = pNow->pNextData;
			}
			else
			{
				// pNow は先頭
				m_mapHash[ nHashIdx ] = NULL;
			}
			
			CHrnWnd* pWndRet = pNow->pWnd;
			deleteMapData( pNow );
			return pWndRet;
		}
		pPrev = pNow;
		pNow = pNow->pNextData;
	}

	return NULL;
}

// -------------------------------------------------------
// 関数名:	CHrnWndMap::GetHrnWndObject()
// 機能:	ウィンドウハンドルにマッピングされている CHrnWnd オブジェクトを返す
// 引数:	HWND	hWnd	ウィンドウハンドル (IN)
// 戻り値:	NULL	マッピングされていない
//		!NULL	マッピングされている CHrnWnd オブジェクト
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
CHrnWnd* CHrnWndMap::GetHrnWndObject( HWND hWnd )
{
	// マップデータの存在するハッシュテーブルの場所を求める
	int	nHashIdx = hashWndHandle( hWnd );

	// そのテーブルから線形リストをたどる
	for( MAPDATA* pNow=m_mapHash[ nHashIdx ]; NULL!=pNow; pNow=pNow->pNextData )
	{
		if ( hWnd == pNow->hWnd )
		{
			// 見つけた!
			return pNow->pWnd;
		}
	}

	return NULL;
}

// -------------------------------------------------------
// 関数名:	CHrnWndMap::newMapData()
// 機能:	新しいマップデータを生成する。
// 引数:	HWND		hWnd	ウィンドウハンドル (IN)
//		CHrnWnd*	pWnd	CHrnWnd オブジェクト (IN)
// 戻り値:	新しいマップデータ (NULL なら失敗)
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
CHrnWndMap::MAPDATA* CHrnWndMap::newMapData( HWND hWnd, CHrnWnd* pWnd )
{
	MAPDATA*	pData = new MAPDATA;

	if ( NULL != pData )
	{
		pData->hWnd = hWnd;
		pData->pWnd = pWnd;
		pData->pNextData = NULL;
	}

	return pData;
}

// -------------------------------------------------------
// 関数名:	CHrnWndMap::deleteMapData()
// 機能:	マップデータを削除する
// 引数:	MAPDATA* pMapData 
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
void CHrnWndMap::deleteMapData( MAPDATA* pMapData )
{
	delete pMapData;
}


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

// -------------------------------------------------------
// 関数名:	CHrnWnd::CHrnWnd()
// 機能:	コンストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
CHrnWnd::CHrnWnd()
{
	m_hWnd = NULL;
	m_orgWndProc = NULL;
}

// -------------------------------------------------------
// 関数名:	CHrnWnd::~CHrnWnd()
// 機能:	デストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
CHrnWnd::~CHrnWnd()
{
	// まだウィンドウが生きているなら消す。
	if ( NULL != m_hWnd )
	{
#ifdef _DEBUG
		::OutputDebugString( _T("CHrnWnd::~CHrnWnd() において、ウィンドウが削除されていないことを検出しました。\n") );
#endif
		m_gHrnWndMap.RemoveHrnWndObject( m_hWnd );
		if ( ::IsWindow( m_hWnd ) )
		{			
#ifdef _DEBUG
			::OutputDebugString( _T("        → ::DestroyWindow() を呼び出します。\n") );
#endif
			::DestroyWindow( m_hWnd );
		}
	}
}

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
//		HINSTANCE	hInstance	インスタンスハンドル (IN)
// 戻り値:	
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnWnd::Create( LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance )
{
	if ( NULL == ::CreateWindow( lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, (LPVOID)this ) )
	{
#ifdef _DEBUG
		DWORD dwError = ::GetLastError();
		_RPT1( _CRT_WARN, "at CHrnWnd::Create() : CreateWindow returns FALSE (last error = %u)\n", dwError );
#endif
		return FALSE;
	}

	return TRUE;
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
//		HINSTANCE	hInstance	インスタンスハンドル (IN)
// 戻り値:	
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnWnd::CreateEx( DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance )
{
	if ( NULL == ::CreateWindowEx( dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, (LPVOID)this ) )
	{
		return FALSE;
	}

	return TRUE;
}

// -------------------------------------------------------
// 関数名:	RegisterClass()
// 機能:	ウィンドウクラスの登録
// 引数:	UINT		style		クラススタイル (IN)
//		int		cbClsExtra	クラスバイトのサイズ (IN)
//		int		cbWndExtra	ウィンドウバイトのサイズ (IN)
//		HINSTANCE	hInstance	インスタンスハンドル (IN)
//		HICON		hIcon		クラスアイコン (IN)
//		HCURSOR		hCursor		クラスカーソル (IN)
//		HBRUSH		hbrBackground	クラス背景ブラシ (IN)
//		LPCTSTR		lpszMenuName	クラスメニュー (IN)
//		LPCTSTR		lpszClassName	クラス名 (IN)
//		HIRON		hIconSm		スモールアイコン (IN)
// 戻り値:	
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	CHrnWndから派生するウィンドウを作成する際は、
//		必ずこの関数で登録したウィンドウクラスを
//		使わなければならない。
// -------------------------------------------------------
ATOM CHrnWnd::RegisterClass( UINT style, int cbClsExtra, int cbWndExtra, HINSTANCE hInstance, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm )
{
	WNDCLASSEX	wc;

	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = style;
	wc.lpfnWndProc = CHrnWnd::cbHrnWndProc;
	wc.cbClsExtra = cbClsExtra;
	wc.cbWndExtra = cbWndExtra;
	wc.hInstance = hInstance;
	wc.hIcon = hIcon;
	wc.hCursor = hCursor;
	wc.hbrBackground = hbrBackground;
	wc.lpszMenuName = lpszMenuName;
	wc.lpszClassName = lpszClassName;
	wc.hIconSm = hIconSm;

	return ::RegisterClassEx( &wc );
}

// -------------------------------------------------------
// 関数名:	CHrnWnd::Subclass()
// 機能:	ウィンドウをサブクラス化し結びつける
// 引数:	HWND	hWnd	サブクラス化するウィンドウのハンドル (IN)
// 戻り値:	BOOL	TRUE:成功 / FALSE:失敗
// 作成:	Hironori Ichimiya / 2000.5.25
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnWnd::Subclass( HWND hWnd )
{
	if ( NULL != m_orgWndProc )
	{
		return FALSE;
	}

	m_orgWndProc = SubclassWindow( hWnd, cbHrnWndProc );
	if ( NULL != m_orgWndProc )
	{
		if ( !m_gHrnWndMap.AddHrnWndObject( hWnd, this ) )
		{
			// 結び付け失敗!!
#ifdef _DEBUG
			::OutputDebugString( _T("CHrnSbclsWnd::SubclassWindow() ウィンドウハンドルとオブジェクトの結びつけに失敗しました!!\n") );
#endif
			SubclassWindow( hWnd, m_orgWndProc );
			return FALSE;
		}
		// オブジェクトにウィンドウハンドルを持たせてやる
		m_hWnd = hWnd;
	}

	return TRUE;
}

// -------------------------------------------------------
// 関数名:	CHrnWnd::UnSubclass()
// 機能:	サブクラス化を解除する。
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.5.25
// 特記事項:	
// -------------------------------------------------------
void CHrnWnd::UnSubclass()
{
	if ( NULL == m_orgWndProc )
	{
		return;
	}

	// ウィンドウハンドルとオブジェクトを切り離す
	m_gHrnWndMap.RemoveHrnWndObject( m_hWnd );
	SubclassWindow( m_hWnd, m_orgWndProc );
	m_orgWndProc = NULL;
	m_hWnd = NULL;
}

// -------------------------------------------------------
// 関数名:	CHrnWnd::cbHrnWndProc()
// 機能:	(隠された)ウィンドウプロシージャ
// 引数:	HWND	hWnd	ウィンドウハンドル (IN)
//		UINT	uMsg	メッセージ (IN)
//		WPARAM	wParam	WPARAM (IN)
//		LPARAM	lParam	LPARAM (IN)
// 戻り値:	LRESULT	処理結果
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	ここで CHrnWnd オブジェクトの wndProc() を
//		呼んでやります。
// -------------------------------------------------------
LRESULT CALLBACK CHrnWnd::cbHrnWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 各ウィンドウを取得 (WM_NCCREATE が来るまでは無効)
	CHrnWnd*	pWnd = NULL;
	LRESULT		lResult = 0;

	switch( uMsg )
	{
	case WM_NCCREATE:
		// ウィンドウハンドルとオブジェクトを結びつける
		pWnd = (CHrnWnd*)( ((LPCREATESTRUCT)lParam)->lpCreateParams );
		if ( NULL != pWnd )
		{
			if ( !m_gHrnWndMap.AddHrnWndObject( hWnd, pWnd ) )
			{
				// 結び付け失敗!!
#ifdef _DEBUG
				::OutputDebugString( _T("ウィンドウハンドルとオブジェクトの結びつけに失敗しました!!\n") );
#endif
				return FALSE;
			}
			// オブジェクトにウィンドウハンドルを持たせてやる
			pWnd->m_hWnd = hWnd;
		}
		break;

	case WM_NCDESTROY:
		// ウィンドウハンドルとオブジェクトを切り離す
		pWnd = m_gHrnWndMap.RemoveHrnWndObject( hWnd );
		break;

	default:
		// 該当オブジェクトを取得
		pWnd = m_gHrnWndMap.GetHrnWndObject( hWnd );
	}

	// ウィンドウプロシージャをコール
	if ( NULL != pWnd )
	{
		try
		{
			lResult = pWnd->wndProc( hWnd, uMsg, wParam, lParam );
		}
		catch( CHrnException* e )
		{
			if ( IDCANCEL == ::MessageBox( NULL, e->GetErrorMessage(), "", MB_OKCANCEL | MB_ICONSTOP ) )
			{
				::PostQuitMessage( 0 );
			}
		}
	}
	else
	{	
		// ウィンドウオブジェクトが存在しない!!
		lResult = ::DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	// 削除のときはウィンドウハンドルを空にする
	if ( WM_NCDESTROY == uMsg && NULL != pWnd )
	{
		pWnd->m_hWnd = NULL;
	}

	return lResult;
}

// -------------------------------------------------------
// 関数名:	CHrnWnd::wndProc()
// 機能:	ウィンドウプロシージャ
// 引数:	HWND	hWnd	ウィンドウハンドル (IN)
//		UINT	uMsg	メッセージ (IN)
//		WPARAM	wParam	WPARAM (IN)
//		LPARAM	lParam	LPARAM (IN)
// 戻り値:	LRESULT	処理結果
// 作成:	Hironori Ichimiya / 2000.4.13
// 特記事項:	派生クラスではこの関数をオーバーライドすることにより
//		イベントをハンドルする。
// -------------------------------------------------------
LRESULT CHrnWnd::wndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// WM_NOTIFY の場合は、送った側にもメッセージを処理するチャンスを与える。
	if ( WM_NOTIFY == uMsg )
	{
		CHrnWnd*	pWnd;
		pWnd = m_gHrnWndMap.GetHrnWndObject( ((LPNMHDR)lParam)->hwndFrom );
		if ( NULL != pWnd )
		{
			LRESULT	result;
			if ( pWnd->OnRefrectNotify( hWnd, uMsg, wParam, lParam, &result ) )
			{
				return result;
			}
		}
	}

	if ( NULL != m_orgWndProc )
	{
		return ::CallWindowProc( m_orgWndProc, hWnd, uMsg, wParam, lParam );
	}
	else
	{
		return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
}

// -------------------------------------------------------
// 関数名:	CHrnWnd::OnRefrectNotify()
// 機能:	WM_NOTIFY の送り側への反射
// 引数:	HWND		hWnd		送った先のウィンドウ (IN)
//		UINT		uMsg		(IN)
//		WPARAM		wParam		WPARAM (IN)
//		LPARAM		lParam		LPARAM (IN)
//		LRESULT*	pResult		WM_NOTIFY への戻り値 (OUT)
// 戻り値:	BOOL	TRUE:処理した / FALSE:処理しなかった
// 作成:	Hironori Ichimiya / 2000.5.25
// 特記事項:	戻り値が FALSE の場合は pResult は無視される。
// -------------------------------------------------------
BOOL CHrnWnd::OnRefrectNotify( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	UNREFERENCED_PARAMETER( hWnd );
	UNREFERENCED_PARAMETER( uMsg );
	UNREFERENCED_PARAMETER( wParam );
	UNREFERENCED_PARAMETER( lParam );
	UNREFERENCED_PARAMETER( pResult );
	return FALSE;
}

