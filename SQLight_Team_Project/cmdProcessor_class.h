#pragma once
#include <iostream>
#include <string>
#include <regex>
#include "utils.h"
#include "column_class.h"
#include "table_class.h"
using namespace std;


struct properFormats {
	string properCt = "CREATE TABLE table_name [IF NOT EXISTS] ( (column_name1, type, size, default_value1), (column_name1, type, size, default_value1)... )";
	string properCi = "CREATE INDEX [IF NOT EXISTS] index_name ON table_name (column_name) - creates an index only on a single column";
	string properDt = "DROP TABLE table_name";
	string properDi = "DROP INDEX index_name";
	string properDplt = "DISPLAY TABLE table_name";
	string properIi = "INSERT INTO table VALUES(...); values are separated by , and they have the exact number and order as the table definition";
	string properDf = "DELETE FROM table_name WHERE column_name = value (deletes allows only one column in the where clause)";
	string properSt = "SELECT (at_least_one_column, ...) | ALL FROM table_name [WHERE column_name = value] - the where clause is optional";
	string properUe = "UPDATE table_name SET column_name = value WHERE  column_name = value (the SET column may be different than the WHERE one)";
};
// se pun aici regex-urile pt ca sunt foarte lungi si nu se intelege
// nu o sa arate formatul potrivit acum chiar daca scrii doar DIsplay TABLE de ex pt ca regex-urile
// urmatoare nu au nimic scris
struct regexList {
	string fullCreateTable = "^\\s*CREATE\\s+TABLE\\s+([A-Za-z][A-Za-z0-9]+)\\s*(IF\\s+NOT\\s+EXISTS)?\\s+\\(\\s*((?:\\(\\s*[A-Za-z][A-Za-z0-9]+\\s*,\\s*[A-Za-z]+\\s*,\\s*[0-9]+\\s*,\\s*[A-Za-z0-9\"'’]+\\s*\\)\\s*,?\\s*)+?)\\s*\\)$";
	string fullCreateIndex = "^\\s*CREATE\\s+INDEX\\s*(IF\\s+NOT\\s+EXISTS)?\\s+([a-zA-Z0-9]+)\\s+ON\\s+([a-zA-Z0-9]+)\\s+\\((\\s*[a-zA-Z0-9]+)\\s*\\)$";
	string fullDropTable = "^\\s*DROP\\s+TABLE\\s+([a-zA-Z0-9]+)\\s*$";
	string fullDropIndex = "^\\s*DROP\\s+INDEX\\s+([a-zA-Z0-9]+)\\s*$";
	string fullDisplayTable = "^\\s*DISPLAY\\s+TABLE\\s+([a-zA-Z0-9]*)\\s*$";
	string fullInsertInto = "^\\s*INSERT\\s+INTO\\s+([a-zA-Z0-9]+)\\s+VALUES\\s*\\(((\\s*[a-zA-Z0-9\"'’”]+\\s*,?\\s*)+)\\)\\s*$";
	string fullDeleteFrom = "^\\s*DELETE\\s+FROM\\s+([a-zA-Z0-9]+)\\s+WHERE\\s+([a-zA-Z0-9]+)\\s+=\\s+([a-zA-Z0-9]+)\\s*$";
	string fullSelect = "^\\s*SELECT\\s*((\\((\\s*[a-zA-Z0-9]+\\s*,?\\s*)+\\))+|(ALL))\\s*FROM\\s+([a-zA-Z0-9]+)+\\s*(WHERE\\s+([a-zA-Z0-9]+)\\s*=\\s*([a-zA-Z0-9]+))?$";
	string fullUpdate = "^\\s*UPDATE\\s+([a-zA-Z0-9]+)\\s+SET\\s+([a-zA-Z0-9]+)\\s*=\\s*([a-zA-Z0-9]+)\\s+WHERE\\s+([a-zA-Z0-9]+)\\s*=\\s*([a-zA-Z0-9]+)\\s*$";
};
// pana aici
class CmdProcessor
{
private:
	string fullCmd = "";
public:
	bool checkCmd(TableBuffer& tableBuffer) {
		properFormats formats;
		regexList rlist;

		regex ct("^\\s*CREATE\\s+TABLE\\s*", regex::icase);
		regex ci("^\\s*CREATE\\s+INDEX\\s*", regex::icase);
		regex dt("^\\s*DROP\\s+TABLE\\s*", regex::icase);
		regex di("^\\s*DROP\\s+INDEX\\s*", regex::icase);
		regex dplt("^\\s*DISPLAY\\s+TABLE\\s*", regex::icase);
		regex ii("^\\s*INSERT\\s+INTO\\s*", regex::icase);
		regex df("^\\s*DELETE\\s+FROM\\s*", regex::icase);
		regex st("^\\s*SELECT\\s*", regex::icase);
		regex ue("^\\s*UPDATE\\s*", regex::icase);

		regex createTableRegex(rlist.fullCreateTable, regex::icase);
		regex createIndexRegex(rlist.fullCreateIndex, regex::icase);
		regex dropTableRegex(rlist.fullDropTable, regex::icase);
		regex dropIndexRegex(rlist.fullDropIndex, regex::icase);
		regex displayTableRegex(rlist.fullDisplayTable, regex::icase);
		regex insertIntoRegex(rlist.fullInsertInto, regex::icase);
		regex deleteFromRegex(rlist.fullDeleteFrom, regex::icase);
		regex selectRegex(rlist.fullSelect, regex::icase);
		regex updateRegex(rlist.fullUpdate, regex::icase);

		smatch matches;

		if (regex_search(this->fullCmd, ct)) {

			if (regex_search(this->fullCmd, matches, createTableRegex)) {
				tableBuffer = tableBuffer + this->createTable(matches);
				return 1;
			}
			else {
				cout << endl << "Proper format is:" << "\n" << formats.properCt;
				return 1;
			}
		}
		else if(regex_search(this->fullCmd, ci)){

			if (regex_search(this->fullCmd, matches, createIndexRegex)) {
				this->createIndex(matches);
				return 1;
			}
			else {
				cout << endl << "Proper format is:" << "\n" << formats.properCi;
				return 1;
			}
		}
		else if (regex_search(this->fullCmd, dt)) {

			if (regex_search(this->fullCmd, matches, dropTableRegex)) {
				this->dropTable(matches);
				return 1;
			}
			else {
				cout << endl << "Proper format is:" << "\n" << formats.properDt;;
				return 1;
			}
		}
		else if (regex_search(this->fullCmd, di)) {

			if (regex_search(this->fullCmd, matches, dropIndexRegex)) {
				this->dropIndex(matches);
				return 1;
			}
			else {
				cout << endl << "Proper format is:" << "\n" << formats.properDi;
				return 1;
			}
		}
		else if (regex_search(this->fullCmd, dplt)) {

			if (regex_search(this->fullCmd, matches, displayTableRegex)) {
				this->displayTable(matches,tableBuffer);
				return 1;
			}
			else {
				cout << endl << "Proper format is:" << "\n" << formats.properDplt;
				return 1;
			}
		}
		else if (regex_search(this->fullCmd, ii)) {

			if (regex_search(this->fullCmd, matches, insertIntoRegex)) {
				this->insertInto(matches, tableBuffer);
				return 1;
			}
			else {
				cout << endl << "Proper format is:" << "\n" << formats.properIi;
				return 1;
			}
		}
		else if (regex_search(this->fullCmd, df)) {

			if (regex_search(this->fullCmd, matches, deleteFromRegex)) {
				this->deleteFrom(matches);
				return 1;
			}
			else {
				cout << endl << "Proper format is:" << "\n" << formats.properDf;
				return 1;
			}
		}
		else if (regex_search(this->fullCmd, st)) {

			if (regex_search(this->fullCmd, matches, selectRegex)) {
				this->select(matches);
				return 1;
			}
			else {
				cout << endl << "Proper format is:" << "\n" << formats.properSt;
				return 1;
			}
		}
		else if (regex_search(this->fullCmd, ue)) {

			if (regex_search(this->fullCmd, matches, updateRegex)) {
				this->update(matches);
				return 1;
			}
			else {
				cout << endl << "Proper format is:" << "\n" << formats.properUe;
				return 1;
			}
		}


		return 0;
	}

