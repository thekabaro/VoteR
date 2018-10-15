#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

#include "NvraRecord.h"
#include "TemplatedArray.h"
#include "Exceptions.h"
#include "Search.h"
#include "NvraComparator.h"
#include "Sorter.h"

int main()
{
	Sorter<NvraRecord> sorter;
	TemplatedArray<NvraRecord> myNvraArray;
	unsigned long totalRead = 0;
	unsigned long totalValid = 0;

	// data input loop
	while (true) {

		// read file
		ifstream inFS;
		string fileName;
		cout << "Enter data file name: ";
		getline(cin, fileName);
		if (fileName == "") {
			break;
		}
		inFS.open(fileName);
		if (!inFS.is_open()) {
			cout << "File is not available." << endl;
			continue;
		}

		TemplatedArray<NvraRecord> currentArray;
		string currentLine;
		int lineNumber = 1;
		bool negativeFound = false;
		bool duplicateFound = false;

		// clear header line
		getline(inFS, currentLine);

		while (getline(inFS, currentLine)) {

			// check for negatives and print
			for (unsigned long i = 0; i < currentLine.length(); i++) {
				if (currentLine.at(i) == '-' && isdigit(currentLine.at(i + 1))) {
					cout << "Invalid data at line " << lineNumber << "." << endl;
					negativeFound = true;
					break;
				}
			}
			if (negativeFound) {
				lineNumber++;
				totalRead++;
				negativeFound = false;
				continue;
			}

			// split sting into an array of strings
			stringstream mySS(currentLine);
			string group;
			string myArray[24];
			int i = 0;
			while (getline(mySS, group, ',')) {
				myArray[i] = group;
				i++;
			}

			// create record and store data
			NvraRecord* currentRecord = new NvraRecord;
			for (unsigned long i = 0; i < 24; i++) {
				if (isdigit(myArray[i].at(myArray[i].length() - 1))) {
					currentRecord->addNum(stoi(myArray[i]));
				}
				else {
					currentRecord->addString(myArray[i]);
				}
			}

			// check if duplicate and print
			duplicateFound = false;
			NvraComparator comparator(0);
			for (unsigned long i = 0; i < currentArray.getSize(); i++) {
				if (binarySearch(*currentRecord, currentArray, comparator) != -1) {
					cout << "Duplicate record ID " << currentRecord->getNum(0) << " at line " << lineNumber << "." << endl;
					duplicateFound = true;
					break;
				}
			}
			lineNumber++;
			totalRead++;
			totalValid++;
			if (duplicateFound) {
				continue;
			}

			// add record to NvraArray
			currentArray.add(currentRecord);
		}

		// check for duplicates between files
		NvraComparator comparator(0);
		for (unsigned long i = 0; i < currentArray.getSize(); i++) {
			long long result = binarySearch(currentArray[i], myNvraArray, comparator);
			if (result != -1) {
				myNvraArray.replaceAt(&currentArray[i], result);
			}
			else {
				myNvraArray.add(&currentArray[i]);
			}
		}
	}

	// sort the array
	NvraComparator comparator(0);
	sorter.sort(myNvraArray, comparator);

	int colSorted = 0;

	// data manipulation loop
	while (true) {
		string userInput;
		cout << "Enter (o)utput, (s)ort, (f)ind, or (q)uit: ";
		cin >> userInput;
		if (userInput == "o") {
			for (unsigned long i = 0; i < myNvraArray.getSize(); i++) {
				cout << myNvraArray[i] << endl;
			}
			cout << "Data lines read: " << totalRead << "; Valid NVRA records read: " << totalValid << "; NVRA records in memory: " << myNvraArray.getSize() << endl;
		}
		else if (userInput == "s") {
			int sortField;
			cout << "Enter sort field (0-23): ";
			cin >> sortField;
			if (sortField < 0 || sortField > 23) {
				continue;
			}
			NvraComparator comparator(sortField);
			sorter.sort(myNvraArray, comparator);
			colSorted = sortField;
		}
		else if (userInput == "f") {
			int sortField;
			cout << "Enter search field (0-23): ";
			cin >> sortField;
			if (sortField < 0 || sortField > 23) {
				continue;
			}
			// string column
			if (sortField == 3 || sortField == 11 || sortField == 12) {
				string fieldValue;
				cout << "Enter exact text on which to search: ";
				getline(cin, fieldValue);
				if (fieldValue == "") {
					continue;
				}
				NvraRecord* tempRecord = new NvraRecord;
				if (sortField == 3) {
					tempRecord->setString(fieldValue, 0);
				}
				else if (sortField == 11) {
					tempRecord->setString(fieldValue, 1);
				}
				else {
					tempRecord->setString(fieldValue, 2);
				}
				NvraComparator comparator(sortField);
				long long result = binarySearch(*tempRecord, myNvraArray, comparator);
				if (result != -1) {
					cout << myNvraArray[result] << endl;
				}
				else {
					cout << "NVRA records found: 0." << endl;
				}
			}
			// num column
			else {
				long long fieldValue;
				cout << "Enter non-negative field value: ";
				cin >> fieldValue;
				if (fieldValue < 0) {
					continue;
				}
				NvraRecord* tempRecord = new NvraRecord;
				int actualSortField = 0;
				for (int i = 0; i < sortField; i++) {
					if (i == 3 || i == 11 || i == 12) {
						continue;
					}
					actualSortField++;
				}
				tempRecord->setNum(fieldValue, actualSortField);
				NvraComparator comparator(sortField);
				// binary search
				if (colSorted == sortField) {
					unsigned long total = 0;
					long long result = binarySearch(*tempRecord, myNvraArray, comparator);
					if (result != -1) {
						if ((result < myNvraArray.getSize() - 1) && (comparator.compare(myNvraArray[result + 1], *tempRecord) == 0)) {
							cout << myNvraArray[result] << endl;
							result++;
							total++;
							while ((result < myNvraArray.getSize() - 1) && (comparator.compare(myNvraArray[result], *tempRecord) == 0)) {
								cout << myNvraArray[result] << endl;
								result++;
								total++;
							}
							cout << "NVRA records found: " << total << "." << endl;
						}
						else {
							cout << myNvraArray[result] << endl;
							cout << "NVRA records found: 1." << endl;
						}
					}
					else {
						cout << "NVRA records found: 0." << endl;
					}
				}
				// linear search
				else {

				}
			}
		}
		else if (userInput == "q") {
			cout << "Thanks for using VoteR." << endl;
			break;
		}
	}

	return 0;
}
