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
	@file		WinMonitorInfo.cpp
	@brief		Implementation of WinMonitiorInfo class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.03.22 created
*/

#include "Prefix.h"

#define COMPILE_MULTIMON_STUBS
#include "WinMonitorInfo.h"

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinMonitorInfo::WinMonitorInfo()
{
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinMonitorInfo::~WinMonitorInfo()
{
	clearMonitorInfos();
}

// ---------------------------------------------------------------------
//! �ێ����Ă��郂�j�^�����N���A���܂��B
// ---------------------------------------------------------------------
void WinMonitorInfo::clearMonitorInfos()
{
	MonitorInfoVector::iterator ite;
	for (ite=monitorInfos.begin(); ite!=monitorInfos.end(); ite++)
	{
		MonitorInformation* info = *ite;
		delete info;
	}
	monitorInfos.clear();
}

// ---------------------------------------------------------------------
//! ���j�^�����ŐV�̏�ԂɍX�V���܂��B
// ---------------------------------------------------------------------
void WinMonitorInfo::Update()
{
	clearMonitorInfos();
	::EnumDisplayMonitors(NULL, NULL, monitorEnumProc, reinterpret_cast<LPARAM>(this));
}

// ---------------------------------------------------------------------
//! ���j�^���񋓊֐�
/*!
	@retval	TRUE	�񋓂𑱂���Ƃ�
	@retval	FALSE	�񋓂𒆎~����Ƃ�
*/
// ---------------------------------------------------------------------
BOOL WinMonitorInfo::monitorEnumProc(
	HMONITOR hMonitor,			//!< ���j�^�n���h��
	HDC /* hdcMonitor */,		//!< ���j�^DC �n���h��
	LPRECT /* lprcMonitor */,	//!< �N���b�s���O�G���A�Ƃ̐ς̋�`
	LPARAM dwData				//!< WinMonitorInfo �I�u�W�F�N�g�ւ̃|�C���^
)
{
	WinMonitorInfo* theObj = reinterpret_cast<WinMonitorInfo*>(dwData);
	
	MonitorInformation* monitorInfo = new MonitorInformation;
	monitorInfo->hMonitor = hMonitor;
	ZeroMemory(&monitorInfo->info, sizeof(MONITORINFO));
	monitorInfo->info.cbSize = sizeof(MONITORINFO);
	if (::GetMonitorInfo(hMonitor, &monitorInfo->info))
	{
		theObj->monitorInfos.push_back(monitorInfo);
	}
	else
	{
		delete monitorInfo;
	}
	
	return TRUE;
}

// ---------------------------------------------------------------------
//! ���j�^�̐����擾���܂��B
/*!
	@return	���j�^�̐�
*/
// ---------------------------------------------------------------------
SInt32 WinMonitorInfo::GetMonitorCount() const
{
	return monitorInfos.size();
}

// ---------------------------------------------------------------------
//! �w�肵���C���f�b�N�X�̃��j�^�����擾���܂��B
/*!
	@return	���j�^���
*/
// ---------------------------------------------------------------------
const MONITORINFO* WinMonitorInfo::GetMonitorInfoAt(
	SInt32 index		//!< �C���f�b�N�X
) const
{
	return &monitorInfos[index]->info;
}

// ---------------------------------------------------------------------
//! �w�肵���E�B���h�E���������郂�j�^�i�����ɏ�������ꍇ�͈�ԑ����܂܂����́j�̏����擾���܂��B
/*!
	@return	���j�^���i���݂��Ȃ����NULL�j
*/
// ---------------------------------------------------------------------
const MONITORINFO* WinMonitorInfo::GetMonitorInfoFromWindow(
	HWND hWnd			//!< �E�B���h�E�n���h��
) const
{
	return getMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST));
}

// ---------------------------------------------------------------------
//! �w�肵�����W���܂܂�郂�j�^�i�ǂ��ɂ��܂܂�Ȃ��ꍇ�͈�ԋ߂����́j�̏����擾���܂��B
/*!
	@return	���j�^���i���݂��Ȃ����NULL�j
*/
// ---------------------------------------------------------------------
const MONITORINFO* WinMonitorInfo::GetMonitorInfoFromPoint(
	POINT& point		//!< ���W
) const
{
	return getMonitorInfo(::MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST));
}

// ---------------------------------------------------------------------
//! �w�肵����`���܂܂�郂�j�^�i�ǂ��ɂ��܂܂�Ȃ��ꍇ�͈�ԋ߂����́A�����ɏ�������ꍇ�͈�ԑ����܂܂����́j�̏����擾���܂��B
/*!
	@return	���j�^���i���݂��Ȃ����NULL�j
*/
// ---------------------------------------------------------------------
const MONITORINFO* WinMonitorInfo::GetMonitorInfoFromRect(
	RECT& rect
) const
{
	return getMonitorInfo(::MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST));
}

// ---------------------------------------------------------------------
//! �w�肵�����j�^�n���h���̃��j�^�̏����擾���܂��B
/*!
	@return	���j�^���i���݂��Ȃ����NULL�j
*/
// ---------------------------------------------------------------------
const MONITORINFO* WinMonitorInfo::getMonitorInfo(HMONITOR hMonitor) const
{
	MonitorInfoVector::const_iterator ite;
	for (ite=monitorInfos.begin(); ite!=monitorInfos.end(); ite++)
	{
		MonitorInformation* info = *ite;
		if (info->hMonitor == hMonitor)
		{
			return &info->info;
		}
	}
	
	return NULL;
}
