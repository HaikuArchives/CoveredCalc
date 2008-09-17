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
	@file		CoverDefParser.cpp
	@brief		Implementation of CoverDefParser class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.08.18 created
*/

#include "Prefix.h"
#include "CoverDefParser.h"
#include "DOMParseEventHandler.h"
#include "DOMUtils.h"
#include "CoverDef.h"
#include "XMLParser.h"
#include "NCDDocument.h"
#include "NCDElement.h"
#include "DOMUtils.h"
#include "CoverDefParseException.h"
#include "UTF8Utils.h"
#include "CoverWindowInfo.h"
#include "CoverToolTipInfo.h"
#include "CoverMainWindowInfo.h"
#include "CoverFontInfo.h"
#include "CoverButtonInfo.h"
#include "CoverDisplayInfo.h"
#include "UTF8Conv.h"
#include "AppSettings.h"
#include "CoveredCalcApp.h"

/**
 *	@brief	Constructor
 */
CoverDefParser::CoverDefParser()
{
	coverDef = NULL;
}

/**
 *	@brief	Destructor
 */
CoverDefParser::~CoverDefParser()
{
	if (NULL != coverDef)
	{
		delete coverDef;
	}
}

/**
 *	@brief	Parses "CoverDef.xml" and generate CoverDef object.
 *	@note	The generated CoverDef object is returned by DetachCoverDef().
 */
void CoverDefParser::Parse(
	const Path& filePath		///< file path of "CoverDef.xml"
)
{
	if (NULL != coverDef)
	{
		delete coverDef;
		coverDef = NULL;
	}

	// read "CoverDef.xml"
	File file;
	file.Open(filePath, FileConstants::OpenMode_ReadOnly, FileConstants::OpenOption_None);
	UInt32 dataSize = file.GetSize();
	XMLParser parser;
	parser.Create();
	Byte* buf = static_cast<Byte*>(parser.GetParseBuffer(dataSize));
	file.Read(buf, dataSize);
	file.Close();

	// parse it into DOM tree
	DOMParseEventHandler handler;
	parser.SetEventHandler(&handler);
	parser.ParseBuffer(dataSize, true);

	// from DOM tree, generate a CoverDef object
	NCDDocument* document = handler.DetachDocument();
	try
	{
		// element "CoverDef"
		NCDElement* documentElement = document->getDocumentElement();
		if (NULL == documentElement)
		{
			throw new CoverDefParseExceptions::InvalidDocumentElement();
		}
		UTF8String documentElementName;
		documentElement->getNodeName(documentElementName);
		if (0 != UTF8Utils::UTF8StrCmpI(documentElementName, TypeConv::AsUTF8("coverDef")))
		{
			throw new CoverDefParseExceptions::InvalidDocumentElement(documentElement->getSourceLine(), documentElement->getSourceColumn());
		}
		
		coverDef = parseTagCoverDef(documentElement);
	}
	catch (...)
	{
		document->release();
		if (NULL != coverDef)
		{
			delete coverDef;
			coverDef = NULL;
		}
		
		throw;
	}
	document->release();
}

/**
 *	@brief	Returns text contents in specified node. (main routine)
 *
 *	The tag "br" and "pre" is understood and is reflected to the result string.
 *	The sequence of white-space characters in a line is treated as a single space
 *	except that it is appeared in "pre" element.
 */
void CoverDefParser::readTextNodeSub(
	const NCDNode* theNode,				///< node
	bool isPreserveSpace,				///< whether space is preserved.
	UTF8String& outText					///< text in the node is returned.
)
{
	NCDNode* childNode = theNode->getFirstChild();
	while (NULL != childNode)
	{
		NCDNode::NodeType nodeType = childNode->getNodeType();
		if (NCDNode::TEXT_NODE == nodeType)
		{
			UTF8String childText;
			DOMUtils::ReadTextNode(childNode, isPreserveSpace, childText);
			outText += childText;
		}
		else if (NCDNode::ELEMENT_NODE == nodeType)
		{
			UTF8String childName;
			childNode->getNodeName(childName);
			if (0 == UTF8Utils::UTF8StrCmpI(childName, TypeConv::AsUTF8("br")))
			{
				outText += TypeConv::AsUTF8('\n');
			}
			else if (0 == UTF8Utils::UTF8StrCmpI(childName, TypeConv::AsUTF8("pre")))
			{
				readTextNodeSub(childNode, true, outText);
			}
		}
	
		childNode = childNode->getNextSibling();
	}
}

