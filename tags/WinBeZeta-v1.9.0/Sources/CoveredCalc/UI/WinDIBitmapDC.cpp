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
	@file		WinDIBitmapDC.cpp
	@brief		Implementation of WinDIBitmapDC class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.10.12 created
*/

#include "Prefix.h"
#include "WinDIBitmapDC.h"
#include "WinBitmapDCException.h"
#include "MemoryException.h"

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinDIBitmapDC::WinDIBitmapDC()
{
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinDIBitmapDC::~WinDIBitmapDC()
{
	WinDIBitmapDC::Destroy();		// �h����̂��Ă΂Ȃ��悤��
}

// ---------------------------------------------------------------------
//! DIB�Ƃ��ăA�N�Z�X�ł���r�b�g�}�b�vDC���\�z���܂��B
/*!
	@throw WinBitmapDCException ���s�����Ƃ�
*/
// ---------------------------------------------------------------------
void WinDIBitmapDC::Create(
	HDC dc,				//!< �쐬�������r�b�g�}�b�vDC�ƌ݊����̂���DC
	SInt32 width,		//!< �r�b�g�}�b�v�̕�
	SInt32 height		//!< �r�b�g�}�b�v�̍���
)
{
	if (IsCreated())
	{
		throw new WinBitmapDCExceptions::AlreadyCreated();
	}
	
	HDC memDC;
	HBITMAP bitmap;
	BITMAPINFO* info = NULL;
	Byte* bits = NULL;
	
	// �w�肳�ꂽDC�ƌ݊����̂��郁����DC���쐬
	memDC = ::CreateCompatibleDC(dc);
	if (NULL == memDC)
	{
		throw new WinBitmapDCException();
	}

	try
	{
		// �w�肳�ꂽDC�ƌ݊��̂���DIB���쐬
		info = createBitmapInfo(width, height);
		try
		{
			bitmap = ::CreateDIBSection(dc, info, DIB_RGB_COLORS, reinterpret_cast<LPVOID*>(&bits), NULL, 0);
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
			free(info);
			info = NULL;
			throw;
		}
	}
	catch (...)
	{
		::DeleteDC(memDC);
		memDC = NULL;
		throw;		
	}

	// DIB������Attach
	dib.Attach(info, bits);
	info = NULL;
	bits = NULL;

	this->memDC = memDC;
	this->bitmap = bitmap;	
}

/**
 *	@brief	BITMAPINFO �𐶐����܂��B�K�� malloc() �Ń������m�ۂ��s���܂��B
 *	@return	�������� BITMAPINFO�B
 */
BITMAPINFO* WinDIBitmapDC::createBitmapInfo(
	SInt32 width,		//!< �r�b�g�}�b�v�̕�
	SInt32 height		//!< �r�b�g�}�b�v�̍���
)
{
	BITMAPINFO*	info = static_cast<BITMAPINFO*>(malloc(sizeof(BITMAPINFO)));
	if (NULL == info)
	{
		MemoryException::Throw();
	}

	::ZeroMemory(info, sizeof(BITMAPINFO));
	info->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	info->bmiHeader.biWidth = width;
	info->bmiHeader.biHeight = height;
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biBitCount = 24;	// 24bpp�Œ�
	info->bmiHeader.biCompression = BI_RGB;
	info->bmiHeader.biSizeImage = 0;
	info->bmiHeader.biXPelsPerMeter = 0;
	info->bmiHeader.biYPelsPerMeter = 0;
	info->bmiHeader.biClrUsed = 0;
	info->bmiHeader.biClrImportant = 0;		

	return info;
}

// ---------------------------------------------------------------------
//! DIB�Ƃ��ăA�N�Z�X�ł���r�b�g�}�b�vDC��j�����܂��B
// ---------------------------------------------------------------------
void WinDIBitmapDC::Destroy()
{
	BITMAPINFO* info = dib.GetInfoAddress();
	dib.Detach();
	
	if (NULL != info)
	{
		free(info);
	}
	
	WinBitmapDC::Destroy();
}
