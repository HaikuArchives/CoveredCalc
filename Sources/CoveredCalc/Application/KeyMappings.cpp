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
	@file		KeyMappings.h
	@brief		Implementation of KeyMappings class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.10.11 created
*/

#include "Prefix.h"
#include "KeyMappings.h"
#include "File.h"
#include "DOMParseEventHandler.h"
#include "DOMUtils.h"
#include "KeyMappingsException.h"
#include "UTF8String.h"
#include "NCDDocument.h"
#include "NCDElement.h"
#include "NCDText.h"
#include "XMLParser.h"
#include "NCDDocumentFactory.h"
#include "UTF8Utils.h"

static const UTF8Char STR_NAMESPACE_URI[]			= "http://hironytic.1st-shrine.net/schema/CoveredCalc-KeyMappings";
static const UTF8Char STR_XMLNS[]					= "xmlns";

static const UTF8Char STR_ELEMENT_KEY_MAPPINGS[]	= "keyMappings";
static const UTF8Char STR_ELEMENT_PLATFORM[]		= "platform";
static const UTF8Char STR_ELEMENT_TITLE[]			= "title";
static const UTF8Char STR_ELEMENT_CATEGORY[]		= "category";
static const UTF8Char STR_ELEMENT_KEY[]				= "key";
static const UTF8Char STR_ATTR_VERSION[]			= "version";
static const UTF8Char STR_ATTR_TARGET[]				= "target";
static const UTF8Char STR_ATTR_NAME[]				= "name";
static const UTF8Char STR_ATTR_CODE[]				= "code";
static const UTF8Char STR_ATTR_MODIFIERS[]			= "modifiers";
static const UTF8Char STR_ATTR_FUNCTION[]			= "function";

const UTF8Char KeyMappings::STR_CATEGORY_MAIN_WINDOW[] = "MainWindow";

/**
 *	@brief	Constructor
 */
KeyMappings::KeyMappings()
{
	loadedDocument = NULL;
}

KeyMappings::~KeyMappings()
{
	if (NULL != loadedDocument)
	{
		loadedDocument->release();
	}
}

/**
 *	@brief	Creates an empty key-mappings.
 */
void KeyMappings::CreateNew()
{
	if (NULL != loadedDocument)
	{
		loadedDocument->release();
		loadedDocument = NULL;
	}

	loadedDocument = NCDDocumentFactory::CreateDocument();
	NCDElement* apexElem = loadedDocument->createElement(STR_ELEMENT_KEY_MAPPINGS);
	apexElem->setAttribute(STR_XMLNS, STR_NAMESPACE_URI);
	loadedDocument->appendChild(apexElem);
	
	setVersion(Version_Current);
}

/**
 *	@brief	Loads key mappings from specified XML file.
 *	@param[in]	xmlFilePath	XML file path.
 */
void KeyMappings::Load(const Path& xmlFilePath)
{
	if (NULL != loadedDocument)
	{
		loadedDocument->release();
		loadedDocument = NULL;
	}

	// read XML file.
	File file;
	file.Open(xmlFilePath, FileConstants::OpenMode_ReadOnly, FileConstants::OpenOption_None);
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
	NCDDocument* document = handler.DetachDocument();
	
	// check this document
	try
	{
		checkDocument(document);
	}
	catch (...)
	{
		delete document;
		throw;
	}
	loadedDocument = document;
}

/**
 *	@brief	Loads key mappings from specified XML buffer.
 *	@param[in]	buffer		XML buffer.
 *	@param[in]	bufferSize	size of buffer.
 */
void KeyMappings::Load(const Byte* buffer, SInt32 bufferSize)
{
	if (NULL != loadedDocument)
	{
		loadedDocument->release();
		loadedDocument = NULL;
	}

	// parse XML into DOM tree
	XMLParser parser;
	parser.Create();
	DOMParseEventHandler handler;
	parser.SetEventHandler(&handler);
	parser.Parse(reinterpret_cast<const char*>(buffer), bufferSize, true);
	NCDDocument* document = handler.DetachDocument();
	
	// check this document
	try
	{
		checkDocument(document);
	}
	catch (...)
	{
		delete document;
		throw;
	}
	loadedDocument = document;
}

