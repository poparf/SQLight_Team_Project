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

	// Returns the index of the column if exists else returns -1
	int isColumn(string name) {
		for (int i = 0; i < this->noColumns; i++) {
			if ((*this->columns[i]).getName() == name)
				return i;
		}
		return -1;
	}

	void deleteFrom(string colName, string valSearched) {
		int colIndex = this->isColumn(colName);
		if (colIndex == -1) {
			throw exception("There is no column with that name.");
		}
		// Iteram prin randuri tinand cont ca
		// nr de articole dintr-un rand este egal cu nr de
		// coloane, stim index-ul coloanei stim si indexul articolului

		int* rowsToDelete = new int[this->noRows];

		int rtdActualSize = 0;
		for (int i = 0; i < this->noRows; i++) {
			int noCells = this->rows[i]->getNoCells();
			Article** cells = this->rows[i]->getCells();

			string cellValue = cells[colIndex]->getData();
			if (cellValue == valSearched) {
				rowsToDelete[rtdActualSize++] = i;
			}

			if (cells != nullptr) {
				for (int j = 0; j < noCells; j++) {
					if (cells[j] != nullptr)
						delete cells[j];
				}
				delete[] cells;
			}
		}

		// Now we have the indexes of the rows to delete
		for (int i = 0; i < rtdActualSize; i++) {
			this->deleteRowByIndex(rowsToDelete[i] - i);
		}

		cout << endl << rtdActualSize << " rows deleted.";
		delete[] rowsToDelete;
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

	void printTableWithWhereClause(string colName, string value) {
		
		int colIndex = this->isColumn(colName);
		if (colIndex == -1) {
			throw exception("There is no column with this name.");
		}
		cout << endl << this->name << endl;

		cout << "Columns:" << endl;
		for (int i = 0; i < this->noColumns; i++) {
			cout << this->columns[i]->getName() << " ";
		}
		cout << endl << "Rows:" << endl;

		for (int i = 0; i < this->noRows; i++) {
			int noCells = this->rows[i]->getNoCells();
			Article * *cells = this->rows[i]->getCells();

			if (cells[colIndex]->getData() == value) {
				for (int j = 0; j < noCells; j++) {
					cout << cells[j]->getData() << " ";
				}
				cout << endl;
			}
		}
	}

	void printTableSpecificColumns(string* colNamesToBePrinted, int noCols) {
		int* colIndexes = new int[noCols];

		for (int i = 0; i < noCols; i++) {
			colIndexes[i] = this->isColumn(colNamesToBePrinted[i]);
			if (colIndexes[i] == -1) {
				cout << endl << "Column " << colNamesToBePrinted[i] << " does not exist." << endl;
			}
		}

		cout << endl << "Columns:" << endl;
		for (int i = 0; i < noCols; i++) {
			if (colIndexes[i] != -1) {
				cout << colNamesToBePrinted[i] << " ";
			}
		}

		cout << endl << "Rows:";

		for (int i = 0; i < this->noRows; i++) {
			cout << endl;
			
			int noCells = this->rows[i]->getNoCells();
			Article** cells = this->rows[i]->getCells();
			for (int j = 0; j < noCols; j++) {
				cout << cells[colIndexes[j]]->getData() << " ";
			}
		}

	}

	void printTableWithWhereClauseAndSpecificColumns(string* colNamesToBePrinted, int noCols, string whereColumn, string valueToMatch) {
		int* colIndexes = new int[noCols];
		int colIndexToBeMatched = this->isColumn(whereColumn);
		if (colIndexToBeMatched == -1) {
			cout << endl << "Column in where clause doesn't exist." << endl;
			return;
		}
			
		for (int i = 0; i < noCols; i++) {
			colIndexes[i] = this->isColumn(colNamesToBePrinted[i]);
			if (colIndexes[i] == -1) {
				cout << endl << "Column " << colNamesToBePrinted[i] << " does not exist." << endl;
			}
		}

		cout << endl << "Columns:" << endl;
		for (int i = 0; i < noCols; i++) {
			if (colIndexes[i] != -1) {
				cout << colNamesToBePrinted[i] << " ";
			}
		}

		cout << endl << "Rows:";

		for (int i = 0; i < this->noRows; i++) {
			int noCells = this->rows[i]->getNoCells();
			Article** cells = this->rows[i]->getCells();

			
			if (cells[colIndexToBeMatched]->getData() == valueToMatch) {
				cout << endl;
				for (int j = 0; j < noCols; j++) {
					cout << cells[colIndexes[j]]->getData() << " ";
				}
			}	
		}
	}

	string getName() {
		return this->name;
	}

	int getNoRows() {
		return this->noRows;
	}

	void update(string setColName, string setValueName, string whereColName, string whereValueName, int whereColIndex, int setColIndex) {
		
		Row** newRows = new Row * [this->noRows];
		
		for (int i = 0; i < this->noRows; i++) {
			int noCells = this->rows[i]->getNoCells();
			Article** cells = this->rows[i]->getCells();

			if (cells[whereColIndex]->getData() == whereValueName) {
				cells[setColIndex]->setData(setValueName);
			}
			newRows[i] = new Row(cells, noCells);
		}

		this->deleteRows();
		this->rows = newRows;
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

	Table filterTableWithWhereClauseAndSpecificColumns(string* colNamesToBePrinted, int noCols, string whereColumn, string valueToMatch, int noGen) {
		int colIndexToBeMatched = isColumn(whereColumn);
		if (colIndexToBeMatched == -1) {
			throw exception("Wrong column name in the where clause.");
		}

		Column** newCols = new Column * [noCols];
		int actualSizeNewCols = 0;

		int* colIndexes = new int[noCols];
		int noColIndexes = 0;

		for (int i = 0; i < noCols; i++) {
			int colIndex = this->isColumn(colNamesToBePrinted[i]);
			if (colIndex != -1) {
				newCols[actualSizeNewCols++] = new Column(*columns[colIndex]);
				colIndexes[noColIndexes++] = colIndex;
			}
			else {
				cout << endl << "Column " << colNamesToBePrinted[i] << " does not exist." << endl;
			}
		}

		Table filteredTable(name + "_SELECT_" + to_string(noGen), newCols, actualSizeNewCols);

		for (int i = 0; i < noRows; i++) {
			Article** cells = rows[i]->getCells();
			int noCells = rows[i]->getNoCells();

			if (cells[colIndexToBeMatched]->getData() == valueToMatch) {
				Article** newCells = new Article * [noColIndexes];

				for (int j = 0; j < noColIndexes; j++) {
					newCells[j] = new Article(*cells[colIndexes[j]]);
				}

				Row newRow(newCells, noColIndexes);
				filteredTable.addRow(newRow); // Add matching rows with specific columns to the new table
			}

			for (int j = 0; j < noCells; j++) {
				delete cells[j];
			}
			delete[] cells;
		}

		return filteredTable;
	}


	Table filterTableWithWhereClause(string colName, string value, int noGen) {
		int colIndex = isColumn(colName);
		if (colIndex == -1) {
			throw exception("There is no column with this name.");
		}

		Table filteredTable(this->name + "_SELECT_" + to_string(noGen), this->columns, this->noColumns); // Create a new filtered table
	

		for (int i = 0; i < noRows; i++) {
			int noCells = rows[i]->getNoCells();
			Article** cells = rows[i]->getCells();
			if (cells[colIndex]->getData() == value) {
				Row r(cells, noCells);
				filteredTable.addRow(r);
			}

			for (int j = 0; j < noCells; j++)
				delete cells[j];
			delete[] cells;
		}

		return filteredTable;
	}

	Table filterTableSpecificColumns(string* colNamesToBePrinted, int noCols, int noGen) {
		//Table filteredTable(this->name + "_filtered", noCols, 0); // Create a new filtered table
		Column** newCols = new Column * [noCols];
		int actualSizeNewCols = 0;

		int* colIndexes = new int[noCols];
		int noColIndexes = 0;
		for (int i = 0; i < noCols; i++) {
			int colIndex = this->isColumn(colNamesToBePrinted[i]);
			if (colIndex != -1) {
				newCols[actualSizeNewCols++] = new Column(*columns[colIndex]);
				colIndexes[noColIndexes++] = colIndex;
			}
			else {
				cout << endl << "Column " << colNamesToBePrinted[i] << " does not exist." << endl;
			}
		}
		Table filteredTable(this->name + "_SELECT_" + to_string(noGen), columns, actualSizeNewCols);
		for (int i = 0; i < noRows; i++) {
			Article** cells = this->rows[i]->getCells();
			int noCells = this->rows[i]->getNoCells();
			Article** newCells = new Article * [noColIndexes];
			for (int j = 0; j < noColIndexes; j++) {
				newCells[j] = new Article(*cells[colIndexes[j]]);
			}

			Row newRow(newCells, noColIndexes);
			filteredTable.addRow(newRow); // Add rows with specific columns to the new table
		}

		return filteredTable;
	}

	






	friend void operator<<(ostream& out, Table& table);
	friend class outTable;
	friend class inTable;
	friend class xmlFile;
	friend class csvTable;
private:

	void deleteRowByIndex(int index) {
		if (index < 0 || index >= this->noRows) {
			throw new invalid_argument("Invalid row index");
		}


		for (int i = index; i < this->noRows - 1; i++) {
			*this->rows[i] = *this->rows[i + 1];
		}

		this->rows[this->noRows - 1] = nullptr;
		this->noRows--;
	}



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