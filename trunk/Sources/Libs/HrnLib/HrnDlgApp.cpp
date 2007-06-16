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

// HrnDlgApp.cpp

#include "Prefix.h"
#include "HrnApp.h"
#include "HrnDlg.h"

// -------------------------------------------------------
// 関数名:	CHrnDlg::DoModal()
// 機能:	モーダルダイアログの生成
// 引数:	HWND	hParent	親ウィンドウのハンドル (IN)
// 戻り値:	EndDialog の戻り値
// 作成:	Hironori Ichimiya / 2000.4.16
// 特記事項:	CHrnAppBaseオブジェクトからインスタンスハンドルを得る。
// -------------------------------------------------------
int CHrnDlg::DoModal( HWND hParent )
{
	return DoModal( hParent, CHrnAppBase::GetAppObject()->GetInstanceHandle() );
}

// -------------------------------------------------------
// 関数名:	CHrnDlg::Create()
// 機能:	モードレスダイアログの生成
// 引数:	HWND	hParent	親ウィンドウのハンドル (IN)
// 戻り値:	TRUE: 成功 / FALSE: 失敗
// 作成:	Hironori Ichimiya / 2004.3.10
// 特記事項:	CHrnAppBaseオブジェクトからインスタンスハンドルを得る。
// -------------------------------------------------------
BOOL CHrnDlg::Create( HWND hParent )
{
	return Create( hParent, CHrnAppBase::GetAppObject()->GetInstanceHandle() );
}
