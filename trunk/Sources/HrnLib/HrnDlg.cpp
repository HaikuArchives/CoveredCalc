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

// HrnDlg.cpp: CHrnDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "Prefix.h"
#include <windowsx.h>
#include <tchar.h>
#include "HrnException.h"
#include "HrnDlg.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

// -------------------------------------------------------
// �֐���:	CHrnDlg::CHrnDlg()
// �@�\:	�e���v���[�g������ɂ��R���X�g���N�^
// ����:	LPCTSTR		szTemplate	���\�[�X�e���v���[�g������ (IN)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.16
// ���L����:	
// -------------------------------------------------------
CHrnDlg::CHrnDlg( LPCTSTR szTemplate )
{
	m_szTemplate = szTemplate;
	m_bModal = TRUE;
}

// -------------------------------------------------------
// �֐���:	CHrnDlg::CHrnDlg()
// �@�\:	�e���v���[�gID�ɂ��R���X�g���N�^
// ����:	UINT		uTemplateID	���\�[�X�e���v���[�gID (IN)
//		HINSTANCE	hInstance	�C���X�^���X�n���h�� (IN)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.16
// ���L����:	
// -------------------------------------------------------
CHrnDlg::CHrnDlg( UINT uTemplateID )
{
	m_szTemplate = MAKEINTRESOURCE( uTemplateID );
	m_bModal = TRUE;
}

// -------------------------------------------------------
// �֐���:	CHrnDlg::~CHrnDlg()
// �@�\:	�f�X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.16
// ���L����:	
// -------------------------------------------------------
CHrnDlg::~CHrnDlg()
{

}

// -------------------------------------------------------
// �֐���:	CHrnDlg::DoModal()
// �@�\:	���[�_���_�C�A���O�̐���
// ����:	HWND	hParent	�e�E�B���h�E�̃n���h�� (IN)
// �߂�l:	EndDialog �̖߂�l
// �쐬:	Hironori Ichimiya / 2000.4.16
// ���L����:	
// -------------------------------------------------------
int CHrnDlg::DoModal( HWND hParent, HINSTANCE hInstance )
{
	m_bModal = TRUE;
	return (int)::DialogBoxParam( hInstance, m_szTemplate, hParent, CHrnDlg::cbHrnDlgProc, (LPARAM)this );
}

// -------------------------------------------------------
// �֐���:	CHrnDlg::Create()
// �@�\:	���[�h���X�_�C�A���O�̐���
// ����:	HWND	hParent	�e�E�B���h�E�̃n���h�� (IN)
// �߂�l:	TRUE: ���� / FALSE: ���s
// �쐬:	Hironori Ichimiya / 2004.3.10
// ���L����:
// -------------------------------------------------------
BOOL CHrnDlg::Create( HWND hParent, HINSTANCE hInstance )
{
	m_bModal = FALSE;
	HWND hWnd = ::CreateDialogParam( hInstance, m_szTemplate, hParent, CHrnDlg::cbHrnDlgProc, (LPARAM)this );
	if (NULL != hWnd)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// -------------------------------------------------------
// �֐���:	CHrnDlg::cbHrnDlgProc()
// �@�\:	(�B���ꂽ)�_�C�A���O�v���V�[�W��
// ����:	HWND	hwndDlg	�E�B���h�E�n���h�� (IN)
//		UINT	uMsg	���b�Z�[�W (IN)
//		WPARAM	wParam	WPARAM (IN)
//		LPARAM	lParam	LPARAM (IN)
// �߂�l:	LRESULT	��������
// �쐬:	Hironori Ichimiya / 2000.4.16
// ���L����:	������ CHrnDlg �I�u�W�F�N�g�� wndProc() ��
//		�Ă�ł��܂��B
// -------------------------------------------------------
INT_PTR CALLBACK CHrnDlg::cbHrnDlgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CHrnDlg*	pDlg = NULL;
	LRESULT		lResult = FALSE;

	switch( uMsg )
	{
	case WM_INITDIALOG:
		// �E�B���h�E�n���h���ƃI�u�W�F�N�g�����т���
		pDlg = (CHrnDlg*)lParam;
		if ( NULL != pDlg )
		{
			if ( !m_gHrnWndMap.AddHrnWndObject( hwndDlg, pDlg ) )
			{
				// ���ѕt�����s!!
#ifdef _DEBUG
				::OutputDebugString( _T("�E�B���h�E�n���h���ƃI�u�W�F�N�g�̌��т��Ɏ��s���܂���!!\n") );
#endif
				if ( pDlg->m_bModal )
				{
					::EndDialog( hwndDlg, IDCANCEL );
				}
				else
				{
					::DestroyWindow( hwndDlg );
				}
				return FALSE;
			}
			// �I�u�W�F�N�g�ɃE�B���h�E�n���h�����������Ă��
			pDlg->m_hWnd = hwndDlg;
		}
		break;
	case WM_NCDESTROY:
		// �E�B���h�E�n���h���ƃI�u�W�F�N�g��؂藣��
		pDlg = (CHrnDlg*)m_gHrnWndMap.RemoveHrnWndObject( hwndDlg );	// �_�E���L���X�g���Ă܂� :P
		break;

	default:
		// �Y���I�u�W�F�N�g���擾
		pDlg = (CHrnDlg*)m_gHrnWndMap.GetHrnWndObject( hwndDlg );	// �_�E���L���X�g���Ă܂� :P
	}

	// �E�B���h�E�v���V�[�W�����R�[��
	if ( NULL != pDlg )
	{
		try
		{
			lResult = pDlg->wndProc( hwndDlg, uMsg, wParam, lParam );
		}
		catch( CHrnException* e )
		{
			if ( IDCANCEL == ::MessageBox( NULL, e->GetErrorMessage(), "", MB_OKCANCEL | MB_ICONSTOP ) )
			{
				::PostQuitMessage( 0 );
			}
		}
	}

	// �폜�̂Ƃ��̓E�B���h�E�n���h������ɂ���
	if ( WM_NCDESTROY == uMsg && NULL != pDlg )
	{
		pDlg->m_hWnd = NULL;
	}

	return (INT_PTR)lResult;
}

