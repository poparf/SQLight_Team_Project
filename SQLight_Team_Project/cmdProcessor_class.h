#pragma once
#include <regex>
#include <functional> 
#include "utils.h"
#include "Column.h"
#include "Table.h"
#include "TableBuffer.h"
#include "Files.h"
#include "RgxManager.h"


#define regexFilePath "regexList.txt"
#define partialRegexFilePath "partialRegexList.txt"

//typedef void (*fun_ptr_t)(smatch, TableBuffer);
//typedef void (CmdProcessor::* mem_fun_ptr_t)(std::smatch, TableBuffer&);
using namespace std;


class properFormats {
public:
	string* formats = nullptr;
	int noFormats = 10;

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
		formats[9] = "IMPORT table_name file_name";
	}


	~properFormats() {
		delete[] formats;
	}
};

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
	function<void(smatch, TableBuffer&)>* fun_ptr_array = nullptr;

	bool xmlGeneration = 0;
	bool csvGeneration = 0;
	int noGen = 0;
	
public:

	string activateXML() {
		if (this->xmlGeneration) {
			return "\nXML generation is already on.\n";
		}
		this->xmlGeneration = true;
		return "\nXML generation activated.\n";
	}

	string deactivateXML() {
		if (this->xmlGeneration == false) {
			return "\nXML generation is already off.";
		}
		this->xmlGeneration = false;
		return "\nXML generation deactivated.";
	}
	string activateCSV() {
		if (this->csvGeneration) {
			return "\nCSV generation is already on.\n";
		}
		this->csvGeneration = true;
		return "\nCSV generation activated.\n";
	}

	string deactivateCSV() {
		if (this->csvGeneration == false) {
			return "\nCSV generation is already off.";
		}
		this->csvGeneration = false;
		return "\nCSV generation deactivated.";
	}


	static const int noFunctions = 10;

	CmdProcessor() {
		fun_ptr_array = new function<void(smatch, TableBuffer&)>[noFunctions];
		fun_ptr_array[0] = bind(&CmdProcessor::createTable, this, placeholders::_1, placeholders::_2);
		fun_ptr_array[1] = bind(&CmdProcessor::createIndex, this, placeholders::_1, placeholders::_2);
		fun_ptr_array[2] = bind(&CmdProcessor::dropTable, this, placeholders::_1, placeholders::_2);
		fun_ptr_array[3] = bind(&CmdProcessor::dropIndex, this, placeholders::_1, placeholders::_2);
		fun_ptr_array[4] = bind(&CmdProcessor::displayTable, this, placeholders::_1, placeholders::_2);
		fun_ptr_array[5] = bind(&CmdProcessor::insertInto, this, placeholders::_1, placeholders::_2);
		fun_ptr_array[6] = bind(&CmdProcessor::deleteFrom, this, placeholders::_1, placeholders::_2);
		fun_ptr_array[7] = bind(&CmdProcessor::select, this, placeholders::_1, placeholders::_2);
		fun_ptr_array[8] = bind(&CmdProcessor::update, this, placeholders::_1, placeholders::_2);
		fun_ptr_array[9] = bind(&CmdProcessor::importCSV, this, placeholders::_1, placeholders::_2);
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

	void insertCommands(string fileName, TableBuffer& tableBuffer) {
		ifstream comenzi(fileName);
		string comanda;
		try {
			if (comenzi.is_open()) {
				while (getline(comenzi, comanda)) {
					this->fullCmd = comanda;
					cout << endl << fullCmd;
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

	void createTable(smatch matches, TableBuffer& tb) {
		regex partitionRegex("[^ ,()][a-zA-Z0-9\"'”’\\s*]*");
		smatch partitionMatches;
		string tableName = matches[1].str();
		// If not exist condition
		if (matches[2] != "") {
			// Checking the buffer
			if (tb.isTable(tableName) != -1) {
				throw exception("Another table found with the same name IN BUFFER.");
			}
			// Checking the files
			ifstream file(tableName + ".bin", ios::binary);
			if (file.is_open()) {

				file.close();
				throw exception("Another table found with the same name ON DISK.");
			}
		}

		string beforePartition = matches[3].str();

		auto words_begin = sregex_iterator(beforePartition.begin(), beforePartition.end(), partitionRegex);
		auto words_end = sregex_iterator();

		// de ce am impartit la 4 ?
		// pentru ca in fiecare paranteza sunt cate 4 valori
		// noi vrem sa creeam cate o coloana pt fiecare paranteza
		// iar fiecare coloana are atribuite cate 4 valori
		int noCols = distance(words_begin, words_end) / 4;
		Column** cols = new Column * [noCols];
		for (int i = 0; i < noCols; i++)
			cols[i] = new Column("", columnTypes::FLOAT, 0, ""); // we initialize a default column since we don t have a default constructor

		int j = 0;
		int k = 0;
		// aici nu putem folosi i pentru ca e de tipul regex iterator nu int
		for (sregex_iterator i = words_begin; i != words_end; ++i) {

			smatch match = *i;
			string match_str = match.str();
			switch (k) {
			case 0:
				cols[j]->setName(match_str);
				break;
			case 1:
				if (toLowerCase(match_str) == "integer")
					cols[j]->setType(columnTypes::INTEGER);
				else if (toLowerCase(match_str) == "float")
					cols[j]->setType(columnTypes::FLOAT);
				else if (toLowerCase(match_str) == "text")
					cols[j]->setType(columnTypes::TEXT);
				else
					throw exception("Type of column must be integer, float or text.");
				break;
			case 2:
				// stoi() transforms strings in integers
				cols[j]->setSize(stoi(match_str));
				break;
			case 3:
				cols[j]->setDef(match_str);
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
		
		// We create the table and save it in the buffer and in files
		Table t(tableName, cols, noCols);
		tb.addTable(t);
		outTable ot;
		ot.write(tableName + ".bin", t);
	}
	
	// Not implemented
	void createIndex(smatch matches, TableBuffer& tableBuffer) {
		cout << endl << "Not implemented." << endl;
		return;
	}

	void dropTable(smatch matches, TableBuffer& tableBuffer) {
		if (remove((matches[1].str() + ".bin").c_str()))
			cout << endl << "There is no file with this name on the disk.";

		int index = tableBuffer.isTable(matches[1].str());
		if (index == -1)
			throw exception("There is not table with this name in buffer.");
	
		tableBuffer.removeTable(index);
	}

	// Not implemented
	void dropIndex(smatch matches, TableBuffer& tableBuffer) {
		cout << endl << "Not implemented." << endl;
		return;
	}

	void displayTable(smatch matches, TableBuffer& tb) {
		
		string tableName = matches[1].str();
		// Checking the buffer
		loadTableIfNecessary(tableName, tb);

		Table t = tb.getTable(tb.isTable(tableName));
		cout << t;

		this->generateExtensions(t);
	}

	void insertInto(smatch matches, TableBuffer& tb) {
	//	// matches[1] =  table name
	//	// matches[2] = ce e in paranteza
		string tableNameInput = matches[1].str();

		loadTableIfNecessary(tableNameInput, tb);

		Table t = tb.getTable(tb.isTable(tableNameInput));

		regex partitionRegex("[^ ,][a-zA-Z0-9\"'\\s*]*");
		string paranteza = matches[2].str();

		auto words_begin = sregex_iterator(paranteza.begin(), paranteza.end(), partitionRegex);
		auto words_end = sregex_iterator();

		int noCells = distance(words_begin, words_end);
		Article** cells = new Article * [noCells];
		
		int j = 0;
		for (sregex_iterator i = words_begin; i != words_end; ++i) {
			smatch match = *i;
			cells[j++] = new Article(match.str());
		}
		Row row(cells, noCells);

		t.addRow(row);
		tb.replaceTable(t);
		outTable ot; 
		ot.writeRow(tableNameInput + ".bin", row, t.getNoRows());
		//ot.write(tableNameInput + ".bin", t);
		for (int i = 0; i < noCells; i++) {
			delete cells[i];
		}
		delete[] cells;
	}

	// Deletes rows from a table
	void deleteFrom(smatch matches, TableBuffer& tb) {
		string tableName = matches[1].str();
		// Checking the buffer
		loadTableIfNecessary(tableName, tb);

		Table t = tb.getTable(tb.isTable(tableName));

		t.deleteFrom(matches[2].str(), matches[3].str());
		tb.replaceTable(t);
		outTable ot;
		ot.write(matches[1].str() + ".bin", t);
		return;
	}

	void select(smatch matches, TableBuffer& tb) {
		//cout << matches[1].str();		  // ce e in paranteza sau all
		//cout << endl << matches[5].str(); // table name
		//cout << endl << matches[7].str(); // coloana din where
		//cout << endl << matches[8].str(); // valoarea din where
		string tableName = matches[5].str();
		loadTableIfNecessary(tableName, tb);


		Table t = tb.getTable(tb.isTable(tableName));
		if (toLowerCase(matches[1].str()) == "all") {
			
			if (matches[7].str() == "") {
				cout << t;
			}
			else {
				Table temp = t.filterTableWithWhereClause(matches[7].str(), matches[8].str(), this->noGen); // rename this..
				cout << temp;
				
				generateExtensions(temp);
			}
		}
		else {
			if (matches[7].str() == "") {
				regex partitionRegex("[^ ,)(][a-zA-Z0-9\"'\\s*]*");
				string paranteza = matches[1].str();

				auto words_begin = sregex_iterator(paranteza.begin(), paranteza.end(), partitionRegex);
				auto words_end = sregex_iterator();

				int noCols = distance(words_begin, words_end);


				int j = 0;
				string* colNamesToBePrinted = new string[noCols];
				for (sregex_iterator i = words_begin; i != words_end; ++i) {
					smatch match = *i;
					colNamesToBePrinted[j++] = match.str();
				}
				
				//t.printTableSpecificColumns(colNamesToBePrinted, noCols);
				Table temp = t.filterTableSpecificColumns(colNamesToBePrinted, noCols, this->noGen); // rename this..
				cout << temp;
				generateExtensions(temp);
			}
			else {

				// You have to print only specific columns
				regex partitionRegex("[^ ,)(][a-zA-Z0-9\"'\\s*]*");
				string paranteza = matches[1].str();

				auto words_begin = sregex_iterator(paranteza.begin(), paranteza.end(), partitionRegex);
				auto words_end = sregex_iterator();

				int noCols = distance(words_begin, words_end);


				int j = 0;
				string* colNamesToBePrinted = new string[noCols];
				for (sregex_iterator i = words_begin; i != words_end; ++i) {
					smatch match = *i;
					colNamesToBePrinted[j++] = match.str();
				}

				string whereColumn = matches[7].str();
				string valueToMatch = matches[8].str();
				//t.printTableWithWhereClauseAndSpecificColumns(colNamesToBePrinted, noCols, whereColumn, valueToMatch); // rename this..
				Table temp = t.filterTableWithWhereClauseAndSpecificColumns(colNamesToBePrinted, noCols, whereColumn, valueToMatch, noGen); // rename this..
				cout << temp;
				generateExtensions(temp);
			}
		}		
	}

	void update(smatch matches, TableBuffer& tb) {
		//cout << endl << matches[1].str(); // table name
		//cout << endl << matches[2].str(); // SET Column
		//cout << endl << matches[3].str(); // SET Value
		//cout << endl << matches[4].str(); // WHERE Column
		//cout << endl << matches[5].str(); // WHERE Value

		this->loadTableIfNecessary(matches[1].str(), tb);
		string tableName = matches[1].str();
		Table t = tb.getTable(tb.isTable(matches[1].str()));
		
		string setColumnName = matches[2].str();
		int setColIndex = t.isColumn(setColumnName);
		if (setColIndex == -1) {
			throw exception("ERROR: Unknown column name in SET.");
		}
		
		string setValueName = matches[3].str();

		string whereColumnName = matches[4].str();
		int whereColumnIndex = t.isColumn(whereColumnName);
		if (whereColumnIndex == -1) {
			throw exception("ERROR: Unknown column name in WHERE clause.");
		}

		string whereValueName = matches[5].str();

		t.update(setColumnName, setValueName, whereColumnName, whereValueName, whereColumnIndex, setColIndex);
		tb.replaceTable(t);
		outTable ot;
		ot.write(tableName + ".bin", t);
		this->generateExtensions(t);
		return;
	}
	

	void importCSV(smatch matches, TableBuffer& tb) {
	
	//	cout << endl << matches[1].str(); // table name
	//	cout << endl << matches[2].str(); // filename

		loadTableIfNecessary(matches[1].str(), tb);
		csvTable csv;
		Table t = tb.getTable(tb.isTable(matches[1].str()));
		csv.readCSV(matches[2].str(), t);
		tb.replaceTable(t);
	}

private:
	void loadTableIfNecessary(string tableNameInput, TableBuffer& tb) {
		if (tb.isTable(tableNameInput) == -1) {
			// Check if the file exists
			ifstream f(tableNameInput + ".bin", ios::binary);
			if (!f.is_open()) {
				throw exception("INSERT_ERROR: There is no table with this name.");
			}

			// Load the table from the file
			inTable it;
			it.readIntoBuffer(tableNameInput + ".bin", tb);
			f.close();
		}
	}

	void generateExtensions(Table& t) {
		if (this->xmlGeneration == true) {
			xmlFile xml;
			xml.generateXML(t.getName(), t);
		}
		if (this->csvGeneration == true) {
			csvTable csv;
			csv.generateCSV(t.getName(), t);
		}
		if (this->csvGeneration || this->xmlGeneration) {
			this->noGen++;
		}
	}
};