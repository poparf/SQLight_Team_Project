#pragma once
#include <iostream>
#include <string>
#include "utils.h"

using namespace std;

enum columnTypes { NUMBER, STRING };

class Column {
private:
	string columnName = "";
	columnTypes type = columnTypes::NUMBER;
	size_t size = 0;
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

	void setSize(size_t input) {
		this->size = input;
	}

	string getColumnName() {
		return this->columnName;
	}

	size_t getSize() {
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