/**
 *	@brief	Returns text contents in specified node.
 *
 *	The tag "br" and "pre" is understood and is reflected to the result string.
 *	The sequence of white-space characters in a line is treated as a single space
 *	except that it is appeared in "pre" element.
 */
void CoverDefParser::readTextNode(
	const NCDNode* theNode,				///< node
	UTF8String& outText					///< text in the node is returned.
)
{
	outText.Empty();

	// search altanate text of current language.
	NCDNode* altTextNode = NULL;
	UTF8String nodeId;
	ASSERT(NCDNode::ELEMENT_NODE == theNode->getNodeType());
	DOMUtils::GetAttrValueIgnoreCase(theNode, TypeConv::AsUTF8("id"), nodeId);
	if (!nodeId.IsEmpty())
	{
		MBCString languageString;
		CoveredCalcApp::GetInstance()->GetCurrentLanguageCode(languageString);
		UTF8String languageStringUtf8;
		UTF8Conv::FromMultiByte(languageStringUtf8, languageString.CString());
		if (!languageString.IsEmpty())
		{
			DOMUtils::NCDNodeVector languageNodeVector;
			DOMUtils::GetAllMatchNode(theNode, TypeConv::AsUTF8("/coverDef/language"), true, languageNodeVector);
			DOMUtils::NCDNodeVector::const_iterator ite;
			for (ite=languageNodeVector.begin(); ite!=languageNodeVector.end(); ite++)
			{
				NCDNode* language = *ite;
				if (NCDNode::ELEMENT_NODE == language->getNodeType())
				{
					UTF8String langAttr;
					DOMUtils::GetAttrValueIgnoreCase(language, TypeConv::AsUTF8("lang"), langAttr);
					if (0 == UTF8Utils::UTF8StrCmpI(languageStringUtf8.CString(), langAttr.CString()))
					{
						DOMUtils::NCDNodeVector altTextNodeVector;
						DOMUtils::GetAllMatchNode(language, TypeConv::AsUTF8("altText"), true, altTextNodeVector);
						DOMUtils::NCDNodeVector::const_iterator altTextIte;
						for (altTextIte=altTextNodeVector.begin(); altTextIte!=altTextNodeVector.end(); altTextIte++)
						{
							NCDNode* altText = *altTextIte;
							if (NCDNode::ELEMENT_NODE == altText->getNodeType())
							{
								UTF8String textIdAttr;
								DOMUtils::GetAttrValueIgnoreCase(altText, TypeConv::AsUTF8("textId"), textIdAttr);
								if (0 == UTF8Utils::UTF8StrCmpI(textIdAttr.CString(), nodeId.CString()))
								{
									altTextNode = altText;
									break;
								}
							}
						}
						if (NULL != altTextNode)
						{
							break;
						}
					}
				}
			}
		}
	}
	readTextNodeSub((NULL == altTextNode) ? theNode : altTextNode, false, outText);
}

// ---------------------------------------------------------------------
//! Converts an hexadecimal digit to SInt32
/*!
	@return UInt8 value
*/
// ---------------------------------------------------------------------
static UInt8 getHexValue(
	UTF8Char character		//!< hexadecimal digit
)
{
	if ('0' <= character && character <= '9')
	{
		return character - '0';
	}
	else if ('a' <= character && character <= 'f')
	{
		return character - 'a' + 10;
	}
	else if ('A' <= character && character <= 'F')
	{
		return character - 'A' + 10;
	}
	
	return 0;
}

