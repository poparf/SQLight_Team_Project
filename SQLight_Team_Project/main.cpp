#include <iostream>
#include <string>

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
	std::string* splitCmd = nullptr;

public:


};



class cmdBuffer 
{
private:	
	std::string inputBuffer = "";

public:
	void setInputBuffer() {
		std::cin >> this->inputBuffer;

		if (this->inputBuffer.length() <= 0) {
			this->inputBuffer = "";
			std::cout << "Error reading the input. Try again.";
		}
	}

	std::string getInputBuffer() {
		return this->inputBuffer;
	}
};

void printLine() {
	std::cout << "\nsqlight> ";
}

int main() {
	cmdBuffer buff;
	while (1) {
		printLine();
		buff.setInputBuffer();

		if (buff.getInputBuffer() == "/quit") {
			exit(EXIT_SUCCESS);
		}
		else {
			// now check for validity of command
		}
		
		std::cout << "Command not recognized: " << buff.getInputBuffer();
	}
}