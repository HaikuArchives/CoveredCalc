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
#include "StringID.h"
#include "WinDialogControlCreator.h"
#include "DialogID.h"

////////////////////////////////////////
#define baseDialog	WinDialog
#define base		PreferencesDlg
////////////////////////////////////////

#define MIN_OPACITY				25
#define MAX_OPACITY				255
#define MIN_EDGE_SMOOTHING		0
#define MAX_EDGE_SMOOTHING		10

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
 *	@brief	Creates dialog controls.
 */
void WinPreferencesDlg::createControls()
{
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
	WinDialogControlCreator dcc(m_hWnd, getDialogLayout());
	MBCString label;
	HWND hControl;
	
	// "Transparency" group box
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_GROUP_TRANSPARENCY));
	hControl = dcc.CreateGroupBox(ALITERAL("IDC_GROUP_TRANSPARENCY"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Opacity" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_OPACITY));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_OPACITY"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Opacity" slider left label ("Transparent")
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_TRANSPARENT));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_TRANSPARENT"), IDC_STC_TRANSPARENT, label, SS_RIGHT, SS_LEFT | SS_CENTER, 0, 0);

	// "Opacity" slider
	hControl = dcc.CreateTrackbar(ALITERAL("IDC_SLDR_OPACITY"), IDC_SLDR_OPACITY, 0, 0, 0, 0);

	// "Opacity" slider right label ("Opaque")
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_OPAQUE));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_OPAQUE"), IDC_STC_OPAQUE, label, SS_LEFT, SS_RIGHT | SS_CENTER, 0, 0);

	// "Cover Edge Smoothing" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_SMOTHING));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_SMOOTHING"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Cover Edge Smoothing" slider left label ("Low")
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_LOW));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_LOW"), IDC_STC_LOW, label, SS_RIGHT, SS_LEFT | SS_CENTER, 0, 0);

	// "Cover Edge Smoothing" slider
	hControl = dcc.CreateTrackbar(ALITERAL("IDC_SLDR_EDGE_SMOOTHING"), IDC_SLDR_EDGE_SMOOTHING, 0, 0, 0, 0);

	// "Cover Edge Smoothing" slider right label ("High")
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_HIGH));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STC_HIGH"), IDC_STC_HIGH, label, SS_LEFT, SS_RIGHT | SS_CENTER, 0, 0);

	// message
	label = stringLoader->LoadNativeString(IDS_PREFERENCES_TRANSPARENCY_MESSAGE);
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_TRANSPARENCY_MESSAGE"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Language" group box
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_GROUP_LANGUAGE));
	hControl = dcc.CreateGroupBox(ALITERAL("IDC_GROUP_LANGUAGE"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Language" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_LANGUAGE));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_LANGUAGE"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Language" combo box
	hControl = dcc.CreateComboBox(ALITERAL("IDC_CMB_LANGUAGE"), IDC_CMB_LANGUAGE, WS_VSCROLL | CBS_DROPDOWNLIST | CBS_SORT, 0, 0, 0);

	// message
	label = stringLoader->LoadNativeString(IDS_PREFERENCES_LANGUAGE_MESSAGE);
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_LANGUAGE_MESSAGE"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Keymap" group box
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_GROUP_KEYMAP));
	hControl = dcc.CreateGroupBox(ALITERAL("IDC_GROUP_KEYMAP"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Keymap" label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_KEYMAP));
	hControl = dcc.CreateStatic(ALITERAL("IDC_STATIC_KEYMAP"), IDC_STATIC, label, WS_GROUP, 0, 0, 0);

	// "Keymap" combo box
	hControl = dcc.CreateComboBox(ALITERAL("IDC_CMB_KEYMAPPINGS"), IDC_CMB_KEYMAPPINGS, WS_VSCROLL | CBS_DROPDOWNLIST, 0, 0, 0);

	// "Edit" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_EDIT_KEYMAP));
	hControl = dcc.CreateButton(ALITERAL("IDC_EDIT_KEYMAPPING"), IDC_EDIT_KEYMAPPING, label, WS_GROUP, 0, 0, 0);

	// "Duplicate" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_DUPLICATE_KEYMAP));
	hControl = dcc.CreateButton(ALITERAL("IDC_DUPLICATE_KEYMAPPING"), IDC_DUPLICATE_KEYMAPPING, label, 0, 0, 0, 0);

	// "Delete" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_DELETE_KEYMAP));
	hControl = dcc.CreateButton(ALITERAL("IDC_DELETE_KEYMAPPING"), IDC_DELETE_KEYMAPPING, label, 0, 0, 0, 0);

	// "OK" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_OK));
	hControl = dcc.CreateButton(ALITERAL("IDOK"), IDOK, label, WS_GROUP | BS_DEFPUSHBUTTON, BS_PUSHBUTTON, 0, 0);
	
	// "Cancel" button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(IDS_PREFERENCES_CANCEL));
	hControl = dcc.CreateButton(ALITERAL("IDCANCEL"), IDCANCEL, label, 0, 0, 0, 0);
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

	// create controls
	createControls();

	// set dialog title
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
	SetWindowText(m_hWnd, stringLoader->LoadNativeString(IDS_PREFERENCES_TITLE).CString());

	// FIXME: フォーカス

	HWND hComboKeymappings = GetDlgItem(hWnd, IDC_CMB_KEYMAPPINGS);
	keyMappingSeparators.Attach(hComboKeymappings);

	HWND hControl;
	HWND hBuddy;
	
	// 透明度
	hControl = GetDlgItem(hWnd, IDC_SLDR_OPACITY);
	SendMessage(hControl, TBM_SETRANGE, FALSE, MAKELONG(MIN_OPACITY, MAX_OPACITY));
	SendMessage(hControl, TBM_SETTICFREQ, (MAX_OPACITY - MIN_OPACITY) / 10, 0);
	hBuddy = GetDlgItem(hWnd, IDC_STC_TRANSPARENT);
	SendMessage(hControl, TBM_SETBUDDY, TRUE, reinterpret_cast<LPARAM>(hBuddy));
	hBuddy = GetDlgItem(hWnd, IDC_STC_OPAQUE);
	SendMessage(hControl, TBM_SETBUDDY, FALSE, reinterpret_cast<LPARAM>(hBuddy));
	
	// カバー境界のなめらかさ
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
 *	@brief	WM_COMMAND ハンドラ
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
 *	@brief	WM_DESTROY ハンドラ
 *	@param[in]	hWnd	ウィンドウハンドル
 *	@param[in]	uMsg	WM_DESTROY
 *	@param[in]	wParam	使用しません。
 *	@param[in]	lParam	使用しません。
 *	@retval 0 このメッセージを処理した。 
 */
LRESULT WinPreferencesDlg::onDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	keyMappingSeparators.Detach();
	return baseDialog::wndProc(hWnd, uMsg, wParam, lParam);
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
		comboIndex = ComboBox_AddString(hWndLangCombo, ALITERAL("# invalid language #"));
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
	
	// コンボボックスの中身をセット
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
	
	// 現在の選択値を選択
	if (CB_ERR == comboIndex)
	{
		comboIndex = ComboBox_AddString(hWndKeyMappingCombo, ALITERAL("# invalid key-mapping #"));
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
 *	@brief	外観の透明度をセットします。
 *	@param	opacity	不透明度
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
 *	@brief	外観の透明度を取得します。
 *	@return	不透明度
 */
SInt32 WinPreferencesDlg::getOpacity()
{
	HWND hSlider = GetDlgItem(m_hWnd, IDC_SLDR_OPACITY);
	return SendMessage(hSlider, TBM_GETPOS, 0, 0);
}

/**
 *	@brief	透明度の設定の有効/無効を設定します。
 *	@param	isEnabled	有効にするなら true、無効にするなら false。
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
 *	@brief	カバー境界のなめらかさを設定します。
 *	@param	edgeSmoothing	スムージングレベル
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
 *	@brief	カバー境界のなめらかさを取得します。
 *	@return	スムージングレベル
 */
SInt32 WinPreferencesDlg::getEdgeSmoothing()
{
	HWND hSlider = GetDlgItem(m_hWnd, IDC_SLDR_EDGE_SMOOTHING);
	return SendMessage(hSlider, TBM_GETPOS, 0, 0);
}

/**
 *	@brief	カバー境界のなめらかさの設定の有効/無効を設定します。
 *	@param	isEnabled	有効にするなら true、無効にするなら false。
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
