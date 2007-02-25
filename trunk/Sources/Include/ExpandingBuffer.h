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
	@file		ExpandingBuffer.h
	@brief		Definition and implementation of ExpandingBuffer class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.11.26 created
*/

#ifndef _EXPANDINGBUFFER_H_
#define _EXPANDINGBUFFER_H_

#include <malloc.h>
#include "MemoryException.h"

// ---------------------------------------------------------------------
//! The buffer which expands according to added data.
// ---------------------------------------------------------------------
template <class TYPE>
class ExpandingBuffer
{
public:
						ExpandingBuffer();
	virtual				~ExpandingBuffer();
	
	void				Init(SInt32 initialAlloc, SInt32 deltaUnit);
	
	void				Append(const TYPE* source, SInt32 length);
	const TYPE*			GetBufferPtr() const { return buffer; }
	SInt32				GetUsedSize() const { return usedSize; }

private:
	void				readyBuffer(SInt32 allocSize);

private:
	TYPE*				buffer;
	SInt32				allocatedSize;
	SInt32				usedSize;
	SInt32				deltaUnit;
};

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
template <class TYPE>
ExpandingBuffer<TYPE>::ExpandingBuffer()
{
	buffer = NULL;
	allocatedSize = 0;
	usedSize = 0;
	deltaUnit = 0;
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
template <class TYPE>
ExpandingBuffer<TYPE>::~ExpandingBuffer()
{
	if (NULL != buffer)
	{
		free(buffer);
	}
}

// ---------------------------------------------------------------------
//! Initialize the object
// ---------------------------------------------------------------------
template <class TYPE>
void ExpandingBuffer<TYPE>::Init(
	SInt32 initialAlloc,		//!< initial allocation size
	SInt32 deltaUnit			//!< expanding unit
)
{
	if (NULL != buffer)
	{
		free(buffer);
	}

	buffer = static_cast<TYPE*>(malloc(initialAlloc * sizeof(TYPE)));
	if (NULL == buffer)
	{
		MemoryException::Throw();
	}
	this->allocatedSize = initialAlloc;
	this->usedSize = 0;
	this->deltaUnit = deltaUnit;
}

// ---------------------------------------------------------------------
//! Appends the data
// ---------------------------------------------------------------------
template <class TYPE>
void ExpandingBuffer<TYPE>::Append(
	const TYPE* source,			//!< data to add
	SInt32 length				//!< the length of data
)
{
	readyBuffer(usedSize + length);
	
	memcpy(buffer + usedSize, source, length * sizeof(TYPE));
	usedSize += length;
}

// ---------------------------------------------------------------------
//! Allocates the buffer if needed
// ---------------------------------------------------------------------
template <class TYPE>
void ExpandingBuffer<TYPE>::readyBuffer(
	SInt32 allocSize			//!< allocation size
)
{
	if (allocatedSize >= allocSize)
	{
		return;
	}
	
	SInt32 newSize = (((allocSize - 1) / deltaUnit) + 1) * deltaUnit;
	
	TYPE* newBuffer = static_cast<TYPE*>(realloc(buffer, newSize));
	if (NULL == newBuffer)
	{
		MemoryException::Throw();
	}
	
	buffer = newBuffer;
	allocatedSize = newSize;
}

#endif // _EXPANDINGBUFFER_H_
