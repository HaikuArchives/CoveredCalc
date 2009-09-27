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
	@file		DialogID.h
	@brief		Definition of dialog ID
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.01.07 Created
*/

#ifdef IMPLEMENT_DIALOGID_CONVERSION
#undef _DIALOGID_H_
#endif

#ifndef _DIALOGID_H_
#define _DIALOGID_H_

ConstUTF8Str ConvertDialogID(SInt32 id);

#ifdef IMPLEMENT_DIALOGID_CONVERSION
	#define DIALOG_ID(name, num)	case num: return TypeConv::AsUTF8(#name);
	ConstUTF8Str ConvertDialogID(SInt32 id)
	{
		switch (id)
		{
#else
	#define DIALOG_ID(name, num)	name = num,
	enum
	{
#endif

	DIALOG_ID(IDD_ABOUT,			5000)
	DIALOG_ID(IDD_COVER_BROWSER,	5001)
	DIALOG_ID(IDD_ABOUT_COVER,		5002)
	DIALOG_ID(IDD_PREFERENCES,		5003)
	DIALOG_ID(IDD_EDIT_KEYMAP,		5004)
	DIALOG_ID(IDD_SELECT_LANGUAGE,	5005)


#undef DIALOG_ID
#ifdef IMPLEMENT_DIALOGID_CONVERSION
		default:
			throw new XMLLangFileExceptions::InvalidDialogID(id);
		}
	}
#else
	};
#endif

#endif // _DIALOGID_H_
