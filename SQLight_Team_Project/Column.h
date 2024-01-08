#pragma once
#include "Index.h"
using namespace std;

enum columnTypes { INTEGER, FLOAT, TEXT };

class Column {
private:
	string name;
	columnTypes type = columnTypes::TEXT;
	int size = 0;
	string def = "";

	Index index;
public:

	Column(string name, columnTypes type, int size, string def) {
		this->setName(name);
		this->setType(type);
		this->setSize(size);
		this->setDef(def);
	}

	void setIndex(string name, int* offsets, int noOffsets) {
		this->index.setOffsets(offsets, noOffsets);
		this->index.setName(name);
		this->index.writeIndex();
	}

	Index getIndex() {
		return this->index;
	}

	void setDef(string input) {
		this->def = input;
	}

	void setName(string input) {
		this->name = input;
	}

	void setType(columnTypes type) {
		this->type = type;
	}

	void setSize(int input) {
		if (input < 0) {
			throw exception("The column maximum size must be positive.");
		}
		this->size = input;
	}

	string getName() {
		return this->name;
	}

	int getSize() {
		return this->size;
	}

	columnTypes getType() {
		return this->type;
	}

	string getDef() {
		return this->def;
	}
};

void operator<<(ostream& console, Column col) {
	console << endl << "Column name: " << col.getName();
	console << endl << "Type: " << col.getType();
	console << endl << "Size: " << col.getSize();
	console << endl << "Default: " << col.getDef();
	console << endl << "~~~~~~~~~~~~~~~~";
}