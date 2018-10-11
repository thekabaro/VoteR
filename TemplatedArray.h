#ifndef TEMPLATED_ARRAY_H
#define TEMPLATED_ARRAY_H

#include "NvraRecord.h"
#include "Exceptions.h"

const int DEFAULT_ARRAY_CAPACITY = 10;			            // capacity used in no arg constructor

template<class DataType>
class TemplatedArray {
	private:
		unsigned long capacity = DEFAULT_ARRAY_CAPACITY;    // maximum capacity, in items
		unsigned long size = 0;							    // actual number of items currently in array
		DataType* data = NULL;					            // pointer to array of items
	public:
		TemplatedArray();						            // constructs array with default capacity
		TemplatedArray(int capacity);			            // constructs array with specified capacity
		virtual ~TemplatedArray();				            // frees array space as object is deleted
		void add(DataType* item);				            // adds item, increments size, doubles capacity as necessary
		DataType get(int index);				            // returns (copy of) item at index
		void addAt(DataType* item, int index);              // adds item at index, increment size and double if necessary
		void replaceAt(DataType* item, int index);          // replace item at index
		void removeAt(int index);                           // remove item at index, decrement size and half if necessary
};

// constructs array with default capacity
template<class DataType>
TemplatedArray<DataType>::TemplatedArray() {
	DataType* itemArray = new DataType[DEFAULT_ARRAY_CAPACITY];
	data = itemArray;
}

// constructs array with specified capacity
template<class DataType>
TemplatedArray<DataType>::TemplatedArray(int capacity) {
	this->capacity = capacity;
	DataType* itemArray = new DataType[capacity];
	data = itemArray;
}

// frees array space as object is deleted
template<class DataType>
TemplatedArray<DataType>::~TemplatedArray() {
	delete[] data;
}

// adds record, increments size, doubles capacity as necessary
template<class DataType>
void TemplatedArray<DataType>::add(DataType* item) {
	if (capacity < size + 1) {
		DataType* myNewItemArray = new DataType[capacity * 2];
		capacity *= 2;
		for (unsigned long i = 0; i < size; i++) {
			myNewItemArray[i] = data[i];
		}
		DataType* temp = data;
		data = myNewItemArray;
		delete[] temp;
		temp = NULL;
	}
	data[size] = *item;
	size++;
}

// returns (copy of) record at index
template<class DataType>
DataType TemplatedArray<DataType>::get(int index) {
	unsigned long longIndex = index;
	if (longIndex >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	return data[index];
}

// adds item at index, increment size and double if necessary
template<class DataType>
void TemplatedArray<DataType>::addAt(DataType* item, int index) {
	unsigned long longIndex = index;
	if (longIndex >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	if (capacity < size + 1) {
		DataType* myNewItemArray = new DataType[capacity * 2];
		capacity *= 2;
		for (unsigned long i = 0; i < size; i++) {
			myNewItemArray[i] = data[i];
		}
		DataType* temp = data;
		data = myNewItemArray;
		delete[] temp;
		temp = NULL;
	}
	if (longIndex == size) {
		data[index] = *item;
	}
	else {
		for (unsigned long i = size - 1; i >= longIndex; i--) {
			data[i + 1] = data[i];
		}
		data[index] = *item;
	}
	size++;
}

// replace item at index
template<class DataType>
void TemplatedArray<DataType>::replaceAt(DataType* item, int index) {
	unsigned long longIndex = index;
	if (longIndex >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	data[index] = *item;
}

// remove item at index, decrement size and half if necessary
template<class DataType>
void TemplatedArray<DataType>::removeAt(int index) {
	unsigned long longIndex = index;
	if (longIndex >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	for (unsigned long i = index; i < size - 1; i++) {
		data[i] = data[i + 1];
	}
	size--;
	if (capacity >= size * 2) {
		DataType* myNewItemArray = new DataType[capacity / 2];
		capacity /= 2;
		for (unsigned long i = 0; i < size; i++) {
			myNewItemArray[i] = data[i];
		}
		DataType* temp = data;
		data = myNewItemArray;
		delete[] temp;
		temp = NULL;
	}
}
#endif
