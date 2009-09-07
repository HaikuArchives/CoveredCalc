/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2009 CoveredCalc Project Contributors
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
	@file		BeColumnListViewAdapter.cpp
	@brief		Implementation of BeColumnListViewAdapter class.
	@author		ICHIMIYA Hironori (Hiron)
	@date		2009.03.21 created
*/

#include "Prefix.h"
#include "BeColumnListViewAdapter.h"
#include "ColumnListView.h"
#include "ColumnTypes.h"
#include "CommandID.h"

/**
 *	@brief	Constructor
 */
BeColumnListViewAdapter::BeColumnListViewAdapter()
{
	stoppedSelectionChangedCommand = ID_NULL;
	stoppedSelectionChangedNotificationCount = 0;
}

/**
 *	@brief	Destructor.
 */
BeColumnListViewAdapter::~BeColumnListViewAdapter()
{
}

/**
 *	@brief	Initializes the object.
 *	@param[in]	view	target BColumnListView.
 */
void BeColumnListViewAdapter::Init(BColumnListView* columnListView)
{
	BeViewAdapter::Init(columnListView);
	
	columnCount = columnListView->CountColumns();
}

/**
 *	@brief	gets target BColumnListView object.
 */
BColumnListView* BeColumnListViewAdapter::getColumnListView()
{
	return static_cast<BColumnListView*>(view);
}

/**
 *	@brief	Returns whether the control is enabled.
 *	@return true if enabled, otherwise disabled.
 */
bool BeColumnListViewAdapter::IsEnabled()
{
	// currently this function is not implemented.
	return true;
}

/**
 *	@brief	Makes the control enabled or disabled.
 *	@param[in]	doEnable	true to enable the control, or false to disable the control.
 */
void BeColumnListViewAdapter::Enable(bool /* doEnable */)
{
	// currently this function is not implemented.
}

/**
 *	@brief	Returns item count in the control.
 *	@return	item count.
 */
SInt32 BeColumnListViewAdapter::GetCount()
{
	BColumnListView* listView = getColumnListView();
	return listView->CountRows();
}

/**
 * @brief	BRow with an item data.
 */
class BeDataRow : public BRow
{
public:
						BeDataRow(float height = 16.0) : BRow(height), data(NULL) { }

	void				SetItemData(void* data) { this->data = data; }
	void*				GetItemData() const { return this->data; }

private:
	void*				data;		///< item data
};

/**
 *	@brief	Adds one item to the control.
 *	@param[in]	texts	item texts for each column.
 *	@param[in]	data	associated data of adding item.
 *	@return	the index of added item.
 */
SInt32 BeColumnListViewAdapter::AddItem(ConstAStr* texts, void* data)
{
	BColumnListView* listView = getColumnListView();

	BeDataRow* row = new BeDataRow();
	SInt32 ix;
	for (ix = 0; ix < columnCount; ++ix)
	{
		row->SetField(new BStringField(texts[ix]), ix);
	}
	row->SetItemData(data);
	listView->AddRow(row);
	
	return listView->CountRows() - 1;
}

/**
 *	@brief	Removes one item from the control.
 *	@param[in]	index	item index.
 */
void BeColumnListViewAdapter::RemoveItem(SInt32 index)
{
	BColumnListView* listView = getColumnListView();
	BRow* row = listView->RowAt(index);
	listView->RemoveRow(row);
	delete row;
}

/**
 *	@brief	Removes all item from the control.
 */
void BeColumnListViewAdapter::RemoveAllItem()
{
	BColumnListView* listView = getColumnListView();
	listView->Clear();
}

/**
 *	@brief	Gets a string at specified index from the control.
 *	@param[in]	rowIndex		row index
 *	@param[in]	columnIndex		column index
 *	@param[out]	text	a string value is returned.
 */
void BeColumnListViewAdapter::GetItemText(SInt32 rowIndex, SInt32 columnIndex, MBCString& text)
{
	BColumnListView* listView = getColumnListView();
	BRow* row = listView->RowAt(rowIndex);
	BStringField* field = dynamic_cast<BStringField*>(row->GetField(columnIndex));
	if (NULL != field)
	{
		text = field->String();
	}
}

/**
 *	@brief	Returns associated data of specified index.
 *	@param[in]	rowIndex	item index.
 *	@return	associated data.
 */
void* BeColumnListViewAdapter::GetItemData(SInt32 rowIndex)
{
	BColumnListView* listView = getColumnListView();
	BeDataRow* row = dynamic_cast<BeDataRow*>(listView->RowAt(rowIndex));
	if (NULL != row)
	{
		return row->GetItemData();
	}
	else
	{
		return NULL;
	}
}

/**
 *	@brief	Sets associated data of specified index.
 *	@param[in]	rowIndex	item index.
 *	@param[in]	data	associated data.
 */
void BeColumnListViewAdapter::SetItemData(SInt32 rowIndex, void* data)
{
	BColumnListView* listView = getColumnListView();
	BeDataRow* row = dynamic_cast<BeDataRow*>(listView->RowAt(rowIndex));
	if (NULL != row)
	{
		return row->SetItemData(data);
	}
}

/**
 *	@brief	Returns an index of selected item.
 *	@return	item index of selected item. or -1 if no item is selected.
 */
SInt32 BeColumnListViewAdapter::GetSelectedItem()
{
	BColumnListView* listView = getColumnListView();
	BRow* row = listView->CurrentSelection();
	return listView->IndexOf(row);
}

/**
 *	@brief	Makes specified item selected.
 *	@param[in]	rowIndex	item index. if this is -1, no item is selected.
 */
void BeColumnListViewAdapter::SetSelectedItem(SInt32 rowIndex)
{
	BColumnListView* listView = getColumnListView();
	BRow* row = listView->RowAt(rowIndex);
	listView->AddToSelection(row);
}

/**
 *	@brief	Stops or restarts selection changed notification.
 *	@param[in]	doStop	true to stop, false to restart.
 */
void BeColumnListViewAdapter::StopSelectionChangedNotification(bool doStop)
{
	BColumnListView* listView = getColumnListView();
	if (doStop)
	{
		if (0 == stoppedSelectionChangedNotificationCount)
		{
			BMessage* message = listView->SelectionMessage();
			if (NULL != message)
			{
				stoppedSelectionChangedCommand = message->what;
				message->what = ID_NULL;
			}
		}
		stoppedSelectionChangedNotificationCount++;
	}
	else
	{
		stoppedSelectionChangedNotificationCount--;
		if (0 == stoppedSelectionChangedNotificationCount)
		{
			BMessage* message = listView->SelectionMessage();
			if (NULL != message)
			{
				message->what = stoppedSelectionChangedCommand;
				stoppedSelectionChangedCommand = ID_NULL;
			}
		}
	}
}
