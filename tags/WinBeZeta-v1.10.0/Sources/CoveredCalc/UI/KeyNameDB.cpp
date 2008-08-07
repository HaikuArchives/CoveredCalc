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
	@file		KeyNameDB.cpp
	@brief		Implementation of KeyNameDB class.
	@author 	ICHIMIYA Hironori (Hiron)
	@date		2008.2.4 created
*/

#include "Prefix.h"
#include "KeyNameDB.h"
#include "Path.h"
#include "File.h"
#include "XMLParser.h"
#include "DOMParseEventHandler.h"
#include "DOMUtils.h"
#include "NCDDocument.h"
#include "NCDElement.h"
#include "UTF8Conv.h"
#include "CoveredCalcApp.h"
#if defined(BEOS)
#include <Font.h>
#endif

#if defined(WIN32)
static const AChar STR_MODIFIER_CTRL[]				= "Ctrl+";
static const AChar STR_MODIFIER_SHIFT[]				= "Shift+";
static const AChar STR_MODIFIER_ALT[]				= "Alt+";
#elif defined(BEOS)
static const AChar STR_MODIFIER_COMMAND[]			= "Command-";
static const AChar STR_MODIFIER_SHIFT[]				= "Shift-";
static const AChar STR_MODIFIER_OPTION[]			= "Option-";
static const AChar STR_MODIFIER_CONTROL[]			= "Control-";
#endif

static const UTF8Char STR_ELEMENT_KEY_NAMES[]		= "keyNames";
static const UTF8Char STR_ELEMENT_PLATFORM[]		= "platform";
static const UTF8Char STR_ELEMENT_NAME_DEF[]		= "nameDef";
static const UTF8Char STR_ATTR_VERSION[]			= "version";
static const UTF8Char STR_ATTR_TARGET[]				= "target";
static const UTF8Char STR_ATTR_CODE[]				= "code";

static const SInt32 Version_Current					= 1;

/**
 * @ brief Constructor.
 */
KeyNameDB::KeyNameDB()
{
#if defined (BEOS)
	mapKeys = NULL;
	mapChars = NULL;
#endif
}

/**
 * @brief Destructor.
 */
KeyNameDB::~KeyNameDB()
{
#if defined (BEOS)
	if (NULL != mapKeys)
	{
		free(mapKeys);
	}
	if (NULL != mapChars)
	{
		free(mapChars);
	}
#endif
}

/**
 * @brief Initializes this object.
 * @param[in] keyNameDefFile a path to key name definition file.
 */
void KeyNameDB::Init(const Path& keyNameDefFile)
{
	if (!keyNameDefFile.IsEmpty())
	{
		// read key name definition file.
		File file;
		file.Open(keyNameDefFile, FileConstants::OpenMode_ReadOnly, FileConstants::OpenOption_None);
		UInt32 dataSize = file.GetSize();
		XMLParser parser;
		parser.Create();
		Byte* buf = static_cast<Byte*>(parser.GetParseBuffer(dataSize));
		file.Read(buf, dataSize);
		file.Close();
		
		DOMParseEventHandler handler;
		parser.SetEventHandler(&handler);
		parser.ParseBuffer(dataSize, true);
		
		NCDDocument* document = handler.DetachDocument();
		if (document != NULL)
		{
			loadKeyNameDef(document);
			document->release();
		}
	}
}

/**
 * @brief Loads key name definitions from DOM tree.
 * @param[in] document DOM tree.
 */
void KeyNameDB::loadKeyNameDef(NCDDocument* document)
{
	// check apex element
	NCDElement* apexElem = document->getDocumentElement();
	if (NULL == apexElem)
	{
		return;
	}
	UTF8String name;
	apexElem->getTagName(name);
	if (0 != name.Compare(STR_ELEMENT_KEY_NAMES))
	{
		return;
	}
	UTF8String version;
	apexElem->getAttribute(STR_ATTR_VERSION, version);
	SInt32 numVersion = atoi(TypeConv::AsASCII(version));
	if (0 == numVersion || Version_Current < numVersion)
	{
		return;
	}
	
	// check platform target.
	bool platformOK = false;
	NCDElement* platformElement = DOMUtils::SearchElementNext(apexElem->getFirstChild(), TypeConv::AsUTF8("platform"), false);
	while (NULL != platformElement)
	{
		UTF8String targetString;
		platformElement->getAttribute(TypeConv::AsUTF8("target"), targetString);
		if (CoveredCalcApp::GetInstance()->CheckPlatform(targetString))
		{
			platformOK = true;
			break;
		}
		platformElement = DOMUtils::SearchElementNext(platformElement->getNextSibling(), TypeConv::AsUTF8("platform"), false);
	}
	if (!platformOK)
	{
		return;
	}
	
	// append to cache
	UTF8String codeUtf8;
	UTF8String nameUtf8;
	MBCString nameMbc;
	NCDElement* nameDefElem = DOMUtils::SearchElementNext(apexElem->getFirstChild(), STR_ELEMENT_NAME_DEF, false);
	while (NULL != nameDefElem)
	{
		nameDefElem->getAttribute(STR_ATTR_CODE, codeUtf8);
		KeyEventParameter::KeyCode keyCode = strtoul(TypeConv::AsASCII(codeUtf8), NULL, 16);
		DOMUtils::ReadTextNode(nameDefElem, false, nameUtf8);
		UTF8Conv::ToMultiByte(nameMbc, nameUtf8);
		keyNameCache[keyCode] = nameMbc;
		
		nameDefElem = DOMUtils::SearchElementNext(nameDefElem->getNextSibling(), STR_ELEMENT_NAME_DEF, false);
	}
}

