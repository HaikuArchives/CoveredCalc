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
	@file		BmpAlpha.cpp
	@brief		Main functions of this application.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.5.5 created
*/

#include "Prefix.h"
#include "DIBitmap.h"
#include "MemoryException.h"
#include "BMPFileLoader.h"
#include "DIBFileLoaderException.h"
#include "BMPFileSaver.h"
#include "DIBFileSaverException.h"
#include "DIBColorLookup.h"
#include "Exception.h"
#include "FileException.h"
#include "PathException.h"

#define VERSION		"1.0.0"
#define COPYRIGHT	"Copyright (c) 2007 CoveredCalc Project Contributors"

namespace BmpAlphaExceptions
{
	/**
	 *	@brief	2 �̃r�b�g�}�b�v�̏c���T�C�Y���قȂ�Ƃ��ɓ������܂��B
	 */
	class BitmapSizeMismatch : public Exception
	{
	public:
		BitmapSizeMismatch()	: Exception() { }
		virtual ~BitmapSizeMismatch() { }
	};
}


/**
 *	@brief	�A���t�@�v���[���𔲂��o�����摜�̂��߂� DIBitmap �I�u�W�F�N�g���쐬���܂��B
 *	@param[in,out]	bitmap	�����Ŏw�肵�� DIBitmap �I�u�W�F�N�g�ɑ΂��č\�z���s���܂��B
 *	@param[in]		sizeX	���������̉摜�T�C�Y
 *	@param[in]		sizeY	���������̉摜�T�C�Y
 */
void CreateExtractedDIBitmap(DIBitmap& bitmap, SInt32 sizeX, SInt32 sizeY)
{
	// 8 bpp �� 256 �i�K�O���[�X�P�[���̃p���b�g�����r�b�g�}�b�v���쐬���܂��B

	BITMAPINFO* bitmapInfo = static_cast<BITMAPINFO*>(DIBitmap::AllocMemory(sizeof(BITMAPINFOHEADER) + 256 * 4));
	if (NULL == bitmapInfo)
	{
		MemoryException::Throw();
	}
	try
	{
		BITMAPINFOHEADER* infoHeader = &bitmapInfo->bmiHeader;
		infoHeader->biSize = sizeof(BITMAPINFOHEADER);
		infoHeader->biWidth = sizeX;
		infoHeader->biHeight = sizeY;
		infoHeader->biPlanes = 1;
		infoHeader->biBitCount = 8;
		infoHeader->biCompression = BI_RGB;
		infoHeader->biSizeImage = 0;
		infoHeader->biXPelsPerMeter = 0;
		infoHeader->biYPelsPerMeter = 0;
		infoHeader->biClrUsed = 0;
		infoHeader->biClrImportant = 0;
		
		RGBQUAD* palette = reinterpret_cast<RGBQUAD*>(reinterpret_cast<Byte*>(bitmapInfo) + infoHeader->biSize);
		SInt32 index;
		for (index = 0; index < 256; index++)
		{
			palette[index].rgbBlue = palette[index].rgbGreen = palette[index].rgbRed = static_cast<Byte>(index);
			palette[index].rgbReserved = 0;
		}
		
		int lineSize = (sizeX + 3) & ~3;	// (((sizeX * 8 + 7) / 8) + 3) & ~3; see DIBitmap::getStorageWidth().
		int bitsSize = lineSize * sizeY;
		Byte* bits = static_cast<Byte*>(DIBitmap::AllocMemory(bitsSize));
		if (NULL == bits)
		{
			MemoryException::Throw();
		}
		try
		{
			memset(bits, 0, bitsSize);
			bitmap.Attach(bitmapInfo, bits);
		}
		catch (...)
		{
			DIBitmap::FreeMemory(bits);
			throw;
		}	
	}
	catch (...)
	{
		DIBitmap::FreeMemory(bitmapInfo);
		throw;
	}
}

/**
 *	@brief	�w�肳�ꂽ�r�b�g�}�b�v��ǂݍ��݁A�A���t�@�v���[�������𔲂��o���ăO���[�X�P�[���r�b�g�}�b�v�Ƃ��ĕۑ����܂��B
 *	@param[in]	sourceBitmapPath	�ǂݍ��ރr�b�g�}�b�v�̃p�X
 *	@param[in]	outBitmapPath		�o�͂���r�b�g�}�b�v�̃p�X
 */
