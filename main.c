#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BOOK_NAME_SIZE 50
#define BUFFER_LENGTH 500
#define LIBRARY_SIZE 1000

typedef struct{ // book structure
    char name[BOOK_NAME_SIZE];
    int id;
    int quantity;
}Book;

void deleteBookByID(const int size,const Book const *books);
void printAllBooks(const Book const *books,const int size);
void sortArray(const Book const *books,Book *sortedBooks,const int size);
void searchByName(const Book const *sortedBooks,const int size);
void searchByID(const Book const *books,const int size,int startIndex,int id);
void instertBook(Book *books);
int loadBooksFromFile(Book *books);
void getInput(char *name,int *id,int *quantity);

int main() // program start point
{
    Book books[LIBRARY_SIZE]; // array of Book structure to save unsorted books
    Book sortedBooks[LIBRARY_SIZE]; // array of Book structure to save sorted books
    int id;
    int size = loadBooksFromFile(books); // loading books from the library file which returns the num of the books
    sortArray(books,sortedBooks,size); // copying the books array to the sortedBooks array then sort it by name

    while(1){// loop until user enter the exit condition then break

        // display all operations
        printf("Welcome to the library. current books number is %d\n\n",size);
        puts("1-Insert a book (id, name, quantity).");
        puts("2-Delete a book by (id).");
        puts("3-Search a book by id and display its name and quantity.");
        puts("4-Search a book by name and display its id and quantity.");
        puts("5-Display all books sorted by name,and their corresponding ids and quantity.");
        puts("6-Display all books unsorted, their ids,names and quantity (as entered).\n");
        printf("choose an operation: ");

        // getting user choice
        int op;
        scanf("%d",&op);
        getchar();

        switch(op){ // check user input to operate the selected operation

        case 1: // insert book if the user choose 1
            instertBook(books);
            size = loadBooksFromFile(books);
            sortArray(books,sortedBooks,size);
            puts("books after insertion:\n");
            printAllBooks(books,size);
            break;

        case 2: // delete book by id if the user choose 2
            deleteBookByID(size,books);
            size = loadBooksFromFile(books);
            sortArray(books,sortedBooks,size);
            puts("books after deletion:\n");
            printAllBooks(books,size);
            break;

        case 3: // search for a book by id if user choose 3
            getInput(NULL,&id,NULL);
            searchByID(books,size,0,id);
            break;

        case 4: // search for a book by name if user choose 4
            searchByName(sortedBooks,size);
            break;

        case 5: // display all books sorted by name if user choose 5
            printAllBooks(sortedBooks,size);
            break;

        case 6: // display all books unsorted if user choose 5
            printAllBooks(books,size);
            break;

        default:
            puts("Wrong input.");
            break;
        }


        puts("\n**************\nDo you want to perform any additional operation ? (yes or no)"); // check if the user wants to continue

        char *condition = "yes"; // keep going condition

        // get user input
        char *userInput;
        gets(userInput);
        if(strcmp(userInput,condition)){ // break if the user wants to exit
            break;
        }

        system("@cls||clear"); // clean the console screen
    }
}

void getInput(char *name,int *id,int *quantity){ // getting input from the user
    if(name != NULL){
        puts("Enter The Book Name: ");
        gets(name); // get book name from the user
    }
    if(id != NULL){
        puts("Enter the id: ");
        scanf("%d",id); // get book id from the user
        getchar();
    }
    if(quantity != NULL){
        puts("Enter the quantity: ");
        scanf("%d",quantity); //get book quantity from the user
        getchar();
    }
}

int loadBooksFromFile(Book *books){ // loading books from file then append it into the books array

    FILE* filePointer;
    int counter =0; // counter for number of books
    char line[BUFFER_LENGTH];

    filePointer = fopen("library.txt", "a+"); // opening the library file

    while(fgets(line, BUFFER_LENGTH, filePointer)) { // loop on the file lines

        char * token = strtok(line, ","); // split the line every comma

        for(int i =0; token != NULL;i++ ) { // loop on the line to split it

            if(i== 0){ // first string is the book name
                strcpy(books[counter].name,token); // set book name
            }else if(i == 1){ // second string is the book id
                books[counter].id = atoi(token); // set book id
            }else{ // third string is the book quantity
                books[counter].quantity = atoi(token);//set book quantity
            }
            token = strtok(NULL, ",");
        }

        if(strcmp(books[counter].name,"\n")){// if name is not empty
            counter++; // add 1 to the number of books
        }
    }

    fclose(filePointer); // closing the library file

    return counter;
}

