#pragma once
#include <fstream>
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

		// aici in loc de noRows ar trebui sa fie ceva next row
		int noRows = writeTable.getNextRow();
		outFile.write((char*)&noCols, sizeof(int));
		outFile.write((char*)&noRows, sizeof(int));

		for (int i = 0; i < noCols; i++) {
			// ColumnName type string
			string colName = inputCols[i].getColumnName();
			int colNamesize = colName.size();
			outFile.write((char*)&colNamesize, sizeof(colNamesize));
			outFile.write(colName.c_str(), colNamesize);
			
			// Type of the column written as int INT  - 0 FLOAT - 1 TEXT -2
			int colType = inputCols[i].getType();
			outFile.write((char*)&colType, sizeof(int));

			// Maximum size of the column
			int colMaxsize = inputCols[i].getSize();
			outFile.write((char*)&colMaxsize, sizeof(colMaxsize));

			// The default value
			string colDefValue = inputCols[i].getDefaultValue();
			int defsize = colDefValue.size();
			outFile.write((char*)&defsize, sizeof(defsize));
			outFile.write(colDefValue.c_str(), defsize);
		}

		delete[] inputCols;
	}

	// Folosita la insert
	// Chiar daca fac amortizarea ar trebui sa scriu in fisier
	// numarul real de rows. adica la fiecare insert sa apelez
	// functia aceasta:
	void writeRows(string* data,Table table) {

		Column* cols = table.getColumns();
		int noCols = table.getNoColumns();
		int defsize;

		for (int i = 0; i < noCols; i++) {
			defsize = cols[i].getSize();
			this->outFile.write(data[i].c_str(), defsize);
		}
		delete[] cols;

		// pointeru e la finalul fisierului
		// seek la 4 bytes dupa
		// rescriem 4 bytes cu next row.
		long pos = outFile.tellp();
		this->outFile.seekp(4);
		int nextRow = table.getNextRow();
		this->outFile.write((char*)&nextRow, sizeof(int));
		this->outFile.seekp(pos);
	}
};

/*
PROBLEma e asa
noi scriem numarul de rows in fisier ( 100 )
chiar dacam am pus 100 noi in fisier punem doar rows urile existente

cum facem sa citim? 

fisier separat
- nr real de rows
	nota: la fiecare double space rescriem fisierul cu nr real de rows
- nr scris de row
	nota: 

	// atunci cand adaug un row ar trebui sa modific in fisier nr rows. si sa scriu nr real de rows
	// adica nr de rows scrise

*/





// the purpose of the class is to read a table from a file from the disk
class inTable {
protected:
	string name = "";
	ifstream file;
	Table fTable;
public:
	inTable(string fileName) {
		try {
			this->name = fileName;
			searchTable(fileName);

			readFileTable();
		}
		catch  (exception& e) {
			cout << endl << e.what();
		}

	}

	Table getTable() {
		return fTable;
	}

	// it searches if the table exists in files if yes it returns it
	void searchTable(string fileName) {
		this->file.open(fileName + ".bin");

		if (!this->file.is_open()) {
			throw exception("Invalid file name.");
		}
	}


private:
	void readFileTable() {
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
		//	- size oof defautl size
		//	- string in the cell
		// citim nr real de rows
		// facem o matrice cu nr de coloane si nr real de rows
		// next row va fi urmatorul rand la nr real de rows
		// astfel la urmatorul insert se apeleaza double space
	
		int noCols;
		file.read((char*)&noCols, sizeof(int));
		int noRows;
		file.read((char*)&noRows, sizeof(int));
		
		Column* columns = new Column[noCols];

		for (int i = 0; i < noCols; i++) {
			int lenColName;
			file.read((char*)&lenColName, sizeof(int));
		
			char* temp1 = new char[lenColName + 1];
			file.read(temp1, lenColName);
			temp1[lenColName] = '\0';
			string colName = temp1;
			delete[] temp1;

			columns[i].setColumnName(colName);

			columnTypes type;
			file.read((char*)&type, sizeof(type));
			columns[i].setType(type);

			int maxSize;
			file.read((char*)&maxSize, sizeof(maxSize));
			columns[i].setSize(maxSize);

			int defValueLen;
			file.read((char*)&defValueLen, sizeof(int));

			char* temp2 = new char[defValueLen + 1];
			file.read(temp2, defValueLen);
			temp2[defValueLen] = '\0';
			string defValue = temp2;
			delete[] temp2;

			columns[i].setDefaultValue(defValue);
		}
			
		Table tempTable(this->name, columns, noCols, noRows);


		/*string** readData = new string * [noCols];
		
		for (int i = 0; i < noCols; i++) {
			readData[i] = new string[noRows];
		}
*/	
		string* row = new string[noCols];

		for (int i = 0; i < noRows; i++) {
			for (int j = 0; j < noCols; j++) {
				//file.read(&readData[i][j][0], columns[i].getSize());
				char* temp3 = new char[columns[i].getSize() + 1];
				file.read(temp3, columns[i].getSize());
				temp3[columns[i].getSize()] = '\0';
				string temp = temp3;
				delete[] temp3;
				row[j] = temp;
			}
			tempTable.insertRow(row);
			
		}

		this->fTable = tempTable;
		delete[] row;
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
