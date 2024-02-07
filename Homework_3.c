// Name: Vamshi Gopari    G#: G01355694   Homework 3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// declaration of functions
void readAndInitFile();
void addAddress();
struct address_t *treeTraversal(struct address_t *node, char *al);
int searchAlias(struct address_t *node, char *al);
void addNode(char *ipAddress);
void lookupAddress(struct address_t *node);
void deleteAddress();
struct address_t *inorderTraversal(struct address_t *node, char *al);
struct address_t *deleteNode(struct address_t *rootNode, char *aliasToDelete);
int validateIpAddress(int num1, int num2, int num3, int num4);
int isAddressDuplicate(struct address_t *node, int num1, int num2, int num3, int num4);
struct address_t *minValueNode(struct address_t *node);
void updateAddress(struct address_t *node);
void updateAddressIP(int input[], struct address_t *updateNode, char al[]);
void updateAddressFields(int input[], struct address_t *updateNode, char al[]);
void displayList(struct address_t *rootNode, int depth, struct address_t *node);
int height(struct address_t *node);
void displayAlias(struct address_t *node);
int findAliasesByLocation(struct address_t *node, int num1, int num2);
void saveToFile(struct address_t *node);
void printToFile(FILE *f, struct address_t *node);

// single node in BST
struct address_t
{
  int octet[4];
  char alias[11];
  struct address_t *leftChild;
  struct address_t *rightChild;
};
struct address_t *rootNode = NULL;
int nodeCount = 0;

int main()
{
  int count = 0;
  readAndInitFile();
  while (count != 8)
  {
    count = 0;
    printf("\nThe options are:");
    printf("\n1. Add Address");
    printf("\n2. Lookup Address");
    printf("\n3. Update Address");
    printf("\n4. Delete Address");
    printf("\n5. Display List");
    printf("\n6. Display Aliases of Location");
    printf("\n7. Save to file");
    printf("\n8. Quit");
    printf("\nEnter option ");
    scanf("%d", &count);
    getchar();
    switch (count)
    {
    case 1:
      addAddress();
      break;
    case 2:
      lookupAddress(rootNode);
      break;
    case 3:
      updateAddress(rootNode);
      break;
    case 4:
      deleteAddress();
      break;
    case 5:
      if (rootNode == NULL)
        printf("\n No nodes to display.Add new nodes");
      else
        displayList(rootNode, 0, NULL);
      printf("Number of addresses: %d", nodeCount);
      break;
    case 6:
      displayAlias(rootNode);
      break;
    case 7:
      saveToFile(rootNode);
      break;
    case 8:
      printf("Thank you");
      break;
    default:
      printf("Enter valid input key");
      break;
    }
  }
}

// Function to read from file and initialize binary tree
void readAndInitFile()
{
  FILE *filePointer;
  filePointer = fopen("CS531_Inet.txt", "r");

  if (filePointer == NULL)
  {
    printf("\nError: uanble to open the file!");
  }
  else
  {
    char ipAddress[100];

    while (fgets(ipAddress, sizeof(ipAddress), filePointer) != NULL)
    {
      addNode(ipAddress);
    }
    fclose(filePointer);
  }
}

// Function to add a new node to the binary tree
void addNode(char *ipAddress)
{
  if (rootNode == NULL)
  {
    struct address_t *newNode = (struct address_t *)malloc(sizeof(struct address_t));
    sscanf(ipAddress, "%d.%d.%d.%d %s", newNode->octet, newNode->octet + 1, newNode->octet + 2, newNode->octet + 3, newNode->alias);
    rootNode = newNode;
    nodeCount++;
  }
  else
  {
    struct address_t *newNode = (struct address_t *)malloc(sizeof(struct address_t));
    sscanf(ipAddress, "%d.%d.%d.%d %s", newNode->octet, newNode->octet + 1, newNode->octet + 2, newNode->octet + 3, newNode->alias);
    struct address_t *currentNode = treeTraversal(rootNode, newNode->alias);
    if (currentNode == NULL)
    {
      printf("Error: Alias already exists");
    }
    else
    {
      int compareAlias = strcmp(newNode->alias, currentNode->alias);
      if (compareAlias < 0)
      {
        currentNode->leftChild = newNode;
      }
      else if (compareAlias > 0)
      {
        currentNode->rightChild = newNode;
      }
      else
      {
        printf("Error: Alias already exists!");
      }
      nodeCount++;
    }
  }
}

