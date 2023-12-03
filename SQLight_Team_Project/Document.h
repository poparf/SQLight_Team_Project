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
	// ce pun aici ?
	
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
		outFile.write((char*)noCols, sizeof(int));
		outFile.write((char*)noRows, sizeof(int));

		for (int i = 0; i < noCols; i++) {
			// ColumnName type string
			string colName = inputCols[i].getColumnName();
			size_t colNameSize = colName.size();
			outFile.write((char*)colNameSize, sizeof(colNameSize));
			outFile.write(colName.c_str(), colNameSize);
			
			// Type of the column written as int INT  - 0 FLOAT - 1 TEXT -2
			int colType = inputCols[i].getType();
			outFile.write((char*)colType, sizeof(int));

			// Maximum size of the column
			size_t colMaxSize = inputCols[i].getSize();
			outFile.write((char*)colMaxSize, sizeof(colMaxSize));

			// The default value
			string colDefValue = inputCols[i].getDefaultValue();
			int defSize = colDefValue.size();
			outFile.write((char*)defSize, sizeof(defSize));
			outFile.write(colDefValue.c_str(), defSize);
		}
	}

	// Folosita la insert
	// Chiar daca fac amortizarea ar trebui sa scriu in fisier
	// numarul real de rows. adica la fiecare insert sa apelez
	// functia aceasta:
	void writeRows(Table& writeTable) {
		// Question: Daca am pentru tabel 
		// un size maximum pt fiecare string salvat
		// ar trebui sa salvez fiecare string
		// cu size ul default chiar daca are mai putin ?
		// Pot face cumva ca sa maresc viteaza
		// sa nu trimit de fiecare data o copie?
		// sa am acces la data dar sa nu pot schimba
		// poate daca am tine matricea cu data in
		// aceasta clasa si astfel apelam getData()
		// doar o data in constructor
	}
};



// Alte intrebari:
// De ce da crash cand scriu peste 100 de rows.
// Este de la functia doubleSpace? Este copy constructor sau operator =?
