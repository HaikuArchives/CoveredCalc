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
	@file		CommandLineParam.cpp
	@brief		Definition of CommandLineParam class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.11.16 created
*/

#include "Prefix.h"
#include "CommandLineParam.h"

static const AChar ParamSettingFileShort[]	= ALITERAL("f");
static const AChar ParamSettingFileLong[] = ALITERAL("-setting-file");
static const AChar ParamSettingLangFileLong[] = ALITERAL("-language-file");

/**
 *	@brief	Constructor
 */
CommandLineParam::CommandLineParam()
{
	settingFile.Empty();
	langFile.Empty();
}

/**
 *	@brief	Destructor
 */
CommandLineParam::~CommandLineParam()
{
}

void CommandLineParam::SetParameter(
	SInt32 argc,	///< number of parameters
	AStr* argv		///< array of each parameter
)
{
	enum ParamSwitch
	{
		ParamSwitch_None,			///< no parameter switch
		ParamSwitch_SettingFile,	///< setting file switch
		ParamSwitch_LangFile,		///< language file switch
	};
	
	ParamSwitch currentSwitch = ParamSwitch_None;
	SInt32 index;
	for (index=1; index<argc; index++)
	{
		AStr currentArg = argv[index];
		if (ParamSwitch_SettingFile == currentSwitch)	// setting file
		{
			settingFile.Assign(currentArg);
			currentSwitch = ParamSwitch_None;
		}
		else if (ParamSwitch_LangFile == currentSwitch)	// language file
		{
			langFile.Assign(currentArg);
			currentSwitch = ParamSwitch_None;
		}
		else if ('-' == currentArg[0])	// parameter switch
		{
			MBCString paramArg = currentArg[1];
			if (0 == paramArg.Compare(ParamSettingFileShort) ||
					0 == paramArg.Compare(ParamSettingFileLong))
			{
				currentSwitch = ParamSwitch_SettingFile;
			}
			else if (0 == paramArg.Compare(ParamSettingLangFileLong))
			{
				currentSwitch = ParamSwitch_LangFile;
			}
			else
			{
				// unknown parameter switch. ignore it.
				currentSwitch = ParamSwitch_None;
			}
		}
		else	// unknown parameter. ignore it.
		{
			currentSwitch = ParamSwitch_None;
		}
	}
}

/**
 *	@brief	Returns whether setting file parameter is specified.
 *	@retval	true	if specified.
 *	@retval	false	if not specified.
 */
bool CommandLineParam::IsSettingFileSpecified() const
{
	return !settingFile.IsEmpty();
}

/**
 *	@brief	Retrieves value of setting file parameter.
 *	@return	setting file parameter path is returned.
 */
const Path& CommandLineParam::GetSettingFile() const
{
	return settingFile;
}

/**
 *	@brief	Returns whether language file parameter is specified.
 *	@retval	true	if specified.
 *	@retval	false	if not specified.
 */
bool CommandLineParam::IsLangFileSpecified() const
{
	return !langFile.IsEmpty();
}

/**
 *	@brief	Retrieves value of language file parameter.
 *	@return	language file parameter path is returned.
 */
const Path& CommandLineParam::GetLangFile() const
{
	return langFile;
}