	void setFullCmd() {
		getline(cin, this->fullCmd);

		if (this->fullCmd.length() <= 0) {
			this->fullCmd = "";

			throw exception("Error reading the input. Try again.");
		}
	}

	string getFullCmd() {
		return this->fullCmd;
	}

private:
	Table createTable(smatch matches) {
		regex partitionRegex("[^ ,()][a-zA-Z0-9\"'”’\\s*]*");
		smatch partitionMatches;



		// matches[0] = toata comanda
		// matches[1] = table name,
		// matches[2] = if not exists asta daca am scris daca nu e ""
		//  iar al patrulea input este doar ce este in paranteza (col_name, etc... )


		if (matches[2] != "") {
			// do smth cause IF NOT EXISTS was used
			cout << endl << "IF NOT EXISTS USED.";

		}
		


		string tableName = matches[1].str();
		string beforePartition = matches[3].str();

		auto words_begin =
			std::sregex_iterator(beforePartition.begin(), beforePartition.end(), partitionRegex);
		auto words_end = std::sregex_iterator();

		// de ce am impartit la 4 ?
		// pt ca in fiecare paranteza sunt cate 4 valori
		// noi vrem sa creeam cate o coloana pt fiecare paranteza iar fiecare coloana are atribuite cate 4 valori
		int noColumns = distance(words_begin, words_end) / 4;
		Column* columns = new Column[noColumns];

		int j = 0;
		int k = 0;
		// aici nu putem folosi i pentru ca e de tipul regex iterator nu int
		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			
			std::smatch match = *i;
			std::string match_str = match.str();
			switch (k) {
			case 0:
				columns[j].setColumnName(match_str);
				break;
			case 1:
				if (toLowerCase(match_str) == "integer")
					columns[j].setType(columnTypes::INTEGER);
				else if (toLowerCase(match_str) == "float")
					columns[j].setType(columnTypes::FLOAT);
				else if (toLowerCase(match_str) == "text")
					columns[j].setType(columnTypes::TEXT);
				else
					throw exception("Type of column must be integer, float or text.");
				break;
			case 2:
				// stoi() transforms strings in integers
				columns[j].setSize(stoi(match_str));
				break;
			case 3:
				columns[j].setDefaultValue(match_str);
				break;
			}

			// valorile din paranteze sunt stocate secvential asa ca iteram la rand.
			// asta inseamna ca o sa dam de column name din 4 in 4 numere: 0 4 8 etc..
			// j-ul este nr coloanei care creste dupa ce trecem prin toate informatiile din coloana
			// care sunt 4 la nr. numele tipul size si default
			// de asta resetam k la 0, k e ca si cazul la care ne aflam
			k += 1;
			if (k == 4) {
				j += 1;
				k = 0;
			}
		}
		// aici in viitor o sa avem o functie care sa salveze tabelul
		// si in fisier 
		Table t(tableName, columns, j);
		
