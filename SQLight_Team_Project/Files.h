#pragma once

#include <sstream>

// A class that writes the table in a binary format
class outTable {
protected:
	ofstream file;
public:
	// Write also the extension of the file when you call the function
	void write(string fileName, Table& t) {
		file.open(fileName, ios::binary | ios::trunc);
		if (!file.is_open())
			throw exception("File couldn't be opened.");

		file.write((char*)&t.noColumns, sizeof(int));
		file.write((char*)&t.noRows, sizeof(int));

		for (int i = 0; i < t.noColumns; i++) {
			string colName = t.columns[i]->getName();
			int nameSize = colName.size() + 1; // +1 is for "\0"

			file.write((char*)&nameSize, sizeof(int));
			file.write(colName.c_str(), sizeof(char) * nameSize);

			// Type of the column written as int INT  - 0 FLOAT - 1 TEXT - 2
			int colType = t.columns[i]->getType();
			file.write((char*)&colType, sizeof(int));

			int colSize = t.columns[i]->getSize();
			file.write((char*)&colSize, sizeof(int));

			string defValue = t.columns[i]->getDef();
			int defValueSize = defValue.size() + 1;
			file.write((char*)&defValueSize, sizeof(int));
			file.write(defValue.c_str(), sizeof(char) * defValueSize);
		}

		for (int i = 0; i < t.noRows; i++) {
			Article** cells = t.rows[i]->getCells();
			int noCells = t.rows[i]->getNoCells();

			file.write((char*)&noCells, sizeof(int));

			for (int j = 0; j < noCells; j++) {
				string data = cells[j]->getData();
				int dataSize = data.size() + 1;

				file.write((char*)&dataSize, sizeof(int));
				file.write(data.c_str(), sizeof(char) * dataSize);
			}
		}
	}

	// Write one more row and change the number of rows in the file
	void writeRow(string fileName, Row row, int actualNoRows) {
		file.open(fileName, ios::binary | ios::in | ios::out); // this clears the file .. .not good
		if (!file.is_open())
			throw exception("File could not be opened when inserting a row.");

		file.seekp(0, ios::end);

		Article** cells = row.getCells();
		int noCells = row.getNoCells();

		file.write((char*)&noCells, sizeof(int));

		for (int j = 0; j < noCells; j++) {
			string data = cells[j]->getData();
			int dataSize = data.size() + 1;

			file.write((char*)&dataSize, sizeof(int));
			file.write(data.c_str(), sizeof(char) * dataSize);
		}

		
		// Change the number of rows in the file
		file.seekp(sizeof(int), ios::beg);
		file.write((char*)&actualNoRows, sizeof(int));
	}

	~outTable() {
		if (file.is_open())
			file.close();
	}
};

// A class that reads the table in the tableBuffer
class inTable {
protected:
	ifstream file;
public:
	// Write also the extension of the file when you call the function
	void readIntoBuffer(string fileName, TableBuffer& tb) {
		if (fileName.substr(fileName.size() - 4) != ".bin") {
			throw exception("The file should have a .bin format.");
		}

		file.open(fileName, ios::binary);
		if (!this->file.is_open())
			throw exception("File couldn't be opened.");


		int noCols;
		int noRows;
		file.read((char*)&noCols, sizeof(int));
		file.read((char*)&noRows, sizeof(int));

		Row** rows = new Row * [noRows];
		Column** cols = new Column * [noCols];

		for (int i = 0; i < noCols; i++) {
			int colNameSize;
			file.read((char*)&colNameSize, sizeof(int));

			char buffer[1000];
			file.read(buffer, sizeof(char) * colNameSize);
			string colName = string(buffer);

			int colType;
			file.read((char*)&colType, sizeof(int));

			int size;
			file.read((char*)&size, sizeof(int));

			int colDefValueSize;
			file.read((char*)&colDefValueSize, sizeof(int));

			char buffer2[1000];
			file.read(buffer2, sizeof(char) * colDefValueSize);
			string defValue = string(buffer2);

			cols[i] = new Column(colName, (columnTypes)colType, size, defValue);
		}

		Table temp(fileName.substr(0, fileName.size() - 4), cols, noCols);

		for (int i = 0; i < noRows; i++) {
			int noCells;
			file.read((char*)&noCells, sizeof(int));
			Article** cells = new Article * [noCells];

			for (int j = 0; j < noCells; j++) {
				int dataSize;
				file.read((char*)&dataSize, sizeof(int));

				char buffer3[1000];
				file.read(buffer3, sizeof(char) * dataSize);
				string data = string(buffer3);

				cells[j] = new Article(data);
			}

			rows[i] = new Row(cells, noCells);
		}

		temp.addManyRows(rows, noRows);
		tb.addTable(temp);


		for (int i = 0; i < noRows; i++) {
			if (rows[i] != nullptr)
				delete rows[i];
		}
		delete[] rows;

		for (int i = 0; i < noCols; i++) {
			if (cols[i] != nullptr)
				delete cols[i];
		}
		delete[] cols;
	}


