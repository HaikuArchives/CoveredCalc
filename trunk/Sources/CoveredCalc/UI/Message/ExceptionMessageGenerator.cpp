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
	@file		ExceptionMessageGenerator.cpp
	@brief		Implementation of ExceptionMessageGenerator class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.01.09 created
*/

#include "Prefix.h"
#include "ExceptionMessageGenerator.h"
#include "MemoryException.h"
#include "XMLParseException.h"
#include "CoverDefParseException.h"
#include "FileException.h"
#include "DIBFileLoaderException.h"
#include "UIControllerException.h"
#include "ColorCodedSkinException.h"
#include "PathException.h"
#include "XMLLangFileException.h"
#include "DialogLayoutException.h"
#include "UTF8Conv.h"
#include "NativeStringLoader.h"
#include "MessageFormatter.h"
#include "StringID.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
ExceptionMessageGenerator::ExceptionMessageGenerator()
{
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
ExceptionMessageGenerator::~ExceptionMessageGenerator()
{
}

// ---------------------------------------------------------------------
//! Initializes the object.
// ---------------------------------------------------------------------
void ExceptionMessageGenerator::Init(
	NativeStringLoader* stringLoader		//!< string loader object.
)
{
	this->stringLoader = stringLoader;
}

// ---------------------------------------------------------------------
//! Generate a message string about the exception.
// ---------------------------------------------------------------------
void ExceptionMessageGenerator::GetMessageString(
	Exception* ex,								//!< exception object
	MBCString& message							//!< OUTPUT. a message is returned.
) const
{
	try
	{
		message.Empty();
		try
		{
			processException(ex, message);
		}
		catch (Exception* ex2)
		{
			ex2->Delete();
			message.Empty();	
		}

		if (message.IsEmpty())
		{
			try
			{
				message = stringLoader->LoadNativeString(IDS_EMSG_GENERIC);
			}
			catch (Exception* ex3)
			{
				ex3->Delete();
				message = ALITERAL("An error has occurred.");
			}

			ConstAStr errorMessage = ex->GetErrorMessage();
			if (NULL != errorMessage && ALITERAL('\0') != errorMessage[0])
			{
				message += ALITERAL("\n");
				message += errorMessage;
			}
		}
	}
	catch (Exception* ex4)
	{
		ex4->Delete();
	}
}

// ---------------------------------------------------------------------
//! Generate a message string about Exception.
// ---------------------------------------------------------------------
void ExceptionMessageGenerator::processException(
	Exception* ex,					//!< exception object
	MBCString& message				//!< OUTPUT. a message is returned. if failed to get message, it gets empty.
) const
{
	if (processMemoryException(ex, message))
		return;
	
	if (processXMLParseException(ex, message))
		return;
	
	if (processFileException(ex, message))
		return;
	
	if (processUIControllerException(ex, message))
		return;
	
	if (processColorCodedSkinException(ex, message))
		return;
	
	if (processPathException(ex, message))
		return;

	if (processDialogLayoutException(ex, message))
		return;
	
	message.Empty();
}

// ---------------------------------------------------------------------
//! Generate a message string about MemoryException.
/*!
	@retval	true	processed.
	@retval false	not processed.
*/
// ---------------------------------------------------------------------
bool ExceptionMessageGenerator::processMemoryException(
	Exception* ex,					//!< exception object
	MBCString& message				//!< OUTPUT. a message is returned.
) const
{
	MemoryException* myException = dynamic_cast<MemoryException*>(ex);
	if (NULL == myException)
	{
		return false;
	}

	try
	{
		message = stringLoader->LoadNativeString(IDS_EMSG_MEMORY);
	}
	catch (MemoryException* mex1)
	{
		try
		{
			message = myException->GetErrorMessage();
		}
		catch (MemoryException* mex2)
		{
			mex2->Delete();
		}
		mex1->Delete();
	}
	
	return true;	
}

// ---------------------------------------------------------------------
//! Generate a message string about XMLParseException.
/*!
	@retval	true	processed.
	@retval false	not processed.
*/
// ---------------------------------------------------------------------
bool ExceptionMessageGenerator::processXMLParseException(
	Exception* ex,					//!< exception object
	MBCString& message				//!< OUTPUT. a message is returned.
) const
{
	if (processCoverDefParseException(ex, message))
		return true;

	if (processXMLLangFileException(ex, message))
		return true;
	
	XMLParseException* myException = dynamic_cast<XMLParseException*>(ex);
	if (NULL == myException)
	{
		return false;
	}

	bool resolved = false;
	MBCString eachMessage;

	// XMLParseExceptions::NoMatchStartTag
	if (!resolved)
	{
		XMLParseExceptions::NoMatchStartTag* myExceptionNMST = dynamic_cast<XMLParseExceptions::NoMatchStartTag*>(ex);
		if (NULL != myExceptionNMST)
		{
			MBCString name;
			UTF8Conv::ToMultiByte(name, myExceptionNMST->GetName());
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_NO_MATCH_START_TAG), name.CString());
			resolved = true;
		}
	}
	
	// XMLParseExceptions::InvalidParentEntity
	if (!resolved)
	{
		XMLParseExceptions::InvalidParentEntity* myExceptionIPE = dynamic_cast<XMLParseExceptions::InvalidParentEntity*>(ex);
		if (NULL != myExceptionIPE)
		{
			MBCString name, parentName;
			UTF8Conv::ToMultiByte(name, myExceptionIPE->GetName());
			UTF8Conv::ToMultiByte(parentName, myExceptionIPE->GetParentName());
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_INVALID_PARENT_ENTITY), name.CString(), parentName.CString());
			resolved = true;
		}
	}
		
	// XMLParseExceptions::MissingAttribute
	if (!resolved)
	{
		XMLParseExceptions::MissingAttribute* myExceptionMA = dynamic_cast<XMLParseExceptions::MissingAttribute*>(ex);
		if (NULL != myExceptionMA)
		{
			MBCString tagName, attrName;
			UTF8Conv::ToMultiByte(tagName, myExceptionMA->GetTagName());
			UTF8Conv::ToMultiByte(attrName, myExceptionMA->GetAttributeName());
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_MISSING_ATTRIBUTE), tagName.CString(), attrName.CString());
			resolved = true;
		}
	}
	
	// XMLParseExceptions::UnknownTag
	if (!resolved)
	{
		XMLParseExceptions::UnknownTag* myExceptionUT = dynamic_cast<XMLParseExceptions::UnknownTag*>(ex);
		if (NULL != myExceptionUT)
		{
			MBCString name;
			UTF8Conv::ToMultiByte(name, myExceptionUT->GetName());
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_UNKNOWN_TAG), name.CString());
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		eachMessage.Empty();
	}
	
	MBCString preMessage = stringLoader->LoadNativeString(IDS_EMSG_XML_PARSE);
	message = preMessage + eachMessage;
	
	if (-1 != myException->GetLine())
	{
		MBCString lineColumn;
		MessageFormatter::Format(lineColumn, stringLoader->LoadNativeString(IDS_EMSG_XML_LINE_COLUMN), myException->GetLine(), myException->GetColumn());
		message += lineColumn;
	}
	return true;
}

