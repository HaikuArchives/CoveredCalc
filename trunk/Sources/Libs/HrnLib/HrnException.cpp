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

// HrnException.cpp - CHrnException
// ��O�N���X

#include "Prefix.h"
#include "HrnException.h"

static TCHAR _NoMessageToReport_[] = _T("�����G���[���������܂����B");
static TCHAR _OutOfMemory_[] = _T("���������s�����Ă��܂��B");

static CHrnMemoryException _TheUniqueMemoryException_;

///////////////////////////////////////////////////////////////////////////// CHrnException
// -------------------------------------------------------
// �֐���:	CHrnException()
// �@�\:	�R���X�g���N�^
// ����:	bAutoDelete	TRUE�ɂ����CHrnException::Delete()�ŃI�u�W�F�N�g�̍폜���s����
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 1999.5.3
// ���L����:	
// -------------------------------------------------------
CHrnException::CHrnException( BOOL bAutoDelete )
{
	m_bAutoDelete = bAutoDelete;
}

// -------------------------------------------------------
// �֐���:	~CHrnException()
// �@�\:	�f�X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 1999.5.3
// ���L����:	
// -------------------------------------------------------
CHrnException::~CHrnException()
{
}

// -------------------------------------------------------
// �֐���:	GetErrorMessage()
// �@�\:	�G���[���b�Z�[�W���擾����
// ����:	(�Ȃ�)
// �߂�l:	�G���[���b�Z�[�W�ւ̃|�C���^(�ꎞ�I�Ȃ���)
// �쐬:	Hironori Ichimiya / 1999.5.3
// ���L����:	
// -------------------------------------------------------
LPCTSTR CHrnException::GetErrorMessage()
{
	return _NoMessageToReport_;
}

// -------------------------------------------------------
// �֐���:	Delete()
// �@�\:	��O�I�u�W�F�N�g�̏������s���B
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 1999.5.3
// ���L����:	
// -------------------------------------------------------
void CHrnException::Delete()
{
	if ( m_bAutoDelete )
	{
		delete this;
	}
}

///////////////////////////////////////////////////////////////////////////// CHrnMemoryException
// -------------------------------------------------------
// �֐���:	CHrnMemoryException()
// �@�\:	�R���X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 1999.5.3
// ���L����:	
// -------------------------------------------------------
CHrnMemoryException::CHrnMemoryException() : CHrnException( FALSE )
{
}

// -------------------------------------------------------
// �֐���:	~CHrnMemoryException()
// �@�\:	�f�X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 1999.5.3
// ���L����:	
// -------------------------------------------------------
CHrnMemoryException::~CHrnMemoryException()
{
}

// -------------------------------------------------------
// �֐���:	GetErrorMessage()
// �@�\:	�G���[���b�Z�[�W���擾����
// ����:	(�Ȃ�)
// �߂�l:	�G���[���b�Z�[�W�ւ̃|�C���^(�ꎞ�I�Ȃ���)
// �쐬:	Hironori Ichimiya / 1999.5.3
// ���L����:	
// -------------------------------------------------------
LPCTSTR CHrnMemoryException::GetErrorMessage()
{
	return _OutOfMemory_;
}

// -------------------------------------------------------
// �֐���:	ThrowHrnMemoryException()
// �@�\:	CHrnMemoryException��O(�ւ̃|�C���^)���X���[����B
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 1999.5.3
// ���L����:	
// -------------------------------------------------------
void ThrowHrnMemoryException()
{
	throw &_TheUniqueMemoryException_;
}
