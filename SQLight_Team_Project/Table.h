#pragma once
#include "Column.h"
#include "Row.h"

using namespace std;

class Table {
protected:
	string name = "";

	Column** columns = nullptr;
	int noColumns = 0;

	Row** rows = nullptr;
	int noRows = 0;
public:
	// Constructor with arguments
	Table(string tableName, Column** columns, int noColumns) {
		this->setColumns(columns, noColumns);
		this->name = tableName;
	}

	// Copy constructor
	Table(const Table& t) {
		this->name = t.name;
		this->setRows(t.rows, t.noRows);
		this->setColumns(t.columns, t.noColumns);
	}

	// Operator = overloaded
	Table operator=(const Table& t) {
		if (&t == this)
			return t;

		this->name = t.name;
		this->setRows(t.rows, t.noRows);
		this->setColumns(t.columns, t.noColumns);

		return t;
	}

	void addManyRows(Row** rows, int noRows) {
		Row** newRows = new Row * [this->noRows + noRows];

		for (int i = 0; i < this->noRows; i++) {
			newRows[i] = new Row(*this->rows[i]);
		}

		for (int i = 0; i < noRows; i++) {
			
			if ((*rows[i]).getNoCells() > this->noColumns) {
				throw exception("You can't add more cells that the number of columns.");
			}

			newRows[this->noRows + i] = new Row(*rows[i]);
		}

		this->deleteRows();

		this->noRows = this->noRows + noRows;
		this->rows = newRows;
	}

	void addRow(Row row) {
		if (row.getNoCells() > this->noColumns) {
			throw exception("You can't add more cells that the number of columns.");
		}

		Row** newRows = new Row * [this->noRows + 1];

		for (int i = 0; i < this->noRows; i++) {
			newRows[i] = new Row(*this->rows[i]);
		}

		newRows[this->noRows] = new Row(row);

		this->deleteRows();

		this->noRows += 1;
		this->rows = newRows;
	}

	string getName() {
		return this->name;
	}

	int getNoRows() {
		return this->noRows;
	}

	// Destructor
	~Table() {
		for (int i = 0; i < this->noColumns; i++) {
			delete this->columns[i];
		}

		delete[] this->columns;

		for (int i = 0; i < this->noRows; i++) {
			delete this->rows[i];
		}

		delete[] this->rows;
	}

	friend void operator<<(ostream& out, Table& table);
	friend class outTable;
	friend class inTable;
private:

	void deleteRows() {
		if (this->rows != nullptr) {
			for (int i = 0; i < this->noRows; i++) {
				if (this->rows[i] != nullptr)
					delete this->rows[i];
			}
			delete[] this->rows;
		}
	}

	void setColumns(Column** columns, int noColumns) {
		if (noColumns == 0) {
			return;
		}

		Column** newCols = new Column * [noColumns];

		for (int i = 0; i < noColumns; i++) {
			newCols[i] = new Column(*columns[i]);
		}
		
		if (this->columns != nullptr) {
			for (int i = 0; i < this->noColumns; i++) {
				if (this->columns[i] != nullptr)
					delete this->columns[i];
			}
			delete[] this->columns;
		}

		this->columns = newCols;
		this->noColumns = noColumns;
	}


	void setRows(Row** rows, int noRows) {
		if (noRows == 0) {
			return;
		}

		Row** newRows = new Row * [noRows];

		for (int i = 0; i < noRows; i++) {
			newRows[i] = new Row(*rows[i]);
		}

		if (this->rows != nullptr) {
			for (int i = 0; i < this->noRows; i++) {
				if (this->rows[i] != nullptr)
					delete this->rows[i];
			}
			delete[] this->rows;
		}

		this->rows = newRows;
		this->noRows = noRows;
	}
};



void operator<<(ostream& out, Table& table) {
	cout << endl << table.name << endl;
	
	cout << endl << "Columns:" << endl;

	for (int i = 0; i < table.noColumns; i++) {
		cout << table.columns[i]->getName() << " ";
	}

	cout << endl << "Rows:" << endl;

	for (int i = 0; i < table.noRows; i++) {
		cout << *table.rows[i];
	}


}