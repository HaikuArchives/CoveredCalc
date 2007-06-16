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

// HrnApp.h: CHrnApp クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HRNAPP_H__C915584C_1895_11D4_9AF5_0060672D00C9__INCLUDED_)
#define AFX_HRNAPP_H__C915584C_1895_11D4_9AF5_0060672D00C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// -------------------------------------------------------
// クラス名:	CHrnAppBase
// 機能:	アプリケーションベースクラス
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
class CHrnAppBase
{
public:
	CHrnAppBase();
	virtual ~CHrnAppBase();

	static CHrnAppBase* GetAppObject() { return m_gOnlyOneAppP; }
	HINSTANCE GetInstanceHandle() const { return m_hInstance; }

protected:
	static CHrnAppBase*	m_gOnlyOneAppP;		// 1つしかない CHrnAppBase オブジェクト
	HINSTANCE		m_hInstance;		// パラメータを記憶する

};

// -------------------------------------------------------
// クラス名:	CHrnApp
// 機能:	アプリケーションクラス
// 作成:	Hironori Ichimiya / 2000.4.22
// 特記事項:	
// -------------------------------------------------------
class CHrnApp : public CHrnAppBase
{
public:
	CHrnApp();
	virtual ~CHrnApp();

	int WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );
		
	HINSTANCE GetPrevInstanceHandle() const { return m_hPrevInstance; }
	LPCSTR GetCmdLine() const { return m_lpCmdLine; }
	int GetCmdShow() const { return m_nCmdShow; }

protected:
	virtual BOOL initInstance();
	virtual void exitInstance();

	virtual int messageLoop();


protected:
	HINSTANCE	m_hPrevInstance;	// パラメータを記憶する
	LPCSTR		m_lpCmdLine;
	int		m_nCmdShow;
};

// -------------------------------------------------------
// クラス名:	CHrnDll
// 機能:	DLLクラス
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	
// -------------------------------------------------------
class CHrnDll : public CHrnAppBase
{
friend BOOL CallHrnDllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved );

public:
	CHrnDll();
	virtual ~CHrnDll();

protected:
	BOOL _dllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved );
	virtual BOOL onDllProcessAttach( LPVOID lpvReserved );
	virtual BOOL onDllThreadAttach( LPVOID lpvReserved );
	virtual BOOL onDllThreadDetach( LPVOID lpvReserved );
	virtual BOOL onDllProcessDetach( LPVOID lpvReserved );
};

// -------------------------------------------------------
// 関数名:	CallHrnDllMain()
// 機能:	CHrnDllから派生したクラスのDLLMainを呼ぶ
// 引数:	
// 戻り値:	
// 作成:	Hironori Ichimiya / 2000.5.18
// 特記事項:	実際はここでそのクラスのインスタンスの作成を行っている。
// -------------------------------------------------------
template<class dllclass>
BOOL CallHrnDllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	dllclass*	pDll;
	if ( DLL_PROCESS_ATTACH == fdwReason )
	{
		pDll = new dllclass;
		return pDll->_dllMain( hinstDLL, fdwReason, lpvReserved );
	}
	else
	{
		pDll = (dllclass*)CHrnAppBase::GetAppObject();
		return pDll->_dllMain( hinstDLL, fdwReason, lpvReserved );
	}
}
	
#endif // !defined(AFX_HRNAPP_H__C915584C_1895_11D4_9AF5_0060672D00C9__INCLUDED_)
