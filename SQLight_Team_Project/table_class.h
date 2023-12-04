#pragma once
#include <iostream>
#include <string>
#include "utils.h"
#include "column_class.h"

using namespace std;

class Table {
private:
	string name = "";
	Column* columns = nullptr;
	int noColumns = 0;

	// Question: Este ok asa ? 
	// Am incercat sa fac cu o clasa Row dar depindea de
	// informatii din clasa Column.
	string** data = nullptr;
	int noRows = 0;
	int rowsAvailable = 0;
	int nextRow = 0;
public:
	Table() {
		this->noRows = 100;
		this->data = new string*[this->noRows];
		this->rowsAvailable = 100;

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
		this->rowsAvailable = 100;

		for (int i = 0; i < this->noRows; i++)
			this->data[i] = new string[this->noColumns];
	}

	void insertRow(string* input) {
		if (this->rowsAvailable == 0) {
			this->doubleSpace();
		}
		for (int i = 0; i < this->noColumns; i++) {
			this->data[this->nextRow][i] = input[i];
		}
		
		this->nextRow += 1;
		this->rowsAvailable -= 1;
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
		this->name = table.name;

		Column* newCols = new Column[table.noColumns];
		for (int i = 0; i < table.noColumns; i++) {
			newCols[i] = table.columns[i];
		}
		delete[] this->columns;
		this->columns = newCols;

		this->noColumns = table.noColumns;


		this->noRows = table.noRows;
		this->rowsAvailable = table.rowsAvailable;
		this->nextRow = table.nextRow;

		for (int i = 0; i < this->noRows; i++) {
			delete[] this->data[i];
		}
		delete[] this->data;

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
		this->rowsAvailable = table.rowsAvailable;
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
		this->rowsAvailable = this->noRows / 2;

		for (int i = 0; i < this->noRows; i++) {
			this->data[i] = newData[i];
		}
		this->data = newData;

	}
};

void operator<<(ostream& console, Table t) {
	console << endl << "~~~~ " << t.getName() << " ~~~~";
	console << endl << "~~ No. of cols: " << t.getNoColumns();
	console << endl << "~~~~~~~~~~~~~~~~";
	Column* columns = t.getColumns();
	for (int i = 0; i < t.getNoColumns(); i++) {
		console << columns[i];
	}

	cout << endl << "~~~~~~~~~~~~~~~~";
	cout << endl << "~~~~ DATA ~~~~~~";
	cout << endl << "~~~~~~~~~~~~~~~~";

	for (int i = 0; i < t.nextRow; i++) {
		cout << "\n";
		for (int j = 0; j < t.noColumns; j++) {
			cout << t.data[i][j] << " ";
		}
	}
	delete[] columns;
}


class TableBuffer {
	Table* tables = nullptr;
	int noTables = 0;
public:
	TableBuffer() {

	}

	bool doesTableExist(string input) {
		for (int i = 0; i < noTables; i++) {
			if (tables[i].getName() == input) {
				return 1;
			}
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