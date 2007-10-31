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
	@file		CoveredCalcApp.h
	@brief		Definition of CoveredCalcApp class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.27 created
*/

#ifndef _COVEREDCALCAPP_H_
#define _COVEREDCALCAPP_H_

#include "MessageBoxProvider.h"
#include "WaitingUI.h"
#include "UIMessageProvider.h"
#include "Path.h"
#include "KeyMappingManager.h"

class CoverManager;
class AppSettings;
class ExceptionMessageGenerator;
class CommandLineParam;
class WinLangFileInfoCollection;
class LangFileInfoCollection;

// ---------------------------------------------------------------------
//! An interface of application
// ---------------------------------------------------------------------
class CoveredCalcApp : virtual public MessageBoxProvider, virtual public WaitingUI
{
public:
											CoveredCalcApp() { }
	virtual									~CoveredCalcApp() { }

	static CoveredCalcApp*					GetInstance();

	virtual CoverManager*					GetCoverManager() = 0;
	virtual AppSettings*					GetAppSettings() = 0;
	virtual UIMessageProvider*				GetMessageProvider() = 0;
	virtual ExceptionMessageGenerator*		GetExceptionMessageGenerator() = 0;
	virtual CommandLineParam*				GetCommandLineParam() = 0;
	virtual void							GetCurrentLanguageCode(MBCString& outLanguage) = 0;
	virtual const LangFileInfoCollection*	GetLangFileInfos() = 0;	
	virtual Path							MakeRelativeLangFilePath(const Path& absolutePath) = 0;
	virtual Path							MakeAbsoluteLangFilePath(const Path& relativePath) = 0;
	virtual Path							ExpandVirtualKeymapFilePath(const Path& virtualPath) = 0;
	virtual void							LoadKeyMappings(const Path& keymapFile) = 0;
	
	virtual KeyMappingManager*				GetKeyMappingManagerForMainWindow() = 0;

	virtual void							ShowCoverBrowser(bool isShow) = 0;
	virtual bool							IsCoverBrowserVisible() = 0;
};

#endif // _COVEREDCALCAPP_H_
