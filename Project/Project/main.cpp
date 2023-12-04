#include <iostream>
#include <string>
#include <stdlib.h>
#include "utils.h"
#include "cmdProcessor_class.h"
#include "table_class.h"
#include <fstream>
#include "Document.h"
using namespace std;

int main() {
	CmdProcessor buffer;
	properFormats formats;
	TableBuffer tableBuffer;
	try {
		buffer.insertCommands(tableBuffer);
	}
	catch (exception& e) {
		cout << endl << e.what();
	}
	

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
					cout << "\n" << formats.properCt;
					cout << "\n" << formats.properCi;
					cout << "\n" << formats.properDf;
					cout << "\n" << formats.properDi;
					cout << "\n" << formats.properDplt;
					cout << "\n" << formats.properDt;
					cout << "\n" << formats.properIi;
					cout << "\n" << formats.properSt;
					cout << "\n" << formats.properUe;
					break;
				default:
					if (!buffer.checkCmd(tableBuffer)) {
						cout << endl << "Command not recognized: " << buffer.getFullCmd();
					}
					
					break;
			}
		}
		catch (const exception& e) {
			cout << e.what() << endl;
			continue;
		}
	}
}