// ---------------------------------------------------------------------
//! Generate a message string about CoverDefParseException.
/*!
	@retval	true	processed.
	@retval false	not processed.
*/
// ---------------------------------------------------------------------
bool ExceptionMessageGenerator::processCoverDefParseException(
	Exception* ex,					//!< exception object
	MBCString& message				//!< OUTPUT. a message is returned.
) const
{
	CoverDefParseException* myException = dynamic_cast<CoverDefParseException*>(ex);
	if (NULL == myException)
	{
		return false;
	}

	bool resolved = false;
	MBCString eachMessage;

	// CoverDefParseExceptions::UnknownID
	if (!resolved)
	{
		CoverDefParseExceptions::UnknownID* myExceptionUID = dynamic_cast<CoverDefParseExceptions::UnknownID*>(ex);
		if (NULL != myExceptionUID)
		{
			MBCString id;
			UTF8Conv::ToMultiByte(id, myExceptionUID->GetID());
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_UNKNOWN_ID), id.CString());
			resolved = true;
		}
	}
	
	// CoverDefParseExceptions::UnknownClass
	if (!resolved)
	{
		CoverDefParseExceptions::UnknownClass* myExceptionUC = dynamic_cast<CoverDefParseExceptions::UnknownClass*>(ex);
		if (NULL != myExceptionUC)
		{
			MBCString classStr;
			UTF8Conv::ToMultiByte(classStr, myExceptionUC->GetClass());
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_UNKNOWN_CLASS), classStr.CString());
			resolved = true;
		}
	}
	
	// CoverDefParseExceptions::UnknownType
	if (!resolved)
	{
		CoverDefParseExceptions::UnknownType* myExceptionUT = dynamic_cast<CoverDefParseExceptions::UnknownType*>(ex);
		if (NULL != myExceptionUT)
		{
			MBCString typeStr;
			UTF8Conv::ToMultiByte(typeStr, myExceptionUT->GetType());
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_UNKNOWN_TYPE), typeStr.CString());
			resolved = true;
		}
	}
	
	// CoverDefParseExceptions::ValidationFailed
	if (!resolved)
	{
		CoverDefParseExceptions::ValidationFailed* myExceptionVF = dynamic_cast<CoverDefParseExceptions::ValidationFailed*>(ex);
		if (NULL != myExceptionVF)
		{
			Exception* innerException = ex->GetInnerException();
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_VALIDATION_FAILED), (NULL != innerException) ? innerException->GetErrorMessage() : ALITERAL(""));
			resolved = true;
		}
	}

	// CoverDefParseExceptions::ColorStringNotUnderstood
	if (!resolved)
	{
		CoverDefParseExceptions::ColorStringNotUnderstood* myExceptionCSNU = dynamic_cast<CoverDefParseExceptions::ColorStringNotUnderstood*>(ex);
		if (NULL != myExceptionCSNU)
		{
			MBCString color;
			UTF8Conv::ToMultiByte(color, myExceptionCSNU->GetColorString());
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_COLOR_STRING_NOT_UNDERSTOOD), color.CString());
			resolved = true;
		}
	}
	
	// CoverDefParseExceptions::UnsupportedCoverDefVersion
	if (!resolved)
	{
		CoverDefParseExceptions::UnsupportedCoverDefVersion* myExceptionUCDV = dynamic_cast<CoverDefParseExceptions::UnsupportedCoverDefVersion*>(ex);
		if (NULL != myExceptionUCDV)
		{
			MBCString version;
			UTF8Conv::ToMultiByte(version, myExceptionUCDV->GetVersionString());
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_UNSUPPORTED_COVERDEF_VERSION), version.CString());
			resolved = true;
		}		
	}
	
	// CoverDefParseExceptions::InvalidDocumentElement
	if (!resolved)
	{
		CoverDefParseExceptions::InvalidDocumentElement* myExceptionIDE = dynamic_cast<CoverDefParseExceptions::InvalidDocumentElement*>(ex);
		if (NULL != myExceptionIDE)
		{
			eachMessage = stringLoader->LoadNativeString(IDS_EMSG_INVALID_COVERDEF);
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		eachMessage.Empty();
	}

	MBCString preMessage = stringLoader->LoadNativeString(IDS_EMSG_COVERDEF_PARSE);
	message = preMessage + eachMessage;

	if (-1 != myException->GetLine())
	{
		MBCString lineColumn;
		MessageFormatter::Format(lineColumn, stringLoader->LoadNativeString(IDS_EMSG_XML_LINE_COLUMN), myException->GetLine(), myException->GetColumn());
		message += lineColumn;
	}
	return true;	
}

