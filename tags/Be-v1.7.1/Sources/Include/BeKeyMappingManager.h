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
	@file		BeKeyMappingManager.h
	@brief		Definition of BeKeyMappingManager class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.08.29 created
*/

#ifndef _BEKEYMAPPINGMANAGER_H_
#define _BEKEYMAPPINGMANAGER_H_

#include "KeyEventParameter.h"

// ---------------------------------------------------------------------
//! キーマッピング管理クラス for BeOS
// ---------------------------------------------------------------------
class BeKeyMappingManager
{
public:
						BeKeyMappingManager();
	virtual				~BeKeyMappingManager();
	
	virtual SInt32		GetFunction(const KeyEventParameter& parameter) = 0;
};

// TODO: 下に書いてあるとおり。キーカスタマイズを実現するときにはこのクラスをなくす。
// ---------------------------------------------------------------------
//! メインウィンドウ用キーマッピング管理クラス for BeOS
/*!
	@note
	将来のキーマッピングのカスタマイズ対応ができるようになれば、
	BeKeyMappingManager がキーと機能の動的な対応関係を管理するはず。
	そうなると、GetFunction は virtual の必要もなくなり、
	メインウィンドウに関わらず、各ウィンドウ用の対応関係の管理を
	BeKeyMappingManager が行えるようになる。（ウィンドウ 1 つに対し、
	BeKeyMappingManager のインスタンスを 1 つ作るが、それらは BeKeyMappingManager
	の派生クラスではなく、BeKeyMappingManager そのものでよいはず）
	そのとき、このクラスは必要なくなるだろう。
	現在は動的な対応関係の管理を実装していないので
	それまでのつなぎとして作ってあるクラス。
*/
// ---------------------------------------------------------------------
class BeMainWindowKeyMappingManager : public BeKeyMappingManager
{
public:
						BeMainWindowKeyMappingManager() { }
	virtual				~BeMainWindowKeyMappingManager() { }

	virtual SInt32		GetFunction(const KeyEventParameter& parameter);
};

#endif // _BEKEYMAPPINGMANAGER_H_
