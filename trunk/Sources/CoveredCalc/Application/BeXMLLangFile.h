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

/**
	@file		BeXMLLangFile.h
	@brief		Definition of BeXMLLangFile class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.12.31 created
*/

#ifndef _BEXMLLANGFILE_H_
#define _BEXMLLANGFILE_H_

#include "XMLLangFile.h"

class BeDialogDesign;

class BeXMLLangFile : public XMLLangFile
{
public:
						BeXMLLangFile();
						~BeXMLLangFile();

	virtual void		LoadString(ConstUTF8Str name, MBCString& message) const;
	void				LoadStringFromID(SInt32 stringID, MBCString& message) const;
	BeDialogDesign*		LoadDialogDesign(SInt32 dialogID) const;

protected:
	virtual bool		checkVersion(ConstUTF8Str version);
};

#endif // _BEXMLLANGFILE_H_
