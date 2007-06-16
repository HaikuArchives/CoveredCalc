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
	@file		PathUtils.cpp
	@brief		Path utilities
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.17 created
*/

#include "Prefix.h"
#include "PathUtils.h"
#include "PathException.h"

// ---------------------------------------------------------------------
//! Path separator
// ---------------------------------------------------------------------
#if defined (WIN32)							// Win32
static const AChar SEPARATOR = '\\';
#else										// except Win32 
static const AChar SEPARATOR = '/';
#endif

// ---------------------------------------------------------------------
//! Searches last separator.
/*!
	@retval -1	not found.
	@retval >=0	index of found separator
*/
// ---------------------------------------------------------------------
static SInt32 findLastSeparator(
	const MBCString& path			//!< original path string
)
{
	return path.RFind(SEPARATOR);
}

// ---------------------------------------------------------------------
//! If there is no separator in the last of path, adds it.
/*!
	@return result string.
*/
// ---------------------------------------------------------------------
MBCString PathUtils::AddSeparator(
	ConstAStr path			//!< original path string
)
{
	MBCString result = path;
	SInt32 separatorIndex = findLastSeparator(result);
	if (separatorIndex != result.Length() - 1)
	{
		result += SEPARATOR;
	}
	return result;
}

// ---------------------------------------------------------------------
//! If there is a separator in the last of path, removes it.
/*!
	@return result string.
*/
// ---------------------------------------------------------------------
MBCString PathUtils::RemoveSeparator(
	ConstAStr path			//!< original path string
)
{
	MBCString result = path;
	if (IsRoot(path))
	{
		return result;
	}
	
	if (findLastSeparator(result) == result.Length() - 1)
	{
		result = result.SubString(0, result.Length() - 1);
	}
	
	return result;
}

// ---------------------------------------------------------------------
//! Connects two paths.
/*!
	@return result string.
*/
// ---------------------------------------------------------------------
MBCString PathUtils::Append(
	ConstAStr path,			//!< path string
	ConstAStr more			//!< appended path string
)
{
	return AddSeparator(path) + more;
}

// ---------------------------------------------------------------------
//! Removes filename part and separator from path.
/*!
	This function removes filename part and separetor from path.
	Unlike GetParentDirectory(), if the last character of path is
	separator, it is simplly removed.

	For example:
		- C:/Hogehoge/File.txt -> C:/Hogehoge
		- C:/abc/def/ -> C:/abc/def
		- C:/ -> C:/
		- abc/hoge -> abc
		- abc -> 
		- /hoge -> /
		- C:hoge -> C:

	@return result string.
*/
// ---------------------------------------------------------------------
MBCString PathUtils::RemoveFileSpec(
	ConstAStr path			//!< original path string
)
{
	MBCString result = path;
	
	SInt32 findSeparator = findLastSeparator(result);
	SInt32 findColon = result.RFind(':');
	if (-1 == findSeparator && -1 == findColon)
	{
		result.Empty();
	}
	else
	{
		SInt32 pos;
		if (-1 == findSeparator)
		{
			pos = findColon;
		}
		else if (-1 == findColon)
		{
			pos = findSeparator;
		}
		else
		{
			pos = max(findColon, findSeparator);
		}
		result = RemoveSeparator(result.SubString(0, pos + 1));
	}
	
	return result;
}

// ---------------------------------------------------------------------
//! Returns parent directory path.
/*!
	This function returns parent directory path of specified path.
	Unlike RemoveFileSpec(), if the last character of path is
	separator, this function returns parent directory.

	For example:
		- C:/Hoge/foo/ -> C:/Hoge

	@return result string.
*/
// ---------------------------------------------------------------------
MBCString PathUtils::GetParentDirectory(
	ConstAStr path			//!< original path string
)
{
	return RemoveFileSpec(RemoveSeparator(path));
}

// ---------------------------------------------------------------------
//! Returns file name part of the path.
/*!
	This function returns file name part of specified path.

	For example:
		- C:/Hoge/foo.txt -> foo.txt
		- C:/Bar/Baz/ -> Baz

	@return result string.
*/
// ---------------------------------------------------------------------
MBCString PathUtils::GetFileTitle(
	ConstAStr path
)
{
	MBCString strRet = RemoveSeparator(path);
	int	idx = findLastSeparator(strRet);
	if ( -1 != idx )
	{
		strRet = strRet.SubString(idx + 1);
	}
	return strRet;	
}