//creating root address while adding a address
void addRootAddress() {
  printf("\nCreating root node");
  char ipAddress[100];
  int input[4];
  struct address_t *newNode = (struct address_t *)malloc(sizeof(struct address_t));
  printf("Enter alias:");
  scanf("%s", newNode->alias);
  printf("\nEnter the Address for %s: ", newNode->alias);
  getchar();
  fgets(ipAddress, sizeof(ipAddress), stdin);
  sscanf(ipAddress, "%d.%d.%d.%d", input, input + 1, input + 2, input + 3);
  int isIPAddressValid = validateIpAddress(input[0], input[1], input[2], input[3]);
  if (isIPAddressValid == 1) {
    for (int i = 0; i < 4; i++)
      *(newNode->octet + i) = input[i];
    rootNode = newNode;
    printf("%s is root", rootNode->alias);
    nodeCount++;
  }
}

//creating non root address while adding a address to tree
void addNonRootAddress() {
  char ipAddress[100];
  struct address_t *newNode = (struct address_t *)malloc(sizeof(struct address_t));
  printf("Enter alias:");
  scanf("%s", newNode->alias);
  int isAliasFound = searchAlias(rootNode, newNode->alias);
  while (isAliasFound == 0) {
    printf("\nAlias already exist\nEnter alias: ");
    scanf("%s", newNode->alias);
    isAliasFound = searchAlias(rootNode, newNode->alias);
  }
  printf("\nEnter the Address for %s: ", newNode->alias);
  getchar();
  fgets(ipAddress, sizeof(ipAddress), stdin);
  sscanf(ipAddress, "%d.%d.%d.%d", newNode->octet, newNode->octet + 1, newNode->octet + 2, newNode->octet + 3);
  int isIPAddressValid = validateIpAddress(*(newNode->octet), *(newNode->octet + 1), *(newNode->octet + 2), *(newNode->octet + 3));
  int isIPAddressDuplicate = isAddressDuplicate(rootNode, *(newNode->octet), *(newNode->octet + 1), *(newNode->octet + 2), *(newNode->octet + 3));
  if (isIPAddressDuplicate == 0) {
    printf("\nIP already exists");
  }
  if (isIPAddressValid == 1 && isIPAddressDuplicate == 1) {
    struct address_t *currentNode = treeTraversal(rootNode, newNode->alias);
    int compareAlias = strcmp(newNode->alias, currentNode->alias);
    if (compareAlias < 0)
      currentNode->leftChild = newNode;
    else {
      if (compareAlias > 0)
        currentNode->rightChild = newNode;
      else
        printf("Alias already exists");
    }
    nodeCount++;
  }
}

// Function to add new values into the BST
void addAddress()
{
  if (rootNode != NULL)
  {
    addNonRootAddress();
  }
  else
  {
    addRootAddress();
  }
}

//search aliases
int searchAlias(struct address_t *node, char *alias)
{
  while (node != NULL)
  {
    int compareAlias = strcmp(alias, node->alias);
    if (compareAlias < 0)
    {
      node = node->leftChild;
    }
    if (compareAlias > 0)
    {
      node = node->rightChild;
    }
    if (compareAlias == 0)
    {
      return 0;
    }
  }
  return 1;
}

// Function to lookup an address to the binary tree
void lookupAddress(struct address_t *node)
{
  if (node == NULL)
  {
    printf("\nNo nodes to search!");
  }
  else
  {
    char alias[11];
    int aliasFound = 0;
    printf("\nEnter the Alias: ");
    scanf("%s", alias);
    while (node != NULL)
    {
      int compareAlias = strcmp(alias, node->alias);
      if (compareAlias < 0)
      {
        node = node->leftChild;
      }
      else if (compareAlias > 0)
      {
        node = node->rightChild;
      }
      else
      {
        aliasFound = 1;
        printf("\n%s: %d.%d.%d.%d", alias, *(node->octet), *(node->octet + 1), *(node->octet + 2), *(node->octet + 3));
        break;
      }
    }
    if (aliasFound == 0)
    {
      printf("\n%s does not exist in the tree.", alias);
    }
  }
}

