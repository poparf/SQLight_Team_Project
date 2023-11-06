#pragma once
#include <iostream>
#include <string>
#include <regex>
#include "utils.h"
#include "column_class.h"
#include "table_class.h"
using namespace std;

class CmdProcessor
{
private:
	string fullCmd = "";
public:
	bool checkCmd() {
		regex createTableRegex("\\s*CREATE\\s+TABLE\\s+([A-Za-z][A-Za-z0-9]*)\\s*(IF\\s+NOT\\s+EXISTS)?\\s+\\(\\s*((?:\\(\\s*[A-Za-z][A-Za-z0-9]*,\\s*[A-Za-z]+,\\s*[0-9]*,\\s*[A-Za-z0-9]+\\s*\\)\\s*,?\\s*)+?)\\s*\\)", regex::icase);
		smatch matches;

		if (regex_search(this->fullCmd, matches, createTableRegex)) {
			this->createTable(matches);
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

private:
	void createTable(smatch matches) {
		regex partitionRegex("[^() ,][a-zA-Z0-9]*");
		smatch partitionMatches;

		// matches[0] = toata comanda
		// matches[1] = table name,
		// matches[2] = if not exists asta daca am scris daca nu e ""
		//  iar al patrulea input este doar ce este in paranteza (col_name, etc... )

		if (matches[2] == "") {
			// do smth
		}
		else {
			// do smth cause IF NOT EXISTS was used
			cout << endl << "IF NOT EXISTS USED.";
		}


		string tableName = matches[1].str();
		string beforePartition = matches[3].str();

		auto words_begin =
			std::sregex_iterator(beforePartition.begin(), beforePartition.end(), partitionRegex);
		auto words_end = std::sregex_iterator();

		// de ce am impartit la 4 ?
		// pt ca in fiecare paranteza sunt cate 4 valori
		// noi vrem sa creeam cate o coloana pt fiecare paranteza iar fiecare coloana are atribuite cate 4 valori
		int noColumns = distance(words_begin, words_end) / 4;
		Column* columns = new Column[noColumns];

		int j = 0;
		int k = 0;
		// aici nu putem folosi i pentru ca e de tipul regex iterator nu int
		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();

			switch (k) {
			case 0:
				columns[j].setColumnName(match_str);
				break;
			case 1:
				if (toLowerCase(match_str) == "number")
					columns[j].setType(columnTypes::NUMBER);
				else if (toLowerCase(match_str) == "string")
					columns[j].setType(columnTypes::STRING);
				else
					throw exception("Type of column must be number or string.");
				break;
			case 2:
				// stoi() transforms strings in integers
				columns[j].setSize(stoi(match_str));
				break;
			case 3:
				columns[j].setDefaultValue(match_str);
				break;
			}

			// valorile din paranteze sunt stocate secvential asa ca iteram la rand.
			// asta inseamna ca o sa dam de column name din 4 in 4 numere: 0 4 8 etc..
			// j-ul este nr coloanei care creste dupa ce trecem prin toate informatiile din coloana
			// care sunt 4 la nr. numele tipul size si default
			// de asta resetam k la 0, k e ca si cazul la care ne aflam
			k += 1;
			if (k == 4) {
				j += 1;
				k = 0;
			}
		}

		Table t(tableName, columns, j);
		cout << t;
		delete[] columns;
		// aici trebuie facut ceva cu tabelul.
		// doSomethingWithTable(t)
	}
};