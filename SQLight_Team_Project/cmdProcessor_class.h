#pragma once
#include <regex>
#include <functional> 
#include "utils.h"
#include "column_class.h"
#include "table_class.h"
#include "Document.h"
#include "RgxManager.h"


#define regexFilePath "regexList.txt"
#define partialRegexFilePath "partialRegexList.txt"

//typedef void (*fun_ptr_t)(smatch, TableBuffer);
//typedef void (CmdProcessor::* mem_fun_ptr_t)(std::smatch, TableBuffer&);
using namespace std;


class properFormats {
public:
	string* formats = nullptr;
	int noFormats = 9;

	properFormats() {
		formats = new string[this->noFormats];
		// may use a map in the future..
		formats[0] = "CREATE TABLE table_name [IF NOT EXISTS] ( (column_name1, type, size, default_value1), (column_name1, type, size, default_value1)... )";
		formats[1] = "CREATE INDEX [IF NOT EXISTS] index_name ON table_name (column_name) - creates an index only on a single column";
		formats[2] = "DROP TABLE table_name";
		formats[3] = "DROP INDEX index_name";
		formats[4] = "DISPLAY TABLE table_name";
		formats[5] = "INSERT INTO table VALUES(...); values are separated by , and they have the exact number and order as the table definition";
		formats[6] = "DELETE FROM table_name WHERE column_name = value (deletes allows only one column in the where clause)";
		formats[7] = "SELECT (at_least_one_column, ...) | ALL FROM table_name [WHERE column_name = value] - the where clause is optional";
		formats[8] = "UPDATE table_name SET column_name = value WHERE  column_name = value (the SET column may be different than the WHERE one)";
	}


	~properFormats() {
		delete[] formats;
	}
};
//
////struct partialRegexList {
////	string partialCreateTable = "^\\s*CREATE\\s+TABLE\\s*";
////	string partialCreateIndex = "^\\s*CREATE\\s+INDEX\\s*";
////};
//
//struct regexList {
//	string fullCreateTable = "^\\s*CREATE\\s+TABLE\\s+([A-Za-z][A-Za-z0-9]+)\\s*(IF\\s+NOT\\s+EXISTS)?\\s+\\(\\s*((?:\\(\\s*[A-Za-z][A-Za-z0-9]+\\s*,\\s*[A-Za-z]+\\s*,\\s*[0-9]+\\s*,\\s*[A-Za-z0-9\"']+\\s*\\)\\s*,?\\s*)+?)\\s*\\)$";
//	string fullCreateIndex = "^\\s*CREATE\\s+INDEX\\s*(IF\\s+NOT\\s+EXISTS)?\\s+([a-zA-Z0-9]+)\\s+ON\\s+([a-zA-Z0-9]+)\\s+\\((\\s*[a-zA-Z0-9]+)\\s*\\)$";
//	string fullDropTable = "^\\s*DROP\\s+TABLE\\s+([a-zA-Z0-9]+)\\s*$";
//	string fullDropIndex = "^\\s*DROP\\s+INDEX\\s+([a-zA-Z0-9]+)\\s*$";
//	string fullDisplayTable = "^\\s*DISPLAY\\s+TABLE\\s+([a-zA-Z0-9]*)\\s*$";
//	string fullInsertInto = "^\\s*INSERT\\s+INTO\\s+([a-zA-Z0-9]+)\\s+VALUES\\s*\\(((\\s*[a-zA-Z0-9\"']+\\s*,?\\s*)+)\\)\\s*$";
//	string fullDeleteFrom = "^\\s*DELETE\\s+FROM\\s+([a-zA-Z0-9]+)\\s+WHERE\\s+([a-zA-Z0-9]+)\\s+=\\s+([a-zA-Z0-9\"']+)\\s*$";
//	string fullSelect = "^\\s*SELECT\\s*((\\((\\s*[a-zA-Z0-9]+\\s*,?\\s*)+\\))+|(ALL))\\s*FROM\\s+([a-zA-Z0-9]+)+\\s*(WHERE\\s+([a-zA-Z0-9]+)\\s*=\\s*([a-zA-Z0-9]+))?$";
//	string fullUpdate = "^\\s*UPDATE\\s+([a-zA-Z0-9]+)\\s+SET\\s+([a-zA-Z0-9]+)\\s*=\\s*([a-zA-Z0-9\"']+)\\s+WHERE\\s+([a-zA-Z0-9]+)\\s*=\\s*([a-zA-Z0-9\"']+)\\s*$";
//};
//
////class RegexManager {
////private:
////	string* partialCommandStr = nullptr;
////	string* fullCommandStr = nullptr;
////	regex* RgxExp = nullptr;
////	regex* RgxExpPartial = nullptr;
////	int noCommands = 0;
////public:
////	RegexManager() {
////		this->partialCommandStr = new string[noCommands];
////		
////	}
////};

// daca se adauga o comanda noua
// trebuie modificat in 3 locuir
// - fun_ptr_array din cmdprocessor
// - noFunctions
// - Fisierul text de care se ocupa rgxmanager
// - functia implementata

class CmdProcessor
{
private:
	string fullCmd = "";
	std::function<void(std::smatch, TableBuffer&)>* fun_ptr_array = nullptr;
public:

	static const int noFunctions = 9;

