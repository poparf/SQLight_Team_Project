#pragma once
#include <regex>
#include <functional> 
#include "utils.h"
#include "Column.h"
#include "Table.h"
#include "TableBuffer.h"
#include "Files.h"
#include "RgxManager.h"
#include <map>

/*
✔Implementation of a class to exemplify the concept of class composition through has-a relations
The extension of classes defined in the previous phases by adding new attributes (necessary in this phase) is done only by derivation and NOT by modifying the existing classes - minimum 2 extensions
✔Implement at least one abstract class (with or without attributes) that must contain at least 2 pure virtual methods
✔Add to an existing class or a new class at least 2 virtual methods (other than pure virtual methods) that are overridden in derived classes
✔By using an abstract class or a basic class (newly defined or existing one) define at least one hierarchy to describe a family of classes (eg for types of expenses, etc.); an example of a family of classes defined to describe geometric shapes

		command



	this will be the way we call the check() command for every each of them

	CreateTable ct;
	CreateIndex ci;
	..

	PrimaryCmd* primaryCmds[PrimaryCmd::counter];

	Initialize each one...
	PrimaryCmd[0] = &ct;


✔Implement at least on vector of pointers to a base class type (the parent of the hierarchy) and use it to manage real objects from the hierarchy (a hierarchy/framework of classes is given by a set of classes that are in a is-a relation between them and have a common parent); an example of a family of classes defined to describe geometric shapes (for this example the pointers in the array will have the Shape type)
Optional Implement at least one vector, set, list, and map collection from the STL library to manage application data (collections are used either in existing classes or in new ones added at this stage). You can refactor an existing class by replacing common arrays with STL collections.
*/


#define regexFilePath "regexList.txt"
#define partialRegexFilePath "partialRegexList.txt"

//typedef void (*fun_ptr_t)(smatch, TableBuffer);
//typedef void (CmdProcessor::* mem_fun_ptr_t)(std::smatch, TableBuffer&);
using namespace std;


// OLD PROPER FORMATS
class properFormats {
public:
	string* formats = nullptr;
	int noFormats = 10;

	properFormats() {
		formats = new string[this->noFormats];
		// may use a map in the future.. ( edit: future is here )
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

// daca se adauga o comanda noua
// trebuie modificat in 3 locuir
// - fun_ptr_array din cmdprocessor
// - noFunctions
// - Fisierul text de care se ocupa rgxmanager
// - functia implementata


/*
Aici ar putea sa fie o interfata pentru comanda
fiecare comanda sa dea inherit la interfata


*/


class Statement
{
protected:
	static string input;

public:
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
		else
			throw exception("Wrong input. Try again.");
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
		this->cmdRgx.assign("^\s*CREATE\s+TABLE\s+([A-Za-z][A-Za-z0-9]+)\s*(IF\s+NOT\s+EXISTS)?\s+\(\s*((?:\(\s*[A-Za-z][A-Za-z0-9]+\s*,\s*[A-Za-z]+\s*,\s*[0-9]+\s*,\s*[A-Za-z0-9\"']+\s*\)\s*,?\s*)+?)\s*\)$");
		this->partialRgx.assign("^\s*CREATE\s+TABLE\s*");
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx))
			return 0;

		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["CREATE TABLE"];
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

		this->process();
		return 1;
	}

private:
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

	void write(Table t) {
		(*tb).addTable(t);
		outTable ot;
		ot.write(matches[1].str() + ".bin", t);
	}
};

class CreateIndex : public CreateOperation {
public:

	CreateIndex() {
		this->cmdRgx.assign("^\s*CREATE\s+INDEX\s*(IF\s+NOT\s+EXISTS)?\s+([a-zA-Z0-9_]+)\s+ON\s+([a-zA-Z0-9]+)\s*\((\s*[a-zA-Z0-9]+)\s*\)$");
		this->partialRgx.assign("^\s*CREATE\s+INDEX\s*");
		this->counter++;
	}

	bool check(TableBuffer& tb) {
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
				cout << endl << "There is already an index with the same name";
				file.close();
				return 0;
			}
		}

		// We have to get the offset for every row.
		loadTableIfNecessary(matches[3].str());
		process();

		return 1;
	}

private:
	void process() {
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
		this->cmdRgx.assign("^\s*INSERT\s+INTO\s+([a-zA-Z0-9]+)\s+VALUES\s*\(((\s*[a-zA-Z0-9\"']+\s*,?\s*)+)\)\s*$");
		this->partialRgx.assign("^\s*INSERT\s+INTO\s*");
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

		// write is called in process
		this->process();
		return 1;
	}


private:
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
		this->cmdRgx.assign("^\s*IMPORT\s+([a-zA-Z0-9]+)\s+([a-zA-Z0-9.]+)$");
		this->partialRgx.assign("^\s*IMPORT\s*");
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

		loadTableIfNecessary(matches[1].str());
		Table t = (*tb).getTable((*tb).isTable(matches[1].str()));
		this->write(t);
		this->process(t);
	}
private:

	void process(Table t) {
		(*tb).replaceTable(t);
	}

	void write(Table t) {
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
		this->cmdRgx.assign("^\s*SELECT\s*((\((\s*[a-zA-Z0-9]+\s*,?\s*)+\))+|(ALL))\s*FROM\s+([a-zA-Z0-9]+)+\s*(WHERE\s+([a-zA-Z0-9]+)\s*=\s*([a-zA-Z0-9]+))?$");
		this->partialRgx.assign("^\s*SELECT\s*");
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
		return 1;
	}
