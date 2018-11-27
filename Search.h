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
	long long result = 0;
	while (low <= high) {
		long long mid = (low + high) / 2;
		long long result = comparator.compare(array[mid], item);
		if (result == 0) {

			// MODIFIED binary search portion (returns the FIRST array location matching the given item)
			if ((mid > 0) && (comparator.compare(array[mid - 1], item) == 0)) {
				mid--;
				while (mid > 0 && comparator.compare(array[mid - 1], item) == 0) {
					mid--;
				}
				return mid;
			}
			else {
				return mid;
			}
		}
		else if (result == 1) {
			high = mid - 1;
			result = mid;
		}
		else {
			low = mid + 1;
			result = mid;
		}
	}
	// if not found, return a negative number corresponding to where in the array it should be inserted
	result = (result + 1) * -1;
	return result;
}

#endif
