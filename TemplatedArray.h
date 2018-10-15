#ifndef TEMPLATED_ARRAY_H
#define TEMPLATED_ARRAY_H

#include "Exceptions.h"

const unsigned DEFAULT_ARRAY_CAPACITY = 10;			      // capacity used in no arg constructor

template<typename T>
class TemplatedArray {
private:
	unsigned long capacity = DEFAULT_ARRAY_CAPACITY;      // maximum capacity, in items
	unsigned long size = 0;							      // actual number of items currently in array
	T* data = NULL;					                      // pointer to array of items
	void doubleCapacity();                                // method to double array capacity
	void halveCapacity();                                 // method to halve array capacity
public:
	TemplatedArray();						              // constructs array with default capacity
	TemplatedArray(unsigned long capacity);			      // constructs array with specified capacity
	virtual ~TemplatedArray();				              // frees array space as object is deleted
	void add(const T* item);				              // adds item, increments size, doubles capacity as necessary
	void addAt(const T* item, unsigned long index);       // adds item at index, increment size and double if necessary
	void replaceAt(const T* item, unsigned long index);   // replace item at index
	void removeAt(unsigned long index);                   // remove item at index, decrement size and half if necessary
	T get(unsigned long index) const;				      // returns (copy of) item at index
	unsigned long getCapacity() const;                    // returns the current capacity of the array
	unsigned long getSize() const;                        // returns the current number of items in the array
	T operator[](unsigned long index) const;              // returns (copy of) item at index
};

// method to double array capacity
template<typename T>
void TemplatedArray<T>::doubleCapacity() {
	T* myNewItemArray = new T[capacity * 2];
	if (myNewItemArray == NULL) {
		throw new ExceptionMemoryNotAvailable();
	}
	capacity *= 2;
	for (unsigned long i = 0; i < size; i++) {
		myNewItemArray[i] = data[i];
	}
	T* temp = data;
	data = myNewItemArray;
	delete[] temp;
	temp = NULL;
}

// method to halve array capacity
template<typename T>
void TemplatedArray<T>::halveCapacity() {
	T* myNewItemArray = new T[capacity / 2];
	if (myNewItemArray == NULL) {
		throw new ExceptionMemoryNotAvailable();
	}
	capacity /= 2;
	for (unsigned long i = 0; i < size; i++) {
		myNewItemArray[i] = data[i];
	}
	T* temp = data;
	data = myNewItemArray;
	delete[] temp;
	temp = NULL;
}

// constructs array with default capacity
template<typename T>
TemplatedArray<T>::TemplatedArray() {
	T* itemArray = new T[DEFAULT_ARRAY_CAPACITY];
	if (itemArray == NULL) {
		throw new ExceptionMemoryNotAvailable();
	}
	data = itemArray;
}

// constructs array with specified capacity
template<typename T>
TemplatedArray<T>::TemplatedArray(unsigned long capacity) {
	this->capacity = capacity;
	T* itemArray = new T[capacity];
	if (itemArray == NULL) {
		throw new ExceptionMemoryNotAvailable();
	}
	data = itemArray;
}

// frees array space as object is deleted
template<typename T>
TemplatedArray<T>::~TemplatedArray() {
	delete[] data;
}

// adds record, increments size, doubles capacity as necessary
template<typename T>
void TemplatedArray<T>::add(const T* item) {
	if (capacity < size + 1) {
		doubleCapacity();
	}
	data[size] = *item;
	size++;
}

// adds item at index, increment size and double if necessary
template<typename T>
void TemplatedArray<T>::addAt(const T* item, unsigned long index) {
	if (index >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	if (capacity < size + 1) {
		doubleCapacity();
	}
	if (index == size) {
		data[index] = *item;
	}
	else {
		for (unsigned long i = size - 1; i >= index; i--) {
			data[i + 1] = data[i];
		}
		data[index] = *item;
	}
	size++;
}

// replace item at index
template<typename T>
void TemplatedArray<T>::replaceAt(const T* item, unsigned long index) {
	if (index >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	data[index] = *item;
}

// remove item at index, decrement size and half if necessary
template<typename T>
void TemplatedArray<T>::removeAt(unsigned long index) {
	if (index >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	for (unsigned long i = index; i < size - 1; i++) {
		data[i] = data[i + 1];
	}
	size--;
	if (capacity >= size * 2) {
		halveCapacity();
	}
}

// returns (copy of) record at index
template<typename T>
T TemplatedArray<T>::get(unsigned long index) const {
	if (index >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	return data[index];
}

// returns the current capacity of the array
template<typename T>
unsigned long TemplatedArray<T>::getCapacity() const {
	return capacity;
}

// returns the current number of items in the array
template<typename T>
unsigned long TemplatedArray<T>::getSize() const {
	return size;
}

// returns (copy of) item at index
template<typename T>
T TemplatedArray<T>::operator[](unsigned long index) const {
	if (index >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	return data[index];
}
#endif
