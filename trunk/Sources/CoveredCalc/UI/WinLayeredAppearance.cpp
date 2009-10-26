/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2009 CoveredCalc Project Contributors
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
	@file		WinLayeredAppearance.cpp
	@brief		Implementation of WinLayeredAppearance class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.4.15 created
*/

#include "Prefix.h"
#include "WinLayeredAppearance.h"
#include "DIBColorLookup.h"
#include "ColorCodedSkin.h"
#include "MemoryException.h"
#include "WinCoveredCalcApp.h"

#define MAX_EDGE_SMOOTHING		10					///< �G�b�W���ڂ������x���̍ő�l

// ---------------------------------------------------------------------
// ���[�U��`���b�Z�[�W
// ---------------------------------------------------------------------
#define UM_UPDATE_LAYERED		(WM_USER + 500)		///< ���C���[�h�E�B���h�E�̍X�V

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinLayeredAppearance::WinLayeredAppearance()
{
	hWnd = NULL;
	pixelWeights = NULL;
	width = 0;
	height = 0;
	isUpdateMessagePosted = false;
	ignoreClipping = false;
	edgeSmoothingLevel = 0;
	totalOpacity = 255;
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinLayeredAppearance::~WinLayeredAppearance()
{
	Exit();
}

// ---------------------------------------------------------------------
//! ���������܂��B
// ---------------------------------------------------------------------
void WinLayeredAppearance::Init(
	HWND hWnd,					//!< �X�L�����`�悳���E�B���h�E
	bool ignoreClipping,		///< �E�B���h�E�̈�̃N���b�s���O�𖳎����邩�ǂ��� (true�Ŗ���)
	SInt32 edgeSmoothingLevel,	///< �N���b�s���O���̃G�b�W���ڂ������x�� (0 �Ȃ�ڂ����Ȃ�, 1�ȏ�łڂ�����)
	SInt32 totalOpacity			///< �S�̂̕s�����x (1�`255�Ŏw��)
)
{
	this->hWnd = hWnd;
	this->ignoreClipping = ignoreClipping;
	if (edgeSmoothingLevel > MAX_EDGE_SMOOTHING)
	{
		this->edgeSmoothingLevel = MAX_EDGE_SMOOTHING;
	}
	else
	{
		this->edgeSmoothingLevel = edgeSmoothingLevel;
	}
	if (totalOpacity < 1)
	{
		this->totalOpacity = 1;
	}
	else if (totalOpacity > 255)
	{
		this->totalOpacity = 255;
	}
	else
	{
		this->totalOpacity = static_cast<UInt8>(totalOpacity);
	}
	DWORD exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle | WinLayeredWindowAPI::cWS_EX_LAYERED);
}

// ---------------------------------------------------------------------
//! �I���������s���܂��B
// ---------------------------------------------------------------------
void WinLayeredAppearance::Exit()
{
	if (pixelWeights != NULL)
	{
		free(pixelWeights);
		pixelWeights = NULL;
	}

	if (hWnd != NULL)
	{
		DWORD exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle & (~WinLayeredWindowAPI::cWS_EX_LAYERED));
		hWnd = NULL;
	}

	// �ێ����Ă��� WinDIBitmapDC ���폜
	if (dibitmapDC.IsCreated())
	{
		dibitmapDC.Destroy();
	}
}

/**
 *	@brief	�E�B���h�E�v���V�[�W���̏������A�s�A�����X�N���X�֓n���܂��B
 *	@return	���������� true.
 */
bool WinLayeredAppearance::RelayWndProc(
	HWND /*hWnd*/,				///< �E�B���h�E�n���h��
	UINT uMsg,				///< ���b�Z�[�W
	WPARAM /* wParam */,	///< ���b�Z�[�W�� WPARAM
	LPARAM /* lParam */,	///< ���b�Z�[�W�� LPARAM
	LRESULT* ret			///< ���������ꍇ�A�E�B���h�E�v���V�[�W���̖߂�l���i�[���܂�
)
{
	if (UM_UPDATE_LAYERED == uMsg)
	{
		if (isUpdateMessagePosted)
		{
			WinLayeredAppearance::UpdateAppearance();
		}
		
		*ret = 0;
		return true;
	}

	return false;
}

