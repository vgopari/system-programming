//  Name: Vamshi Gopari    G#: G01355694        Homework 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_INPUT_LENGTH 10

void read_from_file();
void add_address();
int parse_address_string(char *address_string, int *octets);
int validate_alias(char *alias);
struct address_t *find_address_by_alias(char *alias);
int compare_address(int *octets1, int *octets2);
struct address_t *create_address_node(int *octets, char *alias);
void add_address_node(struct address_t *new_node);
void look_up_address();
void update_address();
void delete_address();
void display_list();
void display_aliases();
void save_to_file();
bool check_duplicate_address(int *octets);
int validnum(int num);

struct address_t
{
    int octet[4];
    char alias[MAX_INPUT_LENGTH + 1];
    struct address_t *next;
};

struct address_t *head = NULL;

int main()
{
    read_from_file(); // Read in the data from the file and create the linked list
    int choice = 0;
    do
    {
        printf("\nMenu options:\n");
        printf("1) Add address\n");
        printf("2) Look up address\n");
        printf("3) Update address\n");
        printf("4) Delete address\n");
        printf("5) Display list\n");
        printf("6) Display aliases for location\n");
        printf("7) Save to file\n");
        printf("8) Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            add_address();
            break;
        case 2:
            look_up_address();
            break;
        case 3:
            update_address();
            break;
        case 4:
            delete_address();
            break;
        case 5:
            display_list();
            break;
        case 6:
            display_aliases();
            break;
        case 7:
            save_to_file();
            break;
        case 8:
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 8);

    return 0;
}

void read_from_file()
{
    FILE *fptr;
    char ip[100];
    fptr = fopen("CS531_Inet.txt", "r");
    if (fptr == NULL)
    {
        printf("\nFile open unsuccessful!");
        return;
    }
    struct address_t *prev = NULL;
    while (fgets(ip, sizeof(ip), fptr) != NULL)
    {
        struct address_t *newnode = (struct address_t *)malloc(sizeof(struct address_t));

        if (sscanf(ip, "%d.%d.%d.%d %99s", newnode->octet, newnode->octet + 1, newnode->octet + 2, newnode->octet + 3, newnode->alias) != 5)
        {
            printf("\nInvalid input: %s", ip);
            free(newnode);
            continue;
        }
        newnode->next = NULL;
        if (prev == NULL)
        {
            head = newnode;
        }
        else
        {
            prev->next = newnode;
        }
        prev = newnode;
    }
    fclose(fptr);
}

// this function adds a address
void add_address()
{
    char input[100];
    char address[16], alias[11];
    int octets[4];

    printf("Enter alias: ");
    fgets(alias, sizeof(alias), stdin);
    alias[strcspn(alias, "\n")] = '\0';

    // Check if alias is valid
    if (!validate_alias(alias))
    {
        printf("Error: Alias must be 10 characters or less\n");
        return;
    }

    // Check if alias already exists
    if (find_address_by_alias(alias))
    {
        printf("Error: Alias already exists\n");
        return;
    }

    // Prompt user for input
    printf("Enter IPv4 address: ");
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = '\0';

    // Parse address string into octets
    if (!parse_address_string(address, octets))
    {
        printf("Error: Invalid IPv4 address\n");
        return;
    }

    if (check_duplicate_address(octets))
    {
        printf("Error: Address already exists\n");
        return;
    }

    // Create new address node and add to list
    struct address_t *new_node = create_address_node(octets, alias);
    if (head == NULL)
    {
        head = new_node;
    }
    else
    {
        add_address_node(new_node);
    }
    printf("Address added successfully\n");
}

// this function checks if all the octets are within the valid range of 0-255
int parse_address_string(char *address_string, int *octets)
{
    int num_parsed = sscanf(address_string, "%d.%d.%d.%d",
                            &octets[0], &octets[1], &octets[2], &octets[3]);
    if (num_parsed != 4)
    {
        return 0;
    }
    for (int i = 0; i < 4; i++)
    {
        if (octets[i] < 0 || octets[i] > 255)
        {
            return 0;
        }
    }
    return 1;
}

// validate alias
int validate_alias(char *alias)
{
    return strlen(alias) <= 10;
}

// Function to check if an address with the given octets already exists in the list
bool check_duplicate_address(int *octets)
{
    struct address_t *current = head;
    while (current != NULL)
    {
        if (compare_address(current->octet, octets) == 0)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Function to check the duplicate aliases
struct address_t *find_address_by_alias(char *alias)
{
    struct address_t *current = head;
    while (current != NULL)
    {
        if (strcasecmp(current->alias, alias) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// This function compares two IP addresses represented as arrays of integers.
int compare_address(int *octets1, int *octets2)
{
    for (int i = 0; i < 4; i++)
    {
        if (octets1[i] < octets2[i])
        {
            return -1;
        }
        else if (octets1[i] > octets2[i])
        {
            return 1;
        }
    }
    return 0;
}

// This function creates a new node for the linked list of addresses,
struct address_t *create_address_node(int *octets, char *alias)
{
    struct address_t *new_node = malloc(sizeof(struct address_t));
    memcpy(new_node->octet, octets, sizeof(new_node->octet));
    strcpy(new_node->alias, alias);
    new_node->next = NULL;
    return new_node;
}

// This function adds a new address node to the end of the linked list
void add_address_node(struct address_t *new_node)
{
    struct address_t *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_node;
}

// display the corresponding address of an alias
void look_up_address()
{
    // Prompt user for alias
    char alias[MAX_INPUT_LENGTH + 1];
    int valid_input = 0;
    while (!valid_input)
    {
        printf("Enter an alias: ");
        if (fgets(alias, sizeof(alias), stdin) == NULL)
        {
            printf("Error reading input.\n");
            return;
        }
        // Remove newline character at end of string, if present
        int len = strlen(alias);
        if (len > 0 && alias[len - 1] == '\n')
        {
            alias[len - 1] = '\0';
        }
        // Check if alias is valid
        struct address_t *current = head;
        while (current != NULL)
        {
            if (strcasecmp(current->alias, alias) == 0)
            {
                // Valid alias
                valid_input = 1;
                break;
            }
            current = current->next;
        }
        if (!valid_input)
        {
            printf("Error: %s does not exist\n", alias);
        }
    }
    // Search for alias in linked list
    struct address_t *current = head;
    while (current != NULL)
    {
        if (strcasecmp(current->alias, alias) == 0)
        {
            // Found matching alias
            printf("Address	for %s: %d.%d.%d.%d\n", current->alias, current->octet[0], current->octet[1], current->octet[2], current->octet[3]);
            return;
        }
        current = current->next;
    }
}

// this function updates the address
void update_address()
{
    // Prompt user for alias
    printf("Enter an alias: ");
    char alias[MAX_INPUT_LENGTH + 1];
    if (fgets(alias, sizeof(alias), stdin) == NULL)
    {
        printf("Error reading input.\n");
        return;
    }
    // Remove newline character at end of string, if present
    int len = strlen(alias);
    if (len > 0 && alias[len - 1] == '\n')
    {
        alias[len - 1] = '\0';
    }
    // Search for alias in linked list
    struct address_t *current = head;
    while (current != NULL)
    {
        if (strcasecmp(current->alias, alias) == 0)
        {
            // Found matching alias
            int octets[4];
            int isValid, isIPvalid;
            // Prompt user to update the IP address
            printf("Update address for %s: %d.%d.%d.%d\n", current->alias, current->octet[0], current->octet[1], current->octet[2], current->octet[3]);
            for (int i = 0; i < 4; i++)
            {
                if (i < 2)
                {
                    printf("Enter location value #%d: ", i + 1);
                }
                else
                {
                    printf("Enter value #%d: ", i + 1);
                }
                // Read input from user
                scanf("%d", &octets[i]);
                // Validate input
                isValid = validnum(octets[i]);
                while (isValid == 0)
                {
                    printf("Illegal entry. ");
                    if (i < 2)
                    {
                        printf("Enter location value #%d: ", i + 1);
                    }
                    else
                    {
                        printf("Enter value #%d: ", i + 1);
                    }
                    scanf("%d", &octets[i]);
                    isValid = validnum(octets[i]);
                }
            }
            // Check if the new IP address already exists in the linked list
            if (check_duplicate_address(octets))
            {
                printf("Error: IP address %d.%d.%d.%d already exists.\n", octets[0], octets[1], octets[2], octets[3]);
            }
            else
            {
                // Update the IP address in the linked list
                for (int i = 0; i < 4; i++)
                {
                    *(current->octet + i) = octets[i];
                }
                printf("Address updated successfully.\n");
            }
            return;
        }
        current = current->next;
    }
    // Alias not found
    printf("Error: %s does not exist.\n", alias);
}

// this function deletes the address
void delete_address()
{
    char alias[MAX_INPUT_LENGTH + 1];

    // Prompt user for alias
    printf("Enter alias of address to delete: ");
    fgets(alias, sizeof(alias), stdin);
    alias[strcspn(alias, "\n")] = '\0'; // remove newline character from input

    // Find address with matching alias
    struct address_t *current = head;
    struct address_t *previous = NULL;
    while (current != NULL)
    {
        if (strcasecmp(current->alias, alias) == 0)
        {
            char confirmation;
            printf("Delete %s %d.%d.%d.%d (y/n): ", current->alias, current->octet[0], current->octet[1], current->octet[2], current->octet[3]);
            scanf(" %c", &confirmation);
            getchar();

            if (tolower(confirmation) == 'y')
            {
                // Delete node from linked list
                if (previous == NULL)
                {
                    head = current->next;
                }
                else
                {
                    previous->next = current->next;
                }
                free(current);
                printf("Address deleted successfully\n");
            }
            else
            {
                printf("Deletion canceled\n");
            }
            return;
        }
        previous = current;
        current = current->next;
    }

    // Address with matching alias not found
    printf("Error: %s does not exist\n", alias);
}

// display all the aliases and addresses
void display_list()
{
    struct address_t *current = head;
    int count = 0;

    if (current == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    while (current != NULL)
    {
        printf("%d.%d.%d.%d %s\n", current->octet[0], current->octet[1],
               current->octet[2], current->octet[3], current->alias);
        current = current->next;
        count++;
    }

    printf("Number of nodes in list: %d\n", count);
}

// validates a octet
int validnum(int num1)
{
    if (num1 < 0 || num1 > 255)
        return 0;
    else
        return 1;
}

// displays aliases based on location
void display_aliases()
{
    char input[100];
    char address_string[16];
    int isValid, num[2], count = 0;

    printf("\nEnter Location value # 1 [0-255]:");
    scanf("%d", &num[0]);
    isValid = validnum(num[0]);
    while (isValid == 0)
    {
        printf("\nillegal entry");
        printf("\nEnter Location value # 1 [0-255]");
        scanf("%d", &num[0]);
        isValid = validnum(num[0]);
    }
    getchar();
    printf("Enter Location value # 2 [0-255]:");
    scanf("%d", &num[1]);
    isValid = validnum(num[1]);
    while (isValid == 0)
    {
        printf("\nillegal entry");
        printf("\nEnter Location valur # 2 [0-255]");
        scanf("%d", &num[1]);
        isValid = validnum(num[1]);
    }

    // Check if any aliases map to this location
    struct address_t *current = head;
    bool found = false;
    while (current != NULL)
    {
        if (current->octet[2] == num[0] && current->octet[3] == num[1])
        {
            printf("%d.%d.%d.%d %s\n", *(current->octet), *(current->octet + 1), *(current->octet + 2), *(current->octet + 3), (current->alias));
            found = true;
        }
        current = current->next;
    }

    // Display error message if no aliases found
    if (!found)
    {
        printf("Error: No aliases found for this location\n");
    }
}

// save all the addresses with aliases in linked list to a file
void save_to_file()
{
    char filename[100];
    printf("Enter filename to save to: ");
    scanf("%99s", filename);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error: Unable to open file\n");
        return;
    }

    struct address_t *current = head;
    while (current != NULL)
    {
        fprintf(fp, "%d.%d.%d.%d %s\n", current->octet[0], current->octet[1],
                current->octet[2], current->octet[3], current->alias);
        current = current->next;
    }

    fclose(fp);
    printf("File saved successfully\n");
}