// ---------------------------------------------------------------------
//! Parses the string which representing color value.
/*!
	@return color
	@throw CoverDefParseExceptions::ColorStringNotUnderstood when failed.
*/
// ---------------------------------------------------------------------
ColorValue CoverDefParser::parseColorString(
	ConstUTF8Str colorString		//!< color string
)
{
	// #rrggbb is accepted. check it
	if (7 > strlen(TypeConv::AsASCII(colorString)))
	{
		throw new CoverDefParseExceptions::ColorStringNotUnderstood(colorString);
	}

	if ('#' != colorString[0])
	{
		throw new CoverDefParseExceptions::ColorStringNotUnderstood(colorString);
	}
	
	int index;
	for (index=1; index<=6; index++)
	{
		if (!isxdigit(colorString[index]))
		{
			throw new CoverDefParseExceptions::ColorStringNotUnderstood(colorString);
		}
	}
	
	// convert to ColorValue
	ColorValue color;
	color.redValue = (getHexValue(colorString[1]) << 4) | getHexValue(colorString[2]);
	color.greenValue = (getHexValue(colorString[3]) << 4) | getHexValue(colorString[4]);
	color.blueValue = (getHexValue(colorString[5]) << 4) | getHexValue(colorString[6]);
	return color;
}

/**
 *	@brief	Retrieves a attribute value from specified node.
 *	@return	true if success.
 *	@throw	XMLParseExceptions::MissingAttribute	when required attribute is not found
 */
bool CoverDefParser::getAttrValue(
	const NCDNode* theNode,		///< element node
	ConstUTF8Str attrName,		///< attribute name
	UTF8String& outValue,		///< attribute value is returned
	bool isRequired				///< whether this attribute is required
)
{
	DOMUtils::GetAttrValueIgnoreCase(theNode, attrName, outValue);
	if (outValue.IsEmpty())
	{
		if (isRequired)
		{
			UTF8String tagName;
			theNode->getNodeName(tagName);
			throw new XMLParseExceptions::MissingAttribute(theNode->getSourceLine(), theNode->getSourceColumn(), tagName, attrName);
		}
		else
		{
			return false;
		}
	}
	return true;
}

/**
 *	@brief	Retrieves a attribute value as a Path object from specified node.
 *	@return	true if success.
 *	@throw	XMLParseExceptions::MissingAttribute	when required attribute is not found
 */
bool CoverDefParser::getPathValue(
	const NCDNode* theNode,		///< element node
	ConstUTF8Str attrName,		///< attribute name
	Path& outPath,				///< attribute value (path) is returned
	bool isRequired				///< whether this attribute is required
)
{
	UTF8String pathValue;
	if (!getAttrValue(theNode, attrName, pathValue, isRequired))
	{
		return false;
	}

	MBCString pathMBCStr;
	UTF8Conv::ToMultiByte(pathMBCStr, pathValue);
	outPath.AssignFromSlashSeparated(pathMBCStr);
	return true;
}

/**
 *	@brief	Retrives boolean value from string.
 *	@param[in]	value	the value.
 *	@param[in]	defaultValue	if value parameter is not understood, then return this value.
 */
static bool getBoolValue(ConstUTF8Str value, bool defaultValue)
{
	if (0 == UTF8Utils::UTF8StrCmpI(value, TypeConv::AsUTF8("true")))
	{
		return true;
	}
	else if (0 == UTF8Utils::UTF8StrCmpI(value, TypeConv::AsUTF8("false")))
	{
		return false;
	}
	else
	{
		return defaultValue;
	}
}

static const UTF8Char SUPPORTED_VERSION_1_0[] = "1.0";
static const UTF8Char SUPPORTED_VERSION_1_1[] = "1.1";
static const UTF8Char SUPPORTED_VERSION_1_2[] = "1.2";
static const UTF8Char SUPPORTED_VERSION_1_3[] = "1.3";
static const UTF8Char SUPPORTED_VERSION_1_4[] = "1.4";
static const UTF8Char SUPPORTED_VERSION_1_5[] = "1.5";

/**
 *	@brief	Parses element node "coverDef" and its descendant node.
 *	@return	CoverDef object which has informations in this "coverDef" node.
 */
