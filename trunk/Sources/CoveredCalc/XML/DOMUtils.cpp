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
	@file		DOMUtils.cpp
	@brief		DOM utilities.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.07.21 created
*/

#include "Prefix.h"
#include "DOMUtils.h"
#include "NCDNode.h"
#include "NCDDocument.h"
#include "NCDElement.h"
#include "NCDNamedNodeMap.h"
#include "NCDAttr.h"
#include "NCDText.h"
#include "TypeConv.h"
#include "UTF8String.h"
#include "UTF8Utils.h"
#include "MemoryException.h"
#include "ExpandingBuffer.h"

namespace DOMUtils {

/// The analyzed information about step in a path.
struct StepInfo
{
	UTF8String		NodeTest;		///< next node test
	SInt32			ContextPos;		///< context position
	bool			IsAttr;			///< whether node is attribute
	ConstUTF8Str	NextPath;		///< remaining path
};

/**
 *	@brief	Analyzes path and returns step information.
 */
static void GetNextPathStep(
	ConstUTF8Str path,				///< path
	StepInfo& step					///< analyzed step information is returned (OUTPUT)
)
{
	// get next step
	// ex) path = "foo[2]/bar/text()"
	//       -> step.NodeTest = "foo[2]"
	//          step.NextPath = "bar/text()"
	ConstUTF8Str delim = TypeConv::AsUTF8(strchr(TypeConv::AsASCII(path), '/'));
	if (NULL == delim)
	{
		delim = path + strlen(TypeConv::AsASCII(path));
	}
	step.NodeTest = UTF8String(path, delim - path);
	if (*delim != TypeConv::AsUTF8('\0'))
	{
		step.NextPath = delim + 1;
	}
	else
	{
		step.NextPath = delim;
	}

	// get context pos
	// ex) * step.NodeTest = "foo"
	//         -> step.NodeTest = "foo"
	//            step.ContextPos = 0
	//     * step.NodeTest = "bar[3]"
	//         -> step.NodeTest = "bar"
	//            step.ContextPos = 3
	step.ContextPos = 0;
	ConstUTF8Str nodeTest = step.NodeTest;
	ConstUTF8Str posExpr = TypeConv::AsUTF8(strchr(TypeConv::AsASCII(nodeTest), '['));
	if (NULL != posExpr)
	{
		step.ContextPos = atoi(TypeConv::AsASCII(posExpr + 1));
		if (0 == step.ContextPos)
		{
			ASSERT(FALSE);		// syntax error.
		}
		step.NodeTest = step.NodeTest.SubString(0, posExpr - nodeTest);
	}

	// the node test "text()" and "comment()" is available.
	// note that the node test "processing-instruction()" and "node()" is NOT available.
	if (0 == step.NodeTest.Compare(TypeConv::AsUTF8("text()")))
	{
		step.NodeTest = TypeConv::AsUTF8("#text");
	}
	else if (0 == step.NodeTest.Compare(TypeConv::AsUTF8("comment()")))
	{
		step.NodeTest = TypeConv::AsUTF8("#comment");
	}
	
	// check whether searching node is attribute
	// ex) * step.NodeTest = "foo"
	//         -> step.NodeTest = "foo"
	//            step.IsAttr = false;
	//     * step.NodeTest = "@bar"
	//         -> step.NodeTest = bar"
	//            step.IsAttr = true;
	step.IsAttr = false;
	if (TypeConv::AsUTF8('@') == step.NodeTest.CString()[0])
	{
		step.IsAttr = true;
		step.NodeTest = step.NodeTest.SubString(1);
	}	
}

/// To find single node by name
class SingleNodeFinder : public NodeVisitor
{
private:
	ConstUTF8Str	name;
	bool			isIgnoreCase;
	SInt32			contextPos;
	NCDNode*		foundNode;
	
public:
	SingleNodeFinder(ConstUTF8Str name, bool isIgnoreCase, SInt32 contextPos)
	{
		this->name = name;
		this->isIgnoreCase = isIgnoreCase;
		this->foundNode = NULL;
		this->contextPos = contextPos;
		ASSERT(0 < contextPos);
	}
	
