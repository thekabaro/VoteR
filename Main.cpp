#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "NvraRecord.h"
#include "TemplatedArray.h"
#include "Exceptions.h"
#include "Search.h"
#include "NvraComparator.h"
#include "Sorter.h"
using namespace std;

int main() {
	Sorter<NvraRecord> sorter;                         // used to sort
	TemplatedArray<NvraRecord> myNvraArray;            // array that holds all of the final, merged data
	unsigned long totalRead = 0;                       // keep track of lines read in
	unsigned long totalValid = 0;                      // keep track of lines that were valid

	/* ----- DATA INPUT LOOP ----- */
	while (true) {

		// read file
		ifstream inFS;
		string fileName;
		cout << "Enter data file name: ";
		getline(cin, fileName);
		if (fileName == "") {             // exit data loop
			break;
		}
		inFS.open(fileName);
		if (!inFS.is_open()) {
			cout << "File is not available." << endl;
			continue;
		}

		TemplatedArray<NvraRecord> currentArray;       // temp array specific to this file
		string currentLine;                            // holds current line
		int lineNumber = 1;                            // keep track of line numbers
		bool negativeFound = false;
		bool duplicateFound = false;

		// clear header line
		getline(inFS, currentLine);

		// parse through all of the lines in the file
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
			for (unsigned long i = 0; i < 23; i++) {
				if (isdigit(myArray[i].at(0)) && (isdigit(myArray[i].at(myArray[i].length() - 1)))) {
					currentRecord->addNum(stoi(myArray[i]));
				}
				else {
					currentRecord->addString(myArray[i]);
				}
			}
			currentRecord->addNum(stoi(myArray[23]));

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
		// (add everything from the temp array to the final array, replacing duplicates)
		NvraComparator comparator(0);
		for (unsigned long i = 0; i < currentArray.getSize(); i++) {
			long long result = binarySearch(currentArray[i], myNvraArray, comparator);
			NvraRecord tempRecord = currentArray[i];
			if (result >= 0) {
				myNvraArray.replaceAt(&tempRecord, result);
			}
			else {
				myNvraArray.add(&tempRecord);
			}
		}
	}

	// if no data was read in, exit program
	if (myNvraArray.getSize() == 0) {
		return 1;
	}

	// sort the array on column 0
	NvraComparator comparator(0);
	sorter.sort(myNvraArray, comparator);

	// keeps track of the last column sorted
	// (to determine binary or linear search when using find)
	int colSorted = 0;

	/* ------ DATA MANIPULATION LOOP ------ */
	while (true) {

		string userInput;
		cout << "Enter (o)utput, (s)ort, (f)ind, or (q)uit: " << endl;
		getline(cin, userInput);

		// output
		if (userInput == "o") {

			while (true) {
				string fileName;
				cout << "Enter output file name: ";
				getline(cin, fileName);
				if (fileName != "") {
					ofstream outFS;
					outFS.open(fileName);
					if (!outFS.is_open()) {
						cout << "File is not available." << endl;
						continue;
					}
					for (unsigned long i = 0; i < myNvraArray.getSize(); i++) {
						outFS << myNvraArray[i] << endl;
					}
					outFS << "Data lines read: " << totalRead << "; Valid NVRA records read: " << totalValid << "; NVRA records in memory: " << myNvraArray.getSize() << endl;
					outFS.close();
					break;
				}
				else {
					for (unsigned long i = 0; i < myNvraArray.getSize(); i++) {
						cout << myNvraArray[i] << endl;
					}
					cout << "Data lines read: " << totalRead << "; Valid NVRA records read: " << totalValid << "; NVRA records in memory: " << myNvraArray.getSize() << endl;
					break;
				}
			}
		}
		// search
		else if (userInput == "s") {

			string input;
			cout << "Enter sort field (0-23): ";
			getline(cin, input);
			int sortField = stoi(input);
			if (sortField < 0 || sortField > 23) {
				continue;
			}
			NvraComparator comparator(sortField);
			sorter.sort(myNvraArray, comparator);
			colSorted = sortField;
		}
		// find
		else if (userInput == "f") {

			// get the search field
			string input;
			cout << "Enter search field (0-23): ";
			getline(cin, input);
			int sortField = stoi(input);
			if (sortField < 0 || sortField > 23) {
				continue;
			}

			// if the column contains a string
			if (sortField == 3 || sortField == 11 || sortField == 12) {

				// get the string value to search for
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

				// if the column is sorted
				if (colSorted == sortField) {

					// perform a binary search
					unsigned long total = 0;
					long long searchResult = binarySearch(*tempRecord, myNvraArray, comparator);
					if (searchResult >= 0) {
						unsigned long result = searchResult;
						if ((result < myNvraArray.getSize()) && (comparator.compare(myNvraArray[result + 1], *tempRecord) == 0)) {
							cout << myNvraArray[result] << endl;
							result++;
							total++;
							while ((result < myNvraArray.getSize()) && (comparator.compare(myNvraArray[result], *tempRecord) == 0)) {
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
				// if the column is NOT sorted
				else {

					// perform a linear search
					unsigned long total = 0;
					for (unsigned long i = 0; i < myNvraArray.getSize(); i++) {
						if (comparator.compare(myNvraArray[i], *tempRecord) == 0) {
							cout << myNvraArray[i] << endl;
							total++;
						}
					}
					if (total != 0) {
						cout << "NVRA records found: " << total << "." << endl;
					}
					else {
						cout << "NVRA records found: 0." << endl;
					}
				}
			}
			// if the column contains a number
			else {

				// get the int value to search for
				string input;
				cout << "Enter non-negative field value: ";
				getline(cin, input);
				int value = stoi(input);
				if (value < 0) {
					continue;
				}
				unsigned int fieldValue = value;
				NvraRecord* tempRecord = new NvraRecord;
				unsigned int actualSortField = 0;
				for (int i = 0; i < sortField; i++) {
					if (i == 3 || i == 11 || i == 12) {
						continue;
					}
					actualSortField++;
				}
				tempRecord->setNum(fieldValue, actualSortField);
				NvraComparator comparator(sortField);

				// if the column is sorted
				if (colSorted == sortField) {

					// perform a binary search
					unsigned long total = 0;
					long long searchResult = binarySearch(*tempRecord, myNvraArray, comparator);
					if (searchResult >= 0) {
						unsigned long result = searchResult;
						if ((result < myNvraArray.getSize()) && (comparator.compare(myNvraArray[result + 1], *tempRecord) == 0)) {
							cout << myNvraArray[result] << endl;
							result++;
							total++;
							while ((result < myNvraArray.getSize()) && (comparator.compare(myNvraArray[result], *tempRecord) == 0)) {
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
				// if the column is NOT sorted
				else {

					// perform a linear search
					unsigned long total = 0;
					for (unsigned long i = 0; i < myNvraArray.getSize(); i++) {
						if (comparator.compare(myNvraArray[i], *tempRecord) == 0) {
							cout << myNvraArray[i] << endl;
							total++;
						}
					}
					if (total != 0) {
						cout << "NVRA records found: " << total << "." << endl;
					}
					else {
						cout << "NVRA records found: 0." << endl;
					}
				}
			}
		}
		// quit
		else if (userInput == "q") {

			// exit the program
			cout << "Thanks for using VoteR." << endl;
			break;
		}
	}

	return 0;
}