// ---------------------------------------------------------------------
//! Determines whether path represents root directory.
/*!
	@retval true path represents root directory.
	@retval false path does not represent root directory.
*/
// ---------------------------------------------------------------------
bool PathUtils::IsRoot(ConstAStr path)
{
	MBCString test = path;
	SInt32 findColon = test.Find(':');
	SInt32 findSlash = test.Find(SEPARATOR);
	
	if (0 == findSlash && 1 == test.Length())
	{
		return true;
	}
	if (1 == findColon && 2 == findSlash && 3 == test.Length())
	{
		return true;
	}
	
	return false;
}

// ---------------------------------------------------------------------
//! Compares two path strings.
/*!
	@retval <0	left < right
	@retval 0	left == right
	@retval >0	left > right
*/
// ---------------------------------------------------------------------
SInt32 PathUtils::ComparePath(
	ConstAStr left,				//!< path 1
	ConstAStr right				//!< path 2
)
{
	// normalize
	MBCString normalLeft = RemoveSeparator(left);
	MBCString normalRight = RemoveSeparator(right);

	// compare
#if defined(WINVER)			// Windows
	return normalLeft.CompareNoCase(normalRight);
#else						// except Windows
	return normalLeft.Compare(normalRight);
#endif
}

// ---------------------------------------------------------------------
//! Counts depth of the path
/*!
	@return depth
*/
// ---------------------------------------------------------------------
static SInt32 countDepth(
	const MBCString path		//!< path string
)
{
	SInt32 ret = 0;
	SInt32 index = 0;
	while (true)
	{
		SInt32 find = path.Find(SEPARATOR, index);
		if (-1 == find)
		{
			return ret;
		}
		ret++;
		index = find + 1;
	}
}

// ---------------------------------------------------------------------
//! Makes relative path from base path.
/*!
	@return relative path
	@throw PathException when failed to make relative path
*/
// ---------------------------------------------------------------------
MBCString PathUtils::MakeRelativePath(
	ConstAStr base,			//!< base path (absolute path)
	ConstAStr target		//!< target path (absolute path)
)
{
	// normalize
	MBCString normalBase = RemoveSeparator(base);
	MBCString normalTarget = RemoveSeparator(target);

	MBCString front, latter;

	// count depth
	SInt32 baseDepth = countDepth(normalBase);
	SInt32 targetDepth = countDepth(normalTarget);
	
	// make base and target same depth
	if (baseDepth < targetDepth)
	{
		SInt32 index = 0;
		SInt32 count;
		for (count=0; count<=baseDepth; count++)
		{
			SInt32 find = normalTarget.Find(SEPARATOR, index);
			ASSERT(-1 != find);
			index = find + 1;
		}
		latter = normalTarget.SubString(index);
		normalTarget = normalTarget.SubString(0, index - 1);
	}
	else if (baseDepth > targetDepth)
	{
		SInt32 count;
		for (count=baseDepth; count>targetDepth; count--)
		{
			front = Append("..", front);
		}
		SInt32 index = 0;
		for (count=0; count<=targetDepth; count++)
		{
			SInt32 find = normalBase.Find(SEPARATOR, index);
			ASSERT(-1 != find);
			index = find + 1;
		}
		normalBase = normalBase.SubString(0, index - 1);
	}
	
	// compare until both path matches
	while (0 != ComparePath(normalBase, normalTarget))
	{
		if (IsRoot(normalBase) || IsRoot(normalTarget))
		{
			throw new PathExceptions::RelativePathNotExist(base, target);		
		}

		if (latter.IsEmpty())
		{
			latter = GetFileTitle(normalTarget);
		}
		else
		{
			latter = Append(GetFileTitle(normalTarget), latter);
		}
		normalTarget = GetParentDirectory(normalTarget);
		
		front = Append("..", front);
		normalBase = GetParentDirectory(normalBase);		
	}
	
	return Append(front, latter);
}
