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
		throw new XMLLangFileExceptions::ValidationFailed(rootElement->getSourceLine(), rootElement->getSourceColumn(), "Not a locale definition file.");
	}

	// check version.
	UTF8String versionString;
	rootElement->getAttribute(TypeConv::AsUTF8("version"), versionString);
	if (!checkVersion(versionString))
	{
		throw new XMLLangFileExceptions::ValidationFailed(rootElement->getSourceLine(), rootElement->getSourceColumn(), "Version mismatch.");
	}

	// check platform target.
	bool platformOK = false;
	NCDElement* platformElement = DOMUtils::SearchElementNext(rootElement->getFirstChild(), TypeConv::AsUTF8("platform"), true);
	while (NULL != platformElement)
	{
		UTF8String targetString;
		platformElement->getAttribute(TypeConv::AsUTF8("target"), targetString);
		if (isTargetSuitable(targetString))
		{
			platformOK = true;
			break;
		}
	}
	if (!platformOK)
	{
		NCDNode* errorNode = (NULL != platformElement) ? platformElement : rootElement;
		throw new XMLLangFileExceptions::ValidationFailed(errorNode->getSourceLine(), errorNode->getSourceColumn(), "Target platform is not suitable.");
	}
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
		throw new XMLLangFileExceptions::ValidationFailed("Language code is not specified.");
	}
	if (langName.IsEmpty())
	{
		throw new XMLLangFileExceptions::ValidationFailed("Language name is not specified.");	
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
 */
void XMLLangFile::LoadString(ConstUTF8Str name, MBCString& message) const
{
	const NCDElement* stringElem = GetStringElement(name);
	if (NULL == stringElem)
	{
		throw new XMLLangFileExceptions::StringNotDefined(TypeConv::AsASCII(name));
	}
	UTF8String value;
	DOMUtils::ReadTextNode(stringElem, true, value);
	UTF8Conv::ToMultiByteWithLineEnding(message, value);
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
