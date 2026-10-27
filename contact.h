#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100
#define MAX_NAME 50
#define MAX_PHONE 20
#define MAX_EMAIL 50

typedef struct {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
} Contact;

typedef struct {
    Contact contact[MAX_CONTACTS];
    int contactCount;
} AddressBook;

// Core contact operations
void initialize(AddressBook *addressBook);
void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(const AddressBook *addressBook);

#endif
