/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2009 CoveredCalc Project Contributors
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
	@file		WinSkinWindow.cpp
	@brief		WinSkinWindow �N���X�̎���
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.24 created
*/

#include "Prefix.h"
#include <windowsx.h>
#include "UIManager.h"
#include "CoveredCalcApp.h"
#include "WinCoveredCalcApp.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "WinSkinWindow.h"
#include "UIControllerException.h"
#include "WinNormalAppearance.h"
#include "WinLayeredAppearance.h"
#include "MemoryException.h"
#include "CoverManager.h"
#include "CoverDef.h"
#include "CoverInfo.h"
#include "CoverMainWindowInfo.h"

////////////////////////////////////////
#define base	CHrnWnd
////////////////////////////////////////

#define AVAILABLE_AREA_MARGIN		32		//!< �L���Ȕ͈͂��㉺���E���ꂼ��̒[����̋����Ŏw�肷��

#define TIMERID_MOUSEHOVER			100		///< TimerID_MouseHover �ɑΉ�����^�C�}ID

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinSkinWindow::WinSkinWindow()
{
	uiManager = NULL;
	hToolTipWnd = NULL;
	szToolTipText[0] = '\0';
	mouseCaptureNestCount = 0;
	SInt32 index;
	for (index = 0; index < TimerID_Max; index++)
	{
		timerNestCount[index] = 0;
		timerElapse[index] = 0;
	}
	mousePositionAvailable = false;
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinSkinWindow::~WinSkinWindow()
{
	if (NULL != uiManager)
	{
		ASSERT(false);		// ���̎��_�ł͊��ɍ폜����Ă���͂��B
		delete uiManager;	// �h����I�u�W�F�N�g�̃f�X�g���N�^�͊��ɌĂ΂�Ă���̂� deleteUIManager �͌ĂׂȂ��B
	}
	ASSERT(0 == mouseCaptureNestCount);
#ifdef _DEBUG
	SInt32 index;
	for (index = 0; index < TimerID_Max; index++)
	{
		ASSERT(0 == timerNestCount[index]);
	}
#endif
}

/**
 *	@brief	ColorCodedSkinAppearance �����������ĕԂ��܂��B
 *	@return	���������� ColorCodedSkinAppearance �I�u�W�F�N�g�B
 */
ColorCodedSkinAppearance* WinSkinWindow::InitSkinAppearance()
{
	const WinLayeredWindowAPI* layeredWindowAPI = WinCoveredCalcApp::GetInstance()->GetLayeredWindowAPI();
	WinCCSAppearance* appearance = NULL;
	if (layeredWindowAPI->IsSupported_UpdateLayeredWindow())
	{
		WinLayeredAppearance* layeredAppearance = new WinLayeredAppearance();
		if (NULL == layeredAppearance)
		{
			MemoryException::Throw();
		}
		appearance = layeredAppearance;
		
		const AppSettings* appSettings = CoveredCalcApp::GetInstance()->GetAppSettings();
		
		const CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
		const CoverMainWindowInfo* mainWindowInfo = manager->GetCoverDef()->GetCoverInfo(manager->GetCurrentCover())->GetMainWindowInfo();
		
		bool ignoreClipping = !mainWindowInfo->IsTransparentRegionUsedAgainstAlpha();
		SInt32 opacity = appSettings->GetMainWindowOpacity();
		SInt32 edgeSmoothingLevel = mainWindowInfo->GetEdgeSmoothingLevel();
		if (edgeSmoothingLevel < 0)
		{
			// auto ���w�肳�ꂽ�ꍇ�̓A�v���P�[�V�����̃v���t�@�����X����擾
			edgeSmoothingLevel = appSettings->GetMainWindowEdgeSmoothing();
		}
		layeredAppearance->Init(m_hWnd, ignoreClipping, edgeSmoothingLevel, opacity);
	}
	else
	{
		WinNormalAppearance* normalAppearance = new WinNormalAppearance();
		if (NULL == normalAppearance)
		{
			MemoryException::Throw();
		}
		appearance = normalAppearance;
		
		normalAppearance->Init(m_hWnd);
	}
	return appearance;
}

/**
 *	@brief	�s�v�ɂȂ��� ColorCodedSkinAppearance ���̂Ă܂��B
 *	@param[in]	appearance	�s�v�ɂȂ��� ColorCodedSkinAppearance �I�u�W�F�N�g�B
 */
void WinSkinWindow::DisposeSkinAppearance(ColorCodedSkinAppearance* appearance)
{
	delete appearance;
}

/**
 *	@brief	���b�Z�[�W���󂯎�����Ƃ��̃}�E�X�ʒu���܂��擾����Ă��Ȃ���Ύ擾���܂�.
 */
void WinSkinWindow::readyMousePosition() const
{
	if (!mousePositionAvailable)
	{
		DWORD pos = GetMessagePos();
		POINT winPoint;
		winPoint.x = GET_X_LPARAM(pos);
		winPoint.y = GET_Y_LPARAM(pos);
		::ScreenToClient(m_hWnd, &winPoint);

		WinSkinWindow* nonConstThis = const_cast<WinSkinWindow*>(this);
		nonConstThis->mousePosition.x = winPoint.x;
		nonConstThis->mousePosition.y = winPoint.y;
		nonConstThis->mousePositionAvailable = true;
	}
}

// ---------------------------------------------------------------------
//! �}�E�X�ʒu���X�N���[�����W�Ŏ擾���܂��B
// ---------------------------------------------------------------------
Point32 WinSkinWindow::GetMouseScreenPosition() const
{
	readyMousePosition();

	POINT winPoint;
	winPoint.x = mousePosition.x;
	winPoint.y = mousePosition.y;
	::ClientToScreen(m_hWnd, &winPoint);
	
	Point32 retPoint;
	retPoint.x = winPoint.x;
	retPoint.y = winPoint.y;
	return retPoint;
}

// ---------------------------------------------------------------------
//! UI���͂ދ�`�̍��W�i�X�N���[�����W�j���擾���܂��B
// ---------------------------------------------------------------------
void WinSkinWindow::GetUIRect(
	Rect32& rect			//!< OUTPUT. UI���͂ދ�`�̍��W���Ԃ�
) const
{
	if (::IsIconic(m_hWnd) || ::IsZoomed(m_hWnd))
	{
		RECT workArea;
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
		WINDOWPLACEMENT windowPlacement;
		ZeroMemory(&windowPlacement, sizeof(windowPlacement));
		windowPlacement.length = sizeof(WINDOWPLACEMENT);
		::GetWindowPlacement(m_hWnd, &windowPlacement);
		rect.left = windowPlacement.rcNormalPosition.left + workArea.left;
		rect.top = windowPlacement.rcNormalPosition.top + workArea.top;
		rect.right = windowPlacement.rcNormalPosition.right + workArea.left;
		rect.bottom = windowPlacement.rcNormalPosition.bottom + workArea.top;
	}
	else
	{
		RECT winRect;
		::GetWindowRect(m_hWnd, &winRect);
		rect.left = winRect.left;
		rect.top = winRect.top;
		rect.right = winRect.right;
		rect.bottom = winRect.bottom;
	}
}

// ---------------------------------------------------------------------
//! UI�̈ʒu���ړ����܂��B
// ---------------------------------------------------------------------
void WinSkinWindow::MoveUI(
	const Point32& topLeft			//!< UI �̍�����W
)
{
	// ���S�ɉ�ʊO�ɏo�����ȂƂ��͏�����ʓ��Ɏc���悤�ɂ��Ă���ȏ�ړ����Ȃ��悤�ɂ���
	// (�������A�����̈悪�L������J�o�[�ɂ��Ă͒m��Ȃ��[����)
	RECT winRect;
	::GetWindowRect(m_hWnd, &winRect);
	::OffsetRect(&winRect, topLeft.x - winRect.left, topLeft.y - winRect.top);

	bool isOutOfWorkArea = true;
	const WinMonitorInfo* monInfoStore = WinCoveredCalcApp::GetInstance()->GetMonitorInformation();
	SInt32 monCount = monInfoStore->GetMonitorCount();
	SInt32 monIndex;
	for (monIndex=0; monIndex<monCount; monIndex++)
	{
		const MONITORINFO* info = monInfoStore->GetMonitorInfoAt(monIndex);
		RECT workRect = info->rcWork;
		::InflateRect(&workRect, -(AVAILABLE_AREA_MARGIN), -(AVAILABLE_AREA_MARGIN));
		RECT intersectRect;
		if (::IntersectRect(&intersectRect, &winRect, &workRect))
		{
			isOutOfWorkArea = false;
			break;
		}
	}

	Point32 correctTopLeft = topLeft;
	if (isOutOfWorkArea)
	{
		const MONITORINFO* info = monInfoStore->GetMonitorInfoFromRect(winRect);
		if (NULL != info)
		{
			SInt32 width = winRect.right - winRect.left;
			SInt32 height = winRect.bottom - winRect.top;
		
			RECT workRect = info->rcWork;
			::InflateRect(&workRect, -(AVAILABLE_AREA_MARGIN), -(AVAILABLE_AREA_MARGIN));

			if (correctTopLeft.x >= workRect.right)
			{
				correctTopLeft.x = workRect.right - 1;
			}
			else if (correctTopLeft.x + width < workRect.left)
			{
				correctTopLeft.x = workRect.left - width;
			}
			if (correctTopLeft.y >= workRect.bottom)
			{
				correctTopLeft.y = workRect.bottom -1;
			}
			else if (correctTopLeft.y + height < workRect.top)
			{
				correctTopLeft.y = workRect.top - height;
			}			
		}
	}

	::SetWindowPos(m_hWnd, NULL, correctTopLeft.x, correctTopLeft.y, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}

// ---------------------------------------------------------------------
//! �}�E�X�h���b�O���J�n����Ƃ��ɌĂ΂�܂��B
/*!
	�h���b�O���̃��b�Z�[�W�����ׂĎ󂯎���悤�ɁA�E�B���h�E���L���v�`�����܂��B
*/
// ---------------------------------------------------------------------
void WinSkinWindow::BeginMouseCapture()
{
	if (0 == mouseCaptureNestCount)
		::SetCapture(m_hWnd);
	
	mouseCaptureNestCount++;
}

// ---------------------------------------------------------------------
//! �}�E�X�h���b�O���I������Ƃ��ɌĂ΂�܂��B
/*!
	�h���b�O�p�̃L���v�`�����������܂��B
*/
// ---------------------------------------------------------------------
void WinSkinWindow::EndMouseCapture()
{
	if (mouseCaptureNestCount > 0)
	{
		mouseCaptureNestCount--;

		if (0 == mouseCaptureNestCount && ::GetCapture() == m_hWnd)
		{
			::ReleaseCapture();
		}
	}
}

// ---------------------------------------------------------------------
//! �E�B���h�E����܂��B
// ---------------------------------------------------------------------
void WinSkinWindow::CloseUI()
{
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
}

// ---------------------------------------------------------------------
//! �E�B���h�E���A�N�e�B�u���ǂ�����Ԃ��܂��B
/*!
	@retval true �A�N�e�B�u�ł��B
	@retval false �A�N�e�B�u�ł͂���܂���B
*/
// ---------------------------------------------------------------------
bool WinSkinWindow::IsUIActive() const
{
	return (::GetActiveWindow() == m_hWnd) ? true : false;
}

// ---------------------------------------------------------------------
//! �E�B���h�E��\�����܂��B
// ---------------------------------------------------------------------
void WinSkinWindow::Show()
{
	::ShowWindow(m_hWnd, SW_SHOW);
}

// ---------------------------------------------------------------------
//! �E�B���h�E���\���ɂ��܂��B
// ---------------------------------------------------------------------
void WinSkinWindow::Hide()
{
	::ShowWindow(m_hWnd, SW_HIDE);
}

// ---------------------------------------------------------------------
//! �E�B���h�E���ŏ������܂��B
// ---------------------------------------------------------------------
void WinSkinWindow::Minimize()
{
	::ShowWindow(m_hWnd, SW_MINIMIZE);
}

// ---------------------------------------------------------------------
//! �ŏ�������Ă��邩�ǂ����𒲂ׂ܂��B
/*!
	@retval true �ŏ�������Ă��܂��B
	@retval false �ŏ�������Ă��܂���B
*/
// ---------------------------------------------------------------------
bool WinSkinWindow::IsUIMinimized() const
{
	return (::IsMinimized(m_hWnd)) ? true : false;
}

// ---------------------------------------------------------------------
//! �ŏ������ꂽ�E�B���h�E�����ɖ߂��܂��B
// ---------------------------------------------------------------------
void WinSkinWindow::Restore()
{
	::ShowWindow(m_hWnd, SW_RESTORE);
}

// ---------------------------------------------------------------------
//! �w�肵�����j���[���̊e�A�C�e���̏�Ԃ��X�V���܂��B
/*!
	@note �T�u���j���[������΁A���̒��̃A�C�e���̏�Ԃ��X�V���܂��B
*/
// ---------------------------------------------------------------------
void WinSkinWindow::updateMenuItemStates(
	HMENU menu				//!< ���j���[�n���h��
)
{
	int count = ::GetMenuItemCount(menu);
	int index;
	for (index=0; index<count; index++)
	{
		MENUITEMINFO itemInfo;
		ZeroMemory(&itemInfo, sizeof(itemInfo));
		itemInfo.cbSize = sizeof(MENUITEMINFO);
		itemInfo.fMask = MIIM_ID | MIIM_SUBMENU | MIIM_TYPE;
		::GetMenuItemInfo(menu, index, TRUE, &itemInfo);
		if (NULL != itemInfo.hSubMenu)
		{
			updateMenuItemStates(itemInfo.hSubMenu);
		}
		else if (!(itemInfo.fType & MFT_SEPARATOR))
		{
			bool isEnabled = true;
			bool isChecked = false;
			SInt32 command = getMenuCommand(itemInfo.wID);
			UInt32 itemStates = uiManager->GetCommandState(command);
			if (itemStates & UIManager::CommandState_Checked)
			{
				isChecked = true;
			}
			if (itemStates & UIManager::CommandState_Disabled)
			{
				isEnabled = false;
			}

			::EnableMenuItem(menu, index, MF_BYPOSITION | ((isEnabled) ? MF_ENABLED : MF_GRAYED));
			if (itemStates & UIManager::CommandState_Radio)
			{
				if (isChecked)
				{
					::CheckMenuRadioItem(menu, index, index, index, MF_BYPOSITION);
				}
				else
				{
					::CheckMenuItem(menu, index, MF_BYPOSITION | MF_UNCHECKED);
				}
			}
			else
			{
				::CheckMenuItem(menu, index, MF_BYPOSITION | ((isChecked) ? MF_CHECKED : MF_UNCHECKED));
			}
		}
	}
}

// ---------------------------------------------------------------------
//! �����ɃE�B���h�E���X�V���܂��B�i�����̈���Ȃ����܂��j
// ---------------------------------------------------------------------
void WinSkinWindow::UpdateUI()
{
	WinCCSAppearance* appearance = static_cast<WinCCSAppearance*>(uiManager->GetSkinAppearance());
	if (appearance != NULL)
	{
		appearance->UpdateAppearance();
	}
}

// ---------------------------------------------------------------------
//! �w�肵�����Ԃ����ҋ@���܂��B
// ---------------------------------------------------------------------
void WinSkinWindow::Wait(
	UInt32 milliseconds		//!< �ҋ@������� (�~���b)
)
{
	::Sleep(milliseconds);
}

/**
 *	@brief	�c�[���`�b�v�e�L�X�g�Ƃ��ĕ\�����镶�����ݒ肵�܂��B
 */
void WinSkinWindow::SetToolTipText(
	ConstAStr text			///< �c�[���`�b�v�e�L�X�g
)
{
	lstrcpyn(szToolTipText, text, sizeof(szToolTipText)/sizeof(AChar));
	
	TOOLINFO ti;
	ZeroMemory(&ti, sizeof(ti));
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = 0;
	ti.hwnd = m_hWnd;
	ti.uId = 1;

	// �O�̂��폜
	::SendMessage(hToolTipWnd, TTM_DELTOOL, 0, reinterpret_cast<LPARAM>(&ti));
	
	// �V�����ǉ�
	if (szToolTipText[0] != '\0')
	{
		::GetClientRect(m_hWnd, &(ti.rect));
		ti.lpszText = szToolTipText;
		::SendMessage(hToolTipWnd, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));
	}
}

/**
 *	@brief	��Ɏ�O�ɕ\���t���O���Z�b�g/���Z�b�g���܂��B
 */
void WinSkinWindow::SetAlwaysOnTopFlag(
	bool isFlagOn			///< �Z�b�g����Ȃ�true, ���Z�b�g����Ȃ�false
)
{
	if (isFlagOn)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);	
	}
	else
	{
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
	}
}

