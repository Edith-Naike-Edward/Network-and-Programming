#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>



#define MAX_BOOKS 100
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100
#define MAX_ISBN_LENGTH 100
#define MAX_PUBLISHER_LENGTH 100
#define MAX_DATE_LENGTH 100

typedef struct
{
    char title[MAX_TITLE_LENGTH];
    char authors[MAX_AUTHOR_LENGTH];
    char isbn[MAX_ISBN_LENGTH];
    char publisher[MAX_PUBLISHER_LENGTH];
    char date[MAX_DATE_LENGTH];
} Book;

int readBooksFromFile(Book books[], const char* filename);
void writeBooksToFile(const Book books[], int numBooks, const char* filename);
void displayCatalog(const Book books[], int numBooks);
int searchForItem(const Book books[], int numBooks, const char* query);
void purchaseItem(Book books[], int numBooks, const char* query);
void payForItem(Book books[], int numBooks, const char* query);

int main()
{
    Book books[MAX_BOOKS];
    int numBooks = readBooksFromFile(books, "Books2.txt");
    if (numBooks == -1)
    {
        printf("Error reading the book file.\n");
        return 1;
    }

    int choice;
    char query[MAX_TITLE_LENGTH];

    do
    {
        printf("1. Display Catalog\n");
        printf("2. Search for Item\n");
        printf("3. Purchase Item\n");
        printf("4. Pay for Item\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayCatalog(books, numBooks);
            break;
        case 2:
            printf("Enter the title, author, or publisher to search: ");
            scanf(" %[^\n]", query);
            searchForItem(books, numBooks, query);
            break;
        case 3:
            printf("Enter the title, author, or publisher of the book to purchase: ");
            scanf(" %[^\n]", query);
            purchaseItem(books, numBooks, query);
            break;
        case 4:
            printf("Enter the title, author, or publisher of the book to purchase and  pay for: ");
            scanf(" %[^\n]", query);
            payForItem(books, numBooks, query);
            break;
        case 5:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
        printf("\n");
    }
    while (choice != 5);

    writeBooksToFile(books, numBooks, "Books2.txt");

    return 0;
}

int readBooksFromFile(Book books[], const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        return -1;
    }

    int numBooks = 0;
    char line[MAX_TITLE_LENGTH + MAX_AUTHOR_LENGTH + MAX_ISBN_LENGTH + MAX_PUBLISHER_LENGTH + MAX_DATE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]", books[numBooks].title, books[numBooks].authors,
               books[numBooks].isbn, books[numBooks].publisher, books[numBooks].date);
        numBooks++;
    }

    fclose(file);

    return numBooks;
}

void writeBooksToFile(const Book books[], int numBooks, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening the file for writing.\n");
        return;
    }

    for (int i = 0; i < numBooks; i++)
    {
        fprintf(file, "%s,%s,%s,%s,%s\n", books[i].title, books[i].authors, books[i].isbn,
                books[i].publisher, books[i].date);
    }

    fclose(file);
}

void displayCatalog(const Book books[], int numBooks)
{
    printf("AVAILABLE BOOKS:\n");
    for (int i = 0; i < numBooks; i++)
    {
        printf("%-6d %-30s %-20s %-15s %-20s %s\n", i + 1, books[i].title, books[i].authors,
               books[i].isbn, books[i].publisher, books[i].date);
    }
}