/**
 * @brief Retrieves a name of key from keycode.
 * @param[in] keyCode keycode.
 * @param[out] keyName key name is returned.
 */
void KeyNameDB::GetKeyName(KeyEventParameter::KeyCode keyCode, MBCString& keyName) const
{
	KeyNameMap::iterator it = keyNameCache.find(keyCode);
	if (it != keyNameCache.end())	// found
	{
		keyName = (*it).second;
	}
	else
	{
		generateKeyName(keyCode, keyName);
		keyNameCache[keyCode] = keyName;
	}
}

/**
 * @brief Generates a name of key from keycode.
 * @param[in] keyCode keycode.
 * @param[out] keyName generated key name is returned.
 */
void KeyNameDB::generateKeyName(KeyEventParameter::KeyCode keyCode, MBCString& keyName) const
{
	AChar buf[128];
	
#if defined(WIN32)
	UINT scanCode = MapVirtualKey(keyCode, 0 /* MAPVK_VK_TO_VSC */);
	if (0 != scanCode)
	{
		if (0 != GetKeyNameText(scanCode << 16, buf, sizeof(buf)/sizeof(AChar)))
		{
			keyName = buf;
			return;
		}
	}
#elif defined(BEOS)
	if (128 > keyCode)
	{
		if (NULL == mapKeys)
		{
			get_key_map(&mapKeys, &mapChars);
		}
		
		SInt32 numChars = 0;
		char* pos = mapChars + mapKeys->normal_map[keyCode];
		SInt32 len = *pos++;
		if (0 < len)
		{
			if (1 == len)
			{
				buf[0] = (isalpha(*pos)) ? (toupper(*pos)) : (*pos);
				buf[1] = '\0';
				numChars = 1;
			}
			else
			{
				SInt32 ix;
				for (ix = 0; ix < len; ix++)
				{
					buf[ix] = *pos++;
					if ((buf[ix] & 0xc0) != 0x80)
					{
						numChars++;
					}
				}
				buf[ix] = '\0';
			}
			
			// check if be_plain_font can display the key name.
			bool hasArray[128];
			be_plain_font->GetHasGlyphs(buf, numChars, hasArray);
			bool canDisplay = true;
			SInt32 ix;
			for (ix = 0; ix < numChars; ix++)
			{
				if (!hasArray[ix])
				{
					canDisplay = false;
					break;
				}
			}
			
			if (canDisplay)
			{
				keyName = buf;
				return;
			}
		}
	}	
#endif
	
	snprintf(buf, sizeof(buf)/sizeof(AChar), "(Keycode-%02lX)", keyCode);
	keyName = buf;
}

/**
 * @brief Generates a name of key from KeyEventParameter.
 * @param[in] keyEventParam KeyEventParameter.
 * @param[out] keyName generated key name is returned.
 */
void KeyNameDB::GetKeyName(const KeyEventParameter* keyEventParam, MBCString& keyName) const
{
	MBCString noModifierKeyName;
	GetKeyName(keyEventParam->GetKeyCode(), noModifierKeyName);

	UInt32 modifiers = keyEventParam->GetModifiers();
	keyName.Empty();
	
#if defined(WIN32)
	if (modifiers & KeyEventParameter::ModifierMask_Ctrl)
	{
		keyName += STR_MODIFIER_CTRL;
	}
	if (modifiers & KeyEventParameter::ModifierMask_Shift)
	{
		keyName += STR_MODIFIER_SHIFT;
	}
	if (modifiers & KeyEventParameter::ModifierMask_Alt)
	{
		keyName += STR_MODIFIER_ALT;
	}
#elif defined(BEOS)
	if (modifiers & KeyEventParameter::ModifierMask_Command)
	{
		keyName += STR_MODIFIER_COMMAND;
	}
	if (modifiers & KeyEventParameter::ModifierMask_Shift)
	{
		keyName += STR_MODIFIER_SHIFT;
	}
	if (modifiers & KeyEventParameter::ModifierMask_Option)
	{
		keyName += STR_MODIFIER_OPTION;
	}
	if (modifiers & KeyEventParameter::ModifierMask_Control)
	{
		keyName += STR_MODIFIER_CONTROL;
	}
#endif

	keyName += noModifierKeyName;
}