CoverDef* CoverDefParser::parseTagCoverDef(
	const NCDElement* coverDefElement		///< "coverDef" element
)
{
	// check "version" attribute
	ConstUTF8Str supportedVersions[] = { SUPPORTED_VERSION_1_0, SUPPORTED_VERSION_1_1, SUPPORTED_VERSION_1_2, SUPPORTED_VERSION_1_3,
											SUPPORTED_VERSION_1_4, SUPPORTED_VERSION_1_5 };

	UTF8String versionString;
	getAttrValue(coverDefElement, TypeConv::AsUTF8("version"), versionString, true);
	bool supported = false;
	SInt32 vIdx;
	for (vIdx=0; vIdx<static_cast<SInt32>(sizeof(supportedVersions)/sizeof(supportedVersions[0])); vIdx++)
	{
		if (0 == UTF8Utils::UTF8StrCmp(versionString, supportedVersions[vIdx]))
		{
			supported = true;
			break;
		}
	}
	if (!supported)
	{
		throw new CoverDefParseExceptions::UnsupportedCoverDefVersion(versionString);
	}

	CoverDef* newCoverDef = new CoverDef();
	try
	{
		NCDNode* childNode;

		// "about" element
		childNode = DOMUtils::GetFirstMatchNode(coverDefElement, TypeConv::AsUTF8("about"), true);
		if (NULL != childNode)
		{
			UTF8String aboutString;
			readTextNode(childNode, aboutString);
			newCoverDef->SetAbout(aboutString);
		}

		// "title" element
		childNode = DOMUtils::GetFirstMatchNode(coverDefElement, TypeConv::AsUTF8("title"), true);
		if (NULL != childNode)
		{
			UTF8String titleString;
			readTextNode(childNode, titleString);
			newCoverDef->SetTitle(titleString);
		}
		
		// "description" element
		childNode = DOMUtils::GetFirstMatchNode(coverDefElement, TypeConv::AsUTF8("description"), true);
		if (NULL != childNode)
		{
			UTF8String descriptionString;
			readTextNode(childNode, descriptionString);
			newCoverDef->SetDescription(descriptionString);
		}
		
		// "cover" element
		DOMUtils::NCDNodeVector childNodeVector;
		DOMUtils::GetAllMatchNode(coverDefElement, TypeConv::AsUTF8("cover"), true, childNodeVector);
		DOMUtils::NCDNodeVector::const_iterator ite;
		for (ite=childNodeVector.begin(); ite!=childNodeVector.end(); ite++)
		{
			NCDNode* childNode = *ite;
			if (NCDNode::ELEMENT_NODE == childNode->getNodeType())
			{
				CoverInfo* coverInfo = parseTagCover(static_cast<NCDElement*>(childNode));
				newCoverDef->AddCoverInfo(coverInfo);
			}
		}
		
		newCoverDef->Validate();
	}
	catch (...)
	{
		delete newCoverDef;
		throw;
	}
	
	return newCoverDef;
}

/**
 *	@brief	Parses element node "cover" and its descendant node.
 *	@return	CoverInfo object which has informations in this "cover" node.
 */
CoverInfo* CoverDefParser::parseTagCover(
	const NCDNode* coverElement		///< "cover" element
)
{
	CoverInfo* newCoverInfo = new CoverInfo();
	try
	{
		DOMUtils::NCDNodeVector childNodeVector;
		DOMUtils::GetAllMatchNode(coverElement, TypeConv::AsUTF8("window"), true, childNodeVector);
		DOMUtils::NCDNodeVector::const_iterator ite;
		for (ite=childNodeVector.begin(); ite!=childNodeVector.end(); ite++)
		{
			NCDNode* childNode = *ite;
			if (NCDNode::ELEMENT_NODE != childNode->getNodeType())
			{
				continue;
			}
			
			UTF8String className;
			getAttrValue(childNode, TypeConv::AsUTF8("class"), className, true);
			if (0 == UTF8Utils::UTF8StrCmpI(className, TypeConv::AsUTF8("Main")))
			{
				newCoverInfo->SetMainWindowInfo(parseTagWindowMain(childNode));
			}
			else
			{
				throw new CoverDefParseExceptions::UnknownClass(className);
			}
		}
		
		newCoverInfo->Validate();
	}
	catch (...)
	{
		delete newCoverInfo;
		throw;
	}
	return newCoverInfo;
}

