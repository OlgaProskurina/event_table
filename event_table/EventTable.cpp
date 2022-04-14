#include "EventTable.h"
#include <algorithm>
#include <numeric>
#include "EventTable.h"

EventTable::EventTable() : m_tableSize(0), m_jobs(), m_availableEvents(), m_paths(), m_pathsLenght(), m_crucialPathLenght(0)
{}

EventTable::~EventTable()
{
	for (int i = 0; i < m_availableEvents.size(); i++)
	{
		delete m_availableEvents[i];
 	}
	for (int i = 0; i < m_tableSize; i++)
	{
		delete m_jobs[i];
	}	
}


const std::vector<Job*>& EventTable::getJobsTable() const { return m_jobs; }


std::vector<Job*>::iterator EventTable::searchAmongStartEvents(std::vector<Job*>::iterator from,
	std::vector<Job*>::iterator to, Event* t_event)
{
	auto iter = std::find_if(from, to,
		[&](Job* tmp) {
			return (tmp->getStartEvent() == t_event);
		});
	return iter;
}


std::vector<Job*>::iterator EventTable::searchAmongEndEvents(std::vector<Job*>::iterator from,
	std::vector<Job*>::iterator to,  Event* t_event)
{
	auto iter = std::find_if(from, to,
		[&](Job* tmp) {
			return (tmp->getEndEvent() == t_event);
		});
	return iter;
}


Event* EventTable::findInitialEvents()
{
	std::vector<Event*> initialList;
	for (const auto& job : m_jobs)
	{
		auto event_ = job->getStartEvent();
		auto iter = searchAmongEndEvents(m_jobs.begin(), m_jobs.end(), event_);
		if (iter == m_jobs.end())
		{
			initialList.push_back(event_);
		}
	}
	std::sort(initialList.begin(), initialList.end());
	initialList.erase(std::unique(initialList.begin(), initialList.end()), initialList.end());
	if (initialList.size() > 1)
	{
		return createFakeInitialEvent(initialList);
	}
	else
	{
		return initialList[0];
	}
}


Event* EventTable::findFinalEvents()
{
	std::vector<Event*> finalList;
	for (const auto& job : m_jobs)
	{
		auto event_ = job->getEndEvent();
		auto iter = searchAmongStartEvents(m_jobs.begin(), m_jobs.end(), event_);
		if (iter == m_jobs.end())
		{
			finalList.push_back(event_);
		}
	}
	std::sort(finalList.begin(), finalList.end());
	finalList.erase(std::unique(finalList.begin(), finalList.end()), finalList.end());
	if (finalList.size() > 1)
	{
		return createFakeFinalEvent(finalList);
	}
	else
	{
		return finalList[0];
	}
}


Event* EventTable::createFakeInitialEvent(std::vector<Event*>& initialList)
{
	Event* newInitialEvent = new Event(rand() % 101 + (-50));
	for (auto const& event_ : initialList)
	{
		addNewJob(newInitialEvent, event_, 0);
	}
	return newInitialEvent;
}


Event* EventTable::createFakeFinalEvent(std::vector<Event*>& finalList)
{
	Event* newFinalEvent = new Event(rand() % 101 + (-50));

	for (auto const& event_ : finalList)
	{
		addNewJob(event_, newFinalEvent, 0);
	}
	return newFinalEvent;
}


