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

// HrnDlg.cpp: CHrnDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "Prefix.h"
#include <windowsx.h>
#include <tchar.h>
#include "HrnException.h"
#include "HrnDlg.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

// -------------------------------------------------------
// 関数名:	CHrnDlg::CHrnDlg()
// 機能:	テンプレート文字列によるコンストラクタ
// 引数:	LPCTSTR		szTemplate	リソーステンプレート文字列 (IN)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.16
// 特記事項:	
// -------------------------------------------------------
CHrnDlg::CHrnDlg( LPCTSTR szTemplate )
{
	m_szTemplate = szTemplate;
	m_bModal = TRUE;
}

// -------------------------------------------------------
// 関数名:	CHrnDlg::CHrnDlg()
// 機能:	テンプレートIDによるコンストラクタ
// 引数:	UINT		uTemplateID	リソーステンプレートID (IN)
//		HINSTANCE	hInstance	インスタンスハンドル (IN)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.16
// 特記事項:	
// -------------------------------------------------------
CHrnDlg::CHrnDlg( UINT uTemplateID )
{
	m_szTemplate = MAKEINTRESOURCE( uTemplateID );
	m_bModal = TRUE;
}

// -------------------------------------------------------
// 関数名:	CHrnDlg::~CHrnDlg()
// 機能:	デストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.16
// 特記事項:	
// -------------------------------------------------------
CHrnDlg::~CHrnDlg()
{

}

// -------------------------------------------------------
// 関数名:	CHrnDlg::DoModal()
// 機能:	モーダルダイアログの生成
// 引数:	HWND	hParent	親ウィンドウのハンドル (IN)
// 戻り値:	EndDialog の戻り値
// 作成:	Hironori Ichimiya / 2000.4.16
// 特記事項:	
// -------------------------------------------------------
int CHrnDlg::DoModal( HWND hParent, HINSTANCE hInstance )
{
	m_bModal = TRUE;
	return (int)::DialogBoxParam( hInstance, m_szTemplate, hParent, CHrnDlg::cbHrnDlgProc, (LPARAM)this );
}

// -------------------------------------------------------
// 関数名:	CHrnDlg::Create()
// 機能:	モードレスダイアログの生成
// 引数:	HWND	hParent	親ウィンドウのハンドル (IN)
// 戻り値:	TRUE: 成功 / FALSE: 失敗
// 作成:	Hironori Ichimiya / 2004.3.10
// 特記事項:
// -------------------------------------------------------
BOOL CHrnDlg::Create( HWND hParent, HINSTANCE hInstance )
{
	m_bModal = FALSE;
	HWND hWnd = ::CreateDialogParam( hInstance, m_szTemplate, hParent, CHrnDlg::cbHrnDlgProc, (LPARAM)this );
	if (NULL != hWnd)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// -------------------------------------------------------
// 関数名:	CHrnDlg::cbHrnDlgProc()
// 機能:	(隠された)ダイアログプロシージャ
// 引数:	HWND	hwndDlg	ウィンドウハンドル (IN)
//		UINT	uMsg	メッセージ (IN)
//		WPARAM	wParam	WPARAM (IN)
//		LPARAM	lParam	LPARAM (IN)
// 戻り値:	LRESULT	処理結果
// 作成:	Hironori Ichimiya / 2000.4.16
// 特記事項:	ここで CHrnDlg オブジェクトの wndProc() を
//		呼んでやります。
// -------------------------------------------------------
INT_PTR CALLBACK CHrnDlg::cbHrnDlgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CHrnDlg*	pDlg = NULL;
	LRESULT		lResult = FALSE;

	switch( uMsg )
	{
	case WM_INITDIALOG:
		// ウィンドウハンドルとオブジェクトを結びつける
		pDlg = (CHrnDlg*)lParam;
		if ( NULL != pDlg )
		{
			if ( !m_gHrnWndMap.AddHrnWndObject( hwndDlg, pDlg ) )
			{
				// 結び付け失敗!!
#ifdef _DEBUG
				::OutputDebugString( _T("ウィンドウハンドルとオブジェクトの結びつけに失敗しました!!\n") );
#endif
				if ( pDlg->m_bModal )
				{
					::EndDialog( hwndDlg, IDCANCEL );
				}
				else
				{
					::DestroyWindow( hwndDlg );
				}
				return FALSE;
			}
			// オブジェクトにウィンドウハンドルを持たせてやる
			pDlg->m_hWnd = hwndDlg;
		}
		break;
	case WM_NCDESTROY:
		// ウィンドウハンドルとオブジェクトを切り離す
		pDlg = (CHrnDlg*)m_gHrnWndMap.RemoveHrnWndObject( hwndDlg );	// ダウンキャストしてまふ :P
		break;

	default:
		// 該当オブジェクトを取得
		pDlg = (CHrnDlg*)m_gHrnWndMap.GetHrnWndObject( hwndDlg );	// ダウンキャストしてまふ :P
	}

	// ウィンドウプロシージャをコール
	if ( NULL != pDlg )
	{
		try
		{
			lResult = pDlg->wndProc( hwndDlg, uMsg, wParam, lParam );
		}
		catch( CHrnException* e )
		{
			if ( IDCANCEL == ::MessageBox( NULL, e->GetErrorMessage(), "", MB_OKCANCEL | MB_ICONSTOP ) )
			{
				::PostQuitMessage( 0 );
			}
		}
	}

	// 削除のときはウィンドウハンドルを空にする
	if ( WM_NCDESTROY == uMsg && NULL != pDlg )
	{
		pDlg->m_hWnd = NULL;
	}

	return (INT_PTR)lResult;
}