/**
 *	@brief	UIController::TimerID �̒l�ɑΉ�����^�C�}ID��Ԃ��܂��B
 *	@param	timerId		UIController::TimerID �̒l
 *	@return	SetTimer Win32API �ɃZ�b�g����^�C�}ID
 */
UINT_PTR WinSkinWindow::timerIdToWinTimerId(UIController::TimerID timerId)
{
	switch (timerId)
	{
	case TimerID_MouseHover:
		return TIMERID_MOUSEHOVER;
	default:
		ASSERT(false);
		return 0;
	}
}

/**
 *	@brief	�^�C�}ID�ɑΉ����� UIController::TimerID �̒l��Ԃ��܂��B
 *	@param	winTimerId	SetTimer Win32API �ɓn�����^�C�}ID
 *	@return	UIController::TimerID �̒l
 */
UIController::TimerID WinSkinWindow::winTimerIdToTimerId(UINT_PTR winTimerId)
{
	switch (winTimerId)
	{
	case TIMERID_MOUSEHOVER:
		return TimerID_MouseHover;
	default:
		ASSERT(false);
		return TimerID_Max;
	}
}

/**
 *	@brief	�^�C�}���J�n���܂��B
 *	@param	timerId	�^�C�}ID
 *	@param	elapse	���ԊԊu
 */