	CmdProcessor() {
		fun_ptr_array = new std::function<void(std::smatch, TableBuffer&)>[noFunctions];
		fun_ptr_array[0] = std::bind(&CmdProcessor::createTable, this, std::placeholders::_1, std::placeholders::_2);
		fun_ptr_array[1] = std::bind(&CmdProcessor::createIndex, this, std::placeholders::_1, std::placeholders::_2);;
		fun_ptr_array[2] = std::bind(&CmdProcessor::dropTable, this, std::placeholders::_1, std::placeholders::_2);;
		fun_ptr_array[3] = std::bind(&CmdProcessor::dropIndex, this, std::placeholders::_1, std::placeholders::_2);;
		fun_ptr_array[4] = std::bind(&CmdProcessor::displayTable, this, std::placeholders::_1, std::placeholders::_2);;
		fun_ptr_array[5] = std::bind(&CmdProcessor::insertInto, this, std::placeholders::_1, std::placeholders::_2);;
		fun_ptr_array[6] = std::bind(&CmdProcessor::deleteFrom, this, std::placeholders::_1, std::placeholders::_2);;
		fun_ptr_array[7] = std::bind(&CmdProcessor::select, this, std::placeholders::_1, std::placeholders::_2);;
		fun_ptr_array[8] = std::bind(&CmdProcessor::update, this, std::placeholders::_1, std::placeholders::_2);;
	}

	bool checkCmd(TableBuffer& tableBuffer) {


		inRegex partialRegexManager;
		partialRegexManager.read(partialRegexFilePath);
		regex** partialRegexesList = partialRegexManager.getRegexes();
		int noCommands = partialRegexManager.getNoCommands();

		inRegex fullRegexManager;
		fullRegexManager.read(regexFilePath);
		regex** fullRegexesList = fullRegexManager.getRegexes();

		

		for (int commandNumber = 0; commandNumber < noCommands; commandNumber++) {
			if (regex_search(this->fullCmd, *partialRegexesList[commandNumber])) {
				smatch matches;
				if (regex_search(this->fullCmd, matches, *fullRegexesList[commandNumber])) {
					fun_ptr_array[commandNumber](matches, tableBuffer);
					return 1;
				}
				else {
					properFormats pf;
					cout << endl << "Proper format is:" << "\n" << pf.formats[commandNumber];
					return 1;
				}
			}
		}
		return 0;
	}

	void insertCommands(TableBuffer& tableBuffer) {
		ifstream comenzi("comenzi.txt");
		string comanda;
		try {
			if (comenzi.is_open()) {
				while (getline(comenzi, comanda)) {
					this->fullCmd = comanda;

					this->checkCmd(tableBuffer);
				}
				comenzi.close();
			}
		}
		catch (exception& e) {
			cout << endl << e.what();
		 }
		
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


	~CmdProcessor() {
		delete[] fun_ptr_array;
	}


private:

	void createTable(smatch matches, TableBuffer& tableBuffer) {
		regex partitionRegex("[^ ,()][a-zA-Z0-9\"'”’\\s*]*");
		smatch partitionMatches;

		// matches[0] = toata comanda
		// matches[1] = table name,
		// matches[2] = if not exists asta daca am scris daca nu e ""
		//  iar al patrulea input este doar ce este in paranteza (col_name, etc... )
		
		string tableName = matches[1].str();

		if (matches[2] != "") {
			cout << endl << "IF NOT EXISTS USED.";
			if (tableBuffer.doesTableExist(tableName)) {
				throw exception("Another table found with the same name.");
			}
		}

		string beforePartition = matches[3].str();

		auto words_begin = sregex_iterator(beforePartition.begin(), beforePartition.end(), partitionRegex);
		auto words_end = sregex_iterator();

		// de ce am impartit la 4 ?
		// pentru ca in fiecare paranteza sunt cate 4 valori
		// noi vrem sa creeam cate o coloana pt fiecare paranteza
		// iar fiecare coloana are atribuite cate 4 valori
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
		BinDocument file(tableName + ".bin");
		file.writeTable(t);

		tableBuffer = tableBuffer + t;
	}

	// trebuie lucrat de aici in jos
	void createIndex(smatch matches, TableBuffer& tableBuffer) {
		return;
	}

	void dropTable(smatch matches, TableBuffer& tableBuffer) {
		cout << matches.str();
		remove((matches[1].str() + ".bin").c_str());
		tableBuffer.remove(matches[1].str());
	}

	void dropIndex(smatch matches, TableBuffer& tableBuffer) {
		return;
	}

	void displayTable(smatch matches, TableBuffer& tableBuffer) {
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

	void insertInto(smatch matches, TableBuffer& tableBuffer) {
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
				cout << endl << "There is no table with name: " << tableNameInput << " in buffer.";
				cout << endl << "Let's check in files.";
				inTable fTable(tableNameInput);

				foundTable = fTable.getTable();
				tableBuffer = tableBuffer + foundTable;
			}
		}

		regex partitionRegex("[^ ,][a-zA-Z0-9\"'\\s*]*");

		smatch input;
		string paranteza = matches[2].str();

		auto words_begin = sregex_iterator(paranteza.begin(), paranteza.end(), partitionRegex);
		auto words_end = sregex_iterator();

		if (foundTable.getNoColumns() != distance(words_begin, words_end)) {
			throw exception("\nError: You should insert as many values as the number of columns in that specific table.");
		}

		string* data = new string[foundTable.getNoColumns()];
		int j = 0;
		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			smatch match = *i;
			string match_str = match.str();
			data[j] = match_str;
			j += 1;
		}

		tableBuffer.insertRowByName(data, tableNameInput);
		BinDocument doc(tableNameInput + ".bin");
		
		doc.writeRows(data, tableBuffer[tableNameInput]);

		delete[] data;
		delete[] tables;
	}

	void deleteFrom(smatch matches, TableBuffer& tableBuffer) {
		return;
	}

	void select(smatch matches, TableBuffer& tableBuffer) {
		return;
	}

	void update(smatch matches, TableBuffer& tableBuffer) {
		return;
	}
};