/// XML format director.
class FormatDirector : public DOMUtils::NodeFormatDirector
{
	virtual DOMUtils::FormatMode GetFormatMode(const NCDNode* /* node */)
	{
		return DOMUtils::FormatMode_Block;
	}
	virtual UInt32 GetTextFormatOptions(const NCDNode* node)
	{
		UTF8String name;
		node->getNodeName(name);
		if (0 == UTF8Utils::UTF8StrCmp(name, STR_ELEMENT_TITLE))
		{
			return DOMUtils::TextFormatOption_Inline | DOMUtils::TextFormatOption_PreserveSpace;
		}
		else
		{
			return DOMUtils::TextFormatOption_None;
		}
	};
	virtual ConstUTF8Str GetIndentChars()
	{
		return TypeConv::AsUTF8("\t");
	};
};

/**
 *	@brief	Saves key mappings to specified XML file.
 *	@param[in]	xmlFilePath	XML file path.
 */
void KeyMappings::Save(const Path& xmlFilePath)
{
	if (NULL == loadedDocument)
	{
		throw new KeyMappingsExceptions::NotReady();
	}

	// format
	FormatDirector director;
	DOMUtils::FormatDocument(loadedDocument, &director);
	
	// save to the file.
	File file;
	file.Open(xmlFilePath, FileConstants::OpenMode_WriteOnly, FileConstants::OpenOption_Create | FileConstants::OpenOption_Erase);
	DOMUtils::WriteOutAsXML(loadedDocument, &file);
	file.Close();
}

/**
 *	@brief	Checks specified document is key-mapping document.
 *	@throw	KeyMappingException	when invalid.
 */
void KeyMappings::checkDocument(NCDDocument* document)
{
	NCDElement* apexElem = document->getDocumentElement();
	if (NULL == apexElem)
	{
		throw new KeyMappingsExceptions::LoadFailed(ALITERAL("missing apex element."));
	}
	
	checkApexElement(apexElem);
}

/**
 *	@brief	Checks specified apex element is valid for key-mappings.
 *	@throw	KeyMappingException	when invalid.
 */
void KeyMappings::checkApexElement(NCDElement* apexElement)
{
	UTF8String name;
	apexElement->getTagName(name);
	if (0 != name.Compare(STR_ELEMENT_KEY_MAPPINGS))
	{
		throw new KeyMappingsExceptions::LoadFailed(ALITERAL("not a key-mappings file."));
	}
	
	// check version
	UTF8String version;
	apexElement->getAttribute(STR_ATTR_VERSION, version);
	SInt32 numVersion = atoi(TypeConv::AsASCII(version));
	if (0 == numVersion || Version_Current < numVersion)
	{
		throw new KeyMappingsExceptions::LoadFailed(ALITERAL("unsupported version."));
	}
}

/**
 *	@brief	Returns apex element of key-mappings XML file.
 *	@return	an apex element.
 *	@throw KeyMappingsExceptions::NotReady	when this object is not ready.
 */
NCDElement* KeyMappings::getApexElement() const
{
	if (NULL == loadedDocument)
	{
		throw new KeyMappingsExceptions::NotReady();
	}
	
	return loadedDocument->getDocumentElement();
}

/**
 *	@brief	Returns version of this key mappings.
 *	@return version (a value in KeyMappings::Version enum)
 */
SInt32 KeyMappings::GetVersion() const
{
	NCDElement* apexElem = getApexElement();
	
	UTF8String versionStr;
	apexElem->getAttribute(STR_ATTR_VERSION, versionStr);
	return atoi(TypeConv::AsASCII(versionStr));
}

