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

/*!
	@file		WinPreferencesDlg.cpp
	@brief		Implementation of WinPreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.1.14 created
*/

#include "Prefix.h"
#include "WinPreferencesDlg.h"
#include <vector>
#include "resource.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcApp.h"
#include "LangFileInfo.h"
#include "WinCoveredCalcApp.h"
#include "WinEditKeymapDlg.h"

////////////////////////////////////////
#define baseDialog	WinDialog
#define base		PreferencesDlg
////////////////////////////////////////

#define MIN_OPACITY				25
#define MAX_OPACITY				255
#define MIN_EDGE_SMOOTHING		0
#define MAX_EDGE_SMOOTHING		10

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
 *	@brief	Shows "Edit Keyboard" dialog and wait until it is closed.
 *	@param[in]		isReadOnly	true when read-only mode.
 *	@param[in,out]	keyMappings	Key-mappings definition.
 *	@retval	true	user closes the dialog by OK button.
 *	@retval	false	user closes the dialog by Cancel button.
 */
bool WinPreferencesDlg::showEditKeyMapDialog(bool isReadOnly, KeyMappings& keyMappings)
{
	WinEditKeymapDlg dlg;
	dlg.Init(isReadOnly, CoveredCalcApp::GetInstance()->GetKeyNameDB());
	dlg.SetKeyMappings(&keyMappings);
	if (IDOK == dlg.DoModal(m_hWnd))
	{
		return true;
	}
	else
	{
		return false;
	}
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
		case WM_DESTROY:
			return onDestroy(hWnd, uMsg, wParam, lParam);
			break;
		default:
			return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
			break;
		}
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(this, ex);
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

	HWND hComboKeymappings = GetDlgItem(hWnd, IDC_CMB_KEYMAPPINGS);
	keyMappingSeparators.Attach(hComboKeymappings);

	HWND hControl;
	HWND hBuddy;
	
	// �����x
	hControl = GetDlgItem(hWnd, IDC_SLDR_OPACITY);
	SendMessage(hControl, TBM_SETRANGE, FALSE, MAKELONG(MIN_OPACITY, MAX_OPACITY));
	SendMessage(hControl, TBM_SETTICFREQ, (MAX_OPACITY - MIN_OPACITY) / 10, 0);
	hBuddy = GetDlgItem(hWnd, IDC_STC_TRANSPARENT);
	SendMessage(hControl, TBM_SETBUDDY, TRUE, reinterpret_cast<LPARAM>(hBuddy));
	hBuddy = GetDlgItem(hWnd, IDC_STC_OPAQUE);
	SendMessage(hControl, TBM_SETBUDDY, FALSE, reinterpret_cast<LPARAM>(hBuddy));
	
	// �J�o�[���E�̂Ȃ߂炩��
	hControl = GetDlgItem(hWnd, IDC_SLDR_EDGE_SMOOTHING);
	SendMessage(hControl, TBM_SETRANGE, FALSE, MAKELONG(MIN_EDGE_SMOOTHING, MAX_EDGE_SMOOTHING));
	SendMessage(hControl, TBM_SETTICFREQ, 1, 0);
	hBuddy = GetDlgItem(hWnd, IDC_STC_LOW);
	SendMessage(hControl, TBM_SETBUDDY, TRUE, reinterpret_cast<LPARAM>(hBuddy));
	hBuddy = GetDlgItem(hWnd, IDC_STC_HIGH);
	SendMessage(hControl, TBM_SETBUDDY, FALSE, reinterpret_cast<LPARAM>(hBuddy));
	
	loadToDialog();
	return TRUE;
}

/**
 *	@brief	WM_COMMAND �n���h��
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
	case IDC_CMB_KEYMAPPINGS:
		if (CBN_SELCHANGE == HIWORD(wParam))
		{
			processKeyMappingSelectionChanged();
			return 0;
		}
		else
		{
			return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	case IDC_EDIT_KEYMAPPING:
		doEditKeyMapping();
		return 0;
		break;
	case IDC_DUPLICATE_KEYMAPPING:
		doDuplicateKeyMapping();
		return 0;
		break;
	case IDC_DELETE_KEYMAPPING:
		doDeleteKeyMapping();
		return 0;
		break;
	case IDOK:
		if (saveFromDialog())
		{
			EndDialog(hWnd, IDOK);
			PostMessage(WinCoveredCalcApp::GetInstance()->GetMainWindow()->m_hWnd, WinSkinWindow::UM_REREAD_SKIN, 0, 0);
		}
		return 0;
		break;
	default:
		return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}

/**
 *	@brief	WM_DESTROY �n���h��
 *	@param[in]	hWnd	�E�B���h�E�n���h��
 *	@param[in]	uMsg	WM_DESTROY
 *	@param[in]	wParam	�g�p���܂���B
 *	@param[in]	lParam	�g�p���܂���B
 *	@retval 0 ���̃��b�Z�[�W�����������B 
 */