/**
 *	@brief	Parses element node "window" and its descendant node.
 *
 *			This functions retrieves informations common to all classes,
 *			which is the values of an atribute "class".
 */
void CoverDefParser::parseTagWindow(
	CoverWindowInfo* newWindowInfo,	///< common information of "window" is stored in this object.
	const NCDNode* windowElement	///< "window" element
)
{
	NCDNode* childNode;
	DOMUtils::NCDNodeVector childNodeVector;
	DOMUtils::NCDNodeVector::const_iterator ite;

	// "basePoint" element
	childNode = DOMUtils::GetFirstMatchNode(windowElement, TypeConv::AsUTF8("basePoint"), true);
	if (NULL != childNode && NCDNode::ELEMENT_NODE == childNode->getNodeType())
	{
		Point32 point = {0, 0};
		UTF8String value;
		if (getAttrValue(childNode, TypeConv::AsUTF8("x"), value, false))
		{
			point.x = atoi(TypeConv::AsASCII(value));
		}
		if (getAttrValue(childNode, TypeConv::AsUTF8("y"), value, false))
		{
			point.y = atoi(TypeConv::AsASCII(value));
		}
		newWindowInfo->SetBasePoint(point);
	}
	
	// "hotting" element
	childNode = DOMUtils::GetFirstMatchNode(windowElement, TypeConv::AsUTF8("hotting"), true);
	if (NULL != childNode && NCDNode::ELEMENT_NODE == childNode->getNodeType())
	{
		UTF8String value;
		if (getAttrValue(childNode, TypeConv::AsUTF8("steps"), value, false))
		{
			SInt32 steps = atoi(TypeConv::AsASCII(value));
			if (steps < 1)
			{
				steps = 1;
			}
			newWindowInfo->SetHottingSteps(steps);
		}
	}

	// "mapItem" element
	DOMUtils::GetAllMatchNode(windowElement, TypeConv::AsUTF8("map/mapItem"), true, childNodeVector);
	for (ite=childNodeVector.begin(); ite!=childNodeVector.end(); ite++)
	{
		childNode = *ite;
		if (NCDNode::ELEMENT_NODE == childNode->getNodeType())
		{
			UTF8String idValue;
			UTF8String colorValue;
			getAttrValue(childNode, TypeConv::AsUTF8("id"), idValue, true);
			getAttrValue(childNode, TypeConv::AsUTF8("color"), colorValue, true);
			
			newWindowInfo->AddMapItem(parseColorString(colorValue), idValue);
		}
	}
	
	// "tooltip" element
	DOMUtils::GetAllMatchNode(windowElement, TypeConv::AsUTF8("tooltip"), true, childNodeVector);
	for (ite=childNodeVector.begin(); ite!=childNodeVector.end(); ite++)
	{
		childNode = *ite;
		if (NCDNode::ELEMENT_NODE == childNode->getNodeType())
		{
			UTF8String mapIdValue;
			UTF8String text;
			getAttrValue(childNode, TypeConv::AsUTF8("mapId"), mapIdValue, true);
			readTextNode(childNode, text);
			CoverToolTipInfo* newToolTipInfo = new CoverToolTipInfo();
			try
			{
				newToolTipInfo->SetMapID(mapIdValue);
				newToolTipInfo->SetToolTipText(text);
			}
			catch (...)
			{
				delete newToolTipInfo;
				throw;
			}
			newWindowInfo->AddToolTipInfo(newToolTipInfo);
		}
	}	

	// "transparentRegion" element
	childNode = DOMUtils::GetFirstMatchNode(windowElement, TypeConv::AsUTF8("transparentRegion"), true);
	if (NULL != childNode && NCDNode::ELEMENT_NODE == childNode->getNodeType())
	{
		UTF8String strValue;
		UTF8String mapIdValue;
		bool usedAgainstAlpha = true;
		SInt32 edgeSmoothing = -1;
		getAttrValue(childNode, TypeConv::AsUTF8("mapId"), mapIdValue, true);
		if (getAttrValue(childNode, TypeConv::AsUTF8("usedAgainstAlpha"), strValue, false))
		{
			usedAgainstAlpha = getBoolValue(strValue, true);
		}
		if (getAttrValue(childNode, TypeConv::AsUTF8("edgeSmoothing"), strValue, false))
		{
			if (0 == UTF8Utils::UTF8StrCmpI(strValue, TypeConv::AsUTF8("auto")))
			{
				edgeSmoothing = -1;
			}
			else if (0 == UTF8Utils::UTF8StrCmpI(strValue, TypeConv::AsUTF8("no")))
			{
				edgeSmoothing = 0;
			}
			else
			{
				edgeSmoothing = atoi(TypeConv::AsASCII(strValue));
				if (0 == edgeSmoothing)
				{
					edgeSmoothing = -1;		// regard as "auto"
				}
			}
		}

		newWindowInfo->SetTransparentRegionID(mapIdValue);
		newWindowInfo->SetTransparentRegionUsedAgainstAlpha(usedAgainstAlpha);
		newWindowInfo->SetEdgeSmoothingLevel(edgeSmoothing);
	}	
}

