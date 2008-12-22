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
	@file		XMLLangFile.cpp
	@brief		Implementation of XmlLangFile class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.12.20 created
*/

#include "Prefix.h"
#include "XMLLangFile.h"
#include "Path.h"
#include "File.h"
#include "XMLParser.h"
#include "DOMParseEventHandler.h"
#include "NCDDocument.h"
#include "NCDNode.h"
#include "NCDElement.h"
#include "UTF8Utils.h"
#include "UTF8Conv.h"
#include "XMLLangFileException.h"
#include "DOMUtils.h"
#include "CoveredCalcApp.h"
#include "DialogLayout.h"

#if defined (WIN32)
	UTF8Char	SUITABLE_VERSION[] = "7";
#elif defined (BEOS)
	UTF8Char	SUITABLE_VERSION[] = "7";
#endif

/**
 *	@brief	Constructor
 */
XMLLangFile::XMLLangFile()
{
	document = NULL;
	isReady = false;
}

/**
 *	@brief	Destructor
 */
XMLLangFile::~XMLLangFile()
{
	Unload();
}

/**
 *	@brief	Loads language file.
 *	@param[in]	language file path.
 */
void XMLLangFile::Load(const Path& langFilePath)
{
	Unload();

	// read file.
	File file;
	file.Open(langFilePath, FileConstants::OpenMode_ReadOnly, FileConstants::OpenOption_None);
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
	document = handler.DetachDocument();

	validateFirst();
	makeTables();
	validateSecond();
	isReady = true;
}

/**
 *	@brief	Unloads language file.
 */
void XMLLangFile::Unload()
{
	dialogElementMap.clear();
	stringElementMap.clear();
	if (NULL != document)
	{
		document->release();
		document = NULL;
	}
	isReady = false;
}

/**
 *	@brief	Validates language file (1st step).
 *	@throw	XMLLangFileExceptions::ValidationFailed		when the file is invalid.
 */
void XMLLangFile::validateFirst()
{
	// check root element.
	NCDElement* rootElement = document->getDocumentElement();
	UTF8String rootName;
	rootElement->getNodeName(rootName);
	if (0 != UTF8Utils::UTF8StrCmp(rootName, TypeConv::AsUTF8("localeDef")))
	{
		throw new XMLLangFileExceptions::ValidationFailed(rootElement->getSourceLine(), rootElement->getSourceColumn(), ALITERAL("Not a locale definition file."));
	}

	// check platform target.
	bool platformOK = false;
	NCDElement* platformElement = DOMUtils::SearchElementNext(rootElement->getFirstChild(), TypeConv::AsUTF8("platform"), false);
	while (NULL != platformElement)
	{
		UTF8String targetString;
		platformElement->getAttribute(TypeConv::AsUTF8("target"), targetString);
		if (CoveredCalcApp::GetInstance()->CheckPlatform(targetString))
		{
			platformOK = true;
			break;
		}
	}
	if (!platformOK)
	{
		NCDNode* errorNode = (NULL != platformElement) ? platformElement : rootElement;
		throw new XMLLangFileExceptions::ValidationFailed(errorNode->getSourceLine(), errorNode->getSourceColumn(), ALITERAL("Target platform is not suitable."));
	}

	// check version.
	UTF8String versionString;
	rootElement->getAttribute(TypeConv::AsUTF8("version"), versionString);
	if (!checkVersion(versionString))
	{
		throw new XMLLangFileExceptions::ValidationFailed(rootElement->getSourceLine(), rootElement->getSourceColumn(), ALITERAL("Version mismatch."));
	}
}

/**
 *	@brief		Checks the version is suitable for this implementation.
 *	@param[in]	version		version string.
 *	@return		true if the version is suitable.
 */
bool XMLLangFile::checkVersion(ConstUTF8Str version)
{
	return (0 == UTF8Utils::UTF8StrCmpI(SUITABLE_VERSION, version));
}

/**
 *	@brief	make name-to-element tables.
 */
