#pragma once

class Index {
protected:
	// No of offsets is equal to the number of rows always !
	string name = "";

	int* offsets = nullptr;
	int noOffsets = 0;
public:
	Index() {};

	Index(string name, int* offsets, int noOffsets) : name(name) {
		setOffsets(offsets, noOffsets);
	}

	void addOffset(int value) {
		int* newOff = new int[this->noOffsets + 1];

		for (int i = 0; i < this->noOffsets; i++) {
			newOff[i] = this->offsets[i];
		}

		newOff[this->noOffsets] = value;

		this->noOffsets++;

		delete[] this->offsets;
		this->offsets = newOff;
	}
	
	void writeIndex() {
		ofstream file(this->name + ".idx", ios::binary);
		if (!file.is_open())
			throw exception("Could not open file to write indexes.");


		file.write((char*)&noOffsets, sizeof(int));

		for (int i = 0; i < noOffsets; i++) {
			file.write((char*)&offsets[i], sizeof(int));
		}

		file.close();
	}

	string getName() {
		return this->name;
	}

	void setOffsets(int* offsets, int noOffsets) {
		if (this->noOffsets < 0) {
			throw exception("No of offsets is cannot be null.");
		}

		if (offsets == nullptr) {
			return;
		}
		
		int* newOff = new int[noOffsets];
		this->noOffsets = noOffsets;

		for (int i = 0; i < noOffsets; i++) {
			newOff[i] = offsets[i];
		}

		delete[] this->offsets;
		this->offsets = newOff;
	}

	void setName(string name) {
		this->name = name;
	};

	// Row index to be deleted
	void deleteOffset(int index) {
		if (index < 0 || index > this->noOffsets) {
			throw exception("Index is invalid.");
		}

		int* newOff = new int[noOffsets - 1];

		for (int i = 0; i < index; i++) {
			newOff[i] = offsets[i];
		}
		for (int j = index; j < noOffsets - 1; j++) {
			newOff[j] = offsets[j + 1];
		}

		delete[] this->offsets;
		this->offsets = newOff;
		this->noOffsets--;
	}


	Index(const Index& r) {
		this->setOffsets(r.offsets, r.noOffsets);
	}

	Index& operator=(const Index& r) {
		if (&r == this)
			return *this;
		
		this->setOffsets(r.offsets, r.noOffsets);

		return *this;
	}

	~Index() {
		delete[] offsets;
	}
};