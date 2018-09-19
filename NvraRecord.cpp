#include <iostream>
using namespace std;

#include "NvraRecord.h"

// overloaded output operator that sends one formatted record to ostream
ostream& operator<<(std::ostream& os, const NvraRecord& record) {
	// FIXME
}

// puts num in array, increments counter
void NvraRecord::addNum(int num) {
	// FIXME
}

// puts string in array, increments counter
void NvraRecord::addString(string string) {
	// FIXME
}

// returns (copy of) num at index in array
int NvraRecord::getNum(int index) {
	// FIXME
}

// returns (copy of) string at index in array
string NvraRecord::getString(int index) {
	// FIXME
}
