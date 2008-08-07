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

// HrnWnd.cpp: CHrnWnd �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "Prefix.h"
#include <crtdbg.h>
#include <tchar.h>
#include <windowsx.h>
#include "HrnException.h"
#include "HrnWnd.h"

// �E�B���h�E�n���h����CHrnWnd�I�u�W�F�N�g�����т���}�b�p�[
CHrnWndMap CHrnWnd::m_gHrnWndMap;

// -------------------------------------------------------
// �֐���:	CHrnWndMap::CHrnWndMap()
// �@�\:	�R���X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
CHrnWndMap::CHrnWndMap()
{
	::ZeroMemory( &m_mapHash, sizeof( m_mapHash ) );
}

// -------------------------------------------------------
// �֐���:	CHrnWndMap::~CHrnWndMap()
// �@�\:	�f�X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
CHrnWndMap::~CHrnWndMap()
{
	// �Ƃ肠�����A�c���Ă�}�b�v�f�[�^������Ώ���
	int nIdx;
	for( nIdx=0; nIdx<HASH_SIZE; nIdx++ )
	{
		MAPDATA*	pNow = m_mapHash[ nIdx ];
		while( NULL != pNow )
		{
			MAPDATA*	pPrev = pNow;
			pNow = pNow->pNextData;
			delete pPrev;
		}
	}
}

// -------------------------------------------------------
// �֐���:	CHrnWndMap::AddHrnWndObject()
// �@�\:	CHrnWnd �I�u�W�F�N�g�ƃE�B���h�E�n���h�������т���
// ����:	HWND		hWnd	�E�B���h�E�n���h�� (IN)
//		CHrnWnd*	pWnd	CHrnWnd �I�u�W�F�N�g (IN)
// �߂�l:	TRUE	����
//		FALSE	���s
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
BOOL CHrnWndMap::AddHrnWndObject( HWND hWnd, CHrnWnd* pWnd )
{
	// �V�����}�b�v�f�[�^�𐶐�
	MAPDATA*	pNewData = newMapData( hWnd, pWnd );
	if ( NULL == pNewData )
	{
		return FALSE;
	}

	// �}�b�v�f�[�^������n�b�V���e�[�u���̏ꏊ�����߂�
	int	nHashIdx = hashWndHandle( hWnd );

	// �����ꏊ�����܂�����A�����փ}�b�v�f�[�^��}��
	pNewData->pNextData = m_mapHash[ nHashIdx ];
	m_mapHash[ nHashIdx ] = pNewData;

	return TRUE;
}

// -------------------------------------------------------
// �֐���:	CHrnWndMap::RemoveHrnWndObject()
// �@�\:	�E�B���h�E�n���h���Ƀ}�b�s���O���ꂽ CHrnWnd �I�u�W�F�N�g��؂藣��
// ����:	HWND	hWnd	�E�B���h�E�n���h�� (IN)
// �߂�l:	NULL	���Ƃ��ƃ}�b�s���O����Ă��Ȃ�
//		!NULL	�}�b�s���O����Ă��� CHrnWnd �I�u�W�F�N�g
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
CHrnWnd* CHrnWndMap::RemoveHrnWndObject( HWND hWnd )
{
	// �}�b�v�f�[�^�̑��݂���n�b�V���e�[�u���̏ꏊ�����߂�
	int	nHashIdx = hashWndHandle( hWnd );

	// ���̃e�[�u��������`���X�g�����ǂ�
	MAPDATA*	pPrev = NULL;
	MAPDATA*	pNow = m_mapHash[ nHashIdx ];
	while( NULL != pNow )
	{
		if ( hWnd == pNow->hWnd )
		{
			// ������!

			if ( NULL != pPrev )
			{
				// pNow �͐擪�ł͂Ȃ��Ƃ���
				pPrev->pNextData = pNow->pNextData;
			}
			else
			{
				// pNow �͐擪
				m_mapHash[ nHashIdx ] = NULL;
			}
			
			CHrnWnd* pWndRet = pNow->pWnd;
			deleteMapData( pNow );
			return pWndRet;
		}
		pPrev = pNow;
		pNow = pNow->pNextData;
	}

	return NULL;
}

