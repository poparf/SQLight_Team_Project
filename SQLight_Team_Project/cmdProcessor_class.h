#pragma once
#include <regex>
#include "utils.h"
#include "Column.h"
#include "Table.h"
#include "TableBuffer.h"
#include "Files.h"
#include "RgxManager.h"
#include <map>

using namespace std;

// NEW PROPER FORMATS
class Format {
public:
	map<string, string> guideline;

	Format() {
		guideline["CREATE TABLE"] = "CREATE TABLE table_name [IF NOT EXISTS] ( (column_name1, type, size, default_value1), (column_name1, type, size, default_value1)... )";
		guideline["CREATE INDEX"] = "CREATE INDEX [IF NOT EXISTS] index_name ON table_name (column_name) - creates an index only on a single column";
		guideline["DROP TABLE"] = "DROP TABLE table_name";
		guideline["DROP INDEX"] = "DROP INDEX index_name";
		guideline["DISPLAY TABLE"] = "DISPLAY TABLE table_name";
		guideline["INSERT INTO"] = "INSERT INTO table VALUES(...); values are separated by , and they have the exact number and order as the table definition";
		guideline["DELETE FROM"] = "DELETE FROM table_name WHERE column_name = value (deletes allows only one column in the where clause)";
		guideline["SELECT"] = "SELECT (at_least_one_column, ...) | ALL FROM table_name [WHERE column_name = value] - the where clause is optional";
		guideline["UPDATE"] = "UPDATE table_name SET column_name = value WHERE  column_name = value (the SET column may be different than the WHERE one)";
		guideline["IMPORT"] = "IMPORT table_name file_name";
	}
};

class Statement
{
protected:
	static string input;

public:
	static string getInput() {
		return input;
	}

	static void setInput(string s) {
		if (s.length() > 0) {
			input = s;
		}
	}

	static void setInput() {
		getline(cin, input);

		if (input.length() <= 0) {
			input = ""; // Reset the input in the statement class
			throw exception("Error reading the input. Try again.");
		}
	};
};


// Since these are simple commands we ll use a string not a regex
class SecondaryCmd : public Statement {
protected:
	string cmd;
public:
	static int counter;

	virtual bool check() {
		if (this->cmd == this->input)
			return 1;
		return 0; // just in case
	};

	virtual void process() = 0;
};

int SecondaryCmd::counter = 0;

class UtilityCmd : public SecondaryCmd {
public:

};



class QuitCmd : public UtilityCmd {
public:
	QuitCmd() {
		this->cmd = "/quit";
		this->counter++;
	}

	void process() {
		exit(EXIT_SUCCESS);
	}
};

class ClearConsoleCmd : public UtilityCmd {
public:
	ClearConsoleCmd() {
		this->cmd = "/clear";
		this->counter++;
	}

	void process() {
		system("cls");
	}
};

class HelpCmd : public UtilityCmd {
public:
	HelpCmd() {
		this->cmd = "/help";
		this->counter++;
	}

	void process() {
		cout << endl << "Available commands at this point:";
		cout << "\n" << "/toggle xml";
		cout << "\n" << "/toggle csv";
		cout << "\n" << "/quit";
		cout << "\n" << "/clear";
		Format pf;
		for (auto it = pf.guideline.begin(); it != pf.guideline.end(); ++it) {
			cout << "\n" << it->second;
		}
	}
};

class SettingsCmd : public SecondaryCmd {
protected:
	static bool xmlGeneration;
	static bool csvGeneration;
public:
	static bool isXMLon() {
		return xmlGeneration;
	}

	static bool isCSVon() {
		return csvGeneration;
	}
};

bool SettingsCmd::xmlGeneration = false;
bool SettingsCmd::csvGeneration = false;

class XML : public SettingsCmd {
public:
	XML() {
		this->counter++;
	}

	void process() {
		if (this->xmlGeneration) {
			this->xmlGeneration = false;
			cout << endl << "XML generation was turned off.";
		}
		else {
			this->xmlGeneration = true;
			cout << endl << "XML generation was turned on.";
		}
	}
};

class CSV : public SettingsCmd {
public:
	CSV() {
		this->cmd = "/toggle csv";
		this->counter++;
	}

	void process() {
		if (this->csvGeneration) {
			this->csvGeneration = false;
			cout << endl << "CSV generation was turned off.";
		}
		else {
			this->csvGeneration = true;
			cout << endl << "CSV generation was turned on.";
		}
	}
	
};


class PrimaryCmd : public Statement {
protected:
	regex cmdRgx;
	regex partialRgx;
	smatch matches;

