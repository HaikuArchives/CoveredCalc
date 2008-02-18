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
	@file		KeyNameDB.h
	@brief		Definition of KeyNameDB class.
	@author 	ICHIMIYA Hironori (Hiron)
	@date		2008.2.4 created
*/

#ifndef _KEYNAMEDB_H_
#define _KEYNAMEDB_H_

#include <map>
#include "KeyEventParameter.h"
#include "MBCString.h"
#if defined(BEOS)
#include <InterfaceDefs.h>
#endif

class Path;
class NCDDocument;

/**
 * @brief This class converts a keycode to its key name.
 */
class KeyNameDB
{
public:
						KeyNameDB();
	virtual				~KeyNameDB();

	void				Init(const Path& keyNameDefFile);
	
	void				GetKeyName(KeyEventParameter::KeyCode keyCode, MBCString& keyName);

private:
	typedef std::map<KeyEventParameter::KeyCode, MBCString> KeyNameMap;

private:
	void				loadKeyNameDef(NCDDocument* document);
	void				generateKeyName(KeyEventParameter::KeyCode keyCode, MBCString& keyName);

private:
	KeyNameMap			keyNameCache;
#if defined(BEOS)
	key_map*			mapKeys;		///< copy of the system keymap (key_map structure)
	char*				mapChars;		///< copy of the system keymap (character data)
#endif
};

#endif // _KEYNAMEDB_H_
