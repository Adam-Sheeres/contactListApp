#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
typedef struct contactInfo *node;

node addNode(node head, char *firstName, char *lastName, char *companyName, char *email, unsigned long phoneNumber) {
  /*this function is to add a node to the linked list*/
  node temp, ptr;
  temp = ptr = NULL;

  /*create a new node*/
  temp = createNode();

  /*copy data to the node*/
  temp->firstName = malloc(sizeof(char) * strlen(firstName) + 1);
  strcpy(temp->firstName, firstName);

  temp->lastName = malloc(sizeof(char) * strlen(lastName) + 1);
  strcpy(temp->lastName, lastName);

  temp->companyName = malloc(sizeof(char) * strlen(companyName) + 1);
  strcpy(temp->companyName, companyName);

  temp->email = malloc(sizeof(char) * strlen(email) + 1);
  strcpy(temp->email, email);

  temp->phoneNumber = phoneNumber;

  /*check if this is the first node of the list*/
  if (head == NULL) {
    head = temp;
  } else {
    ptr = head;
    while (ptr->next != NULL) {
      ptr = ptr->next;
    }
    ptr->next = temp;
  }
  return head;
}

node createNode() {
  /*create a new node*/
  node temp;
  temp = (node)malloc(sizeof(struct contactInfo));
  temp->next = NULL;
  return temp;
}

void printList(node head, int numberOfContacts) {
  /*print the list of all nodes*/
  node temp;
  temp = NULL;
  int i;
  char alphabet;
  char prevAlph;
  /*sort the names first*/
  sortNames(head);

  alphabet = 'A';
  for (i = 0; i < numberOfContacts; i++) {
    temp = head;
    while(temp != NULL) {
      if (temp->order == i + 1) {
        if (strlen(temp->lastName) > 0) {
          alphabet = temp->lastName[0];
          if(prevAlph != alphabet) {
            printf("%c\n", alphabet);
            prevAlph = alphabet;
          }
          printf("  %d.  %s %s\n", temp->order, temp->firstName, temp->lastName);
        } else if (strlen(temp->companyName) > 0) {
          printf("  %d.  %s\n", temp->order, temp->companyName);
          prevAlph = temp->companyName[0];
        }
      }
      temp = temp->next;
    }
  }
}

void freeList(node head) {
  /*free the list*/
  node temp, ptr;
  temp = ptr = NULL;

  temp = head;
  while(temp->next != NULL) {
    ptr = temp;
    free(ptr->firstName);
    free(ptr->lastName);
    free(ptr->companyName);
    free(ptr->email);
    free(ptr);
    temp = temp->next;
  }
}

node getInfo(node head) {
  /*ask the user for the information*/
  char *firstName;
  char *lastName;
  char *companyName;
  char *email;
  char buffer[50];
  char action;
  unsigned long phoneNumber;

  printf("First Name: ");
  fgets(buffer, 50, stdin);
  if(buffer[0] == '\n') {
    buffer[0] = '\0';
  } else {
    strtok(buffer, "\n");
  }
  firstName = malloc(sizeof(char) * strlen(buffer));
  strcpy(firstName, buffer);
  strcpy(buffer, "");

  printf("Last Name: ");
  fgets(buffer, 50, stdin);
  if(buffer[0] == '\n') {
    buffer[0] = '\0';
  } else {
    strtok(buffer, "\n");
  }
  lastName = malloc(sizeof(char) * strlen(buffer));
  strcpy(lastName, buffer);
  strcpy(buffer, "");

  while(1) {
    printf("Company Name: ");
    fgets(buffer, 50, stdin);
    if(buffer[0] == '\n') {
      buffer[0] = '\0';
    } else {
      strtok(buffer, "\n");
    }
    companyName = malloc(sizeof(char) * strlen(buffer));
    strcpy(companyName, buffer);
    strcpy(buffer, "");
    if(lastName[0] != '\0' && companyName[0] == '\0') {
      break;
    } else if (lastName[0] == '\0' && companyName[0] != '\0') {
      break;
    } else if (lastName[0] != '\0' && companyName[0] != '\0') {
      break;
    }
  }

  while(1) {
    printf("Phone Number: ");
    scanf("%lu", &phoneNumber);
    getchar();
    if (phoneNumber >= 999999 && phoneNumber <= 9999999) { /*7 digits*/
      break;
    } else if (phoneNumber >= 999999999 && phoneNumber <= 9999999999) { /*10 digits*/
      break;
    }
  }

  printf("Email: ");
  fgets(buffer, 50, stdin);
  if(buffer[0] == '\n') {
    buffer[0] = '\0';
  } else {
    strtok(buffer, "\n");
  }
  email = malloc(sizeof(char) * strlen(buffer));
  strcpy(email, buffer);
  strcpy(buffer, "");

  while(1) {
    /*ask for another actoin*/
    printf("Action: ");
    scanf("%c", &action);
    getchar();

    if(action == 's' || action == 'S') {
      head = addNode(head, firstName, lastName, companyName, email, phoneNumber);
        free(firstName);
        free(lastName);
        free(companyName);
        free(email);
        return head;
    } else if (action == 'r' || action == 'R') {
        free(firstName);
        free(lastName);
        free(companyName);
        free(email);
        return head;
    }
  }
}

