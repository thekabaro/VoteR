#ifndef HASH_TABLE
#define HASH_TABLE

#include <iostream>
#include "Exceptions.h"
#include "Comparator.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Hasher.h"
using namespace std;

const unsigned int SCHEDULE_SIZE = 25;			// the number of items in the size schedule
const unsigned int SCHEDULE[SCHEDULE_SIZE] = { 1, 2, 5, 11, 23, 53, 107, 223, 449, 907, 1823, 3659, 7309, 14621, 29243, 58511, 117023, 234067, 468157, 936319, 1872667, 3745283, 7490573, 14981147, 29962343 };		// the size schedule (all primes)
const unsigned int DEFAULT_SCHEDULE_INDEX = 3;	// the default position in the size schedule
const unsigned long DEFAULT_BASE_CAPACITY = SCHEDULE[DEFAULT_SCHEDULE_INDEX]; 	// the default size of the array
const float DEFAULT_MAX_LOAD_FACTOR = 0.9f;		// the default load factor used to determine when to increase the table size
const float DEFAULT_MIN_LOAD_FACTOR = 0.4f;		// the default load factor used to determine when to decrease the table size

template <typename T>
class HashTable {
	template <typename U>
	friend class HashTableEnumerator;						// necessary to allow the enumerator to access the table's private data
private:
	Comparator<T>* comparator = NULL;						// used to determine item equality
	Hasher<T>* hasher = NULL;								// used to compute hash value
	unsigned long size = 0;									// actual number of items currently in hash table
	float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR;			// the load factor used to determine when to increase the table size
	float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR;			// the load factor used to determine when to decrease the table size
	unsigned int scheduleIndex = DEFAULT_SCHEDULE_INDEX;	// the index of current location in the size schedule
	unsigned long baseCapacity = DEFAULT_BASE_CAPACITY;		// the size of the array
	unsigned long totalCapacity = baseCapacity;				// the size of the array plus chains of more than one link
	OULinkedList<T>** table = NULL;							// table will be an array of pointers to OULinkedLists of type T
	// you may add additional member variables and functions here to support the operation of your code
public:
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher);			// creates an empty table of DEFAULT_BASE_CAPACITY
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher,             // if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
		unsigned long size,
		float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR,
		float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR);
	virtual ~HashTable();

	// if an equivalent item is not already present, insert item at proper location and return true
	// if an equivalent item is already present, leave table unchanged and return false
	bool insert(const T* item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool replace(const T* item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool remove(const T* item);

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionHashTableAccess
	T find(const T* item) const;

	unsigned long getSize() const;						// returns the current number of items in the table
	unsigned long getBaseCapacity() const;				// returns the current base capacity of the table
	unsigned long getTotalCapacity() const;				// returns the current total capacity of the table
	float getLoadFactor() const;						// returns the current load factor of the table
	unsigned long getBucketNumber(const T* item) const;	// returns the bucket number for an item using its hash function mod array size

};

// put implementation for HashTable here

// creates an empty table of DEFAULT_BASE_CAPACITY
template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher) {
	this->comparator = comparator;
	this->hasher = hasher;
	table = new OULinkedList<T>*[DEFAULT_BASE_CAPACITY];
	for (unsigned long i = 0; i < DEFAULT_BASE_CAPACITY; i++) {
		table[i] = NULL;
	}
}

// if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher, unsigned long size, float maxLoadFactor, float minLoadFactor) {
	this->comparator = comparator;
	this->hasher = hasher;
	this->maxLoadFactor = maxLoadFactor;
	this->minLoadFactor = minLoadFactor;
	unsigned int selectedIndex = 0;
	while ((float)SCHEDULE[selectedIndex] <= ((float)size * (1 / maxLoadFactor))) {
		selectedIndex++;
	}
	scheduleIndex = selectedIndex;
	baseCapacity = SCHEDULE[selectedIndex];
	totalCapacity = baseCapacity;
	table = new OULinkedList<T>*[SCHEDULE[selectedIndex]];
	for (unsigned long i = 0; i < SCHEDULE[selectedIndex]; i++) {
		table[i] = NULL;
	}
}

// destructor
template <typename T>
HashTable<T>::~HashTable() {
	delete[] table;
}

