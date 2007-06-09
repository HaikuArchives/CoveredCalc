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
	@file		BeFile.cpp
	@brief		Implementation of BeFile class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.10.21 created
*/

#include "Prefix.h"
#include "BeFile.h"
#include "FileException.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
BeFile::BeFile()
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
BeFile::~BeFile()
{
}

// ---------------------------------------------------------------------
//! Opens specified file.
/*!
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
void BeFile::Open(
	const Path& filePath,				//!< the filename with relative or absolute path.
	FileConstants::OpenMode openMode,	//!< to open for read, write, or both.
	UInt32 openOption					//!< combination of OpenOption flags.
)
{
	Close();

	uint32 openModeBe;
	analyzeOpenParameter(openMode, openOption, &openModeBe);

	status_t err = fileObj.SetTo(filePath.GetPathString(), openModeBe);
	if (B_NO_ERROR != err)
	{
		switch (err)
		{
		case B_BAD_VALUE:
			throw new FileExceptions::FileNotFound(filePath);
			break;
		default:
			throwNewFileException(err, filePath);
			break;
		}
	}
	
	this->filePath = filePath;
}

// ---------------------------------------------------------------------
//! Analyzes the parameter of Open() function and returns the parameter of BFile::SetTo() BeOS API.
/*!
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
void BeFile::analyzeOpenParameter(
	FileConstants::OpenMode openMode,	//!< to open for read, write, or both.
	UInt32 openOption,					//!< combination of OpenOption flags.
	uint32* openModeBe					//!< OUTPUT. the parameter of BFile::SetTo() BeOS API.
)
{
	switch (openMode)
	{
	case FileConstants::OpenMode_ReadOnly:
		*openModeBe = B_READ_ONLY;
		break;
	case FileConstants::OpenMode_WriteOnly:
		*openModeBe = B_WRITE_ONLY;
		break;
	case FileConstants::OpenMode_ReadWrite:
		*openModeBe = B_READ_WRITE;
		break;
	}
	
	if (openOption & FileConstants::OpenOption_Create)
	{
		*openModeBe |= B_CREATE_FILE;
	}
	if (openOption & FileConstants::OpenOption_FailIfExists)
	{
		*openModeBe |= B_FAIL_IF_EXISTS;
	}
	if (openOption & FileConstants::OpenOption_Erase)
	{
		*openModeBe |= B_ERASE_FILE;
	}
}

// ---------------------------------------------------------------------
//! Closes the file if opened.
/*!
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
void BeFile::Close()
{
	fileObj.Unset();
	this->filePath.Empty();
}

// ---------------------------------------------------------------------
//! Reads data in the file.
/*!
	@return number of bytes read actually.
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
UInt32 BeFile::Read(
	void* buffer,		//!< OUTPUT. the read data is returned.
	UInt32 count		//!< number of bytes to read.
)
{
	ASSERT(B_NO_ERROR == fileObj.InitCheck());

	ssize_t numRead = fileObj.Read(buffer, count);
	if (0 > numRead)
	{
		throwNewFileException(numRead);
	}
	return numRead;
}

// ---------------------------------------------------------------------
//! Writes data in the file.
/*!
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
void BeFile::Write(
	const void* buffer,		//!< the data to write.
	UInt32 count			//!< number of bytes to write.
)
{
	ASSERT(B_NO_ERROR == fileObj.InitCheck());

	ssize_t numWrite = fileObj.Write(buffer, count);
	if (0 > numWrite)
	{
		throwNewFileException(numWrite);
	}
}

// ---------------------------------------------------------------------
//! Moves the file pointer of the file.
/*!
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
void BeFile::Seek(
	SInt32 offset,						//!< number of bytes to move.
	FileConstants::SeekOrigin origin	//!< starting point for move.
)
{
	ASSERT(B_NO_ERROR == fileObj.InitCheck());

	int32 seekMode = 0;
	switch (origin)
	{
	case FileConstants::SeekOrigin_Begin:
		seekMode = SEEK_SET;
		break;
	case FileConstants::SeekOrigin_Current:
		seekMode = SEEK_CUR;
		break;
	case FileConstants::SeekOrigin_End:
		seekMode = SEEK_END;
		break;
	default:
		ASSERT(false);
	}
	
	status_t err = fileObj.Seek(offset, seekMode);
	if (0 > err)
	{
		throwNewFileException(err);
	}
}

// ---------------------------------------------------------------------
//! Returns the length of opened file in bytes.
/*!
	@return file length in bytes.
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
UInt32 BeFile::GetSize() const
{
	ASSERT(B_NO_ERROR == fileObj.InitCheck());
	
	off_t size;
	status_t err = fileObj.GetSize(&size);
	if (B_NO_ERROR != err)
	{
		throwNewFileException(err);
	}
	
	return size;
}

// ---------------------------------------------------------------------
//! Creates new FileException or its derived object according to the last error.
// ---------------------------------------------------------------------
void BeFile::throwNewFileException(
	status_t lastError					//!< the return value of BeOS API.
) const
{
	static const Path emptyPath;
	throwNewFileException(lastError, emptyPath);
}

// ---------------------------------------------------------------------
//! Creates new FileException or its derived object according to the last error.
// ---------------------------------------------------------------------
void BeFile::throwNewFileException(
	status_t lastError,					//!< the return value of BeOS API.
	const Path& filePath				//!< this parameter is pass to FileException.
) const
{
	const Path& exFilePath = (filePath.IsEmpty()) ? this->filePath : filePath;

	switch (lastError)
	{
	case B_ENTRY_NOT_FOUND:
		throw new FileExceptions::FileNotFound(exFilePath);
		break;
	
	case B_FILE_EXISTS:
		throw new FileExceptions::FileAlreadyExists(exFilePath);
		break;
	
	case B_PERMISSION_DENIED:
		throw new FileExceptions::AccessDenied(exFilePath);
		break;

	case B_DEVICE_FULL:
		throw new FileExceptions::DeviceFull(exFilePath);
		break;
	
	default:
		throw new FileException(exFilePath);
		break;
	}
}
