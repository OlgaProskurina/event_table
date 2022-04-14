#pragma once
#include <iostream>

class Event
{
private:
	int m_event;
	unsigned int m_earlyTerm;
public:
	Event(int event);
	void changeEarlyTerm(int newEarlyTerm);
	int getEvent() const;
	int getEarlyTerm() const;
	friend bool operator==(const Event& a, const Event& b);
	friend std::ostream& operator<<(std::ostream& out, const Event& event);
	friend bool operator!=(const Event& a, const Event& b);
};