	virtual ~SingleNodeFinder() { }
	
	/// returns found node.
	NCDNode* GetFoundNode() const { return foundNode; }
	
	/**
	 *	@brief	checks the name of target node, and if the searching node is found
	 *			returns false to stop iteration.
	 *	@retval	true	continue iteration
	 *	@retval	false	stop iteration
	 */
	virtual bool Visit(
		NCDNode* node	///< target node
	)
	{
		UTF8String targetName;
		node->getNodeName(targetName);
		if (   (isIgnoreCase && 0 == UTF8Utils::UTF8StrCmpI(targetName, name))
			|| (!isIgnoreCase && 0 == UTF8Utils::UTF8StrCmp(targetName, name)) )
		{
			contextPos--;
			if (0 >= contextPos)
			{
				foundNode = node;
				return false;
			}
		}
		
		return true;
	}
};

/**
 * Retrieves a first node which matches specified path expression.
 *
 * @return the first node which matches path. if no node is found, NULL is returned.
 * @note The baseNode parameter cannot be NULL though if the path parameter represents even absolute path,
 *			because it is needed to get document node from the baseNode.
 *			In this case, it is recommended to specify document node in baseNode parameter.
 */
NCDNode* GetFirstMatchNode(
	const NCDNode* baseNode,	///< base node.
	ConstUTF8Str path,			///< XPath-like expression (such as "foo/bar[2]/@attr")
	bool isIgnoreCase			///< when true, name comparation is case-insensitive.
)
{
	// if absolute path is specified, convert it to relative path
	if (TypeConv::AsUTF8('/') == path[0])
	{
		if (NCDNode::DOCUMENT_NODE != baseNode->getNodeType())
		{
			baseNode = baseNode->getOwnerDocument();
		}
		path++;
	}

	while (TypeConv::AsUTF8('\0') != path[0] && NULL != baseNode)
	{
		StepInfo stepInfo;
		GetNextPathStep(path, stepInfo);
		path = stepInfo.NextPath;
		if (0 == stepInfo.ContextPos)
		{
			stepInfo.ContextPos = 1;	// the first one.
		}
		
		// search node
		SingleNodeFinder finder(stepInfo.NodeTest, isIgnoreCase, stepInfo.ContextPos);
		if (stepInfo.IsAttr)
		{
			ForEachAttribute(baseNode, &finder);
			baseNode = finder.GetFoundNode();
		}
		else
		{
			if (0 != UTF8Utils::UTF8StrCmp(stepInfo.NodeTest, TypeConv::AsUTF8(".")))
			{
				if (0 == UTF8Utils::UTF8StrCmp(stepInfo.NodeTest, TypeConv::AsUTF8("..")))
				{
					baseNode = baseNode->getParentNode();
				}
				else
				{
					ForEachChild(baseNode, &finder);
					baseNode = finder.GetFoundNode();
				}
			}
		}
	}
	
	return const_cast<NCDNode*>(baseNode);
}

/// To find multiple nodes by name
class MultipleNodeFinder : public NodeVisitor
{
private:
	ConstUTF8Str	name;
	bool			isIgnoreCase;
	NCDNodeVector*	foundNodes;
	
public:
	MultipleNodeFinder(ConstUTF8Str name, bool isIgnoreCase, NCDNodeVector& outNodes)
	{
		this->name = name;
		this->isIgnoreCase = isIgnoreCase;
		this->foundNodes = &outNodes;
	}
	
	virtual ~MultipleNodeFinder() { }
	
