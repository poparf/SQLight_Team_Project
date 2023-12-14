#pragma once


class RegexManager {
protected:
	regex** regexCommand = nullptr;
	static int noCommands;

	~RegexManager() {
		for (int i = 0; i < noCommands; i++)
			delete regexCommand[i];
		delete[] regexCommand;
	}

public:
	regex** getRegexes() {
		regex** newRegexes = new regex*[this->noCommands];
		for (int i = 0; i < this->noCommands; i++) {
			newRegexes[i] = regexCommand[i];
		}

		return newRegexes;
	}

	int getNoCommands() {
		return this->noCommands;
	}
};

int RegexManager::noCommands = 0;

// read regex as string and initialize the regex
class inRegex : public RegexManager {
public:
	void read(string filePath) {
		ifstream rgxFile(filePath);

		string* lines;
		int noLines;
		if (rgxFile.is_open()) {

			rgxFile >> noLines;
			this->noCommands = noLines;
			lines = new string[this->noCommands];
			int i = 0;
			while (!rgxFile.eof()) {
				char buffer[1000];
				rgxFile.ignore();
				rgxFile.getline(buffer, 1000);
				lines[i++] = string(buffer);
			}

			initialize(lines);
			rgxFile.close();
		}
		else {
			throw exception("File not found.");
		}
		
	}

protected:
	void initialize(string* commands) {
		regexCommand = new regex*[this->noCommands];
		for (int i = 0; i < this->noCommands; i++) {
			regexCommand[i] = new regex(commands[i], regex::icase);
		}
	}
};
