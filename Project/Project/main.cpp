#include <iostream>
#include <regex>
#include <string>
#include <stdlib.h>
#include <fstream>
#include "utils.h"
#include "cmdProcessor_class.h"
#include "TableBuffer.h"
#include "Files.h"
#include "RgxManager.h"
using namespace std;


int main(int argc, char* argv[]) {
	CmdProcessor buffer;
	properFormats formats;
	TableBuffer tableBuffer;
	properFormats pf;

	// Loading files using text files from the cmdl
	while (--argc > 0) {
		cout << endl << argv[argc] << endl;
		buffer.insertCommands(string(argv[argc]), tableBuffer);
		
	}



	try {
		buffer.insertCommands("comenzi.txt", tableBuffer);
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
					cout << "\n" << "/activate xml";
					cout << "\n" << "/activate csv";
					cout << "\n" << "/deactivate xml";
					cout << "\n" << "/deactivate csv";
					cout << "\n" << "/quit";
					cout << "\n" << "/clear";
					for (int i = 0; i < pf.noFormats; i++) {
						cout << "\n" << pf.formats[i];
					}
					
					break;
				case 4:
					// csv
					cout << buffer.activateCSV();
					break;
				case 5:
					// xml
					cout << buffer.activateXML();
					break;
				case 6:
					// deactivate xml
					cout << buffer.deactivateXML();
					break;
				case 7:
					// deactivate csv
					cout << buffer.deactivateCSV();
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
