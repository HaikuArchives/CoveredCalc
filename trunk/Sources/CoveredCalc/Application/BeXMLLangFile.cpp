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

/**
	@file		BeXMLLangFile.cpp
	@brief		Implementation of BeXMLLangFile class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.12.31 created
*/

#include "Prefix.h"
#include "BeXMLLangFile.h"
#include "UTF8Utils.h"
#include "XMLLangFileException.h"
#include "DOMUtils.h"
#include "TypeConv.h"
#include "BeDialogDesign.h"
#include "DialogID.h"
#include "StringID.h"

////////////////////////////////////////
#define base	XMLLangFile
////////////////////////////////////////

UTF8Char	SUITABLE_VERSION[] = "5";

#define DEFINE_ID(name)	case name: return TypeConv::AsUTF8(#name);
static ConstUTF8Str _convertDialogID(SInt32 id)
{
	switch (id)
	{
		DEFINE_ID(IDD_ABOUT)
		DEFINE_ID(IDD_COVER_BROWSER)
		DEFINE_ID(IDD_ABOUT_COVER)
		DEFINE_ID(IDD_PREFERENCES)

	default:
		throw new XMLLangFileExceptions::InvalidDialogID(id);
	}
}

static ConstUTF8Str _convertStringID(SInt32 id)
{
	switch (id)
	{
		DEFINE_ID(IDS_APPNAME)
		DEFINE_ID(IDS_COVER_BROWSER_COLUMN_NAME)
		DEFINE_ID(IDS_COVER_BROWSER_COLUMN_DESCRIPTION)
		DEFINE_ID(IDS_EMSG_INVALID_KEYMAPPINGS)
		DEFINE_ID(IDS_EMSG_GET_KEYMAPPINGS)
		DEFINE_ID(IDS_EMSG_LOAD_KEYMAPPINGS)
		DEFINE_ID(IDS_EMSG_INVALID_LANGUAGE)
		DEFINE_ID(IDS_EMSG_GET_LANGUAGE)
		DEFINE_ID(IDS_EMSG_CREATE_COVER_BROWSER)
		DEFINE_ID(IDS_EMSG_LOAD_SETTING)
		DEFINE_ID(IDS_EMSG_READY_DEFAULT_SETTING_FILE)
		DEFINE_ID(IDS_EMSG_CREATE_MAIN_WINDOW)
		DEFINE_ID(IDS_EMSG_SAVE_SETTING)
		DEFINE_ID(IDS_EMSG_GENERIC)
		DEFINE_ID(IDS_EMSG_XML_LINE_COLUMN)
		DEFINE_ID(IDS_EMSG_XML_PARSE)
		DEFINE_ID(IDS_EMSG_NO_MATCH_START_TAG)
		DEFINE_ID(IDS_EMSG_INVALID_PARENT_ENTITY)
		DEFINE_ID(IDS_EMSG_MISSING_ATTRIBUTE)
		DEFINE_ID(IDS_EMSG_UNKNOWN_TAG)
		DEFINE_ID(IDS_EMSG_COVERDEF_PARSE)
		DEFINE_ID(IDS_EMSG_UNKNOWN_ID)
		DEFINE_ID(IDS_EMSG_UNKNOWN_CLASS)
		DEFINE_ID(IDS_EMSG_UNKNOWN_TYPE)
		DEFINE_ID(IDS_EMSG_VALIDATION_FAILED)
		DEFINE_ID(IDS_EMSG_COLOR_STRING_NOT_UNDERSTOOD)
		DEFINE_ID(IDS_EMSG_UNSUPPORTED_COVERDEF_VERSION)
		DEFINE_ID(IDS_EMSG_INVALID_COVERDEF)
		DEFINE_ID(IDS_EMSG_COLOR_CODED_SKIN)
		DEFINE_ID(IDS_EMSG_INIT_SKIN)
		DEFINE_ID(IDS_EMSG_FILE_OPERATION)
		DEFINE_ID(IDS_EMSG_FILE_FILENAME)
		DEFINE_ID(IDS_EMSG_FILE_ACCESS_DENIED)
		DEFINE_ID(IDS_EMSG_FILE_DEVICE_FULL)
		DEFINE_ID(IDS_EMSG_FILE_ALREADY_EXISTS)
		DEFINE_ID(IDS_EMSG_FILE_NOT_FOUND)
		DEFINE_ID(IDS_EMSG_FILE_IO_ERROR)
		DEFINE_ID(IDS_EMSG_FILE_SHARING_VIOLATION)
		DEFINE_ID(IDS_EMSG_DIBFILE_LOAD)
		DEFINE_ID(IDS_EMSG_DIBFILE_BROKEN_FILE)
		DEFINE_ID(IDS_EMSG_DIBFILE_UNKNOWN_FORMAT)
		DEFINE_ID(IDS_EMSG_MEMORY)
		DEFINE_ID(IDS_EMSG_PATH_OPERATION)
		DEFINE_ID(IDS_EMSG_PATH_MAKE_RELATIVE)
		DEFINE_ID(IDS_EMSG_UI_CONTROL)
		DEFINE_ID(IDS_EMSG_UI_SHOW_MENU)
		DEFINE_ID(IDS_EMSG_UI_SHOW_DIALOG)
		DEFINE_ID(IDS_EMSG_LANG_VALIDATION_FAILED)
		DEFINE_ID(IDS_EMSG_LANG_STRING_NOT_DEFINED)
	
	default:
		throw new XMLLangFileExceptions::InvalidStringID(id);
	}
}

/**
 *	@brief	Constructor.
 */
BeXMLLangFile::BeXMLLangFile()
{
}

/**
 *	@brief	Destructor.
 */
BeXMLLangFile::~BeXMLLangFile()
{
}

/**
 *	@brief		Checks the version is suitable for this implementation.
 *	@param[in]	version		version string.
 *	@return		true if the version is suitable.
 */
bool BeXMLLangFile::checkVersion(ConstUTF8Str version)
{
	return (0 == UTF8Utils::UTF8StrCmpI(SUITABLE_VERSION, version));
}

/**
 *	@brief		Loads string.
 *	@param[in]	name	string name.
 *	@param[out]	message		loaded text is returned.
 */
void BeXMLLangFile::LoadString(ConstUTF8Str name, MBCString& message) const
{
	MBCString innerMessage;
	base::LoadString(name, innerMessage);
	message = LT(innerMessage);
}

/**
 *	@brief		Loads string from string ID.
 *	@param[in]	stringID	string id.
 *	@param[out]	message		loaded text is returned.
 */
void BeXMLLangFile::LoadStringFromID(SInt32 stringID, MBCString& message) const
{
	ConstUTF8Str name = _convertStringID(stringID);
	LoadString(name, message);
}

/**
 *	@brief		Loads dialog design.
 *	@param[in]	dialogID	dialog id.
 *	@return 	loaded design is returned.
 */
BeDialogDesign* BeXMLLangFile::LoadDialogDesign(SInt32 dialogID) const
{
	ConstUTF8Str name = _convertDialogID(dialogID);
	const NCDElement* dialogElem = GetDialogElement(name);
	BeDialogDesign* newDesign = new BeDialogDesign();
	try
	{
		newDesign->Load(dialogElem);
	}
	catch (...)
	{
		delete newDesign;
		throw;
	}
	return newDesign;
}
