#pragma once
using namespace std;
#define NO_SECONDARY_COMMANDS 7

string toLowerCase(string input) {
	for (int i = 0; i < input.length(); i++) {
		if (input[i] >= 65 && input[i] <= 90) {
			input[i] += 32;
		}
	}
	return input;
}

void printLine() {
	cout << "\nsqlight> ";
}

int checkSecondaryCommand(string fullCmd) {
	string additionalCommands[NO_SECONDARY_COMMANDS] = { "/quit", "/clear", "/help", "/activate csv", "/activate xml", "/deactivate xml", "/deactivate csv"};

	for (int i = 0; i < NO_SECONDARY_COMMANDS; i++) {
		if (fullCmd == additionalCommands[i]) {
			return i + 1;
		}
	}
	return -1;
}

string getCurrentDate() {
	// Get the current time
	time_t currentTime = time(nullptr);

	// Convert the current time to a tm structure
	tm timeInfo = {};
	localtime_s(&timeInfo, &currentTime);

	// Format the date and time as "dd_mm_yyyy_hh_mm_ss"
	char date[20];
	strftime(date, sizeof(date), "%d_%m_%Y_%H_%M_%S", &timeInfo);

	return string(date);
}