	static const regex partitionRegex;
public:
	static int counter; // increases with each command instance created
	static TableBuffer* tb;
	static Format pf; // pf from proper format

	virtual bool check() = 0;

	// Buffer releated
	virtual void process() = 0;
protected:
	void loadTableIfNecessary(string tableNameInput) {
		if ((*tb).isTable(tableNameInput) == -1) {
			// Check if the file exists
			ifstream f(tableNameInput + ".bin", ios::binary);
			if (!f.is_open()) {
				throw exception("INSERT_ERROR: There is no table with this name.");
			}

			// Load the table from the file
			inTable it;
			it.readIntoBuffer(tableNameInput + ".bin", (*tb));
			f.close();
		}
	}

	void generateExtensions(Table& t) {
		if (SettingsCmd::isXMLon() == true) {
			xmlFile xml;
			xml.generateXML(t.getName(), t);
		}
		if (SettingsCmd::isCSVon() == true) {
			csvTable csv;
			csv.generateCSV(t.getName(), t);
		}
	}
};

Format PrimaryCmd::pf = Format();
const regex PrimaryCmd::partitionRegex("[^ ,()][a-zA-Z0-9\"'”’\s*]*");
int PrimaryCmd::counter = 0;
TableBuffer* PrimaryCmd::tb = TableBuffer::getInstance(); 

class CreateOperation : public PrimaryCmd {
public:
	// Write into a file using a file class
	// Every Create command does this
	virtual void write() = 0;
};


class CreateTable : public CreateOperation {
public:
	CreateTable() {
		this->cmdRgx.assign("^\\s*CREATE\\s+TABLE\\s+([A-Za-z][A-Za-z0-9]+)\\s*(IF\\s+NOT\\s+EXISTS)?\\s+\\(\\s*((?:\\(\\s*[A-Za-z][A-Za-z0-9]+\\s*,\\s*[A-Za-z]+\\s*,\\s*[0-9]+\\s*,\\s*[A-Za-z0-9\"]+\\s*\\)\\s*,?\\s*)+?)\\s*\\)$", regex::icase);
		this->partialRgx.assign("^\\s*CREATE\\s+TABLE\\s*", regex::icase);
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx))
			return 0;

		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["CREATE TABLE"];
			return 0;
		}

		string tableName = matches[1].str();
		// If not exist condition
		if (matches[2] != "") {
			// Checking the buffer
			if ((*tb).isTable(tableName) != -1) {
				throw exception("Another table found with the same name IN BUFFER.");
			}
			// Checking the files
			ifstream file(tableName + ".bin", ios::binary);
			if (file.is_open()) {

				file.close();
				throw exception("Another table found with the same name ON DISK.");
			}
		}
		return 1;
	}

	void process() {
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
		Table t(matches[1].str(), cols, noCols);
		this->write(t);
	}
private:
	void write() {  };
	void write(Table t) {
		(*tb).addTable(t);
		outTable ot;
		ot.write(matches[1].str() + ".bin", t);
	}
};

class CreateIndex : public CreateOperation {
public:

	CreateIndex() {
		this->cmdRgx.assign("^\s*CREATE\\s+INDEX\\s*(IF\\s+NOT\\s+EXISTS)?\\s+([a-zA-Z0-9_]+)\\s+ON\\s+([a-zA-Z0-9]+)\\s*\\((\\s*[a-zA-Z0-9]+)\\s*\\)$", regex::icase);
		this->partialRgx.assign("^\\s*CREATE\\s+INDEX\\s*", regex::icase);
		this->counter++;
	}

	bool check() {
		//cout << endl << matches[1].str(); // if not exists
		//cout << endl << matches[2].str(); // index name
		//cout << endl << matches[3].str(); // table name
		//cout << endl << matches[4].str(); // column name
		
		if (!regex_search(this->input, this->partialRgx)) {
			return 0;
		}
		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["CREATE INDEX"];
			return 0;
		}

