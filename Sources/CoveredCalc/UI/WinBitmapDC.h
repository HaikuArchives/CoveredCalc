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
	@file		WinBitmapDC.h
	@brief		Definition of WinBitmapDC class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.10.11 created
*/

#ifndef _WINBITMAPDC_H_
#define _WINBITMAPDC_H_

// ---------------------------------------------------------------------
//! (DDB�ɂ��)�E�B���h�E�̃_�u���o�b�t�@�����O�N���X
/*!
	���������(�f�o�C�X�ˑ�)�r�b�g�}�b�v���쐬���A
	�f�o�C�X�R���e�L�X�g�Ƃ��Ĉ�����悤�ɂ���N���X�B
	�_�u���o�b�t�@�����O���������邽�߂Ɏg���B
*/
// ---------------------------------------------------------------------
class WinBitmapDC
{
public:
						WinBitmapDC();
	virtual				~WinBitmapDC();
	
	virtual	void		Create(HDC dc, SInt32 width, SInt32 height);
	virtual	void		Destroy();
	virtual bool		IsCreated() const { return (NULL == bitmap) ? false : true; }
	
						operator HDC() const;

protected:
	HBITMAP				bitmap;		//!< ��������̃r�b�g�}�b�v
	HBITMAP				oldBitmap;	//!< ������DC��SelectObject���Ă�ԂɊo���Ă����O�̃r�b�g�}�b�v
	HDC					memDC;		//!< ������DC
};

#endif // _WINBITMAPDC_H_