// -------------------------------------------------------
// �֐���:	CHrnWndMap::GetHrnWndObject()
// �@�\:	�E�B���h�E�n���h���Ƀ}�b�s���O����Ă��� CHrnWnd �I�u�W�F�N�g��Ԃ�
// ����:	HWND	hWnd	�E�B���h�E�n���h�� (IN)
// �߂�l:	NULL	�}�b�s���O����Ă��Ȃ�
//		!NULL	�}�b�s���O����Ă��� CHrnWnd �I�u�W�F�N�g
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
CHrnWnd* CHrnWndMap::GetHrnWndObject( HWND hWnd )
{
	// �}�b�v�f�[�^�̑��݂���n�b�V���e�[�u���̏ꏊ�����߂�
	int	nHashIdx = hashWndHandle( hWnd );

	// ���̃e�[�u��������`���X�g�����ǂ�
	for( MAPDATA* pNow=m_mapHash[ nHashIdx ]; NULL!=pNow; pNow=pNow->pNextData )
	{
		if ( hWnd == pNow->hWnd )
		{
			// ������!
			return pNow->pWnd;
		}
	}

	return NULL;
}

// -------------------------------------------------------
// �֐���:	CHrnWndMap::newMapData()
// �@�\:	�V�����}�b�v�f�[�^�𐶐�����B
// ����:	HWND		hWnd	�E�B���h�E�n���h�� (IN)
//		CHrnWnd*	pWnd	CHrnWnd �I�u�W�F�N�g (IN)
// �߂�l:	�V�����}�b�v�f�[�^ (NULL �Ȃ玸�s)
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
CHrnWndMap::MAPDATA* CHrnWndMap::newMapData( HWND hWnd, CHrnWnd* pWnd )
{
	MAPDATA*	pData = new MAPDATA;

	if ( NULL != pData )
	{
		pData->hWnd = hWnd;
		pData->pWnd = pWnd;
		pData->pNextData = NULL;
	}

	return pData;
}

// -------------------------------------------------------
// �֐���:	CHrnWndMap::deleteMapData()
// �@�\:	�}�b�v�f�[�^���폜����
// ����:	MAPDATA* pMapData 
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
void CHrnWndMap::deleteMapData( MAPDATA* pMapData )
{
	delete pMapData;
}


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

// -------------------------------------------------------
// �֐���:	CHrnWnd::CHrnWnd()
// �@�\:	�R���X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
CHrnWnd::CHrnWnd()
{
	m_hWnd = NULL;
	m_orgWndProc = NULL;
}

// -------------------------------------------------------
// �֐���:	CHrnWnd::~CHrnWnd()
// �@�\:	�f�X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
CHrnWnd::~CHrnWnd()
{
	// �܂��E�B���h�E�������Ă���Ȃ�����B
	if ( NULL != m_hWnd )
	{
#ifdef _DEBUG
		::OutputDebugString( _T("CHrnWnd::~CHrnWnd() �ɂ����āA�E�B���h�E���폜����Ă��Ȃ����Ƃ����o���܂����B\n") );
#endif
		m_gHrnWndMap.RemoveHrnWndObject( m_hWnd );
		if ( ::IsWindow( m_hWnd ) )
		{			
#ifdef _DEBUG
			::OutputDebugString( _T("        �� ::DestroyWindow() ���Ăяo���܂��B\n") );
#endif
			::DestroyWindow( m_hWnd );
		}
	}
}

