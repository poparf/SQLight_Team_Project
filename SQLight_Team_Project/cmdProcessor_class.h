#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "utils.h"
#include "column_class.h"
using namespace std;

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

			// Aici 1 reprezinta al doilea match
			// primul match este toata instructiunea, al doilea table name, iar al trilea input este doar ce este in paranteza (col_name, etc... )
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
					if (columns[j].getType() == columnTypes::NUMBER)
						columns[j].setDefaultValue(stoi(match_str));
					else if (columns[j].getType() == columnTypes::STRING)
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


			// aici tre sa faci ceva cu coloanele si dupa sa le stergi...
			// altfel nu salvezi pointerul cate array ul de coloane si le pierzi
			for (int i = 0; i < noColumns; i++) {
				cout << columns[i];
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