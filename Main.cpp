#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

#include "NvraRecord.h"
#include "TemplatedArray.h"
#include "Exceptions.h"

int main()
{
	string currentLine;
	vector<int> recordIDs;
	int lineNumber = 1;
	bool duplicateFound = false;
	bool negativeFound = false;
	int currentDataNum = 0;
	TemplatedArray<NvraRecord> myNvraArray;

	// Ignore headers
	getline(cin, currentLine);

	// Loop through each line
	while (getline(cin, currentLine)) {

		// Check for negatives and print
		negativeFound = false;
		for (unsigned long i = 0; i < currentLine.length(); i++) {
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

		// Check if duplicate and print
		duplicateFound = false;
		int recordID = stoi(currentLine.substr(0, currentLine.find(',')));
		for (unsigned long i = 0; i < recordIDs.size(); i++) {
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
		stringstream mySS(currentLine);
		string group;
		string myArray[24];
		int i = 0;
		while (getline(mySS, group, ',')) {
			myArray[i] = group;
			i++;
		}

		// Create record and store data
		NvraRecord* currentRecord = new NvraRecord;
		for (unsigned long i = 0; i < 24; i++) {
			if (isdigit(myArray[i].at(myArray[i].length() - 1))) {
				currentRecord->addNum(stoi(myArray[i]));
			}
			else {
				currentRecord->addString(myArray[i]);
			}
		}

		// Add record to NvraArray
		myNvraArray.add(currentRecord);
		currentDataNum++;
	}

	// Print out valid lines backwards
	for (int i = currentDataNum - 1; i >= 0; i--) {
		cout << myNvraArray.get(i);
		cout << endl;
	}

	return 0;
}
