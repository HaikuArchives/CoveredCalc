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
	@file		WinNormalAppearance.cpp
	@brief		Implementation of WinNormalAppearance class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.4.7 created
*/

#include "Prefix.h"
#include "WinNormalAppearance.h"
#include "DIBColorLookup.h"
#include "ColorCodedSkin.h"

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinNormalAppearance::WinNormalAppearance()
{
	hWnd = NULL;
	hWndRgn = NULL;
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinNormalAppearance::~WinNormalAppearance()
{
	Exit();
}

// ---------------------------------------------------------------------
//! ���������܂��B
// ---------------------------------------------------------------------
void WinNormalAppearance::Init(
	HWND hWnd			//!< �X�L�����`�悳���E�B���h�E
)
{
	this->hWnd = hWnd;
	this->hWndRgn = NULL;
}

// ---------------------------------------------------------------------
//! �I���������s���܂��B
// ---------------------------------------------------------------------
void WinNormalAppearance::Exit()
{
	// �E�B���h�E�̐؂�������ɖ߂�
	if (NULL != hWndRgn)
	{
		::SetWindowRgn(hWnd, NULL, TRUE);
		::DeleteObject(hWndRgn);
		hWndRgn = NULL;
	}

	// �ێ����Ă��� WinDIBitmapDC ���폜
	if (dibitmapDC.IsCreated())
	{
		dibitmapDC.Destroy();
	}
	
	hWnd = NULL;
}

/**
 *	@brief	�E�B���h�E�v���V�[�W���̏������A�s�A�����X�N���X�֓n���܂��B
 *	@return	���������� true.
 */
bool WinNormalAppearance::RelayWndProc(
	HWND hWnd,				///< �E�B���h�E�n���h��
	UINT uMsg,				///< ���b�Z�[�W
	WPARAM /* wParam */,	///< ���b�Z�[�W�� WPARAM
	LPARAM /* lParam */,	///< ���b�Z�[�W�� LPARAM
	LRESULT* ret			///< ���������ꍇ�A�E�B���h�E�v���V�[�W���̖߂�l���i�[���܂�
)
{
	if (WM_PAINT == uMsg)
	{
		PAINTSTRUCT ps;
		HDC dc = NULL;

		RECT rect;
		::GetClientRect(hWnd, &rect);
		
		dc = ::BeginPaint(hWnd, &ps);
		if (NULL != dc)
		{
			try
			{	
				::BitBlt(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dibitmapDC, 0, 0, SRCCOPY);
			}
			catch (...)
			{
				::EndPaint(hWnd, &ps);
				throw;
			}
			::EndPaint(hWnd, &ps);
		}
		
		*ret = 0;
		return true;
	}

	return false;
}

/**
 *	@brief	�O�ς��X�V���܂��B�i�����̈���Ȃ����܂��j
 */
void WinNormalAppearance::UpdateAppearance()
{
	::UpdateWindow(hWnd);
}

// ---------------------------------------------------------------------
//! �X�L���̃T�C�Y��ύX���ꂽ�Ƃ��ɌĂ΂�܂��B
// ---------------------------------------------------------------------
void WinNormalAppearance::ChangeSkinSize(
	SInt32 width,		//!< �V������
	SInt32 height		//!< �V��������
)
{
	// �ێ����Ă��� WinDIBitmapDC ���č쐬
	if (dibitmapDC.IsCreated())
	{
		dibitmapDC.Destroy();
	}
	
	HDC hDC = ::GetDC(hWnd);
	try
	{
		dibitmapDC.Create(hDC, width, height);
	}
	catch (...)
	{
		::ReleaseDC(hWnd, hDC);
		throw;
	}
	::ReleaseDC(hWnd, hDC);	

	// �E�B���h�E�̃T�C�Y��ύX
	RECT winRect;
	::GetWindowRect(hWnd, &winRect);
	::MoveWindow(hWnd, winRect.left, winRect.top, width, height, FALSE);
}

// ---------------------------------------------------------------------
//! �X�L���̓����̈���E�B���h�E����؂���܂��B
// ---------------------------------------------------------------------
void WinNormalAppearance::ClipSkinRegion(
	const DIBitmap* mapBitmap,		//!< �F�}�b�v�p�r�b�g�}�b�v
	ColorValue transparentColor		//!< �����F
)
{
	// ���łɐ؂����Ă���ΐ؂�������ɖ߂�
	if (NULL != hWndRgn)
	{
		::SetWindowRgn(hWnd, NULL, TRUE);
		::DeleteObject(hWndRgn);
		hWndRgn = NULL;
	}
	
	hWndRgn = ::CreateRectRgn(0, 0, 0, 0);
	
	// �����łȂ����������[�W�����ɒǉ�
	DIBColorLookup mapColorLookup(mapBitmap);
	SInt32 width = mapBitmap->GetWidth();
	SInt32 height = mapBitmap->GetHeight();
	HRGN rgn;
	SInt32 row;
	for (row=0; row<height; row++)
	{
		SInt32 contiguousPixels = 0;
		SInt32 col;
		mapColorLookup.InitLocation(0, row);
		for (col=0; col<width; col++)
		{
			if (mapColorLookup.LookupNextColor() != transparentColor)
			{
				contiguousPixels++;
			}
			else
			{
				if (0 != contiguousPixels)
				{
					rgn = ::CreateRectRgn(col - contiguousPixels, row, col, row + 1);
					::CombineRgn(hWndRgn, hWndRgn, rgn, RGN_OR);
					::DeleteObject(rgn);
					contiguousPixels = 0;
				}
			}
		}
		if (0 != contiguousPixels)
		{
			rgn = ::CreateRectRgn(col - contiguousPixels, row, col, row + 1);
			::CombineRgn(hWndRgn, hWndRgn, rgn, RGN_OR);
			::DeleteObject(rgn);
		}		
	}
	
	// �E�B���h�E��؂���
	::SetWindowRgn(hWnd, hWndRgn, TRUE);
}

// ---------------------------------------------------------------------
//! �X�L���̐؂�����Ȃ����܂��B
// ---------------------------------------------------------------------
void WinNormalAppearance::UnclipSkinRegion()
{
	// ���łɐ؂����Ă���ΐ؂�������ɖ߂�
	if (NULL != hWndRgn)
	{
		::SetWindowRgn(hWnd, NULL, TRUE);
		::DeleteObject(hWndRgn);
		hWndRgn = NULL;
	}
}

// ---------------------------------------------------------------------
//! �F�}�b�v��̎w�肳�ꂽ�F�ɑΉ����镔����`�悵�܂��B
// ---------------------------------------------------------------------
void WinNormalAppearance::DrawSkinByColor(
	const Point32& drawPoint,		//!< �`���̍�����W
	const DIBitmap* mapBitmap,		//!< �F�}�b�v�p�r�b�g�}�b�v
	const DIBitmap* skinBitmap,		//!< �`�挳�X�L���p�r�b�g�}�b�v
	ColorValue color,				//!< �F
	const Rect32& skinRect			//!< �`�挳�̕`�悷��̈�
)
{
	if (!dibitmapDC.IsCreated())
	{
		ASSERT(false);
		return;
	}

	DIBitmap* bitmap = dibitmapDC.GetDIB();
	DIBColorLookup mapColorLookup(mapBitmap);
	DIBColorLookup skinColorLookup(skinBitmap);
	
	SInt32 posY;
	for (posY=skinRect.top; posY<=skinRect.bottom; posY++)
	{
		Byte* line = bitmap->GetBitsLineAddress(posY - skinRect.top + drawPoint.y);
		line += 3 * drawPoint.x;
		skinColorLookup.InitLocation(skinRect.left, posY);
		mapColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			if (mapColorLookup.LookupNextColor() == color)
			{
				line[0] = skinColor.blueValue;
				line[1] = skinColor.greenValue;
				line[2] = skinColor.redValue;
			}
			line += 3;
		}
	}
	
	invalidateRect(drawPoint, skinRect);
}