/**
 *	@brief	�O�ς��X�V���܂��B�i�����̈���Ȃ����܂��j
 */
void WinLayeredAppearance::UpdateAppearance()
{
	isUpdateMessagePosted = false;

	RECT rcWindow;
	::GetWindowRect(hWnd, &rcWindow);
	
	POINT windowTopLeft;
	windowTopLeft.x = rcWindow.left;
	windowTopLeft.y = rcWindow.top;
	
	SIZE windowSize;
	windowSize.cx = rcWindow.right - rcWindow.left;
	windowSize.cy = rcWindow.bottom - rcWindow.top;
	
	POINT surfaceTopLeft;
	surfaceTopLeft.x = surfaceTopLeft.y = 0;

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = totalOpacity;
	bf.AlphaFormat = AC_SRC_ALPHA;

	const WinLayeredWindowAPI* api = WinCoveredCalcApp::GetInstance()->GetLayeredWindowAPI();
	api->UpdateLayeredWindow(hWnd, NULL, &windowTopLeft, &windowSize, dibitmapDC, &surfaceTopLeft, 0, &bf, WinLayeredWindowAPI::cULW_ALPHA);
}

// ---------------------------------------------------------------------
//! �X�L���̃T�C�Y��ύX���ꂽ�Ƃ��ɌĂ΂�܂��B
// ---------------------------------------------------------------------
void WinLayeredAppearance::ChangeSkinSize(
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
	
	this->width = width;
	this->height = height;
}

// ---------------------------------------------------------------------
//! �X�L���̓����̈���E�B���h�E����؂���܂��B
// ---------------------------------------------------------------------
void WinLayeredAppearance::ClipSkinRegion(
	const DIBitmap* mapBitmap,		//!< �F�}�b�v�p�r�b�g�}�b�v
	ColorValue transparentColor		//!< �����F
)
{
	if (ignoreClipping)
	{
		return;
	}

	if (NULL != pixelWeights)
	{
		free(pixelWeights);
		pixelWeights = NULL;
	}

	pixelWeights = static_cast<UInt8*>(malloc(width * height));
	if (NULL == pixelWeights)
	{
		MemoryException::Throw();
	}
	memset(pixelWeights, 0xff, width * height * sizeof(UInt8));

	// �������� 0xff �ŏ�����
	memset(pixelWeights, 0xff, width * height * sizeof(UInt8));

	// ���������� weight 0 �ɂ���
	DIBColorLookup mapColorLookup(mapBitmap);
	SInt32 row;
	for (row=0; row<height; row++)
	{
		SInt32 col;
		mapColorLookup.InitLocation(0, row);
		for (col=0; col<width; col++)
		{
			if (mapColorLookup.LookupNextColor() == transparentColor)
			{
				pixelWeights[row * width + col] = 0;
			}
		}
	}

	// �ڂ���
	int count;
	for (count = 0; count < edgeSmoothingLevel; count++)
	{
		blurEdgeWeight();
	}
}

static UInt8 getWeight(UInt8* weights, SInt32 width, SInt32 height, SInt32 posX, SInt32 posY)
{
	if (posX < 0 || posX >= width || posY < 0 || posY >= height)
	{
		// �̈悩��͂ݏo��ꍇ�͏d�� 0
		return 0;
	}
	else
	{
		return weights[posY * width + posX];
	}
}

/**
 *	@brief	�؂���̒[�̏d�݂��ڂ����܂��B
 */
