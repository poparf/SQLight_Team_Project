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
Mai trebuie facut doar select ul folosind index

*/
string Statement::input;

int main(int argc, char* argv[]) {
	cout << endl << "/help for list of commands." << endl;
	cout << "You can also insert a list of commands by opening the program in the console and specifying a text file with commands separated by new lines." << endl;
	cout << "Also you got provided with comenzi.txt and comenzi2.txt to test the program with.";

	TableBuffer* tableBuffer = TableBuffer::getInstance();

	CreateTable createTable;
	CreateIndex createIndex;
	InsertRow insertRow;
	Import importCmd;
	SelectValues selectValues;
	DisplayTable displayTable;
	UpdateTable updateTable;
	DropTable dropTable;
	DropIndex dropIndex;
	DeleteFrom deleteFrom;

	PrimaryCmd** primaryCmds = new PrimaryCmd*[PrimaryCmd::counter];
	primaryCmds[0] = &createTable;
	primaryCmds[1] = &createIndex;
	primaryCmds[2] = &insertRow;
	primaryCmds[3] = &importCmd;
	primaryCmds[4] = &selectValues;
	primaryCmds[5] = &updateTable;
	primaryCmds[6] = &dropTable;
	primaryCmds[7] = &dropIndex;
	primaryCmds[8] = &deleteFrom;
	primaryCmds[9] = &displayTable;

	QuitCmd quit;
	ClearConsoleCmd clear;
	HelpCmd help;
	CSV csv;
	XML xml;

	SecondaryCmd** secondaryCmds = new SecondaryCmd * [SecondaryCmd::counter];
	secondaryCmds[0] = &quit;
	secondaryCmds[1] = &clear;
	secondaryCmds[2] = &help;
	secondaryCmds[3] = &csv;
	secondaryCmds[4] = &xml;

	while (--argc > 0) {
		string fileName = string(argv[argc]);
		ifstream comenzi(fileName);
		string comanda;
		try {
			if (comenzi.is_open()) {

				while (getline(comenzi, comanda)) {

					try {
						Statement::setInput(comanda);

						if (Statement::getInput()[0] == '/')
						{
							for (int i = 0; i < SecondaryCmd::counter; i++) {
								if (secondaryCmds[i] != nullptr) {
									if (secondaryCmds[i]->check()) {
										secondaryCmds[i]->process();
										continue;
									}
								}
							}
						}
						else
						{
							for (int i = 0; i < PrimaryCmd::counter; i++) {
								if (primaryCmds[i] != nullptr) {
									if (primaryCmds[i]->check()) {
										primaryCmds[i]->process();
										continue;
									}
								}
							}
						}
					}
					catch (const exception& e) {
						cout << e.what() << endl;
						continue;
					};

				}

				comenzi.close();
			}
		}
		catch (exception& e) {
			cout << endl << e.what();
		}
	}


	while (1) {
		cout << endl << "SQLite>";

		try {
			Statement::setInput();

			if (Statement::getInput()[0] == '/')
			{
				for (int i = 0; i < SecondaryCmd::counter; i++) {
					if (secondaryCmds[i] != nullptr) {
						if (secondaryCmds[i]->check()) {
							secondaryCmds[i]->process();
							continue;
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < PrimaryCmd::counter; i++) {
					if (primaryCmds[i] != nullptr) {
						if (primaryCmds[i]->check()) {
							primaryCmds[i]->process();
							continue;
						}
					}
				}
			}
		}
		catch (const exception& e) {
			cout << e.what() << endl;
			continue;
		};
	};
};
