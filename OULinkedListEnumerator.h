#ifndef OU_LINKED_LIST_ENUMERATOR
#define OU_LINKED_LIST_ENUMERATOR

#include "Enumerator.h"
#include "OULink.h"
#include "Exceptions.h"

template <typename T>
class OULinkedListEnumerator : public Enumerator<T>
{
private:
	OULink<T>* current;
public:
	OULinkedListEnumerator(OULink<T>* first);
	bool hasNext() const;
	T next();
	T peek() const;
};

/* -- Implementation goes here -- */
template<typename T>
OULinkedListEnumerator<T>::OULinkedListEnumerator(OULink<T>* first) {
	current = first;
}

template<typename T>
bool OULinkedListEnumerator<T>::hasNext() const {
	if (current != NULL) {
		return true;
	}
	return false;
}

template<typename T>
T OULinkedListEnumerator<T>::next() {
	if (!hasNext()) {
		throw new ExceptionEnumerationBeyondEnd();
	}
	T temp = *(current->data);
	current = current->next;
	return temp;
}

template<typename T>
T OULinkedListEnumerator<T>::peek() const {
	if (!hasNext()) {
		throw new ExceptionEnumerationBeyondEnd();
	}
	return *(current->data);
}

#endif // !OU_LINKED_LIST_ENUMERATOR