		//Table* returnTable = new Table(tableName, columns, j);
		return t;
	}

	// trebuie lucrat de aici in jos
	void createIndex(smatch matches) {
		return;
	}

	void dropTable(smatch matches) {
		cout << matches.str();
		return;
	}

	void dropIndex(smatch matches) {
		return;
	}

	void displayTable(smatch matches, TableBuffer tableBuffer) {
		int size = tableBuffer.getNoTables();
		Table* tables = tableBuffer.getTables();
		string tableNameInput = matches[1].str();
		for (int i = 0; i < size; i++) {
			if (tableNameInput == tables[i].getName()) {
				cout << tables[i];
				delete[] tables;
				return;
			}
		}
		
		cout << endl << "There is no table with name: " << tableNameInput;
	
		delete[] tables;
	}

	void insertInto(smatch matches, TableBuffer tableBuffer) {
		// matches[1] =  table name
		// matches[2] = ce e in paranteza

		int size = tableBuffer.getNoTables();
		Table* tables = tableBuffer.getTables();
		string tableNameInput = matches[1].str();
		Table foundTable;
		int k;
		for (k = 0; k < size; k++) {
			if (tableNameInput == tables[k].getName()) {
				 foundTable = tables[k];
				break;
			}
			else {
				cout << endl << "There is no table with name: " << tableNameInput;
				throw exception("\nError: Table not found.");
			}
		}
		
		regex partitionRegex("[^ ,][a-zA-Z0-9\"'”’\\s*]*");

		smatch input;
		string paranteza = matches[2].str();

		auto words_begin = sregex_iterator(paranteza.begin(), paranteza.end(), partitionRegex);
		auto words_end = sregex_iterator();

		if (foundTable.getNoColumns() != distance(words_begin, words_end)) {
			throw exception("\nError: You should insert as many values as the number of columns in that specific table.");
		}

		string data[100];
		int j = 0;
		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			smatch match = *i;
			string match_str = match.str();
			cout << endl << match_str;
			data[j] = match_str;
			j += 1;
		}

		//tableBuffer.insertRowByName(data, tableNameInput);
		foundTable.insertRow(data);
		delete[] tables;
	}

	void deleteFrom(smatch matches) {
		return;
	}

	void select(smatch matches) {
		return;
	}

	void update(smatch matches) {
		return;
	}
};