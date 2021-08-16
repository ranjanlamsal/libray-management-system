#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "head.h"
void mainmenu();
//structure to store date
typedef struct Date
{
    int yyyy;
    int mm;
    int dd;
} Date;
typedef struct userinfo
{
    char username[MAX_SIZE_USER_NAME];
    char password[MAX_SIZE_PASSWORD];
} userinfo;
typedef struct BooksInfo // to call in program
{
    unsigned int book_id;         // declare the integer data type
    char title[MAX_BOOK_NAME];    // declare the character data type
    char author[MAX_AUTHOR_NAME]; // declare the charecter data type
    Date bookIssueDate;           // declare the integer data type
} BooksInfo;

void clear()
{
#ifdef _WIN32 //identifying the os of user and clearing the screen accordingly
    system("cls");
#else
    system("clear");
#endif
}

int isValidDate(Date *validDate)
{
    //check range of year,month and day
    if (validDate->yyyy > MAX_YR ||
        validDate->yyyy < MIN_YR)
        return 0;
    if (validDate->mm < 1 || validDate->mm > 12)
        return 0;
    if (validDate->dd < 1 || validDate->dd > 31)
        return 0;
    return 1;
}

void printCenter(const char *message)
{
    int len = 0;
    int pos = 0;
    //calculate how many space need to print
    len = (78 - strlen(message)) / 2;
    printf("\t\t\t");
    for (pos = 0; pos < len; pos++)
    {
        //print space
        printf(" ");
    }
    //print message
    printf("%s", message);
}

void key()
{
    char w;
    printCenter("Press any key to continue...\n\n");
    scanf("%c", &w);
    mainmenu();
}

void head(const char *message)
{
    printf("\n\t\t\t############---------------------------------------------------############\n");
    printCenter(message);
    printf("\n\t\t\t############---------------------------------------------------############\n\n");
}

