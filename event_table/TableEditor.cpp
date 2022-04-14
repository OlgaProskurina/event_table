#include "TableEditor.h"
#include <algorithm>
#include <Windows.h>


TableEditor::TableEditor() : m_table(new EventTable()) {}


TableEditor::~TableEditor()
{
	delete m_table;
}

void TableEditor::drawGanttDiagram()
{
	auto jobs = m_table->getJobsTable();
	std::cout << "Диаграмма Ганта" << std::endl;
	std::cout << "\t\t   ";
	for (int i = 0; i < m_table->getCrucialPathLenght(); i++) { std::cout << i << " "; }

	std::cout << std::endl;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < jobs.size(); i++)
	{
		std::cout << "(" << *jobs[i]->getStartEvent() << " , " << *jobs[i]->getEndEvent() << ")" << "\t|";

		int amount = jobs[i]->getStartEvent()->getEarlyTerm();

		SetConsoleTextAttribute(console, (WORD)((15 << 4) | 15));
		for (int k = 0; k < amount; k++) {
			std::cout << k << " ";
		}

		if (jobs[i]->getStartEvent() != jobs[0]->getStartEvent()) { std::cout << ++amount << " "; }

		for (int k = 0; k < jobs[i]->getJobDuration(); k++)
		{
			SetConsoleTextAttribute(console, (WORD)((2 << 4) | 2));
			std::cout << amount + k << " ";
		}

		if (jobs[i]->getStartEvent() == jobs[0]->getStartEvent()) { std::cout << jobs[i]->getJobDuration() << " "; }

		SetConsoleTextAttribute(console, (WORD)((15 << 4) | 0));
		std::cout << std::endl;
		std::cout << "\t\t|" << std::endl;
	}
}


void TableEditor::changeJobDuration()
{
	int jobIndex(0), newDuration(0);
	std::cout << "Введите номер работы:"; std::cin >> jobIndex;
	jobIndex--;
	std::cout << "Введите продолжительность:"; std::cin >> newDuration;
	try
	{
		m_table->changeJobDuration(newDuration, jobIndex);
	}
	catch (const std::exception& error)
	{
		std::cout << error.what() << std::endl;
	}

}


void TableEditor::deleteJob()
{
	int jobIndex(0);
	std::cout << "Введите номер работы:"; std::cin >> jobIndex;
	jobIndex--;
	m_table->deleteJob(jobIndex);
}


void TableEditor::displayTable()
{
	std::cout << "Таблица работ:" << std::endl;
	std::cout << "№|\t"<<"i\t"<<"j\t"<<"t\t" << std::endl;
	const auto& jobs = m_table->getJobsTable();
	for (int i = 0; i < jobs.size(); i++)
	{
		std::cout << i + 1 << " |\t" << *jobs[i]->getStartEvent() << "\t" << *jobs[i]->getEndEvent() << "\t " <<
			jobs[i]->getJobDuration() << "\t" << std::endl;
	}
}


void TableEditor::createJob()
{
	int startEventIndex(0), endEventIndex(0), jobDuration(0);
	displayAvailableEvents();
	std::cout << "Введите номер начального события:"; std::cin >> startEventIndex;
	std::cout << "Введите номер кнечного события:"; std::cin >> endEventIndex;
	std::cout << "Введите продолжительность работы:"; std::cin >> jobDuration;
	startEventIndex--;
	endEventIndex--;
	try
	{
		m_table->addNewJob(m_table->getAvailableEvents()[startEventIndex],
			m_table->getAvailableEvents()[endEventIndex], jobDuration);
	}
	catch (const std::exception& error)
	{
		std::cout << error.what() << std::endl;
	}

}


void TableEditor::createEvent()
{
	int event_;
	std::cout << "Введите событие: "; std::cin >> event_;
	m_table->addNewEvent(event_);

}


void TableEditor::editTable()
{
	char choice;
	do
	{
		std::cout << std::endl << "1. Удалить работу" << std::endl;
		std::cout << "2. Изменить продолжительность работы" << std::endl;
		std::cout << "3. Добавить новую работу" << std::endl;
		std::cout << "4. Добавить событие" << std::endl;
		std::cout << "5. Вывести таблицу работ." << std::endl;
		std::cout << "6. Вывести диграмму Ганта." << std::endl;
		std::cout << "7. Выйти из меню редактирования" << std::endl;
		std::cout << std::endl << ">> ";
		std::cin >> choice;
		switch (choice)
		{
		case'1':
			deleteJob(); break;
		case'2':
			changeJobDuration(); break;
		case'3':
			createJob(); break;
		case'4':
			createEvent(); break; 
		case'5':
			displayTable(); break; 
		case'6':
			drawGanttDiagram(); break;
		default:
			break;
		}

	} while (choice != '7');

}

void TableEditor::displayAvailableEvents()
{
	std::cout << "Доступные события:" << std::endl;
	for (int i = 0; i < m_table->getAvailableEvents().size(); i++)
	{
		std::cout << i + 1 << " |\t" << m_table->getAvailableEvents()[i]->getEvent() << std::endl;
	}
}