int writeToFile(node head, int numberOfContacts) {
  /*write all the nodes to the file*/
  struct contact newContact;
  FILE* fp;
  node temp;

  temp = head;

  fp = fopen("./bin/myContactList.db", "wb");
  if (fp == NULL) {
    printf("Unable to open file");
    return 1;
  }

  if(numberOfContacts == 0) {
    fclose(fp);
    return 0;
  }

  while(temp != NULL) {
    /*get positions*/
    newContact.first_name_posn = ftell(fp) + 48;
    newContact.last_name_posn = newContact.first_name_posn + strlen(temp->firstName) + 1;
    newContact.company_name_posn = newContact.last_name_posn + strlen(temp->lastName) + 1;
    newContact.email_posn = newContact.company_name_posn + strlen(temp->companyName) + 1;
    newContact.next = newContact.email_posn + strlen(temp->email);
    newContact.phone_number = temp->phoneNumber;

    /*write positions*/
    fwrite(&newContact, 1, sizeof(struct contact), fp);

    /*write the data*/
    fwrite(temp->firstName, strlen(temp->firstName) + 1, sizeof(char), fp);
    fwrite(temp->lastName, strlen(temp->lastName) + 1, sizeof(char), fp);
    fwrite(temp->companyName, strlen(temp->companyName) + 1, sizeof(char), fp);
    fwrite(temp->email, strlen(temp->email), sizeof(char), fp);

    /*go on to next set of data*/
    temp = temp->next;
  }
  return 0;
}

node readFromFile(node head) {
  /*read all nodes from the file*/
  FILE* fp;
  struct contact newContact;
  char *firstName;
  char *lastName;
  char *companyName;
  char *email;
  char buffer[50];
  unsigned long phoneNumber;

  fp = fopen("./bin/myContactList.db", "rb");
  if (fp == NULL) {
    return head;
  }

  fseek(fp, 0, SEEK_SET); /*go to the begining of the file*/

  while(fread(&newContact, 1, sizeof(struct contact), fp)) {
    fread(buffer, newContact.last_name_posn - newContact.first_name_posn, 1, fp); /*read from the file*/
    firstName = malloc(sizeof(char) * strlen(buffer) + 1); /*create space for the values*/
    strcpy(firstName, buffer); /*copy into space*/
    strcpy(buffer, ""); /*reset buffer*/

    fread(buffer, newContact.company_name_posn - newContact.last_name_posn, 1, fp);
    lastName = malloc(sizeof(char) * strlen(buffer) + 1);
    strcpy(lastName, buffer);
    strcpy(buffer, "");

    fread(buffer, newContact.email_posn - newContact.company_name_posn, 1, fp);
    companyName = malloc(sizeof(char) * strlen(buffer) + 1);
    strcpy(companyName, buffer);
    strcpy(buffer, "");

    fread(buffer, newContact.next - newContact.email_posn, 1, fp);
    email = malloc(sizeof(char) * strlen(buffer) + 1);
    strcpy(email, buffer);
    strcpy(buffer, "");

    phoneNumber = newContact.phone_number;

    head = addNode(head, firstName, lastName, companyName, email, phoneNumber);

    free(firstName);
    free(lastName);
    free(companyName);
    free(email);
    fseek(fp, newContact.next, SEEK_SET);
  }
  return head;
}

int getNumberOfContacts(node head) {
  /*count the number of existing nodes*/
  int count;
  node temp;

  temp = head;
  count = 0;

  while(temp!= NULL) {
    temp = temp->next;
    count++;
  }
  return count;
}