void Extract(const Path& sourceBitmapPath, const Path& outBitmapPath)
{
	// �\�[�X�r�b�g�}�b�v�̓ǂݍ���
	DIBitmap sourceBitmap;
	BMPFileLoader loader;
	loader.Load(&sourceBitmap, sourceBitmapPath);
	
	int sizeX = sourceBitmap.GetWidth();
	int sizeY = sourceBitmap.GetHeight();
	
	// �A���t�@�v���[���𔲂��o�����O���[�X�P�[���r�b�g�}�b�v����
	DIBitmap outBitmap;
	CreateExtractedDIBitmap(outBitmap, sizeX, sizeY);
	
	DIBColorLookup lookup(&sourceBitmap);
	Byte* outLine;
	int posY;
	for (posY = 0; posY < sizeY; posY++)
	{
		lookup.InitLocation(0, posY);
		outLine = outBitmap.GetBitsLineAddress(posY);
		int posX;
		for (posX = 0; posX < sizeX; posX++)
		{
			ColorValue color = lookup.LookupNextColor();
			*outLine = color.alphaValue;
			outLine++;
		}
	}
	
	// ���������r�b�g�}�b�v��ۑ�
	BMPFileSaver saver;
	saver.Save(&outBitmap, outBitmapPath);
}

/**
 *	@brief	�A���t�@�v���[�������������摜�̂��߂� DIBitmap �I�u�W�F�N�g���쐬���܂��B
 *	@param[in,out]	bitmap	�����Ŏw�肵�� DIBitmap �I�u�W�F�N�g�ɑ΂��č\�z���s���܂��B
 *	@param[in]		sizeX	���������̉摜�T�C�Y
 *	@param[in]		sizeY	���������̉摜�T�C�Y
 */
void CreateMergedDIBitmap(DIBitmap& bitmap, SInt32 sizeX, SInt32 sizeY)
{
	// 32 bpp �A���t�@�v���[���t���̃r�b�g�}�b�v���쐬���܂��B

	BITMAPINFO* bitmapInfo = static_cast<BITMAPINFO*>(DIBitmap::AllocMemory(sizeof(BITMAPV4HEADER) + 3 * 4));
	if (NULL == bitmapInfo)
	{
		MemoryException::Throw();
	}
	try
	{
		BITMAPV4HEADER* infoHeader = reinterpret_cast<BITMAPV4HEADER*>(&bitmapInfo->bmiHeader);
		::memset(infoHeader, 0, sizeof(BITMAPV4HEADER));
		infoHeader->bV4Size = sizeof(BITMAPV4HEADER);
		infoHeader->bV4Width = sizeX;
		infoHeader->bV4Height = sizeY;
		infoHeader->bV4Planes = 1;
		infoHeader->bV4BitCount = 32;
		infoHeader->bV4V4Compression = BI_BITFIELDS;
		infoHeader->bV4SizeImage = 0;
		infoHeader->bV4XPelsPerMeter = 0;
		infoHeader->bV4YPelsPerMeter = 0;
		infoHeader->bV4ClrUsed = 0;
		infoHeader->bV4ClrImportant = 0;
		infoHeader->bV4RedMask = 0x00FF0000;
		infoHeader->bV4GreenMask = 0x0000FF00;
		infoHeader->bV4BlueMask = 0x000000FF;
		infoHeader->bV4AlphaMask = 0xFF000000;
		
		UInt32* colorMasks = reinterpret_cast<UInt32*>(reinterpret_cast<Byte*>(bitmapInfo) + infoHeader->bV4Size);
		colorMasks[0] = infoHeader->bV4RedMask;
		colorMasks[1] = infoHeader->bV4GreenMask;
		colorMasks[2] = infoHeader->bV4BlueMask;

		int lineSize = (sizeX * 4 + 3) & ~3;	// (((sizeX * 32 + 7) / 8) + 3) & ~3; see DIBitmap::getStorageWidth().
		int bitsSize = lineSize * sizeY;
		Byte* bits = static_cast<Byte*>(DIBitmap::AllocMemory(bitsSize));
		if (NULL == bits)
		{
			MemoryException::Throw();
		}
		try
		{
			memset(bits, 0, bitsSize);
			bitmap.Attach(bitmapInfo, bits);
		}
		catch (...)
		{
			DIBitmap::FreeMemory(bits);
			throw;
		}	
	}
	catch (...)
	{
		DIBitmap::FreeMemory(bitmapInfo);
		throw;
	}
}

