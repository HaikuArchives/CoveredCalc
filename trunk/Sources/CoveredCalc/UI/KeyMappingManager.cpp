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
		record.KeyCode = strtoul(TypeConv::AsASCII(value), NULL, 16);
		if (!item->GetModifiers(value))
		{
			continue;
		}
		record.Modifiers = analyzeModifierMask(value);
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

/**
 *	@brief	Analyzes a string that shows modifier-key masks.
 *	@param[in]	modifierMaskStr	modifier-key masks string.
 *	@return modifier masks.
 */
UInt32 KeyMappingManager::analyzeModifierMask(ConstUTF8Str modifierMaskStr)
{
	ConstAStr maskStr = TypeConv::AsASCII(modifierMaskStr);
	UInt32	mask = KeyEventParameter::ModifierMask_None;
	
	while ('\0' != maskStr[0])
	{
		SInt32 length;
		ConstAStr separator = strchr(maskStr, '+');
		if (NULL == separator)
		{
			length = strlen(maskStr);
		}
		else
		{
			length = separator - maskStr;
		}
#if defined (WIN32)
		if (5 == length && 0 == memcmp(maskStr, "shift", 5))
		{
			mask |= KeyEventParameter::ModifierMask_Shift;
		}
		else if (4 == length && 0 == memcmp(maskStr, "ctrl", 4))
		{
			mask |= KeyEventParameter::ModifierMask_Ctrl;
		}
		else if (3 == length && 0 == memcmp(maskStr, "alt", 4))
		{
			mask |= KeyEventParameter::ModifierMask_Alt;
		}
#elif defined (BEOS)
		if (5 == length && 0 == memcmp(maskStr, "shift", 5))
		{
			mask |= KeyEventParameter::ModifierMask_Shift;
		}
		else if (7 == length && 0 == memcmp(maskStr, "command", 7))
		{
			mask |= KeyEventParameter::ModifierMask_Command;
		}
		else if (7 == length && 0 == memcmp(maskStr, "control", 7))
		{
			mask |= KeyEventParameter::ModifierMask_Control;
		}
		else if (6 == length && 0 == memcmp(maskStr, "option", 6))
		{
			mask |= KeyEventParameter::ModifierMask_Option;
		}
#endif
		
		if (NULL == separator)
		{
			maskStr += length;
		}
		else
		{
			maskStr += length + 1;
		}
	}
	
	return mask;
}


/**
 * @brief	Retrieves a function from a keyboard parameter.
 * @param parameter[in] keyboard parameter.
 * @return function ID.
 */
SInt32 KeyMappingManager::GetFunction(const KeyEventParameter& parameter) const
{
	KeyEventParameter::KeyCode keyCode = parameter.GetKeyCode();
	UInt32 modifiers = parameter.GetModifiers() & KeyEventParameter::ModifierMask_AllMask;

	SInt32 length = mapping.size();
	SInt32 index;
	for (index = 0; index < length; index++)
	{
		if (mapping[index].KeyCode == keyCode &&
			mapping[index].Modifiers == modifiers)
		{
			return mapping[index].Function;
		}
	}
	return KeyFuncOperation::KeyFunc_None;
}