		if (matches[1].str() != "") {
			// Check if it is already an index there with the same name
			fstream file(matches[1].str() + ".idx");
			if (file.is_open()) {
				file.close();
				throw exception("There is already an index with the same name");
			}
		}
		return 1;
	}

	void process() {
		this->loadTableIfNecessary(matches[3].str());
		Table t = (*tb).getTable((*tb).isTable(matches[3].str()));

		int noOffsets = 0;
		int colIndex = t.isColumn(matches[4].str());
		if (colIndex == -1) {
			throw exception("Column not found.");
		}
		Column col = t.getColumn(colIndex);
		//offsets = t.getOffsets(matches[4].str(), noOffsets); !!!!!!!!!!!!! // The processing part should be done here no in the table itself

		////////
		ifstream file(t.getName() + ".bin", ios::binary);

		if (!file.is_open())
			throw exception("File couldn't be opened.");

		int* offsets = new int[9999];

		int noCols;
		int noRows;
		file.read((char*)&noCols, sizeof(int));
		file.read((char*)&noRows, sizeof(int));

		Row** rows = new Row * [noRows];
		Column** cols = new Column * [noCols];

		for (int i = 0; i < noCols; i++) {
			int colNameSize;
			file.read((char*)&colNameSize, sizeof(int));

			char buffer[1000];
			file.read(buffer, sizeof(char) * colNameSize);
			string colName = string(buffer);

			int colType;
			file.read((char*)&colType, sizeof(int));

			int size;
			file.read((char*)&size, sizeof(int));

			int colDefValueSize;
			file.read((char*)&colDefValueSize, sizeof(int));

			char buffer2[1000];
			file.read(buffer2, sizeof(char) * colDefValueSize);
			string defValue = string(buffer2);

			cols[i] = new Column(colName, (columnTypes)colType, size, defValue);
		}


		for (int i = 0; i < noRows; i++) {
			int noCells;
			file.read((char*)&noCells, sizeof(int));
			Article** cells = new Article * [noCells];

			for (int j = 0; j < noCells; j++) {
				if (j == colIndex) {
					offsets[noOffsets++] = file.tellg();
					if (noOffsets == 9999) {
						int* newOffsets = new int[noOffsets * 2];
						for (int i = 0; i < noOffsets; i++)
							newOffsets[i] = offsets[i];

						delete[] offsets;
						offsets = newOffsets;
					}
				}

				int dataSize;
				file.read((char*)&dataSize, sizeof(int));

				char buffer3[1000];
				file.read(buffer3, sizeof(char) * dataSize);
				string data = string(buffer3);

				cells[j] = new Article(data);
			}

			rows[i] = new Row(cells, noCells);
		}

		for (int i = 0; i < noRows; i++) {
			if (rows[i] != nullptr)
				delete rows[i];
		}
		delete[] rows;

		for (int i = 0; i < noCols; i++) {
			if (cols[i] != nullptr)
				delete cols[i];
		}
		delete[] cols;


		int* newOffsets = new int[noOffsets];
		for (int i = 0; i < noOffsets; i++)
			newOffsets[i] = offsets[i];

		delete[] offsets;
		offsets = newOffsets;
		

		/////////////////
		col.setIndex(matches[2].str(), offsets, noOffsets);

		t.setColumn(col, colIndex);
		(*tb).replaceTable(t);
	}
	private:
	// TODO
	/*
	Indexes are not loaded into the table from the files yet
	The logic of writing indexes to the files is in a special class but called when we setIndex() to a column
	*/
	void write() {

	}
};

class InsertRow : public CreateOperation {
public:

	InsertRow() {
		this->cmdRgx.assign("^\\s*INSERT\\s+INTO\\s+([a-zA-Z0-9]+)\\s+VALUES\\s*\\(((\\s*[a-zA-Z0-9\"']+\\s*,?\\s*)+)\\)\\s*$", regex::icase);
		this->partialRgx.assign("^\\s*INSERT\\s+INTO\\s*", regex::icase);
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx)) {
			return 0;
		}
		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["INSERT ROW"];
			return 0;
		}
		return 1;
	}

	void process() {
		//	// matches[1] =  table name
		//	// matches[2] = ce e in paranteza
		string tableNameInput = matches[1].str();

		loadTableIfNecessary(tableNameInput);

		Table t = (*tb).getTable((*tb).isTable(tableNameInput));

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
		(*tb).replaceTable(t);

		for (int i = 0; i < noCells; i++) {
			delete cells[i];
		}
		delete[] cells;
		
		this->write(row, t);
	}
private:
	void write() {}; // how do i fix this?
	void write(Row row, Table t) {
		outTable ot;
		ot.writeRow(matches[1].str() + ".bin", row, t.getNoRows());
	}

};

// Import table data from CSV
// Extending InsertRow functionality: requirment 2
class Import : public InsertRow {
public:

