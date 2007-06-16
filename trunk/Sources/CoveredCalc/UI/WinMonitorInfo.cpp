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
//! コンストラクタ
// ---------------------------------------------------------------------
WinMonitorInfo::WinMonitorInfo()
{
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
WinMonitorInfo::~WinMonitorInfo()
{
	clearMonitorInfos();
}

// ---------------------------------------------------------------------
//! 保持しているモニタ情報をクリアします。
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
//! モニタ情報を最新の状態に更新します。
// ---------------------------------------------------------------------
void WinMonitorInfo::Update()
{
	clearMonitorInfos();
	::EnumDisplayMonitors(NULL, NULL, monitorEnumProc, reinterpret_cast<LPARAM>(this));
}

// ---------------------------------------------------------------------
//! モニタ情報列挙関数
/*!
	@retval	TRUE	列挙を続けるとき
	@retval	FALSE	列挙を中止するとき
*/
// ---------------------------------------------------------------------
BOOL WinMonitorInfo::monitorEnumProc(
	HMONITOR hMonitor,			//!< モニタハンドル
	HDC /* hdcMonitor */,		//!< モニタDC ハンドル
	LPRECT /* lprcMonitor */,	//!< クリッピングエリアとの積の矩形
	LPARAM dwData				//!< WinMonitorInfo オブジェクトへのポインタ
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
//! モニタの数を取得します。
/*!
	@return	モニタの数
*/
// ---------------------------------------------------------------------
SInt32 WinMonitorInfo::GetMonitorCount() const
{
	return monitorInfos.size();
}

// ---------------------------------------------------------------------
//! 指定したインデックスのモニタ情報を取得します。
/*!
	@return	モニタ情報
*/
// ---------------------------------------------------------------------
const MONITORINFO* WinMonitorInfo::GetMonitorInfoAt(
	SInt32 index		//!< インデックス
) const
{
	return &monitorInfos[index]->info;
}

// ---------------------------------------------------------------------
//! 指定したウィンドウが所属するモニタ（複数に所属する場合は一番多く含まれるもの）の情報を取得します。
/*!
	@return	モニタ情報（存在しなければNULL）
*/
// ---------------------------------------------------------------------
const MONITORINFO* WinMonitorInfo::GetMonitorInfoFromWindow(
	HWND hWnd			//!< ウィンドウハンドル
) const
{
	return getMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST));
}

// ---------------------------------------------------------------------
//! 指定した座標が含まれるモニタ（どこにも含まれない場合は一番近いもの）の情報を取得します。
/*!
	@return	モニタ情報（存在しなければNULL）
*/
// ---------------------------------------------------------------------
const MONITORINFO* WinMonitorInfo::GetMonitorInfoFromPoint(
	POINT& point		//!< 座標
) const
{
	return getMonitorInfo(::MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST));
}

// ---------------------------------------------------------------------
//! 指定した矩形が含まれるモニタ（どこにも含まれない場合は一番近いもの、複数に所属する場合は一番多く含まれるもの）の情報を取得します。
/*!
	@return	モニタ情報（存在しなければNULL）
*/
// ---------------------------------------------------------------------
const MONITORINFO* WinMonitorInfo::GetMonitorInfoFromRect(
	RECT& rect
) const
{
	return getMonitorInfo(::MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST));
}

// ---------------------------------------------------------------------
//! 指定したモニタハンドルのモニタの情報を取得します。
/*!
	@return	モニタ情報（存在しなければNULL）
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