void XMLLangFile::makeTables()
{
	NCDElement* rootElement = document->getDocumentElement();
	UTF8String nodeName;
	UTF8String name;
	NCDNode* node = rootElement->getFirstChild();
	while (NULL != node)
	{
		if (NCDNode::ELEMENT_NODE == node->getNodeType())
		{
			NCDElement* element = static_cast<NCDElement*>(node);
			element->getNodeName(nodeName);
			if (0 == UTF8Utils::UTF8StrCmp(nodeName, TypeConv::AsUTF8("string")))
			{
				element->getAttribute(TypeConv::AsUTF8("name"), name);
				stringElementMap[name] = element;
			}
			else if (0 == UTF8Utils::UTF8StrCmp(nodeName, TypeConv::AsUTF8("dialog")))
			{
				element->getAttribute(TypeConv::AsUTF8("name"), name);
				dialogElementMap[name] = element;
			}
			else if (0 == UTF8Utils::UTF8StrCmp(nodeName, TypeConv::AsUTF8("langName")))
			{
				UTF8String value;
				DOMUtils::ReadTextNode(element, true, value);
				UTF8Conv::ToMultiByte(langName, value);
			}
			else if (0 == UTF8Utils::UTF8StrCmp(nodeName, TypeConv::AsUTF8("langCode")))
			{
				UTF8String value;
				DOMUtils::ReadTextNode(element, true, value);
				UTF8Conv::ToMultiByte(langCode, value);
			}
		}
		node = node->getNextSibling();
	}
}

/**
 *	@brief	Validates language file (2nd step).
 *	@throw	XMLLangFileExceptions::ValidationFailed		when the file is invalid.
 */
void XMLLangFile::validateSecond()
{
	if (langCode.IsEmpty())
	{
		throw new XMLLangFileExceptions::ValidationFailed(ALITERAL("Language code is not specified."));
	}
	if (langName.IsEmpty())
	{
		throw new XMLLangFileExceptions::ValidationFailed(ALITERAL("Language name is not specified."));
	}
}

/**
 *	@brief	returns "dialog" element whose attribute "name" equals specified name.
 *	@param[in]	name	name.
 *	@return	"dialog" element. or NULL if not found.
 */
const NCDElement* XMLLangFile::GetDialogElement(ConstUTF8Str name) const
{
	if (!isReady)
	{
		return NULL;
	}
	
	NameToElementMap::const_iterator ite = dialogElementMap.find(name);
	if (ite != dialogElementMap.end())
	{
		return (*ite).second;
	}
	else
	{
		return NULL;
	}
}

/**
 *	@brief	returns "string" element whose attribute "name" equals specified name.
 *	@param[in]	name	name.
 *	@return	"string" element. or NULL if not found.
 */
const NCDElement* XMLLangFile::GetStringElement(ConstUTF8Str name) const
{
	if (!isReady)
	{
		return NULL;
	}
	
	NameToElementMap::const_iterator ite = stringElementMap.find(name);
	if (ite != stringElementMap.end())
	{
		return (*ite).second;
	}
	else
	{
		return NULL;
	}
}

/**
 *	@brief		Loads string.
 *	@param[in]	name	string name.
 *	@param[out]	message		loaded text is returned.
 *	@return		true when success. or false if failed.
 */
bool XMLLangFile::LoadString(ConstUTF8Str name, MBCString& message) const
{
	const NCDElement* stringElem = GetStringElement(name);
	if (NULL == stringElem)
	{
		return false;
	}
	UTF8String value;
	DOMUtils::ReadTextNode(stringElem, true, value);
	UTF8Conv::ToMultiByteWithLineEnding(message, value);
	return true;
}

/**
 *	@brief	Append dialog layout information to specified DialogLayout object.
 *	@param[in]	name	dialog name.
 *	@param[out]	layout	layout information is appended to this object.
 */
