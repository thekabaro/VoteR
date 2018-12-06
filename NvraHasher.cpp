#include "NvraHasher.h"

// simply return the record ID
unsigned long NvraHasher::hash(const NvraRecord& item) const {
	return item.getNum(0);
}