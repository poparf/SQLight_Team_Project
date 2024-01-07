#include "Article.h"


class Row {
protected:
	Article** cells = nullptr;
	int noCells = 0;
public:

	// Constructor with argumnets
	Row(Article** cells, int noCells) {
		this->noCells = noCells;
		this->cells = new Article*[noCells];

		for (int i = 0; i < noCells; i++) {
			this->cells[i] = new Article((*cells[i]).getData());
		}
	}

	Article** getCells() {
		Article** newCells = new Article * [this->noCells];

		for (int i = 0; i < this->noCells; i++) {
			newCells[i] = new Article(*this->cells[i]);
		}

		return newCells;

	}

	int getNoCells() {
		return this->noCells;
	}


	// Copy Constructor
	Row(const Row& r) {
		this->noCells = r.noCells;

		Article** newCells = new Article * [r.noCells];

		for (int i = 0; i < r.noCells; i++) {
			newCells[i] = new Article(*r.cells[i]);
		}

		this->cells = newCells;
	}


	// Operator = overloaded
	Row operator=(const Row& r) {

		if (&r == this)
			return r;
		
		this->noCells = r.noCells;

		Article** newCells = new Article * [r.noCells];

		for (int i = 0; i < r.noCells; i++) {
			newCells[i] = new Article(*r.cells[i]);
		}

		this->cells = newCells;

		return r;
	}

	friend void operator<<(ostream& out, Row& r);

	// Destructor
	~Row() {
		for (int i = 0; i < this->noCells; i++) {
			delete this->cells[i];
		}

		delete[] this->cells;
	}
};


void operator<<(ostream& out, Row& r) {
	for (int i = 0; i < r.noCells; i++) {
		cout << *r.cells[i];
	}
	cout << endl;
}