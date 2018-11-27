#ifndef OU_LINKED_LIST
#define OU_LINKED_LIST

#include "OULink.h"
#include "Comparator.h"
#include "OULinkedListEnumerator.h"
#include "Exceptions.h"

// OULinkedList stands for Ordered, Unique Linked List. It is a linked list that is always maintained in
// order (based on the comparator provided to it when the list is created) and that only contains unique
// items (that is, duplicates are not allowed)
template <typename T>
class OULinkedList {
	// template <typename T>  // not necessary
	// friend class OULinkedListEnumerator;  // not necessary
private:
	Comparator<T>* comparator = NULL;				// used to determine list order and item equality
	unsigned long size = 0;							// actual number of items currently in list
	OULink<T>* first = NULL;						// pointer to first link in list
	OULink<T>* last = NULL;							// pointer to last link in list
public:
	OULinkedList(Comparator<T>* comparator);		// creates empty linked list with comparator
	virtual ~OULinkedList();						// deletes all links and their data items

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave list unchanged and return false
	bool insert(const T* item);

	// if item is greater than item at last, append item at end and return true
	// if item is less than or equal to item at last, leave list unchanged and return false
	bool append(const T* item);  // now const

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool replace(const T* item);  // now const

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool remove(const T* item);  // now const

	// if any items are present, return a copy of the first item
	// if no items are present, throw new ExceptionLinkedListAccess
	T get() const;

	// if an equivalent item is present, return a copy of the first such item
	// if an equivalent item is not present, throw a new ExceptionLinkedListAccess
	T find(const T* item) const;

	unsigned long getSize() const;					// returns the current number of items in the list

	OULinkedListEnumerator<T> enumerator() const;	// create an enumerator for this linked list
};

/* -- Implementation goes here -- */
template<typename T>
OULinkedList<T>::OULinkedList(Comparator<T>* comparator) {
	this->comparator = comparator;
}

template<typename T>
OULinkedList<T>::~OULinkedList() {
	if (first != NULL) {
		delete first;
	}
}

template<typename T>
bool OULinkedList<T>::insert(const T* item) {
	if (first == NULL) {
		OULink<T>* newLink = new OULink<T>(item);
		first = newLink;
		last = newLink;
		size++;
		return true;
	}
	else {
		if (comparator->compare(*item, *last->data) == 1) {
			OULink<T>* newLink = new OULink<T>(item);
			last->next = newLink;
			last = newLink;
			size++;
			return true;
		}
		else if (comparator->compare(*item, *first->data) == -1) {
			OULink<T>* newLink = new OULink<T>(item);
			newLink->next = first;
			first = newLink;
			size++;
			return true;
		}
		else {
			OULink<T>* currLink = first;
			OULink<T>* prevLink = NULL;
			for (unsigned long i = 0; i < size; i++) {
				if (comparator->compare(*item, *currLink->data) <= 0) {
					if (comparator->compare(*item, *currLink->data) == -1) {
						OULink<T>* newLink = new OULink<T>(item);
						prevLink->next = newLink;
						newLink->next = currLink;
						size++;
						return true;
					}
					else {
						return false;
					}
				}
				prevLink = currLink;
				currLink = currLink->next;
			}
			return true;
		}
	}
}

template<typename T>
bool OULinkedList<T>::append(const T* item) {
	if (first == NULL) {
		OULink<T>* newLink = new OULink<T>(item);
		first = newLink;
		last = newLink;
		size++;
		return true;
	}
	else {
		if (comparator->compare(*item, *last->data) == 1) {
			OULink<T>* newLink = new OULink<T>(item);
			last->next = newLink;
			last = newLink;
			size++;
			return true;
		}
		else {
			return false;
		}
	}
}

template<typename T>
bool OULinkedList<T>::replace(const T* item) {
	if (first == NULL) {
		return false;
	}
	else if (comparator->compare(*item, *first->data) == 0) {
		OULink<T>* newLink = new OULink<T>(item);
		delete first->data;
		newLink->next = first->next;
		first = newLink;
		return true;
	}
	else {
		OULink<T>* currLink = first;
		OULink<T>* prevLink = NULL;
		for (unsigned long i = 0; i < size; i++) {
			if (comparator->compare(*item, *currLink->data) == 0) {
				OULink<T>* newLink = new OULink<T>(item);
				delete currLink->data;
				newLink->next = currLink->next;
				prevLink->next = newLink;
				if (i == size - 1) {
					last = newLink;
				}
				return true;
			}
			prevLink = currLink;
			currLink = currLink->next;
		}
		return false;
	}
}

template<typename T>
bool OULinkedList<T>::remove(const T* item) {
	if (first == NULL) {
		return false;
	}
	else if (comparator->compare(*item, *first->data) == 0) {
		if (first->next != NULL) {
			delete first->data;
			first = first->next;
			size--;
			return true;
		}
		else {
			delete first->data;
			first = NULL;
			size--;
			return true;
		}
	}
	else {
		OULink<T>* currLink = first;
		OULink<T>* prevLink = NULL;
		for (unsigned long i = 0; i < size; i++) {
			if (comparator->compare(*item, *currLink->data) == 0) {
				if (i == size - 1) {
					delete currLink->data;
					prevLink->next = NULL;
					last = prevLink;
					size--;
					return true;
				}
				else {
					delete currLink->data;
					prevLink->next = currLink->next;
					size--;
					return true;
				}
			}
			prevLink = currLink;
			currLink = currLink->next;
		}
		return false;
	}
}

template<typename T>
T OULinkedList<T>::get() const {
	if (first == NULL) {
		throw new ExceptionLinkedListAccess();
	}
	return *first->data;
}

template<typename T>
T OULinkedList<T>::find(const T* item) const {
	OULink<T>* currLink = first;
	for (unsigned long i = 0; i < size; i++) {
		if (comparator->compare(*item, *currLink->data) == 0) {
			return *currLink->data;
		}
	}
	throw new ExceptionLinkedListAccess();
}

template<typename T>
unsigned long OULinkedList<T>::getSize() const {
	return size;
}

template<typename T>
OULinkedListEnumerator<T> OULinkedList<T>::enumerator() const {
	OULinkedListEnumerator<T> myEnum(first);
	return myEnum;
}

#endif // !OU_LINKED_LIST
