#pragma once
#include <iostream>
using namespace std;

class Article {
protected:
	string data;
public:

	// Constructor with argument
	Article(string input) {
		setData(input);
	}

	void setData(string input) {
		this->data = input;
	}

	string getData() {
		return this->data;
	}

	// Operator = overloaded
	void operator=(Article& l) {
		this->data = l.data;
	}

	// Copy constructor
	Article(Article& l) {
		this->data = l.data;
	}

};