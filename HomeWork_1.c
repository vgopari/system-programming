//  Name: Vamshi Gopari    G#: G01355694
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char strings[10][50 + 1];

int isValidString(char *str);
int isDuplicateString(char strings[][50 + 1], int size, char *str);
void bubbleSort(char strings[][50 + 1], int size, int order);
void printingValues(char strings[][50 + 1], int order);

int isValidString(char *str)
{
  int i, len = strlen(str);
  for (i = 0; i < len; i++)
  {
    if (str[i] == '+' || str[i] == '*' || str[i] == '#' || str[i] == '$' || str[i] == '%' || str[i] == '^' || str[i] == '(' || str[i] == ')')
      return 0;
  }
  return 1;
}

int isDuplicateString(char strings[][50 + 1], int size, char *str)
{

  int i;
  for (i = 0; i < size; i++)
  {
    if (strcmp(strings[i], str) == 0)
      return 1;
  }
  return 0;
}

void bubbleSort(char strings[][50 + 1], int size, int order)
{
  int i, j;
  char temp[50 + 1];

  for (i = 0; i < size - 1; i++)
  {
    for (j = 0; j < size - i - 1; j++)
    {
      if (order == 0)
      {
        if (strcmp(strings[j], strings[j + 1]) > 0)
        {
          strcpy(temp, strings[j]);
          strcpy(strings[j], strings[j + 1]);
          strcpy(strings[j + 1], temp);
        }
      }
      else
      {
        if (strcmp(strings[j], strings[j + 1]) < 0)
        {
          strcpy(temp, strings[j]);
          strcpy(strings[j], strings[j + 1]);
          strcpy(strings[j + 1], temp);
        }
      }
    }
  }
}

void printingValues(char strings[][50 + 1], int order)
{
  printf("\nStrings in %s order:\n", order ? "descending" : "ascending");
  int i;
  for (i = 0; i < 10; i++)
    printf("%s\n", strings[i]);

  if (order == 0)
  {
    printf("String with lowest ASCII value: %s\n", strings[0]);
    printf("String with highest ASCII value: %s\n", strings[10 - 1]);
  }
  else
  {
    printf("String with lowest ASCII value: %s\n", strings[10 - 1]);
    printf("String with highest ASCII value: %s\n", strings[0]);
  }
}

int main()
{
  int i, order = -1;

  for (i = 0; i < 10; i++)
  {
    while (1)
    {
      printf("Enter string %d: ", i + 1);
      fgets(strings[i], 50 + 1, stdin);

      int len = strlen(strings[i]);
      if (strings[i][len - 1] == '\n')
        strings[i][len - 1] = '\0';
      len--;

      if (len == 0)
      {
        printf("Error: Empty string. Try again.\n");
        continue;
      }

      if (!isValidString(strings[i]))
      {
        printf("Error: String contains invalid characters (+, *, #, $, %c, ^, (, or )). Try again.\n", '%');
        continue;
      }

      if (isDuplicateString(strings, i, strings[i]))
      {
        printf("Error: Duplicate string. Try again.\n");
        continue;
      }
      if (len > 25)
      {
        printf("Error: String length exceeds %d. Try again.\n", 25);
        continue;
      }
      break;
    }
  }
  while (order == -1)
  {
    printf("Enter order (A/D): ");
    char ch = getchar();
    if (toupper(ch) == 'A')
      order = 0;
    else if (toupper(ch) == 'D')
      order = 1;
    else
    {
      printf("Error: Invalid order specification.\n");
    }
  }

  bubbleSort(strings, 10, order);
  printingValues(strings, order);

  return 0;
}