// if an equivalent item is not already present, insert item at proper location and return true
// if an equivalent item is already present, leave table unchanged and return false
template <typename T>
bool HashTable<T>::insert(const T* item) {
	unsigned long bucketNum = hasher->hash(*item) % SCHEDULE[scheduleIndex];
	if (table[bucketNum] == NULL) {
		OULinkedList<T>* newLinkedList = new OULinkedList<T>(comparator);
		newLinkedList->append(item);
		size++;
		table[bucketNum] = newLinkedList;
		if (getLoadFactor() > maxLoadFactor) {
			scheduleIndex++;
			totalCapacity = SCHEDULE[scheduleIndex];
			OULinkedList<T>** newTable = new OULinkedList<T>*[SCHEDULE[scheduleIndex]];
			for (unsigned long i = 0; i < SCHEDULE[scheduleIndex]; i++) {
				newTable[i] = NULL;
			}
			for (unsigned long i = 0; i < baseCapacity; i++) {
				if (table[i] != NULL) {
					OULinkedListEnumerator<T> myEnum = table[i]->enumerator();
					while (myEnum.hasNext()) {
						T* temp = new T(myEnum.next());
						unsigned long newBucketNum = hasher->hash(*temp) % SCHEDULE[scheduleIndex];
						if (newTable[newBucketNum] == NULL) {
							OULinkedList<T>* newestLinkedList = new OULinkedList<T>(comparator);
							newestLinkedList->append(temp);
							newTable[newBucketNum] = newestLinkedList;
						}
						else {
							newTable[newBucketNum]->insert(temp);
							totalCapacity++;
						}
					}
				}
			}
			baseCapacity = SCHEDULE[scheduleIndex];
			table = newTable;
		}
		return true;
	}
	else {
		if (table[bucketNum]->insert(item)) {
			if (getLoadFactor() > maxLoadFactor) {
				
			}
			size++;
			totalCapacity++;
			return true;
		}
		return false;
	}
}

// if an equivalent item is already present, replace item and return true
// if an equivalent item is not already present, leave table unchanged and return false
template <typename T>
bool HashTable<T>::replace(const T* item) {
	for (unsigned long i = 0; i < baseCapacity; i++) {
		if (table[i] != NULL) {
			OULinkedListEnumerator<T> tempEnum = table[i]->enumerator();
			while (tempEnum.hasNext()) {
				T* temp = new T(tempEnum.next());
				if (comparator->compare(*item, *temp) == 0) {
					table[i]->replace(item);
					return true;
				}
			}
		}
	}
	return false;
}

// if an equivalent item is already present, remove item and return true
// if an equivalent item is not already present, leave table unchanged and return false
template <typename T>
bool HashTable<T>::remove(const T* item) {
	for (unsigned long i = 0; i < baseCapacity; i++) {
		if (table[i] != NULL) {
			OULinkedListEnumerator<T> tempEnum = table[i]->enumerator();
			while (tempEnum.hasNext()) {
				T* temp = new T(tempEnum.next());
				if (comparator->compare(*item, *temp) == 0) {
					table[i]->remove(item);
					if (getLoadFactor() < minLoadFactor) {
						scheduleIndex--;
						totalCapacity = SCHEDULE[scheduleIndex];
						OULinkedList<T>** newTable = new OULinkedList<T>*[SCHEDULE[scheduleIndex]];
						for (unsigned long i = 0; i < SCHEDULE[scheduleIndex]; i++) {
							newTable[i] = NULL;
						}
						for (unsigned long i = 0; i < baseCapacity; i++) {
							if (table[i] != NULL) {
								OULinkedListEnumerator<T> myEnum = table[i]->enumerator();
								while (myEnum.hasNext()) {
									T* temp = new T(myEnum.next());
									unsigned long newBucketNum = hasher->hash(*temp) % SCHEDULE[scheduleIndex];
									if (newTable[newBucketNum] == NULL) {
										OULinkedList<T>* newestLinkedList = new OULinkedList<T>(comparator);
										newestLinkedList->append(temp);
										newTable[newBucketNum] = newestLinkedList;
									}
									else {
										newTable[newBucketNum]->insert(temp);
										totalCapacity++;
									}
								}
							}
						}
						baseCapacity = SCHEDULE[scheduleIndex];
						table = newTable;
					}
					return true;
				}
			}
		}
	}
	return false;
}

// if an equivalent item is present, return a copy of the item
// if an equivalent item is not present, throw a new ExceptionHashTableAccess
template <typename T>
T HashTable<T>::find(const T* item) const {
	for (unsigned long i = 0; i < baseCapacity; i++) {
		if (table[i] != NULL) {
			OULinkedListEnumerator<T> tempEnum = table[i]->enumerator();
			while (tempEnum.hasNext()) {
				T* temp = new T(tempEnum.next());
				if (comparator->compare(*item, *temp) == 0) {
					return *temp;
				}
			}
		}
	}
	throw new ExceptionHashTableAccess();
}

// returns the current number of items in the table
template <typename T>
unsigned long HashTable<T>::getSize() const {
	return size;
}

// returns the current base capacity of the table
template <typename T>
unsigned long HashTable<T>::getBaseCapacity() const {
	return baseCapacity;
}

// returns the current total capacity of the table
template <typename T>
unsigned long HashTable<T>::getTotalCapacity() const {
	return totalCapacity;
}

// returns the current load factor of the table
template <typename T>
float HashTable<T>::getLoadFactor() const {
	return ((float)size) / ((float)totalCapacity);
}

// returns the bucket number for an item using its hash function mod array size
template <typename T>
unsigned long HashTable<T>::getBucketNumber(const T* item) const {
	return hasher->hash(*item) % SCHEDULE[scheduleIndex];
}

#endif // !HASH_TABLE