#include <iostream>
#include <regex>
#include <string>
#include <stdlib.h>
#include <fstream>
#include "utils.h"
#include "cmdProcessor_class.h"
#include "table_class.h"
#include "Document.h"
#include "RgxManager.h"
using namespace std;


int main() {
	CmdProcessor buffer;
	properFormats formats;
	TableBuffer tableBuffer;
	properFormats pf;

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
					for (int i = 0; i < pf.noFormats; i++) {
						cout << "\n" << pf.formats[i];
					}
					
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
