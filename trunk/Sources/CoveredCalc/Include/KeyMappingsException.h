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
	@file		KeyMappingsException.h
	@brief		Definition of KeyMappingsException
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.10.11 created
*/

#ifndef _KEYMAPPINGSEXCEPTION_H_
#define _KEYMAPPINGSEXCEPTION_H_

#include "Exception.h"
#include "MBCString.h"

/**
 *	@brief	This exception is about key mappings.
 */
class KeyMappingsException : public Exception
{
public:
						KeyMappingsException() : Exception() {}
						KeyMappingsException(Exception* inner) : Exception(inner) {}
	virtual				~KeyMappingsException() {}
};

namespace KeyMappingsExceptions
{
	/**
	 *	@brief	Thrown when KeyMappings object is not ready.
	 *	@note	KeyMappings object is not ready until CreateNew() or Load() is called.
	 */
	class NotReady : public KeyMappingsException
	{
	public:
							NotReady() : KeyMappingsException() {}
		virtual				~NotReady() { }

		virtual	ConstAStr	GetErrorMessage() const
								{ return "Key-mappings is not ready yet."; }
	};

	/**
	 *	@brief	Thrown when failed to load a key-mapping file.
	 */
	class LoadFailed : public KeyMappingsException
	{
	public:
							LoadFailed() : KeyMappingsException() { }
							LoadFailed(AStr message) : KeyMappingsException() { this->message = message; }
		virtual				~LoadFailed() { }

		virtual	ConstAStr	GetErrorMessage() const
								{ return "Failed to load a key-mappings file."; }

		ConstAStr			GetMessage() const { return message; }

	private:
		MBCString			message;
	};
}

#endif // _KEYMAPPINGSEXCEPTION_H_