void WinLayeredAppearance::blurEdgeWeight()
{
	// �e�_�Ƃ��� 8 �ߖT�̏d�݂̕��ς��Ƃ邱�Ƃłڂ����܂��B
	const SInt32 BLUR_RANGE	= 1;

	UInt8* tempWeights = static_cast<UInt8*>(malloc(width * height));
	if (NULL == tempWeights)
	{
		MemoryException::Throw();
	}
	try
	{
		UInt32 total;
		UInt8 oldest;

		// �������ɕ��ς��Ƃ�
		SInt32 posY, posX;
		for (posY = 0; posY < height; posY++)
		{
			oldest = 0;
			total = 0;
			for (posX = 0; posX < BLUR_RANGE; posX++)
			{
				total += getWeight(pixelWeights, width, height, posX, posY);
			}
			for (posX = 0; posX < width; posX++)
			{
				total += getWeight(pixelWeights, width, height, posX + BLUR_RANGE, posY) - oldest;
				oldest = getWeight(pixelWeights, width, height, posX - BLUR_RANGE, posY);
				tempWeights[posY * width + posX] = static_cast<UInt8>((total + BLUR_RANGE) / (2 * BLUR_RANGE + 1));	// ���鐔�̔������ɑ����Ă����Ďl�̌ܓ����G�~�����[�g				
			}
		}

		// �c�����ɕ��ς��Ƃ�
		for (posX = 0; posX < width; posX++)
		{
			oldest = 0;
			total = 0;
			for (posY = 0; posY < BLUR_RANGE; posY++)
			{
				total += getWeight(tempWeights, width, height, posX, posY);
			}
			for (posY = 0; posY < height; posY++)
			{
				total += getWeight(tempWeights, width, height, posX, posY + BLUR_RANGE) - oldest;
				oldest = getWeight(tempWeights, width, height, posX, posY - BLUR_RANGE);
				// �ŏ�����؂蔲����Ă������� (�A���t�@�l0) �͐؂蔲���ꂽ�܂܂ɂȂ�悤��
				if (pixelWeights[posY * width + posX] != 0)
				{
					pixelWeights[posY * width + posX] = static_cast<UInt8>((total + BLUR_RANGE) / (2 * BLUR_RANGE + 1));	// ���鐔�̔������ɑ����Ă����Ďl�̌ܓ����G�~�����[�g
				}
			}
		}
	}
	catch (...)
	{
		free(tempWeights);
		throw;
	}
	free(tempWeights);
}


// ---------------------------------------------------------------------
//! �X�L���̐؂�����Ȃ����܂��B
// ---------------------------------------------------------------------
void WinLayeredAppearance::UnclipSkinRegion()
{
	if (NULL != pixelWeights)
	{
		free(pixelWeights);
		pixelWeights = NULL;
	}
}

// ---------------------------------------------------------------------
//! �F�}�b�v��̎w�肳�ꂽ�F�ɑΉ����镔�����X�V���܂��B
// ---------------------------------------------------------------------
void WinLayeredAppearance::UpdateMapArea(
	const Rect32& skinRect,			//!< �����Ώۂ̗̈�
	const DIBitmap* mapBitmap,		//!< �F�}�b�v�p�r�b�g�}�b�v
	ColorValue color,				//!< �F
	const DIBitmap* skinBitmap		//!< �`�挳�X�L���p�r�b�g�}�b�v
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
		Byte* line = bitmap->GetBitsLineAddress(posY);
		line += 4 * skinRect.left;
		skinColorLookup.InitLocation(skinRect.left, posY);
		mapColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX=skinRect.left; posX<=skinRect.right; posX++)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			if (mapColorLookup.LookupNextColor() == color)
			{
				UInt32 alpha = static_cast<UInt32>(skinColor.alphaValue);
				if (NULL != pixelWeights)
				{
					alpha = alpha * pixelWeights[width * posY + posX] / 255;
				}
				line[0] = static_cast<Byte>(static_cast<UInt32>(skinColor.blueValue) * alpha / 255);
				line[1] = static_cast<Byte>(static_cast<UInt32>(skinColor.greenValue) * alpha / 255);
				line[2] = static_cast<Byte>(static_cast<UInt32>(skinColor.redValue) * alpha / 255);
				line[3] = static_cast<Byte>(alpha);
			}
			line += 4;
		}
	}
	
	updateLayeredWindow();
}

/**
 *	@brief	�F�}�b�v��̎w�肳�ꂽ�F�ɑΉ����镔����
 *			2 �̃X�L���p�r�b�g�}�b�v�ō��������F�ŕ`�悵�܂��B
 *	@param	skinRect	�����Ώۂ̗̈�
 *	@param	mapBitmap	�F�}�b�v�p�r�b�g�}�b�v
 *	@param	color		�F
 *	@param	skinBitmap1	�`�挳�X�L���p�r�b�g�}�b�v1
 *	@param	skinBitmap2	�`�挳�X�L���p�r�b�g�}�b�v2
 *	@para,	ratio		������ (0�`ColorCodedSkin::BlendRatio_Max)
 */
