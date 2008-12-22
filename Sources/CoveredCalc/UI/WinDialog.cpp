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
	@file		WinDialog.cpp
	@brief		Implementation of WinDialog class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.03.22 created
*/

#include "Prefix.h"
#include "WinDialog.h"
#include "WinCoveredCalcApp.h"
#include "WinMessageBoxProvider.h"
#include "MemoryException.h"
#include "UTF8Conv.h"

////////////////////////////////////////
#define base	CHrnDlg
////////////////////////////////////////

/**
 *	@brief	Constructor
 *	@param[in]	dialogID	dialog ID
 */
WinDialog::WinDialog(SInt32 dialogID)
	: base(static_cast<UINT>(0))
{
	this->dialogID = dialogID;
}

/**
 *	@brief	モーダルダイアログを表示します。
 *	@return	ダイアログの戻り値
 */
int WinDialog::DoModal(
	HWND hParent	///< 親ウィンドウ
)
{
	readyDialogLayout();

	int result;
	BYTE* dialogTemplate = createDialogTemplate();
	try
	{
		m_bModal = TRUE;
		result = ::DialogBoxIndirectParam(WinCoveredCalcApp::GetInstance()->GetInstanceHandle(),
											reinterpret_cast<LPDLGTEMPLATEW>(dialogTemplate),
											hParent, CHrnDlg::cbHrnDlgProc, reinterpret_cast<LPARAM>(this));
	}
	catch (...)
	{
		free(dialogTemplate);
		throw;
	}
	free(dialogTemplate);
	return result;
}

/**
 *	@brief	モードレスダイアログを生成します。
 *	@retval	TRUE	成功
 *	@retval	FALSE	失敗
 */
BOOL WinDialog::Create(
	HWND hParent	///< 親ウィンドウ
)
{
	readyDialogLayout();

	BOOL result;
	BYTE* dialogTemplate = createDialogTemplate();
	try
	{
		m_bModal = FALSE;
		HWND hWnd = ::CreateDialogIndirectParam(WinCoveredCalcApp::GetInstance()->GetInstanceHandle(),
											reinterpret_cast<LPDLGTEMPLATEW>(dialogTemplate),
											hParent, CHrnDlg::cbHrnDlgProc, reinterpret_cast<LPARAM>(this));
		result = (NULL == hWnd) ? FALSE : TRUE;
	}
	catch (...)
	{
		free(dialogTemplate);
		throw;
	}
	free(dialogTemplate);
	return result;
}

/**
 *	@brief	Loads dialog layout.
 */
void WinDialog::readyDialogLayout()
{
	CoveredCalcApp::GetInstance()->LoadDialogLayout(dialogID, dialogLayout);
	dialogLayout.Compute();
}

/**
 *	@brief	Creates dialog template on memory.
 *	@return	created dialog template is returned.
 *			it is allocated by malloc(). the caller must free() it later. 
 */
BYTE* WinDialog::createDialogTemplate()
{
	/*
		typedef struct {
			WORD dlgVer;
			WORD signature;
			DWORD helpID;
			DWORD exStyle;
			DWORD style;
			WORD cDlgItems;
			short x;
			short y;
			short cx;
			short cy;
			sz_Or_Ord menu;
			sz_Or_Ord windowClass;
			WCHAR title[titleLen];
			WORD pointsize;
			WORD weight;
			BYTE italic;
			BYTE charset;
			WCHAR typeface[stringLen];
		} DLGTEMPLATEEX;
	*/

	CoveredCalcApp* app = CoveredCalcApp::GetInstance();

	DialogFont dialogFont;
	app->LoadDialogFont(dialogID, dialogFont);

	// typeface (Unicode)
	std::wstring typeface;
#if defined(_UNICODE)
	AStr buf = UTF8Conv::ToMultiByte(dialogFont.Typeface.CString());
	typeface = buf;
	free(buf);
#else
	int wideCharCount = ::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(dialogFont.Typeface.CString()), -1, NULL, 0);
	LPWSTR buf = static_cast<LPWSTR>(malloc(wideCharCount * sizeof(WCHAR)));
	if (NULL == buf)
	{
		MemoryException::Throw();
	}
	::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(dialogFont.Typeface.CString()), -1, buf, wideCharCount);
	typeface = buf;
	free(buf);
