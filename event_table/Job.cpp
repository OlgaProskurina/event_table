#include "Job.h"

Job::Job(Event* startEvent, Event* endEvent, int duration) :
	m_startEvent(startEvent), m_endEvent(endEvent), m_JobDuration(duration)
{
	if (startEvent == endEvent) {
		throw std::exception("��������� � �������� ������� ���������.");
	}
	if (duration < 0)
	{
		throw std::exception("����������������� ������ �� ����� ���� �������������.");
	}

}



void Job::changeDuration(int newDuration)
{
	if (newDuration < 0)
	{
		throw std::exception("����������������� ������ �� ����� ���� �������������.");
	}
	m_JobDuration = newDuration;
}


int Job::getJobDuration() const { return m_JobDuration; }


Event* Job::getStartEvent() const { return m_startEvent; }


Event* Job::getEndEvent() const { return m_endEvent; }


bool operator==(const Job& a, const Job& b)
{
	return a.getStartEvent() == b.getStartEvent()
		&& a.getEndEvent() == b.getEndEvent();
}

