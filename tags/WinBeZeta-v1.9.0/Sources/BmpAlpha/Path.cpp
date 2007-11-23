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
	@file		Path.cpp
	@brief		Implementation of Path class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.01.26 created
*/

#include "Prefix.h"
#include "Path.h"
#include "PathUtils.h"

// ---------------------------------------------------------------------
//! Constructor with no parameter
// ---------------------------------------------------------------------
Path::Path()
{
}

// ---------------------------------------------------------------------
//! Constructor with ConstAStr parameter
// ---------------------------------------------------------------------
Path::Path(
	ConstAStr path			//!< path string
)
{
	pathString = path;
}

// ---------------------------------------------------------------------
//! Copy constructor
// ---------------------------------------------------------------------
Path::Path(
	const Path& other		//!< other Path object
)
{
	pathString = other.pathString;
}

// ---------------------------------------------------------------------
//! Assigns specified path to this object
// ---------------------------------------------------------------------
void Path::Assign(
	ConstAStr path			//!< path string
)
{
	pathString = path;
}

// ---------------------------------------------------------------------
//! Assigns specified path to this object
// ---------------------------------------------------------------------
void Path::Assign(
	const Path& other		//!< other Path object
)
{
	if (this != &other)
	{
		pathString = other.pathString;
	}
}

// ---------------------------------------------------------------------
//! Assigns from slash separated path string
// ---------------------------------------------------------------------
void Path::AssignFromSlashSeparated(
	ConstAStr path			//!< slash separated path
)
{
	pathString = path;

#if defined (WIN32)							// Win32
	// convert slashes to back slashes
	SInt32 index = 0;
	while (true)
	{
		SInt32 find = pathString.Find('/', index);
		if (-1 == find)
		{
			return;
		}
		pathString[find] = '\\';
		index = find + 1;
	}
#endif	// defined Win32	
}

// ---------------------------------------------------------------------
//! Assignment operator
// ---------------------------------------------------------------------
Path& Path::operator=(
	const Path& other
)
{
	Assign(other);
	return *this;	
}

// ---------------------------------------------------------------------
//! Returns a path string.
/*!
	@return path string
*/
// ---------------------------------------------------------------------
ConstAStr Path::GetPathString() const
{
	return pathString;
}

// ---------------------------------------------------------------------
//! Returns a path string as slash separated.
// ---------------------------------------------------------------------
void Path::GetSlashSeparatedPathString(
	MBCString& path			//!< OUTPUT. result path string
) const
{
	path = pathString;

#if defined (WIN32)							// Win32
	// convert back slashes to slashes
	SInt32 index = 0;
	while (true)
	{
		SInt32 find = path.Find('\\', index);
		if (-1 == find)
		{
			return;
		}
		path[find] = '/';
		index = find + 1;
	}
#endif	// defined Win32	
}

// ---------------------------------------------------------------------
//! Returns a file title part in the path.
// ---------------------------------------------------------------------
void Path::GetFileName(
	MBCString& path			//!< OUTPUT. file title part in the path
) const
{
	path = PathUtils::GetFileTitle(pathString);
}

// ---------------------------------------------------------------------
//! Connect other path at the tail of this path.
/*!
	@return result path.
*/
// ---------------------------------------------------------------------
Path Path::Append(
	ConstAStr path		//!< other path string
) const
{
	return Path(PathUtils::Append(pathString, path));
}

// ---------------------------------------------------------------------
//! Connect other path at the tail of this path.
/*!
	@return result path.
*/
// ---------------------------------------------------------------------
Path Path::Append(
	const Path& other		//!< other Path object
) const
{
	return Path(PathUtils::Append(pathString, other.pathString));
}

// ---------------------------------------------------------------------
//! Returns Path object which is set to parent directory of this path.
/*!
	@return result path.
*/
// ---------------------------------------------------------------------
Path Path::GetParent() const
{
	return Path(PathUtils::GetParentDirectory(pathString));
}

// ---------------------------------------------------------------------
//! Returns whether this path is root path.
/*!
	@retval true root path.
	@retval false non-root path.
*/
// ---------------------------------------------------------------------
bool Path::IsRoot() const
{
	return PathUtils::IsRoot(pathString);
}

// ---------------------------------------------------------------------
//! Compares this object to other Path object
/*!
	@retval <0	this < other
	@retval 0	this == other
	@retval >0	this > other
*/
// ---------------------------------------------------------------------
SInt32 Path::Compare(
	const Path& other		//!< other path object
) const
{
	return PathUtils::ComparePath(pathString, other.pathString);
}

// ---------------------------------------------------------------------
//! Makes relative path of specified target from this path.
/*!
	@throw PathException when failed to make relative path
*/
// ---------------------------------------------------------------------
void Path::MakeRelativePath(
	const Path& target,		//!< target path
	Path& result			//!< OUTPUT. result path
) const
{
	result.Assign(PathUtils::MakeRelativePath(pathString, target.pathString));
}
