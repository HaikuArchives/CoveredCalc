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
 *	@brief	コンストラクタ
 */
WinPreferencesDlg::WinPreferencesDlg() : baseDialog(IDD_PREFERENCES)
{
	langComboInfos = NULL;
}

/**
 *	@brief	デストラクタ
 */
WinPreferencesDlg::~WinPreferencesDlg()
{
}

/**
 *	@brief	ウィンドウプロシージャ
	@return 各メッセージに対応する戻り値
 */
LRESULT WinPreferencesDlg::wndProc(
	HWND hWnd,			///< ウィンドウハンドル
	UINT uMsg,			///< メッセージ
	WPARAM wParam,		///< メッセージの WPARAM
	LPARAM lParam		///< メッセージの LPARAM
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
 *	@brief	WM_INITDIALOG ハンドラ
 *	@retval TRUE wParam で指定されたコントロールにフォーカスを与えてください。
 *	@retval FALSE フォーカスを与えないでください。
 */
LRESULT WinPreferencesDlg::onInitDialog(
	HWND hWnd,			///< ウィンドウハンドル
	UINT uMsg,			///< メッセージ
	WPARAM wParam,		///< メッセージの WPARAM
	LPARAM lParam		///< メッセージの LPARAM
)
{
	baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
	
	loadToDialog();
	return TRUE;
}

/**	@brief	WM_COMMAND ハンドラ
 *	@retval 0 このメッセージを処理した。
 */
LRESULT WinPreferencesDlg::onCommand(
	HWND hWnd,			///< ウィンドウハンドル
	UINT uMsg,			///< メッセージ
	WPARAM wParam,		///< メッセージの WPARAM
	LPARAM lParam		///< メッセージの LPARAM
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
 *	@brief	言語コンボボックスの内容をセットし、現在の選択肢を更新します。
 */
void WinPreferencesDlg::setLanguage(
	const LangFileInfoCollection& langFileInfos,	///< 言語コンボボックスの内容になる情報を含むコレクション
	const Path& currentLangFilePath					///< 現在の選択
)
{
	langComboInfos = &langFileInfos;
	
	HWND hWndLangCombo = GetDlgItem(m_hWnd, IDC_CMB_LANGUAGE);
	ASSERT(NULL != hWndLangCombo);
	if (NULL == hWndLangCombo)
		return;
	
	// コンボボックスの中身をセット
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
	
	// 現在の選択値を選択
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
 *	@brief	言語コンボボックスの現在の値を取得します。
 *	@return	取得できれば true、そうでなければ false を返します。
 *	@note	取得できない場合はこの関数内でエラーメッセージを表示します。
 */
bool WinPreferencesDlg::getLanguage(
	Path& langFilePath		///< OUTPUT. 現在選択されている言語ファイルのパスが返されます。
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
