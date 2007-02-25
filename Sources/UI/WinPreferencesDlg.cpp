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
	@file		WinPreferencesDlg.cpp
	@brief		Implementation of WinPreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.1.14 created
*/

#include "Prefix.h"
#include "WinPreferencesDlg.h"
#include "resource.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcApp.h"
#include "LangFileInfo.h"

////////////////////////////////////////
#define baseDialog	WinDialog
#define base		PreferencesDlg
////////////////////////////////////////

/**
 *	@brief	�R���X�g���N�^
 */
WinPreferencesDlg::WinPreferencesDlg() : baseDialog(IDD_PREFERENCES)
{
	langComboInfos = NULL;
}

/**
 *	@brief	�f�X�g���N�^
 */
WinPreferencesDlg::~WinPreferencesDlg()
{
}

/**
 *	@brief	�E�B���h�E�v���V�[�W��
	@return �e���b�Z�[�W�ɑΉ�����߂�l
 */
LRESULT WinPreferencesDlg::wndProc(
	HWND hWnd,			///< �E�B���h�E�n���h��
	UINT uMsg,			///< ���b�Z�[�W
	WPARAM wParam,		///< ���b�Z�[�W�� WPARAM
	LPARAM lParam		///< ���b�Z�[�W�� LPARAM
)
{
	try
	{
		switch (uMsg)
		{
		case WM_INITDIALOG:
			return onInitDialog(hWnd, uMsg, wParam, lParam);
			break;
		case WM_COMMAND:
			return onCommand(hWnd, uMsg, wParam, lParam);
			break;
		default:
			return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
			break;
		}
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
		ex->Delete();
	}
	
	return 0;
}

/**
 *	@brief	WM_INITDIALOG �n���h��
 *	@retval TRUE wParam �Ŏw�肳�ꂽ�R���g���[���Ƀt�H�[�J�X��^���Ă��������B
 *	@retval FALSE �t�H�[�J�X��^���Ȃ��ł��������B
 */
LRESULT WinPreferencesDlg::onInitDialog(
	HWND hWnd,			///< �E�B���h�E�n���h��
	UINT uMsg,			///< ���b�Z�[�W
	WPARAM wParam,		///< ���b�Z�[�W�� WPARAM
	LPARAM lParam		///< ���b�Z�[�W�� LPARAM
)
{
	baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
	
	loadToDialog();
	return TRUE;
}

/**	@brief	WM_COMMAND �n���h��
 *	@retval 0 ���̃��b�Z�[�W�����������B
 */
LRESULT WinPreferencesDlg::onCommand(
	HWND hWnd,			///< �E�B���h�E�n���h��
	UINT uMsg,			///< ���b�Z�[�W
	WPARAM wParam,		///< ���b�Z�[�W�� WPARAM
	LPARAM lParam		///< ���b�Z�[�W�� LPARAM
)
{
	WORD command = LOWORD(wParam);
	switch (command)
	{
	case IDOK:
		if (saveFromDialog())
		{
			EndDialog(hWnd, IDOK);
		}
		return 0;
		break;
	default:
		return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}

/**
 *	@brief	����R���{�{�b�N�X�̓��e���Z�b�g���A���݂̑I�������X�V���܂��B
 */
void WinPreferencesDlg::setLanguage(
	const LangFileInfoCollection& langFileInfos,	///< ����R���{�{�b�N�X�̓��e�ɂȂ�����܂ރR���N�V����
	const Path& currentLangFilePath					///< ���݂̑I��
)
{
	langComboInfos = &langFileInfos;
	
	HWND hWndLangCombo = GetDlgItem(m_hWnd, IDC_CMB_LANGUAGE);
	ASSERT(NULL != hWndLangCombo);
	if (NULL == hWndLangCombo)
		return;
	
	// �R���{�{�b�N�X�̒��g���Z�b�g
	ComboBox_ResetContent(hWndLangCombo);
	SInt32 selectIndex = -1;
	SInt32 count = langFileInfos.GetCount();
	SInt32 index;
	for (index=0; index<count; index++)
	{
		const LangFileInfo& info = langFileInfos.GetAt(index);
		int addedComboIndex = ComboBox_AddString(hWndLangCombo, info.GetLanguageName().CString());
		if (CB_ERR != addedComboIndex)
		{
			ComboBox_SetItemData(hWndLangCombo, addedComboIndex, index);
		}
		
		if (0 == info.GetPath().Compare(currentLangFilePath))
		{
			selectIndex = index;
		}
	}
	
	// ���݂̑I��l��I��
	int comboIndex = CB_ERR;
	if (-1 < selectIndex)
	{
		int cmbCount = ComboBox_GetCount(hWndLangCombo);
		int cmbIndex;
		for (cmbIndex=0; cmbIndex<cmbCount; cmbIndex++)
		{
			SInt32 itemIndex = ComboBox_GetItemData(hWndLangCombo, cmbIndex);
			if (itemIndex == selectIndex)
			{
				comboIndex = cmbIndex;
				break;
			}
		}
		ASSERT(CB_ERR != comboIndex);
	}
	else
	{
		comboIndex = ComboBox_AddString(hWndLangCombo, "# invalid language #");
		if (CB_ERR != comboIndex)
		{
			ComboBox_SetItemData(hWndLangCombo, comboIndex, -1);
		}
	}
	
	if (CB_ERR != comboIndex)
	{
		ComboBox_SetCurSel(hWndLangCombo, comboIndex);
	}	
}

/**
 *	@brief	����R���{�{�b�N�X�̌��݂̒l���擾���܂��B
 *	@return	�擾�ł���� true�A�����łȂ���� false ��Ԃ��܂��B
 *	@note	�擾�ł��Ȃ��ꍇ�͂��̊֐����ŃG���[���b�Z�[�W��\�����܂��B
 */
bool WinPreferencesDlg::getLanguage(
	Path& langFilePath		///< OUTPUT. ���ݑI������Ă��錾��t�@�C���̃p�X���Ԃ���܂��B
)
{
	HWND hWndLangCombo = GetDlgItem(m_hWnd, IDC_CMB_LANGUAGE);
	ASSERT(NULL != hWndLangCombo);
	ASSERT(NULL != langComboInfos);
	if (NULL == hWndLangCombo || NULL == langComboInfos)
	{
		CoveredCalcApp::GetInstance()->DoMessageBox(IDS_EMSG_GET_LANGUAGE,
				MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);	
		return false;
	}

	int comboIndex = ComboBox_GetCurSel(hWndLangCombo);
	if (CB_ERR == comboIndex)
	{
		CoveredCalcApp::GetInstance()->DoMessageBox(IDS_EMSG_GET_LANGUAGE,
				MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		SetFocus(hWndLangCombo);	
		return false;
	}
	SInt32 itemIndex = ComboBox_GetItemData(hWndLangCombo, comboIndex);
	if (itemIndex < 0 || itemIndex >= langComboInfos->GetCount())
	{
		CoveredCalcApp::GetInstance()->DoMessageBox(IDS_EMSG_INVALID_LANGUAGE,
				MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		SetFocus(hWndLangCombo);	
		return false;
	}
	
	langFilePath = langComboInfos->GetAt(itemIndex).GetPath();
	return true;
}
