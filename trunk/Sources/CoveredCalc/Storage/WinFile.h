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
	@file		WinFile.h
	@brief		Definition of WinFile class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.22 created
*/

#ifndef _WINFILE_H_
#define _WINFILE_H_

#include "Path.h"
#include "FileConstants.h"

class FileException;

// ---------------------------------------------------------------------
//! This class is used in reading/writing data of a file for Windows
// ---------------------------------------------------------------------
class WinFile
{
public:
						WinFile();
	virtual				~WinFile();
	
	void				Open(const Path& filePath, FileConstants::OpenMode openMode, UInt32 openOption);
	void				Close();
	
	UInt32				Read(void* buffer, UInt32 count);
	void				Write(const void* buffer, UInt32 count);
	
	void				Seek(SInt32 offset, FileConstants::SeekOrigin origin);
	
	UInt32				GetSize() const;

	static ConstAStr	GetLineBreak() { return ALITERAL("\r\n"); }
	static ConstSStr	GetLineBreakS() { return "\r\n"; }
	static void			Remove(const Path& filePath);

private:
	static	void		analyzeOpenParameter(FileConstants::OpenMode openMode, UInt32 openOption, DWORD* dwDesiredAccess, DWORD* dwShareMode, DWORD* dwCreateDisposition);
	void				throwNewFileException(DWORD lastError) const;
	void				throwNewFileException(DWORD lastError, const Path& filePath) const;
	static	void		throwNewFileExceptionDirect(DWORD lastError, const Path& filePath);

private:
	HANDLE				fileHandle;

private:
	Path				filePath;
};

#endif // _WINFILE_H_