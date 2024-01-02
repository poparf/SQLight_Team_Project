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


	// Copy Constructor
	Row(const Row& r) {
		this->noCells = r.noCells;

		Article** newCells = new Article * [r.noCells];

		for (int i = 0; i < r.noCells; i++) {
			*newCells[i] = *r.cells[i];
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
			*newCells[i] = *r.cells[i];
		}

		this->cells = newCells;

		return r;
	}

	
	// Destructor
	~Row() {
		for (int i = 0; i < this->noCells; i++) {
			delete this->cells[i];
		}

		delete[] this->cells;
	}
};