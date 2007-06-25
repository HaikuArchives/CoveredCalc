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
	@file		Path.h
	@brief		Definition of Path class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.01.25 created
*/

#ifndef _PATH_H_
#define _PATH_H_

#include "MBCString.h"

// ---------------------------------------------------------------------
//! This class represents path name in file system.
// ---------------------------------------------------------------------
class Path
{
public:
						Path();

	explicit			Path(ConstAStr path);
						Path(const Path& other);

	void				Assign(ConstAStr path);
	void				Assign(const Path& other);
	void				AssignFromSlashSeparated(ConstAStr path);

	Path& 				operator=(const Path& other);

	ConstAStr			GetPathString() const;
	void				GetSlashSeparatedPathString(MBCString& path) const;
	void				GetFileName(MBCString& path) const;

	Path				Append(ConstAStr path) const;
	Path				Append(const Path& other) const;

	Path				GetParent() const;
	bool				IsRoot() const;

	void				Empty() { pathString.Empty(); }
	bool				IsEmpty() const { return pathString.IsEmpty(); }

	SInt32				Compare(const Path& other) const;

	void				MakeRelativePath(const Path& target, Path& result) const;

private:	
	MBCString			pathString;
};

/**
 *	@brief	The comparator for two Path.
 */
class PathComparator
{
public:
	bool operator()(const Path& left, const Path& right) const
	{
		return left.Compare(right) < 0 ? true : false;
	}
};

#endif // _PATH_H_
