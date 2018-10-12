#ifndef TEMPLATED_ARRAY_H
#define TEMPLATED_ARRAY_H

#include "Exceptions.h"

const int DEFAULT_ARRAY_CAPACITY = 10;			        // capacity used in no arg constructor

template<typename T>
class TemplatedArray {
private:
	unsigned long capacity = DEFAULT_ARRAY_CAPACITY;    // maximum capacity, in items
	unsigned long size = 0;							    // actual number of items currently in array
	T* data = NULL;					                    // pointer to array of items
public:
	TemplatedArray();						            // constructs array with default capacity
	TemplatedArray(int capacity);			            // constructs array with specified capacity
	virtual ~TemplatedArray();				            // frees array space as object is deleted
	void add(T* item);				                    // adds item, increments size, doubles capacity as necessary
	T get(int index);				                    // returns (copy of) item at index
	void addAt(T* item, int index);                     // adds item at index, increment size and double if necessary
	void replaceAt(T* item, int index);                 // replace item at index
	void removeAt(int index);                           // remove item at index, decrement size and half if necessary
};

// constructs array with default capacity
template<typename T>
TemplatedArray<T>::TemplatedArray() {
	T* itemArray = new T[DEFAULT_ARRAY_CAPACITY];
	data = itemArray;
}

// constructs array with specified capacity
template<typename T>
TemplatedArray<T>::TemplatedArray(int capacity) {
	this->capacity = capacity;
	T* itemArray = new T[capacity];
	data = itemArray;
}

// frees array space as object is deleted
template<typename T>
TemplatedArray<T>::~TemplatedArray() {
	delete[] data;
}

// adds record, increments size, doubles capacity as necessary
template<typename T>
void TemplatedArray<T>::add(T* item) {
	if (capacity < size + 1) {
		T* myNewItemArray = new T[capacity * 2];
		capacity *= 2;
		for (unsigned long i = 0; i < size; i++) {
			myNewItemArray[i] = data[i];
		}
		T* temp = data;
		data = myNewItemArray;
		delete[] temp;
		temp = NULL;
	}
	data[size] = *item;
	size++;
}

// returns (copy of) record at index
template<typename T>
T TemplatedArray<T>::get(int index) {
	unsigned long longIndex = index;
	if (longIndex >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	return data[index];
}

// adds item at index, increment size and double if necessary
template<typename T>
void TemplatedArray<T>::addAt(T* item, int index) {
	unsigned long longIndex = index;
	if (longIndex >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	if (capacity < size + 1) {
		T* myNewItemArray = new T[capacity * 2];
		capacity *= 2;
		for (unsigned long i = 0; i < size; i++) {
			myNewItemArray[i] = data[i];
		}
		T* temp = data;
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
template<typename T>
void TemplatedArray<T>::replaceAt(T* item, int index) {
	unsigned long longIndex = index;
	if (longIndex >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	data[index] = *item;
}

// remove item at index, decrement size and half if necessary
template<typename T>
void TemplatedArray<T>::removeAt(int index) {
	unsigned long longIndex = index;
	if (longIndex >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	for (unsigned long i = index; i < size - 1; i++) {
		data[i] = data[i + 1];
	}
	size--;
	if (capacity >= size * 2) {
		T* myNewItemArray = new T[capacity / 2];
		capacity /= 2;
		for (unsigned long i = 0; i < size; i++) {
			myNewItemArray[i] = data[i];
		}
		T* temp = data;
		data = myNewItemArray;
		delete[] temp;
		temp = NULL;
	}
}
#endif