void welcome()
{
    printf("\n\n\n\n\n");
    printf("\n\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    printf("\n\t\t\t        =                       WELCOME                        =");
    printf("\n\t\t\t        =                         TO                           =");
    printf("\n\t\t\t        =                      RANJAN'S                        =");
    printf("\n\t\t\t        =                       LIBRARY                        =");
    printf("\n\t\t\t        =                     MANAGEMENT                       =");
    printf("\n\t\t\t        =                       SYSTEM                         =");
    printf("\n\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n\n");

    key();
}

void add_book()
{
    clear();
    int days;
    int status = 0;
    BooksInfo addBooksInfo = {0};
    FILE *fp;
    fp = fopen(BOOK_FILE, "a");

    if (fp == NULL)
    {
        printf("\n\t\t\tFile Cannot Be Created !!");
        exit(1);
    }
    head("ADD NEW BOOKS");
    printf("\n\n\t\t\tENTER BOOK DETAILS BELOW:");
    printf("\n\t\t\t---------------------------------------------------------------------------\n");

    printf("\n\t\t\tBook ID NO  :");
    scanf("%d", &addBooksInfo.book_id);

    printf("\n\t\t\tBook NAME   : ");
    scanf(" %[^\n]s", addBooksInfo.title);

    printf("\n\t\t\tAUTHOR NAME : ");
    scanf(" %[^\n]s", addBooksInfo.author);

    do
    {
        //get date year,month and day from user
        printf("\n\t\t\tEnter date in format (day/month/year): ");
        scanf("%d/%d/%d", &addBooksInfo.bookIssueDate.dd, &addBooksInfo.bookIssueDate.mm, &addBooksInfo.bookIssueDate.yyyy);
        //check date validity
        status = isValidDate(&addBooksInfo.bookIssueDate);
        if (!status)
        {
            printf("\n\t\t\tPlease enter a valid date.\n");
        }
    } while (!status);
    fwrite(&addBooksInfo, sizeof(addBooksInfo), 1, fp);
    fclose(fp);
    clear();
    printCenter("BOOK ADDED SUCCESSFULLY..\n\n");
    key();
}

void search_book()
{
    clear();
    FILE *fp;
    int book_id, is_found = 0, choice;
    BooksInfo addBooksInfo = {0};
    char title[MAX_BOOK_NAME] = {0};
    clear();

    fp = fopen(BOOK_FILE, "r");
    printCenter("Enter Book Id to search:\n");
    scanf("%d", &book_id);

    if (fp == NULL)
    {
        printf("\n\t\t\tFile is not created yet Created !!\n Add some books first!!!\n\n");
        exit(1);
    }
    while (fread(&addBooksInfo, sizeof(BooksInfo), 1, fp))
    {
        if (addBooksInfo.book_id == book_id)
        {
            is_found = 1;
            clear();
            printf("\n\t\t\tBook id = %u\n", addBooksInfo.book_id);
            printf("\t\t\tBook name = %s", addBooksInfo.title);
            printf("\t\t\tBook authorName = %s", addBooksInfo.author);
            printf("\t\t\tBook issue date(day/month/year) =  (%d/%d/%d)", addBooksInfo.bookIssueDate.dd,
                    addBooksInfo.bookIssueDate.mm, addBooksInfo.bookIssueDate.yyyy);
        }
    }
    if (!is_found)
    {
        printCenter("BOOK Not Found!!!\n");
    }
    fclose(fp);

    key();
}


void view_Books()
{
    int is_found = 0;
    BooksInfo BooksInfo;
    FILE *fp = NULL;
    int status = 0;
    unsigned int count_Book = 1;
    head("BOOKS LIST");
    fp = fopen(BOOK_FILE, "r");
    if (fp == NULL)
    {
        printf("\n\t\t\tFile is not created. Add some books first!!!\n\n");
        exit(1);
    }
    printf("\n\n\t\t\tS.N.\t\tTitle\t\tAuthor\t\tBook ID\n");
    while (fread(&BooksInfo, sizeof(BooksInfo), 1, fp))
    {
        printf("\t\t\t%d: \t %s \t\t%s \t\t %d \n\n\n", count_Book, BooksInfo.title, BooksInfo.author, BooksInfo.book_id);
        is_found = 1;
        ++count_Book;
    }
    fclose(fp);
    if (!is_found)
    {
        printf("\n\t\t\tNo Record");
    }
    key();
}

void delete_Book()
{
    int is_found = 0, bookDelete = 0;
    BooksInfo addBooksInfo = {0};
    FILE *fp = NULL;
    FILE *tempfile = NULL;
    int status = 0;
    head("Delete Books Details");
    fp = fopen(BOOK_FILE, "r");
    if (fp == NULL)
    {
        printf("\n\t\t\tFile is not created. No books recorded!!!\n");
        exit(1);
    }
    tempfile = fopen("tempfile.txt", "a");
    if (tempfile == NULL)
    {
        fclose(fp);
        printf("\n\t\t\tFile did not open\n");
    }

    printf("\n\t\t\tEnter Book ID NO. for delete:");
    scanf("%d", &bookDelete);
    while (fread(&addBooksInfo, sizeof(addBooksInfo), 1, fp))
    {
        if (addBooksInfo.book_id != bookDelete)
        {
            fwrite(&addBooksInfo, sizeof(addBooksInfo), 1, tempfile);
        }
        else
        {
            is_found = 1;
        }
    }

    if (is_found)
    {
        printf("\n\t\t\tRecord deleted successfully.....");
    }
    else
    {
        printf("\n\t\t\tRecord not found");
    }

    fclose(fp);
    fclose(tempfile);
    remove(BOOK_FILE);
    rename("tempfile.txt", BOOK_FILE);

    key();
}

void edit_book()
{
    int is_found = 0, book_Edit_id = 0, book_id;
    BooksInfo addBooksInfo = {0};
    FILE *fp = NULL;
    FILE *tempfile = NULL;
    int status = 0;
    printCenter("EDIT BOOKS DETAILS");
    fp = fopen(BOOK_FILE, "r");
    if (fp == NULL)
    {
        printf("\n\t\t\tFile is not created. No books recorded!!!\n");
        exit(1);
    }
    tempfile = fopen("tempfile.txt", "a");

    printf("\n\t\t\tEnter Book ID NO. to edit:");
    scanf("%d", &book_Edit_id);
    while (fread(&addBooksInfo, sizeof(addBooksInfo), 1, fp))
    {
        if (addBooksInfo.book_id != book_Edit_id)
        {
            fwrite(&addBooksInfo, sizeof(addBooksInfo), 1, tempfile);
        }
        else
        {
            is_found = 1;
        }
    }

    if (is_found)
    {
        BooksInfo Edited_Books_Info = {0};
        FILE *fp = NULL;
        head("EDIT BOOK");
        printf("\n\n\t\t\tENTER BOOK DETAILS BELOW:");
        printf("\n\t\t\t---------------------------------------------------------------------------\n");

        printf("\n\t\t\tBook ID NO  :");
        scanf("%d", &addBooksInfo.book_id);

        printf("\n\t\t\tBook NAME   : ");
        scanf(" %[^\n]s", &addBooksInfo.title);

        printf("\n\t\t\tAUTHOR NAME : ");
        scanf(" %[^\n]s", &addBooksInfo.author);
        do
        {
            //get date year,month and day from user
            printf("\n\t\t\tEnter date in format (day/month/year): ");
            scanf("%d/%d/%d", &addBooksInfo.bookIssueDate.dd, &addBooksInfo.bookIssueDate.mm, &addBooksInfo.bookIssueDate.yyyy);
            //check date validity
            status = isValidDate(&addBooksInfo.bookIssueDate);
            if (!status)
            {
                printf("\n\t\t\tPlease enter a valid date.\n");
            }
        } while (!status);
        fwrite(&addBooksInfo, sizeof(addBooksInfo), 1, tempfile);
    }

    else
    {
        printf("\n\t\t\tRecord not found");
    }

    fclose(fp);
    fclose(tempfile);
    remove(BOOK_FILE);
    rename("tempfile.txt", BOOK_FILE);

    printf("\n\t\t\tBook edited successfully......\n\n");

    key();
}

void mainmenu()
{
    int choice = 0;
    do
    {
        head("MAIN MENU");
        printf("\n\n\t\t\t1.Add Books");
        printf("\n\t\t\t2.Search Books");
        printf("\n\t\t\t3.View Books");
        printf("\n\t\t\t4.Delete Book");
        printf("\n\t\t\t5.Edit Book");
        printf("\n\t\t\t0.Exit");
        printf("\n\n\n\t\t\tEnter choice => ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            head("ADD BOOK");
            printCenter("To updates book added in the library\n\n");
            add_book();
            break;
        case 2:
            head("DISCOVER BOOK");
            printCenter("To search book that exist in the library\n\n");
            search_book();
            break;
        case 3:
            head("VIEW BOOKS");
            printCenter("To view all the books that exist in the library\n\n");
            view_Books();
            break;
        case 4:
            head("DELETE BOOKS");
            printCenter("To delete the book that exist in the library\n\n");
            delete_Book();
            break;
        case 5:
            head("EDIT BOOKS");
            printCenter("To EDIT the book that exist in the library\n\n");
            edit_book();
            break;
        case 0:
            head("THANK YOU");
            exit(1);
            break;
        default:
            printf("\n\n\n\t\t\tINVALID INPUT!!! Try again...");
            key();
        }                  //Switch Ended
    } while (choice != 0); //Loop Ended
}

int main()
{
    welcome();
}