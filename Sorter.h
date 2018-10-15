#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

template <typename T>
class Sorter {
private:
	static void swap(TemplatedArray<T>& array, long long i, long long j);
	static void quickSort(TemplatedArray<T>& array, long long left, long long right, const Comparator<T>& comparator);
public:
	static void sort(TemplatedArray<T>& array, const Comparator<T>& comparator);
};

template <typename T>
void Sorter<T>::swap(TemplatedArray<T>& array, long long i, long long j) {
	T temp = array[i];
	array.replaceAt(&array[j], i);
	array.replaceAt(&temp, j);
}

template <typename T>
void Sorter<T>::quickSort(TemplatedArray<T>& array, long long left, long long right, const Comparator<T>& comparator) {
	if (left < right) {
		long long int pivot = (left + right) / 2;
		long long int i = left - 1;
		long long j = right + 1;
		do {
			do {
				i++;
			} while ((i < array.getSize()) && (comparator.compare(array[i], array[pivot]) <= 0));
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

template <typename T>
void Sorter<T>::sort(TemplatedArray<T>& array, const Comparator<T>& comparator) {
	if (array.getSize() == 0) {
		return;
	}
	quickSort(array, 0, array.getSize() - 1, comparator);
}

#endif
