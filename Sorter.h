#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

template <typename T>
class Sorter {
private:
	// templated swap function
	static void swap(TemplatedArray<T>& array, long long i, long long j);
	// templated quick sort (using the psuedocode from the exam)
	static void quickSort(TemplatedArray<T>& array, long long left, long long right, const Comparator<T>& comparator);
public:
	// the initial starting point of sorter.h
	static void sort(TemplatedArray<T>& array, const Comparator<T>& comparator);
};

// templated swap function
template <typename T>
void Sorter<T>::swap(TemplatedArray<T>& array, long long i, long long j) {
	T temp = array[i];
	T arrayJ = array[j];
	array.replaceAt(&arrayJ, i);
	array.replaceAt(&temp, j);
}

// templated quick sort (using the psuedocode from the exam)
template <typename T>
void Sorter<T>::quickSort(TemplatedArray<T>& array, long long left, long long right, const Comparator<T>& comparator) {
	if (left < right) {
		long long sizeOfArray = array.getSize();
		long long pivot = (left + right) / 2;
		long long i = left - 1;
		long long j = right + 1;
		do {
			do {
				i++;
			} while ((i < sizeOfArray) && (comparator.compare(array[i], array[pivot]) <= 0));
			do {
				j--;
			} while ((j >= i) && (comparator.compare(array[j], array[pivot]) >= 0));
			if (i < j) {
				swap(array, i, j);
			}
		} while (i < j);
		if (i < pivot) {
			j = i;
		}
		swap(array, pivot, j);
		quickSort(array, left, j - 1, comparator);
		quickSort(array, j + 1, right, comparator);
	}
}

// the initial starting point of sorter.h
template <typename T>
void Sorter<T>::sort(TemplatedArray<T>& array, const Comparator<T>& comparator) {
	// already sorted (only 1 element)
	if (array.getSize() == 0) {
		return;
	}

	// start the sort
	quickSort(array, 0, array.getSize() - 1, comparator);
}

#endif
