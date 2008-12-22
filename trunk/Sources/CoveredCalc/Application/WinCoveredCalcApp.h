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
#include "WinLayeredWindowAPI.h"

class WinMessageFilter;

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

	virtual bool					CheckPlatform(ConstUTF8Str platform);

	WinMonitorInfo*					GetMonitorInformation()			{ return &monitorInfo; }
	const WinMonitorInfo*			GetMonitorInformation() const	{ return &monitorInfo; }

	virtual void					LoadDialogFont(SInt32 dialogId, DialogFont& outFont);

	const WinLayeredWindowAPI*		GetLayeredWindowAPI() const		{ return &apiLayeredWindow; }

	const WinMainWindow*			GetMainWindow() const			{ return &mainWindow; }

	void							InstallMessageFilter(WinMessageFilter* filter) { messageFilterManager.InstallMessageFilter(filter); }
	void							UninstallMessageFilter(WinMessageFilter* filter) {  messageFilterManager.UninstallMessageFilter(filter); }
	bool							CallMessageFilters(MSG* msg) { return messageFilterManager.CallFilters(msg); }

	// MessageBoxProvider �C���^�t�F�[�X�̎���
	virtual Button					DoMessageBox(ConstAStr message, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);
	virtual Button					DoMessageBox(SInt32 messageId, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);

	// WaitingUI �C���^�t�F�[�X�̎���
	virtual void					BeginWaitingUI();
	virtual void					EndWaitingUI();

protected:
	virtual void					loadLangFile(const Path& path);

private:
									WinCoveredCalcApp();
									~WinCoveredCalcApp();
	
	virtual BOOL					initInstance();
	virtual void					exitInstance();
	virtual int						messageLoop();

protected:
	virtual const Path&				getAppFolderPath();
	virtual const Path&				getUserSettingsPath();
	
private:
	void							loadKeyMappingsOnInit();
	void							loadKeyNameDB();
	
private:
	static WinCoveredCalcApp*		theInstance;		//!< �B��̃C���X�^���X
	Path							appFolderPath;		//!< �A�v���P�[�V�����̂���t�H���_�̃p�X
	Path							userSettingsPath;	///< ���[�U�[�ݒ��ۑ�����t�H���_�̃p�X
	WinMainWindow					mainWindow;			//!< ���C���E�B���h�E
	WinCoverBrowser					coverBrowser;		//!< �J�o�[�u���E�U�E�B���h�E
	HCURSOR							waitCursor;			//!< �������������E�F�C�g�J�[�\��
	HCURSOR							originalCursor;		//!< �E�F�C�g�J�[�\���ɂ���O�̃J�[�\��
	SInt32							waitingUICount;		//!< ���� EndWaitingUI() �̑O�� BeginWaitingUI() ���Ăяo���ꂽ��
	bool							isInEnableCoveredCalcWindows;	//!< EnableCoveredCalcWindows �̏��������ǂ���
	WinMonitorInfo					monitorInfo;		//!< ���j�^���
	WinLayeredWindowAPI				apiLayeredWindow;	///< ���C���[�h�E�B���h�E�֘A�� API ���b�p�[
	DialogFont*						dialogFont;			///< Dialog font.

private:
	class MessageFilterManager
	{
	public:
									MessageFilterManager();
									~MessageFilterManager();
									
		void						InstallMessageFilter(WinMessageFilter* filter);
		void						UninstallMessageFilter(WinMessageFilter* filter);

		bool						CallFilters(MSG* msg);

	private:
		bool						canEraseItem()	{ return (enumerating > 0) ? false : true; }
		void						eraseNoUseItems();
		
	private:
		typedef	std::vector<WinMessageFilter*>	FilterVector;
		FilterVector				filters;		///< installed filters
		SInt32						enumerating;	///< incremented at beginning of DoForEach func and decremented at endding of DoForEach func.
	};
	MessageFilterManager			messageFilterManager;	///< Manager for WinMessageFilters
};

#endif // _COVEREDCALCWINAPP_H_