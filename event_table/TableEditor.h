#pragma once
#include "EventTable.h"

class TableEditor
{
private:
	EventTable* m_table;

	void displayAvailableEvents();
	void createEvent();
	void createJob();
	void displayTable();
	void drawGanttDiagram();
	void changeJobDuration();
	void deleteJob();
public:
	TableEditor();
	~TableEditor();
	void editTable();

};

