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

// HrnWndApp.cpp

#include "Prefix.h"
#include "HrnApp.h"
#include "HrnWnd.h"

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
// �߂�l:	
// �쐬:	Hironori Ichimiya / 2000.11.19
// ���L����:	
// -------------------------------------------------------
BOOL CHrnWnd::Create( LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu )
{
	return Create( lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, CHrnAppBase::GetAppObject()->GetInstanceHandle() );
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
// �߂�l:	
// �쐬:	Hironori Ichimiya / 2000.11.19
// ���L����:	
// -------------------------------------------------------
BOOL CHrnWnd::CreateEx( DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu )
{
	return CreateEx( dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, CHrnAppBase::GetAppObject()->GetInstanceHandle() );
}

