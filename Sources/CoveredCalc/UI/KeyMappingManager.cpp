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
	@file		KeyMappingManager.cpp
	@brief		implementation of KeyMappingManager class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.2.3 created
*/

#include "Prefix.h"
#include "KeyMappingManager.h"
#include "KeyMappings.h"
#include "KeyFuncOperation.h"
#include "UTF8String.h"

static const UTF8Char STR_MODIFIER_NONE[] = "none";
#if defined (WIN32)
static const UTF8Char STR_MODIFIER_SHIFT[] = "shift";
static const SInt32 MODIFIER_SHIFT_LENGTH = sizeof(STR_MODIFIER_SHIFT)/sizeof(AChar) - 1;
static const UTF8Char STR_MODIFIER_CTRL[] = "ctrl";
static const SInt32 MODIFIER_CTRL_LENGTH = sizeof(STR_MODIFIER_CTRL)/sizeof(AChar) - 1;
static const UTF8Char STR_MODIFIER_ALT[] = "alt";
static const SInt32 MODIFIER_ALT_LENGTH = sizeof(STR_MODIFIER_ALT)/sizeof(AChar) - 1;
static const AChar CHAR_MODIFIER_SEPARATOR = '+';
#elif defined (BEOS)
static const UTF8Char STR_MODIFIER_SHIFT[] = "shift";
static const SInt32 MODIFIER_SHIFT_LENGTH = sizeof(STR_MODIFIER_SHIFT)/sizeof(AChar) - 1;
static const UTF8Char STR_MODIFIER_COMMAND[] = "command";
static const SInt32 MODIFIER_COMMAND_LENGTH = sizeof(STR_MODIFIER_COMMAND)/sizeof(AChar) - 1;
static const UTF8Char STR_MODIFIER_CONTROL[] = "control";
static const SInt32 MODIFIER_CONTROL_LENGTH = sizeof(STR_MODIFIER_CONTROL)/sizeof(AChar) - 1;
static const UTF8Char STR_MODIFIER_OPTION[] = "option";
static const SInt32 MODIFIER_OPTION_LENGTH = sizeof(STR_MODIFIER_OPTION)/sizeof(AChar) - 1;
static const AChar CHAR_MODIFIER_SEPARATOR = '-';
#endif

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
KeyMappingManager::KeyMappingManager()
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
KeyMappingManager::~KeyMappingManager()
{
}

/**
 *	@brief	Creates an object.
 *	@param[in] keyMapping	key-mapping definition.
 *	@param[in] category		category name
 *	@param[in] keyFuncOperation		KeyFuncOperation object
 */
void KeyMappingManager::Create(const KeyMappings* keyMappings, ConstUTF8Str category, const KeyFuncOperation* keyFuncOperation)
{
	Clear();
	
	KeyMappings::KeyMappingItemEnumerator* enumerator = keyMappings->NewItemEnumeratorFor(category);
	while (enumerator->MoveNext())
	{
		KeyMappingItem* item = enumerator->GetCurrent();
		
		KMRecord record;
		UTF8String value;
		if (!item->GetKeyCode(value))
		{
			continue;
		}
		record.KeyEventParam.SetKeyCode(strtoul(TypeConv::AsASCII(value), NULL, 16));
		if (!item->GetModifiers(value))
		{
			continue;
		}
		record.KeyEventParam.SetModifiers(analyzeModifierMask(value));
		if (!item->GetFunction(value))
		{
			continue;
		}
		record.Function = keyFuncOperation->FuncNameToKeyFunc(value);

		mapping.push_back(record);
	}
	delete enumerator;
}

/**
 *	@brief	Clears all mappings.
 */
void KeyMappingManager::Clear()
{
	mapping.clear();
}

class KMRecordItem : public KeyMappingItem
{
private:
	const UTF8String*			keyCodePtr;
	const UTF8String*			modifiersPtr;
	const UTF8String*			functionPtr;
	
public:
	KMRecordItem(const UTF8String& keyCode, const UTF8String& modifiers, const UTF8String& function)
	{
		keyCodePtr = &keyCode;
		modifiersPtr = &modifiers;
		functionPtr = &function;
	}
	virtual ~KMRecordItem() {}
	
	virtual bool GetKeyCode(UTF8String& keyCode) const
	{
		keyCode = *keyCodePtr;
		return true;
	}
	
	virtual bool GetModifiers(UTF8String& modifiers) const
	{
		modifiers = *modifiersPtr;
		return true;
	}

