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
	@file		CoverDefParser.h
	@brief		Definition of CoverDefParser class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.08.18 created
*/

#ifndef _COVERDEFPARSER_H_
#define _COVERDEFPARSER_H_

class CoverDef;
class CoverInfo;
class CoverWindowInfo;
class CoverMainWindowInfo;
class NCDNode;
class NCDElement;
class UTF8String;
class Path;

#include "ColorValue.h"

/**
 *	@brief	This class parses "CoverDef.xml" and generates CoverDef object.
 */
class CoverDefParser
{
public:
								CoverDefParser();
	virtual						~CoverDefParser();
	
	void						Parse(const Path& filePath);
	CoverDef*					DetachCoverDef();

private:
	void						readTextNode(const NCDNode* theNode, UTF8String& outText);
	void						readTextNodeSub(const NCDNode* theNode, bool isPreserveSpace, UTF8String& outText);
	ColorValue					parseColorString(ConstUTF8Str colorString);
	bool						getAttrValue(const NCDNode* theNode, ConstUTF8Str attrName, UTF8String& outValue, bool isRequired);
	bool						getPathValue(const NCDNode* theNode, ConstUTF8Str attrName, Path& outPath, bool isRequired);

	CoverDef*					parseTagCoverDef(const NCDElement* coverDefElement);
	CoverInfo*					parseTagCover(const NCDNode* coverElement);
	void						parseTagWindow(CoverWindowInfo* newWindowInfo, const NCDNode* windowElement);
	CoverMainWindowInfo*		parseTagWindowMain(const NCDNode* windowElement);

private:
	CoverDef*					coverDef;
};

#endif // _COVERDEFPARSER_H_
