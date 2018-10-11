#include <iostream>
#include <string>
using namespace std;

#include "NvraRecord.h"

// overloaded output operator that sends one formatted record to ostream
ostream& operator<<(std::ostream& os, const NvraRecord& record) {
	int numCounter = 0;
	int strCounter = 0;
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
void NvraRecord::addNum(int num) {
	nums[numCtr] = num;
	numCtr++;
}

// puts string in array, increments counter
void NvraRecord::addString(string string) {
	strings[strCtr] = string;
	strCtr++;
}

// returns (copy of) num at index in array
int NvraRecord::getNum(int index) {
	return nums[index];
}

// returns (copy of) string at index in array
string NvraRecord::getString(int index) {
	return strings[index];
}