// -------------------------------------------------------
// 関数名:	CHrnDlg::wndProc()
// 機能:	ウィンドウプロシージャ
// 引数:	HWND	hWnd	ウィンドウハンドル (IN)
//		UINT	uMsg	メッセージ (IN)
//		WPARAM	wParam	WPARAM (IN)
//		LPARAM	lParam	LPARAM (IN)
// 戻り値:	LRESULT	処理結果
// 作成:	Hironori Ichimiya / 2000.4.16
// 特記事項:	派生クラスではこの関数をオーバーライドにより
//		イベントをハンドルする。
// -------------------------------------------------------
LRESULT CHrnDlg::wndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
				return SetDlgMsgResult( hWnd, uMsg, result );
			}
		}
	}

	// デフォルトで親ウィンドウの中央に配置します。
	if ( WM_INITDIALOG == uMsg )
	{
		HWND	parentH;
		parentH = ::GetParent( hWnd );
		RECT	parentRect;
		if ( NULL == parentH )
		{	// トップレベルのダイアログ
			::SystemParametersInfo( SPI_GETWORKAREA, 0, &parentRect, 0 );
		}
		else
		{
			::GetWindowRect( parentH, &parentRect );
		}

		RECT	myRect;

		::GetWindowRect( hWnd, &myRect );

		::MoveWindow( hWnd,
				( parentRect.right - parentRect.left ) / 2 - ( myRect.right - myRect.left ) / 2 + parentRect.left,
				( parentRect.bottom - parentRect.top ) / 2 - ( myRect.bottom - myRect.top ) / 2 + parentRect.top,
				( myRect.right - myRect.left ), ( myRect.bottom - myRect.top ), FALSE );
		return FALSE;
	}

	// デフォルトで IDCANCEL, IDOK は EndDialog を呼び出します。
	if ( m_bModal )
	{
		if ( WM_COMMAND == uMsg )
		{
			WORD	wId = LOWORD( wParam );
			if ( IDCANCEL == wId || IDOK == wId )
			{
				::EndDialog( hWnd, (int)wId );
				return TRUE;
			}
		}
	}

	// 処理しなかったメッセージは FALSE を返す。
	return FALSE;
}
