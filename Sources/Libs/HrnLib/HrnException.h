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

// HrnException.h - CHrnException
// 例外クラス

#ifndef __HRN_EXCEPTION_H_
#define __HRN_EXCEPTION_H_

#include <windows.h>

// CHrnException - ベースとなる例外クラス
class CHrnException
{
public:
	CHrnException( BOOL bAutoDelete );
	virtual ~CHrnException();

	virtual LPCTSTR GetErrorMessage();
	void Delete();

protected:
	BOOL	m_bAutoDelete;
};

// CHrnMemoryException - メモリ不足例外
class CHrnMemoryException : public CHrnException
{
public:
	CHrnMemoryException();
	virtual ~CHrnMemoryException();

	LPCTSTR GetErrorMessage();
};

void ThrowHrnMemoryException();

#endif // __HRN_EXCEPTION_H_