/**
 *	@brief	Converts from main window class name to CoverMainWindowInfo::ButtonClass value.
 *	@return	CoverMainWindowInfo::ButtonClass value
 */
static CoverMainWindowInfo::ButtonClass getMainWindowButtonClass(
	ConstUTF8Str className			///< class name string
)
{
#define HANDLE_CLASS_NAME(name, buttonClass)								\
	if (0 == UTF8Utils::UTF8StrCmpI(className, TypeConv::AsUTF8(name)))		\
	{																		\
		return buttonClass;													\
	}

	HANDLE_CLASS_NAME("0",				CoverMainWindowInfo::ButtonClass_0)
	HANDLE_CLASS_NAME("1",				CoverMainWindowInfo::ButtonClass_1)
	HANDLE_CLASS_NAME("2",				CoverMainWindowInfo::ButtonClass_2)
	HANDLE_CLASS_NAME("3",				CoverMainWindowInfo::ButtonClass_3)
	HANDLE_CLASS_NAME("4",				CoverMainWindowInfo::ButtonClass_4)
	HANDLE_CLASS_NAME("5",				CoverMainWindowInfo::ButtonClass_5)
	HANDLE_CLASS_NAME("6",				CoverMainWindowInfo::ButtonClass_6)
	HANDLE_CLASS_NAME("7",				CoverMainWindowInfo::ButtonClass_7)
	HANDLE_CLASS_NAME("8",				CoverMainWindowInfo::ButtonClass_8)
	HANDLE_CLASS_NAME("9",				CoverMainWindowInfo::ButtonClass_9)
	HANDLE_CLASS_NAME("A",				CoverMainWindowInfo::ButtonClass_A)
	HANDLE_CLASS_NAME("B",				CoverMainWindowInfo::ButtonClass_B)
	HANDLE_CLASS_NAME("C",				CoverMainWindowInfo::ButtonClass_C)
	HANDLE_CLASS_NAME("D",				CoverMainWindowInfo::ButtonClass_D)
	HANDLE_CLASS_NAME("E",				CoverMainWindowInfo::ButtonClass_E)
	HANDLE_CLASS_NAME("F",				CoverMainWindowInfo::ButtonClass_F)
	HANDLE_CLASS_NAME("Point",			CoverMainWindowInfo::ButtonClass_Point)
	HANDLE_CLASS_NAME("Clear",			CoverMainWindowInfo::ButtonClass_Clear)
	HANDLE_CLASS_NAME("BackSpace",		CoverMainWindowInfo::ButtonClass_BS)
	HANDLE_CLASS_NAME("Equal",			CoverMainWindowInfo::ButtonClass_Equal)
	HANDLE_CLASS_NAME("Plus",			CoverMainWindowInfo::ButtonClass_Plus)
	HANDLE_CLASS_NAME("Minus",			CoverMainWindowInfo::ButtonClass_Minus)
	HANDLE_CLASS_NAME("Times",			CoverMainWindowInfo::ButtonClass_Times)
	HANDLE_CLASS_NAME("Div",			CoverMainWindowInfo::ButtonClass_Div)
	HANDLE_CLASS_NAME("Negate",			CoverMainWindowInfo::ButtonClass_Negate)
	HANDLE_CLASS_NAME("Hex",			CoverMainWindowInfo::ButtonClass_Hex)
	HANDLE_CLASS_NAME("Dec",			CoverMainWindowInfo::ButtonClass_Dec)
	HANDLE_CLASS_NAME("Oct",			CoverMainWindowInfo::ButtonClass_Oct)
	HANDLE_CLASS_NAME("Bin",			CoverMainWindowInfo::ButtonClass_Bin)
	HANDLE_CLASS_NAME("Minimize",		CoverMainWindowInfo::ButtonClass_Minimize)
	HANDLE_CLASS_NAME("Close",			CoverMainWindowInfo::ButtonClass_Close)

#undef HANDLE_CLASS_NAME
	
	throw new CoverDefParseExceptions::UnknownClass(className);
}

