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
	@file		BeKeyMappingManager.cpp
	@brief		Implementation of BeKeyMappingManager class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.08.29 created
*/

#include "Prefix.h"
#include "BeKeyMappingManager.h"
#include <InterfaceDefs.h>
#include "KeyMappings.h"
#include "KeyFuncOperation.h"
#include "UTF8String.h"

// ---------------------------------------------------------------------
//! コンストラクタ
// ---------------------------------------------------------------------
BeKeyMappingManager::BeKeyMappingManager()
{
}

// ---------------------------------------------------------------------
//! デストラクタ
// ---------------------------------------------------------------------
BeKeyMappingManager::~BeKeyMappingManager()
{
}

/**
 *	@brief	オブジェクトの構築を行います。
 *	@param[in] keyMapping	キーマッピング定義
 *	@param[in] category		カテゴリー名
 *	@param[in] keyFuncOperation		key-function 定義操作オブジェクト
 */
void BeKeyMappingManager::Create(const KeyMappings* keyMappings, ConstUTF8Str category, const KeyFuncOperation* keyFuncOperation)
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
		record.ModifierMask = analyzeModifierMask(value);
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
 *	@brief	マッピングをクリアします。
 */
void BeKeyMappingManager::Clear()
{
	mapping.clear();
}

/**
 *	@brief	修飾キーのマスク文字列を解析します。
 *	@param[in]	modifierMaskStr	修飾キーマスク文字列
 *	@return 修飾キーマスク
 */
int32 BeKeyMappingManager::analyzeModifierMask(ConstUTF8Str modifierMaskStr)
{
	ConstAStr maskStr = TypeConv::AsASCII(modifierMaskStr);
	int32	mask = 0;
	
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
		if (5 == length && 0 == memcmp(maskStr, "shift", 5))
		{
			mask |= B_SHIFT_KEY;
		}
		else if (7 == length && 0 == memcmp(maskStr, "command", 7))
		{
			mask |= B_COMMAND_KEY;
		}
		else if (7 == length && 0 == memcmp(maskStr, "control", 7))
		{
			mask |= B_CONTROL_KEY;
		}
		else if (6 == length && 0 == memcmp(maskStr, "option", 6))
		{
			mask |= B_OPTION_KEY;
		}
		
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
 * @brief	指定したキーボードパラメータに対応する機能を取得します。
 * @return 対応する機能番号
 */
SInt32 BeKeyMappingManager::GetFunction(
	const KeyEventParameter& parameter		//!< キーボードパラメータ
) const
{
	int32 keyCode = parameter.GetKeyCode();
	int32 modifierMask = parameter.GetModifiers() & (B_SHIFT_KEY | B_COMMAND_KEY | B_CONTROL_KEY | B_OPTION_KEY);

	SInt32 length = mapping.size();
	SInt32 index;
	for (index = 0; index < length; index++)
	{
		if (mapping[index].KeyCode == keyCode &&
			mapping[index].ModifierMask == modifierMask)
		{
			return mapping[index].Function;
		}
	}
	return KeyFuncOperation::KeyFunc_None;
}
