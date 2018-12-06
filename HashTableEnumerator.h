#ifndef HASH_TABLE_ENUMERATOR
#define HASH_TABLE_ENUMERATOR

#include "Enumerator.h"
#include "Exceptions.h"
#include "HashTable.h"

template <typename T>
class HashTableEnumerator : public Enumerator<T>
{
private:
	unsigned long bucket = 0;								// the current bucket during the enumeration process
	OULinkedListEnumerator<T>* chainEnumerator = NULL;		// used to move through the linked list of the current bucket
	HashTable<T>* hashTable = NULL;							// pointer to the HashTable being enumerated
	// you may add additional member variables and functions here to support the operation of your code
public:
	HashTableEnumerator(HashTable<T>* hashTable);			// constructor needs a pointer to the HashTable to be enumerated
	virtual ~HashTableEnumerator();
	bool hasNext() const;									// true if there are elements that have not yet been returned via next()
	T next();												// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;											// throws ExceptionEnumerationBeyondEnd if no next item is available
};

// put implementation for HashTableEnumerator here

// constructor needs a pointer to the HashTable to be enumerated
template <typename T>
HashTableEnumerator<T>::HashTableEnumerator(HashTable<T>* hashTable) {
	this->hashTable = hashTable;
	while (hashTable->table[bucket] == NULL) {
		bucket++;
	}
	if (bucket >= hashTable->getBaseCapacity()) {
		throw new ExceptionHashTableAccess();
	}
	chainEnumerator = new OULinkedListEnumerator<T>(hashTable->table[bucket]->enumerator());
}

// destructor
template <typename T>
HashTableEnumerator<T>::~HashTableEnumerator() {
	delete hashTable;
}

// true if there are elements that have not yet been returned via next()
template <typename T>
bool HashTableEnumerator<T>::hasNext() const {
	unsigned long tempBucket = bucket;
	if (chainEnumerator->hasNext()) {
		return true;
	}
	else {
		while (true) {
			tempBucket++;
			if (tempBucket >= hashTable->baseCapacity) {
				return false;
			}
			if (hashTable->table[tempBucket] != NULL) {
				return true;
			}
		}
	}
}

// throws ExceptionEnumerationBeyondEnd if no next item is available
template <typename T>
T HashTableEnumerator<T>::next() {
	if (!hasNext()) {
		throw new ExceptionEnumerationBeyondEnd();
	}
	if (chainEnumerator->hasNext()) {
		return chainEnumerator->next();
	}
	else {
		while (true) {
			bucket++;
			if (hashTable->table[bucket] != NULL) {
				chainEnumerator = new OULinkedListEnumerator<T>(hashTable->table[bucket]->enumerator());
				return chainEnumerator->next();
			}
		}
	}
}

// throws ExceptionEnumerationBeyondEnd if no next item is available
template <typename T>
T HashTableEnumerator<T>::peek() const {
	unsigned long tempBucket = bucket;
	OULinkedListEnumerator<T>* tempChainEnumerator = chainEnumerator;
	if (!hasNext()) {
		throw new ExceptionEnumerationBeyondEnd();
	}
	if (chainEnumerator->hasNext()) {
		return chainEnumerator->peek();
	}
	else {
		while (true) {
			tempBucket++;
			if (hashTable->table[tempBucket] != NULL) {
				tempChainEnumerator = new OULinkedListEnumerator<T>(hashTable->table[tempBucket]->enumerator());
				return tempChainEnumerator->peek();
			}
		}
	}
}

#endif // !HASH_TABLE_ENUMERATOR