// -------------------------------------------------------
// �֐���:	CHrnWnd::Create()
// �@�\:	�E�B���h�E���쐬����B
// ����:	LPCTSTR		lpClassName	�E�B���h�E�N���X�� (IN)
//		LPCTSTR		lpWindowName	�E�B���h�E�� (IN)
//		DWORD		dwStyle		�E�B���h�E�X�^�C�� (IN)
//		int		x		�E�B���h�E�̏���x���W (IN)
//		int		y		�E�B���h�E�̏���y���W (IN)
//		int		nWidth		�E�B���h�E�̕�( IN)
//		int		nHeight		�E�B���h�E�̍��� (IN)
//		HWND		hWndParent	�e�E�B���h�E (IN)
//		HMENU		hMenu		���j���[�܂��͎q�E�B���h�E��ID (IN)
//		HINSTANCE	hInstance	�C���X�^���X�n���h�� (IN)
// �߂�l:	
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
BOOL CHrnWnd::Create( LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance )
{
	if ( NULL == ::CreateWindow( lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, (LPVOID)this ) )
	{
#ifdef _DEBUG
		DWORD dwError = ::GetLastError();
		_RPT1( _CRT_WARN, "at CHrnWnd::Create() : CreateWindow returns FALSE (last error = %u)\n", dwError );
#endif
		return FALSE;
	}

	return TRUE;
}

// -------------------------------------------------------
// �֐���:	CHrnWnd::CreateEx()
// �@�\:	�g���X�^�C���t���ŃE�B���h�E���쐬����B
// ����:	DWORD		dwExStyle	�g���X�^�C�� (IN)
//		LPCTSTR		lpClassName	�E�B���h�E�N���X�� (IN)
//		LPCTSTR		lpWindowName	�E�B���h�E�� (IN)
//		DWORD		dwStyle		�E�B���h�E�X�^�C�� (IN)
//		int		x		�E�B���h�E�̏���x���W (IN)
//		int		y		�E�B���h�E�̏���y���W (IN)
//		int		nWidth		�E�B���h�E�̕� (IN)
//		int		nHeight		�E�B���h�E�̍��� (IN)
//		HWND		hWndParent	�e�E�B���h�E (IN)
//		HMENU		hMenu		���j���[�܂��͎q�E�B���h�E��ID (IN)
//		HINSTANCE	hInstance	�C���X�^���X�n���h�� (IN)
// �߂�l:	
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	
// -------------------------------------------------------
BOOL CHrnWnd::CreateEx( DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance )
{
	if ( NULL == ::CreateWindowEx( dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, (LPVOID)this ) )
	{
		return FALSE;
	}

	return TRUE;
}

// -------------------------------------------------------
// �֐���:	RegisterClass()
// �@�\:	�E�B���h�E�N���X�̓o�^
// ����:	UINT		style		�N���X�X�^�C�� (IN)
//		int		cbClsExtra	�N���X�o�C�g�̃T�C�Y (IN)
//		int		cbWndExtra	�E�B���h�E�o�C�g�̃T�C�Y (IN)
//		HINSTANCE	hInstance	�C���X�^���X�n���h�� (IN)
//		HICON		hIcon		�N���X�A�C�R�� (IN)
//		HCURSOR		hCursor		�N���X�J�[�\�� (IN)
//		HBRUSH		hbrBackground	�N���X�w�i�u���V (IN)
//		LPCTSTR		lpszMenuName	�N���X���j���[ (IN)
//		LPCTSTR		lpszClassName	�N���X�� (IN)
//		HIRON		hIconSm		�X���[���A�C�R�� (IN)
// �߂�l:	
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	CHrnWnd����h������E�B���h�E���쐬����ۂ́A
//		�K�����̊֐��œo�^�����E�B���h�E�N���X��
//		�g��Ȃ���΂Ȃ�Ȃ��B
// -------------------------------------------------------
ATOM CHrnWnd::RegisterClass( UINT style, int cbClsExtra, int cbWndExtra, HINSTANCE hInstance, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm )
{
	WNDCLASSEX	wc;

	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = style;
	wc.lpfnWndProc = CHrnWnd::cbHrnWndProc;
	wc.cbClsExtra = cbClsExtra;
	wc.cbWndExtra = cbWndExtra;
	wc.hInstance = hInstance;
	wc.hIcon = hIcon;
	wc.hCursor = hCursor;
	wc.hbrBackground = hbrBackground;
	wc.lpszMenuName = lpszMenuName;
	wc.lpszClassName = lpszClassName;
	wc.hIconSm = hIconSm;

	return ::RegisterClassEx( &wc );
}

