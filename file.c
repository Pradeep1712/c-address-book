#include <stdio.h>
#include <stdlib.h>
#include "file.h"

// Write all contacts to the CSV file
void saveContactsToFile(const AddressBook *addressBook) {
    FILE *fp = fopen("Contacts.csv", "w");
    if (!fp) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    fprintf(fp, "#%d\n", addressBook->contactCount);
    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fp, "%s,%s,%s\n", addressBook->contact[i].name, addressBook->contact[i].phone, addressBook->contact[i].email);
    }
    
    fclose(fp);
    printf("Contacts saved successfully.\n");
}

// Read contacts from the CSV file during initialization
void loadContactsFromFile(AddressBook *addressBook) {
    FILE *fp = fopen("Contacts.csv", "r");
    if (!fp) return;

    if (fscanf(fp, "#%d\n", &addressBook->contactCount) == 1) {
        for (int i = 0; i < addressBook->contactCount; i++) {
            fscanf(fp, "%[^,],%[^,],%[^\n]\n", addressBook->contact[i].name, addressBook->contact[i].phone, addressBook->contact[i].email);
        }
    }
    fclose(fp);
}