/**
 *	@brief	Sets version of this key mappings.
 *	@param[in]	version	version (a value in KeyMappings::Version enum)
 */
void KeyMappings::setVersion(SInt32 version)
{
	NCDElement* apexElem = getApexElement();

	UTF8Char versionStr[32];
	snprintf(TypeConv::AsASCII(versionStr), sizeof(versionStr)/sizeof(UTF8Char), "%d", static_cast<int>(version));
	apexElem->setAttribute(STR_ATTR_VERSION, versionStr);
}	

/**
 *	@brief	Returns target platform.
 *	@param[out]	platform	target platform string is returned.
 *	@return true if platform is set.
 */
bool KeyMappings::GetPlatform(UTF8String& platform) const
{
	NCDElement* apexElem = getApexElement();

	NCDNode* platformNode = DOMUtils::GetFirstMatchNode(apexElem, STR_ELEMENT_PLATFORM, false);
	if (NULL == platformNode || platformNode->getNodeType() != NCDNode::ELEMENT_NODE)
	{
		return false;
	}
	NCDElement* platformElem = static_cast<NCDElement*>(platformNode);
	platformElem->getAttribute(STR_ATTR_TARGET, platform);
	return !platform.IsEmpty();
}

/**
 *	@brief	Sets target platform
 *	@param[in]	platform	target platform string. or NULL when you want to clear it.
 */
void KeyMappings::SetPlatform(ConstUTF8Str platform)
{
	NCDElement* apexElem = getApexElement();

	NCDElement* platformElem = NULL;
	NCDNode* platformNode = DOMUtils::GetFirstMatchNode(apexElem, STR_ELEMENT_PLATFORM, false);
	if (NULL != platformNode && platformNode->getNodeType() == NCDNode::ELEMENT_NODE)
	{
		platformElem = static_cast<NCDElement*>(platformNode);
	}

	if (NULL == platform)
	{
		if (NULL != platformElem)
		{
			platformElem->getParentNode()->removeChild(platformElem);
			platformElem->release();
		}
	}
	else
	{
		if (NULL == platformElem)
		{
			platformElem = apexElem->getOwnerDocument()->createElement(STR_ELEMENT_PLATFORM);
			apexElem->insertBefore(platformElem, apexElem->getFirstChild());
		}
		platformElem->setAttribute(STR_ATTR_TARGET, platform);
	}
}

/**
 *	@brief	Returns title of this key-mappings.
 *	@param[out]	title	the title is returned.
 *	@return true if the title is set.
 */
bool KeyMappings::GetTitle(UTF8String& title) const
{
	NCDElement* apexElem = getApexElement();

	NCDNode* titleNode = DOMUtils::GetFirstMatchNode(apexElem, STR_ELEMENT_TITLE, false);
	if (NULL == titleNode || titleNode->getNodeType() != NCDNode::ELEMENT_NODE)
	{
		return false;
	}
	
	DOMUtils::ReadTextNode(titleNode, false, title);
	return true;
}

/**
 *	@brief	Sets a title of this key-mappings.
 *	@param[in]	title	a title of a key-mapping file.
 */
void KeyMappings::SetTitle(ConstUTF8Str title)
{
	NCDElement* apexElem = getApexElement();

	NCDElement* titleElem = NULL;
	NCDNode* titleNode = DOMUtils::GetFirstMatchNode(apexElem, STR_ELEMENT_TITLE, false);
	if (NULL != titleNode && titleNode->getNodeType() == NCDNode::ELEMENT_NODE)
	{
		titleElem = static_cast<NCDElement*>(titleNode);
	}

	if (NULL == title)
	{
		if (NULL != titleElem)
		{
			titleElem->getParentNode()->removeChild(titleElem);
			titleElem->release();
		}
	}
	else
	{
		if (NULL == titleElem)
		{
			titleElem = apexElem->getOwnerDocument()->createElement(STR_ELEMENT_TITLE);
			apexElem->insertBefore(titleElem, apexElem->getFirstChild());
		}
		NCDNode* childNode = titleElem->getFirstChild();
		while (NULL != childNode)
		{
			NCDNode* nextChild = childNode->getNextSibling();
			titleElem->removeChild(childNode);
			childNode->release();
			
			childNode = nextChild;
		}
		NCDNode* textNode = titleElem->getOwnerDocument()->createTextNode(title);
		titleElem->appendChild(textNode);
	}
}

