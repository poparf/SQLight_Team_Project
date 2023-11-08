#pragma once
#include <iostream>
#include <string>
#include "table_class.h"
#include "column_class.h"
using namespace std;


class Row {
	int rowId = 0;
	string* data = nullptr;
public:
	~Row() {
		delete[] data;
	}
};
