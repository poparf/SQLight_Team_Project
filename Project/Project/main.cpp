#include <iostream>
#include <string>
#include <regex>
#include "utils.h"
#include "column_class.h"
#include "cmdProcessor_class.h"
#include "table_class.h"
#include <stdlib.h>
/*
*CREATE TABLE, CREATE INDEX, DROP TABLE, DROP INDEX, DISPLAY TABLE
*Accepted CRUD commands for managing data are INSERT, SELECT, UPDATE, DELETE
*/
using namespace std;

#define NO_SECONDARY_COMANDS 3

int checkSecondaryCommand(string fullCmd) {
	string additionalCommands[NO_SECONDARY_COMANDS] = { "/quit", "/clear", "/help" };

	for (int i = 0; i < NO_SECONDARY_COMANDS; i++) {
		if (fullCmd == additionalCommands[i]) {
			return i + 1;
		}
	}
}


int main() {
	CmdProcessor buffer;

	while (1) {
		printLine();

		try {
			buffer.setFullCmd();
		}
		catch (const exception& e) {
			cout << e.what() << endl;
			continue; // Reset the loop
		}
		

		try {
			
			switch (checkSecondaryCommand(buffer.getFullCmd()))
			{
				case 1:
					exit(EXIT_SUCCESS);
					break;
				case 2:
					system("cls");
					break;
				case 3:
					cout << endl << "Available commands at this point:";
					cout << "\n" << "/quit";
					cout << "\n" << "/clear";
					cout << "\n" << "1.CREATE TABLE table_name [IF NOT EXISTS] ( (column_name1, type, size, default_value1), (column_name1, type, size, default_value1)... )";
					cout << "\n" << "* Type must be 'string' or 'number'.";
					break;
				default:
					if (buffer.checkCmd()) {

					}
					else {
						cout << "Command not recognized: " << buffer.getFullCmd();
					}
					break;
			}
		}
		catch (const exception& e) {
			cout << e.what() << endl;
			continue; // Reset the loop
		}
		
	}
}