	virtual bool GetFunction(UTF8String& function) const
	{
		function = *functionPtr;
		return true;
	}
};

void KeyMappingManager::WriteOut(KeyMappings* keyMappings, ConstUTF8Str category, const KeyFuncOperation* keyFuncOperation)
{
	keyMappings->RemoveAllItem(category);

	UTF8String keyCode;
	UTF8String modifiers;
	UTF8String function;

	SInt32 length = mapping.size();
	SInt32 index;
	for (index = 0; index < length; index++)
	{
		UTF8Char buf[16];
		sprintf(TypeConv::AsASCII(buf), "%02lX", mapping[index].KeyEventParam.GetKeyCode());
		keyCode = buf;
		makeModifierMask(mapping[index].KeyEventParam.GetModifiers(), modifiers);
		keyFuncOperation->KeyFuncToFuncName(mapping[index].Function, function);
		KMRecordItem item(keyCode, modifiers, function);
		keyMappings->AddItem(category, &item);
	}	
}

/**
 *	@brief	Analyzes a string that shows modifier-key masks.
 *	@param[in]	modifierMaskStr	modifier-key masks string.
 *	@return modifier masks.
 */
UInt32 KeyMappingManager::analyzeModifierMask(ConstUTF8Str modifierMaskStr)
{
	UInt32	mask = KeyEventParameter::ModifierMask_None;
	
	while ('\0' != modifierMaskStr[0])
	{
		SInt32 length;
		ConstUTF8Str separator = TypeConv::AsUTF8(strchr(TypeConv::AsASCII(modifierMaskStr), CHAR_MODIFIER_SEPARATOR));
		if (NULL == separator)
		{
			length = strlen(TypeConv::AsASCII(modifierMaskStr));
		}
		else
		{
			length = separator - modifierMaskStr;
		}
#if defined (WIN32)
		if (MODIFIER_SHIFT_LENGTH == length && 0 == memcmp(modifierMaskStr, STR_MODIFIER_SHIFT, MODIFIER_SHIFT_LENGTH))
		{
			mask |= KeyEventParameter::ModifierMask_Shift;
		}
		else if (MODIFIER_CTRL_LENGTH == length && 0 == memcmp(modifierMaskStr, STR_MODIFIER_CTRL, MODIFIER_CTRL_LENGTH))
		{
			mask |= KeyEventParameter::ModifierMask_Ctrl;
		}
		else if (MODIFIER_ALT_LENGTH == length && 0 == memcmp(modifierMaskStr, STR_MODIFIER_ALT, MODIFIER_ALT_LENGTH))
		{
			mask |= KeyEventParameter::ModifierMask_Alt;
		}
#elif defined (BEOS)
		if (MODIFIER_SHIFT_LENGTH == length && 0 == memcmp(modifierMaskStr, STR_MODIFIER_SHIFT, MODIFIER_SHIFT_LENGTH))
		{
			mask |= KeyEventParameter::ModifierMask_Shift;
		}
		else if (MODIFIER_COMMAND_LENGTH == length && 0 == memcmp(modifierMaskStr, STR_MODIFIER_COMMAND, MODIFIER_COMMAND_LENGTH))
		{
			mask |= KeyEventParameter::ModifierMask_Command;
		}
		else if (MODIFIER_CONTROL_LENGTH == length && 0 == memcmp(modifierMaskStr, STR_MODIFIER_CONTROL, MODIFIER_CONTROL_LENGTH))
		{
			mask |= KeyEventParameter::ModifierMask_Control;
		}
		else if (MODIFIER_OPTION_LENGTH == length && 0 == memcmp(modifierMaskStr, STR_MODIFIER_OPTION, MODIFIER_OPTION_LENGTH))
		{
			mask |= KeyEventParameter::ModifierMask_Option;
		}
#endif
		
		if (NULL == separator)
		{
			modifierMaskStr += length;
		}
		else
		{
			modifierMaskStr += length + 1;
		}
	}
	
	return mask;
}

/**
 *	@brief	Creates a string that shows modifier-key masks.
 *	@param[in]	modifierMask	modifier masks
 *	@param[out]	modifierMaskStr	modifier-key masks string is returned.
 */
