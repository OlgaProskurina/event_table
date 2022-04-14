#pragma once
#include "Event.h"

class Job
{
private:
	Event* m_startEvent;
	Event* m_endEvent;
	int m_JobDuration;
public:
	Job(Event* startEvent, Event* endEvent, int duration);
	void changeDuration(int newDuration);
	int getJobDuration() const;
	Event* getStartEvent() const;
	Event* getEndEvent() const;
	friend bool operator==(const Job& a, const Job& b);
};

