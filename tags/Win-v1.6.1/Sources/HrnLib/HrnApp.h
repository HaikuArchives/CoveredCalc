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

// HrnApp.h: CHrnApp �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HRNAPP_H__C915584C_1895_11D4_9AF5_0060672D00C9__INCLUDED_)
#define AFX_HRNAPP_H__C915584C_1895_11D4_9AF5_0060672D00C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// -------------------------------------------------------
// �N���X��:	CHrnAppBase
// �@�\:	�A�v���P�[�V�����x�[�X�N���X
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
// -------------------------------------------------------
class CHrnAppBase
{
public:
	CHrnAppBase();
	virtual ~CHrnAppBase();

	static CHrnAppBase* GetAppObject() { return m_gOnlyOneAppP; }
	HINSTANCE GetInstanceHandle() const { return m_hInstance; }

protected:
	static CHrnAppBase*	m_gOnlyOneAppP;		// 1�����Ȃ� CHrnAppBase �I�u�W�F�N�g
	HINSTANCE		m_hInstance;		// �p�����[�^���L������

};

// -------------------------------------------------------
// �N���X��:	CHrnApp
// �@�\:	�A�v���P�[�V�����N���X
// �쐬:	Hironori Ichimiya / 2000.4.22
// ���L����:	
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
	HINSTANCE	m_hPrevInstance;	// �p�����[�^���L������
	LPCSTR		m_lpCmdLine;
	int		m_nCmdShow;
};

// -------------------------------------------------------
// �N���X��:	CHrnDll
// �@�\:	DLL�N���X
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
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
// �֐���:	CallHrnDllMain()
// �@�\:	CHrnDll����h�������N���X��DLLMain���Ă�
// ����:	
// �߂�l:	
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	���ۂ͂����ł��̃N���X�̃C���X�^���X�̍쐬���s���Ă���B
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
