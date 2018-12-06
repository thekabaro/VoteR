#include <iostream>
#include <string>
using namespace std;

#include "NvraRecord.h"

// overloaded output operator that sends one formatted record to ostream
ostream& operator<<(std::ostream& os, const NvraRecord& record) {
	unsigned int numCounter = 0;
	unsigned int strCounter = 0;
	for (unsigned long i = 0; i < 23; i++) {
		if (i == 3 || i == 11 || i == 12) {
			os << record.strings[strCounter];
			strCounter++;
		}
		else {
			os << record.nums[numCounter];
			numCounter++;
		}
		os << ';';
	}
	os << record.nums[20];
	return os;
}

// puts num in array, increments counter
void NvraRecord::addNum(unsigned int num) {
	if (numCtr == MAX_NUMS) {
		throw new ExceptionIndexOutOfRange();
	}
	nums[numCtr] = num;
	numCtr++;
}

// puts string in array, increments counter
void NvraRecord::addString(string string) {
	if (strCtr == MAX_STRINGS) {
		throw new ExceptionIndexOutOfRange();
	}
	strings[strCtr] = string;
	strCtr++;
}

// returns (copy of) num at index in array
unsigned int NvraRecord::getNum(unsigned int index) const {
	if (index >= MAX_NUMS) {
		throw new ExceptionIndexOutOfRange();
	}
	return nums[index];
}

// returns (copy of) string at index in array
string NvraRecord::getString(unsigned int index) const {
	if (index >= MAX_STRINGS) {
		throw new ExceptionIndexOutOfRange();
	}
	return strings[index];
}

// sets nums[index] to num, doesn't change counter
void NvraRecord::setNum(unsigned int num, unsigned int index) {
	if (index >= MAX_NUMS) {
		throw new ExceptionIndexOutOfRange();
	}
	nums[index] = num;
}

// sets strings[index] to string, doesn't change counter
void NvraRecord::setString(std::string string, unsigned int index) {
	if (index >= MAX_STRINGS) {
		throw new ExceptionIndexOutOfRange();
	}
	strings[index] = string;
}