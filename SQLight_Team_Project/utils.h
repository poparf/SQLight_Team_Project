#pragma once
#include <string>
#include <iostream>
using namespace std;

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