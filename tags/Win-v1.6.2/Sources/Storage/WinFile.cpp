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
	@file		WinFile.cpp
	@brief		Implementation of WinFile class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.9.25 created
*/

#include "Prefix.h"
#include "WinFile.h"
#include "FileException.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
WinFile::WinFile()
{
	fileHandle = INVALID_HANDLE_VALUE;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
WinFile::~WinFile()
{
	if (INVALID_HANDLE_VALUE != fileHandle)
	{
		::CloseHandle(fileHandle);
	}
}

// ---------------------------------------------------------------------
//! Opens specified file.
/*!
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
void WinFile::Open(
	const Path& filePath,				//!< the filename with relative or absolute path.
	FileConstants::OpenMode openMode,	//!< to open for read, write, or both.
	UInt32 openOption					//!< combination of OpenOption flags.
)
{
	Close();

	DWORD dwDesiredAccess;
	DWORD dwShareMode;
	DWORD dwCreateDisposition;
	analyzeOpenParameter(openMode, openOption, &dwDesiredAccess, &dwShareMode, &dwCreateDisposition);

	fileHandle = ::CreateFile(filePath.GetPathString(), dwDesiredAccess, dwShareMode, NULL, dwCreateDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == fileHandle)
	{
		throwNewFileException(::GetLastError(), filePath);
	}
	
	this->filePath = filePath;
}

// ---------------------------------------------------------------------
//! Analyzes the parameter of Open() function and returns the parameter of CreateFile() Win32API.
/*!
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
void WinFile::analyzeOpenParameter(
	FileConstants::OpenMode openMode,	//!< to open for read, write, or both.
	UInt32 openOption,					//!< combination of OpenOption flags.
	DWORD* dwDesiredAccess,				//!< OUTPUT. The parameter of CreateFile() Win32API.
	DWORD* dwShareMode,					//!< OUTPUT. The parameter of CreateFile() Win32API.
	DWORD* dwCreateDisposition			//!< OUTPUT. The parameter of CreateFile() Win32API.
)
{
	*dwDesiredAccess = 0;
	*dwShareMode = 0;
	*dwCreateDisposition = 0;

	switch (openMode)
	{
	case FileConstants::OpenMode_ReadOnly:
		*dwDesiredAccess = GENERIC_READ;
		*dwShareMode = FILE_SHARE_READ;
		break;
	case FileConstants::OpenMode_WriteOnly:
		*dwDesiredAccess = GENERIC_WRITE;
		*dwShareMode = 0;
		break;
	case FileConstants::OpenMode_ReadWrite:
		*dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
		*dwShareMode = 0;
		break;
	default:
		ASSERT(false);
	}

	if (openOption & FileConstants::OpenOption_Create)
	{
		if (openOption & FileConstants::OpenOption_FailIfExists)
		{
			*dwCreateDisposition = CREATE_NEW;
		}
		else if (openOption & FileConstants::OpenOption_Erase)
		{
			*dwCreateDisposition = CREATE_ALWAYS;
		}
		else
		{
			*dwCreateDisposition = OPEN_ALWAYS;
		}
	}
	else
	{
		if (openOption & FileConstants::OpenOption_FailIfExists)
		{
			ASSERT(false);	// ignores this option. what do the caller want to do?
		}
		if (openOption & FileConstants::OpenOption_Erase)
		{
			*dwCreateDisposition = TRUNCATE_EXISTING;
		}
		else
		{
			*dwCreateDisposition = OPEN_EXISTING;
		}
	}
}

// ---------------------------------------------------------------------
//! Closes the file if opened.
/*!
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
void WinFile::Close()
{
	BOOL ret = TRUE;
	DWORD lastError = ERROR_SUCCESS;
	
	if (INVALID_HANDLE_VALUE != fileHandle)
	{
		ret = ::CloseHandle(fileHandle);
		lastError = ::GetLastError();
		fileHandle = INVALID_HANDLE_VALUE;
	}

	if (!ret)
	{
		Path filePath = this->filePath;
		this->filePath.Empty();
		throwNewFileException(lastError, filePath);
	}
	
	this->filePath.Empty();	
}

// ---------------------------------------------------------------------
//! Reads data in the file.
/*!
	@return number of bytes read actually.
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
UInt32 WinFile::Read(
	void* buffer,		//!< OUTPUT. the read data is returned.
	UInt32 count		//!< number of bytes to read.
)
{
	ASSERT(INVALID_HANDLE_VALUE != fileHandle);
	
	DWORD numRead;
	if (!::ReadFile(fileHandle, buffer, count, &numRead, NULL))
	{
		throwNewFileException(::GetLastError());
	}
	return numRead;
}

// ---------------------------------------------------------------------
//! Writes data in the file.
/*!
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
void WinFile::Write(
	const void* buffer,		//!< the data to write.
	UInt32 count			//!< number of bytes to write.
)
{
	ASSERT(INVALID_HANDLE_VALUE != fileHandle);

	DWORD numWrite;
	if (!::WriteFile(fileHandle, buffer, count, &numWrite, NULL))
	{
		throwNewFileException(::GetLastError());
	}
}

// ---------------------------------------------------------------------
//! Moves the file pointer of the file.
/*!
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
void WinFile::Seek(
	SInt32 offset,						//!< number of bytes to move.
	FileConstants::SeekOrigin origin	//!< starting point for move.
)
{
	ASSERT(INVALID_HANDLE_VALUE != fileHandle);

	DWORD dwMoveMethod = 0;
	switch (origin)
	{
	case FileConstants::SeekOrigin_Begin:
		dwMoveMethod = FILE_BEGIN;
		break;
	case FileConstants::SeekOrigin_Current:
		dwMoveMethod = FILE_CURRENT;
		break;
	case FileConstants::SeekOrigin_End:
		dwMoveMethod = FILE_END;
		break;
	default:
		ASSERT(false);
	}
	
	if (INVALID_SET_FILE_POINTER == ::SetFilePointer(fileHandle, offset, NULL, dwMoveMethod))
	{
		DWORD lastError = ::GetLastError();
		if (ERROR_SUCCESS != lastError)
		{
			throwNewFileException(lastError);
		}
	}
}

// ---------------------------------------------------------------------
//! Returns the length of opened file in bytes.
/*!
	@return file length in bytes.
	@throw FileException when failed
*/
// ---------------------------------------------------------------------
UInt32 WinFile::GetSize() const
{
	ASSERT(INVALID_HANDLE_VALUE != fileHandle);
	
	DWORD ret = ::GetFileSize(fileHandle, NULL);
	if (0xffffffff == ret)
	{
		throwNewFileException(::GetLastError());
	}
	
	return ret;
}

// ---------------------------------------------------------------------
//! Creates new FileException or its derived object according to the last error.
/*!
	@return new FileException or its derived object.
*/
// ---------------------------------------------------------------------
void WinFile::throwNewFileException(
	DWORD lastError			//!< the return value of GetLastError() Win32API.
) const
{
	static const Path emptyPath;
	throwNewFileException(lastError, emptyPath);
}

// ---------------------------------------------------------------------
//! Creates new FileException or its derived object according to the last error.
/*!
	@return new FileException or its derived object.
*/
// ---------------------------------------------------------------------
void WinFile::throwNewFileException(
	DWORD lastError,		//!< the return value of GetLastError() Win32API.
	const Path& filePath	//!< this parameter is pass to FileException.
							/*! if this parameter is empty, this function uses
								filePath member variable. */
) const
{
	const Path& exFilePath = (filePath.IsEmpty()) ? this->filePath : filePath;
	
	switch (lastError)
	{
	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
	// case ERROR_NOT_READY:
		throw new FileExceptions::FileNotFound(exFilePath);
		break;
	
	case ERROR_ACCESS_DENIED:
		throw new FileExceptions::AccessDenied(exFilePath);
		break;

	case ERROR_WRITE_FAULT:
	case ERROR_READ_FAULT:
		throw new FileExceptions::IOError(exFilePath);
		break;
	
	case ERROR_SHARING_VIOLATION:
	case ERROR_LOCK_VIOLATION:
		throw new FileExceptions::SharingViolation(exFilePath);
		break;
	
	case ERROR_HANDLE_DISK_FULL:
	case ERROR_DISK_FULL:
		throw new FileExceptions::DeviceFull(exFilePath);
		break;
		
	case ERROR_ALREADY_EXISTS:
		throw new FileExceptions::FileAlreadyExists(exFilePath);
		break;
	
	default:
		throw new FileException(exFilePath);
		break;
	}
}
