struct contact {
  unsigned long phone_number;
  long first_name_posn;
  long last_name_posn;
  long company_name_posn;
  long email_posn;
  long next;
};
struct contactInfo {
  char *firstName;
  char *lastName;
  char *companyName;
  char *email;
  int order;
  unsigned long phoneNumber;
  struct contactInfo *next;
};

typedef struct contactInfo *node;

node createNode(void);
node addNode(node, char*, char*, char*, char*, unsigned long);
void printList(node, int);
void freeList(node);
node getInfo(node);
int writeToFile(node, int);
node readFromFile(node);
int getNumberOfContacts(node);
void sortNames(node);
node showOneContact(node, int);
node editContact(node, int);
node deleteContact(node, int);
