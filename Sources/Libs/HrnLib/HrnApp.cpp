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

// HrnApp.cpp: CHrnApp �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "Prefix.h"
#include <crtdbg.h>
#include "HrnApp.h"

CHrnAppBase* CHrnAppBase::m_gOnlyOneAppP = NULL;		// 1�����Ȃ� CHrnAppBase �I�u�W�F�N�g

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

// CHrnAppBase
////////////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------
// �֐���:	CHrnAppBase::CHrnAppBase()
// �@�\:	�R���X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
// -------------------------------------------------------
CHrnAppBase::CHrnAppBase()
{
	m_hInstance = NULL;
}

// -------------------------------------------------------
// �֐���:	CHrnAppBase::~CHrnAppBase()
// �@�\:	�f�X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
// -------------------------------------------------------
CHrnAppBase::~CHrnAppBase()
{
}

// CHrnApp
////////////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------
// �֐���:	CHrnApp::CHrnApp()
// �@�\:	�R���X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.22
// ���L����:	
// -------------------------------------------------------
CHrnApp::CHrnApp()
{
	m_hPrevInstance = NULL;
	m_lpCmdLine = NULL;
	m_nCmdShow = SW_SHOWDEFAULT;

}

// -------------------------------------------------------
// �֐���:	CHrnApp::~CHrnApp()
// �@�\:	�f�X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.22
// ���L����:	
// -------------------------------------------------------
CHrnApp::~CHrnApp()
{

}

// -------------------------------------------------------
// �֐���:	CHrnApp::WinMain()
// �@�\:	�v���O�����̃G���g���|�C���g
// ����:	
// �߂�l:	
// �쐬:	Hironori Ichimiya / 2000.4.22
// ���L����:	������Ăׂ�̂̓v���O������1�񂾂��B
// -------------------------------------------------------
int CHrnApp::WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	int	retValue = 0;

	// 1�񂵂��Ă�ł͂����Ȃ��B
	if ( NULL != m_gOnlyOneAppP )
	{
		_ASSERT( FALSE );
		return 0;
	}

	// �B��� �L����CHrnAppBase�I�u�W�F�N�g�͎������g
	m_gOnlyOneAppP = this;

	// �p�����[�^��ۑ����Ă���
	m_hInstance = hInstance;
	m_hPrevInstance = hPrevInstance;
	m_lpCmdLine = lpCmdLine;
	m_nCmdShow = nCmdShow;

	// �C���X�^���X������
	if ( initInstance() )
	{
		// ���b�Z�[�W���[�v
		retValue = messageLoop();
	}

	// �C���X�^���X�I������
	exitInstance();

	return retValue;
}

// -------------------------------------------------------
// �֐���:	CHrnApp::initInstance()
// �@�\:	�C���X�^���X������
// ����:	(�Ȃ�)
// �߂�l:	TRUE: ���� / FALSE: ���s
// �쐬:	Hironori Ichimiya / 2000.4.22
// ���L����:	
// -------------------------------------------------------
BOOL CHrnApp::initInstance()
{
	return TRUE;
}

// -------------------------------------------------------
// �֐���:	CHrnApp::exitInstance()
// �@�\:	�C���X�^���X�I������
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.4.22
// ���L����:	
// -------------------------------------------------------
void CHrnApp::exitInstance()
{
}



// -------------------------------------------------------
// �֐���:	CHrnApp::messageLoop()
// �@�\:	���b�Z�[�W���[�v
// ����:	(�Ȃ�)
// �߂�l:	�A�v���P�[�V�����̖߂�l
// �쐬:	Hironori Ichimiya / 2000.4.22
// ���L����:	�L�[�{�[�h�A�N�Z�����[�^���g������A���[�h���X�_�C�A���O��
//		�g�����肷��ꍇ�̓I�[�o�[���C�h���āA
//		TranslateAccelerator �� IsDialogMessage ���g���B
// -------------------------------------------------------
int CHrnApp::messageLoop()
{
	MSG	msg;
	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return msg.wParam;
}