/**
 *	@brief	�w�肳�ꂽ�r�b�g�}�b�v�ƃA���t�@�v���[���̃r�b�g�}�b�v���������ăA���t�@�`�����l���t���r�b�g�}�b�v�Ƃ��ĕۑ����܂��B
 *	@param[in]	rgbBitmapPath	R,G,B �̃v���[�������r�b�g�}�b�v�̃p�X
 *	@param[in]	alphaBitmapPath	�A���t�@�v���[���̃r�b�g�}�b�v
 *	@param[in]	outBitmapPath	�o�͂���r�b�g�}�b�v�̃p�X
 */
void Merge(const Path& rgbBitmapPath, const Path& alphaBitmapPath, const Path& outBitmapPath)
{
	// �\�[�X�r�b�g�}�b�v�̓ǂݍ���
	DIBitmap rgbBitmap;
	BMPFileLoader rgbLoader;
	rgbLoader.Load(&rgbBitmap, rgbBitmapPath);
	
	int sizeX = rgbBitmap.GetWidth();
	int sizeY = rgbBitmap.GetHeight();
	
	DIBitmap alphaBitmap;
	BMPFileLoader alphaLoader;
	alphaLoader.Load(&alphaBitmap, alphaBitmapPath);
	if (alphaBitmap.GetWidth() != sizeX || alphaBitmap.GetHeight() != sizeY)
	{
		throw new BmpAlphaExceptions::BitmapSizeMismatch();
	}
	
	// ���������r�b�g�}�b�v����
	DIBitmap outBitmap;
	CreateMergedDIBitmap(outBitmap, sizeX, sizeY);
	
	DIBColorLookup rgbLookup(&rgbBitmap);
	DIBColorLookup alphaLookup(&alphaBitmap);
	Byte* outLine;
	int posY;
	for (posY = 0; posY < sizeY; posY++)
	{
		rgbLookup.InitLocation(0, posY);
		alphaLookup.InitLocation(0, posY);
		outLine = outBitmap.GetBitsLineAddress(posY);
		int posX;
		for (posX = 0; posX < sizeX; posX++)
		{
			ColorValue rgbColor = rgbLookup.LookupNextColor();
			ColorValue alphaColor = alphaLookup.LookupNextColor();
			*outLine++ = rgbColor.blueValue;
			*outLine++ = rgbColor.greenValue;
			*outLine++ = rgbColor.redValue;
			*outLine++ = alphaColor.redValue;
		}
	}
	
	// ���������r�b�g�}�b�v��ۑ�
	BMPFileSaver saver;
	saver.Save(&outBitmap, outBitmapPath);
}

/**
 *	@brief	1�s���̃��b�Z�[�W���o�͂��܂��B
 *	@param[in]	message	���b�Z�[�W
 */
void OutputMessageLine(const char* message)
{
	puts(message);
}

bool outputMemoryError(Exception* e)
{
	MemoryException* ex = dynamic_cast<MemoryException*>(e);
	if (NULL == ex)
	{
		return false;
	}
	
	OutputMessageLine("Out of memory.");
	return true;
}

bool outputDIBFileLoaderError(FileException* e)
{
	DIBFileLoaderException* ex = dynamic_cast<DIBFileLoaderException*>(e);
	if (NULL == ex)
	{
		return false;
	}

	MBCString message;
	if (NULL != dynamic_cast<DIBFileLoaderExceptions::BrokenFile*>(ex))
	{
		message = "Bitmap file is broken.";
	}
	else if (NULL != dynamic_cast<DIBFileLoaderExceptions::UnknownFileFormat*>(ex))
	{
		message = "Unknown file format.";
	}
	else
	{
		message = "Bitmap load error.";
	}

	MBCString fileName;
	ex->GetFileName().GetFileName(fileName);

	if (!fileName.IsEmpty())
	{
		message += " (";
		message += fileName;
		message += ")";
	}
	
	OutputMessageLine(message);
	return true;
}

