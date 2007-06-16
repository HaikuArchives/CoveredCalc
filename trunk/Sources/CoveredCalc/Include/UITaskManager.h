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
	@file		UITaskManager.h
	@brief		Definition of UITaskManager class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.11.04 created
*/

#ifndef _UITASKMANAGER_H_
#define _UITASKMANAGER_H_

#include <vector>

class UITask;

typedef	std::vector<UITask*>	UITaskVector;

/**
 *	@brief	This class manages UITask objects which are currently running.
 */
class UITaskManager
{
public:
						UITaskManager();
	virtual				~UITaskManager();

	void				StartTask(UITask* task);
	void				EndTask(UITask* task);
	void				EndAllTasks();

	UITask*				FindOneTask(SInt32 taskClass);
	bool				FindAllTask(SInt32 taskClass, UITaskVector& vector);

	class Command
	{
	public:
		/**
		 *	@brief	Execute command with a task.
		 *	@retval	true	wants to continue enumerating.
		 *	@retval	false	wants to stop enumerating.
		 */
		virtual bool	DoIt(UITask* task) = 0;
	};
	bool				DoForEach(Command* command);

private:
	bool				canEraseItem()	{ return (enumerating > 0) ? false : true; }
	void				eraseNoUseItems();

private:
	UITaskVector		tasks;			///< tasks
	SInt32				enumerating;	///< incremented at beginning of DoForEach func and decremented at endding of DoForEach func.
};

#endif // _UITASKMANAGER_H_
