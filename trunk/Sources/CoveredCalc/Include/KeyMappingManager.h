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
	@file		KeyMappingManager.h
	@brief		Definition of KeyMappingManager class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.24 created
*/

#ifndef _KEYMAPPINGMANAGER_H_
#define _KEYMAPPINGMANAGER_H_

#include <vector>
#include "KeyEventParameter.h"

class KeyMappings;
class KeyFuncOperation;

/**
 * @brief This class holds key-mapping settings.
 */
class KeyMappingManager
{
public:
						KeyMappingManager();
	virtual				~KeyMappingManager();
	
	void				Create(const KeyMappings* keyMappings, ConstUTF8Str category, const KeyFuncOperation* keyFuncOperation);
	void				Clear();
	
	SInt32				GetFunction(const KeyEventParameter& parameter) const;

private:
	UInt32				analyzeModifierMask(ConstUTF8Str modifierMaskStr);
	
private:
	struct KMRecord
	{
		KeyEventParameter::KeyCode	KeyCode;
		UInt32						Modifiers;
		SInt32						Function;
	};
	typedef std::vector<KMRecord>	KMVector;
	
	KMVector			mapping;
};

#endif // _KEYMAPPINGMANAGER_H_