// Function to delete address to the binary tree
void deleteAddress()
{
  // Check if the tree is empty
  if (rootNode == NULL)
  {
    printf("\nNo node to delete. Add new nodes");
  }
  else
  {
    // Check if the rootNode is the only node in the tree
    if (rootNode != NULL && rootNode->rightChild == NULL && rootNode->leftChild == NULL)
    {
      char userInput;
      printf("\nDelete rootNode %s %d.%d.%d.%d?(y/n)", rootNode->alias, *(rootNode->octet), *(rootNode->octet + 1), *(rootNode->octet + 2), *(rootNode->octet + 3));
      scanf("%c", &userInput);

      // Prompt user for valid input
      while (userInput != 'y' && userInput != 'n')
      {
        // remove excess characters
        while (getchar() != '\n')
          ;
        printf("\nPlease enter a valid Input:");
        printf("\nDelete %s %d.%d.%d.%d?(y/n)", rootNode->alias, *(rootNode->octet), *(rootNode->octet + 1), *(rootNode->octet + 2), *(rootNode->octet + 3));
        scanf("%c", &userInput);
      }

      // Delete the rootNode
      printf("Last node deleted: %s", rootNode->alias);
      nodeCount--;
      rootNode = NULL;
    }
    else
    {
      char alias[100], userInput;
      printf("\nEnter the alias: ");
      scanf("%s", alias);
      getchar();

      // Find the node to delete
      struct address_t *nodeToDelete = inorderTraversal(rootNode, alias);

      // Prompt user if the node is found
      if (nodeToDelete == NULL)
      {
        printf("\n%s does not exist", alias);
      }
      else
      {
        printf("\nDelete %s %d.%d.%d.%d?(y/n)", nodeToDelete->alias, *(nodeToDelete->octet), *(nodeToDelete->octet + 1), *(nodeToDelete->octet + 2), *(nodeToDelete->octet + 3));
        scanf("%c", &userInput);

        // Prompt user for valid input
        while (userInput != 'y' && userInput != 'n')
        {
          // remove excess characters
          while (getchar() != '\n')
            ;
          printf("\nPlease enter a valid input:");
          printf("\nDelete %s %d.%d.%d.%d?(y/n)", nodeToDelete->alias, *(nodeToDelete->octet), *(nodeToDelete->octet + 1), *(nodeToDelete->octet + 2), *(nodeToDelete->octet + 3));
          scanf("%c", &userInput);
        }
      }

      // Delete the node if the user confirms
      if (userInput == 'y')
      {
        rootNode = deleteNode(rootNode, alias);
        if (rootNode != NULL)
        {
          nodeCount--;
          printf("\n%s deleted", alias);
        }
      }

      // Do nothing if the user cancels
      if (userInput == 'n')
      {
        printf("\nDeletion of node - cancelled");
      }
    }
  }
}

//method to tree traversal 
struct address_t *treeTraversal(struct address_t *node, char *alias)
{
  while (node != NULL)
  {
    int compareAlias = strcmp(alias, node->alias);
    if (compareAlias == 0)
    {
      return NULL; // exact match found
    }
    else if (compareAlias < 0)
    {
      if (node->leftChild == NULL)
      {
        return node; // closest match found
      }
      else
      {
        node = node->leftChild;
      }
    }
    else
    {
      if (node->rightChild == NULL)
      {
        return node; // closest match found
      }
      else
      {
        node = node->rightChild;
      }
    }
  }
  return NULL; // tree is empty
}

// Function for inorder tree traversal
struct address_t *inorderTraversal(struct address_t *root, char *alias)
{
  struct address_t *current = root;
  while (current != NULL)
  {
    if (strcmp(current->alias, alias) == 0)
    {
      return current;
    }
    else if (strcmp(alias, current->alias) < 0)
    {
      current = current->leftChild;
    }
    else
    {
      current = current->rightChild;
    }
  }
  return NULL;
}

