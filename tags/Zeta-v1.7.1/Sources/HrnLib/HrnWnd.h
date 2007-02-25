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

// HrnWnd.h: CHrnWnd �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HRNWND_H__1E16F9DF_8EB0_4DFC_8A73_9A5D4C5EB9FD__INCLUDED_)
#define AFX_HRNWND_H__1E16F9DF_8EB0_4DFC_8A73_9A5D4C5EB9FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHrnWnd;

// -------------------------------------------------------
// �N���X��:	CHrnWndMap
// �@�\:	�E�B���h�E�n���h����CHrnWnd�I�u�W�F�N�g��
//		���т���f�[�^�Ǘ��N���X
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
class CHrnWndMap
{
public:
	CHrnWndMap();
	virtual ~CHrnWndMap();

	BOOL AddHrnWndObject( HWND hWnd, CHrnWnd* pWnd );
	CHrnWnd* RemoveHrnWndObject( HWND hWnd );
	CHrnWnd* GetHrnWndObject( HWND hWnd );

protected:
	enum
	{
		HASH_SIZE = 256,	// �n�b�V���e�[�u���̃T�C�Y
	};
	struct MAPDATA
	{
		HWND		hWnd;
		CHrnWnd*	pWnd;
		MAPDATA*	pNextData;
	};
	
	int hashWndHandle( HWND hWnd );
	MAPDATA* newMapData( HWND hWnd, CHrnWnd* pWnd );
	void deleteMapData( MAPDATA* pMapData );

protected:
	MAPDATA*	m_mapHash[ HASH_SIZE ];
};

// -------------------------------------------------------
// �֐���:	CHrnWndMap::hashWndHandle()
// �@�\:	�n�b�V���e�[�u���̃C���f�b�N�X�����߂�B
// ����:	HWND	hWnd (IN)
// �߂�l:	�C���f�b�N�X ( 0 �` HASH_SIZE - 1 )
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
inline int CHrnWndMap::hashWndHandle( HWND hWnd )
{
	// NT/2000 �� Spy++ ���Ă݂���A�E�B���h�E�n���h���� 2 �̔{���ɂȂ��Ă����B
	// ���Ȃ݂ɁAWin98 �̏ꍇ�� 4 �̔{���B
	// ���̕��@���ƁAWin98 �̂Ƃ��̓n�b�V���e�[�u���̔��������g���Ȃ���
	// ������Ƃ����Δ�����B�����A�A�����ăE�B���h�E�����ƁA
	// �E�B���h�E�n���h���͏���(NT/2000�Ȃ�2���A98�Ȃ�4����)�C���N�������g
	// ����邱�Ƃ��l����ƁA�A�����ăE�B���h�E��������Ƃ���
	// NT/2000 �ŕK��2���1��Ԃ���(=�x���Ȃ�)���́A�}�V�ƍl����B
	// �����āA��C��256���E�B���h�E���Ȃ����낤����A
	// ���ǁA2���1��Ԃ���͕̂΂��Ă��邱�ƂɂȂ�̂ŁB
	return ( (DWORD)hWnd / 2 ) % HASH_SIZE;
}

// -------------------------------------------------------
// �N���X��:	CHrnWnd
// �@�\:	Windows�̃E�B���h�E�̃N���X
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
class CHrnWnd  
{
public:
	CHrnWnd();
	virtual ~CHrnWnd();

	BOOL Create( LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	BOOL CreateEx( DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );

	BOOL Create( LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu );
	BOOL CreateEx( DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu );

	static ATOM RegisterClass( UINT style, int cbClsExtra, int cbWndExtra, HINSTANCE hInstance, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm );

	BOOL Subclass( HWND hWnd );
	void UnSubclass();


public:
	HWND	m_hWnd;
	WNDPROC	m_orgWndProc;


public:
	virtual BOOL OnRefrectNotify( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult );	

protected:
	virtual LRESULT wndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:
	static CHrnWndMap	m_gHrnWndMap;		// �E�B���h�E�n���h���ƃI�u�W�F�N�g�����т���}�b�p�[


protected:
	static LRESULT CALLBACK cbHrnWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};

#endif // !defined(AFX_HRNWND_H__1E16F9DF_8EB0_4DFC_8A73_9A5D4C5EB9FD__INCLUDED_)
