#include <iostream>
#include <string>
#include <sstream>
/*
* Accepted Commands
CREATE TABLE, CREATE INDEX, DROP TABLE, DROP INDEX, DISPLAY TABLE

We will receive a string as a command and we split each word
and validate each one

*/

enum basicCmdWords { CREATE, TABLE, INDEX, DROP, DISPLAY };

class CmdProcessor
{
private:
	std::string fullCmd = "";
	//std::string* splitCmd = nullptr;
	std::stringstream splitCmd;
	

public:
	void split() {

	
		std::stringstream ss(this->fullCmd);
		//std::string word;
		//while (ss >> word) { // Extract word from the stream.
		//	std::cout << "word: " << word << std::endl;
		//}
		//std::cout << std::endl;


		//// Assuming that every word is separated by a space ' '
		//this->noWords = 1;
		//for (int i = 0; i < fullCmd.length(); i++) {
		//	if (fullCmd[i] == ' ') {
		//		this->noWords++;
		//	}
		//}

		//delete[] splitCmd;
		//// We create an array of strings, each string representing a word
		//this->splitCmd = new std::string[this->noWords];

		//int i = 0;
		//while (this->fullCmd[i] == ' ') {
		//	i++; // Skip the consecutive spaces
		//}

		//int j = i;
		//int k = 0;
		//for (; i < fullCmd.length(); i++) {

		//	if (this->fullCmd[i] == ' ') {
		//		this->splitCmd[k] = this->fullCmd.substr(j, i - j);
		//		j = i + 1;
		//		k += 1;
		//	}
		//}
		//// Adding the last word if the string doesn't end with a space
		//if (j < this->fullCmd.length()) {
		//	this->splitCmd[k] = this->fullCmd.substr(j, this->fullCmd.length() - j);
		//}
	}

	bool checkCmd() {
		split();
		// aici verifci fiecare tip de comanda
		// splitCmd contine fiecare cuvant din input
		// se scoate cuvantul din stream cu operatorul >>
		return 1;
	}

	void setFullCmd() {
		
		// Wrong version: cin takes only the first word
		//std::cin >> this->fullCmd;

		std::getline(std::cin, this->fullCmd);

		if (this->fullCmd.length() <= 0) {
			this->fullCmd = "";
			
			throw std::exception("Error reading the input. Try again.");
		}
	}

	std::string getFullCmd() {
		return this->fullCmd;
	}
};


void printLine() {
	std::cout << "\nsqlight> ";
}

int main() {
	CmdProcessor buffer;

	while (1) {
		printLine();

		try {
			buffer.setFullCmd();
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
			continue; // Reset the loop
		}


		//  TODO: remake this first if
		if (buffer.getFullCmd() == "/quit") {
			exit(EXIT_SUCCESS);
		}
		else if(buffer.checkCmd()) {
			// now check for validity of command
		}
		else {
			std::cout << "Command not recognized: " << buffer.getFullCmd();
		}
	}
}