void WinSkinWindow::BeginTimer(UIController::TimerID timerId, UInt32 elapse)
{
	if (timerNestCount[timerId] == 0 || timerElapse[timerId] != elapse)
	{
		SetTimer(m_hWnd, timerIdToWinTimerId(timerId), elapse, NULL);
		timerElapse[timerId] = elapse;
	}
	timerNestCount[timerId]++;
}

/**
 *	@brief	�^�C�}���I�����܂��B
 *	@param	timerId	�^�C�}ID
 */
void WinSkinWindow::EndTimer(UIController::TimerID timerId)
{
	if (timerNestCount[timerId] > 0)
	{
		timerNestCount[timerId]--;
		if (timerNestCount[timerId] == 0)
		{
			KillTimer(m_hWnd, timerIdToWinTimerId(timerId));
		}
	}
}

/**
 *	@brief	Called when cover definition is changed.
 */
void WinSkinWindow::CoverDefChanged()
{
	uiManager->RereadSkin();
}

/**
 *	@brief	Called when current cover number is changed.
 */
void WinSkinWindow::CurrentCoverChanged()
{
	uiManager->RereadSkin();
}

// ---------------------------------------------------------------------
//! �E�B���h�E�v���V�[�W��
/*!
	@return �e���b�Z�[�W�ɑΉ�����߂�l
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::wndProc(
	HWND hWnd,			// �E�B���h�E�n���h��
	UINT uMsg,			// ���b�Z�[�W
	WPARAM wParam,		// ���b�Z�[�W�� WPARAM
	LPARAM lParam		// ���b�Z�[�W�� LPARAM
)
{
	LRESULT ret = 0;
	bool isProcessed = false;
	if (uiManager != NULL)
	{
		WinCCSAppearance* appearance = static_cast<WinCCSAppearance*>(uiManager->GetSkinAppearance());
		if (appearance != NULL)
		{
			isProcessed = appearance->RelayWndProc(hWnd, uMsg, wParam, lParam, &ret);
		}
	}

	mousePositionAvailable = false;

	try
	{
		// �c�[���`�b�v�E�B���h�E�ւ̃����[
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			{
				DWORD posVal;
				MSG msg;
				posVal = GetMessagePos();
				msg.hwnd = hWnd;
				msg.message = uMsg;
				msg.wParam = wParam;
				msg.lParam = lParam;
				msg.time = GetMessageTime();
				msg.pt.x = LOWORD(posVal);
				msg.pt.y = HIWORD(posVal);
				::SendMessage(hToolTipWnd, TTM_RELAYEVENT, 0, reinterpret_cast<LPARAM>(&msg));
			}
			break;
		}
	
		// �e���b�Z�[�W�̃n���h�����O
		switch (uMsg)
		{
		case WM_CREATE:
			return onCreate(hWnd, uMsg, wParam, lParam);
			break;
		case WM_DESTROY:
			return onDestroy(hWnd, uMsg, wParam, lParam);
			break;
		case WM_MOUSEMOVE:
			return onMouseMove(hWnd, uMsg, wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
			return onLButtonDown(hWnd, uMsg, wParam, lParam);
			break;
		case WM_LBUTTONUP:
			return onLButtonUp(hWnd, uMsg, wParam, lParam);
			break;
		case WM_RBUTTONDOWN:
			return onRButtonDown(hWnd, uMsg, wParam, lParam);
			break;
		case WM_RBUTTONUP:
			return onRButtonUp(hWnd, uMsg, wParam, lParam);
			break;
		case WM_ACTIVATE:
			return onActivate(hWnd, uMsg, wParam, lParam);
			break;
		case UM_ACTIVATED:
			return onUMActivated(hWnd, uMsg, wParam, lParam);
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			return onKeyUp(hWnd, uMsg, wParam, lParam);
			break;
		case WM_TIMER:
			return onTimer(hWnd, uMsg, wParam, lParam);
			break;
		case UM_REREAD_SKIN:
			return onRereadSkin(hWnd, uMsg, wParam, lParam);
			break;
		case WM_COMMAND:
			return onCommand(hWnd, uMsg, wParam, lParam);
			break;

		default:
			if (isProcessed)
			{
				return ret;
			}
			else
			{
				return base::wndProc(hWnd, uMsg, wParam, lParam);
			}
		}
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
		ex->Delete();
	}
	
	return 0;
}

// ---------------------------------------------------------------------
//! WM_CREATE �n���h��
/*!
	@retval 0 ����
	@retval -1 ���s
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onCreate(
	HWND hWnd,		// �E�B���h�E�n���h��
	UINT uMsg,		// WM_CREATE
	WPARAM wParam,	// ���p���Ȃ��p�����[�^
	LPARAM lParam	// �E�B���h�E�쐬�������� CREATESTRUCT �\���̂ւ̃|�C���^
)
{
	if (-1 == base::wndProc(hWnd, uMsg, wParam, lParam))
	{
		return -1;
	}

	// �c�[���`�b�v�E�B���h�E�̍쐬
	hToolTipWnd = ::CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
						WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
						CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT,
						hWnd, NULL, CHrnApp::GetAppObject()->GetInstanceHandle(),
						NULL);
	::SendMessage(hToolTipWnd, TTM_ACTIVATE, static_cast<WPARAM>(TRUE), 0);
	
	// UI �}�l�[�W�����쐬
	if (NULL != uiManager)
	{
		deleteUIManager(uiManager);
	}

	try
	{
		uiManager = createUIManager();
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex, MessageBoxProvider::ButtonType_OK, MessageBoxProvider::AlertType_Warning);
		ex->Delete();
		return -1;
	}
	
	// MessageFilter �Ƃ��Ď������g���C���X�g�[��
	WinCoveredCalcApp::GetInstance()->InstallMessageFilter(this);

	// register CoverChangedEventHandler
	CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
	manager->RegisterCoverChangeEventHandler(this);

	return 0;
}

// ---------------------------------------------------------------------
//! WM_DESTROY �n���h��
/*!
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onDestroy(
	HWND hWnd,		// �E�B���h�E�n���h��
	UINT uMsg,		// WM_DESTROY
	WPARAM wParam,	// ���p���Ȃ��p�����[�^
	LPARAM lParam	// ���p���Ȃ��p�����[�^
)
{
	// unregister CoverChangedEventHandler
	CoverManager* manager = CoveredCalcApp::GetInstance()->GetCoverManager();
	manager->UnregisterCoverChangeEventHandler(this);
	
	// MessageFilter �Ƃ��Ă̎������g��j��
	WinCoveredCalcApp::GetInstance()->UninstallMessageFilter(this);

	// �}�E�X�L���v�`�����c���Ă��������
	if (mouseCaptureNestCount > 0)
	{
		if (GetCapture() == m_hWnd)
		{
			ReleaseCapture();
		}
		mouseCaptureNestCount = 0;
	}
	// �^�C�}���c���Ă�����I��
	SInt32 index;
	for (index = 0; index < TimerID_Max; index++)
	{
		if (timerNestCount[index] > 0)
		{
			KillTimer(m_hWnd, timerIdToWinTimerId(static_cast<TimerID>(index)));
			timerNestCount[index] = 0;
		}
	}

	base::wndProc(hWnd, uMsg, wParam, lParam);

	if (NULL != uiManager)
	{
		deleteUIManager(uiManager);
		uiManager = NULL;
	}
	
	::DestroyWindow(hToolTipWnd);
	hToolTipWnd = NULL;
	
	return 0;
}

// ---------------------------------------------------------------------
//! WM_MOUSEMOVE �n���h��
/*!
	UIManager �ɃC�x���g��`���܂��B
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onMouseMove(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_MOUSEMOVE
	WPARAM wParam,	//!< �l�X�ȉ��z�L�[��������Ă��邩�ǂ��̃t���O
	LPARAM lParam	//!< ���ʃ��[�h���}�E�X�J�[�\���� X ���W�A��ʃ��[�h�� Y ���W
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	mousePosition.x = GET_X_LPARAM(lParam);
	mousePosition.y = GET_Y_LPARAM(lParam);
	mousePositionAvailable = true;
	
	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->MouseMove();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_LBUTTONDOWN �n���h��
/*!
	UIManager �ɃC�x���g��`���܂��B
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT	WinSkinWindow::onLButtonDown(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_LBUTTONDOWN
	WPARAM wParam,	//!< �l�X�ȉ��z�L�[��������Ă��邩�ǂ��̃t���O
	LPARAM lParam	//!< ���ʃ��[�h���}�E�X�J�[�\���� X ���W�A��ʃ��[�h�� Y ���W
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	mousePosition.x = GET_X_LPARAM(lParam);
	mousePosition.y = GET_Y_LPARAM(lParam);
	mousePositionAvailable = true;

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->Button1Down();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_LBUTTONUP �n���h��
/*!
	UIManager �ɃC�x���g��`���܂��B
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT	WinSkinWindow::onLButtonUp(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_LBUTTONUP
	WPARAM wParam,	//!< �l�X�ȉ��z�L�[��������Ă��邩�ǂ��̃t���O
	LPARAM lParam	//!< ���ʃ��[�h���}�E�X�J�[�\���� X ���W�A��ʃ��[�h�� Y ���W
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	mousePosition.x = GET_X_LPARAM(lParam);
	mousePosition.y = GET_Y_LPARAM(lParam);
	mousePositionAvailable = true;

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->Button1Up();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_RBUTTONDOWN �n���h��
/*!
	UIManager �ɃC�x���g��`���܂��B
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT	WinSkinWindow::onRButtonDown(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_RBUTTONDOWN
	WPARAM wParam,	//!< �l�X�ȉ��z�L�[��������Ă��邩�ǂ��̃t���O
	LPARAM lParam	//!< ���ʃ��[�h���}�E�X�J�[�\���� X ���W�A��ʃ��[�h�� Y ���W
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	mousePosition.x = GET_X_LPARAM(lParam);
	mousePosition.y = GET_Y_LPARAM(lParam);
	mousePositionAvailable = true;

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->Button2Down();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_RBUTTONUP �n���h��
/*!
	UIManager �ɃC�x���g��`���܂��B
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT	WinSkinWindow::onRButtonUp(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_RBUTTONUP
	WPARAM wParam,	//!< �l�X�ȉ��z�L�[��������Ă��邩�ǂ��̃t���O
	LPARAM lParam	//!< ���ʃ��[�h���}�E�X�J�[�\���� X ���W�A��ʃ��[�h�� Y ���W
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	mousePosition.x = GET_X_LPARAM(lParam);
	mousePosition.y = GET_Y_LPARAM(lParam);
	mousePositionAvailable = true;

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->Button2Up();
	}

	return 0;
}

// ---------------------------------------------------------------------
//! WM_ACTIVATE �n���h��
/*!
	UM_ACTIVATED ���������g�Ƀ|�X�g���܂��B
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onActivate(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_ACTIVATE
	WPARAM wParam,	//!< ���ʃ��[�h���A�N�e�B�u������邩��A�N�e�B�u������邩�������A��ʃ��[�h���E�B���h�E���ŏ�������Ă��邩�ǂ���������
	LPARAM lParam	//!< �A�N�e�B�u/��A�N�e�B�u�ɂȂ�E�B���h�E�̃n���h��
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	// ���[�_���_�C�A���O���o�Ă���Ƃ��̑Ώ�
	bool willBeActive = (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) ? true : false;
	if (willBeActive && !::IsWindowEnabled(m_hWnd))
	{
		// ���Ԃ�A���[�_���_�C�A���O���o�Ă����ԂŃA�N�e�B�u�ɂ��悤�Ƃ����̂ŁA
		// �_�C�A���O�̕����A�N�e�B�u�ɂ���
		WinCoveredCalcApp::GetInstance()->ActivateModalDialog();
		
		// �������g�̓A�N�e�B�u�ɂȂ�Ȃ��i�Ȃ��������Ƃɂ���j
		return 0;
	}

	// ���̃��b�Z�[�W�̎��_�ł́A���ۂɃA�N�e�B�u��/��A�N�e�B�u����
	// �܂�����Ă��Ȃ��̂ŁAUM_ACTIVATED ���������g�Ƀ|�X�g���Ă�����
	// ������̃n���h���� UIManager �ɃC�x���g��`����
	::PostMessage(m_hWnd, UM_ACTIVATED, 0, 0);

	return 0;
}

// ---------------------------------------------------------------------
//! UM_ACTIVATED �n���h��
/*!
	UIManager �ɃC�x���g��`���܂��B
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onUMActivated(
	HWND /*hWnd*/,		//!< �E�B���h�E�n���h��
	UINT /*uMsg*/,		//!< UM_ACTIVATED
	WPARAM /*wParam*/,	//!< ���p���Ȃ��p�����[�^
	LPARAM /*lParam*/	//!< ���p���Ȃ��p�����[�^
)
{
	UIManager* uiManager = getUIManager();
	if (NULL != uiManager)
	{
		uiManager->UIActivated();
	}

	return 0;
}

