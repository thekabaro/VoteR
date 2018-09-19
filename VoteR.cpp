#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

#include "NvraRecord.h"
#include "NvraArray.h"

struct nvraRecord {
	int nums[21];           // holds the numeric data, in column order
	string strings[3];      // holds the string data, in column order
};

struct nvraArray {
	int capacity;               // maximum capacity, in records
	nvraRecord* data = NULL;    // pointer to array of records
};

nvraArray* doubleNvraArray(nvraArray* currentNvraArray) {
	nvraRecord* myNewRecordArray = new nvraRecord[currentNvraArray->capacity * 2];
	nvraArray* myNewArray = new nvraArray;
	myNewArray->data = myNewRecordArray;
	myNewArray->capacity = currentNvraArray->capacity * 2;
	for (int i = 0; i < currentNvraArray->capacity; i++) {
		myNewRecordArray[i] = currentNvraArray->data[i];
	}
	delete[] currentNvraArray->data;
	currentNvraArray->data = NULL;
	delete currentNvraArray;
	currentNvraArray = NULL;
	return myNewArray;
}

int main()
{
	string currentLine;
	vector<int> recordIDs;
	int lineNumber = 1;
	bool duplicateFound = false;
	bool negativeFound = false;

	int currentDataNum = 0;


	nvraRecord* recordArray = new nvraRecord[10];
	nvraArray* dataCollection = new nvraArray;
	dataCollection->capacity = 10;
	dataCollection->data = recordArray;

	// Ignore headers
	getline(cin, currentLine);

	// Loop through each line
	while (getline(cin, currentLine)) {

		// Check for negatives
		negativeFound = false;
		for (unsigned int i = 0; i < currentLine.length(); i++) {
			if (currentLine.at(i) == '-' && isdigit(currentLine.at(i + 1))) {
				cout << "Invalid data at line " << lineNumber << "." << endl;
				negativeFound = true;
				break;
			}
		}
		if (negativeFound) {
			lineNumber++;
			continue;
		}

		// Check if duplicate
		duplicateFound = false;
		int recordID = stoi(currentLine.substr(0, currentLine.find(',')));
		for (unsigned int i = 0; i < recordIDs.size(); i++) {
			if (recordIDs[i] == recordID) {
				cout << "Duplicate record ID " << recordID << " at line " << lineNumber << "." << endl;
				duplicateFound = true;
				break;
			}
		}
		lineNumber++;
		if (duplicateFound) {
			continue;
		}
		recordIDs.push_back(recordID);

		// Split sting into an array of strings
		stringstream mySS(currentLine);                // https://stackoverflow.com/questions/11719538/how-to-use-stringstream-to-separate-comma-separated-strings
		string group;                                  // Used user Yola's code to help me split a string into an array of strings in C++
		string myArray[24];                            // (since there is no split function)
		int i = 0;                                     //
		while (getline(mySS, group, ',')) {            //
			myArray[i] = group;                        //
			i++;                                       //
		}                                              //

		// WORKSPACE --------------------------------------------

		nvraRecord currentRecord;
		currentRecord.strings[0] = myArray[3];
		currentRecord.strings[1] = myArray[11];
		currentRecord.strings[2] = myArray[12];
		currentRecord.nums[0] = stoi(myArray[0]);
		currentRecord.nums[1] = stoi(myArray[1]);
		currentRecord.nums[2] = stoi(myArray[2]);
		currentRecord.nums[3] = stoi(myArray[4]);
		currentRecord.nums[4] = stoi(myArray[5]);
		currentRecord.nums[5] = stoi(myArray[6]);
		currentRecord.nums[6] = stoi(myArray[7]);
		currentRecord.nums[7] = stoi(myArray[8]);
		currentRecord.nums[8] = stoi(myArray[9]);
		currentRecord.nums[9] = stoi(myArray[10]);
		currentRecord.nums[10] = stoi(myArray[13]);
		currentRecord.nums[11] = stoi(myArray[14]);
		currentRecord.nums[12] = stoi(myArray[15]);
		currentRecord.nums[13] = stoi(myArray[16]);
		currentRecord.nums[14] = stoi(myArray[17]);
		currentRecord.nums[15] = stoi(myArray[18]);
		currentRecord.nums[16] = stoi(myArray[19]);
		currentRecord.nums[17] = stoi(myArray[20]);
		currentRecord.nums[18] = stoi(myArray[21]);
		currentRecord.nums[19] = stoi(myArray[22]);
		currentRecord.nums[20] = stoi(myArray[23]);

		if (dataCollection->capacity < currentDataNum + 1) {
			dataCollection = doubleNvraArray(dataCollection);
		}

		dataCollection->data[currentDataNum] = currentRecord;
		currentDataNum++;

		// WORKSPACE --------------------------------------------

		/*
		// Output lines, taking off leading zeroes
		for (unsigned int i = 0; i < 24; i++) {
			if (isdigit(myArray[i].at(myArray[i].length() - 1))) {
				cout << stoi(myArray[i]);
			}
			else {
				cout << myArray[i];
			}
			if (i < 23) {
				cout << ";";
			}
		}
		cout << endl;
		*/
	}

	for (int i = currentDataNum - 1; i >= 0; i--) {
		nvraRecord current = dataCollection->data[i];
		for (int j = 0; j < 24; j++) {
			switch (j) {
			case 0: cout << current.nums[0]; break;
			case 1: cout << current.nums[1]; break;
			case 2: cout << current.nums[2]; break;
			case 3: cout << current.strings[0]; break;
			case 4: cout << current.nums[3]; break;
			case 5: cout << current.nums[4]; break;
			case 6: cout << current.nums[5]; break;
			case 7: cout << current.nums[6]; break;
			case 8: cout << current.nums[7]; break;
			case 9: cout << current.nums[8]; break;
			case 10: cout << current.nums[9]; break;
			case 11: cout << current.strings[1]; break;
			case 12: cout << current.strings[2]; break;
			case 13: cout << current.nums[10]; break;
			case 14: cout << current.nums[11]; break;
			case 15: cout << current.nums[12]; break;
			case 16: cout << current.nums[13]; break;
			case 17: cout << current.nums[14]; break;
			case 18: cout << current.nums[15]; break;
			case 19: cout << current.nums[16]; break;
			case 20: cout << current.nums[17]; break;
			case 21: cout << current.nums[18]; break;
			case 22: cout << current.nums[19]; break;
			case 23: cout << current.nums[20]; break;
			}
			if (j < 23) {
				cout << ";";
			}
		}
		cout << endl;
	}

	return 0;
}
