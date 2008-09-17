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

// HrnException.cpp - CHrnException
// 例外クラス

#include "Prefix.h"
#include "HrnException.h"

static TCHAR _NoMessageToReport_[] = _T("内部エラーが発生しました。");
static TCHAR _OutOfMemory_[] = _T("メモリが不足しています。");

static CHrnMemoryException _TheUniqueMemoryException_;

///////////////////////////////////////////////////////////////////////////// CHrnException
// -------------------------------------------------------
// 関数名:	CHrnException()
// 機能:	コンストラクタ
// 引数:	bAutoDelete	TRUEにするとCHrnException::Delete()でオブジェクトの削除が行われる
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 1999.5.3
// 特記事項:	
// -------------------------------------------------------
CHrnException::CHrnException( BOOL bAutoDelete )
{
	m_bAutoDelete = bAutoDelete;
}

// -------------------------------------------------------
// 関数名:	~CHrnException()
// 機能:	デストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 1999.5.3
// 特記事項:	
// -------------------------------------------------------
CHrnException::~CHrnException()
{
}

// -------------------------------------------------------
// 関数名:	GetErrorMessage()
// 機能:	エラーメッセージを取得する
// 引数:	(なし)
// 戻り値:	エラーメッセージへのポインタ(一時的なもの)
// 作成:	Hironori Ichimiya / 1999.5.3
// 特記事項:	
// -------------------------------------------------------
LPCTSTR CHrnException::GetErrorMessage()
{
	return _NoMessageToReport_;
}

// -------------------------------------------------------
// 関数名:	Delete()
// 機能:	例外オブジェクトの消去を行う。
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 1999.5.3
// 特記事項:	
// -------------------------------------------------------
void CHrnException::Delete()
{
	if ( m_bAutoDelete )
	{
		delete this;
	}
}

///////////////////////////////////////////////////////////////////////////// CHrnMemoryException
// -------------------------------------------------------
// 関数名:	CHrnMemoryException()
// 機能:	コンストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 1999.5.3
// 特記事項:	
// -------------------------------------------------------
CHrnMemoryException::CHrnMemoryException() : CHrnException( FALSE )
{
}

// -------------------------------------------------------
// 関数名:	~CHrnMemoryException()
// 機能:	デストラクタ
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 1999.5.3
// 特記事項:	
// -------------------------------------------------------
CHrnMemoryException::~CHrnMemoryException()
{
}

// -------------------------------------------------------
// 関数名:	GetErrorMessage()
// 機能:	エラーメッセージを取得する
// 引数:	(なし)
// 戻り値:	エラーメッセージへのポインタ(一時的なもの)
// 作成:	Hironori Ichimiya / 1999.5.3
// 特記事項:	
// -------------------------------------------------------
LPCTSTR CHrnMemoryException::GetErrorMessage()
{
	return _OutOfMemory_;
}

// -------------------------------------------------------
// 関数名:	ThrowHrnMemoryException()
// 機能:	CHrnMemoryException例外(へのポインタ)をスローする。
// 引数:	(なし)
// 戻り値:	(なし)
// 作成:	Hironori Ichimiya / 1999.5.3
// 特記事項:	
// -------------------------------------------------------
void ThrowHrnMemoryException()
{
	throw &_TheUniqueMemoryException_;
}