/**
 *	@brief	Filters message.
 *	@param[in,out] msg	message.
 *	@return false to skip this message. true to dispatch this message.
 */
bool WinSkinWindow::FilterMessage(MSG* msg)
{
	if (msg->hwnd != m_hWnd)
		return true;
	
	if (WM_KEYDOWN == msg->message || WM_SYSKEYDOWN == msg->message)
	{
		// �}�E�X�{�^���A�C���L�[�͖���
		switch (msg->wParam)
		{
		case VK_LBUTTON:
		case VK_RBUTTON:
		case VK_MBUTTON:
		case VK_SHIFT:
		case VK_CONTROL:
		case VK_MENU:
			return true;
		}

		UIManager* uiManager = getUIManager();
		ASSERT(NULL != uiManager);
		if (NULL != uiManager)
		{
			KeyEventParameter parameter;
			UInt32 modifiers = 0;
			modifiers |= ((::GetKeyState(VK_SHIFT)   & 0x8000) ? KeyEventParameter::ModifierMask_Shift : 0);
			modifiers |= ((::GetKeyState(VK_CONTROL) & 0x8000) ? KeyEventParameter::ModifierMask_Ctrl  : 0);
			modifiers |= ((::GetKeyState(VK_MENU)    & 0x8000) ? KeyEventParameter::ModifierMask_Alt   : 0);
			parameter.SetKeyCode(static_cast<DWORD>(msg->wParam));
			parameter.SetModifiers(modifiers);
			return uiManager->KeyDown(parameter);
		}
	}
	return true;
}