// Function to delete a node to the binary tree
struct address_t *deleteNode(struct address_t *rootNode, char *aliasToDelete)
{
  // Traverse the tree to find the node to delete
  if (rootNode == NULL)
  {
    return rootNode;
  }
  int compareResult = strcmp(aliasToDelete, rootNode->alias);
  if (compareResult < 0)
  {
    rootNode->leftChild = deleteNode(rootNode->leftChild, aliasToDelete);
  }
  else if (compareResult > 0)
  {
    rootNode->rightChild = deleteNode(rootNode->rightChild, aliasToDelete);
  }
  else
  {
    if (rootNode->leftChild == NULL)
    {
      struct address_t *temp = rootNode->rightChild;
      free(rootNode);
      return temp;
    }
    else if (rootNode->rightChild == NULL)
    {
      struct address_t *temp = rootNode->leftChild;
      free(rootNode);
      return temp;
    }
    struct address_t *temp = minValueNode(rootNode->rightChild);
    strcpy(rootNode->alias, temp->alias);
    rootNode->rightChild = deleteNode(rootNode->rightChild, temp->alias);
  }
  return rootNode;
}

//check if the ip address is a valid address
int validateIpAddress(int num1, int num2, int num3, int num4)
{
  if (num1 < 0 || num1 > 255 || num2 < 0 || num2 > 255 || num3 < 0 || num3 > 255 || num4 < 0 || num4 > 255)
  {
    printf("Invalid IP address");
    return 0;
  }
  else
    return 1;
}

// find the duplicate address
int isAddressDuplicate(struct address_t *node, int num1, int num2, int num3, int num4)
{
  if (node == NULL)
    return 1;
  if (*(node->octet) == num1 && *(node->octet + 1) == num2 && *(node->octet + 2) == num3 && *(node->octet + 3) == num4)
    return 0;
  if (isAddressDuplicate(node->leftChild, num1, num2, num3, num4) == 0 || 
      isAddressDuplicate(node->rightChild, num1, num2, num3, num4) == 0)
    return 0;
  return 1;
}

//find the minimum value node
struct address_t *minValueNode(struct address_t *node)
{
  struct address_t *currentNode = node;
  while (currentNode && currentNode->leftChild != NULL)
    currentNode = currentNode->leftChild;
  return currentNode;
}

//update the address
void updateAddress(struct address_t *node)
{
  if (node == NULL)
    printf("\nNo node to update fields. Add new nodes");
  else
  {
    int input[4];
    char al[11];
    printf("\nEnter Alias: ");
    scanf("%s", al);
    struct address_t *updateNode = inorderTraversal(rootNode, al);
    if (updateNode == NULL)
    {
      printf("\nNode does not exist");
    }
    else
    {
      updateAddressFields(input, updateNode, al);
    }
  }
}

// update each address field
void updateAddressFields(int input[], struct address_t *updateNode, char al[])
{
  printf("\nUpdate address for %s: %d.%d.%d.%d", (updateNode->alias), *(updateNode->octet), *(updateNode->octet + 1), *(updateNode->octet + 2), *(updateNode->octet + 3));
  for (int i = 0; i < 4; i++)
  {
    if (i < 2)
    {
      printf("\nEnter location value # %d: ", i + 1);
    }
    else
    {
      printf("\nEnter value # %d: ", i + 1);
    }
    scanf("%d", &input[i]);
    int isValidNum = (input[i] < 0 || input[i] > 255) ? 0 : 1; // validnum(input[i]);
    while (isValidNum == 0)
    {
      printf("\nIllegal Entry");
      if (i < 2)
      {
        printf("\nEnter location value # %d: ", i + 1);
      }
      else
      {
        printf("\nEnter value # %d: ", i + 1);
        scanf("%d", &input[i]);
      }
      isValidNum = (input[i] < 0 || input[i] > 255) ? 0 : 1;
    }
  }
  updateAddressIP(input, updateNode, al);
}

// update the address
void updateAddressIP(int input[], struct address_t *updateNode, char al[])
{
  int isIPdup = isAddressDuplicate(rootNode, input[0], input[1], input[2], input[3]);
  if (isIPdup == 0)
    printf("\nIP already exists");
  if (isIPdup == 1)
  {
    for (int i = 0; i < 4; i++)
    {
      *(updateNode->octet + i) = input[i];
    }
    printf("Address of %s updated", al);
  }
}

