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
	@file		BeFile.h
	@brief		Definition of BeFile class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.22 created
*/

#ifndef _BEFILE_H_
#define _BEFILE_H_

#include <storage/File.h>
#include "Path.h"
#include "FileConstants.h"

class FileException;

// ---------------------------------------------------------------------
//! This class is used in reading/writing data of a file for BeOS.
// ---------------------------------------------------------------------
class BeFile
{
public:
						BeFile();
	virtual				~BeFile();
	
	void				Open(const Path& filePath, FileConstants::OpenMode openMode, UInt32 openOption);
	void				Close();
	
	UInt32				Read(void* buffer, UInt32 count);
	void				Write(const void* buffer, UInt32 count);
	
	void				Seek(SInt32 offset, FileConstants::SeekOrigin origin);
	
	UInt32				GetSize() const;

	static ConstAStr	GetLineBreak() { return "\n"; }

private:
	static	void		analyzeOpenParameter(FileConstants::OpenMode openMode, UInt32 openOption, uint32* openModeBe);
	void				throwNewFileException(status_t lastError) const;
	void				throwNewFileException(status_t lastError, const Path& filePath) const;

private:
	BFile				fileObj;

private:
	Path				filePath;
};

#endif // _BEFILE_H_