// ---------------------------------------------------------------------
//! WM_KEYUP �n���h��
/*!
	UIManager �ɃC�x���g��`���܂��B
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onKeyUp(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_KEYUP
	WPARAM wParam,	//!< ���z�L�[�R�[�h
	LPARAM lParam	//!< �t���O
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	// �}�E�X�{�^���͖���
	switch (wParam)
	{
	case VK_LBUTTON:
	case VK_RBUTTON:
	case VK_MBUTTON:
	case VK_SHIFT:
	case VK_CONTROL:
	case VK_MENU:
		return 0;
	}

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		KeyEventParameter parameter;
		UInt32 modifiers = 0;
		modifiers |= ((::GetKeyState(VK_SHIFT)   & 0x8000) ? KeyEventParameter::ModifierMask_Shift : 0);
		modifiers |= ((::GetKeyState(VK_CONTROL) & 0x8000) ? KeyEventParameter::ModifierMask_Ctrl  : 0);
		modifiers |= ((::GetKeyState(VK_MENU)    & 0x8000) ? KeyEventParameter::ModifierMask_Alt   : 0);
		parameter.SetKeyCode(static_cast<DWORD>(wParam));
		parameter.SetModifiers(modifiers);
		uiManager->KeyUp(parameter);
	}

	return 0;
}

/**
 *	@brief	WM_TIMER �n���h��
 *
 *	UIManager �ɃC�x���g��`���܂��B
 *	@retval	0	���������Ƃ�
 */