	Import() {
		this->cmdRgx.assign("^\\s*IMPORT\\s+([a-zA-Z0-9]+)\\s+([a-zA-Z0-9.]+)$", regex::icase);
		this->partialRgx.assign("^\\s*IMPORT\\s*", regex::icase);
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx)) {
			return 0;
		}
		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["IMPORT"];
			return 0;
		}
		//	cout << endl << matches[1].str(); // table name
		//	cout << endl << matches[2].str(); // filename


		return 1;
	}

	void process() {
		loadTableIfNecessary(matches[1].str());
		Table t = (*tb).getTable((*tb).isTable(matches[1].str()));
		(*tb).replaceTable(t);
		this->write(t);
	}
private:

	void write() {};
	void write(Table& t) {
		csvTable csv;
		csv.readCSV(matches[2].str(), t);
	}
};

class ReadOperation : public PrimaryCmd {
protected:
	regex partialRgx;
public:
	virtual void display() = 0;

	// CSV/XML files
	virtual void raport() = 0;
};

class SelectValues : public ReadOperation {
public:
	SelectValues() {
		this->cmdRgx.assign("^\\s*SELECT\\s*((\\((\\s*[a-zA-Z0-9]+\\s*,?\\s*)+\\))+|(ALL))\\s*FROM\\s+([a-zA-Z0-9]+)+\\s*(WHERE\\s+([a-zA-Z0-9]+)\\s*=\\s*([a-zA-Z0-9]+))?$", regex::icase);
		this->partialRgx.assign("^\\s*SELECT\\s*", regex::icase);
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx)) {
			return 0;
		}
		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["CREATE INDEX"];
			return 0;
		}
		return 1;
	}
	void process() {
		//cout << matches[1].str();		  // ce e in paranteza sau all
		//cout << endl << matches[5].str(); // table name
		//cout << endl << matches[7].str(); // coloana din where
		//cout << endl << matches[8].str(); // valoarea din where
		string tableName = matches[5].str();
		loadTableIfNecessary(tableName);


		Table t = (*tb).getTable((*tb).isTable(tableName));
		if (toLowerCase(matches[1].str()) == "all") {
			// all columns with no where
			if (matches[7].str() == "") {
				cout << t;
				generateExtensions(t);
			} // all columns with where
			else {
				Table temp = t.filterTableWithWhereClause(matches[7].str(), matches[8].str()); // rename this..
				cout << temp;
				generateExtensions(temp);
			}
		}
		else {
			//specific columns with no where
			if (matches[7].str() == "") {
				string paranteza = matches[1].str();

				auto words_begin = sregex_iterator(paranteza.begin(), paranteza.end(), this->partitionRegex);
				auto words_end = sregex_iterator();

				int noCols = distance(words_begin, words_end);


				int j = 0;
				string* colNamesToBePrinted = new string[noCols];
				for (sregex_iterator i = words_begin; i != words_end; ++i) {
					smatch match = *i;
					colNamesToBePrinted[j++] = match.str();
				}


				Table temp = t.filterTableSpecificColumns(colNamesToBePrinted, noCols); // rename this..
				cout << temp;
				generateExtensions(temp);
			}
			else {
				// specific columns with where
				// You have to print only specific columns
				string paranteza = matches[1].str();

				auto words_begin = sregex_iterator(paranteza.begin(), paranteza.end(), this->partitionRegex);
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
				Table temp = t.filterTableWithWhereClauseAndSpecificColumns(colNamesToBePrinted, noCols, whereColumn, valueToMatch); // rename this..

				cout << temp;
				generateExtensions(temp);
			}
		}
	}

private:
	void display() {};
	void display(Table& temp) {
		cout << temp;
	}
	void raport() {};
	void raport(Table& temp) {
		generateExtensions(temp);
	}

};


class DisplayTable : public ReadOperation {
public:
	
	DisplayTable() {
		this->cmdRgx.assign("^\\s*DISPLAY\\s+TABLE\\s+([a-zA-Z0-9]*)\\s*$", regex::icase);
		this->partialRgx.assign("^\\s*DISPLAY\\s+TABLE\\s*", regex::icase);
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx))
			return 0;

		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["DISPLAY TABLE"];
			return 0;
		}

		return 1;
	}

	void process() {
		// Checking the buffer
		loadTableIfNecessary(matches[1].str());

		Table t = (*tb).getTable((*tb).isTable(matches[1].str()));
		cout << t;

		this->generateExtensions(t);
	}
private:
	void display() {
		Table t = (*tb).getTable((*tb).isTable(matches[1].str()));
		cout << t;
	}

	void raport() {
		Table t = (*tb).getTable((*tb).isTable(matches[1].str()));
		this->generateExtensions(t);
	}
};

class UpdateOperation : public PrimaryCmd {
protected:
	regex partialRgx;
public:
	virtual void write() = 0;
};

