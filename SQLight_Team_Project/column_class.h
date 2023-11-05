#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "utils.h"
using namespace std;

enum columnTypes { NUMBER, STRING };

class Column {
private:
	string columnName = "";
	columnTypes type = columnTypes::NUMBER;
	size_t size = 0;
	// aici daca am putea sa scapam de una de care nu avem nevoie ar fi super
	float numDefaultValue = 0;
	string strDefaultValue = "";
	// default value is created based on the type
public:

	void setDefaultValue(string input) {
		if (this->type == columnTypes::STRING) {
			this->strDefaultValue = input;
		}
		else {
			throw exception("Wrong input. Type is number.");
		}
	}

	void setDefaultValue(float input) {
		if (this->type == columnTypes::NUMBER) {
			this->numDefaultValue = input;
		}
		else {
			throw exception("Wrong input. Type is string.");
		}
	}

	void setColumnName(string input) {
		// validari aici
		// e ok asa pentru ca e string si nu char* deci nu facem shallow copy
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

	float getNumDefaultValue() {
		return this->numDefaultValue;
	}

	string getStrDefaultValue() {
		return this->strDefaultValue;
	}
};

void operator<<(ostream& console, Column col) {
	console << endl << "~~~~~~~~~~~~~~~~";
	console << endl << "Column name: " << col.getColumnName();
	console << endl << "Type: " << col.getType();
	console << endl << "Size: " << col.getSize();

	if (col.getType() == columnTypes::NUMBER) {
		console << endl << "Default: " << col.getNumDefaultValue();
	}
	else {
		console << endl << "Default: " << col.getStrDefaultValue();
	}
}