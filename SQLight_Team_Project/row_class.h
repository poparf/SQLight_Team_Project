#pragma once
#include <iostream>
#include <string>
#include "table_class.h"
#include "column_class.h"
using namespace std;


// nu functioneaza asa


//
//class Row {
//private:
//	string* data = nullptr;
//
//	// mai gandeste te
//	int noCols = 0;
//public:
//
//	Row() {};
//	void insertData(string* input) {
//		// verificam lungimea prin regex deci nu mai e nevoie de validare aici
//		delete[] data;
//		this->data = new string[noCols];
//
//		for (int i = 0; i < noCols; i++) {
//			this->data[i] = input[i];
//		}
//	}
//
//	//  constructor
//	Row() {
//
//		// initializam valorile cu default value
//		int noCols = table.getNoColumns();
//
//		this->data = new string[noCols];
//
//		Column* cols = table.getColumns();
//		for (int i = 0; i < noCols; i++) {
//			data[i] = cols[i].getDefaultValue();
//		}
//
//		delete[] cols;
//	}
//
//	//copy constructor
//	Row(Row& right) {
//		this->noCols = right.noCols;
//		delete[] data;
//		this->data = new string[noCols];
//
//		for (int i = 0; i < noCols; i++) {
//			this->data[i] = right.data[i];
//		}
//	}
//
//	Row& operator=(Row& right) {
//		this->noCols = right.noCols;
//		delete[] data;
//		this->data = new string[noCols];
//
//		for (int i = 0; i < noCols; i++) {
//			this->data[i] = right.data[i];
//		}
//
//		return *this;
//	}
//
//	// destructor
//	~Row() {
//		delete[] data;
//	}
//};