void WinLayeredAppearance::UpdateMapAreaWithBlend(
	const Rect32& skinRect,
	const DIBitmap* mapBitmap,
	ColorValue color,
	const DIBitmap* skinBitmap1,
	const DIBitmap* skinBitmap2,
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
		Byte* line = bitmap->GetBitsLineAddress(posY);
		line += 4 * skinRect.left;
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
				UInt32 red = (skinColor1.redValue * ratio + skinColor2.redValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max;
				UInt32 green = (skinColor1.greenValue * ratio + skinColor2.greenValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max;
				UInt32 blue = (skinColor1.blueValue * ratio + skinColor2.blueValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max;
				UInt32 alpha = (skinColor1.alphaValue * ratio + skinColor2.alphaValue * (ColorCodedSkin::BlendRatio_Max - ratio)) / ColorCodedSkin::BlendRatio_Max;
				if (NULL != pixelWeights)
				{
					alpha = alpha * pixelWeights[width * posY + posX] / 255;
				}
				line[0] = static_cast<Byte>(blue * alpha / 255);
				line[1] = static_cast<Byte>(green * alpha / 255);
				line[2] = static_cast<Byte>(red * alpha / 255);
				line[3] = static_cast<Byte>(alpha);
			}
			line += 4;
		}
	}
	
	updateLayeredWindow();
}


// ---------------------------------------------------------------------
//! �w�肳�ꂽ��`�̃X�L����`�悵�܂��B
// ---------------------------------------------------------------------
void WinLayeredAppearance::UpdateRect(
	const Rect32& skinRect,			//!< �`�悷���`
	const DIBitmap* skinBitmap		//!< �`�挳�X�L���p�r�b�g�}�b�v
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
	for (posY = skinRect.top; posY <= skinRect.bottom; ++posY)
	{
		Byte* line = bitmap->GetBitsLineAddress(posY);
		line += 4 * skinRect.left;
		skinColorLookup.InitLocation(skinRect.left, posY);
		SInt32 posX;
		for (posX = skinRect.left; posX <= skinRect.right; ++posX)
		{
			ColorValue skinColor = skinColorLookup.LookupNextColor();
			UInt32 alpha = static_cast<UInt32>(skinColor.alphaValue);
			if (NULL != pixelWeights)
			{
				alpha = alpha * pixelWeights[width * posY + posX] / 255;
			}
			line[0] = static_cast<Byte>(static_cast<UInt32>(skinColor.blueValue) * alpha / 255);
			line[1] = static_cast<Byte>(static_cast<UInt32>(skinColor.greenValue) * alpha / 255);
			line[2] = static_cast<Byte>(static_cast<UInt32>(skinColor.redValue) * alpha / 255);
			line[3] = static_cast<Byte>(alpha);
			line += 4;
		}
	}

	updateLayeredWindow();
}

/**
 * @brief Overpaint a rectangle of specified bitmap to specified position.
 * @param[in] drawPoint upperleft position of the appearance
 * @param[in] sourceBitmap source bitmap image
 * @param[in] sourceRect rectangle of source bitmap
 * @param[in] transparentColor this color in source bitmap is not painted.
 *              specify NULL when not to use it.
 */