//display all the aliases and addresses
void displayList(struct address_t *rootNode, int depth, struct address_t *node)
{
  if (rootNode == NULL)
  {
    return;
  }
  else
  {
    displayList(rootNode->leftChild, depth + 1, node);
    printf("%d.%d.%d.%d %s (Depth: %d, Height: %d, Parent: ", *(rootNode->octet), *(rootNode->octet + 1), *(rootNode->octet + 2), *(rootNode->octet + 3), rootNode->alias, depth, height(rootNode));
    if (rootNode == NULL)
    {
      printf("NONE)\n");
    }
    else
    {
      printf("%s)\n", rootNode->alias);
    }
    displayList(rootNode->rightChild, depth + 1, node);
  }
}

//to find the height of a node
int height(struct address_t *rootNode)
{
  if (rootNode == NULL)
  {
    return -1;
  }
  else
  {
    int leftHeight = height(rootNode->leftChild);
    int rightHeight = height(rootNode->rightChild);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
  }
}

// display aliases using location
void displayAlias(struct address_t *node)
{
  int nums[2], isValidNum, count = 0;
  if (node == NULL)
  {
    printf("\nBinary Search Tree is Empty");
  }
  else
  {
    printf("\nEnter Location value # 1 [0-255]:");
    scanf("%d", &nums[0]);
    isValidNum = (nums[0] < 0 || nums[0] > 255) ? 0 : 1;
    while (isValidNum == 0)
    {
      printf("\nIllegal entry. Enter Location value # 1 [0-255]");
      scanf("%d", &nums[0]);
      isValidNum = (nums[0] < 0 || nums[0] > 255) ? 0 : 1;
    }
    getchar();
    printf("Enter Location value # 2 [0-255]:");
    scanf("%d", &nums[1]);
    isValidNum = (nums[1] < 0 || nums[1] > 255) ? 0 : 1;
    while (isValidNum == 0)
    {
      printf("\nIllegal entry. Enter Location valur # 2 [0-255]");
      scanf("%d", &nums[1]);
      isValidNum = (nums[1] < 0 || nums[1] > 255) ? 0 : 1;
    }
    printf("Location: %d.%d\n", nums[0], nums[1]);
    int foundNodes = findAliasesByLocation(rootNode, nums[0], nums[1]);
    if (foundNodes == 0)
      printf("Addresses of location %d.%d not found\n", nums[0], nums[1]);
  }
}

//Recursively find the aliases using location
int findAliasesByLocation(struct address_t *rootNode, int num1, int num2)
{
  if (rootNode == NULL)
    return 1;
  if (*(rootNode->octet) == num1 && *(rootNode->octet + 1) == num2)
    printf("%s\n", rootNode->alias);
  if (findAliasesByLocation(rootNode->leftChild, num1, num2) == 0 || findAliasesByLocation(rootNode->rightChild, num1, num2) == 0)
    return 0;
  return 1;
}

//Save all address to a text file
void saveToFile(struct address_t *rootNode)
{
  if (rootNode == NULL)
  {
    printf("Binary Search Tree is Empty");
  }
  else
  {
    FILE *filePointer;
    char fileName[100];
    printf("Enter file name: ");
    scanf("%s", fileName);
    filePointer = fopen(fileName, "w");
    if (filePointer == NULL)
      printf("Error: Unsuccessful file open\n");
    else
    {
      printToFile(filePointer, rootNode);
      printf("\nFile Saved");
      fclose(filePointer);
    }
  }
}

void printToFile(FILE *f, struct address_t *rootNode)
{
  if (rootNode == NULL)
  {
    return;
  }
  else
  {
    printToFile(f, rootNode->leftChild);
    fprintf(f, "%d.%d.%d.%d %s\n", *(rootNode->octet), *(rootNode->octet + 1), *(rootNode->octet + 2), *(rootNode->octet + 3), rootNode->alias);
    printToFile(f, rootNode->rightChild);
  }
}