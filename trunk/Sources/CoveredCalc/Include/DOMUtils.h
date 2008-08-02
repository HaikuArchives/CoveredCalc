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
	@file		DOMUtils.h
	@brief		DOM utilities.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.07.21 created
*/

#ifndef _DOMUTILS_H_
#define _DOMUTILS_H_

class NCDNode;
class NCDDocument;
class NCDElement;
class UTF8String;

#include "File.h"
#include <vector>

/// DOM Utilities.
namespace DOMUtils
{
	/// This interface is used to iterate over nodes.
	class NodeVisitor
	{
	public:
								NodeVisitor() { }
		virtual					~NodeVisitor() { }
		
		/**
		 * do something to specified node.
		 *
		 * @retval	true	continue iteration
		 * @retval	false	stop iteration
		 */
		virtual bool			Visit(
									NCDNode* node	///< target node
								) = 0;
	};

	typedef std::vector<NCDNode*>	NCDNodeVector;

	enum FormatMode
	{
		FormatMode_Inline = 0,
		FormatMode_Block
	};
	
	enum TextFormatOptions
	{
		TextFormatOption_None			= 0x00,
		TextFormatOption_PreserveSpace	= 0x01,
		TextFormatOption_Inline			= 0x02,
	};
	
	/**
	 * @brief This interface provides directions in formatting.
	 */
	class NodeFormatDirector
	{
	public:
									NodeFormatDirector() { }
		virtual						~NodeFormatDirector() { }
		
		/**
		 * @brief Returns format mode for specified node.
		 */
		virtual FormatMode			GetFormatMode(const NCDNode* node) = 0;
		
		/**
		 * @brief Returns text format options for specified node.
		 */
		virtual UInt32				GetTextFormatOptions(const NCDNode* node) = 0;
		
		/**
		 * @brief Returns the characters for one level indent.
		 */
		virtual ConstUTF8Str		GetIndentChars() = 0;
	};

	NCDNode*					GetFirstMatchNode(const NCDNode* baseNode, ConstUTF8Str path, bool isIgnoreCase);
	void						GetAllMatchNode(const NCDNode* baseNode, ConstUTF8Str path, bool isIgnoreCase, NCDNodeVector& outNodes);
	void						GetAttrValueIgnoreCase(const NCDNode* baseNode, ConstUTF8Str attrName, UTF8String& outValue);
	bool						ForEachChild(const NCDNode* baseNode, NodeVisitor* visitor);
	bool						ForEachAttribute(const NCDNode* baseElement, NodeVisitor* visitor);
	NCDElement*					SearchElementNext(const NCDNode* baseNode, ConstUTF8Str targetName, bool isIgnoreCase);
	NCDElement*					SearchElementPrev(const NCDNode* baseNode, ConstUTF8Str targetName, bool isIgnoreCase);
	void						ReadTextNode(const NCDNode* node, bool isPreserveSpace, UTF8String& text);
	void						WriteOutAsXML(const NCDDocument* document, File* outFile);
	void						FormatDocument(NCDDocument* document, NodeFormatDirector* director);
	void						FormatNodeContent(NCDNode* parentNode, NodeFormatDirector* director, SInt32 indentLevel);

#ifdef DEBUG
	void						OutputDOMTreeForDebug(const NCDNode* baseNode, File* outFile);
#endif	// DEBUG
}

#endif // _DOMUTILS_H_