/**
 *	@brief	Searches a category element which name matches specified in the parameter.
 *	@param[in]	category	category name to find.
 *	@return	category element, or NULL if it is not found.
 */
NCDElement* KeyMappings::findCategoryElement(ConstUTF8Str category) const
{
	UTF8String name;
	NCDElement* apexElem = getApexElement();
	NCDElement* categoryElem = DOMUtils::SearchElementNext(apexElem->getFirstChild(), STR_ELEMENT_CATEGORY, false);
	while (NULL != categoryElem)
	{
		static_cast<NCDElement*>(categoryElem)->getAttribute(STR_ATTR_NAME, name);
		if (0 == name.Compare(category))
		{
			return categoryElem;
		}
	}
	return NULL;
}

/**
 *	@brief	Appends one key-mapping item.
 *	@param[in]	category	category name.
 *	@param[in]	item		key-mapping item.
 */
void KeyMappings::AddItem(ConstUTF8Str category, KeyMappingItem* item)
{
	NCDElement* categoryElem = findCategoryElement(category);
	if (NULL == categoryElem)
	{
		// if not found, create a new category element.
		NCDElement* apexElem = getApexElement();
		categoryElem = apexElem->getOwnerDocument()->createElement(STR_ELEMENT_CATEGORY);
		categoryElem->setAttribute(STR_ATTR_NAME, category);
		apexElem->appendChild(categoryElem);
	}
	
	UTF8String value;
	NCDElement* keyElem = categoryElem->getOwnerDocument()->createElement(STR_ELEMENT_KEY);
	if (item->GetKeyCode(value))
	{
		keyElem->setAttribute(STR_ATTR_CODE, value);
	}
	if (item->GetModifiers(value))
	{
		keyElem->setAttribute(STR_ATTR_MODIFIERS, value);
	}
	if (item->GetFunction(value))
	{
		keyElem->setAttribute(STR_ATTR_FUNCTION, value);
	}
	categoryElem->appendChild(keyElem);
}

/**
 *	@brief	Removes one key-mapping item.
 *	@param[in]	category	category name.
 *	@param[in]	item		key-mapping item.
 */
void KeyMappings::RemoveItem(ConstUTF8Str category, KeyMappingItem* item)
{
	NCDElement* categoryElem = findCategoryElement(category);
	if (NULL == categoryElem)
	{
		return;
	}

	UTF8String keyCode, modifiers, function;
	if (!item->GetKeyCode(keyCode))
	{
		keyCode.Empty();
	}
	if (!item->GetModifiers(modifiers))
	{
		modifiers.Empty();
	}
	if (!item->GetFunction(function))
	{
		function.Empty();
	}
	
	UTF8String value;
	NCDElement* nextKeyElem = DOMUtils::SearchElementNext(categoryElem->getFirstChild(), STR_ELEMENT_KEY, false);
	while (NULL != nextKeyElem)
	{
		NCDElement* keyElem = nextKeyElem;
		nextKeyElem = DOMUtils::SearchElementNext(keyElem->getNextSibling(), STR_ELEMENT_KEY, false);
		
		keyElem->getAttribute(STR_ATTR_CODE, value);
		if (0 != keyCode.Compare(value))
		{
			continue;
		}
		keyElem->getAttribute(STR_ATTR_MODIFIERS, value);
		if (0 != modifiers.Compare(value))
		{
			continue;
		}
		keyElem->getAttribute(STR_ATTR_FUNCTION, value);
		if (0 != function.Compare(value))
		{
			continue;
		}
		
		// found
		categoryElem->removeChild(keyElem);
		keyElem->release();
		if (NULL == DOMUtils::SearchElementNext(categoryElem->getFirstChild(), STR_ELEMENT_KEY, false))
		{
			categoryElem->getParentNode()->removeChild(categoryElem);
			categoryElem->release();
		}
	}
}

