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
	@file		CommandLineParam.h
	@brief		Definition of CommandLineParam class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.11.16 created
*/

#ifndef _COMMANDLINEPARAM_H_
#define _COMMANDLINEPARAM_H_

#include "Path.h"

/**
 *	@brief	This class analyzes command line parameters and hold them.
 */
class CommandLineParam
{
public:
								CommandLineParam();
	virtual						~CommandLineParam();
	
	void						SetParameter(SInt32 argc, AStr* argv);
	
	bool						IsSettingFileSpecified() const;
	const Path&					GetSettingFile() const;
	
	bool						IsLangFileSpecified() const;
	const Path&					GetLangFile() const;

private:
	Path						settingFile;			///< setting file path.
	Path						langFile;				///< language file path.
};

#endif // _COMMANDLINEPARAM_H_