/**
 *	@brief	�F�}�b�v��̎w�肳�ꂽ�F�ɑΉ����镔����
 *			2 �̃X�L���p�r�b�g�}�b�v�ō��������F�ŕ`�悵�܂��B
 *	@param	drawPoint	�`���̍�����W
 *	@param	mapBitmap	�F�}�b�v�p�r�b�g�}�b�v
 *	@param	skinBitmap1	�`�挳�X�L���p�r�b�g�}�b�v1
 *	@param	skinBitmap2	�`�挳�X�L���p�r�b�g�}�b�v2
 *	@param	color		�F
 *	@param	skinRect	�`�挳�̕`�悷��̈�
 *	@para,	ratio		������ (0�`ColorCodedSkin::BlendRatio_Max)
 */
void WinNormalAppearance::DrawBlendSkinByColor(
	const Point32& drawPoint,
	const DIBitmap* mapBitmap,
	const DIBitmap* skinBitmap1,
	const DIBitmap* skinBitmap2,
	ColorValue color,
	const Rect32& skinRect,
	UInt32 ratio
)
{
	if (!dibitmapDC.IsCreated())
	{
		ASSERT(false);
		return;
	}

	DIBitmap* bitmap = dibitmapDC.GetDIB();
	DIBColorLookup mapColorLookup(mapBitmap);
	DIBColorLookup skinColorLookup1(skinBitmap1);
	DIBColorLookup skinColorLookup2(skinBitmap2);
	
	SInt32 posY;
	for (posY=skinRect.top; posY<=skinRect.bottom; posY++)
	{
		Byte* line = bitmap->GetBitsLineAddress(posY - skinRect.top + drawPoint.y);
		line += 3 * drawPoint.x;
		skinColorLookup1.InitLocation(skinRect.left, posY);
		skinColorLookup2.InitLocation(skinRect.left, posY);
		mapColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor1 = skinColorLookup1.LookupNextColor();
			ColorValue skinColor2 = skinColorLookup2.LookupNextColor();
			if (mapColorLookup.LookupNextColor() == color)
			{
				line[0] = static_cast<Byte>((skinColor1.blueValue * ratio + skinColor2.blueValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max);
				line[1] = static_cast<Byte>((skinColor1.greenValue * ratio + skinColor2.greenValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max);
				line[2] = static_cast<Byte>((skinColor1.redValue * ratio + skinColor2.redValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max);
			}
			line += 3;
		}
	}
	
	invalidateRect(drawPoint, skinRect);
}


// ---------------------------------------------------------------------
//! �w�肳�ꂽ��`�̃X�L����`�悵�܂��B
// ---------------------------------------------------------------------
void WinNormalAppearance::CopySkin(
	const Point32& drawPoint,		//!< �`���̍�����W
	const DIBitmap* skinBitmap,		//!< �`�挳�X�L���p�r�b�g�}�b�v
	const Rect32& skinRect			//!< �`�挳�̕`�悷��̈�
)
{
	if (!dibitmapDC.IsCreated())
	{
		ASSERT(false);
		return;
	}

	DIBitmap* bitmap = dibitmapDC.GetDIB();
	DIBColorLookup skinColorLookup(skinBitmap);
	
	SInt32 posY;
	for (posY=skinRect.top; posY<=skinRect.bottom; posY++)
	{
		Byte* line = bitmap->GetBitsLineAddress(posY - skinRect.top + drawPoint.y);
		line += 3 * drawPoint.x;
		skinColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			line[0] = skinColor.blueValue;
			line[1] = skinColor.greenValue;
			line[2] = skinColor.redValue;
			line += 3;
		}
	}

	invalidateRect(drawPoint, skinRect);
}

// ---------------------------------------------------------------------
//! �w�肳�ꂽ��`�̃X�L����`�悵�܂��B�����������F�Ɏw�肳�ꂽ�F�̃s�N�Z���̓R�s�[���܂���B
// ---------------------------------------------------------------------
void WinNormalAppearance::CopySkin(
	const Point32& drawPoint,		//!< �`���̍�����W
	const DIBitmap* skinBitmap,		//!< �`�挳�X�L���p�r�b�g�}�b�v
	const Rect32& skinRect,			//!< �`�挳�̕`�悷��̈�
	ColorValue transparentColor		//!< �����F
)
{
	if (!dibitmapDC.IsCreated())
	{
		ASSERT(false);
		return;
	}

	DIBitmap* bitmap = dibitmapDC.GetDIB();
	DIBColorLookup skinColorLookup(skinBitmap);
	
	SInt32 posY;
	for (posY=skinRect.top; posY<=skinRect.bottom; posY++)
	{
		Byte* line = bitmap->GetBitsLineAddress(posY - skinRect.top + drawPoint.y);
		line += 3 * drawPoint.x;
		skinColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			if (skinColor != transparentColor)
			{
				line[0] = skinColor.blueValue;
				line[1] = skinColor.greenValue;
				line[2] = skinColor.redValue;
			}
			line += 3;
		}
	}

	invalidateRect(drawPoint, skinRect);
}

// ---------------------------------------------------------------------
//! �w�肳�ꂽ��`�𖳌������܂��B
// ---------------------------------------------------------------------
void WinNormalAppearance::invalidateRect(
	const Point32& drawPoint,		//!< �`���̍�����W
	const Rect32& skinRect			//!< �`�挳�̕`�悷��̈�
)
{
	RECT invRect;
	invRect.left = drawPoint.x;
	invRect.top = drawPoint.y;
	invRect.right = drawPoint.x + skinRect.right - skinRect.left + 1;	// Windows ���W�n�̓s�N�Z���Ԃɍ��W������̂� +1 ����B
	invRect.bottom = drawPoint.y + skinRect.bottom - skinRect.top + 1;	//  0  1  2		... �s�N�Z�����W
																		// |��|��|��|
																		// 0  1  2  3   ... Windows ���W
	::InvalidateRect(hWnd, &invRect, TRUE);
}