class UpdateTable : public UpdateOperation {
public:
	UpdateTable() {
		this->cmdRgx.assign("^\\s*UPDATE\\s+([a-zA-Z0-9]+)\\s+SET\\s+([a-zA-Z0-9]+)\\s*=\\s*([a-zA-Z0-9\"']+)\\s+WHERE\\s+([a-zA-Z0-9]+)\\s*=\\s*([a-zA-Z0-9\']+)\\s*$", regex::icase);
		this->partialRgx.assign("^\\s*UPDATE\\s*", regex::icase);
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx))
			return 0;

		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["UPDATE TABLE"];
			return 0;
		}

		return 1;
	}

	void process() {
		//cout << endl << matches[1].str(); // table name
		//cout << endl << matches[2].str(); // SET Column
		//cout << endl << matches[3].str(); // SET Value
		//cout << endl << matches[4].str(); // WHERE Column
		//cout << endl << matches[5].str(); // WHERE Value

		this->loadTableIfNecessary(matches[1].str());
		string tableName = matches[1].str();
		Table t = (*tb).getTable((*tb).isTable(matches[1].str()));

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
		(*tb).replaceTable(t);
	
		this->write();
	}
private:
	void write() {
		outTable ot;
		Table t = (*tb).getTable((*tb).isTable(matches[1].str()));
		ot.write(t.getName() + ".bin", t);
		this->generateExtensions(t);
	}
};

class DeleteOperation : public PrimaryCmd {
public:
	virtual void del() = 0;
};

class DropTable : public DeleteOperation {
public:
	DropTable() {
		this->cmdRgx.assign("^\\s*DROP\\s+TABLE\\s+([a-zA-Z0-9]+)\\s*$", regex::icase);
		this->partialRgx.assign("^\\s*DROP\\s+TABLE\\s*", regex::icase);
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx))
			return 0;
		
		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["DROP TABLE"];
			return 0;
		}
		return 1;
	}

	void process() {
		int index = (*tb).isTable(matches[1].str());
		if (index == -1)
			throw exception("There is not table with this name in buffer.");

		(*tb).removeTable(index);
		this->del();
	}
private:
	void del() {
		if (remove((matches[1].str() + ".bin").c_str()))
			cout << endl << "There is no file with this name on the disk.";
	}
};

class DropIndex : public DeleteOperation {
public:
	DropIndex() {
		this->cmdRgx.assign("^\\s*DROP\\s+INDEX\\s+([a-zA-Z0-9]+)\\s*$", regex::icase);
		this->partialRgx.assign("^\\s*DROP\\s+INDEX\\s*", regex::icase);
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx))
			return 0;

		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["DROP INDEX"];
			return 0;
		}
		return 1;
	}
	void process() {

		// TODO: 
		this->del();


		int noTables = (*tb).getNoTables();
		for (int i = 0; i < noTables; i++) {
			Table t = (*tb).getTable(i);
			int noCols = t.getNoColumns();
			for (int j = 0; j < noCols; j++) {
				Column col = t.getColumn(j);
				Index colIndex = col.getIndex();
				if (colIndex.getName() == matches[1].str()) {
					col.setIndex("", nullptr, 0);
					t.setColumn(col, j);
					(*tb).replaceTable(t);
					return;
				}
			}
		}
		return;
	}
private:
	void del() {
		if (remove((matches[1].str() + ".idx").c_str()))
			cout << endl << "There is no index with this name on the disk.";
	}
};

class DeleteFrom : public DeleteOperation {
public:

	DeleteFrom() {
		this->cmdRgx.assign("^\\s*DELETE\\s+FROM\\s+([a-zA-Z0-9]+)\\s+WHERE\\s+([a-zA-Z0-9]+)\\s+=\\s+([a-zA-Z0-9]+)\\s*$", regex::icase);
		this->partialRgx.assign("^\\s*DELETE\\s+FROM\\s*", regex::icase);
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx))
			return 0;

		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["DELETE FROM"];
			return 0;
		}
		return 1;
	}
	void process() {
		string tableName = matches[1].str();
		// Checking the buffer
		loadTableIfNecessary(tableName);

		Table t = (*tb).getTable((*tb).isTable(tableName));
		t.deleteFrom(matches[2].str(), matches[3].str());
		(*tb).replaceTable(t);

		outTable ot;
		ot.write(matches[1].str() + ".bin", t);
	}
private:
	void del() {
		string tableName = matches[1].str();
		Table t = (*tb).getTable((*tb).isTable(tableName));
		t.deleteFrom(matches[2].str(), matches[3].str());
		(*tb).replaceTable(t);
	}
};