#pragma once
#include <fstream>
#include <iostream>
#include "table_class.h"
using namespace std;

class Document
{
protected:
	string fileName;
	ifstream inFile;
	ofstream outFile;

public:
	Document(string fileName) : fileName(fileName) {

	
	}

	~Document() {
		inFile.close();
		outFile.close();
	}
};


class TxtDocument : public Document {

};


class CsvDocument : public Document {

};


class BinDocument : public Document {
private:

public:
	BinDocument(string binName) : Document(binName) {
		outFile.open(this->fileName, ios::app | ios::binary);
		if (!outFile.is_open()) {
			throw exception("File couldn't be opened.");
		}
	}
	

	// Folosita la create table
	void writeTable(Table& writeTable) {
		// Convention to write into the file as follows:
		// no of columns
		// no of rows
		// Column i
		//   - length of col name
		//   - columnName
		//   - columnType
		//   - max size of the column
		//   - length of defaultValue
		//   - defaultValue
		// ABOVE REPEATS AS MANY COLUMNS AS WE HAVE..
		// 
		// Row 1  the number of cells is equal to the no of columns
		//	- size of cell string
		//	- string in the cell
		//  
		Column* inputCols = writeTable.getColumns();
		int noCols = writeTable.getNoColumns();
		int noRows = writeTable.getNoRows();
		outFile.write((char*)&noCols, sizeof(int));
		outFile.write((char*)&noRows, sizeof(int));

		for (int i = 0; i < noCols; i++) {
			// ColumnName type string
			string colName = inputCols[i].getColumnName();
			int colNameSize = colName.size();
			outFile.write((char*)&colNameSize, sizeof(colNameSize));
			outFile.write(colName.c_str(), colNameSize);
			
			// Type of the column written as int INT  - 0 FLOAT - 1 TEXT -2
			int colType = inputCols[i].getType();
			outFile.write((char*)&colType, sizeof(int));

			// Maximum size of the column
			int colMaxSize = inputCols[i].getSize();
			outFile.write((char*)&colMaxSize, sizeof(colMaxSize));

			// The default value
			string colDefValue = inputCols[i].getDefaultValue();
			int defSize = colDefValue.size();
			outFile.write((char*)&defSize, sizeof(defSize));
			outFile.write(colDefValue.c_str(), defSize);
		}

		delete[] inputCols;
	}

	// Folosita la insert
	// Chiar daca fac amortizarea ar trebui sa scriu in fisier
	// numarul real de rows. adica la fiecare insert sa apelez
	// functia aceasta:
	void writeRows(string* data, Table& table) {
	
		Column* cols = table.getColumns();
		int noCols = table.getNoColumns();
		int defSize;

		for (int i = 0; i < noCols; i++) {
			defSize = cols[i].getSize();
			this->outFile.write(data[i].c_str(), defSize);
		}
		delete[] cols;
	}
};



// Intrebari
// Am inclus gresit fisierele ?
// Trebuia sa scriu doar declaratiile in header si functia in sine in cpp ?
// Ce ar trebui sa scriu in clasa parinte Document?
//  Este okay conventia pe care am ales o la fisierul binar ?
// Chiar daca fac amortizarea pot sa scriu in fisier numarul real de randuri nu ?
// Cand scriu data din tabel in fisier data de pe rows cu ce size ar trebui sa o pun ?
// Cu maximum size si sa scriu posibil biti in plus degeaba? altfel nu stiu cum as putea citi
// De ce da crash cand scriu peste 100 de rows.
// Este de la functia doubleSpace? Este copy constructor sau operator =?
// Cum sterg corect o matrice alocata dinamic ? e de ajuns delete[] matrix?
