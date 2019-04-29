#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct contactInfo *node;

int main() {
  node head;
  char action;
  head = NULL;
  int numberOfContacts, numChoice;

  numberOfContacts = numChoice = 0;

  head = readFromFile(head);

  while(1) {
    printf("Number of Contacts = %d", numberOfContacts = getNumberOfContacts(head));
    printList(head, numberOfContacts);

    printf("Action: ");
    scanf("%c", &action);
    getchar();

    if (action == 'A' || action == 'a') {
      head = getInfo(head);
      action = '\0';
    } else if (action == 'X' || action == 'x') {
        writeToFile(head, numberOfContacts);
        if (numberOfContacts!= 0) {
          freeList(head);  
        }
        break;
    } else if ((numChoice = atoi(&action)) && numChoice <= numberOfContacts) {
      head = showOneContact(head, numChoice);
    }
  }
  return 0;
}
