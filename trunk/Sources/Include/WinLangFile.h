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
	@file		WinLangFile.h
	@brief		WinLangFile クラスの定義
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.12.18 created
*/

#ifndef _WINLANGFILE_H_
#define _WINLANGFILE_H_

#include "MBCString.h"
#include "Exception.h"

class Path;

/**
 *	@brief	Windows 用言語ファイルを表します。
 */
class WinLangFile
{
public:
								WinLangFile();
	virtual						~WinLangFile();
	
	void						Load(const Path& langFilePath);
	void						Unload();
	
	bool						CheckVersion();
	
	HINSTANCE					GetHandle() const	{ return hInstance; }

	DWORD						GetVersion();
	void						GetLanguageCode(MBCString& langCode);
	void						GetLanguageName(MBCString& langName);
	LANGID						GetLangID();
	
private:
	HINSTANCE					hInstance;		///< インスタンスハンドル
};

#endif // _WINLANGFILE_H_
