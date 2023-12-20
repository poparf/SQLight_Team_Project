#pragma once
#include "utils.h"
#include "column_class.h"

using namespace std;

class Table {
private:
	string name = "";
	Column* columns = nullptr;
	int noColumns = 0;

	string** data = nullptr;
	int noRows = 0;
	int nextRow = 0;
public:

	void setColumns(Column* inputCols, int inputNoCols) {
		Column* newColumns = new Column[inputNoCols];

		for (int i = 0; i < inputNoCols; i++)
			newColumns[i] = inputCols[i];

		if (this->columns != nullptr)
			delete[] this->columns;

		this->columns = newColumns;
		this->noColumns = inputNoCols;
	}
	void setData(string** inputData, int noRows, int nextRow, int noColumns) {
		
		this->nextRow = nextRow;
		this->setNoColumns(noColumns);
		

		if (this->data != nullptr) {
			for (int i = 0; i < this->noRows; i++) {
				delete[] this->data[i];
			}
			delete[] this->data;
		}
		this->noRows = noRows;
		this->nextRow = nextRow;

		string** newData;
		newData = new string * [noRows];
		for (int i = 0; i < noRows; i++) {
			newData[i] = new string[noColumns];
		}

		for (int i = 0; i < noRows; i++) {
			for (int j = 0; j < noColumns; j++) {
				newData[i][j] = inputData[i][j];
			}
		}

		this->data = newData;
	}

	// Convention to write into the file as follows:
		// no of columns
		// no of rows
		// Column i
		//   - length of col name
		//   - columnName
		//   - columnType
		//   - max size of the column
		//   - length of defaultValue
		//   - defaultValue
		// ABOVE REPEATS AS MANY COLUMNS AS WE HAVE..
		// 
		// Row 1  the number of cells is equal to the no of columns
		//	- size of cell string
		//	- string in the cell
	
	void setNoRows(int noRows) {
		if (noRows < 0)
			throw exception("Negativen no rows");

		this->noRows = noRows;
	}

	void setNextRowForFile() {
		this->nextRow = noRows;
	}

	

	Table() {
		this->noRows = 100;
		this->data = new string*[this->noRows];


		for (int i = 0; i < this->noRows; i++)
			this->data[i] = new string[this->noColumns];
	}

	Table(string inputName, Column* inputColumns, int inputNoColumns) {
		this->columns = new Column[inputNoColumns];

		this->setNoColumns(inputNoColumns);
		for (int i = 0; i < this->noColumns; i++) {
			this->columns[i] = inputColumns[i];
		}

		this->setName(inputName);

		this->noRows = 100;
		this->data = new string*[this->noRows];
		

		for (int i = 0; i < this->noRows; i++)
			this->data[i] = new string[this->noColumns];
	}


	Table(string inputName, Column* inputColumns, int inputNoColumns, int noRows) {
		this->columns = new Column[inputNoColumns];

		this->setNoColumns(inputNoColumns);
		for (int i = 0; i < this->noColumns; i++) {
			this->columns[i] = inputColumns[i];
		}

		this->setName(inputName);

		this->noRows = noRows;
		this->data = new string * [this->noRows];


		for (int i = 0; i < this->noRows; i++)
			this->data[i] = new string[this->noColumns];
	}

	void insertRow(string* input) {
	
		if (this->nextRow == this->noRows) {
			this->doubleSpace();
		}

		for (int i = 0; i < this->noColumns; i++) {
			this->data[this->nextRow][i] = input[i];
		}
		
		this->nextRow += 1;

	}

	string getName() {
		return this->name;
	}

	int getNextRow() {
		return this->nextRow;
	}

	string** getData() {
		// trimited doar nr real de data
		int rows = this->nextRow - 1;
		string** newData = new string*[rows];

		for (int i = 0; i < rows; i++) {
			newData[i] = new string[this->noColumns];
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < this->noColumns; j++) {
				newData[i][j] = this->data[i][j];
			}
		}

		return newData;
	}

	int getNoColumns() {
		return this->noColumns;
	}

	void setName(string input) {
		// validare: daca exista un table cu acelasi dam throw
		this->name = input;
	}

	void setNoColumns(int input) {
		if (input < 0) {
			throw exception("Number of columns must be positive.");
		}
		this->noColumns = input;
	}


	Column* getColumns() {
		Column* newColumns = new Column[this->noColumns];

		for (int i = 0; i < this->noColumns; i++) {
			newColumns[i] = this->columns[i];
		}

		return newColumns;
	}

	void operator=(const Table& table) {
		
		if (&table == this) {
			return;
		}
		
		this->name = table.name;

		Column* newCols = new Column[table.noColumns];
		for (int i = 0; i < table.noColumns; i++) {
			newCols[i] = table.columns[i];
		}
		delete[] this->columns;
		this->columns = newCols;

		this->noColumns = table.noColumns;


		for (int i = 0; i < this->noRows; i++) {
			delete[] this->data[i];
		}
		delete[] this->data;

		this->noRows = table.noRows;
		this->nextRow = table.nextRow;


		data = new string * [table.noRows];
		for (int i = 0; i < table.noRows; i++) {
			data[i] = new string[table.noColumns];
		}

		for (int i = 0; i < table.noRows; i++) {
			for (int j = 0; j < table.noColumns; j++) {
				data[i][j] = table.data[i][j];
			}
		}
	}

	int getNoRows() {
		return this->noRows;
	}

	Table(const Table& table) {
		this->name = table.name;
		
		Column* newCols = new Column[table.noColumns];
		for (int i = 0; i < table.noColumns; i++) {
			newCols[i] = table.columns[i];
		}
		this->columns = newCols;

		this->noColumns = table.noColumns;

		this->noRows = table.noRows;
		this->nextRow = table.nextRow;

		data = new string * [table.noRows];
		for (int i = 0; i < table.noRows; i++) {
			data[i] = new string[table.noColumns];
		}

		for (int i = 0; i < table.noRows; i++) {
			for (int j = 0; j < table.noColumns; j++) {
				data[i][j] = table.data[i][j];
			}
		}
	}

	~Table() {
		delete[] this->columns;

		for (int i = 0; i < this->noRows; i++) {
			delete[] this->data[i];
		}
		delete[] this->data;
	}

	friend void operator<<(ostream& console, Table t);
