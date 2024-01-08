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


/*
Mai trebuie facut create index si drop index
- se salveaza offsetul pt fiecare valoare de la coloana ceruta ca si valoare int. pt fiecare row salvam un int
- le salvam intr-un alt fisier binar si le citim atunci cand citim si tabelul in buffer
- la select dam check daca coloana din select e si n index daca da ne folosim de index sa o afisam
- la insert adaugam noul row la index

Refactoring la procesorul de comenzi
sa folosesc upcasting la comenzi pentru a le avea pe toate intr-un singur array


*/


int main(int argc, char* argv[]) {
	CmdProcessor buffer;
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
	


	cout << endl << "/help for list of commands." << endl;
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