#endif

	// window rect
	RECT rectWindow;
	rectWindow.left = dialogLayout.GetComputedValue(ALITERAL("Window.left"));
	rectWindow.top = dialogLayout.GetComputedValue(ALITERAL("Window.top"));
	rectWindow.right = dialogLayout.GetComputedValue(ALITERAL("Window.right"));
	rectWindow.bottom = dialogLayout.GetComputedValue(ALITERAL("Window.bottom"));

	// allocate memory and set values.
	BYTE* result = NULL;
	BYTE* ptr = NULL;
	enum
	{
		Phase_Counting = 0,
		Phase_Assigning,
		Phase_NoMore
	};
	SInt32 phase;
	for (phase = Phase_Counting; phase < Phase_NoMore; ++phase)
	{
		if (Phase_Counting == phase)
		{
			ptr = NULL;
		}
		else if (Phase_Assigning == phase)
		{
			SInt32 bufSize = ptr - static_cast<BYTE*>(NULL);
			ptr = result = static_cast<BYTE*>(malloc(bufSize));
			if (NULL == ptr)
			{
				MemoryException::Throw();
			}
		}

		// WORD dlgVer;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<WORD*>(ptr) = 1;
		}
		ptr += sizeof(WORD);

		// WORD signature;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<WORD*>(ptr) = 0xffff;
		}
		ptr += sizeof(WORD);

		// DWORD helpID;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<DWORD*>(ptr) = dialogID;
		}
		ptr += sizeof(DWORD);

		// DWORD exStyle;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<DWORD*>(ptr) = getDialogExStyle();
		}
		ptr += sizeof(DWORD);

		// DWORD style;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<DWORD*>(ptr) = getDialogStyle();
		}
		ptr += sizeof(DWORD);

		// WORD cDlgItems;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<WORD*>(ptr) = 0;
		}
		ptr += sizeof(WORD);

		// short x;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<short*>(ptr) = static_cast<short>(rectWindow.left);
		}
		ptr += sizeof(short);

		// short y;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<short*>(ptr) = static_cast<short>(rectWindow.top);
		}
		ptr += sizeof(short);

		// short cx;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<short*>(ptr) = static_cast<short>(rectWindow.right - rectWindow.left);
		}
		ptr += sizeof(short);

		// short cy;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<short*>(ptr) = static_cast<short>(rectWindow.bottom - rectWindow.top);
		}
		ptr += sizeof(short);

		// sz_Or_Ord menu;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<WORD*>(ptr) = 0x0000;
		}
		ptr += sizeof(WORD);

		// sz_Or_Ord windowClass;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<WORD*>(ptr) = 0x0000;
		}
		ptr += sizeof(WORD);

		// WCHAR title[titleLen];
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<WCHAR*>(ptr) = L'\0';
		}
		ptr += sizeof(WCHAR);

		// WORD pointsize;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<WORD*>(ptr) = dialogFont.PointSize;
		}
		ptr += sizeof(WORD);

		// WORD weight;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<WORD*>(ptr) = FW_NORMAL;
		}
		ptr += sizeof(WORD);

		// BYTE italic;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<BYTE*>(ptr) = FALSE;
		}
		ptr += sizeof(BYTE);

		// BYTE charset;
		if (Phase_Assigning == phase)
		{
			*reinterpret_cast<BYTE*>(ptr) = dialogFont.Charset;
		}
		ptr += sizeof(BYTE);

		// WCHAR typeface[stringLen];
		if (Phase_Assigning == phase)
		{
			wcscpy(reinterpret_cast<WCHAR*>(ptr), typeface.c_str());
		}
		ptr += sizeof(WCHAR) * (typeface.length() + 1);
	}

	return result;
}

/**
 *	@brief	Returns style of this dialog.
 */
DWORD WinDialog::getDialogStyle()
{
	return DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU;
}

/**
 *	@brief	Returns extended style of this dialog.
 */
