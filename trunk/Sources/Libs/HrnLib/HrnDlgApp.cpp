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

// HrnDlgApp.cpp

#include "Prefix.h"
#include "HrnApp.h"
#include "HrnDlg.h"

// -------------------------------------------------------
// �֐���:	CHrnDlg::DoModal()
// �@�\:	���[�_���_�C�A���O�̐���
// ����:	HWND	hParent	�e�E�B���h�E�̃n���h�� (IN)
// �߂�l:	EndDialog �̖߂�l
// �쐬:	Hironori Ichimiya / 2000.4.16
// ���L����:	CHrnAppBase�I�u�W�F�N�g����C���X�^���X�n���h���𓾂�B
// -------------------------------------------------------
int CHrnDlg::DoModal( HWND hParent )
{
	return DoModal( hParent, CHrnAppBase::GetAppObject()->GetInstanceHandle() );
}

// -------------------------------------------------------
// �֐���:	CHrnDlg::Create()
// �@�\:	���[�h���X�_�C�A���O�̐���
// ����:	HWND	hParent	�e�E�B���h�E�̃n���h�� (IN)
// �߂�l:	TRUE: ���� / FALSE: ���s
// �쐬:	Hironori Ichimiya / 2004.3.10
// ���L����:	CHrnAppBase�I�u�W�F�N�g����C���X�^���X�n���h���𓾂�B
// -------------------------------------------------------
BOOL CHrnDlg::Create( HWND hParent )
{
	return Create( hParent, CHrnAppBase::GetAppObject()->GetInstanceHandle() );
}