int searchForItem(const Book books[], int numBooks, const char* query)
{
    int matches[MAX_BOOKS];
    int numMatches = 0;

    // Convert query to lowercase and remove extra spaces
    char lowercaseQuery[MAX_TITLE_LENGTH];
    int j = 0;
    for (int i = 0; query[i] != '\0'; i++)
    {
        if (query[i] != ' ' || (j > 0 && query[i-1] != ' '))   // Skip extra spaces
        {
            lowercaseQuery[j] = tolower(query[i]);
            j++;
        }
    }
    lowercaseQuery[j] = '\0';

    // Check for partial matches (case-insensitive)
    for (int i = 0; i < numBooks; i++)
    {
        char lowercaseTitle[MAX_TITLE_LENGTH];
        char lowercaseAuthors[MAX_AUTHOR_LENGTH];
        char lowercasePublisher[MAX_PUBLISHER_LENGTH];
        // Convert book fields to lowercase for comparison
        for (int j = 0; books[i].title[j] != '\0'; j++)
        {
            lowercaseTitle[j] = tolower(books[i].title[j]);
        }
        lowercaseTitle[strlen(books[i].title)] = '\0';
        for (int j = 0; books[i].authors[j] != '\0'; j++)
        {
            lowercaseAuthors[j] = tolower(books[i].authors[j]);
        }
        lowercaseAuthors[strlen(books[i].authors)] = '\0';
        for (int j = 0; books[i].publisher[j] != '\0'; j++)
        {
            lowercasePublisher[j] = tolower(books[i].publisher[j]);
        }
        lowercasePublisher[strlen(books[i].publisher)] = '\0';

        if (strstr(lowercaseTitle, lowercaseQuery) != NULL ||
                strstr(lowercaseAuthors, lowercaseQuery) != NULL ||
                strstr(lowercasePublisher, lowercaseQuery) != NULL)
        {
            matches[numMatches] = i;
            numMatches++;
        }

    }

    if (numMatches > 0)
    {
        printf("Found %d match(es):\n", numMatches);
        printf("S/No   ISBN           Title                Authors       Date of publication     Publisher        \n");
        for (int i = 0; i < numMatches; i++)
        {
            int index = matches[i];
            printf("%d  %s            %s                   %s             %s                      %s\n",
                   index + 1, books[index].isbn, books[index].title,books[index].authors, books[index].publisher, books[index].date);
        }
    }
    else
    {
        printf("No matches found.\n");
    }

    return numMatches;
}
void purchaseItem(Book books[], int numBooks, const char* query)
{
    int matches[MAX_BOOKS];
    int numMatches = 0;

    // Convert query to lowercase and remove extra spaces
    char lowercaseQuery[MAX_TITLE_LENGTH];
    int j = 0;
    for (int i = 0; query[i] != '\0'; i++)
    {
        if (query[i] != ' ' || (j > 0 && query[i-1] != ' '))   // Skip extra spaces
        {
            lowercaseQuery[j] = tolower(query[i]);
            j++;
        }
    }
    lowercaseQuery[j] = '\0';

    // Check for partial matches (case-insensitive)
    for (int i = 0; i < numBooks; i++)
    {
        char lowercaseTitle[MAX_TITLE_LENGTH];
        char lowercaseAuthors[MAX_AUTHOR_LENGTH];
        char lowercasePublisher[MAX_PUBLISHER_LENGTH];
        // Convert book fields to lowercase for comparison
        for (int j = 0; books[i].title[j] != '\0'; j++)
        {
            lowercaseTitle[j] = tolower(books[i].title[j]);
        }
        lowercaseTitle[strlen(books[i].title)] = '\0';
        for (int j = 0; books[i].authors[j] != '\0'; j++)
        {
            lowercaseAuthors[j] = tolower(books[i].authors[j]);
        }
        lowercaseAuthors[strlen(books[i].authors)] = '\0';
        for (int j = 0; books[i].publisher[j] != '\0'; j++)
        {
            lowercasePublisher[j] = tolower(books[i].publisher[j]);
        }
        lowercasePublisher[strlen(books[i].publisher)] = '\0';

        if (strstr(lowercaseTitle, lowercaseQuery) != NULL ||
                strstr(lowercaseAuthors, lowercaseQuery) != NULL ||
                strstr(lowercasePublisher, lowercaseQuery) != NULL)
        {
            matches[numMatches] = i;
            numMatches++;
        }
    }

    if (numMatches > 0)
    {
        printf("Found %d match(es):\n", numMatches);
        printf("S/No  ISBN   Title   Authors   Date of publication  Publisher\n");
        for (int i = 0; i < numMatches; i++)
        {
            int index = matches[i];
            printf("%d  %s            %s                   %s             %s                      %s\n",
                   index + 1, books[index].isbn, books[index].title,books[index].authors, books[index].publisher, books[index].date);
        }

        bool validSelection = false;
        int selection;
        while (!validSelection)
        {
            printf("Enter the S/No of the book to purchase, in the listed S/No: ");
            scanf("%d", &selection);

            // Check if the selection is within the range of valid S/Nos
            for (int i = 0; i < numMatches; i++)
            {
                if (selection == matches[i] + 1)
                {
                    validSelection = true;
                    break;
                }
            }

            if (!validSelection)
            {
                printf("Invalid S/No. Please enter a listed S/No: \n");
            }
        }

        int index = selection - 1;
        printf("You have purchased the following book:\n");
        printf("S/No ISBN  Title  Authors  Date of publication  Publisher\n");
        printf("%d  %s            %s                   %s             %s                      %s\n",
               index + 1, books[index].isbn, books[index].title,books[index].authors, books[index].publisher, books[index].date);

        printf("Purchase successful.\n");
    }
    else
    {
        printf("No matches found.\n");
    }
}