private:
	void process() {
		// lose of time to move the logic here..
	}

	void display(Table& temp) {
		cout << temp;
	}

	void raport(Table& temp) {
		generateExtensions(temp);
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
		this->cmdRgx.assign("^\s*UPDATE\s+([a-zA-Z0-9]+)\s+SET\s+([a-zA-Z0-9]+)\s*=\s*([a-zA-Z0-9\"']+)\s+WHERE\s+([a-zA-Z0-9]+)\s*=\s*([a-zA-Z0-9\"']+)\s*$");
		this->partialRgx.assign("^\s*UPDATE\s*");
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx))
			return 0;

		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["UPDATE TABLE"];
			return 0;
		}

		//cout << endl << matches[1].str(); // table name
		//cout << endl << matches[2].str(); // SET Column
		//cout << endl << matches[3].str(); // SET Value
		//cout << endl << matches[4].str(); // WHERE Column
		//cout << endl << matches[5].str(); // WHERE Value

		this->loadTableIfNecessary(matches[1].str());
		this->process(); // write is called in process
		return 1;
	}
private:
	void process() {
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


		this->write(t);
	}

	void write(Table& t) {
		outTable ot;
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
		this->cmdRgx.assign("^\s*DROP\s+TABLE\s+([a-zA-Z0-9]+)\s*$");
		this->partialRgx.assign("^\s*DROP\s+TABLE\s*");
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx))
			return 0;
		
		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["DROP TABLE"];
			return 0;
		}

		int index = (*tb).isTable(matches[1].str());
		if (index == -1)
			throw exception("There is not table with this name in buffer.");

		this->process(index);
		this->del();
		return 1;
	}
private:
	void process(int& index) {
		(*tb).removeTable(index);
	}

	void del() {
		if (remove((matches[1].str() + ".bin").c_str()))
			cout << endl << "There is no file with this name on the disk.";
	}
};

class DropIndex : public DeleteOperation {
public:
	DropIndex() {
		this->cmdRgx.assign("^\s*DROP\s+INDEX\s+([a-zA-Z0-9]+)\s*$");
		this->partialRgx.assign("^\s*DROP\s+INDEX\s*");
		this->counter++;
	}

	bool check() {
		if (!regex_search(this->input, this->partialRgx))
			return 0;

		if (!regex_search(this->input, this->matches, this->cmdRgx)) {
			cout << endl << pf.guideline["DROP INDEX"];
			return 0;
		}

		this->del();
		this->process();
		return 1;
	}
private:
	void process() {
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

	void del() {
		if (remove((matches[1].str() + ".idx").c_str()))
			cout << endl << "There is no index with this name on the disk.";
	}
};


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


	/*
	La toate comenzile in loc sa le procesez in table
	ar trebui sa folosesc getters
	si sa dau replace in buffer dupa
	*/
	void createTable(smatch matches, TableBuffer& tb) {
		regex partitionRegex("[^ ,()][a-zA-Z0-9\"'”’\\s*]*");

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

	void createIndex(smatch matches, TableBuffer& tb) {

		//cout << endl << matches[1].str(); // if not exists
		//cout << endl << matches[2].str(); // index name
		//cout << endl << matches[3].str(); // table name
		//cout << endl << matches[4].str(); // column name

		if (matches[1].str() != "") {
			// Check if it is already an index there with the same name
			fstream file(matches[1].str() + ".idx");
			if (file.is_open()) {
				cout << endl << "There is already an index with the same name";
				file.close();
				return;
			}
		}

		// We have to get the offset for every row.
		loadTableIfNecessary(matches[3].str(), tb);
		Table t = tb.getTable(tb.isTable(matches[3].str()));

		int* offsets = nullptr;
		int noOffsets = 0;
		int colIndex = t.isColumn(matches[4].str());
		if (colIndex == -1) {
			throw exception("Column not found.");
		}
		Column col = t.getColumn(colIndex);
		offsets = t.getOffsets(matches[4].str(), noOffsets);
		col.setIndex(matches[2].str(), offsets, noOffsets);

		t.setColumn(col, colIndex);
		tb.replaceTable(t);

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

	void dropIndex(smatch matches, TableBuffer& tb) {
		// delete from files first
		if (remove((matches[1].str() + ".idx").c_str()))
			cout << endl << "There is no index with this name on the disk.";

		int noTables = tb.getNoTables();
		for (int i = 0; i < noTables; i++) {
			Table t = tb.getTable(i);
			int noCols = t.getNoColumns();
			for (int j = 0; j < noCols; j++) {
				Column col = t.getColumn(j);
				Index colIndex = col.getIndex();
				if (colIndex.getName() == matches[1].str()) {
					col.setIndex("", nullptr, 0);
					t.setColumn(col, j);
					tb.replaceTable(t);
					return;
				}
			}
		}
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
				Table temp = t.filterTableSpecificColumns(colNamesToBePrinted, noCols); // rename this..
				cout << temp;
				generateExtensions(temp);
			}
			else {
				// specific columns with where
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
				Table temp = t.filterTableWithWhereClauseAndSpecificColumns(colNamesToBePrinted, noCols, whereColumn, valueToMatch); // rename this..
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