// CHrnDll
////////////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------
// �֐���:	CHrnDll::CHrnDll()
// �@�\:	�R���X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
// -------------------------------------------------------
CHrnDll::CHrnDll()
{
}

// -------------------------------------------------------
// �֐���:	CHrnDll::~CHrnDll()
// �@�\:	�f�X�g���N�^
// ����:	(�Ȃ�)
// �߂�l:	(�Ȃ�)
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
// -------------------------------------------------------
CHrnDll::~CHrnDll()
{
}

// -------------------------------------------------------
// �֐���:	CHrnDll::_dllMain()
// �@�\:	DllMain()
// ����:	
// �߂�l:	
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
// -------------------------------------------------------
BOOL CHrnDll::_dllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch( fdwReason )
	{
	case DLL_PROCESS_ATTACH:
		// 1�񂵂��Ă΂�Ȃ��͂��B
		if ( NULL != m_gOnlyOneAppP )
		{
			_ASSERT( FALSE );
			return FALSE;
		}

		// �B��� �L����CHrnAppBase�I�u�W�F�N�g�𐶐�
		m_gOnlyOneAppP = this;

		// �p�����[�^��ۑ����Ă���
		m_hInstance = hinstDLL;

		return onDllProcessAttach( lpvReserved );
		break;

	case DLL_THREAD_ATTACH:
		if ( NULL == m_gOnlyOneAppP )
		{
			_ASSERT( FALSE );
			return FALSE;
		}
		return onDllThreadAttach( lpvReserved );
		break;

	case DLL_THREAD_DETACH:
		if ( NULL == m_gOnlyOneAppP )
		{
			_ASSERT( FALSE );
			return FALSE;
		}
		return onDllThreadDetach( lpvReserved );
		break;

	case DLL_PROCESS_DETACH:
		if ( NULL == m_gOnlyOneAppP )
		{
			_ASSERT( FALSE );
			return FALSE;
		}
		return onDllProcessDetach( lpvReserved );
		break;

	default:
		return FALSE;
	}
}

// -------------------------------------------------------
// �֐���:	CHrnDll::onDllProcessAttach()
// �@�\:	DLL_PROCESS_ATTACH �̂Ƃ��̏���
// ����:	LPVOID lpvReserved
// �߂�l:	BOOL
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
// -------------------------------------------------------
BOOL CHrnDll::onDllProcessAttach( LPVOID lpvReserved )
{
	UNREFERENCED_PARAMETER( lpvReserved );
	return TRUE;
}

// -------------------------------------------------------
// �֐���:	CHrnDll::onDllThreadAttach()
// �@�\:	DLL_Thread_ATTACH �̂Ƃ��̏���
// ����:	LPVOID lpvReserved
// �߂�l:	BOOL
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
// -------------------------------------------------------
BOOL CHrnDll::onDllThreadAttach( LPVOID lpvReserved )
{
	UNREFERENCED_PARAMETER( lpvReserved );
	return TRUE;
}

// -------------------------------------------------------
// �֐���:	CHrnDll::onDllThreadDetach()
// �@�\:	DLL_THREAD_DETACH �̂Ƃ��̏���
// ����:	LPVOID lpvReserved
// �߂�l:	BOOL
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
// -------------------------------------------------------
BOOL CHrnDll::onDllThreadDetach( LPVOID lpvReserved )
{
	UNREFERENCED_PARAMETER( lpvReserved );
	return TRUE;
}

// -------------------------------------------------------
// �֐���:	CHrnDll::onDllProcessDetach()
// �@�\:	DLL_PROCESS_Detach �̂Ƃ��̏���
// ����:	LPVOID lpvReserved
// �߂�l:	BOOL
// �쐬:	Hironori Ichimiya / 2000.5.18
// ���L����:	
// -------------------------------------------------------
BOOL CHrnDll::onDllProcessDetach( LPVOID lpvReserved )
{
	UNREFERENCED_PARAMETER( lpvReserved );
	return TRUE;
}