LRESULT WinSkinWindow::onTimer(
   	HWND hWnd,		///< �E�B���h�E�n���h��
	UINT uMsg,		///< WM_TIMER
	WPARAM wParam,	///< �^�C�}ID
	LPARAM lParam	///< �R�[���o�b�N�֐�
)
{
	base::wndProc(hWnd, uMsg, wParam, lParam);

	UIManager* uiManager = getUIManager();
	ASSERT(NULL != uiManager);
	if (NULL != uiManager)
	{
		uiManager->TimerEvent(winTimerIdToTimerId(static_cast<UINT_PTR>(wParam)));
	}

	return 0;
}

/**
 *	@brief	UM_REREAD_SKIN �n���h��
 *	@retval	0 ���������Ƃ�
 */
LRESULT WinSkinWindow::onRereadSkin(
   	HWND /*hWnd*/,		///< �E�B���h�E�n���h��
	UINT /*uMsg*/,		///< UM_REREAD_SKIN
	WPARAM /*wParam*/,	///< ���p���Ȃ��p�����[�^
	LPARAM /*lParam*/	///< ���p���Ȃ��p�����[�^
)
{
	getUIManager()->RereadSkin();
	return 0;
}

// ---------------------------------------------------------------------
//! WM_COMMAND �n���h��
/*!
	@retval 0 ���̃��b�Z�[�W����������
*/
// ---------------------------------------------------------------------
LRESULT WinSkinWindow::onCommand(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< WM_COMMAND
	WPARAM wParam,		//!< ��ʃ��[�h���ʒm�R�[�h�A���ʃ��[�h���R�}���hID
	LPARAM lParam		//!< ���̃��b�Z�[�W�𑗂����R���g���[���̃n���h��
)
{
	WORD menuID = LOWORD(wParam);

	SInt32 command = getMenuCommand(menuID);
	if (NULL != uiManager && UIManager::Command_None != command)
	{
		uiManager->ExecuteCommand(command);
		return 0;
	}
	else
	{
		return base::wndProc(hWnd, uMsg, wParam, lParam);
	}
}