void instertBook(Book *books){ // adding a book to the library

    FILE* filePointer;
    Book book;
    filePointer = fopen("library.txt", "a+"); // opening the library file

    getInput(book.name,&book.id,&book.quantity); // getting input from user

    fprintf(filePointer,"\n%s,%d,%d",book.name,book.id,book.quantity); // append the book to the file

    fclose(filePointer); // closing the file

    puts("Added the book to the library");
}


void searchByID(const Book const *books,const int size,int startIndex,int id){ // search for a book by id

    if(startIndex>size){ // check if the no book found
        puts("Not Found");
        return;
    }

    if(books[startIndex].id == id){ // check if the book id is equal to the id
        printf("name: %s,Quantity: %d\n",books[startIndex].name,books[startIndex].quantity); // printing book details
        return;
    }
    return searchByID(books,size,startIndex+1,id); // using recursion to search for the book
}

void searchByName(const Book const *sortedBooks,const int size){ // searching for the book by name

    //getting the name from the user
    char name[BOOK_NAME_SIZE];
    getInput(name,NULL,NULL);

    // binary search
    int start=0,end=size;
    int middle;

    //searching for the name
     while (start <= end)
    {
        middle = start+(end-1)/2;
        if(strcmp(sortedBooks[middle].name,name)>0){
            end = middle-1;
        }
        else if (strcmp(sortedBooks[middle].name,name)==0) {// check if the book name equal to the name
            printf("id: %d,Quantity: %d\n",sortedBooks[middle].id,sortedBooks[middle].quantity); // display book info if found
            return;
        }
        else if(strcmp(sortedBooks[middle].name,name)<0){
        start = middle +1;
        }
    }
    puts("Not Found"); // when the loop ends without finding the book display not found
    return;
}

void sortArray(const Book const *books,Book *sortedBooks,const int size){ // sorting books by name

    //copying books to sortedBooks
    for(int i =0;i<size;i++){
        sortedBooks[i] = books[i];
    }

    //sorting the array
    for(int i =0;i<size;i++){
        for(int j =0;j<size-1;j++){
            if(strcmp(sortedBooks[j].name,sortedBooks[j+1].name)>0){ // check if the first name > second name then reverse them
                Book temp = sortedBooks[j+1];
                sortedBooks[j+1] = sortedBooks[j];
                sortedBooks[j] = temp;
            }
        }
    }
}

void printAllBooks(const Book const *books,const int size){ // display all books
    printf("\n%-10s%-50s%-10s%-10s\n\n","num","name","id","quantity");
    for(int i=0;i<size;i++){ // loop from 0 to the size of the array and print every book
        printf("%-10d%-50s%-10d%-10d\n",i+1,books[i].name,books[i].id,books[i].quantity);
    }
}

void deleteBookByID(const int size,const Book const *books){ // deleting book by id
    FILE *tempFile; // temp file to move all books except the selected id
    char line[BUFFER_LENGTH];

    //opening temp file
    tempFile = fopen("temp.txt","w");

    // getting the book id from the user
    int id;
    getInput(NULL,&id,NULL);

    for(int i=0;i<size;i++) { // loop on every line in the main file
        if(books[i].id != id){ // check if the book id not equal to id then add the book to the temp file
            if(books[i].name != NULL){
                fprintf(tempFile,"%s,%d,%d\n",books[i].name,books[i].id,books[i].quantity);
            }
        }else{
            printf("(%s) book has been deleted.\n",books[i].name);
        }
    }
    fclose(tempFile);

    remove("library.txt"); // removing the main file
    rename("temp.txt","library.txt"); // rename the temp file to be as same as the main file
}