// -------------------------------------------------------
// �֐���:	CHrnDlg::wndProc()
// �@�\:	�E�B���h�E�v���V�[�W��
// ����:	HWND	hWnd	�E�B���h�E�n���h�� (IN)
//		UINT	uMsg	���b�Z�[�W (IN)
//		WPARAM	wParam	WPARAM (IN)
//		LPARAM	lParam	LPARAM (IN)
// �߂�l:	LRESULT	��������
// �쐬:	Hironori Ichimiya / 2000.4.16
// ���L����:	�h���N���X�ł͂��̊֐����I�[�o�[���C�h�ɂ��
//		�C�x���g���n���h������B
// -------------------------------------------------------
LRESULT CHrnDlg::wndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
				return SetDlgMsgResult( hWnd, uMsg, result );
			}
		}
	}

	// �f�t�H���g�Őe�E�B���h�E�̒����ɔz�u���܂��B
	if ( WM_INITDIALOG == uMsg )
	{
		HWND	parentH;
		parentH = ::GetParent( hWnd );
		RECT	parentRect;
		if ( NULL == parentH )
		{	// �g�b�v���x���̃_�C�A���O
			::SystemParametersInfo( SPI_GETWORKAREA, 0, &parentRect, 0 );
		}
		else
		{
			::GetWindowRect( parentH, &parentRect );
		}

		RECT	myRect;

		::GetWindowRect( hWnd, &myRect );

		::MoveWindow( hWnd,
				( parentRect.right - parentRect.left ) / 2 - ( myRect.right - myRect.left ) / 2 + parentRect.left,
				( parentRect.bottom - parentRect.top ) / 2 - ( myRect.bottom - myRect.top ) / 2 + parentRect.top,
				( myRect.right - myRect.left ), ( myRect.bottom - myRect.top ), FALSE );
		return FALSE;
	}

	// �f�t�H���g�� IDCANCEL, IDOK �� EndDialog ���Ăяo���܂��B
	if ( m_bModal )
	{
		if ( WM_COMMAND == uMsg )
		{
			WORD	wId = LOWORD( wParam );
			if ( IDCANCEL == wId || IDOK == wId )
			{
				::EndDialog( hWnd, (int)wId );
				return TRUE;
			}
		}
	}

	// �������Ȃ��������b�Z�[�W�� FALSE ��Ԃ��B
	return FALSE;
}