/**
 *	@brief	Parses element node "window" with its attribute "class" is "Main",
 *			and parses its descendant node.
 *	@return	CoverMainWindowInfo object which has informations in this "window" node.
 */
CoverMainWindowInfo* CoverDefParser::parseTagWindowMain(
	const NCDNode* windowElement	///< "window" element (class="Main")
)
{
	CoverMainWindowInfo* newWindowInfo = new CoverMainWindowInfo();
	try
	{
		// retrieves common information of "window" tag.
		parseTagWindow(newWindowInfo, windowElement);

		NCDNode* childNode;
		DOMUtils::NCDNodeVector childNodeVector;
		DOMUtils::NCDNodeVector::const_iterator ite;
		
		// "map" element
		childNode = DOMUtils::GetFirstMatchNode(windowElement, TypeConv::AsUTF8("map"), true);
		if (NULL != childNode && NCDNode::ELEMENT_NODE == childNode->getNodeType())
		{
			Path pathSrc;
			getPathValue(childNode, TypeConv::AsUTF8("src"), pathSrc, true);

			newWindowInfo->SetImageFilePath(CoverMainWindowInfo::ImageClass_ColorMap, pathSrc);
		}
		
		// "coverImage" element
		DOMUtils::GetAllMatchNode(windowElement, TypeConv::AsUTF8("coverImage"), true, childNodeVector);
		for (ite=childNodeVector.begin(); ite!=childNodeVector.end(); ite++)
		{
			childNode = *ite;
			if (NCDNode::ELEMENT_NODE == childNode->getNodeType())
			{
				Path pathSrc;
				getPathValue(childNode, TypeConv::AsUTF8("src"), pathSrc, true);

				CoverMainWindowInfo::ImageClass imageClass;
				UTF8String classValue;
				getAttrValue(childNode, TypeConv::AsUTF8("class"), classValue, true);
				if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("BackgroundActive")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_BackgroundActive;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("BackgroundInactive")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_BackgroundInactive;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("Pressed")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_Pressed;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("GrayedActive")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_GrayedActive;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("GrayedInactive")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_GrayedInactive;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("CheckedActive")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_CheckedActive;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("CheckedInactive")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_CheckedInactive;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("CheckedPressed")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_CheckedPressed;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("HotActive")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_HotActive;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("HotInactive")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_HotInactive;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("CheckedHotActive")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_CheckedHotActive;
				}
				else if (0 == UTF8Utils::UTF8StrCmpI(classValue, TypeConv::AsUTF8("CheckedHotInactive")))
				{
					imageClass = CoverMainWindowInfo::ImageClass_CheckedHotInactive;
				}
				else
				{
					throw new CoverDefParseExceptions::UnknownClass(classValue);
				}

				newWindowInfo->SetImageFilePath(imageClass, pathSrc);
			}
		}

		// "font" element
		childNode = DOMUtils::GetFirstMatchNode(windowElement, TypeConv::AsUTF8("font"), true);
		if (NULL != childNode && NCDNode::ELEMENT_NODE == childNode->getNodeType())
		{
			Path pathSrc;
			getPathValue(childNode, TypeConv::AsUTF8("src"), pathSrc, true);
			newWindowInfo->SetImageFilePath(CoverMainWindowInfo::ImageClass_Font, pathSrc);

			UTF8String typeValue, widthValue, heightValue, charSpaceValue, transparentColorValue;
			getAttrValue(childNode, TypeConv::AsUTF8("type"), typeValue, true);
			getAttrValue(childNode, TypeConv::AsUTF8("width"), widthValue, true);
			getAttrValue(childNode, TypeConv::AsUTF8("height"), heightValue, true);
			getAttrValue(childNode, TypeConv::AsUTF8("charSpace"), charSpaceValue, true);
			getAttrValue(childNode, TypeConv::AsUTF8("transparentColor"), transparentColorValue, true);
			
			CoverFontInfo::Type fontType;
			if (0 == UTF8Utils::UTF8StrCmpI(typeValue, TypeConv::AsUTF8("number")))
			{
				fontType = CoverFontInfo::Type_Number;
			}
			else
			{
				throw new CoverDefParseExceptions::UnknownType(typeValue);
			}

			CoverFontInfo* newFontInfo = new CoverFontInfo();
			try
			{
				newFontInfo->SetType(fontType);
				newFontInfo->SetWidth(atoi(TypeConv::AsASCII(widthValue)));
				newFontInfo->SetHeight(atoi(TypeConv::AsASCII(heightValue)));
				newFontInfo->SetCharSpace(atoi(TypeConv::AsASCII(charSpaceValue)));
				newFontInfo->SetTransparentColor(parseColorString(transparentColorValue));				
			}
			catch (...)
			{
				delete newFontInfo;
				throw;
			}
			newWindowInfo->SetFontInfo(newFontInfo);
		}
		
		// "button" element
		DOMUtils::GetAllMatchNode(windowElement, TypeConv::AsUTF8("button"), true, childNodeVector);
		for (ite=childNodeVector.begin(); ite!=childNodeVector.end(); ite++)
		{
			childNode = *ite;
			if (NCDNode::ELEMENT_NODE == childNode->getNodeType())
			{
				UTF8String classValue, mapIdValue;
				getAttrValue(childNode, TypeConv::AsUTF8("class"), classValue, true);
				getAttrValue(childNode, TypeConv::AsUTF8("mapId"), mapIdValue, true);
				
				CoverMainWindowInfo::ButtonClass buttonClass = getMainWindowButtonClass(classValue);
				
				CoverButtonInfo* newButtonInfo = new CoverButtonInfo();
				try
				{
					newButtonInfo->SetMapID(mapIdValue);
				}
				catch (...)
				{
					delete newButtonInfo;
					throw;
				}
				newWindowInfo->SetButtonInfo(buttonClass, newButtonInfo);
			}
		}
		
		// "display" element
		childNode = DOMUtils::GetFirstMatchNode(windowElement, TypeConv::AsUTF8("display"), true);
		if (NULL != childNode && NCDNode::ELEMENT_NODE == childNode->getNodeType())
		{
			UTF8String mapIdValue;
			getAttrValue(childNode, TypeConv::AsUTF8("mapId"), mapIdValue, true);
			CoverDisplayInfo* newDisplayInfo = new CoverDisplayInfo();
			try
			{
				newDisplayInfo->SetMapID(mapIdValue);
			}
			catch (...)
			{
				delete newDisplayInfo;
				throw;
			}
			newWindowInfo->SetDisplayInfo(newDisplayInfo);
		}		
		
		newWindowInfo->Validate();
		newWindowInfo->ResolveMapId();
	}
	catch (...)
	{
		delete newWindowInfo;
		throw;
	}
	return newWindowInfo;
}

/**
 *	@brief	Retrieves parsing result and detaches it from this object.
 *	@return	pointer to CoverDef object, which is the result of parsing
 */
CoverDef* CoverDefParser::DetachCoverDef()
{
	CoverDef* ret = coverDef;
	coverDef = NULL;
	return ret;	
}