void EventTable::findFullPath()
{
	std::vector<std::vector<Event*>> paths;
	std::vector<std::vector<int>> pathsLenght;
	std::vector<Event*> pathTmp;
	std::vector<int> lenghtTmp;
	pathTmp.push_back(m_jobs[0]->getStartEvent());
	lenghtTmp.push_back(m_jobs[0]->getJobDuration());
	pathsLenght.push_back(lenghtTmp);
	paths.push_back(pathTmp);
	for (int i = 0; i < m_tableSize; i++)
	{
		for (int j = 0; j < paths.size(); j++)
		{
			if (paths[j][paths.size() - 1] == m_jobs[i]->getStartEvent())
			{
				paths[j].push_back(m_jobs[i]->getEndEvent());
				pathsLenght[j].push_back(m_jobs[i]->getJobDuration());
			}
			else if (paths[j][paths.size() - 2] == m_jobs[i]->getStartEvent())
			{
				pathTmp = paths[j];
				lenghtTmp = pathsLenght[j];
				pathTmp.pop_back();
				lenghtTmp.pop_back();
				pathTmp.push_back(m_jobs[i]->getEndEvent());
				lenghtTmp.push_back(m_jobs[i]->getJobDuration());
				bool isNew(true);
				for (const auto& path : paths)
				{
					if (path == pathTmp)
					{
						isNew = false;
					}
				}
				if (isNew)
				{
					j++;
					paths.insert(paths.cbegin() + j, pathTmp);
					pathsLenght.insert(pathsLenght.cbegin() + j, lenghtTmp);
				}
			}
		}
	}
	m_pathsLenght = pathsLenght;
	m_paths = paths;
}


void EventTable::findEarlyTerms()
{
	int maxEarlyTerm(0);
	int crucialPathLenght(0);

	for (int i = 0; i < m_availableEvents.size(); i++)
	{
		for (int j = 0; j < m_paths.size(); j++)
		{
			auto iter = std::find_if(m_paths[j].begin(), m_paths[j].end(),
				[&](const Event* tmp) {
					return (tmp == m_availableEvents[i]);
				}
			);

			int indexLenght = std::distance(m_paths[j].begin(), iter);

			if (iter != m_paths[j].end())
			{
				int sumEarlyTerm =
					std::accumulate(m_pathsLenght[j].begin(), m_pathsLenght[j].begin() + indexLenght + 1, 0);
				maxEarlyTerm = (std::max)(maxEarlyTerm, sumEarlyTerm);
			}
		}
		crucialPathLenght = (std::max)(maxEarlyTerm, crucialPathLenght);
		m_availableEvents[i]->changeEarlyTerm(maxEarlyTerm);
		maxEarlyTerm = 0;
	}
	m_crucialPathLenght = crucialPathLenght;
}


void EventTable::sortTable()
{
	Event* tmp(findInitialEvents());
	findFinalEvents();
	std::iter_swap(m_jobs.begin(), searchAmongStartEvents(m_jobs.begin(), m_jobs.end(), tmp));
	for (int i = 1; i < m_tableSize; i++)
	{
		auto iter = searchAmongStartEvents(m_jobs.begin() + i, m_jobs.end(), tmp);
		if (iter != m_jobs.end())
		{
			std::iter_swap(m_jobs.begin() + i, iter);
		}
		else
		{
			tmp = m_jobs[m_tableSize - i]->getEndEvent();
		}
	}
	findFullPath();
	findEarlyTerms();
}


void EventTable::deleteJob(int jobIndex)
{
	delete m_jobs[jobIndex];
	m_jobs.erase(m_jobs.begin() + jobIndex);
	sortTable();
}


void EventTable::changeJobDuration(int newDuration, int jobIndex)
{
	try {
		m_jobs[jobIndex]->changeDuration(newDuration);
		sortTable();
	}
	catch (const std::exception&) {
		throw;
	}
}


int EventTable::getCrucialPathLenght() const { return m_crucialPathLenght; }


const std::vector<Event*>& EventTable::getAvailableEvents() const { return m_availableEvents; }


void EventTable::addNewJob(Event* startEvent, Event* endEvent, int jobDuration)
{
	try {
		Job* newJob = new Job(startEvent, endEvent, jobDuration);
		m_jobs.push_back(newJob);
		m_tableSize = m_jobs.size();
		sortTable();
	}
	catch (const std::exception&) {
		throw;
	}
}

void EventTable::addNewEvent(int event_)
{
	Event* newEvent = new Event(event_);
	m_availableEvents.push_back(newEvent);
}

