#ifndef FILE_H
#define FILE_H
#include "contact.h"

// File I/O operations
void saveContactsToFile(const AddressBook *addressBook);
void loadContactsFromFile(AddressBook *addressBook);

#endif
