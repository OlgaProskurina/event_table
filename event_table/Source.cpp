#include "TableEditor.h"

int main()
{
	setlocale(LC_ALL, "ru");
	TableEditor* editor = new TableEditor();
	editor->editTable();
	delete editor;
}