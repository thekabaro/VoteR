#ifndef NVRA_ARRAY_H
#define NVRA_ARRAY_H

#include "NvraRecord.h"

const int DEFAULT_ARRAY_CAPACITY = 10;			// capacity used in no arg constructor

class NvraArray {
private:
	int capacity = DEFAULT_ARRAY_CAPACITY;		// maximum capacity, in records
	int size = 0;								// actual number of items currently in array
	NvraRecord* data = NULL;					// pointer to array of records
public:
	NvraArray();								// constructs array with default capacity
	NvraArray(int capacity);					// constructs array with specified capacity
	virtual ~NvraArray();						// frees array space as object is deleted
	void add(NvraRecord* record);				// adds record, increments size, doubles capacity as necessary
	NvraRecord get(int index);					// returns (copy of) record at index
};
#endif
