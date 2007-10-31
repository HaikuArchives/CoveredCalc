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
	@file		WinKeyMappingManager.cpp
	@brief		WinKeyMappingManager �N���X�̎���
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.24 created
*/

#include "Prefix.h"
#include "WinKeyMappingManager.h"
#include "KeyMappings.h"
#include "KeyFuncOperation.h"
#include "UTF8String.h"

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinKeyMappingManager::WinKeyMappingManager()
{
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinKeyMappingManager::~WinKeyMappingManager()
{
}

/**
 *	@brief	�I�u�W�F�N�g�̍\�z���s���܂��B
 *	@param[in] keyMapping	�L�[�}�b�s���O��`
 *	@param[in] category		�J�e�S���[��
 *	@param[in] keyFuncOperation		key-function ��`����I�u�W�F�N�g
 */
void WinKeyMappingManager::Create(const KeyMappings* keyMappings, ConstUTF8Str category, const KeyFuncOperation* keyFuncOperation)
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
		record.VirtualKeyCode = strtoul(TypeConv::AsASCII(value), NULL, 16);
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
 *	@brief	�}�b�s���O���N���A���܂��B
 */
void WinKeyMappingManager::Clear()
{
	mapping.clear();
}

/**
 *	@brief	�C���L�[�̃}�X�N���������͂��܂��B
 *	@param[in]	modifierMaskStr	�C���L�[�}�X�N������
 *	@return �C���L�[�}�X�N
 */
UInt32 WinKeyMappingManager::analyzeModifierMask(ConstUTF8Str modifierMaskStr)
{
	ConstAStr maskStr = TypeConv::AsASCII(modifierMaskStr);
	UInt32	mask = WinKeyEventParameter::ModifierMask_None;
	
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
			mask |= WinKeyEventParameter::ModifierMask_Shift;
		}
		else if (4 == length && 0 == memcmp(maskStr, "ctrl", 4))
		{
			mask |= WinKeyEventParameter::ModifierMask_Ctrl;
		}
		else if (3 == length && 0 == memcmp(maskStr, "alt", 4))
		{
			mask |= WinKeyEventParameter::ModifierMask_Alt;
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
 * @brief	�w�肵���L�[�{�[�h�p�����[�^�ɑΉ�����@�\���擾���܂��B
 * @return �Ή�����@�\�ԍ�
 */
SInt32 WinKeyMappingManager::GetFunction(
	const KeyEventParameter& parameter		//!< �L�[�{�[�h�p�����[�^
) const
{
	DWORD virtualKeyCode = parameter.GetVirtualKeyCode();
	UInt32 modifierMask = parameter.GetModifierMask();

	SInt32 length = mapping.size();
	SInt32 index;
	for (index = 0; index < length; index++)
	{
		if (mapping[index].VirtualKeyCode == virtualKeyCode &&
			mapping[index].ModifierMask == modifierMask)
		{
			return mapping[index].Function;
		}
	}
	return KeyFuncOperation::KeyFunc_None;
}