void KeyMappingManager::makeModifierMask(UInt32 modifierMask, UTF8String& modifierMaskStr)
{
	modifierMaskStr.Empty();
	
#if defined(WIN32)
	if (modifierMask & KeyEventParameter::ModifierMask_Ctrl)
	{
		if (!modifierMaskStr.IsEmpty())
		{
			modifierMaskStr += TypeConv::AsUTF8(CHAR_MODIFIER_SEPARATOR);
		}
		modifierMaskStr += STR_MODIFIER_CTRL;
	}
	if (modifierMask & KeyEventParameter::ModifierMask_Shift)
	{
		if (!modifierMaskStr.IsEmpty())
		{
			modifierMaskStr += TypeConv::AsUTF8(CHAR_MODIFIER_SEPARATOR);
		}
		modifierMaskStr += STR_MODIFIER_SHIFT;
	}
	if (modifierMask & KeyEventParameter::ModifierMask_Alt)
	{
		if (!modifierMaskStr.IsEmpty())
		{
			modifierMaskStr += TypeConv::AsUTF8(CHAR_MODIFIER_SEPARATOR);
		}
		modifierMaskStr += STR_MODIFIER_ALT;
	}
#elif defined (BEOS)
	if (modifierMask & KeyEventParameter::ModifierMask_Shift)
	{
		if (!modifierMaskStr.IsEmpty())
		{
			modifierMaskStr += TypeConv::AsUTF8(CHAR_MODIFIER_SEPARATOR);
		}
		modifierMaskStr += STR_MODIFIER_SHIFT;
	}
	if (modifierMask & KeyEventParameter::ModifierMask_Command)
	{
		if (!modifierMaskStr.IsEmpty())
		{
			modifierMaskStr += TypeConv::AsUTF8(CHAR_MODIFIER_SEPARATOR);
		}
		modifierMaskStr += STR_MODIFIER_COMMAND;
	}
	if (modifierMask & KeyEventParameter::ModifierMask_Control)
	{
		if (!modifierMaskStr.IsEmpty())
		{
			modifierMaskStr += TypeConv::AsUTF8(CHAR_MODIFIER_SEPARATOR);
		}
		modifierMaskStr += STR_MODIFIER_CONTROL;
	}
	if (modifierMask & KeyEventParameter::ModifierMask_Option)
	{
		if (!modifierMaskStr.IsEmpty())
		{
			modifierMaskStr += TypeConv::AsUTF8(CHAR_MODIFIER_SEPARATOR);
		}
		modifierMaskStr += STR_MODIFIER_OPTION;
	}
#endif

	if (modifierMaskStr.IsEmpty())
	{
		modifierMaskStr = STR_MODIFIER_NONE;
	}
}

/**
 * @brief	Retrieves a function from a keyboard parameter.
 * @param parameter[in] keyboard parameter.
 * @return function ID.
 */
SInt32 KeyMappingManager::GetFunction(const KeyEventParameter& parameter) const
{
	SInt32 length = mapping.size();
	SInt32 index;
	for (index = 0; index < length; index++)
	{
		if (mapping[index].KeyEventParam.IsSame(parameter))
		{
			return mapping[index].Function;
		}
	}
	return KeyFuncOperation::KeyFunc_None;
}

/**
 * @brief	Adds a key to specified function.
 * @param[in]	function	the function which key is assigned to.
 * @param[in]	parameter	key.
 */
void KeyMappingManager::AddKey(SInt32 function, const KeyEventParameter& parameter)
{
	KMRecord record;
	record.Function = function;
	record.KeyEventParam = parameter;
	mapping.push_back(record);
}

/**
 * @brief	Removes a key from specified function.
 * @param[in]	fuinction	the function which key was assigned to.
 * @param[in]	parameter	key.
 */
void KeyMappingManager::RemoveKey(SInt32 function, const KeyEventParameter& parameter)
{
	SInt32 length = mapping.size();
	SInt32 index;
	for (index = length - 1; index >= 0; index--)
	{
		if (mapping[index].Function == function &&
				mapping[index].KeyEventParam.IsSame(parameter))
		{
			mapping.erase(mapping.begin() + index);
		}
	}
}

/**
 * @brief	Iterates each key assigned to specified function.
 * @param[in] function
 * @param[in] visitor
 */
bool KeyMappingManager::ForEachKey(SInt32 function, KeyMappingManager::KeyVisitor* visitor) const
{
	SInt32 length = mapping.size();
	SInt32 index;
	for (index = 0; index < length; index++)
	{
		if (mapping[index].Function == function)
		{
			if (!visitor->Visit(mapping[index].KeyEventParam))
			{
				return false;
			}
		}
	}
	return true;
}
