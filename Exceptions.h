#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

// exceptions to throw
class Exception {};
class ExceptionIndexOutOfRange : Exception {};
class ExceptionMemoryNotAvailable : Exception {};
class ExceptionLinkedListAccess : Exception {};
class ExceptionEnumerationBeyondEnd : Exception {};

#endif
