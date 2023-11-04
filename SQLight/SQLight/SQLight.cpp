#include <iostream>
#include <string>
#include <sstream>
#include <regex>
/*
* Accepted Commands
CREATE TABLE, CREATE INDEX, DROP TABLE, DROP INDEX, DISPLAY TABLE

We will receive a string as a command and we split each word
and validate each one

*/
using namespace std;

class CmdProcessor
{
private:
	string fullCmd = "";
public:
	bool checkCmd() {
		regex createTableRegex("\\s*CREATE\\s+TABLE\\s+[A-Za-z][A-Za-z0-9]*\\s*(?:IF\\s+NOT\\s+EXISTS)?\\s+\\(\\s*((?:\\(\\s*[A-Za-z][A-Za-z0-9]*,\\s*[A-Za-z]+,\\s*[A-Za-z]+\\s*\\)\\s*,?\\s*)+?)\\s*\\)", std::regex::icase);
		smatch matches;

		if (regex_search(fullCmd, matches, createTableRegex)) {
			cout << "Match found." << matches[0];

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