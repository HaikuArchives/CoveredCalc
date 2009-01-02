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
	@file		CommandID.h
	@brief		Definition of command ID
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.09.01 Created
*/

#ifndef _COMMANDID_H_
#define _COMMANDID_H_

enum
{
	ID_NULL							= 0,

	ID_COVER_BROWSER				= 'Cvbr',
	ID_ABOUT						= 'Abut',
	ID_PROCESSED					= 'Prcs',

	// generic dialog
	ID_DIALOG_OK					= 'dOK_',
	ID_DIALOG_CANCEL				= 'dCcl',
	ID_DIALOG_ACTIVATE				= 'dAct',
	
	// main window
	ID_MAIN_MINIMIZE				= 'mMin',
	ID_MAIN_CLOSE					= 'mCls',
	ID_MAIN_ABOUT_COVER				= 'mAbc',
	ID_MAIN_ALWAYS_ON_TOP			= 'mTop',
	ID_MAIN_LOCK_POS				= 'mLck',
	ID_RADIX_HEX					= 'rHex',
	ID_RADIX_DECIMAL				= 'rDec',
	ID_RADIX_OCTAL					= 'rOct',
	ID_RADIX_BINARY					= 'rBin',
	
	// cover browser
	ID_COVERBROWSER_RELOAD			= 'bRld',
	ID_COVERBROWSER_APPLY			= 'bApl',
	ID_COVERBROWSER_CLOSE			= 'bCls',
	
	// about current cover dialog
	ID_ABOUTCURCOV_DO_UPDATE		= 'acUp',
	
	// preferences
	ID_PREFERENCES					= 'Pref',
	ID_PREF_KEYMAP_SELECTED			= 'pKmS',
	ID_PREF_EDIT_KEYMAP				= 'pEdK',
	ID_PREF_DUPLICATE_KEYMAP		= 'pDuK',
	ID_PREF_DELETE_KEYMAP			= 'pDeK',
	
	// edit keyboard
	ID_EDITKEYMAP_REMOVE			= 'kRmv',
	ID_EDITKEYMAP_ASSIGN			= 'kAsn',
	ID_EDITKEYMAP_NAME_MODIFIED		= 'kNmM',
	ID_EDITKEYMAP_FUNCTION_SELECTED	= 'kFcS',
	ID_EDITKEYMAP_CURRENT_KEY_SELECTED = 'kCkS',
	ID_EDITKEYMAP_KEYINPUT_CHANGED	= 'kKyC',
};

#endif // _COMMANDID_H_