// -------------------------------------------------------
// �֐���:	CHrnWnd::Subclass()
// �@�\:	�E�B���h�E���T�u�N���X�������т���
// ����:	HWND	hWnd	�T�u�N���X������E�B���h�E�̃n���h�� (IN)
// �߂�l:	BOOL	TRUE:���� / FALSE:���s
// �쐬:	Hironori Ichimiya / 2000.5.25
// ���L����:	
// -------------------------------------------------------
BOOL CHrnWnd::Subclass( HWND hWnd )
{
	if ( NULL != m_orgWndProc )
	{
		return FALSE;
	}

	m_orgWndProc = SubclassWindow( hWnd, cbHrnWndProc );
	if ( NULL != m_orgWndProc )
	{
		if ( !m_gHrnWndMap.AddHrnWndObject( hWnd, this ) )
		{
			// ���ѕt�����s!!
#ifdef _DEBUG
			::OutputDebugString( _T("CHrnSbclsWnd::SubclassWindow() �E�B���h�E�n���h���ƃI�u�W�F�N�g�̌��т��Ɏ��s���܂���!!\n") );
#endif
			SubclassWindow( hWnd, m_orgWndProc );
			return FALSE;
		}
		// �I�u�W�F�N�g�ɃE�B���h�E�n���h�����������Ă��
		m_hWnd = hWnd;
	}

	return TRUE;
}

// -------------------------------------------------------
// �֐���:	CHrnWnd::UnSubclass()
// �@�\:	�T�u�N���X������������B
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.5.25
// ���L����:	
// -------------------------------------------------------
void CHrnWnd::UnSubclass()
{
	if ( NULL == m_orgWndProc )
	{
		return;
	}

	// �E�B���h�E�n���h���ƃI�u�W�F�N�g��؂藣��
	m_gHrnWndMap.RemoveHrnWndObject( m_hWnd );
	SubclassWindow( m_hWnd, m_orgWndProc );
	m_orgWndProc = NULL;
	m_hWnd = NULL;
}

// -------------------------------------------------------
// �֐���:	CHrnWnd::cbHrnWndProc()
// �@�\:	(�B���ꂽ)�E�B���h�E�v���V�[�W��
// ����:	HWND	hWnd	�E�B���h�E�n���h�� (IN)
//		UINT	uMsg	���b�Z�[�W (IN)
//		WPARAM	wParam	WPARAM (IN)
//		LPARAM	lParam	LPARAM (IN)
// �߂�l:	LRESULT	��������
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	������ CHrnWnd �I�u�W�F�N�g�� wndProc() ��
//		�Ă�ł��܂��B
// -------------------------------------------------------
LRESULT CALLBACK CHrnWnd::cbHrnWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// �e�E�B���h�E���擾 (WM_NCCREATE ������܂ł͖���)
	CHrnWnd*	pWnd = NULL;
	LRESULT		lResult = 0;

	switch( uMsg )
	{
	case WM_NCCREATE:
		// �E�B���h�E�n���h���ƃI�u�W�F�N�g�����т���
		pWnd = (CHrnWnd*)( ((LPCREATESTRUCT)lParam)->lpCreateParams );
		if ( NULL != pWnd )
		{
			if ( !m_gHrnWndMap.AddHrnWndObject( hWnd, pWnd ) )
			{
				// ���ѕt�����s!!
#ifdef _DEBUG
				::OutputDebugString( _T("�E�B���h�E�n���h���ƃI�u�W�F�N�g�̌��т��Ɏ��s���܂���!!\n") );
#endif
				return FALSE;
			}
			// �I�u�W�F�N�g�ɃE�B���h�E�n���h�����������Ă��
			pWnd->m_hWnd = hWnd;
		}
		break;

	case WM_NCDESTROY:
		// �E�B���h�E�n���h���ƃI�u�W�F�N�g��؂藣��
		pWnd = m_gHrnWndMap.RemoveHrnWndObject( hWnd );
		break;

	default:
		// �Y���I�u�W�F�N�g���擾
		pWnd = m_gHrnWndMap.GetHrnWndObject( hWnd );
	}

	// �E�B���h�E�v���V�[�W�����R�[��
	if ( NULL != pWnd )
	{
		try
		{
			lResult = pWnd->wndProc( hWnd, uMsg, wParam, lParam );
		}
		catch( CHrnException* e )
		{
			if ( IDCANCEL == ::MessageBox( NULL, e->GetErrorMessage(), "", MB_OKCANCEL | MB_ICONSTOP ) )
			{
				::PostQuitMessage( 0 );
			}
		}
	}
	else
	{	
		// �E�B���h�E�I�u�W�F�N�g�����݂��Ȃ�!!
		lResult = ::DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	// �폜�̂Ƃ��̓E�B���h�E�n���h������ɂ���
	if ( WM_NCDESTROY == uMsg && NULL != pWnd )
	{
		pWnd->m_hWnd = NULL;
	}

	return lResult;
}