void XMLLangFile::LoadDialogLayout(ConstUTF8Str name, DialogLayout& layout) const
{
	const NCDElement* dialogElem = GetDialogElement(name);
	if (NULL != dialogElem)
	{
		ConstUTF8Str NAME_POSITION = TypeConv::AsUTF8("position");
		NCDElement* positionElem = DOMUtils::SearchElementNext(dialogElem->getFirstChild(), NAME_POSITION, false);
		while (NULL != positionElem)
		{
			UTF8String attrValue;
			MBCString name, offsetFrom, value;
			positionElem->getAttribute(TypeConv::AsUTF8("name"), attrValue);
			UTF8Conv::ToMultiByte(name, attrValue.CString());
			if (!name.IsEmpty())
			{
				positionElem->getAttribute(TypeConv::AsUTF8("offset-from"), attrValue);
				UTF8Conv::ToMultiByte(offsetFrom, attrValue.CString());
				positionElem->getAttribute(TypeConv::AsUTF8("value"), attrValue);
				UTF8Conv::ToMultiByte(value, attrValue.CString());

				DLValue dlValue = ttoi(value);
				layout.SetLayoutItem(name, offsetFrom, dlValue);
			}

			positionElem = DOMUtils::SearchElementNext(positionElem->getNextSibling(), NAME_POSITION, false);
		}
	}
}

/**
 *	@brief		Returns language name.
 *	@param[out]	langName	language name is returned.
 */
void XMLLangFile::GetLanguageName(MBCString& langName) const
{
	if (!isReady)
	{
		langName.Empty();
		return;
	}
	langName = this->langName;
}

/**
 *	@brief		Returns language code.
 *	@param[out]	langCode	language code is returned.
 */
void XMLLangFile::GetLanguageCode(MBCString& langCode) const
{
	if (!isReady)
	{
		langCode.Empty();
		return;
	}
	langCode = this->langCode;
}

#if defined (WIN32)
/**
 *	@brief Converts access mnemonic escape character (from '~' to '&')
 *	@param[in]	srcString	soruce string.
 *	@return	converted string.
 */
MBCString XMLLangFile::ConvertAccessMnemonic(const MBCString& srcString)
{
	MBCString result = srcString;
	SInt32 length = result.Length();
	SInt32 index;
	for (index = 0; index < length; ++index)
	{
		if (ALITERAL('~') == result.GetAt(index))
		{
			result.SetAt(index, ALITERAL('&'));
		}
	}
	return result;
}

/**
 *	@brief	Loads dialog font informations.
 *	@param[out]	outDialogFonts	dialog font informations are returned (in document order).
 */
void XMLLangFile::LoadDialogFont(DialogFontVector& outDialogFonts) const
{
	outDialogFonts.clear();

	NCDElement* rootElement = document->getDocumentElement();
	ConstUTF8Str NAME_DIALOGFONT = TypeConv::AsUTF8("dialogFont");
	NCDElement* dialogFontElem = DOMUtils::SearchElementNext(rootElement->getFirstChild(), NAME_DIALOGFONT, false);
	while (NULL != dialogFontElem)
	{
		DialogFont df;
		UTF8String value;

		dialogFontElem->getAttribute(TypeConv::AsUTF8("targetOSVer"), value);
		UTF8Conv::ToMultiByte(df.TargetOSVersion, value.CString());

		dialogFontElem->getAttribute(TypeConv::AsUTF8("charset"), value);
		df.Charset = static_cast<BYTE>(atoi(TypeConv::AsASCII(value.CString())));

		dialogFontElem->getAttribute(TypeConv::AsUTF8("typeface"), value);
		df.Typeface = value;

		dialogFontElem->getAttribute(TypeConv::AsUTF8("size"), value);
		df.PointSize = static_cast<WORD>(atoi(TypeConv::AsASCII(value.CString())));

		outDialogFonts.push_back(df);

		dialogFontElem = DOMUtils::SearchElementNext(dialogFontElem->getNextSibling(), NAME_DIALOGFONT, false);
	}
}
#endif