	~inTable() {
		if (file.is_open())
			file.close();
	}
};


class xmlFile {
protected:
	ofstream xmlTable;
public:

	xmlFile() {};

	~xmlFile() {
		if (xmlTable.is_open())
			xmlTable.close();
	}
	// Do not include a header.
	void generateXML(string tableName, Table& t) {
		this->xmlTable.open(tableName + ".xml");

		xmlTable << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
		xmlTable << "<Table name=\"" << t.name << "\">\n";

		// Write columns
		xmlTable << "\t<Columns>\n";
		for (int i = 0; i < t.noColumns; ++i) {
			xmlTable << "\t\t<Column name=\"" << t.columns[i]->getName() << "\" />\n";
		}
		xmlTable << "\t</Columns>\n";

		// Write rows
		xmlTable << "\t<Rows>\n";
		for (int i = 0; i < t.noRows; ++i) {
			xmlTable << "\t\t<Row>\n";
			Article** cells = t.rows[i]->getCells();
			for (int j = 0; j < t.noColumns; ++j) {
				if (t.columns[j]->getType() == columnTypes::TEXT) {
					xmlTable << "\t\t\t<Cell type=\"" << "Text" << "\">" << cells[j]->getData() << "</Cell>\n";
				}
				else {
					xmlTable << "\t\t\t<Cell type=\"" << "Number" << "\">" << cells[j]->getData() << "</Cell>\n";
				}

				
			}
			xmlTable << "\t\t</Row>\n";
		}
		xmlTable << "\t</Rows>\n";
		xmlTable << "</Table>\n";
	}
};


class csvTable {
protected:
	ofstream csvFile;
public:
	// Do not include a header.
	void generateCSV(string tableName, Table & t) {
		csvFile.open(tableName + ".csv");

		// Write header row with column names
		for (int i = 0; i < t.noColumns; ++i) {
			csvFile << t.columns[i]->getName();
			if (i < t.noColumns - 1) {
				csvFile << ",";
			}
		}
		csvFile << "\n";

		// Write data rows
		for (int i = 0; i < t.noRows; ++i) {
			Article** cells = t.rows[i]->getCells();
			for (int j = 0; j < t.noColumns; ++j) {
				csvFile << cells[j]->getData();
				if (j < t.noColumns - 1) {
					csvFile << ",";
				}
			}
			csvFile << "\n";
		}
	}

	void readCSV(string fileName, Table& t) {
		ifstream csvFile(fileName);

		if (!csvFile.is_open()) {
			cout << endl << fileName;
			cout << "Error opening CSV file." << endl;
			return;
		}

		string line;
		// Read data rows
		while (getline(csvFile, line)) {
			stringstream ss(line);
			string single;
			Article** cells = new Article * [100];
			int noCells = 0;
			while (getline(ss, single, ',')) {
				cells[noCells++] = new Article(single);
			}
			Row row(cells, noCells);
		

			t.addRow(row); // Assuming you have a function to add a row to the table
		
			for (int i = 0; i < noCells; i++) {
				delete cells[i];
			}
			delete[] cells;
		}

		csvFile.close();
	}

	~csvTable() {
		if(csvFile.is_open())
			csvFile.close();
	}
};