LRESULT WinPreferencesDlg::onDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	keyMappingSeparators.Detach();
	return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
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
		DoMessageBox(IDS_EMSG_GET_LANGUAGE,
				MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);	
		return false;
	}

	int comboIndex = ComboBox_GetCurSel(hWndLangCombo);
	if (CB_ERR == comboIndex)
	{
		DoMessageBox(IDS_EMSG_GET_LANGUAGE,
				MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		SetFocus(hWndLangCombo);	
		return false;
	}
	SInt32 itemIndex = ComboBox_GetItemData(hWndLangCombo, comboIndex);
	if (itemIndex < 0 || itemIndex >= langComboInfos->GetCount())
	{
		DoMessageBox(IDS_EMSG_INVALID_LANGUAGE,
				MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		SetFocus(hWndLangCombo);	
		return false;
	}
	
	langFilePath = langComboInfos->GetAt(itemIndex).GetPath();
	return true;
}

/**
 *	@brief	Sets key-mapping menu and current item.
 *	@param	keyMappingInfos			This collection contains informations about all key-mapping menu items.
 *	@param	currentKeyMappingPath	Current selection.
 *	@note	The keyMappingInfos object is valid until the next call of setKeyMapping, or until the dialog is closed.
 */
void WinPreferencesDlg::setKeyMapping(const KeyMappingsInfoPtrVector& keyMappingsInfos, const Path& currentKeyMappingPath)
{
	HWND hWndKeyMappingCombo = GetDlgItem(m_hWnd, IDC_CMB_KEYMAPPINGS);
	ASSERT(NULL != hWndKeyMappingCombo);
	if (NULL == hWndKeyMappingCombo)
		return;
	
	// �R���{�{�b�N�X�̒��g���Z�b�g
	int comboIndex = CB_ERR;
	KMCategory category = KMCategory_Invalid;
	keyMappingSeparators.ClearAllSeparators();
	ComboBox_ResetContent(hWndKeyMappingCombo);
	SInt32 count = keyMappingsInfos.size();
	SInt32 index;
	for (index = 0; index < count; index++)
	{
		const KeyMappingsInfo* info = keyMappingsInfos[index];
		int addedComboIndex = ComboBox_AddString(hWndKeyMappingCombo, info->title);
		if (CB_ERR != addedComboIndex)
		{
			ComboBox_SetItemData(hWndKeyMappingCombo, addedComboIndex, info);
		}
		if (category != info->category)
		{
			if (KMCategory_Invalid != category)
			{
				keyMappingSeparators.AddSeparatorAt(addedComboIndex - 1);
			}
			category = info->category;
		}
		
		if (0 == info->keyMapFilePath.Compare(currentKeyMappingPath))
		{
			comboIndex = addedComboIndex;
		}
	}
	
	// ���݂̑I��l��I��
	if (CB_ERR == comboIndex)
	{
		comboIndex = ComboBox_AddString(hWndKeyMappingCombo, "# invalid key-mapping #");
		if (CB_ERR != comboIndex)
		{
			ComboBox_SetItemData(hWndKeyMappingCombo, comboIndex, NULL);
		}
		if (comboIndex > 0)
		{
			keyMappingSeparators.AddSeparatorAt(comboIndex - 1);
		}
	}
	if (CB_ERR != comboIndex)
	{
		ComboBox_SetCurSel(hWndKeyMappingCombo, comboIndex);
		processKeyMappingSelectionChanged();
	}	
}

/**
 *	@brief	Retrieves current item of key-mapping menu.
 *	@param[in]	doErrorProcessing	if this parameter is true and an error has occured, the error message is shown in this function.
 *	@return pointer to key-mapping file info which is selected on key-mapping menu. NULL should be returned when an error has occured.
 */
const PreferencesDlg::KeyMappingsInfo* WinPreferencesDlg::getKeyMapping(bool doErrorProcessing)
{
	HWND hWndKeyMappingCombo = GetDlgItem(m_hWnd, IDC_CMB_KEYMAPPINGS);
	ASSERT(NULL != hWndKeyMappingCombo);
	if (NULL == hWndKeyMappingCombo)
	{
		if (doErrorProcessing)
		{
			DoMessageBox(IDS_EMSG_GET_KEYMAPPINGS,
					MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Stop);
		}
		return NULL;
	}

	int comboIndex = ComboBox_GetCurSel(hWndKeyMappingCombo);
	if (CB_ERR == comboIndex)
	{
		if (doErrorProcessing)
		{
			DoMessageBox(IDS_EMSG_GET_KEYMAPPINGS,
					MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
			SetFocus(hWndKeyMappingCombo);
		}
		return NULL;
	}
	const KeyMappingsInfo* itemInfo = reinterpret_cast<const KeyMappingsInfo*>(ComboBox_GetItemData(hWndKeyMappingCombo, comboIndex));
	if (NULL == itemInfo)
	{
		if (doErrorProcessing)
		{
			DoMessageBox(IDS_EMSG_INVALID_KEYMAPPINGS,
					MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
			SetFocus(hWndKeyMappingCombo);
		}
		return NULL;
	}
	
	return itemInfo;
}

/**
 *	@brief	Enables or disables "Edit keymapping" button.
 *	@param	isEnabled	enables the control when true.
 */
void WinPreferencesDlg::enableEditKeyMapping(bool isEnabled)
{
	HWND hControl;
	hControl = GetDlgItem(m_hWnd, IDC_EDIT_KEYMAPPING);
	EnableWindow(hControl, isEnabled);
}

/**
 *	@brief	Enables or disables "Duplicate keymapping" button.
 *	@param	isEnabled	enables the control when true.
 */
void WinPreferencesDlg::enableDuplicateKeyMapping(bool isEnabled)
{
	HWND hControl;
	hControl = GetDlgItem(m_hWnd, IDC_DUPLICATE_KEYMAPPING);
	EnableWindow(hControl, isEnabled);
}

/**
 *	@brief	Enables or disables "Delete keymapping" button.
 *	@param	isEnabled	enables the control when true.
 */
void WinPreferencesDlg::enableDeleteKeyMapping(bool isEnabled)
{
	HWND hControl;
	hControl = GetDlgItem(m_hWnd, IDC_DELETE_KEYMAPPING);
	EnableWindow(hControl, isEnabled);
}

/**
 *	@brief	�O�ς̓����x���Z�b�g���܂��B
 *	@param	opacity	�s�����x
 */
void WinPreferencesDlg::setOpacity(SInt32 opacity)
{
	if (opacity < MIN_OPACITY)
	{
		opacity = MIN_OPACITY;
	}
	else if (opacity > MAX_OPACITY)
	{
		opacity = MAX_OPACITY;
	}
	
	HWND hSlider = GetDlgItem(m_hWnd, IDC_SLDR_OPACITY);
	SendMessage(hSlider, TBM_SETPOS, TRUE, opacity);
}

/**
 *	@brief	�O�ς̓����x���擾���܂��B
 *	@return	�s�����x
 */
SInt32 WinPreferencesDlg::getOpacity()
{
	HWND hSlider = GetDlgItem(m_hWnd, IDC_SLDR_OPACITY);
	return SendMessage(hSlider, TBM_GETPOS, 0, 0);
}

/**
 *	@brief	�����x�̐ݒ�̗L��/������ݒ肵�܂��B
 *	@param	isEnabled	�L���ɂ���Ȃ� true�A�����ɂ���Ȃ� false�B
 */
void WinPreferencesDlg::enableOpacity(bool isEnabled)
{
	HWND hControl;
	
	hControl = GetDlgItem(m_hWnd, IDC_STC_OPACITY);
	EnableWindow(hControl, isEnabled);
	
	hControl = GetDlgItem(m_hWnd, IDC_STC_TRANSPARENT);
	EnableWindow(hControl, isEnabled);
	
	hControl = GetDlgItem(m_hWnd, IDC_SLDR_OPACITY);
	EnableWindow(hControl, isEnabled);
	
	hControl = GetDlgItem(m_hWnd, IDC_STC_OPAQUE);
	EnableWindow(hControl, isEnabled);
}

/**
 *	@brief	�J�o�[���E�̂Ȃ߂炩����ݒ肵�܂��B
 *	@param	edgeSmoothing	�X���[�W���O���x��
 */
void WinPreferencesDlg::setEdgeSmoothing(SInt32 edgeSmoothing)
{
	if (edgeSmoothing < MIN_EDGE_SMOOTHING)
	{
		edgeSmoothing = MIN_EDGE_SMOOTHING;
	}
	else if (edgeSmoothing > MAX_EDGE_SMOOTHING)
	{
		edgeSmoothing = MAX_EDGE_SMOOTHING;
	}
	
	HWND hSlider = GetDlgItem(m_hWnd, IDC_SLDR_EDGE_SMOOTHING);
	SendMessage(hSlider, TBM_SETPOS, TRUE, edgeSmoothing);
}

/**
 *	@brief	�J�o�[���E�̂Ȃ߂炩�����擾���܂��B
 *	@return	�X���[�W���O���x��
 */
SInt32 WinPreferencesDlg::getEdgeSmoothing()
{
	HWND hSlider = GetDlgItem(m_hWnd, IDC_SLDR_EDGE_SMOOTHING);
	return SendMessage(hSlider, TBM_GETPOS, 0, 0);
}

/**
 *	@brief	�J�o�[���E�̂Ȃ߂炩���̐ݒ�̗L��/������ݒ肵�܂��B
 *	@param	isEnabled	�L���ɂ���Ȃ� true�A�����ɂ���Ȃ� false�B
 */
void WinPreferencesDlg::enableEdgeSmoothing(bool isEnabled)
{
	HWND hControl;
	
	hControl = GetDlgItem(m_hWnd, IDC_STC_EDGE_SMOOTHING);
	EnableWindow(hControl, isEnabled);
	
	hControl = GetDlgItem(m_hWnd, IDC_STC_LOW);
	EnableWindow(hControl, isEnabled);
	
	hControl = GetDlgItem(m_hWnd, IDC_SLDR_EDGE_SMOOTHING);
	EnableWindow(hControl, isEnabled);
	
	hControl = GetDlgItem(m_hWnd, IDC_STC_HIGH);
	EnableWindow(hControl, isEnabled);
}
