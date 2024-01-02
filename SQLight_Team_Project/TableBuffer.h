#pragma once
#include "Table.h"

class TableBuffer {
protected:
	Table** tables = nullptr;
	int noTables = 0;
public:

	TableBuffer() {}

	void addTable(Table t) {

		Table** newTables = new Table * [this->noTables + 1];

		for (int i = 0; i < this->noTables; i++) {
			*newTables[i] = *this->tables[i];
		}

		*newTables[this->noTables] = t;

		if (this->tables != nullptr) {
			for (int i = 0; i < this->noTables; i++) {
				if (this->tables[i] != nullptr)
					delete this->tables[i];
			}
			delete[] this->tables;
		}

		this->noTables += 1;
		this->tables = newTables;
	}


	// Returns the index of the table if exists else returns -1
	int isTable(string name) {
		for (int i = 0; i < this->noTables; i++) {
			if ((*this->tables[i]).getName() == name)
				return i;
		}
		return -1;
	}

	// Search for a table in the buffer by index and returns it, if out of range throws an error
 	Table getTable(int index) {
		if (index >= this->noTables) {
			throw exception("Index is out of range.");
		}

		return (*this->tables[index]);
	}

	// No copy constructor or operator= needed
	TableBuffer(TableBuffer& tb) = delete;
	void operator=(const TableBuffer&) = delete;

	~TableBuffer() {
		for (int i = 0; i < this->noTables; i++) {
			delete[] this->tables[i];
		}
		delete this->tables;
	}
};