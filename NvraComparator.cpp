#include "NvraComparator.h"

NvraComparator::NvraComparator(unsigned int column) {
	this->column = column;
}

// returns -1 if item1 < item2, 0 if item1 == item2, +1 if item1 > item2
int NvraComparator::compare(const NvraRecord& item1, const NvraRecord& item2) const {
	unsigned int numCtr = 0;
	unsigned int strCtr = 0;
	bool isNum;
	// determine if the column contains a number or a string
	for (unsigned int i = 0; i <= column; i++) {
		if (i == column) {
			if (i == 3 || i == 11 || i == 12) {
				isNum = false;
				break;
			}
			else {
				isNum = true;
				break;
			}
		}
		if (i == 3 || i == 11 || i == 12) {
			strCtr++;
		}
		else {
			numCtr++;
		}
	}
	// if the column contains a number
	if (isNum) {
		if (item1.getNum(numCtr) < item2.getNum(numCtr)) {
			return -1;
		}
		else if (item1.getNum(numCtr) == item2.getNum(numCtr)) {
			return 0;
		}
		else {
			return 1;
		}
	}
	// if the column contains a string
	else {
		if (item1.getString(strCtr) < item2.getString(strCtr)) {
			return -1;
		}
		else if (item1.getString(strCtr) == item2.getString(strCtr)) {
			return 0;
		}
		else {
			return 1;
		}
	}
}
