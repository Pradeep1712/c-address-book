#include <stdio.h>
#include <stdlib.h>
#include "contact.h"
#include "file.h"

// Entry point and main execution loop
int main() {
    int choice;
    char input[50];
    AddressBook addressBook;
    
    initialize(&addressBook); 

    do {
        printf("\n--- Address Book Menu ---\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Save and Exit\n");		

        printf("Enter your choice: ");
        scanf(" %[^\n]", input);
        choice = atoi(input);

        switch (choice) {
            case 1: createContact(&addressBook); break;
            case 2: searchContact(&addressBook); break;
            case 3: editContact(&addressBook); break;
            case 4: deleteContact(&addressBook); break;
            case 5: listContacts(&addressBook); break;
            case 6: 
                printf("Saving and Exiting...\n");
                saveContactsToFile(&addressBook); 
                break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
    
    return 0;
}
