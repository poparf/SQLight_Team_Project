#pragma once
#include "Table.h"

class TableBuffer {
protected:
	Table** tables = nullptr;
	int noTables = 0;

	static TableBuffer* instance;
	TableBuffer() {}
public:

	static TableBuffer* getInstance() {
		if (instance == nullptr)
			instance = new TableBuffer();
		
		return instance;
	}


	int getNoTables() {
		return this->noTables;
	}

	void addTable(Table t) {

		Table** newTables = new Table * [this->noTables + 1];

		for (int i = 0; i < this->noTables; i++) {
			newTables[i] = new Table(*this->tables[i]);
		}

		newTables[this->noTables] = new Table(t);

		this->deleteTables();

		this->noTables += 1;
		this->tables = newTables;
	}

	void removeTable(int index) {
			if (index < 0 || index >= this->noTables) {
				throw exception("Invalid row index");
			}


			for (int i = index; i < this->noTables - 1; i++) {
				*this->tables[i] = *this->tables[i + 1];
			}

			this->tables[this->noTables - 1] = nullptr;
			this->noTables--;
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
		if (index >= this->noTables || index < 0) {
			throw exception("Index is out of range.");
		}

		return (*this->tables[index]);
	}


	// Replaces a table in the TableBuffer with another one
	void setTable(Table table, int index) {
		if (index >= this->noTables || index < 0) {
			throw exception("The index is out of range.");
		}

		(*this->tables[index]) = table;
	}

	// Replaces the table in the buffer with the same name with the table input
	void replaceTable(Table table) {
		for (int i = 0; i < this->noTables; i++) {
			if (this->tables[i]->getName() == table.getName()) {
				*this->tables[i] = table;
				return;
			}
		}

		throw exception("There is no other table with this name so you cannot replace it.");
	}

	// No copy constructor or operator= needed
	TableBuffer(TableBuffer& tb) = delete;
	void operator=(const TableBuffer&) = delete;

	~TableBuffer() {
		for (int i = 0; i < this->noTables; i++) {
			delete this->tables[i];
		}
		delete[] this->tables;
	}

	friend void operator<<(ostream& out, TableBuffer& tb);
private:

	void deleteTables() {
		if (this->tables != nullptr) {
			for (int i = 0; i < this->noTables; i++) {
				if (this->tables[i] != nullptr)
					delete this->tables[i];
			}
			delete[] this->tables;
		}
	}
};

void operator<<(ostream& out, TableBuffer& tb) {
	out << endl << "~~~~~~~~~~~~~~~" << endl;
	for (int i = 0; i < tb.noTables; i++) {
		out << tb.tables[i]->getName() << endl;
	}
	out << endl << "~~~~~~~~~~~~~~~" << endl;
}

TableBuffer* TableBuffer::instance = nullptr;