void sortNames(node head) {
  char **nameList;
  node temp;
  int count, i, j, order;
  char buffer[50];
  char nameBuffer[50];

  temp = NULL;
  count = order = 0;

  temp = head;
  nameList = malloc(sizeof(char) * 99);
  while(temp != NULL) {
    if ((strlen(temp->lastName) == 0) && strlen(temp->firstName) == 0) { /*if they didn't put a last name*/
      strcpy(buffer, temp->companyName);
    } else {
      strcpy(buffer, temp->lastName);
      strcat(buffer, " ");
      strcat(buffer, temp->firstName);
    }
    nameList[count] = malloc(sizeof(char) * strlen(buffer) + 1);
    strcpy(nameList[count], buffer);
    count++;
    temp = temp->next;
  }
  for (j = 0; j < count; j++) {
    for (i = 0; i < count - 1; i++) {
      if (strcmp(nameList[i], nameList[i + 1]) > 0) { /*if the previous is greater than the current*/
        strcpy(buffer, nameList[i]);
        strcpy(nameList[i], nameList[i+1]);
        strcpy(nameList[i+1], buffer);
      }
    }
  }
  for (i = 0; i < count; i++) {
      temp = head;
      while(temp != NULL) {
      if ((strlen(temp->lastName) == 0) && strlen(temp->firstName) == 0) { /*if they didn't put a last name*/
        strcpy(nameBuffer, temp->companyName);
      } else {
        strcpy(nameBuffer, temp->lastName);
        strcat(nameBuffer, " ");
        strcat(nameBuffer, temp->firstName);
      }

      strcpy(buffer, nameList[i]);
      if (strcmp(buffer, nameBuffer) == 0) {
        temp->order = order + 1;
        order++;
      } else if(strcmp(buffer, nameBuffer) == 0) {
        temp->order = order + 1;
        order++;
      }
      temp = temp->next;
    }
  }

  printf("\n\n");
  free(nameList);
}

node showOneContact(node head, int order) {
  /*print only one node*/
  node temp;
  char action;

  temp = NULL;
  action = '\0';

  temp = head;
  /*find the node*/
  while(temp!=NULL) {
    if (temp->order == order) {
      printf("\nContact #%d\n", temp->order);
      printf("First Name: %s\n", temp->firstName);
      printf("Last Name: %s\n", temp->lastName);
      printf("Company Name: %s\n", temp->companyName);
      printf("Phone Number: %ld\n", temp->phoneNumber);
      printf("Email: %s\n\n", temp->email);
      break;
    }
    temp = temp->next;
  }
  while(1) {
    printf("Action: ");
    scanf("%c", &action);
    getchar();

    if (action == 'r' || action =='R') {
      return head;
    } else if (action == 'e' || action == 'E') {
      head = editContact(head, order);
      return head;
    } else if (action == 'd' || action == 'D') {
      head = deleteContact(head, order);
      return head;
    }
  }
}

node editContact(node head, int order) {
  /*edit an existing node*/
  node temp;
  char action;
  unsigned long phoneNumber;
  char buffer[50];
  char* email;

  temp = NULL;
  action = '\0';
  phoneNumber = 0;

  temp = head;
  while(temp!=NULL) {
    if (temp->order == order) {
      printf("\nContact #%d\n", temp->order);
      printf("First Name: %s\n", temp->firstName);
      printf("Last Name: %s\n", temp->lastName);
      printf("Company Name: %s\n", temp->companyName);

      while(1) {
        printf("Phone Number: ");
        scanf("%lu", &phoneNumber);
        getchar();
        if (phoneNumber >= 999999 && phoneNumber <= 9999999) { /*7 digits*/
          break;
        } else if (phoneNumber >= 999999999 && phoneNumber <= 9999999999) { /*10 digits*/
          break;
        }
      }
      printf("Email: ");
      fgets(buffer, 50, stdin);
      if(buffer[0] == '\n') {
        buffer[0] = '\0';
      } else {
        strtok(buffer, "\n");
      }
      email = malloc(sizeof(char) * strlen(buffer));
      strcpy(email, buffer);
      strcpy(buffer, "");

      break;
    }
    temp = temp->next;
  }

  while(1) {
    printf("Action: ");
    scanf("%c", &action);
    getchar();

    if (action == 'r' || action =='R') {
      free(email);
      return head;
    } else if (action == 's' || action == 'S') {
      if (temp->phoneNumber != phoneNumber) {
        temp->phoneNumber = phoneNumber;
      }
      if (strlen(email) > 0) {
        strcpy(temp->email, email);
      }
      free(email);
      return head;
    } else if (action == 'd' || action == 'D') {
      free(email);
      head = deleteContact(head, order);
      return head;
    }
  }
}

node deleteContact(node head, int order) {
  /*delete a single node*/
  node temp, prev;
  temp = prev = NULL;

  temp = head;
  while(temp!=NULL && temp->order != order) { /*search for the needed node*/
    prev = temp;
    temp = temp->next;
  }

  if (temp == head) { /*if the node is the head*/
    head = temp->next;
    free(temp->firstName);
    free(temp->lastName);
    free(temp->companyName);
    free(temp->email);
    free(temp);
    return head;
  } else {
    /*free and set*/
    prev->next = temp->next;
    free(temp->firstName);
    free(temp->lastName);
    free(temp->companyName);
    free(temp->email);
    free(temp);
    return head;
  }
}
