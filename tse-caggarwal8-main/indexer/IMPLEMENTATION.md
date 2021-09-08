# CS50 TSE Indexer
## Implementation Spec

## CJ Aggarwal, May 2021

Indexer reads files in a pageDirectory, creates webpages, and builds an index that tracks the number of instances of each word in each document (specified by id).

The indexer then prints the index to a file.

## Data Structures

We utilize the index_t* data structure for this module. It is made from a hashtable that stores words and a counter that stores the documentID and the count of each word.

This data structure comes from index.c - please see index.h for more information on the functions and methods within index.c

## Control Flow

Indexer is implemented in indexer.c which has five functions.

### Main

Main checks the arguments by calling checkArgs. If it is valid, then it calls indexBuild, then index_save to write to the given file, then it deletes the index, closes the file, and returns 0.

### checkArgs

Confirms that the arguments are valid. The given pageDirectory must be a readable crawler directory and the given filename must be writeable.

There also should only be two arguments.

Validate pageDirectory by calling pagedir_validate
Validate indexFilename by opening it for writing

### indexBuild

Builds an index out of the given pageDirectory.

Pseudocode:

        initialize new index_t*
        start id at 1
        initialize a new webpage and set it to null
        create a new filename and write the pageDirectory and id to it in the format pageDirectory/id
        loop as long as filename can be read
             create a new webpage out of file by calling createWebpage
             call indexPage
             increment id
             delete webpage
             close file
             write pageDirectory and the new id to filename in the format pageDirectory/id
        return the index_t*

### indexPage

Scans the given webpage and adds words to the given index

Pseudocode:

        create a character pointer for the next word
        set an integer (pos) to 0
        loop as long as the next word from the webpage at the pos is not null
             if the word length is greather than or equal to three
                normalize the word
                create a new counter and set it to the output of index_find of the word
                if the counter is null
                   create a new counter with counter_new
                   add to the counter at the id
                   insert it into the index
                otherwise
                   add to the counter at the id
             free the character pointer

### createWebpage

Given a file, create a webpage.

Using file_readLine - set url to the first line and depth to an integer casting of the second line.
Set the html to the output of file_readFile
Create and return the new webpage with these parameters

## Other Modules

### pagedir.c

In pagedir.c, all we have added is a way to validate that a given directory is a readable crawler directory.

### pagedir_validate

To do so, we create a new filename as follows: pageDirectory/.crawler
If this file can be read, then the directory is a readable crawler directory. Otherwise, it is not.

### word.c

In word.c, we added a function to normalize a word.

### normalizeWord

To do so, we loop through the length of a word and call 'tolower'. This sets all letters to lowercase for us.

### index.c

In index.c, we created the index_t* data structure that was described above. It contains a significant number of functions, most of which are in fact one line. As such, please see index.h for more information about the file.

### libsc50

We also continue to make use of the data structure and modules within libcs50.

## Function Prototypes

Detailed descriptions of each function can be found within each file

### indexer.c

```c
int main(int argc, char* argv[]);
bool checkArgs(int argc, char* argv[]);
index_t* indexBuild(char* pageDirectory);
void indexPage(index_t* index, webpage_t* webpage, int id);
webpage_t* createWebpage(FILE* fp);
```

### word.c

```c
void normalizeWord(char* word);
```

### index.c

```c
index_t* index_new(int slots);
void* index_find(index_t* index, char* key);
bool index_insert(index_t* index, char* key, void* item);
void deleteFunction(void* item);
void index_delete(index_t* index);
void countersIterateFunction(void* fp, const int key, int count);
void iterateFunction(void* fp, const char* key, void* item);
void index_save(index_t* index, FILE* fp);
void index_load(index_t* index, FILE* fp);
```

### pagedir.c

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(char* directory);
```

## Error handling and recovery

This was written with help from the crawler implementation spec.

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

All code (hopefully) uses defensive-programming tactics to catch and exit.

## Testing

### Integration/system testing

We wrote a bash script, testing.sh, to check whether indexer.c works according to specs. It compares our outputs with the given, correct outputs, and it, thus far, has not produced any differences. We also use indextest to confirm that indexes can be loaded and saved correctly.

In addition, we run indexer with numerous wrong arguments, including no arguments, too many arguments, non-readable directories, non-writeable files, and more. Please see testing.sh for more detailed information.

We also used valgrind to check for memory leaks in both indexer and indextest. It shows that there are none!

Finally, we tested indexer against working directories like letters-10, letters-0, toscrape-1, and wikipedia-1.