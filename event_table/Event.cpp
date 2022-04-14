#include "Event.h"

Event::Event(int event) : m_event(event), m_earlyTerm(0) {}


void Event::changeEarlyTerm(int newEarlyTerm)
{
	if (newEarlyTerm < 0)
	{
		throw std::exception("Ранний срок свершения события не может быть отрицательным.");
	}
	m_earlyTerm = newEarlyTerm;
}


int Event::getEvent() const { return m_event; }


int Event::getEarlyTerm() const { return m_earlyTerm; }


bool operator==(const Event& a, const Event& b)
{
	return a.m_event == b.m_event;
}


std::ostream& operator<<(std::ostream& out, const Event& event)
{
	out << event.m_event;
	return out;
}

bool operator!=(const Event& a, const Event& b)
{
	return a.m_event != b.m_event;
}

