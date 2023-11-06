#pragma once
#include <iostream>
#include <string>
#include "utils.h"
#include "column_class.h"

using namespace std;

class Table {
private:
	string name = " ";
	Column* columns = nullptr;
	int noColumns = 0;
public:
	Table(string inputName, Column* inputColumns, int inputNoColumns) {
		this->columns = new Column[inputNoColumns];

		this->setNoColumns(inputNoColumns);
		for (int i = 0; i < this->noColumns; i++) {
			this->columns[i] = inputColumns[i];
		}

		this->setName(inputName);
	}

	string getName() {
		return this->name;
	}

	int getNoColumns() {
		return this->noColumns;
	}

	void setName(string input) {
		// validare: daca exista un table cu acelasi dam throw
		this->name = input;
	}

	void setNoColumns(int input) {
		this->noColumns = input;
	}

	Column* getColumns() {
		Column* newColumns = new Column[this->noColumns];

		for (int i = 0; i < this->noColumns; i++) {
			newColumns[i] = this->columns[i];
		}

		return newColumns;
	}

	Table(Table& table) {
		this->name = table.getName();
		this->columns = table.getColumns();
		this->noColumns = table.getNoColumns();
	}

	~Table() {
		delete[] this->columns;
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
	delete[] columns;
}