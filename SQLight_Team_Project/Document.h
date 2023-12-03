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

	void writeTable(Table writeTable) {
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

			//

		}
	}
};

