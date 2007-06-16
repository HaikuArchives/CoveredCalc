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
	UIMessageProvider* uiMessageProvider		//!< message provider object.
)
{
	this->uiMessageProvider = uiMessageProvider;
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
				uiMessageProvider->GetMessage(IDS_EMSG_GENERIC, message);
			}
			catch (Exception* ex3)
			{
				ex3->Delete();
				message = "An error has occurred.";			
			}

			ConstAStr errorMessage = ex->GetErrorMessage();
			if (NULL != errorMessage && '\0' != errorMessage[0])
			{
				message += "\n";
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
		uiMessageProvider->GetMessage(IDS_EMSG_MEMORY, message);
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
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_NO_MATCH_START_TAG, myExceptionNMST->GetName());
			resolved = true;
		}
	}
	
	// XMLParseExceptions::InvalidParentEntity
	if (!resolved)
	{
		XMLParseExceptions::InvalidParentEntity* myExceptionIPE = dynamic_cast<XMLParseExceptions::InvalidParentEntity*>(ex);
		if (NULL != myExceptionIPE)
		{
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_INVALID_PARENT_ENTITY, myExceptionIPE->GetName(), myExceptionIPE->GetParentName());
			resolved = true;
		}
	}
		
	// XMLParseExceptions::MissingAttribute
	if (!resolved)
	{
		XMLParseExceptions::MissingAttribute* myExceptionMA = dynamic_cast<XMLParseExceptions::MissingAttribute*>(ex);
		if (NULL != myExceptionMA)
		{
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_MISSING_ATTRIBUTE, myExceptionMA->GetTagName(), myExceptionMA->GetAttributeName());
			resolved = true;
		}
	}
	
	// XMLParseExceptions::UnknownTag
	if (!resolved)
	{
		XMLParseExceptions::UnknownTag* myExceptionUT = dynamic_cast<XMLParseExceptions::UnknownTag*>(ex);
		if (NULL != myExceptionUT)
		{
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_UNKNOWN_TAG, myExceptionUT->GetName());
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		eachMessage.Empty();
	}
	
	MBCString preMessage;
	uiMessageProvider->GetMessage(IDS_EMSG_XML_PARSE, preMessage);
	message = preMessage + eachMessage;
	
	if (-1 != myException->GetLine())
	{
		MBCString lineColumn;
		uiMessageProvider->GetNFormatMessage(lineColumn, IDS_EMSG_XML_LINE_COLUMN, myException->GetLine(), myException->GetColumn());
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
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_UNKNOWN_ID, myExceptionUID->GetID());
			resolved = true;
		}
	}
	
	// CoverDefParseExceptions::UnknownClass
	if (!resolved)
	{
		CoverDefParseExceptions::UnknownClass* myExceptionUC = dynamic_cast<CoverDefParseExceptions::UnknownClass*>(ex);
		if (NULL != myExceptionUC)
		{
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_UNKNOWN_CLASS, myExceptionUC->GetClass());
			resolved = true;
		}
	}
	
	// CoverDefParseExceptions::UnknownType
	if (!resolved)
	{
		CoverDefParseExceptions::UnknownType* myExceptionUT = dynamic_cast<CoverDefParseExceptions::UnknownType*>(ex);
		if (NULL != myExceptionUT)
		{
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_UNKNOWN_TYPE, myExceptionUT->GetType());
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
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_VALIDATION_FAILED, (NULL != innerException) ? innerException->GetErrorMessage() : "");			
			resolved = true;
		}
	}

	// CoverDefParseExceptions::ColorStringNotUnderstood
	if (!resolved)
	{
		CoverDefParseExceptions::ColorStringNotUnderstood* myExceptionCSNU = dynamic_cast<CoverDefParseExceptions::ColorStringNotUnderstood*>(ex);
		if (NULL != myExceptionCSNU)
		{
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_COLOR_STRING_NOT_UNDERSTOOD, myExceptionCSNU->GetColorString());
			resolved = true;
		}
	}
	
	// CoverDefParseExceptions::UnsupportedCoverDefVersion
	if (!resolved)
	{
		CoverDefParseExceptions::UnsupportedCoverDefVersion* myExceptionUCDV = dynamic_cast<CoverDefParseExceptions::UnsupportedCoverDefVersion*>(ex);
		if (NULL != myExceptionUCDV)
		{
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_UNSUPPORTED_COVERDEF_VERSION, myExceptionUCDV->GetVersionString());
			resolved = true;
		}		
	}
	
	// CoverDefParseExceptions::InvalidDocumentElement
	if (!resolved)
	{
		CoverDefParseExceptions::InvalidDocumentElement* myExceptionIDE = dynamic_cast<CoverDefParseExceptions::InvalidDocumentElement*>(ex);
		if (NULL != myExceptionIDE)
		{
			uiMessageProvider->GetMessage(IDS_EMSG_INVALID_COVERDEF, eachMessage);
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		eachMessage.Empty();
	}

	MBCString preMessage;
	uiMessageProvider->GetMessage(IDS_EMSG_COVERDEF_PARSE, preMessage);
	message = preMessage + eachMessage;

	if (-1 != myException->GetLine())
	{
		MBCString lineColumn;
		uiMessageProvider->GetNFormatMessage(lineColumn, IDS_EMSG_XML_LINE_COLUMN, myException->GetLine(), myException->GetColumn());
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
			uiMessageProvider->GetMessage(IDS_EMSG_FILE_ACCESS_DENIED, message);
			resolved = true;
		}
	}
	
	// FileExceptions::DeviceFull
	if (!resolved)
	{
		FileExceptions::DeviceFull* myExceptionDF = dynamic_cast<FileExceptions::DeviceFull*>(ex);
		if (NULL != myExceptionDF)
		{
			uiMessageProvider->GetMessage(IDS_EMSG_FILE_DEVICE_FULL, message);
			resolved = true;
		}
	}
	
	// FileExceptions::FileAlreadyExists
	if (!resolved)
	{
		FileExceptions::FileAlreadyExists* myExceptionFAE = dynamic_cast<FileExceptions::FileAlreadyExists*>(ex);
		if (NULL != myExceptionFAE)
		{
			uiMessageProvider->GetMessage(IDS_EMSG_FILE_ALREADY_EXISTS, message);
			resolved = true;
		}
	}
		
	// FileExceptions::FileNotFound
	if (!resolved)
	{
		FileExceptions::FileNotFound* myExceptionFNF = dynamic_cast<FileExceptions::FileNotFound*>(ex);
		if (NULL != myExceptionFNF)
		{
			uiMessageProvider->GetMessage(IDS_EMSG_FILE_NOT_FOUND, message);
			resolved = true;
		}
	}
	
	// FileExceptions::IOError
	if (!resolved)
	{
		FileExceptions::IOError* myExceptionIOE = dynamic_cast<FileExceptions::IOError*>(ex);
		if (NULL != myExceptionIOE)
		{
			uiMessageProvider->GetMessage(IDS_EMSG_FILE_IO_ERROR, message);
			resolved = true;
		}
	}
	
	// FileExceptions::SharingViolation
	if (!resolved)
	{
		FileExceptions::SharingViolation* myExceptionSV = dynamic_cast<FileExceptions::SharingViolation*>(ex);
		if (NULL != myExceptionSV)
		{
			uiMessageProvider->GetMessage(IDS_EMSG_FILE_SHARING_VIOLATION, message);
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		uiMessageProvider->GetMessage(IDS_EMSG_FILE_OPERATION, message);
	}
	
	const Path& fileName = myException->GetFileName();
	if (!fileName.IsEmpty())
	{
		MBCString fileNameMessage;
		uiMessageProvider->GetNFormatMessage(fileNameMessage, IDS_EMSG_FILE_FILENAME, fileName.GetPathString());
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
			uiMessageProvider->GetMessage(IDS_EMSG_DIBFILE_BROKEN_FILE, message);
			resolved = true;
		}
	}
	
	// DIBFileLoaderExceptions::UnknownFileFormat
	if (!resolved)
	{
		DIBFileLoaderExceptions::UnknownFileFormat* myExceptionUFF = dynamic_cast<DIBFileLoaderExceptions::UnknownFileFormat*>(ex);
		if (NULL != myExceptionUFF)
		{
			uiMessageProvider->GetMessage(IDS_EMSG_DIBFILE_UNKNOWN_FORMAT, message);
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		uiMessageProvider->GetMessage(IDS_EMSG_DIBFILE_LOAD, message);
	}
	
	const Path& fileName = myException->GetFileName();
	if (!fileName.IsEmpty())
	{
		MBCString fileNameMessage;
		uiMessageProvider->GetNFormatMessage(fileNameMessage, IDS_EMSG_FILE_FILENAME, fileName.GetPathString());
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
			uiMessageProvider->GetNFormatMessage(message, IDS_EMSG_UI_SHOW_MENU, menuId);
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
			uiMessageProvider->GetNFormatMessage(message, IDS_EMSG_UI_SHOW_DIALOG, dialogId);
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		uiMessageProvider->GetMessage(IDS_EMSG_UI_CONTROL, message);	
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
			uiMessageProvider->GetMessage(IDS_EMSG_INIT_SKIN, message);
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		uiMessageProvider->GetMessage(IDS_EMSG_COLOR_CODED_SKIN, message);
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

			uiMessageProvider->GetNFormatMessage(message, IDS_EMSG_PATH_MAKE_RELATIVE,
											(target) ? target : "",
											(basePath) ? basePath : "");
			resolved = true;
		}
	}
	
	if (!resolved)
	{
		uiMessageProvider->GetMessage(IDS_EMSG_PATH_OPERATION, message);
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
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_LANG_VALIDATION_FAILED, myExceptionVF->GetErrorMessage());
			resolved = true;
		}
	}

	// XMLLangFileExceptions::StringNotDefined
	if (!resolved)
	{
		XMLLangFileExceptions::StringNotDefined* myExceptionSND = dynamic_cast<XMLLangFileExceptions::StringNotDefined*>(ex);
		if (NULL != myExceptionSND)
		{
			uiMessageProvider->GetNFormatMessage(eachMessage, IDS_EMSG_LANG_STRING_NOT_DEFINED, myExceptionSND->GetName());
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
		uiMessageProvider->GetNFormatMessage(lineColumn, IDS_EMSG_XML_LINE_COLUMN, myException->GetLine(), myException->GetColumn());
		message += lineColumn;
	}
	return true;	
}
