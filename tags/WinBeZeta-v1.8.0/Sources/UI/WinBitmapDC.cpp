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
	@file		WinBitmapDC.cpp
	@brief		Implementation of WinBitmapDC class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.10.11 created
*/

#include "Prefix.h"
#include "WinBitmapDC.h"
#include "WinBitmapDCException.h"

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinBitmapDC::WinBitmapDC()
{
	bitmap = NULL;
	oldBitmap = NULL;
	memDC = NULL;
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinBitmapDC::~WinBitmapDC()
{
	WinBitmapDC::Destroy();	// �h����̂��Ă΂Ȃ��悤��
}

// ---------------------------------------------------------------------
//! �r�b�g�}�b�vDC���\�z���܂��B
/*!
	@throw WinBitmapDCException �֐������s�����Ƃ�
*/
// ---------------------------------------------------------------------
void WinBitmapDC::Create(
	HDC dc,			//!< �쐬�������r�b�g�}�b�vDC�ƌ݊����̂���DC
	SInt32 width,	//!< �r�b�g�}�b�v�̕�
	SInt32 height	//!< �r�b�g�}�b�v�̍���
)
{
	if (IsCreated())
	{
		throw new WinBitmapDCExceptions::AlreadyCreated();
	}
	
	HDC memDC;
	HBITMAP bitmap;
	
	// �w�肳�ꂽDC�ƌ݊����̂��郁����DC���쐬
	memDC = ::CreateCompatibleDC(dc);
	if (NULL == memDC)
	{
		throw new WinBitmapDCException();
	}
	
	try
	{
		// �w�肳�ꂽDC�ƌ݊����̂���r�b�g�}�b�v���쐬
		bitmap = ::CreateCompatibleBitmap(dc, width, height);
		if (NULL == bitmap)
		{
			throw new WinBitmapDCException();
		}		
		
		try
		{
			// ������DC �Ƀr�b�g�}�b�v�����т��Ă���
			this->oldBitmap = static_cast<HBITMAP>(::SelectObject(memDC, bitmap));
			if (NULL == this->oldBitmap)
			{
				throw new WinBitmapDCException();
			}
		}
		catch (...)
		{
			::DeleteObject(bitmap);
			bitmap = NULL;
			throw;
		}
	}
	catch (...)
	{
		::DeleteDC(memDC);
		memDC = NULL;
		throw;
	}

	this->memDC = memDC;
	this->bitmap = bitmap;
}

// ---------------------------------------------------------------------
//! �r�b�g�}�b�vDC��j�����܂��B
// ---------------------------------------------------------------------
void WinBitmapDC::Destroy()
{
	if (NULL != memDC)
	{
		if (NULL != oldBitmap)
		{
			::SelectObject(memDC, oldBitmap);
			oldBitmap = NULL;
		}
		::DeleteDC(memDC);
		memDC = NULL;
	}
	
	if (NULL != bitmap)
	{
		::DeleteObject(bitmap);
		bitmap = NULL;
	}
}

// ---------------------------------------------------------------------
//! HDC�ւ̃L���X�g���Z�q
/*!
	@return �f�o�C�X�R���e�L�X�g�n���h��
	@note
	����ɂ���Ď擾����DC�ɕʂ̃r�b�g�}�b�v��
	SelectObject�����肵�Ă͂����܂���B
*/
// ---------------------------------------------------------------------
WinBitmapDC::operator HDC() const
{
	return memDC;
}