private:
	void doubleSpace() {
		if (this->noRows == 0) {
			string** newData = new string * [2];

			for (int i = 0; i < 2;  i++) {
				newData[i] = new string[this->noColumns];
			}
			delete[] this->data;

			this->nextRow = 0;
			this->noRows = 2;

			this->data = newData;
			return;
		}
		string** newData = new string * [this->noRows * 2];
		
		for (int i = 0; i < this->noRows * 2; i++) {
			newData[i] = new string[this->noColumns];
		}

		for (int i = 0; i < this->noRows; i++) {
			for (int j = 0; j < this->noColumns; j++) {
				newData[i][j] = this->data[i][j];
			}
		}

		for (int i = 0; i < this->noRows; i++) {
			delete[] this->data[i];
		}
		delete[] this->data;
		
		this->nextRow = this->noRows + 1;
		this->noRows *= 2;

		this->data = newData;
	}
};

void operator<<(ostream& console, Table t) {

	console << endl;
	Column* columns = t.getColumns();
	console << " ";
	for (int i = 0; i < t.noColumns; i++) {
		console << columns[i].getColumnName();
		console << " ";
	}
	console << endl;
	for (int i = 0; i < t.noColumns; i++) {
		console << " ";
		for (int j = 0; j < columns[i].getColumnName().length(); j++) {
			console << "-";
		}
	}

	for (int i = 0; i < t.nextRow; i++) {
		cout << "\n";
		console << " ";
		for (int j = 0; j < t.noColumns; j++) {
			console << t.data[i][j] << " ";
		}
	}
	delete[] columns;
}


class TableBuffer {
	Table* tables = nullptr;
	int noTables = 0;
public:


	Table operator[](string name) {
		for (int i = 0; i < this->noTables; i++) {
			if (tables[i].getName() == name) {
				return tables[i];
			}
		}
	}

	/*
	* whne a command is inserted
	* we should check if we have a file associated with the table name+
	* if yes then we read it in the tablebuffer
	* if not we return error
	* 
	* then we process the command inside the buffer and replace the old file 
	*/

	void remove(string tableName) {
		if (!this->doesTableExist(tableName)) {
			throw exception("Table does not exist.");
		}

		this->noTables -= 1;
		Table* newTables = new Table[this->noTables];
		
		int j = 0;
		for (int i = 0; i < this->noTables + 1; i++) {
			if (this->tables[i].getName() == tableName) {
				i++; // just skip the table..
			}
			else {
				newTables[j++] = this->tables[i];
			}
		}

		delete[] this->tables;

		this->tables = newTables;
	}

	// it searches if the table exists in files if yes it returns it
	Table searchTable(string fileName) {
		
	}

	TableBuffer() {

	}


	// check also in files
	bool doesTableExist(string input) {
		for (int i = 0; i < noTables; i++) {
			if (tables[i].getName() == input) {
				cout << endl << "The table exists in buffer.";
				return 1;
			}
		}

		ifstream temp(input + ".bin");
		if (temp.is_open()) {
			cout << endl << "The table exists in a file.";
			cout << endl << "Load ?";
			return 1;
		}
	
		return 0;
		
	}

	void insertRowByName(string* data, string tableName) {
		for (int i = 0; i < this->noTables; i++) {
			if (this->tables[i].getName() == tableName) {
				this->tables[i].insertRow(data);
				break;
			}
		}
	}

	Table* getTables() {
		Table* newTables = new Table[this->noTables];
		for (int i = 0; i < this->noTables; i++) {
			newTables[i] = this->tables[i];
		}
		return newTables;
	}

	int getNoTables() {
		return this->noTables;
	}

	TableBuffer(const TableBuffer& tableBuffer) {
		this->noTables = tableBuffer.noTables; 
		this->tables = new Table[tableBuffer.noTables];
		for (int i = 0; i < tableBuffer.noTables; i++) {
			this->tables[i] = tableBuffer.tables[i];
		}
	}

	void insertTable(const Table& input) {
		Table* newTables = new Table[this->noTables + 1];

		for (int i = 0; i < this->noTables; i++) {
			newTables[i] = this->tables[i];
		}

		newTables[this->noTables] = input;

		this->noTables += 1;

		delete[] this->tables;

		this->tables = newTables;
	}

	TableBuffer operator+(const Table& right) {

		TableBuffer copy = *this;
		copy.insertTable(right);
		
		return copy;
	}

	void operator=(const TableBuffer& right) {
		if (this == &right) {
			return;
		}
		delete[] this->tables;
		this->tables = new Table[right.noTables];
		for (int i = 0; i < right.noTables; i++) {
			this->tables[i] = right.tables[i];
		}
		this->noTables = right.noTables;

	}

	~TableBuffer() {
		delete[] this->tables;
	}
};