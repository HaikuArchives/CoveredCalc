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
	@file		Prefix.h
	@brief		プリコンパイル済みヘッダーとなるヘッダ
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.22 created
*/

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ヘッダーから使用されていない部分を除外します。
#endif

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows 95 および Windows NT 4 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0400		// これを Windows 98 および Windows 2000 またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINNT 0x0400		// これを Windows 98 および Windows 2000 またはそれ以降のバージョン向けに適切な値に変更してください。
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE			// IE 4.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0400	// これを IE 5.0  またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

// 無視してもい警告メッセージの削除
#pragma warning(disable: 4127)  // ASSERT で「warning C4127: 条件式が定数です。」と言われないために
#pragma warning(disable: 4710)	// 関数 ～ はインライン関数ではありません。
#pragma warning(disable: 4702)	// 制御が渡らないコードです。

// VS2005 の CRT セキュリティ強化に関する未推奨警告を表示しない。
#define _CRT_SECURE_NO_DEPRECATE 1

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>

#include "DebugSupport.h"
#include "PortDefs.h"

// FIXME: 最後には消します。
#include "resource.h"