#pragma once
#include <iostream>
#include <string>
#include "utils.h"

using namespace std;

enum columnTypes { INTEGER, FLOAT, TEXT };

class Column {
private:
	string columnName = "";
	columnTypes type = columnTypes::TEXT;
	int size = 0;
	string defaultValue = "";
public:

	void setDefaultValue(string input) {
		this->defaultValue = input;
	}

	void setColumnName(string input) {
		this->columnName = input;
	}

	void setType(columnTypes type) {
		this->type = type;
	}

	void setSize(int input) {
		this->size = input;
	}

	string getColumnName() {
		return this->columnName;
	}

	int getSize() {
		return this->size;
	}

	columnTypes getType() {
		return this->type;
	}

	string getDefaultValue() {
		return this->defaultValue;
	}
};

void operator<<(ostream& console, Column col) {
	console << endl << "Column name: " << col.getColumnName();
	console << endl << "Type: " << col.getType();
	console << endl << "Size: " << col.getSize();
	console << endl << "Default: " << col.getDefaultValue();
	console << endl << "~~~~~~~~~~~~~~~~";
}