/**
 *	@brief	Removes all key-mapping items in a specified category.
 *	@param[in]	category	category name
 */
void KeyMappings::RemoveAllItem(ConstUTF8Str category)
{
	NCDElement* categoryElem = findCategoryElement(category);
	if (NULL == categoryElem)
	{
		return;
	}

	categoryElem->getParentNode()->removeChild(categoryElem);
	categoryElem->release();
}

/**
 *	@brief	The KeyMappingItem interface to access to a key element in the DOM tree.
 */
class DomKeyMappingItem : public KeyMappingItem
{
public:
	DomKeyMappingItem()
	{
		this->keyElem = NULL;
	}

	void SetKeyElement(NCDElement* keyElem)
	{
		this->keyElem = keyElem;
	}

	NCDElement* GetKeyElement() const
	{
		return keyElem;
	}

	virtual bool GetKeyCode(UTF8String& keyCode) const
	{
		keyElem->getAttribute(STR_ATTR_CODE, keyCode);
		return !keyCode.IsEmpty();
	}

	virtual bool GetModifiers(UTF8String& modifiers) const
	{
		keyElem->getAttribute(STR_ATTR_MODIFIERS, modifiers);
		return !modifiers.IsEmpty();
	}

	virtual bool GetFunction(UTF8String& function) const
	{
		keyElem->getAttribute(STR_ATTR_FUNCTION, function);
		return !function.IsEmpty();
	}

private:
	NCDElement*	keyElem;
};

/**
 *	@brief	Enumerates key-mapping items represented as a key element in the specified category element.
 */
class DomKeyMappingItemEnumerator : public KeyMappings::KeyMappingItemEnumerator
{
public:
	DomKeyMappingItemEnumerator(NCDElement* categoryElem)
	{
		this->categoryElem = categoryElem;
	}

	virtual KeyMappingItem* GetCurrent()
	{
		if (NULL == currentKeyItem.GetKeyElement())
		{
			return NULL;
		}
		
		return &currentKeyItem;
	}
	
	virtual bool MoveNext()
	{
		if (NULL == categoryElem)
		{
			return false;
		}
	
		NCDElement* keyElem = currentKeyItem.GetKeyElement();
		if (NULL == keyElem)
		{
			keyElem = DOMUtils::SearchElementNext(categoryElem->getFirstChild(), STR_ELEMENT_KEY, false);
		}
		else
		{
			keyElem = DOMUtils::SearchElementNext(keyElem->getNextSibling(), STR_ELEMENT_KEY, false);
		}
		
		if (NULL == keyElem)
		{
			return false;
		}
		else
		{
			currentKeyItem.SetKeyElement(keyElem);
			return true;
		}
	}
	
	virtual bool Reset()
	{
		currentKeyItem.SetKeyElement(NULL);
		return true;
	}

private:
	NCDElement*	categoryElem;
	DomKeyMappingItem currentKeyItem;
};

/**
 *	@brief	Returnes new KeyMappingItemEnumerator object which enumerates in the specified category.
 *	@param[in]	category	category name.
 *	@return a pointer to a new KeyMappingItemEnumerator object. The caller is responsible to delete this pointer.
 */
KeyMappings::KeyMappingItemEnumerator* KeyMappings::NewItemEnumeratorFor(ConstUTF8Str category) const
{
	NCDElement* categoryElem = findCategoryElement(category);
	return new DomKeyMappingItemEnumerator(categoryElem);	// note that categoryElem can be NULL.
}
