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
	@file		WinLangFile.cpp
	@brief		WinLangFile �N���X�̎���
	@author		ICHIMIYA Hironori (Hiron)
	@date		20053.12.18 created
*/

#include "Prefix.h"
#include "WinLangFile.h"
#include "Path.h"
#include "MemoryException.h"
#include "LangFileException.h"

typedef DWORD (APIENTRY *PFNCCNLGETVERSION)();
typedef DWORD (APIENTRY *PFNCCNLGETLANGUAGECODE)(char* szLangCode);
typedef DWORD (APIENTRY *PFNCCNLGETLANGUAGENAME)(char* szLangName);
typedef LANGID (APIENTRY *PFNCCNLGETLANGID)();

#define SPEC_VERSION	(4)		// ����t�@�C���d�l�̃o�[�W����

/**
 *	@brief	�R���X�g���N�^
 */
WinLangFile::WinLangFile()
{
	hInstance = NULL;
}

/**
 *	@brief	�f�X�g���N�^
 */
WinLangFile::~WinLangFile()
{
	Unload();		// �������ĂȂ����
}

/**
 *	@brief	����t�@�C�����t�@�C������ǂݍ��݂܂��B
 */
void WinLangFile::Load(
	const Path& langFilePath	///< ����t�@�C���̃p�X
)
{
	Unload();
	hInstance = LoadLibrary(langFilePath.GetPathString());
	if (NULL == hInstance)
	{
		throw new LangFileExceptions::LoadFailed(langFilePath);
	}
}

/**
 *	@brief	�ǂݍ���ł��錾��t�@�C�����A�����[�h���܂��B
 */
void WinLangFile::Unload()
{
	if (NULL != hInstance)
	{
		FreeLibrary(hInstance);
		hInstance = NULL;
	}
}

/**
 *	@brief	���̃N���X�������d�l���T�|�[�g���Ă���o�[�W�������ǂ����𒲂ׂ܂��B
 *	@retval	true	�T�|�[�g���Ă���o�[�W�����ł��B
 *	@retval	false	�T�|�[�g���Ă��Ȃ��o�[�W�����ł��B
 */
bool WinLangFile::CheckVersion()
{
	if (SPEC_VERSION == GetVersion())
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 *	@brief	����t�@�C���d�l�̃o�[�W������Ԃ��܂��B
 *	@return	�o�[�W����
 */
DWORD WinLangFile::GetVersion()
{
	PFNCCNLGETVERSION pfnCCNLGetVersion = reinterpret_cast<PFNCCNLGETVERSION>(GetProcAddress(hInstance, "CCNLGetVersion"));
	if (NULL == pfnCCNLGetVersion)
	{
		return 0;
	}

	try
	{
		return pfnCCNLGetVersion();
	}
	catch (...)
	{
		return 0;
	}
}

/**
 *	@brief	����R�[�h��Ԃ��܂��B
 */
void WinLangFile::GetLanguageCode(
	MBCString& langCode			///< ����R�[�h���Ԃ�܂��B
)
{
	langCode.Empty();
	PFNCCNLGETLANGUAGECODE pfnCCNLGetLanguageCode = reinterpret_cast<PFNCCNLGETLANGUAGECODE>(GetProcAddress(hInstance, "CCNLGetLanguageCode"));
	if (NULL == pfnCCNLGetLanguageCode)
	{
		return;
	}

	DWORD length;
	try
	{
		length = pfnCCNLGetLanguageCode(NULL);
	}
	catch (...)
	{
		return;
	}

	char* langCodeBuf = static_cast<char*>(malloc(length + 1));
	if (NULL == langCodeBuf)
	{
		MemoryException::Throw();
	}
	try
	{
		pfnCCNLGetLanguageCode(langCodeBuf);
	}
	catch (...)
	{
		free(langCodeBuf);
		return;
	}
	langCode = langCodeBuf;
	free(langCodeBuf);
}

/**
 *	@brief	���ꖼ��Ԃ��܂��B
 */
void WinLangFile::GetLanguageName(
	MBCString& langName			///< ���ꖼ���Ԃ�܂��B
)
{
	langName.Empty();
	PFNCCNLGETLANGUAGENAME pfnCCNLGetLanguageName = reinterpret_cast<PFNCCNLGETLANGUAGENAME>(GetProcAddress(hInstance, "CCNLGetLanguageName"));
	if (NULL == pfnCCNLGetLanguageName)
	{
		return;
	}

	DWORD length;
	try
	{
		length = pfnCCNLGetLanguageName(NULL);
	}
	catch (...)
	{
		return;
	}

	char* langNameBuf = static_cast<char*>(malloc(length + 1));
	if (NULL == langNameBuf)
	{
		MemoryException::Throw();
	}
	try
	{
		pfnCCNLGetLanguageName(langNameBuf);
	}
	catch (...)
	{
		free(langNameBuf);
		return;
	}
	langName = langNameBuf;
	free(langNameBuf);
}

/**
 *	@brief	���̌���� LANGID ��Ԃ��܂��B
 *	@return	LANGID�i���s����� 0�j
 */
LANGID WinLangFile::GetLangID()
{
	PFNCCNLGETLANGID pfnCCNLGetLangID = reinterpret_cast<PFNCCNLGETLANGID>(GetProcAddress(hInstance, "CCNLGetLangID"));
	if (NULL == pfnCCNLGetLangID)
	{
		return 0;
	}
	return pfnCCNLGetLangID();
}
