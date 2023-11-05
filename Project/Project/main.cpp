#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "utils.h"
#include "column_class.h"
#include "cmdProcessor_class.h"
/*
*CREATE TABLE, CREATE INDEX, DROP TABLE, DROP INDEX, DISPLAY TABLE
*Accepted CRUD commands for managing data are INSERT, SELECT, UPDATE, DELETE
*/
using namespace std;


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
			if (buffer.getFullCmd() == "/quit") {
				exit(EXIT_SUCCESS);
			}
			else if (buffer.checkCmd()) {
				// now check for validity of command
			}
			else {
				cout << "Command not recognized: " << buffer.getFullCmd();
			}
		}
		catch (const exception& e) {
			cout << e.what() << endl;
			continue; // Reset the loop
		}
		
	}
}