bool outputDIBFileSaverError(FileException* e)
{
	DIBFileSaverException* ex = dynamic_cast<DIBFileSaverException*>(e);
	if (NULL == ex)
	{
		return false;
	}

	MBCString message;
	message = "Bitmap save error.";

	MBCString fileName;
	ex->GetFileName().GetFileName(fileName);

	if (!fileName.IsEmpty())
	{
		message += " (";
		message += fileName;
		message += ")";
	}
	
	OutputMessageLine(message);
	return true;
}

bool outputFileError(Exception* e)
{
	FileException* ex = dynamic_cast<FileException*>(e);
	if (NULL == ex)
	{
		return false;
	}
	
	if (outputDIBFileLoaderError(ex))		return true;
	if (outputDIBFileSaverError(ex))		return true;
	
	MBCString message;
	if (NULL != dynamic_cast<FileExceptions::AccessDenied*>(ex))
	{
		message = "Access Denied.";
	}
	else if (NULL != dynamic_cast<FileExceptions::DeviceFull*>(ex))
	{
		message = "Device Full.";
	}
	else if (NULL != dynamic_cast<FileExceptions::FileNotFound*>(ex))
	{
		message = "File not found.";
	}
	else if (NULL != dynamic_cast<FileExceptions::IOError*>(ex))
	{
		message = "I/O Error.";
	}
	else if (NULL != dynamic_cast<FileExceptions::SharingViolation*>(ex))
	{
		message = "File is being used by another program.";
	}
	else
	{
		message = "File error.";
	}
	
	MBCString fileName;
	ex->GetFileName().GetFileName(fileName);

	if (!fileName.IsEmpty())
	{
		message += " (";
		message += fileName;
		message += ")";
	}
	
	OutputMessageLine(message);
	return true;
}

bool outputPathError(Exception* e)
{
	PathException* ex = dynamic_cast<PathException*>(e);
	if (NULL == ex)
	{
		return false;
	}
	
	MBCString message;
	message = "Error has occured in path operation.";
	OutputMessageLine(message);
	return true;
}

bool outputBitmapSizeError(Exception* e)
{
	BmpAlphaExceptions::BitmapSizeMismatch* ex = dynamic_cast<BmpAlphaExceptions::BitmapSizeMismatch*>(e);
	if (NULL == ex)
	{
		return false;
	}
	
	OutputMessageLine("Bitmap sizes (width or height) are mismatch.");
	return true;
}

/**
 *	@brief	�G���[���b�Z�[�W�̏o��
 *	@param[in]	ex	�G���[�̌��ɂȂ�����O
 */
void OutputError(Exception* ex)
{
	Exception* inner = ex->GetInnerException();
	if (NULL != inner)
	{
		OutputError(inner);
	}

	if (outputMemoryError(ex))		return;
	if (outputFileError(ex))		return;
	if (outputPathError(ex))		return;
	if (outputBitmapSizeError(ex))	return;
	
	OutputMessageLine("Unknown error has occured.");
}

void ShowUsage()
{
	OutputMessageLine("Usage:");
	OutputMessageLine("  bmpalpha extract <source-bitmap> <out-alpha-bitmap>");
	OutputMessageLine("  bmpalpha merge <rgb-bitmap> <alpha-bitmap> <out-bitmap>");
}

int _tmain(int argc, _TCHAR* argv[])
{
	OutputMessageLine("bmpalpha version " VERSION);
	OutputMessageLine(COPYRIGHT);
	OutputMessageLine("");

	try
	{
		if (argc < 2)
		{
			ShowUsage();
			return -1;
		}
		if (0 == strcmp(argv[1], "extract"))
		{
			if (argc < 4)
			{
				ShowUsage();
				return -1;
			}
			Path sourceBitmapPath(argv[2]);
			Path outBitmapPath(argv[3]);
			Extract(sourceBitmapPath, outBitmapPath);
		}
		else if (0 == strcmp(argv[1], "merge"))
		{
			if (argc < 5)
			{
				ShowUsage();
				return -1;
			}
			Path rgbBitmapPath(argv[2]);
			Path alphaBitmapPath(argv[3]);
			Path outBitmapPath(argv[4]);
			Merge(rgbBitmapPath, alphaBitmapPath, outBitmapPath);
		}
		else
		{
			ShowUsage();
			return -1;
		}
	}
	catch (Exception* ex)
	{
		OutputError(ex);
		ex->Delete();
		OutputMessageLine("Failed.");
		return -128;
	}

	OutputMessageLine("Succeeded.");
	return 0;
}

