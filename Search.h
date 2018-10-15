#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

// returns FIRST array location matching the given item (based on the comparator)
template <typename T>
long long binarySearch(const T& item, const TemplatedArray<T>& array, const Comparator<T>& comparator) {
	long long low = 0;
	long long high;
	if (array.getSize() != 0) {
		high = array.getSize() - 1;
	}
	else {
		return -1;
	}
	while (low <= high) {
		long long mid = (low + high) / 2;
		int result = comparator.compare(array[mid], item);
		if (result == 0) {
			return mid;
		}
		else if (result == 1) {
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}
	return -1;
}

#endif
