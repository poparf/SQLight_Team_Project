#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
/*
* Accepted Commands
CREATE TABLE, CREATE INDEX, DROP TABLE, DROP INDEX, DISPLAY TABLE



*/
using namespace std;


enum columnTypes { NUMBER, STRING };

class Column {
private:
	string columnName = "";
	columnTypes type;
	size_t size = 0;
	// aici daca am putea sa scapam de una de care nu avem nevoie ar fi super
	float numDefaultValue = 0;
	string strDefaultValue = "";
	// default value is created based on the type
public:

	void setDefaultValue(string input) {
		if (this->type == columnTypes::STRING) {
			this->strDefaultValue = input;
		}
		else {
			throw exception("Wrong input. Type is number.");
		}
	}

	void setDefaultValue(float input) {
		if (this->type == columnTypes::NUMBER) {
			this->numDefaultValue = input;
		}
		else {
			throw exception("Wrong input. Type is string.");
		}
	}


	void setColumnName(string input) {
		// validari aici

			// e ok asa pentru ca e string si nu char* deci nu facem shallow copy
		this->columnName = input;

	}

	void setType(columnTypes type) {
		this->type = type;
	}

	void setSize(size_t input) {
		this->size = input;
	}

	columnTypes getType() {
		return this->type;
	}



	void print() {
		cout << endl << "~~~~~~~~~~~~~~~~";
		cout << endl << "Column name: " << this->columnName;
		cout << endl << "Type: " << this->type;
		cout << endl << "Size: " << this->size;

		if (this->type == columnTypes::NUMBER) {
			cout << endl << "Default: " << this->numDefaultValue;
		}
		else {
			cout << endl << "Default: " << this->strDefaultValue;
		}

	}
};

class CmdProcessor
{
private:
	string fullCmd = "";
public:
	bool checkCmd() {
		regex createTableRegex("\\s*CREATE\\s+TABLE\\s+([A-Za-z][A-Za-z0-9]*)\\s*(?:IF\\s+NOT\\s+EXISTS)?\\s+\\(\\s*((?:\\(\\s*[A-Za-z][A-Za-z0-9]*,\\s*[A-Za-z]+,\\s*[0-9]*,\\s*[A-Za-z0-9]+\\s*\\)\\s*,?\\s*)+?)\\s*\\)", std::regex::icase);
		smatch matches;

		if (regex_search(this->fullCmd, matches, createTableRegex)) {
			regex partitionRegex("[^() ,][a-zA-Z0-9]*");
			smatch partitionMatches;
			string tableName = matches[1].str();
			string beforePartition = matches[2].str();

			auto words_begin =
				std::sregex_iterator(beforePartition.begin(), beforePartition.end(), partitionRegex);
			auto words_end = std::sregex_iterator();

			// de ce am impartit la 4 ?
			// pt ca in fiecare paranteza sunt cate 4 valori
			// noi vrem sa creeam cate o coloana pt fiecare paranteza iar fiecare coloana are atribuite cate 4 valori
			int noColumns = distance(words_begin, words_end) / 4;
			Column* columns = new Column[noColumns];

			// taken from regex documentation
			int j = 0;
			int k = 0;
			// aici nu putem folosi i pentru ca e de tipul regex iterator nu int
			for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
				std::smatch match = *i;
				std::string match_str = match.str();

				// aici se scoate fiecare column_name type size si default+value
				std::cout << "  " << match_str << '\n';

				switch (k) {
				case 0:
					columns[j].setColumnName(match_str);
					break;
				case 1:
					if (match_str == "number")
						columns[j].setType(columnTypes::NUMBER);
					else if (match_str == "string")
						columns[j].setType(columnTypes::STRING);
					break;
				case 2:
					// transforms strings in integers
					columns[j].setSize(stoi(match_str));
					break;
				case 3:
					if (columns[j].getType() == columnTypes::NUMBER)
						columns[j].setDefaultValue(stoi(match_str));
					else if (columns[j].getType() == columnTypes::STRING)
						columns[j].setDefaultValue(match_str);
					break;
				}

				k += 1;
				if (k == 4) {
					j += 1;
					k = 0;
				}
			}


			// aici tre sa faci ceva cu coloanele si dupa sa le stergi...
			// altfel nu salvezi pointerul cate array ul de coloane si le pierzi
			for (int i = 0; i < noColumns; i++) {
				columns[i].print();
			}
			return 1;
		}



		return 0;
	}


	void setFullCmd() {
		getline(cin, this->fullCmd);

		if (this->fullCmd.length() <= 0) {
			this->fullCmd = "";

			throw exception("Error reading the input. Try again.");
		}
	}

	string getFullCmd() {
		return this->fullCmd;
	}
};


void printLine() {
	cout << "\nsqlight> ";
}

int main() {
	CmdProcessor buffer;

	while (1) {
		printLine();

		try {
			buffer.setFullCmd();
		}
		catch (const exception& e) {
			cout << e.what() << endl;
			continue; // Reset the loop
		}


		//  TODO: remake this first if
		if (buffer.getFullCmd() == "/quit") {
			exit(EXIT_SUCCESS);
		}
		else if (buffer.checkCmd()) {
			// now check for validity of command
		}
		else {
			cout << "Command not recognized: " << buffer.getFullCmd();
		}
	}
}