// -------------------------------------------------------
// �֐���:	CHrnWnd::wndProc()
// �@�\:	�E�B���h�E�v���V�[�W��
// ����:	HWND	hWnd	�E�B���h�E�n���h�� (IN)
//		UINT	uMsg	���b�Z�[�W (IN)
//		WPARAM	wParam	WPARAM (IN)
//		LPARAM	lParam	LPARAM (IN)
// �߂�l:	LRESULT	��������
// �쐬:	Hironori Ichimiya / 2000.4.13
// ���L����:	�h���N���X�ł͂��̊֐����I�[�o�[���C�h���邱�Ƃɂ��
//		�C�x���g���n���h������B
// -------------------------------------------------------
LRESULT CHrnWnd::wndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// WM_NOTIFY �̏ꍇ�́A���������ɂ����b�Z�[�W����������`�����X��^����B
	if ( WM_NOTIFY == uMsg )
	{
		CHrnWnd*	pWnd;
		pWnd = m_gHrnWndMap.GetHrnWndObject( ((LPNMHDR)lParam)->hwndFrom );
		if ( NULL != pWnd )
		{
			LRESULT	result;
			if ( pWnd->OnRefrectNotify( hWnd, uMsg, wParam, lParam, &result ) )
			{
				return result;
			}
		}
	}

	if ( NULL != m_orgWndProc )
	{
		return ::CallWindowProc( m_orgWndProc, hWnd, uMsg, wParam, lParam );
	}
	else
	{
		return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
}

// -------------------------------------------------------
// �֐���:	CHrnWnd::OnRefrectNotify()
// �@�\:	WM_NOTIFY �̑��葤�ւ̔���
// ����:	HWND		hWnd		��������̃E�B���h�E (IN)
//		UINT		uMsg		(IN)
//		WPARAM		wParam		WPARAM (IN)
//		LPARAM		lParam		LPARAM (IN)
//		LRESULT*	pResult		WM_NOTIFY �ւ̖߂�l (OUT)
// �߂�l:	BOOL	TRUE:�������� / FALSE:�������Ȃ�����
// �쐬:	Hironori Ichimiya / 2000.5.25
// ���L����:	�߂�l�� FALSE �̏ꍇ�� pResult �͖��������B
// -------------------------------------------------------
BOOL CHrnWnd::OnRefrectNotify( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	UNREFERENCED_PARAMETER( hWnd );
	UNREFERENCED_PARAMETER( uMsg );
	UNREFERENCED_PARAMETER( wParam );
	UNREFERENCED_PARAMETER( lParam );
	UNREFERENCED_PARAMETER( pResult );
	return FALSE;
}

