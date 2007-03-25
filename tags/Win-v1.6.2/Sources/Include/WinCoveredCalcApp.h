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
	@file		WinCoveredCalcApp.h
	@brief		WinCoveredCalcApp �N���X�̒�`
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.22 created
*/

#ifndef _COVEREDCALCWINAPP_H_
#define _COVEREDCALCWINAPP_H_

#include "HrnApp.h"
#include "CoveredCalcAppBase.h"
#include "WinMainWindow.h"
#include "WinCoverBrowser.h"
#include "WinMonitorInfo.h"
#include "WinLangFile.h"

// ---------------------------------------------------------------------
//! CoveredCalc Windows �� �A�v���P�[�V�����N���X
/*!
	CoveredCalc Windows �ł̃A�v���P�[�V�����S�̂�\���܂��B
	���̃N���X�̓V���O���g���ł���A�B��̃I�u�W�F�N�g�������̉����܂���B
*/
// ---------------------------------------------------------------------
class WinCoveredCalcApp : public CHrnApp, public CoveredCalcAppBase
{
public:
	static void						CreateInstance();
	static void						DeleteInstance();

	static WinCoveredCalcApp*		GetInstance() { return theInstance; }

	void							Quit();

	void							EnableCoveredCalcWindows(bool isEnabled);
	void							ActivateModalDialog();

	virtual void					ShowCoverBrowser(bool isShow);
	virtual bool					IsCoverBrowserVisible();

	WinMonitorInfo*					GetMonitorInformation()			{ return &monitorInfo; }
	const WinMonitorInfo*			GetMonitorInformation() const	{ return &monitorInfo; }

	HINSTANCE						GetLangResHandle() const;

	// MessageBoxProvider �C���^�t�F�[�X�̎���
	virtual Button					DoMessageBox(ConstAStr message, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);
	virtual Button					DoMessageBox(SInt32 messageId, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);

	// WaitingUI �C���^�t�F�[�X�̎���
	virtual void					BeginWaitingUI();
	virtual void					EndWaitingUI();

private:
									WinCoveredCalcApp();
									~WinCoveredCalcApp();
	
	virtual BOOL					initInstance();
	virtual void					exitInstance();
	virtual int						messageLoop();

protected:
	virtual const Path&				getAppFolderPath() { return appFolderPath; }
	virtual void					readyDefaultSettingFilePath(Path& settingFilePath);

private:
	void							makeAppFolderPath();
	void							createFolder(const Path& path);
	void							loadLangFile(const Path& path);
	SInt32							autoSelectLangFile();
	
private:
	static WinCoveredCalcApp*		theInstance;		//!< �B��̃C���X�^���X
	Path							appFolderPath;		//!< �A�v���P�[�V�����̂���t�H���_�̃p�X
	WinMainWindow					mainWindow;			//!< ���C���E�B���h�E
	WinCoverBrowser					coverBrowser;		//!< �J�o�[�u���E�U�E�B���h�E
	HCURSOR							waitCursor;			//!< �������������E�F�C�g�J�[�\��
	HCURSOR							originalCursor;		//!< �E�F�C�g�J�[�\���ɂ���O�̃J�[�\��
	SInt32							waitingUICount;		//!< ���� EndWaitingUI() �̑O�� BeginWaitingUI() ���Ăяo���ꂽ��
	bool							isInEnableCoveredCalcWindows;	//!< EnableCoveredCalcWindows �̏��������ǂ���
	WinMonitorInfo					monitorInfo;		//!< ���j�^���
	WinLangFile						langFile;			///< ����t�@�C��
};

#endif // _COVEREDCALCWINAPP_H_