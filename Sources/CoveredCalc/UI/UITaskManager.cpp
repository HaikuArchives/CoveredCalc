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
	@brief		Implementation of UITaskManager class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.11.05 created
*/

#include "Prefix.h"
#include "UITaskManager.h"
#include "UITask.h"

/**
 *	@brief	Constructor
 */
UITaskManager::UITaskManager()
{
	enumerating = 0;
}

/**
 *	@brief	Destructor
 */
UITaskManager::~UITaskManager()
{
	EndAllTasks();
}

/**
 *	@brief		Starts specified task.
 *	@param[in]	task	a task which is starting.
 */
void UITaskManager::StartTask(UITask *task)
{
	ASSERT(NULL != task);
	if (NULL == task)
		return;

	tasks.push_back(task);
	task->StartTask();
}

/**
 *	@brief		Ends specified task.
 *	@param[in]	task	a task which will be end.
 */
void UITaskManager::EndTask(UITask *task)
{
	ASSERT(NULL != task);
	if (NULL == task)
		return;

	UITaskVector::iterator ite;
	for (ite = tasks.begin(); ite != tasks.end(); ite++)
	{
		if (task == *ite)
		{
			delete *ite;
			*ite = NULL;
			if (canEraseItem())
			{
				tasks.erase(ite);
			}
			break;
		}
	}
}

/**
 *	@brief	Ends all tasks.
 */
void UITaskManager::EndAllTasks()
{
	UITaskVector::iterator ite;
	for (ite = tasks.begin(); ite != tasks.end(); ite++)
	{
		delete *ite;
		*ite = NULL;
	}
	if (canEraseItem())
	{
		tasks.clear();
	}
}

/**
 *	@brief		Searches a task which is specified task class.
 *	@param[in]	taskClass	task class of searching.
 *	@return		pointer to UITask object. or NULL when not found.
 */
UITask* UITaskManager::FindOneTask(SInt32 taskClass)
{
	UITaskVector::iterator ite;
	for (ite = tasks.begin(); ite != tasks.end(); ite++)
	{
		UITask* aTask = *ite;
		if (NULL != aTask && aTask->GetClass() == taskClass)
		{
			return aTask;
		}
	}
	return NULL;
}

/**
 *	@brief		Searches all tasks which are specified task class.
 *	@param[in]	taskClass	task class of searching.
 *	@param[out]	vector	found tasks are added to this vector.
 *	@retval		true	when one or more tasks are found.
 *	@retval		false	not found.
 */
bool UITaskManager::FindAllTask(SInt32 taskClass, UITaskVector& vector)
{
	bool ret = false;
	UITaskVector::iterator ite;
	for (ite = tasks.begin(); ite != tasks.end(); ite++)
	{
		UITask* aTask = *ite;
		if (NULL != aTask && aTask->GetClass() == taskClass)
		{
			vector.push_back(aTask);
			ret = true;
		}
	}
	return ret;
}

/**
 *	@brief		Executes specified command for each tasks.
 *	@param[in]	command		command to execute.
 *	@retval		true	when any command did not return false.
 *	@retval		false	when some command returned true.
 */
bool UITaskManager::DoForEach(UITaskManager::Command *command)
{
	enumerating++;

	bool ret = true;
	try
	{
		UITaskVector::iterator ite;
		for (ite = tasks.begin(); ite != tasks.end(); ite++)
		{
			if (NULL != *ite)
			{
				if (!command->DoIt(*ite))
				{
					ret = false;
					break;
				}
			}
		}
	}
	catch (...)
	{
		enumerating--;
		eraseNoUseItems();
		throw;
	}
	enumerating--;
	eraseNoUseItems();

	return ret;
}

/**
 *	@brief	Erases deleted task.
 *	@note	in task enumerating, deleted tasks are not erased at the time. Such task is replaced by NULL pointer.
 */
void UITaskManager::eraseNoUseItems()
{
	if (!canEraseItem())
	{
		return;
	}

	UITaskVector::iterator ite;
	for (ite = tasks.begin(); ite != tasks.end(); )
	{
		if (NULL == *ite)
		{
			ite = tasks.erase(ite);
		}
		else
		{
			ite++;
		}
	}
}
