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

// HrnApp.cpp: CHrnApp クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "Prefix.h"
#include <crtdbg.h>
#include "HrnApp.h"

CHrnAppBase* CHrnAppBase::m_gOnlyOneAppP = NULL;		// 1つしかない CHrnAppBase オブジェクト

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

// CHrnAppBase
////////////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------
// 関数名:	CHrnAppBase::CHrnAppBase()
// 機能:	コンストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
CHrnAppBase::CHrnAppBase()
{
	m_hInstance = NULL;
}

// -------------------------------------------------------
// 関数名:	CHrnAppBase::~CHrnAppBase()
// 機能:	デストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
CHrnAppBase::~CHrnAppBase()
{
}

// CHrnApp
////////////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------
// 関数名:	CHrnApp::CHrnApp()
// 機能:	コンストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.22
// 特記事項:	
// -------------------------------------------------------
CHrnApp::CHrnApp()
{
	m_hPrevInstance = NULL;
	m_lpCmdLine = NULL;
	m_nCmdShow = SW_SHOWDEFAULT;

}

// -------------------------------------------------------
// 関数名:	CHrnApp::~CHrnApp()
// 機能:	デストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.22
// 特記事項:	
// -------------------------------------------------------
CHrnApp::~CHrnApp()
{

}

// -------------------------------------------------------
// 関数名:	CHrnApp::WinMain()
// 機能:	プログラムのエントリポイント
// 引数:	
// 戻り値:	
// 作成:	Hironori Ichimiya / 2000.4.22
// 特記事項:	これを呼べるのはプログラムで1回だけ。
// -------------------------------------------------------
int CHrnApp::WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	int	retValue = 0;

	// 1回しか呼んではいけない。
	if ( NULL != m_gOnlyOneAppP )
	{
		_ASSERT( FALSE );
		return 0;
	}

	// 唯一の 有効なCHrnAppBaseオブジェクトは自分自身
	m_gOnlyOneAppP = this;

	// パラメータを保存しておく
	m_hInstance = hInstance;
	m_hPrevInstance = hPrevInstance;
	m_lpCmdLine = lpCmdLine;
	m_nCmdShow = nCmdShow;

	// インスタンス初期化
	if ( initInstance() )
	{
		// メッセージループ
		retValue = messageLoop();
	}

	// インスタンス終了処理
	exitInstance();

	return retValue;
}

// -------------------------------------------------------
// 関数名:	CHrnApp::initInstance()
// 機能:	インスタンス初期化
// 引数:	(なし)
// 戻り値:	TRUE: 成功 / FALSE: 失敗
// 作成:	Hironori Ichimiya / 2000.4.22
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnApp::initInstance()
{
	return TRUE;
}

// -------------------------------------------------------
// 関数名:	CHrnApp::exitInstance()
// 機能:	インスタンス終了処理
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.4.22
// 特記事項:	
// -------------------------------------------------------
void CHrnApp::exitInstance()
{
}



// -------------------------------------------------------
// 関数名:	CHrnApp::messageLoop()
// 機能:	メッセージループ
// 引数:	(なし)
// 戻り値:	アプリケーションの戻り値
// 作成:	Hironori Ichimiya / 2000.4.22
// 特記事項:	キーボードアクセラレータを使ったり、モードレスダイアログを
//		使ったりする場合はオーバーライドして、
//		TranslateAccelerator や IsDialogMessage を使う。
// -------------------------------------------------------
int CHrnApp::messageLoop()
{
	MSG	msg;
	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return msg.wParam;
}

// CHrnDll
////////////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------
// 関数名:	CHrnDll::CHrnDll()
// 機能:	コンストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
CHrnDll::CHrnDll()
{
}

// -------------------------------------------------------
// 関数名:	CHrnDll::~CHrnDll()
// 機能:	デストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
CHrnDll::~CHrnDll()
{
}

// -------------------------------------------------------
// 関数名:	CHrnDll::_dllMain()
// 機能:	DllMain()
// 引数:	
// 戻り値:	
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnDll::_dllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch( fdwReason )
	{
	case DLL_PROCESS_ATTACH:
		// 1回しか呼ばれないはず。
		if ( NULL != m_gOnlyOneAppP )
		{
			_ASSERT( FALSE );
			return FALSE;
		}

		// 唯一の 有効なCHrnAppBaseオブジェクトを生成
		m_gOnlyOneAppP = this;

		// パラメータを保存しておく
		m_hInstance = hinstDLL;

		return onDllProcessAttach( lpvReserved );
		break;

	case DLL_THREAD_ATTACH:
		if ( NULL == m_gOnlyOneAppP )
		{
			_ASSERT( FALSE );
			return FALSE;
		}
		return onDllThreadAttach( lpvReserved );
		break;

	case DLL_THREAD_DETACH:
		if ( NULL == m_gOnlyOneAppP )
		{
			_ASSERT( FALSE );
			return FALSE;
		}
		return onDllThreadDetach( lpvReserved );
		break;

	case DLL_PROCESS_DETACH:
		if ( NULL == m_gOnlyOneAppP )
		{
			_ASSERT( FALSE );
			return FALSE;
		}
		return onDllProcessDetach( lpvReserved );
		break;

	default:
		return FALSE;
	}
}

// -------------------------------------------------------
// 関数名:	CHrnDll::onDllProcessAttach()
// 機能:	DLL_PROCESS_ATTACH のときの処理
// 引数:	LPVOID lpvReserved
// 戻り値:	BOOL
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnDll::onDllProcessAttach( LPVOID lpvReserved )
{
	UNREFERENCED_PARAMETER( lpvReserved );
	return TRUE;
}

// -------------------------------------------------------
// 関数名:	CHrnDll::onDllThreadAttach()
// 機能:	DLL_Thread_ATTACH のときの処理
// 引数:	LPVOID lpvReserved
// 戻り値:	BOOL
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnDll::onDllThreadAttach( LPVOID lpvReserved )
{
	UNREFERENCED_PARAMETER( lpvReserved );
	return TRUE;
}

// -------------------------------------------------------
// 関数名:	CHrnDll::onDllThreadDetach()
// 機能:	DLL_THREAD_DETACH のときの処理
// 引数:	LPVOID lpvReserved
// 戻り値:	BOOL
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnDll::onDllThreadDetach( LPVOID lpvReserved )
{
	UNREFERENCED_PARAMETER( lpvReserved );
	return TRUE;
}

// -------------------------------------------------------
// 関数名:	CHrnDll::onDllProcessDetach()
// 機能:	DLL_PROCESS_Detach のときの処理
// 引数:	LPVOID lpvReserved
// 戻り値:	BOOL
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
BOOL CHrnDll::onDllProcessDetach( LPVOID lpvReserved )
{
	UNREFERENCED_PARAMETER( lpvReserved );
	return TRUE;
}
