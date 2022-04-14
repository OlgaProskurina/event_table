#pragma once
#include <vector>
#include "Job.h"

class EventTable
{
private:
	int m_tableSize;
	std::vector<Job*> m_jobs;
	std::vector<Event*> m_availableEvents;
	std::vector<std::vector<Event*>> m_paths;
	std::vector<std::vector<int>> m_pathsLenght;
	int m_crucialPathLenght;

	Event* findInitialEvents();
	Event* findFinalEvents();
	Event* createFakeInitialEvent(std::vector<Event*>& initialList);
	Event* createFakeFinalEvent(std::vector<Event*>& finalList);
	void sortTable();
	void findFullPath();
	void findEarlyTerms();
	std::vector<Job*>::iterator searchAmongStartEvents(std::vector<Job*>::iterator from,
		std::vector<Job*>::iterator to,  Event* t_event);
	std::vector<Job*>::iterator searchAmongEndEvents(std::vector<Job*>::iterator from,
		std::vector<Job*>::iterator to,  Event* t_event);
public:
	EventTable();

	~EventTable();

	void addNewJob(Event* startEvent, Event* endEvent, int jobDuration);

	void addNewEvent(int event_);

	const std::vector<Job*>& getJobsTable() const;

	void deleteJob(int jobIndex);

	void changeJobDuration(int newDuration, int jobIndex);

	int getCrucialPathLenght() const;

	const std::vector<Event*>& getAvailableEvents() const;

};

