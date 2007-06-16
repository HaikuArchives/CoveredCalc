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
	@file		CoverManager.cpp
	@brief		Implementation of CoverManager class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.16 created
*/

#include "Prefix.h"
#include "XMLParser.h"
#include "Exception.h"
#include "CoverDef.h"
#include "ColorCodedSkin.h"
#include "CoverChangeEventHandler.h"
#include "CoverManager.h"
#include "CoverDefParser.h"

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
CoverManager::CoverManager()
{
	coverDef = NULL;	
	currentCover = 0;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
CoverManager::~CoverManager()
{
	if (NULL != coverDef)
	{
		delete coverDef;
	}
}

// ---------------------------------------------------------------------
//! Reads the XML file "CoverDef.xml"
/*!
	@exception FileException Thrown when failed in reading files.
	@exception CoverDefParseException Thrown when failed in parsing "CoverDef.xml".
*/
// ---------------------------------------------------------------------
void CoverManager::Load(
	const Path& filePath,						//!< file path of "CoverDef.xml"
	SInt32 coverNo								//!< cover No
)
{
	// save old cover infomations
	CoverDef* oldCoverDef = coverDef;
	Path oldCoverFilePath = coverFilePath;
	SInt32 oldCoverNo = currentCover;

	// read "CoverDef.xml"
	CoverDefParser parser;
	parser.Parse(filePath);
	coverDef = parser.DetachCoverDef();

	// set file path and cover No.
	coverFilePath = filePath;
	if (0 <= coverNo && coverNo < coverDef->GetNumCoverInfos())
	{
		currentCover = coverNo;
	}
	else
	{
		currentCover = 0;
	}
	
	// fire an event
	try
	{
		CoverChangeEventHandlerList::iterator eventIte;
		for (eventIte=coverChangeEventHandlers.begin(); eventIte!=coverChangeEventHandlers.end(); eventIte++)
		{
			(*eventIte)->CoverDefChanged();
		}
	}
	catch (Exception*)
	{
		// restore old cover
		delete coverDef;
		coverDef = oldCoverDef;
		coverFilePath = oldCoverFilePath;
		currentCover = oldCoverNo;

		try
		{		
			CoverChangeEventHandlerList::iterator eventIte;
			for (eventIte=coverChangeEventHandlers.begin(); eventIte!=coverChangeEventHandlers.end(); eventIte++)
			{
				(*eventIte)->CoverDefChanged();
			}
		}
		catch (Exception* ex2)
		{
			// ignore...
			ex2->Delete();
		}
		
		throw;
	}

	if (NULL != oldCoverDef)
	{	
		delete oldCoverDef;
	}
}

// ---------------------------------------------------------------------
//! Sets current cover number
// ---------------------------------------------------------------------
void CoverManager::SetCurrentCover(
	SInt32 currentCover		// current cover no.
)
{
	ASSERT(NULL != coverDef);
	if (NULL != coverDef)
	{
		if (0 <= currentCover && currentCover < coverDef->GetNumCoverInfos() && this->currentCover != currentCover)
		{
			SInt32 oldCoverNo = this->currentCover;
			this->currentCover = currentCover;
			
			// fire an event
			try
			{
				CoverChangeEventHandlerList::iterator eventIte;
				for (eventIte=coverChangeEventHandlers.begin(); eventIte!=coverChangeEventHandlers.end(); eventIte++)
				{
					(*eventIte)->CurrentCoverChanged();
				}
			}
			catch (Exception*)
			{
				// restore old cover no.
				this->currentCover = oldCoverNo;
				try
				{
					CoverChangeEventHandlerList::iterator eventIte;
					for (eventIte=coverChangeEventHandlers.begin(); eventIte!=coverChangeEventHandlers.end(); eventIte++)
					{
						(*eventIte)->CurrentCoverChanged();
					}
				}
				catch (Exception* ex2)
				{
					// ignore...
					ex2->Delete();
				}
				
				throw;
			}
		}
	}
}

// ---------------------------------------------------------------------
//! Registers CoverChangeEventHandler object
// ---------------------------------------------------------------------
void CoverManager::RegisterCoverChangeEventHandler(
	CoverChangeEventHandler* handler				//!< handler
)
{
	coverChangeEventHandlers.push_back(handler);
}

// ---------------------------------------------------------------------
//! Unregisters CoverChangeEventHandler object
// ---------------------------------------------------------------------
void CoverManager::UnregisterCoverChangeEventHandler(
	CoverChangeEventHandler* handler				//!< handler
)
{
	coverChangeEventHandlers.remove(handler);
}

