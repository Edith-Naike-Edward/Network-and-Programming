#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            printf("Enter the title, author, or publisher of the book to pay for: ");
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

    // Check for partial matches
    for (int i = 0; i < numBooks; i++)
    {
        if (strstr(books[i].title, query) != NULL || strstr(books[i].authors, query) != NULL ||
                strstr(books[i].publisher, query) != NULL)
        {
            matches[numMatches] = i;
            numMatches++;
        }
    }

    if (numMatches > 0)
    {
        printf("Found %d match(es):\n", numMatches);
        printf("S/No   ISBN           Title                Authors            Publisher        Date of publication\n");
        for (int i = 0; i < numMatches; i++)
        {
            int index = matches[i];
            printf("%d %s %s %s %s %s\n", index + 1, books[index].isbn, books[index].title,books[index].authors, books[index].publisher, books[index].date);
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

    // Check for partial matches
    for (int i = 0; i < numBooks; i++)
    {
        if (strstr(books[i].title, query) != NULL || strstr(books[i].authors, query) != NULL ||
                strstr(books[i].publisher, query) != NULL)
        {
            matches[numMatches] = i;
            numMatches++;
        }
    }

    if (numMatches > 0)
    {
        printf("Found %d match(es):\n", numMatches);
        printf("S/No   Title                         Authors                ISBN            Publisher        Date of publication\n");
        for (int i = 0; i < numMatches; i++)
        {
            int index = matches[i];
            printf("%-6d %-30s %-20s %-15s %-20s %s\n", index + 1, books[index].title, books[index].authors,
                   books[index].isbn, books[index].publisher, books[index].date);
        }

        int selection;
        printf("Enter the S/No of the book to purchase: ");
        scanf("%d", &selection);

        if (selection >= 1 && selection <= numMatches)
        {
            int index = matches[selection - 1];
            printf("You have purchased the following book:\n");
            printf("Title: %s\n", books[index].title);
            printf("Author(s): %s\n", books[index].authors);
            printf("ISBN: %s\n", books[index].isbn);
            printf("Publisher: %s\n", books[index].publisher);
            printf("Date of publication: %s\n", books[index].date);

            // Perform the purchase logic here
            // ...

            printf("Purchase successful.\n");
        }
        else
        {
            printf("Invalid selection.\n");
        }
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

    // Check for partial matches
    for (int i = 0; i < numBooks; i++)
    {
        if (strstr(books[i].title, query) != NULL || strstr(books[i].authors, query) != NULL ||
                strstr(books[i].publisher, query) != NULL)
        {
            matches[numMatches] = i;
            numMatches++;
        }
    }

    if (numMatches > 0)
    {
        printf("Found %d match(es):\n", numMatches);
        printf("S/No   Title                         Authors                ISBN            Publisher        Date of publication\n");
        for (int i = 0; i < numMatches; i++)
        {
            int index = matches[i];
            printf("%-6d %-30s %-20s %-15s %-20s %s\n", index + 1, books[index].title, books[index].authors,
                   books[index].isbn, books[index].publisher, books[index].date);
        }

        int selection;
        printf("Enter the S/No of the book to pay for: ");
        scanf("%d", &selection);

        if (selection >= 1 && selection <= numMatches)
        {
            int index = matches[selection - 1];
            printf("You have paid for the following book:\n");
            printf("Title: %s\n", books[index].title);
            printf("Author(s): %s\n", books[index].authors);
            printf("ISBN: %s\n", books[index].isbn);
            printf("Publisher: %s\n", books[index].publisher);
            printf("Date of publication: %s\n", books[index].date);

            // Perform the payment logic here
            // ...

            printf("Payment successful.\n");
        }
        else
        {
            printf("Invalid selection.\n");
        }
    }
    else
    {
        printf("No matches found.\n");
    }
}