DWORD WinDialog::getDialogExStyle()
{
	return 0;
}

// ---------------------------------------------------------------------
//! ウィンドウプロシージャ
/*!
	@return 各メッセージに対応する戻り値
*/
// ---------------------------------------------------------------------
LRESULT WinDialog::wndProc(
	HWND hWnd,			//!< ウィンドウハンドル
	UINT uMsg,			//!< メッセージ
	WPARAM wParam,		//!< メッセージの WPARAM
	LPARAM lParam		//!< メッセージの LPARAM
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return onInitDialog(hWnd, uMsg, wParam, lParam);
		break;		
	default:
		return base::wndProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}

// ---------------------------------------------------------------------
//! WM_INITDIALOG ハンドラ
/*!
	@retval TRUE wParam で指定されたコントロールにフォーカスを与えてください。
	@retval FALSE フォーカスを与えないでください。
*/
// ---------------------------------------------------------------------
LRESULT WinDialog::onInitDialog(
	HWND hWnd,			//!< ウィンドウハンドル
	UINT uMsg,			//!< WM_INITDIALOG
	WPARAM wParam,		//!< handle to the control to receive the default keyboard focus
	LPARAM lParam		//!< additional initialization data
)
{
	LRESULT result = base::wndProc(hWnd, uMsg, wParam, lParam);
	
	// 画面外にはみ出そうなら補正する
	const WinMonitorInfo* monitorInfo = WinCoveredCalcApp::GetInstance()->GetMonitorInformation();
	const MONITORINFO* monInfo = monitorInfo->GetMonitorInfoFromWindow(m_hWnd);
	if (NULL != monInfo)
	{
		RECT windowRect;
		::GetWindowRect(m_hWnd, &windowRect);
		
		LONG delta;
		bool moved = false;
		
		// 下端
		delta = windowRect.bottom - monInfo->rcWork.bottom;
		if (delta > 0)
		{
			::OffsetRect(&windowRect, 0, -delta);
			moved = true;
		}
		
		// 右端
		delta = windowRect.right - monInfo->rcWork.right;
		if (delta > 0)
		{
			::OffsetRect(&windowRect, -delta, 0);
			moved = true;
		}
		
		// 上端
		delta = windowRect.top - monInfo->rcWork.top;
		if (delta < 0)
		{
			::OffsetRect(&windowRect, 0, -delta);
			moved = true;
		}
		
		// 左端
		delta = windowRect.left - monInfo->rcWork.left;
		if (delta < 0)
		{
			::OffsetRect(&windowRect, -delta, 0);
			moved = true;
		}

		if (moved)
		{		
			::SetWindowPos(m_hWnd, NULL, windowRect.left, windowRect.top, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
		}
	}
	
	return result;
}

// ---------------------------------------------------------------------
//! メッセージボックスを表示します。
/*!
	@return ユーザが選択したボタン
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button WinDialog::DoMessageBox(
	ConstAStr message,							//!< message text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	HWND hOwner;
	if (NULL != m_hWnd && ::IsWindow(m_hWnd))
	{
		hOwner = m_hWnd;
	}
	else
	{
		hOwner = NULL;
	}
	WinMessageBoxProvider messageBoxProvider(hOwner);
	return messageBoxProvider.DoMessageBox(message, buttonType, alertType, defaultButton);
}

// ---------------------------------------------------------------------
//! メッセージボックスを表示します。
/*!
	@return ユーザが選択したボタン
*/
// ---------------------------------------------------------------------
MessageBoxProvider::Button WinDialog::DoMessageBox(
	SInt32 messageId,							//!< message id of the text
	ButtonType buttonType,						//!< button type
	AlertType alertType,						//!< alert type
	Button defaultButton /* = Button_None */	//!< default button
)
{
	HWND hOwner;
	if (NULL != m_hWnd && ::IsWindow(m_hWnd))
	{
		hOwner = m_hWnd;
	}
	else
	{
		hOwner = NULL;
	}
	WinMessageBoxProvider messageBoxProvider(hOwner);
	return messageBoxProvider.DoMessageBox(messageId, buttonType, alertType, defaultButton);
}
