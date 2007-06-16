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
	@file		WinMonitorInfo.h
	@brief		Definition of WinMonitiorInfo class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.03.22 created
*/

#ifndef _WINMONITORINFO_H_
#define _WINMONITORINFO_H_

#include <vector>
#include <multimon.h>

// ---------------------------------------------------------------------
//! （マルチ）モニタ情報を保持するクラスです。
// ---------------------------------------------------------------------
class WinMonitorInfo
{
public:
								WinMonitorInfo();
	virtual						~WinMonitorInfo();

	void						Update();
	
	SInt32						GetMonitorCount() const;
	const MONITORINFO*			GetMonitorInfoAt(SInt32 index) const;
	
	const MONITORINFO*			GetMonitorInfoFromWindow(HWND hWnd) const;
	const MONITORINFO*			GetMonitorInfoFromPoint(POINT& point) const;
	const MONITORINFO*			GetMonitorInfoFromRect(RECT& rect) const;

private:
	struct MonitorInformation
	{
		HMONITOR				hMonitor;
		MONITORINFO				info;
	};
	typedef std::vector<MonitorInformation*>	MonitorInfoVector;

private:
	void						clearMonitorInfos();
	static BOOL CALLBACK		monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
	const MONITORINFO*			getMonitorInfo(HMONITOR hMonitor) const;

private:
	MonitorInfoVector			monitorInfos;
};

#endif // _WINMONITORINFO_H_
