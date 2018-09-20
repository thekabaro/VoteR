#include <iostream>
using namespace std;

#include "NvraArray.h"

// constructs array with default capacity
NvraArray::NvraArray() {
	NvraRecord* recordArray = new NvraRecord[DEFAULT_ARRAY_CAPACITY];
	data = recordArray;
}

// constructs array with specified capacity
NvraArray::NvraArray(int capacity) {
	this->capacity = capacity;
	NvraRecord* recordArray = new NvraRecord[capacity];
	data = recordArray;
}

// frees array space as object is deleted
NvraArray::~NvraArray() {
	delete []data;
}

// adds record, increments size, doubles capacity as necessary
void NvraArray::add(NvraRecord* record) {
	if (capacity < size + 1) {
		NvraRecord* myNewRecordArray = new NvraRecord[capacity * 2];
		capacity *= 2;
		for (int i = 0; i < size; i++) {
			myNewRecordArray[i] = data[i];
		}
		NvraRecord* temp = data;
		data = myNewRecordArray;
		delete[] temp;
		temp = NULL;
	}
	data[size] = *record;
	size++;
}

// returns (copy of) record at index
NvraRecord NvraArray::get(int index) {
	return data[index];
}
