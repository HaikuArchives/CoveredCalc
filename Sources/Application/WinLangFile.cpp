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
	@file		WinLangFile.cpp
	@brief		WinLangFile クラスの実装
	@author		ICHIMIYA Hironori (Hiron)
	@date		20053.12.18 created
*/

#include "Prefix.h"
#include "WinLangFile.h"
#include "Path.h"
#include "MemoryException.h"
#include "LangFileException.h"

typedef DWORD (APIENTRY *PFNCCNLGETVERSION)();
typedef DWORD (APIENTRY *PFNCCNLGETLANGUAGECODE)(char* szLangCode);
typedef DWORD (APIENTRY *PFNCCNLGETLANGUAGENAME)(char* szLangName);
typedef LANGID (APIENTRY *PFNCCNLGETLANGID)();

#define SPEC_VERSION	(4)		// 言語ファイル仕様のバージョン

/**
 *	@brief	コンストラクタ
 */
WinLangFile::WinLangFile()
{
	hInstance = NULL;
}

/**
 *	@brief	デストラクタ
 */
WinLangFile::~WinLangFile()
{
	Unload();		// 解放されてなければ
}

/**
 *	@brief	言語ファイルをファイルから読み込みます。
 */
void WinLangFile::Load(
	const Path& langFilePath	///< 言語ファイルのパス
)
{
	Unload();
	hInstance = LoadLibrary(langFilePath.GetPathString());
	if (NULL == hInstance)
	{
		throw new LangFileExceptions::LoadFailed(langFilePath);
	}
}

/**
 *	@brief	読み込んでいる言語ファイルをアンロードします。
 */
void WinLangFile::Unload()
{
	if (NULL != hInstance)
	{
		FreeLibrary(hInstance);
		hInstance = NULL;
	}
}

/**
 *	@brief	このクラスが扱う仕様がサポートしているバージョンかどうかを調べます。
 *	@retval	true	サポートしているバージョンです。
 *	@retval	false	サポートしていないバージョンです。
 */
bool WinLangFile::CheckVersion()
{
	if (SPEC_VERSION == GetVersion())
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 *	@brief	言語ファイル仕様のバージョンを返します。
 *	@return	バージョン
 */
DWORD WinLangFile::GetVersion()
{
	PFNCCNLGETVERSION pfnCCNLGetVersion = reinterpret_cast<PFNCCNLGETVERSION>(GetProcAddress(hInstance, "CCNLGetVersion"));
	if (NULL == pfnCCNLGetVersion)
	{
		return 0;
	}

	try
	{
		return pfnCCNLGetVersion();
	}
	catch (...)
	{
		return 0;
	}
}

/**
 *	@brief	言語コードを返します。
 */
void WinLangFile::GetLanguageCode(
	MBCString& langCode			///< 言語コードが返ります。
)
{
	langCode.Empty();
	PFNCCNLGETLANGUAGECODE pfnCCNLGetLanguageCode = reinterpret_cast<PFNCCNLGETLANGUAGECODE>(GetProcAddress(hInstance, "CCNLGetLanguageCode"));
	if (NULL == pfnCCNLGetLanguageCode)
	{
		return;
	}

	DWORD length;
	try
	{
		length = pfnCCNLGetLanguageCode(NULL);
	}
	catch (...)
	{
		return;
	}

	char* langCodeBuf = static_cast<char*>(malloc(length + 1));
	if (NULL == langCodeBuf)
	{
		MemoryException::Throw();
	}
	try
	{
		pfnCCNLGetLanguageCode(langCodeBuf);
	}
	catch (...)
	{
		free(langCodeBuf);
		return;
	}
	langCode = langCodeBuf;
	free(langCodeBuf);
}

/**
 *	@brief	言語名を返します。
 */
void WinLangFile::GetLanguageName(
	MBCString& langName			///< 言語名が返ります。
)
{
	langName.Empty();
	PFNCCNLGETLANGUAGENAME pfnCCNLGetLanguageName = reinterpret_cast<PFNCCNLGETLANGUAGENAME>(GetProcAddress(hInstance, "CCNLGetLanguageName"));
	if (NULL == pfnCCNLGetLanguageName)
	{
		return;
	}

	DWORD length;
	try
	{
		length = pfnCCNLGetLanguageName(NULL);
	}
	catch (...)
	{
		return;
	}

	char* langNameBuf = static_cast<char*>(malloc(length + 1));
	if (NULL == langNameBuf)
	{
		MemoryException::Throw();
	}
	try
	{
		pfnCCNLGetLanguageName(langNameBuf);
	}
	catch (...)
	{
		free(langNameBuf);
		return;
	}
	langName = langNameBuf;
	free(langNameBuf);
}

/**
 *	@brief	この言語の LANGID を返します。
 *	@return	LANGID（失敗すれば 0）
 */
LANGID WinLangFile::GetLangID()
{
	PFNCCNLGETLANGID pfnCCNLGetLangID = reinterpret_cast<PFNCCNLGETLANGID>(GetProcAddress(hInstance, "CCNLGetLangID"));
	if (NULL == pfnCCNLGetLangID)
	{
		return 0;
	}
	return pfnCCNLGetLangID();
}
