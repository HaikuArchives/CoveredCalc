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
	@file		Enumerator.h
	@brief		Definition of Enumerator interface.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2007.10.11 created
*/

#ifndef _ENUMERATOR_H_
#define _ENUMERATOR_H_

/**
 *	@brief	The base interface of the reading data in sequence.
 *
 *	Enumerator can access items in sequence, but cannot do modifiable operation to the collection.
 *	
 *	Initially, the enumerator is positioned before the first element in the collection.
 *	Reset() moves the enumerator to this position.
 *	Call MoveNext() before you get the data by calling GetCurrent().
 *	GetCurrent() returns the element at the current position.
 *	If the enumerator is positioned at the last element, MoveNext() returns false and its position is not moved.
 *	The pointer returned by GetCurrent() is temporary pointer and it becomes invalid when next MoveNext() or Reset() is called.
 *	
 *	An enumerator is valid until the collection changes.
 *	If the collection changed, a behavior of subsequent call MoveNext(), Reset() or GetCurrent() is undefined.
 *
 */
template <class TYPE>
class Enumerator
{
public:
						Enumerator() {}
	virtual				~Enumerator() {}
	
	/**
	 *	@brief	Returns a pointer to the current element.
	 */
	virtual TYPE*		GetCurrent() = 0;
	
	/**
	 *	@brief	Moves to the next element.
	 *	@return	true if it moved to the next element. false if it is the end of collection.
	 */
	virtual bool		MoveNext() = 0;
	
	/**
	 *	@brief	Moves to the initial position.
	 *	@return	true if it successfully moved. otherwise false (it includes that implementation does not support this operation).
	 */
	virtual bool		Reset() = 0;
};

#endif // _ENUMERATOR_H_
