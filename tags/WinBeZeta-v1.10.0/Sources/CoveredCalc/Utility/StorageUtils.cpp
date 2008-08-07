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
	@file		StorageUtils.cpp
	@brief		Storage utilities
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.2.16 created
*/

#include "Prefix.h"
#include "StorageUtils.h"
#include "Path.h"
#include "FileException.h"
#if defined(BEOS)
#include <Entry.h>
#include <Directory.h>
#endif

namespace StorageUtils {

/**
 *	@brief	Create folders to specified path if it is not exist.
 *	@param[in]	folderPath	path of a folder.
 */
void ReadyFolder(const Path& folderPath)
{
#if defined (WIN32)							// Win32
	LPCTSTR pathString = folderPath.GetPathString();
	DWORD attributes = ::GetFileAttributes(pathString);
	if (0xffffffff != attributes)				// it already exists
	{
		if (!(attributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			// it exists but is not a directory.
			throw new FileExceptions::FileAlreadyExists(folderPath);
		}
		
		// do nothing because it already exists.
		return;
	}
	else										// it may not exist
	{
		DWORD lastError = ::GetLastError();
		switch (lastError)
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			break;								// it doesn't exist
		case ERROR_ACCESS_DENIED:
			throw new FileExceptions::AccessDenied(folderPath);
			break;
		default:
			throw new FileException(folderPath);
			break;
		}
	}
	
	ASSERT(!folderPath.IsRoot());		// through root folder must exist...
	if (folderPath.IsRoot())
	{
		return;					// check for avoiding infinite recursion which may destroy the stack.
	}
	
	// ready parent folder
	ReadyFolder(folderPath.GetParent());

	// create the leaf folder.
	if (!::CreateDirectory(pathString, NULL))
	{
		DWORD lastError = ::GetLastError();
		switch (lastError)
		{
		case ERROR_ACCESS_DENIED:
			throw new FileExceptions::AccessDenied(folderPath);
			break;
		case ERROR_WRITE_FAULT:
			throw new FileExceptions::IOError(folderPath);
			break;
		case ERROR_SHARING_VIOLATION:
		case ERROR_LOCK_VIOLATION:
			throw new FileExceptions::SharingViolation(folderPath);
			break;
		case ERROR_ALREADY_EXISTS:
			throw new FileExceptions::FileAlreadyExists(folderPath);
			break;
		default:
			throw new FileException(folderPath);
			break;
		}
	}
#elif defined (BEOS)						// BeOS
	const char* pathString = folderPath.GetPathString();
	BEntry entry(pathString);
	if (entry.Exists())				// it already exists
	{
		if (!entry.IsDirectory())
		{
			// it should be a folder.
			throw new FileExceptions::FileAlreadyExists(folderPath);
		}

		// do nothing because it already exists.
		return;
	}

	// create
	status_t result = ::create_directory(pathString, 0755);
	if (B_OK != result)
	{
		switch (result)
		{
		case B_ENTRY_NOT_FOUND:
			throw new FileExceptions::FileNotFound(folderPath);
			break;
		
		case B_FILE_EXISTS:
			throw new FileExceptions::FileAlreadyExists(folderPath);
			break;
		
		case B_NOT_ALLOWED:
		case B_PERMISSION_DENIED:
			throw new FileExceptions::AccessDenied(folderPath);
			break;
	
		case B_DEVICE_FULL:
			throw new FileExceptions::DeviceFull(folderPath);
			break;
		
		default:
			throw new FileException(folderPath);
			break;
		}
	}
#else
	please implement this function. 
#endif
}

}
