#pragma once
using namespace std;

string toLowerCase(string input) {
	for (int i = 0; i < input.length(); i++) {
		if (input[i] >= 65 && input[i] <= 90) {
			input[i] += 32;
		}
	}
	return input;
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

// doesn t work idk why
template<class T>
void checkCommands(T** t) {
	for (int i = 0; i < T::counter; i++) {
		if (t[i] != nullptr) {
			if (t[i]->check()) {
				t[i]->process();
				return;
			}
		}
	}
}