void WinLayeredAppearance::OverpaintImage(
	const Point32& drawPoint,
	const DIBitmap* sourceBitmap,
	const Rect32& sourceRect,
	const ColorValue* transparentColor
)
{
	if (!dibitmapDC.IsCreated())
	{
		ASSERT(false);
		return;
	}

	DIBitmap* bitmap = dibitmapDC.GetDIB();
	DIBColorLookup sourceColorLookup(sourceBitmap);
	
	SInt32 posY;
	for (posY = sourceRect.top; posY <= sourceRect.bottom; ++posY)
	{
		Byte* line = bitmap->GetBitsLineAddress(posY - sourceRect.top + drawPoint.y);
		line += 4 * drawPoint.x;
		sourceColorLookup.InitLocation(sourceRect.left, posY);
		SInt32 posX;
		for (posX = sourceRect.left; posX <= sourceRect.right; ++posX)
		{
			ColorValue sourceColor = sourceColorLookup.LookupNextColor();
			if (NULL == transparentColor || sourceColor != *transparentColor)
			{
				UInt32 alphaDest = line[3];
				UInt32 alphaSource = static_cast<UInt32>(sourceColor.alphaValue);
				UInt32 pxWeight = (NULL == pixelWeights) ? 255 : pixelWeights[width * (posY - sourceRect.top + drawPoint.y) + (posX - sourceRect.left + drawPoint.x)];
				if (0 == pxWeight)
				{
					alphaDest = 0;
				}
				else
				{
					alphaDest = alphaDest * 255 / pxWeight; 
				}

				UInt32 alphaNew255 = (255 * 255) - (255 - alphaSource) * (255 - alphaDest);	// = 255 * alphaNew
				UInt32 colorDestFactor = (255 - alphaSource) * alphaDest;
				UInt32 colorSourceFactor = 255 * alphaSource;
				line[0] = static_cast<Byte>((colorDestFactor * static_cast<UInt32>(line[0]) + colorSourceFactor * static_cast<UInt32>(sourceColor.blueValue)) / alphaNew255);
				line[1] = static_cast<Byte>((colorDestFactor * static_cast<UInt32>(line[1]) + colorSourceFactor * static_cast<UInt32>(sourceColor.greenValue)) / alphaNew255);
				line[2] = static_cast<Byte>((colorDestFactor * static_cast<UInt32>(line[2]) + colorSourceFactor * static_cast<UInt32>(sourceColor.redValue)) / alphaNew255);
				line[3] = static_cast<Byte>((alphaNew255 * pxWeight) / (255 * 255));

				/* formula above is derived by following:
				 * let Od = opacity of destination (0..1), Cd = color element of destination (0..255),
				 * and Cb = color element of background
				 * then destination color element is
				 *     (1 - Od) * Cb   +   Od * Cb
				 * when overpaints source pixel, where Os = opacity of source and Cs = color element of source,
				 * it changes to 
				 *     (1 - Os) * ((1 - Od) * Cb   +   Od * Cb)   +    Os * Cs
				 *   = (1 - Os) * (1 - Od) * Cb    +   (1 - Os) * Od * Cd + Os * Cs
				 *   = (1 - On) * Cb    +    On * Cn     (where On = new opacity, Cn = new color element)
				 * hence 
				 *     (1 - Os) * (1 - Od) = (1 - On)
				 *                             => On = 1 - (1 - Os) * (1 - Od)
				 *     (1 - Os) * Od * Cd + Os * Cs = On * Cn
				 *                             => Cn = ((1 - Os) * Od * Cd + Os * Cs) / On
				 *
				 * actually the opacity is represented as an alpha value (0..255),
				 *  Od = Ad / 255,    Os = As / 255      (where Ad = alpha value of destination, As = that of source)
				 * then On is
				 *  On = 1  -  (1 - As / 255) * (1 - Ad / 255)
				 *     = ((255 * 255) - (255 - As) * (255 - Ad)) / (255 * 255)
				 *     = An / 255                        (where An = new alpha value)
				 *
				 *       (1 - As / 255) * (Ad / 255) * Cd + (As / 255) * Cs
				 *  Cn = ---------------------------------------------------
				 *       (255 * 255 - (255 - As) * (255 - Ad)) / (255 * 255)
				 *
				 *     = .... which is transformed to ....
				 *
				 *       (255 - As) * Ad * Cd   +   255 * As * Cs
				 *     = -----------------------------------------
				 *         (255 * 255) - (255 - As) * (255 - Ad)
				 *
				 *  now let alphaNew255 = 255 * 255 - (255 - As) * (255 - Ad),
				 *          colorDestFactor = (255 - As) * Ad, and
				 *          colorSourceFactor = 255 * As
				 *  then
				 *   An = alphaNew255 / 255
				 *   Cn = (colorDestFactor * Cd  + colorSourceFactor * Cs)  /  alphaNew255
				 */      
			}
			line += 4;
		}
	}

	updateLayeredWindow();
}

/**
 *	@brief	���C���[�h�E�B���h�E���X�V���܂��B
 */
void WinLayeredAppearance::updateLayeredWindow()
{
	if (!isUpdateMessagePosted)
	{
		::PostMessage(hWnd, UM_UPDATE_LAYERED, 0, 0);
		isUpdateMessagePosted = true;
	}
}