void payForItem(Book books[], int numBooks, const char* query)
{


int matches[MAX_BOOKS];
int numMatches = 0;

// Convert query to lowercase and remove extra spaces
char lowercaseQuery[MAX_TITLE_LENGTH];
int j = 0;
for (int i = 0; query[i] != '\0'; i++)
{
    if (query[i] != ' ' || (j > 0 && query[i-1] != ' '))   // Skip extra spaces
    {
        lowercaseQuery[j] = tolower(query[i]);
        j++;
    }
}
lowercaseQuery[j] = '\0';

// Check for partial matches (case-insensitive)
for (int i = 0; i < numBooks; i++)
{
    char lowercaseTitle[MAX_TITLE_LENGTH];
    char lowercaseAuthors[MAX_AUTHOR_LENGTH];
    char lowercasePublisher[MAX_PUBLISHER_LENGTH];
    // Convert book fields to lowercase for comparison
    for (int j = 0; books[i].title[j] != '\0'; j++)
    {
        lowercaseTitle[j] = tolower(books[i].title[j]);
    }
    lowercaseTitle[strlen(books[i].title)] = '\0';
    for (int j = 0; books[i].authors[j] != '\0'; j++)
    {
        lowercaseAuthors[j] = tolower(books[i].authors[j]);
    }
    lowercaseAuthors[strlen(books[i].authors)] = '\0';
    for (int j = 0; books[i].publisher[j] != '\0'; j++)
    {
        lowercasePublisher[j] = tolower(books[i].publisher[j]);
    }
    lowercasePublisher[strlen(books[i].publisher)] = '\0';

    if (strstr(lowercaseTitle, lowercaseQuery) != NULL ||
            strstr(lowercaseAuthors, lowercaseQuery) != NULL ||
            strstr(lowercasePublisher, lowercaseQuery) != NULL)
    {
        matches[numMatches] = i;
        numMatches++;
    }
}

if (numMatches > 0)
{
    printf("Found %d match(es):\n", numMatches);
    printf("S/No   ISBN           Title                Authors              Date of publication      Publisher\n");
    for (int i = 0; i < numMatches; i++)
    {
        int index = matches[i];
        printf("%d  %s  %s     %s     %s         %s\n",
               index + 1, books[index].isbn, books[index].title,books[index].authors, books[index].publisher, books[index].date);
    }

    bool validSelection = false;
    int selection;
    while (!validSelection)
    {
        printf("Enter the S/No of the book you want to purchase and pay for, in the listed S/No: ");
        scanf("%d", &selection);

        // Check if the selection is within the range of valid S/Nos
        for (int i = 0; i < numMatches; i++)
        {
            if (selection == matches[i] + 1)
            {
                validSelection = true;
                break;
            }
        }

        if (!validSelection)
        {
            printf("Invalid S/No. Please enter a listed S/No: \n");
        }
    }

    int index = selection - 1;
    printf("You have selected the following book:\n");
    printf("S/No   ISBN           Title                Authors     Date of publication       Publisher        \n");
    printf("%d  %s %s %s %s  %s\n",
           index + 1, books[index].isbn, books[index].title,books[index].authors,books[index].date), books[index].publisher;

    // Generate random price between 500 and 2500
    srand(time(NULL));  // Initialize random seed
    int price = rand() % 2001 + 500;
    printf("The price of the selected book is: %d\n", price);

    printf("Do you want to proceed with the payment? (Y/N): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter the amount of money you have: ");
        int amount;
        scanf("%d", &amount);

        if (amount == price)
        {
            printf("Payment successful.\n");
        }
        else if (amount > price)
        {
            printf("Payment successful. Your change: %d\n", amount - price);
        }
        else
        {
            printf("Payment failed. Remaining balance: %d\n", price - amount);
        }
    }
    else
    {
        printf("Payment canceled.\n");
    }


}
else
{
    printf("No matches found.\n");
}


}
