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

/*!
	@file		WinKeyMappingManager.h
	@brief		WinKeyMappingManager �N���X�̒�`
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.24 created
*/

#ifndef _WINKEYMAPPINGMANAGER_H_
#define _WINKEYMAPPINGMANAGER_H_

#include "KeyEventParameter.h"

// ---------------------------------------------------------------------
//! �L�[�}�b�s���O�Ǘ��N���X for Windows
// ---------------------------------------------------------------------
class WinKeyMappingManager
{
public:
						WinKeyMappingManager();
	virtual				~WinKeyMappingManager();
	
	virtual SInt32		GetFunction(const KeyEventParameter& parameter) = 0;
};

// TODO: ���ɏ����Ă���Ƃ���B�L�[�J�X�^�}�C�Y����������Ƃ��ɂ͂��̃N���X���Ȃ����B
// ---------------------------------------------------------------------
//! ���C���E�B���h�E�p�L�[�}�b�s���O�Ǘ��N���X for Windows
/*!
	@note
	�����̃L�[�}�b�s���O�̃J�X�^�}�C�Y�Ή����ł���悤�ɂȂ�΁A
	WinKeyMappingManager ���L�[�Ƌ@�\�̓��I�ȑΉ��֌W���Ǘ�����͂��B
	�����Ȃ�ƁAGetFunction �� virtual �̕K�v���Ȃ��Ȃ�A
	���C���E�B���h�E�Ɋւ�炸�A�e�E�B���h�E�p�̑Ή��֌W�̊Ǘ���
	WinKeyMappingManager ���s����悤�ɂȂ�B�i�E�B���h�E 1 �ɑ΂��A
	WinKeyMappingManager �̃C���X�^���X�� 1 ��邪�A������ WinKeyMappingManager
	�̔h���N���X�ł͂Ȃ��AWinKeyMappingManager ���̂��̂ł悢�͂��j
	���̂Ƃ��A���̃N���X�͕K�v�Ȃ��Ȃ邾�낤�B
	���݂͓��I�ȑΉ��֌W�̊Ǘ����������Ă��Ȃ��̂�
	����܂ł̂Ȃ��Ƃ��č���Ă���N���X�B
*/
// ---------------------------------------------------------------------
class WinMainWindowKeyMappingManager : public WinKeyMappingManager
{
public:
						WinMainWindowKeyMappingManager() { }
	virtual				~WinMainWindowKeyMappingManager() { }

	virtual SInt32		GetFunction(const KeyEventParameter& parameter);
};

#endif // _WINKEYMAPPINGMANAGER_H_