	/**
	 *	@brief	checks the name of target node, and if the searching node is found
     *			returns false to stop iteration.
	 *	@retval	true	continue iteration
	 *	@retval	false	stop iteration
	 */
	virtual bool Visit(
		NCDNode* node	///< target node
	)
	{
		UTF8String targetName;
		node->getNodeName(targetName);
		if (   (isIgnoreCase && 0 == UTF8Utils::UTF8StrCmpI(targetName, name))
			|| (!isIgnoreCase && 0 == UTF8Utils::UTF8StrCmp(targetName, name)) )
		{
			foundNodes->push_back(node);
		}
		
		return true;
	}
};

/**
 * Retrieves a all node which matches specified path expression.
 *
 * @note The baseNode parameter cannot be NULL though if the path parameter represents even absolute path,
 *			because it is needed to get document node from the baseNode.
 *			In this case, it is recommended to specify document node in baseNode parameter.
 */
void GetAllMatchNode(
	const NCDNode* baseNode,	///< base node.
	ConstUTF8Str path,			///< XPath-like expression (such as "foo/bar[2]/@attr")
	bool isIgnoreCase,			///< when true, name comparation is case-insensitive.
	NCDNodeVector& outNodes		///< OUTPUT. found node is added to this vector.
)
{
	// if absolute path is specified, convert it to relative path
	if (TypeConv::AsUTF8('/') == path[0])
	{
		if (NCDNode::DOCUMENT_NODE != baseNode->getNodeType())
		{
			baseNode = baseNode->getOwnerDocument();
		}
		path++;
	}

	if (TypeConv::AsUTF8('\0') != path[0] && NULL != baseNode)
	{
		StepInfo stepInfo;
		GetNextPathStep(path, stepInfo);
		path = stepInfo.NextPath;
		
		// search node
		NCDNodeVector childNodes;
		if (stepInfo.IsAttr)
		{
			if (0 != stepInfo.ContextPos)
			{
				SingleNodeFinder finder(stepInfo.NodeTest, isIgnoreCase, stepInfo.ContextPos);
				ForEachAttribute(baseNode, &finder);
				NCDNode* foundNode = finder.GetFoundNode();
				if (NULL != foundNode)
				{
					childNodes.push_back(foundNode);
				}
			}
			else
			{
				MultipleNodeFinder finder(stepInfo.NodeTest, isIgnoreCase, childNodes);
				ForEachAttribute(baseNode, &finder);				
			}
		}
		else
		{
			if (0 != UTF8Utils::UTF8StrCmp(stepInfo.NodeTest, TypeConv::AsUTF8(".")))
			{
				if (0 == UTF8Utils::UTF8StrCmp(stepInfo.NodeTest, TypeConv::AsUTF8("..")))
				{
					childNodes.push_back(baseNode->getParentNode());
				}
				else
				{
					if (0 != stepInfo.ContextPos)
					{
						SingleNodeFinder finder(stepInfo.NodeTest, isIgnoreCase, stepInfo.ContextPos);
						ForEachChild(baseNode, &finder);
						NCDNode* foundNode = finder.GetFoundNode();
						if (NULL != foundNode)
						{
							childNodes.push_back(foundNode);
						}
					}
					else
					{
						MultipleNodeFinder finder(stepInfo.NodeTest, isIgnoreCase, childNodes);
						ForEachChild(baseNode, &finder);
					}
				}
			}
		}

		if (!childNodes.empty())
		{
			if ('\0' == path[0])
			{
				outNodes.assign(childNodes.begin(), childNodes.end());
			}
			else
			{
				NCDNodeVector::const_iterator ite;
				for (ite=childNodes.begin(); ite!=childNodes.end(); ite++)
				{
					GetAllMatchNode(*ite, path, isIgnoreCase, outNodes);
				}
			}
		}
		else
		{
			outNodes.clear();
		}
	}
}

/**
 *	@brief	Retrieves a value of specified attribute. The name comparison is case-insensitive.
 */
void GetAttrValueIgnoreCase(
	const NCDNode* baseNode,	///< which should be element node
	ConstUTF8Str attrName,		///< attribute name
	UTF8String& outValue		///< value is returned (OUTPUT)
)
{
	SingleNodeFinder finder(attrName, true, 1);
	ForEachAttribute(baseNode, &finder);
	NCDNode* attrNode = finder.GetFoundNode();
	if (NULL == attrNode)
	{
		outValue.Empty();
	}
	else
	{
		attrNode->getNodeValue(outValue);
	}
}

/**
 * @brief	Iterates over all child of specified node
 * @retval	true	we iterated over all child.
 * @retval	false	the iteration is stopped by user.
 */
bool ForEachChild(
	const NCDNode* baseNode,	///< base node
	NodeVisitor* visitor		///< visitor interface
)
{
	NCDNode* node = baseNode->getFirstChild();
	while (NULL != node)
	{
		if (!visitor->Visit(node))
			return false;
			
		node = node->getNextSibling();
	}
	
	return true;
}

/**
 * Iterates over all attribute of specified element node
 *
 * @retval	true	we iterated over all child.
 * @retval	false	the iteration is stopped by user.
 */
bool ForEachAttribute(
	const NCDNode* baseElement,	///< base node which should be element node
	NodeVisitor* visitor		///< visitor interface
)
{
	NCDNamedNodeMap* attrMap = baseElement->getAttributes();
	if (NULL != attrMap)
	{
		UInt32 count = attrMap->getLength();
		UInt32 index;
		for (index=0; index<count; index++)
		{
			if (!visitor->Visit(attrMap->item(index)))
				return false;
		}
	}
	
	return true;
}

/**
 *	@brief	Searches next element which name is specified in parameter.
 *	@param	baseNode		start node.
 *	@param	targetName		name of target node.
 *	@param	isIgnoreCase	the name-comparison is case-sensitive when this is false.
 *	@note	The suffix "Next" stands for the direction of search. This function may return baseNode directly when it has the target name.
 */
NCDElement* SearchElementNext(const NCDNode* baseNode, ConstUTF8Str targetName, bool isIgnoreCase)
{
	UTF8String name;
	const NCDNode* node = baseNode;
	while (NULL != node)
	{
		if (NCDNode::ELEMENT_NODE == node->getNodeType())
		{
			node->getNodeName(name);
			bool isMatch = (isIgnoreCase) ? (0 == UTF8Utils::UTF8StrCmpI(name, targetName)) : (0 == UTF8Utils::UTF8StrCmp(name, targetName));
			if (isMatch)
			{
				return static_cast<NCDElement*>(const_cast<NCDNode*>(node));
			}
		}
		node = node->getNextSibling();
	}
	return NULL;
}

/**
 *	@brief	Searches previous element which name is specified in parameter.
 *	@param	baseNode		start node.
 *	@param	targetName		name of target node.
 *	@param	isIgnoreCase	the name-comparison is case-sensitive when this is false.
 *	@note	The suffix "Prev" stands for the direction of search. This function may return baseNode directly when it has the target name.
 */
NCDElement* SearchElementPrev(const NCDNode* baseNode, ConstUTF8Str targetName, bool isIgnoreCase)
{
	UTF8String name;
	const NCDNode* node = baseNode;
	while (NULL != node)
	{
		if (NCDNode::ELEMENT_NODE == node->getNodeType())
		{
			node->getNodeName(name);
			bool isMatch = (isIgnoreCase) ? (0 == UTF8Utils::UTF8StrCmpI(name, targetName)) : (0 == UTF8Utils::UTF8StrCmp(name, targetName));
			if (isMatch)
			{
				return static_cast<NCDElement*>(const_cast<NCDNode*>(node));
			}
		}
		node = node->getPreviousSibling();
	}
	return NULL;
}

typedef ExpandingBuffer<UTF8Char>	ExUTF8Buffer;

/**
 *	@brief	Retrieves text contents in specified node.
 *	@param[in]	node		node.
 *	@param[in]	isPreserveSpace	whether space is preserved.
 *	@param[out]	text	text contents is returned.
 */
void ReadTextNode(const NCDNode* node, bool isPreserveSpace, UTF8String& text)
{
	if (NCDNode::TEXT_NODE == node->getNodeType())
	{
		UTF8String childText;
		node->getNodeValue(childText);
		if (isPreserveSpace)
		{
			text = childText;
		}
		else
		{
			ExUTF8Buffer buffer;
			buffer.Init(64, 32);
			
			SInt32 length = childText.Length();
			bool isLineHead = true;
			bool isInSpaces = false;
			SInt32 index;
			for (index=0; index<length; index++)
			{
				UTF8Char ch = childText[index];
				bool isSpace = ('\x20' == ch || '\x09' == ch || '\x0a' == ch);

				if (isSpace)
				{
					isInSpaces = true;
				}
				else
				{
					if (isInSpaces)
					{
						// ignore spaces which appear in line head.
						if (!isLineHead)	
						{
							UTF8Char space = '\x20';
							buffer.Append(&space, 1);
						}
						isInSpaces = false;
					}
					
					buffer.Append(&ch, 1);
					
					if ('\n' == ch)
					{
						isLineHead = true;
					}
					else
					{
						isLineHead = false;
					}
				}
			}
			UTF8Char nullChar = '\0';
			buffer.Append(&nullChar, 1);
			
			text = buffer.GetBufferPtr();
		}
	}
	else
	{
		text.Empty();
		UTF8String aText;
		NCDNode* childNode = node->getFirstChild();
		while (NULL != childNode)
		{
			ReadTextNode(childNode, isPreserveSpace, aText);
			text += aText;

			childNode = childNode->getNextSibling();
		}
	}
}

/**
 * Outputs text in DOM.
 *
 * if text includes line break character, replace it to OS-specific line break character.
 */
static void WriteOutDOMText(
	ConstUTF8Str text,			///< output text
	File* outFile				///< destination file
)
{
	ConstAStr lineBreak = outFile->GetLineBreak();
	UInt32 lineBreakLength = strlen(lineBreak);

	ConstAStr textA = TypeConv::AsASCII(text);
	
	while ('\0' != *textA)
	{
		ConstAStr lineBreakPos = strchr(textA, '\n');
		if (NULL == lineBreakPos)
		{
			outFile->Write(textA, strlen(textA));
			break;
		}
		else
		{
			outFile->Write(textA, (lineBreakPos - textA) * sizeof(AChar));
			outFile->Write(lineBreak, lineBreakLength);
			textA = lineBreakPos + 1;
		}
	}
}

/**
 * Outputs specified DOM tree as XML format.
 */
void WriteOutAsXML(
	const NCDDocument* document,	///< document node
	File* outFile					///< output destination file (already opened)
)
{
	/// Collect element's attributes
	class AttrCollector : public NodeVisitor
	{
	private:
		UTF8String*	outString;	///< output string
	
	public:
		AttrCollector(
			UTF8String* outString	///< output string
		)
		{
			this->outString = outString;
		}
		
		virtual bool Visit(
			NCDNode* node			///< target node
		)
		{
			if (NCDNode::ATTRIBUTE_NODE == node->getNodeType())	// should be always true
			{
				NCDAttr* attrNode = static_cast<NCDAttr*>(node);
				
				UTF8String name;
				UTF8String value;
				attrNode->getName(name);
				attrNode->getValue(value);
				
				*outString += TypeConv::AsUTF8(" ");
				*outString += name;
				*outString += TypeConv::AsUTF8("=\"");
				*outString += value;
				*outString += TypeConv::AsUTF8("\"");
			}
			return true;			
		}
	};

	/// Node outputting visitor
	class NodeOutputter : public NodeVisitor
	{
	private:
		File* outFile;			///< output destination file
		
	public:
		NodeOutputter(
			File* outFile		///< output destination file
		)
		{
			this->outFile = outFile;
		}
		
		virtual bool Visit(
			NCDNode* node		///< target node
		)
		{
			return Visit(static_cast<const NCDNode*>(node));
		}
		
		bool Visit(
			const NCDNode* node	///< target node
		)
		{
			NCDNode::NodeType nodeType = node->getNodeType();
			switch (nodeType)
			{
			case NCDNode::DOCUMENT_NODE:
				{
					// output xml declaration
					UTF8Char xmlDeclaration[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
					WriteOutDOMText(xmlDeclaration, outFile);

					// output all node.
					NodeOutputter childOutputter(outFile);
					ForEachChild(node, &childOutputter);					
				}
				break;
			
			case NCDNode::ELEMENT_NODE:
				{
					UTF8String name;
					node->getNodeName(name);
					UTF8String elementBegin = TypeConv::AsUTF8("<");
					elementBegin += name;
					
					// collect attributes
					AttrCollector attrCollector(&elementBegin);
					ForEachAttribute(node, &attrCollector);
					
					if (node->hasChildNodes())
					{
						elementBegin += TypeConv::AsUTF8(">");
						outFile->Write(elementBegin, elementBegin.Length());
						
						NodeOutputter childOutputter(outFile);
						ForEachChild(node, &childOutputter);
						
						UTF8String elementEnd = TypeConv::AsUTF8("</");
						elementEnd += name;
						elementEnd += TypeConv::AsUTF8(">");
						outFile->Write(elementEnd, elementEnd.Length());
					}
					else
					{
						elementBegin += TypeConv::AsUTF8(" />");
						outFile->Write(elementBegin, elementBegin.Length());						
					}
				}
				break;
			
			case NCDNode::TEXT_NODE:
				{
					UTF8String value;
					node->getNodeValue(value);
					WriteOutDOMText(value, outFile);
				}
				break;
			
			case NCDNode::COMMENT_NODE:
				{
					UTF8String value;
					node->getNodeValue(value);
					
					UTF8String comment = TypeConv::AsUTF8("<!--");
					comment += value;
					comment += TypeConv::AsUTF8("-->");
					outFile->Write(comment, comment.Length());
				}
				break;
			
			default:
				break;
			}
			
			return true;			
		}	
	};

	NodeOutputter outputter(outFile);
	outputter.Visit(document);
	WriteOutDOMText(TypeConv::AsUTF8("\n"), outFile);
}

/**
 * @brief Format DOM tree.
 * @param[in] document document object of the tree.
 * @param[in] director	formatting director.
 */
void FormatDocument(NCDDocument* document, NodeFormatDirector* director)
{
	FormatNodeContent(document->getDocumentElement(), director, 0);
}

/**
 * @brief Format child nodes of specified parent node.
 * @param[in] parentNode parent node. this node must be a instance of NCDDocument or NCDElement.
 * @param[in] director	formatting director.
 * @param[in] indentLevel a number of indent depth.
 */
void FormatNodeContent(NCDNode* parentNode, NodeFormatDirector* director, SInt32 indentLevel)
{
	ConstUTF8Str indentChars = director->GetIndentChars();
	UInt32 textFormatOptions = director->GetTextFormatOptions(parentNode);
	bool isContentPreserveSpace = (0 != (textFormatOptions & TextFormatOption_PreserveSpace));
	bool isTextFormatInline = (0 != (textFormatOptions & TextFormatOption_Inline));
	
	// pretreatment: strip line-breaks and spaces.
	if (!isContentPreserveSpace)
	{
		NCDNode* nextNode = parentNode->getFirstChild();
		while (NULL != nextNode)
		{
			NCDNode* node = nextNode;
			nextNode = node->getNextSibling();
			
			NCDNode::NodeType nodeType = node->getNodeType();
			if (NCDNode::TEXT_NODE == nodeType)
			{
				UTF8String newValue;
				ReadTextNode(node, false, newValue);
				if (newValue.IsEmpty())
				{
					parentNode->removeChild(node);
					node->release();
				}
				else
				{
					node->setNodeValue(newValue.CString());
				}
			}
		}
	}
	
	// format children
	bool postIndent = false;
	NCDNode* nextNode = parentNode->getFirstChild();
	while (NULL != nextNode)
	{
		NCDNode* node = nextNode;
		nextNode = node->getNextSibling();
		bool preIndent = false;

		NCDNode::NodeType nodeType = node->getNodeType();
		if (NCDNode::ELEMENT_NODE == nodeType)
		{
			FormatMode elemFormatMode;
			if (isContentPreserveSpace)
			{
				elemFormatMode = FormatMode_Inline;
			}
			else
			{
				elemFormatMode = director->GetFormatMode(node);
			}
			if (FormatMode_Block == elemFormatMode)
			{
				preIndent = true;
				postIndent = true;
			}
			FormatNodeContent(node, director, indentLevel + ((FormatMode_Block == elemFormatMode) ? 1 : 0));
		}
		else if (NCDNode::TEXT_NODE == nodeType)
		{
			if (!isContentPreserveSpace && !isTextFormatInline)
			{
				preIndent = true;
				postIndent = true;
			}
			
			if (!isContentPreserveSpace)
			{
				// TEXT FORMATTING HERE IF REQUIRED.
				
				// Is text formatting required?
				// Currently no. So it does nothing here.
			}
		}
				
		if (preIndent)
		{
			UTF8String preSpaces = TypeConv::AsUTF8("\n");
			int ix;
			for (ix = 0; ix < indentLevel + 1; ix++)
			{
				preSpaces += indentChars;
			}
			NCDNode* prevNode = node->getPreviousSibling();
			if (NULL != prevNode && NCDNode::TEXT_NODE == prevNode->getNodeType())
			{
				UTF8String value;
				prevNode->getNodeValue(value);
				value += preSpaces;
				prevNode->setNodeValue(value.CString());
			}
			else
			{
				NCDDocument* doc = (NCDNode::DOCUMENT_NODE == parentNode->getNodeType()) ? static_cast<NCDDocument*>(parentNode) : parentNode->getOwnerDocument();
				parentNode->insertBefore(doc->createTextNode(preSpaces.CString()), node);
			}
		}
	}
	if (postIndent)
	{
		UTF8String postSpaces = TypeConv::AsUTF8("\n");
		int ix;
		for (ix = 0; ix < indentLevel; ix++)
		{
			postSpaces += indentChars;
		}
		NCDNode* lastChild = parentNode->getLastChild();
		if (NULL != lastChild && NCDNode::TEXT_NODE == lastChild->getNodeType())
		{
			UTF8String value;
			lastChild->getNodeValue(value);
			value += postSpaces;
			lastChild->setNodeValue(value.CString());
		}
		else
		{
			NCDDocument* doc = (NCDNode::DOCUMENT_NODE == parentNode->getNodeType()) ? static_cast<NCDDocument*>(parentNode) : parentNode->getOwnerDocument();
			parentNode->appendChild(doc->createTextNode(postSpaces.CString()));
		}
	}
}

#ifdef DEBUG
/**
 * Outputs DOM tree to specified file (for debug)
 */
void OutputDOMTreeForDebug(
	const NCDNode* baseNode,	///< this node and its descendant nodes are outputted.
	File* outFile				///< output destination file (already opened)
)
{
	/// Outputting visitor
	class NodeOutputter : public NodeVisitor
	{
	private:
		SInt32	depth;			///< depth of visiting node
		File* outFile;			///< output destination file
		
	public:
		NodeOutputter(
			SInt32 depth,		///< depth of visiting node
			File* outFile		///< output destination file
		)
		{
			this->depth = depth;
			this->outFile = outFile;
		}
		
		virtual bool Visit(
			NCDNode* node		///< target node
		)
		{
			return Visit(static_cast<const NCDNode*>(node));
		}
		
		bool Visit(
			const NCDNode* node	///< target node
		)
		{
			// output this node.
			{
				// indent
				UTF8Char spChar = TypeConv::AsUTF8(' ');
				for (SInt32 sp=0; sp<depth*2; sp++)
				{
					outFile->Write(&spChar, sizeof(spChar));
				}
			
				NCDNode::NodeType nodeType = node->getNodeType();
				UTF8String name;
				node->getNodeName(name);
				
				AChar typeStr[16 + 3];
				ConstAStr lineBreak = outFile->GetLineBreak();
				sprintf(typeStr, "%d : ", nodeType);
				outFile->Write(typeStr, strlen(typeStr));
				outFile->Write(name, name.Length());
				outFile->Write(lineBreak, strlen(lineBreak));
			}
			
			// output children
			NodeOutputter childOutputter(depth + 1, outFile);
			ForEachChild(node, &childOutputter);
			
			return true;			
		}
	};
	
	NodeOutputter outputter(0, outFile);
	outputter.Visit(baseNode);
}
#endif

}	// end of namespace DOMUtils