// ---------------------------------------------------------------------
//! Generate a message string about FileException.
/*!
	@retval	true	processed.
	@retval false	not processed.
*/
// ---------------------------------------------------------------------
bool ExceptionMessageGenerator::processFileException(
	Exception* ex,					//!< exception object
	MBCString& message				//!< OUTPUT. a message is returned.
) const
{
	if (processDIBFileLoaderException(ex, message))
		return true;
	
	FileException* myException = dynamic_cast<FileException*>(ex);
	if (NULL == myException)
	{
		return false;
	}

	bool resolved = false;
	
	// FileExceptions::AccessDenied
	if (!resolved)
	{
		FileExceptions::AccessDenied* myExceptionAD = dynamic_cast<FileExceptions::AccessDenied*>(ex);
		if (NULL != myExceptionAD)
		{
			message = stringLoader->LoadNativeString(IDS_EMSG_FILE_ACCESS_DENIED);
			resolved = true;
		}
	}
	
	// FileExceptions::DeviceFull
	if (!resolved)
	{
		FileExceptions::DeviceFull* myExceptionDF = dynamic_cast<FileExceptions::DeviceFull*>(ex);
		if (NULL != myExceptionDF)
		{
			message = stringLoader->LoadNativeString(IDS_EMSG_FILE_DEVICE_FULL);
			resolved = true;
		}
	}
	
	// FileExceptions::FileAlreadyExists
	if (!resolved)
	{
		FileExceptions::FileAlreadyExists* myExceptionFAE = dynamic_cast<FileExceptions::FileAlreadyExists*>(ex);
		if (NULL != myExceptionFAE)
		{
			message = stringLoader->LoadNativeString(IDS_EMSG_FILE_ALREADY_EXISTS);
			resolved = true;
		}
	}
		
	// FileExceptions::FileNotFound
	if (!resolved)
	{
		FileExceptions::FileNotFound* myExceptionFNF = dynamic_cast<FileExceptions::FileNotFound*>(ex);
		if (NULL != myExceptionFNF)
		{
			message = stringLoader->LoadNativeString(IDS_EMSG_FILE_NOT_FOUND);
			resolved = true;
		}
	}
	
	// FileExceptions::IOError
	if (!resolved)
	{
		FileExceptions::IOError* myExceptionIOE = dynamic_cast<FileExceptions::IOError*>(ex);
		if (NULL != myExceptionIOE)
		{
			message = stringLoader->LoadNativeString(IDS_EMSG_FILE_IO_ERROR);
			resolved = true;
		}
	}
	
	// FileExceptions::SharingViolation
	if (!resolved)
	{
		FileExceptions::SharingViolation* myExceptionSV = dynamic_cast<FileExceptions::SharingViolation*>(ex);
		if (NULL != myExceptionSV)
		{
			message = stringLoader->LoadNativeString(IDS_EMSG_FILE_SHARING_VIOLATION);
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		message = stringLoader->LoadNativeString(IDS_EMSG_FILE_OPERATION);
	}
	
	const Path& fileName = myException->GetFileName();
	if (!fileName.IsEmpty())
	{
		MBCString fileNameMessage;
		MessageFormatter::Format(fileNameMessage, stringLoader->LoadNativeString(IDS_EMSG_FILE_FILENAME), fileName.GetPathString());
		message += fileNameMessage;
	}

	return true;	
}

// ---------------------------------------------------------------------
//! Generate a message string about DIBFileLoaderException.
/*!
	@retval	true	processed.
	@retval false	not processed.
*/
// ---------------------------------------------------------------------
bool ExceptionMessageGenerator::processDIBFileLoaderException(
	Exception* ex,					//!< exception object
	MBCString& message				//!< OUTPUT. a message is returned.
) const
{
	DIBFileLoaderException* myException = dynamic_cast<DIBFileLoaderException*>(ex);
	if (NULL == myException)
	{
		return false;
	}

	bool resolved = false;

	// DIBFileLoaderExceptions::BrokenFile
	if (!resolved)
	{
		DIBFileLoaderExceptions::BrokenFile* myExceptionBF = dynamic_cast<DIBFileLoaderExceptions::BrokenFile*>(ex);
		if (NULL != myExceptionBF)
		{
			message = stringLoader->LoadNativeString(IDS_EMSG_DIBFILE_BROKEN_FILE);
			resolved = true;
		}
	}
	
	// DIBFileLoaderExceptions::UnknownFileFormat
	if (!resolved)
	{
		DIBFileLoaderExceptions::UnknownFileFormat* myExceptionUFF = dynamic_cast<DIBFileLoaderExceptions::UnknownFileFormat*>(ex);
		if (NULL != myExceptionUFF)
		{
			message = stringLoader->LoadNativeString(IDS_EMSG_DIBFILE_UNKNOWN_FORMAT);
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		message = stringLoader->LoadNativeString(IDS_EMSG_DIBFILE_LOAD);
	}
	
	const Path& fileName = myException->GetFileName();
	if (!fileName.IsEmpty())
	{
		MBCString fileNameMessage;
		MessageFormatter::Format(fileNameMessage, stringLoader->LoadNativeString(IDS_EMSG_FILE_FILENAME), fileName.GetPathString());
		message += fileNameMessage;
	}

	return true;	
}

// ---------------------------------------------------------------------
//! Generate a message string about UIControllerException.
/*!
	@retval	true	processed.
	@retval false	not processed.
*/
// ---------------------------------------------------------------------
bool ExceptionMessageGenerator::processUIControllerException(
	Exception* ex,					//!< exception object
	MBCString& message				//!< OUTPUT. a message is returned.
) const
{
	UIControllerException* myException = dynamic_cast<UIControllerException*>(ex);
	if (NULL == myException)
	{
		return false;
	}

	bool resolved = false;
	
	// UIControllerExceptions::FailedToShowContextMenu
	if (!resolved)
	{
		UIControllerExceptions::FailedToShowContextMenu* myExceptionFTSCM = dynamic_cast<UIControllerExceptions::FailedToShowContextMenu*>(ex);
		if (NULL != myExceptionFTSCM)
		{
			SInt32 menuId = myExceptionFTSCM->GetMenuID();
			MessageFormatter::Format(message, stringLoader->LoadNativeString(IDS_EMSG_UI_SHOW_MENU), menuId);
			resolved = true;
		}
	}
	
	// UIControllerExceptions::FailedToShowDialog
	if (!resolved)
	{
		UIControllerExceptions::FailedToShowDialog* myExceptionFTSD = dynamic_cast<UIControllerExceptions::FailedToShowDialog*>(ex);
		if (NULL != myExceptionFTSD)
		{
			SInt32 dialogId = myExceptionFTSD->GetDialogID();
			MessageFormatter::Format(message, stringLoader->LoadNativeString(IDS_EMSG_UI_SHOW_DIALOG), dialogId);
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		message = stringLoader->LoadNativeString(IDS_EMSG_UI_CONTROL);	
	}
	
	return true;
}

// ---------------------------------------------------------------------
//! Generate a message string about ColorCodedSkinException.
/*!
	@retval	true	processed.
	@retval false	not processed.
*/
// ---------------------------------------------------------------------
bool ExceptionMessageGenerator::processColorCodedSkinException(
	Exception* ex,					//!< exception object
	MBCString& message				//!< OUTPUT. a message is returned.
) const
{
	ColorCodedSkinException* myException = dynamic_cast<ColorCodedSkinException*>(ex);
	if (NULL == myException)
	{
		return false;
	}

	bool resolved = false;
	
	// ColorCodedSkinExceptions::BadInitParams
	if (!resolved)
	{
		ColorCodedSkinExceptions::BadInitParams* myExceptionBIP = dynamic_cast<ColorCodedSkinExceptions::BadInitParams*>(ex);
		if (NULL != myExceptionBIP)
		{
			message = stringLoader->LoadNativeString(IDS_EMSG_INIT_SKIN);
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		message = stringLoader->LoadNativeString(IDS_EMSG_COLOR_CODED_SKIN);
	}
	
	return true;
}

// ---------------------------------------------------------------------
//! Generate a message string about PathException.
/*!
	@retval	true	processed.
	@retval false	not processed.
*/
// ---------------------------------------------------------------------
bool ExceptionMessageGenerator::processPathException(
	Exception* ex,					//!< exception object
	MBCString& message				//!< OUTPUT. a message is returned.
) const
{
	PathException* myException = dynamic_cast<PathException*>(ex);
	if (NULL == myException)
	{
		return false;
	}

	bool resolved = false;
	
	// PathExceptions::RelativePathNotExist
	if (!resolved)
	{
		PathExceptions::RelativePathNotExist* myExceptionRPNE = dynamic_cast<PathExceptions::RelativePathNotExist*>(ex);
		if (NULL != myExceptionRPNE)
		{
			ConstAStr target = myExceptionRPNE->GetTarget();
			ConstAStr basePath = myExceptionRPNE->GetBase();

			MessageFormatter::Format(message,
											stringLoader->LoadNativeString(IDS_EMSG_PATH_MAKE_RELATIVE),
											(target) ? target : ALITERAL(""),
											(basePath) ? basePath : ALITERAL(""));
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		message = stringLoader->LoadNativeString(IDS_EMSG_PATH_OPERATION);
	}
	
	return true;
}

// ---------------------------------------------------------------------
//! Generate a message string about XMLLangFileException.
/*!
	@retval	true	processed.
	@retval false	not processed.
*/
// ---------------------------------------------------------------------
bool ExceptionMessageGenerator::processXMLLangFileException(
	Exception* ex,					//!< exception object
	MBCString& message				//!< OUTPUT. a message is returned.
) const
{
	XMLLangFileException* myException = dynamic_cast<XMLLangFileException*>(ex);
	if (NULL == myException)
	{
		return false;
	}

	bool resolved = false;
	MBCString eachMessage;

	// XMLLangFileExceptions::ValidationFailed
	if (!resolved)
	{
		XMLLangFileExceptions::ValidationFailed* myExceptionVF = dynamic_cast<XMLLangFileExceptions::ValidationFailed*>(ex);
		if (NULL != myExceptionVF)
		{
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_LANG_VALIDATION_FAILED), myExceptionVF->GetErrorMessage());
			resolved = true;
		}
	}

	// XMLLangFileExceptions::StringNotDefined
	if (!resolved)
	{
		XMLLangFileExceptions::StringNotDefined* myExceptionSND = dynamic_cast<XMLLangFileExceptions::StringNotDefined*>(ex);
		if (NULL != myExceptionSND)
		{
			MessageFormatter::Format(eachMessage, stringLoader->LoadNativeString(IDS_EMSG_LANG_STRING_NOT_DEFINED), myExceptionSND->GetName());
			resolved = true;
		}
	}
	
	// XMLLangFileExceptions::InvalidDialogID
	// XMLLangFileExceptions::InvalidStringID
		
	if (!resolved)
	{
		eachMessage.Empty();
	}

	message = eachMessage;

	if (-1 != myException->GetLine())
	{
		MBCString lineColumn;
		MessageFormatter::Format(lineColumn, stringLoader->LoadNativeString(IDS_EMSG_XML_LINE_COLUMN), myException->GetLine(), myException->GetColumn());
		message += lineColumn;
	}
	return true;	
}

/**
 *	@brief	Generates a message string about DialogLayoutException.
 *	@retval	true	processed.
 *	@retval	false	not processed.
 */
bool ExceptionMessageGenerator::processDialogLayoutException(Exception* ex, MBCString& message) const
{
	DialogLayoutException* myException = dynamic_cast<DialogLayoutException*>(ex);
	if (NULL == myException)
	{
		return false;
	}

	bool resolved = false;

	// DialogLayoutExceptions::FailedToCompute
	if (!resolved)
	{
		DialogLayoutExceptions::FailedToCompute* myExceptionFTC = dynamic_cast<DialogLayoutExceptions::FailedToCompute*>(ex);
		if (NULL != myExceptionFTC)
		{
			ConstAStr arrow = ALITERAL(" -> ");
			MBCString leafMessage;
			MBCString itemName = myExceptionFTC->GetItemName();
			MBCString itemRefPath;
			MBCString itemRefPathLine = itemName;
			Exception* iex = myExceptionFTC->GetInnerException();
			while (NULL != iex)
			{
				DialogLayoutExceptions::FailedToCompute* ftcInner = dynamic_cast<DialogLayoutExceptions::FailedToCompute*>(iex);
				if (NULL != ftcInner)
				{
					itemRefPathLine += arrow;
					MBCString innerItemName = ftcInner->GetItemName();
					if (itemRefPathLine.Length() + innerItemName.Length() >= 80)
					{
						itemRefPath += itemRefPathLine;
						itemRefPathLine = ALITERAL("\n  ");
					}
					itemRefPathLine += innerItemName;
					iex = ftcInner->GetInnerException();
				}
				else
				{
					GetMessageString(iex, leafMessage);
					break;
				}
			}

			itemRefPath += itemRefPathLine;
			MessageFormatter::Format(message, stringLoader->LoadNativeString(IDS_EMSG_DIALOG_LAYOUT_COMPUTE), itemName.CString(), leafMessage.CString(), itemRefPath.CString());
			resolved = true;
		}
	}

	// DialogLayoutExceptions::ItemNotFound
	if (!resolved)
	{
		DialogLayoutExceptions::ItemNotFound* myExceptionINF = dynamic_cast<DialogLayoutExceptions::ItemNotFound*>(ex);
		if (NULL != myExceptionINF)
		{
			MessageFormatter::Format(message, stringLoader->LoadNativeString(IDS_EMSG_DIALOG_LAYOUT_NOT_FOUND), myExceptionINF->GetItemName().CString());
			resolved = true;
		}
	}

	// DialogLayoutExceptions::CircularReference
	if (!resolved)
	{
		DialogLayoutExceptions::CircularReference* myExceptionCR = dynamic_cast<DialogLayoutExceptions::CircularReference*>(ex);
		if (NULL != myExceptionCR)
		{
			message = stringLoader->LoadNativeString(IDS_EMSG_DIALOG_LAYOUT_CIRCULAR_REF);
			resolved = true;
		}
	}

	// DialogLayoutExceptions::ItemNotComputed
	if (!resolved)
	{
		// This must be a bug of the application. It must not be occured in end-user environment.
	}

	return true;
}
