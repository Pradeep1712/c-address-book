#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"

// Initialize address book and load existing records from file
void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
}

// Format and display all stored contacts
void listContacts(const AddressBook *addressBook) {
    printf("\nNumber of contacts: %d\n", addressBook->contactCount);
    if (addressBook->contactCount == 0) return;

    printf("\n%-5s %-25s %-15s %-30s\n", "S.No", "Name", "Phone Number", "Email ID");
    for (int i = 0; i < addressBook->contactCount; i++) {
        printf("%-5d %-25s %-15s %-30s\n", i + 1, addressBook->contact[i].name, addressBook->contact[i].phone, addressBook->contact[i].email);
    }
}

// Validate name format: min 4 characters, containing only letters and spaces
static int isValidName(const char *name) {
    int len = strlen(name);
    if (len < 4) return 0;
    for (int i = 0; i < len; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

// Validate phone format: exactly 10 digits starting with 6, 7, 8, or 9
static int isValidPhone(const char *phone) {
    if (strlen(phone) != 10) return 0;
    if (phone[0] < '6' || phone[0] > '9') return 0;
    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

// Validate email format: lowercase only, valid characters, includes @ and .com
static int isValidEmail(const char *email) {
    int len = strlen(email);
    if (len < 6) return 0;
    
    int atIdx = -1, dotIdx = -1;
    for (int i = 0; i < len; i++) {
        if (isupper(email[i])) return 0;
        if (email[i] == '@') {
            if (atIdx != -1) return 0;
            atIdx = i;
        } else if (email[i] == '.') {
            dotIdx = i;
        } else if (!islower(email[i]) && !isdigit(email[i])) {
            return 0;
        }
    }
    
    if (atIdx == -1 || dotIdx == -1 || atIdx > dotIdx) return 0;
    if (dotIdx - atIdx < 5) return 0; 
    if (strcmp(email + len - 4, ".com") != 0) return 0;
    
    return 1;
}

// Check for phone number duplication, allowing exclusion of a specific index during edits
static int isDuplicatePhone(const AddressBook *ab, const char *phone, int excludeIndex) {
    for (int i = 0; i < ab->contactCount; i++) {
        if (i != excludeIndex && strcmp(ab->contact[i].phone, phone) == 0) return 1;
    }
    return 0;
}

// Check for email duplication, allowing exclusion of a specific index during edits
static int isDuplicateEmail(const AddressBook *ab, const char *email, int excludeIndex) {
    for (int i = 0; i < ab->contactCount; i++) {
        if (i != excludeIndex && strcmp(ab->contact[i].email, email) == 0) return 1;
    }
    return 0;
}

// Generic input handler with validation and a 3-attempt limit
static int getInput(const char *prompt, char *buffer, int (*validate)(const char*), const AddressBook *ab, int dupType, int excludeIndex) {
    for (int attempt = 1; attempt <= 3; attempt++) {
        printf("%s", prompt);
        scanf(" %[^\n]", buffer);
        
        if (!validate(buffer)) {
            printf("Error: Invalid format.\n");
        } else if (dupType == 1 && isDuplicatePhone(ab, buffer, excludeIndex)) {
            printf("Error: Phone number already exists.\n");
        } else if (dupType == 2 && isDuplicateEmail(ab, buffer, excludeIndex)) {
            printf("Error: Email already exists.\n");
        } else {
            return 1;
        }
        
        if (attempt < 3) printf("%d attempts remaining.\n", 3 - attempt);
    }
    printf("All attempts exhausted. Returning to main menu.\n");
    return 0;
}

// Add a new contact to the address book
void createContact(AddressBook *addressBook) {
    if (addressBook->contactCount >= MAX_CONTACTS) {
        printf("Address book is full.\n");
        return;
    }

    char name[MAX_NAME], phone[MAX_PHONE], email[MAX_EMAIL];

    if (!getInput("\nEnter name (min 4 chars): ", name, isValidName, NULL, 0, -1)) return;
    if (!getInput("Enter phone (10 digits, starts 6-9): ", phone, isValidPhone, addressBook, 1, -1)) return;
    if (!getInput("Enter email: ", email, isValidEmail, addressBook, 2, -1)) return;

    strcpy(addressBook->contact[addressBook->contactCount].name, name);
    strcpy(addressBook->contact[addressBook->contactCount].phone, phone);
    strcpy(addressBook->contact[addressBook->contactCount].email, email);
    addressBook->contactCount++;
    
    printf("Contact created successfully.\n");
}

// Helper function to search contacts and populate an array of matching indices
static int findContacts(const AddressBook *addressBook, int option, const char *query, int *matchIndices) {
    int count = 0;
    for (int i = 0; i < addressBook->contactCount; i++) {
        int match = 0;
        if (option == 1 && strcmp(addressBook->contact[i].name, query) == 0) match = 1;
        else if (option == 2 && strcmp(addressBook->contact[i].phone, query) == 0) match = 1;
        else if (option == 3 && strcmp(addressBook->contact[i].email, query) == 0) match = 1;
        
        if (match) matchIndices[count++] = i;
    }
    return count;
}

// Search and display contacts
void searchContact(AddressBook *addressBook) {
    int option;
    char query[MAX_EMAIL], input[10];
    int matchIndices[MAX_CONTACTS];

    printf("\nSearch by: 1. Name  2. Phone  3. Email  4. Exit\nChoice: ");
    scanf(" %[^\n]", input);
    option = atoi(input);

    if (option < 1 || option > 3) return;

    printf("Enter search term: ");
    scanf(" %[^\n]", query);

    int count = findContacts(addressBook, option, query, matchIndices);
    
    if (count == 0) {
        printf("No matching contacts found.\n");
    } else {
        printf("\nFound %d contact(s):\n", count);
        printf("%-5s %-25s %-15s %-30s\n", "S.No", "Name", "Phone Number", "Email ID");
        for (int i = 0; i < count; i++) {
            Contact *c = &addressBook->contact[matchIndices[i]];
            printf("%-5d %-25s %-15s %-30s\n", i + 1, c->name, c->phone, c->email);
        }
    }
}

// Edit a specific field of an existing contact
void editContact(AddressBook *addressBook) {
    int option;
    char query[MAX_EMAIL], input[10];
    int matchIndices[MAX_CONTACTS];

    printf("\nSearch contact to edit by: 1. Name  2. Phone  3. Email\nChoice: ");
    scanf(" %[^\n]", input);
    option = atoi(input);

    if (option < 1 || option > 3) return;

    printf("Enter search term: ");
    scanf(" %[^\n]", query);

    int count = findContacts(addressBook, option, query, matchIndices);
    if (count == 0) {
        printf("No matching contacts found.\n");
        return;
    }

    printf("\nFound %d contact(s):\n", count);
    for (int i = 0; i < count; i++) {
        Contact *c = &addressBook->contact[matchIndices[i]];
        printf("%d. %-20s %-15s %s\n", i + 1, c->name, c->phone, c->email);
    }

    printf("\nEnter the S.No to edit (1-%d): ", count);
    scanf(" %[^\n]", input);
    int serial = atoi(input);
    if (serial < 1 || serial > count) {
        printf("Invalid selection.\n");
        return;
    }

    int targetIdx = matchIndices[serial - 1];
    
    printf("Edit: 1. Name  2. Phone  3. Email\nChoice: ");
    scanf(" %[^\n]", input);
    int editOption = atoi(input);

    char newVal[MAX_EMAIL];
    if (editOption == 1) {
        if (getInput("Enter new name: ", newVal, isValidName, NULL, 0, targetIdx)) {
            strcpy(addressBook->contact[targetIdx].name, newVal);
            printf("Name updated.\n");
        }
    } else if (editOption == 2) {
        if (getInput("Enter new phone: ", newVal, isValidPhone, addressBook, 1, targetIdx)) {
            strcpy(addressBook->contact[targetIdx].phone, newVal);
            printf("Phone updated.\n");
        }
    } else if (editOption == 3) {
        if (getInput("Enter new email: ", newVal, isValidEmail, addressBook, 2, targetIdx)) {
            strcpy(addressBook->contact[targetIdx].email, newVal);
            printf("Email updated.\n");
        }
    }
}

// Delete a contact and shift subsequent array elements
void deleteContact(AddressBook *addressBook) {
    int option;
    char query[MAX_EMAIL], input[10];
    int matchIndices[MAX_CONTACTS];

    printf("\nSearch contact to delete by: 1. Name  2. Phone  3. Email\nChoice: ");
    scanf(" %[^\n]", input);
    option = atoi(input);

    if (option < 1 || option > 3) return;

    printf("Enter search term: ");
    scanf(" %[^\n]", query);

    int count = findContacts(addressBook, option, query, matchIndices);
    if (count == 0) {
        printf("No matching contacts found.\n");
        return;
    }

    printf("\nFound %d contact(s):\n", count);
    for (int i = 0; i < count; i++) {
        Contact *c = &addressBook->contact[matchIndices[i]];
        printf("%d. %-20s %-15s %s\n", i + 1, c->name, c->phone, c->email);
    }

    printf("\nEnter the S.No to delete (1-%d): ", count);
    scanf(" %[^\n]", input);
    int serial = atoi(input);
    if (serial < 1 || serial > count) {
        printf("Invalid selection.\n");
        return;
    }

    int targetIdx = matchIndices[serial - 1];
    for (int i = targetIdx; i < addressBook->contactCount - 1; i++) {
        addressBook->contact[i] = addressBook->contact[i + 1];
    }
    
    addressBook->contactCount--;
    printf("Contact deleted successfully.\n");
}
