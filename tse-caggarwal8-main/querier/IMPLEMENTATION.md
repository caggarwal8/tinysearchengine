# CS50 TSE Querier
## Implementation Spec

## CJ Aggarwal, May 2021

This implementation spec was written with the help of the given implementation spec for the crawler module.

## Data structures

We utilize a few data structures:

The given index from the indexfile
A counter to store the document IDs and the scores for the documents that satisfy the query
A score that is a struct - this contains an id and a score and it will be used to build the final array of scores
A countersHolder struct which holds two different counters - this is used to help with handling "ands"

## Control Flow

Pseudocode:

        parse arguments and ensure that they are valid
              ensure the pageDirectory is a crawler directory
              ensure that indexfile is readable
        load the indexfile into a new index
        print "Query? " to stdout and parse stdin as long as not EOF
              ensure that stdin is valid
                     ensure that all characters are either letters or spaces
                     ensure that there is no misuse of operators
              normalize the query
              reprint the normalized query to stdout
              create the counter
                     iterate through the array of words and use different counter iterators to account for the use of ands, ors, and implicit ands
                     also account for and precedence over or
              create the array of scores from the counter
                     loop through the files
                          if score of a file is greater than zero
                             add it to the array of scores
              sort the array of scores in descending order
              print the array of scores to stdout
              ask for another query
        delete the index
        exit with zero status if no errors
        
## Function prototypes

The querier is built through a lot of individual functions, but, together, they create the module.

These functions are as follows:

```c
bool checkArgs(int argc, char* argv[]);
void startQuery(index_t* index, char* pageDir);
void executeQuery(char* words[], int num, char* input, char* pageDir, index_t* index);
void print(score_t** scoresRanked, int numOfMatches, char* pageDir);
bool validChars(char* input);
bool parse(char* input, char** words, int* num);
void normalize(char* input, char** words, int* num);
bool validOperators(char** words, int* num);
bool isOperator(char* word);
counters_t* counterCreation(index_t* index, char** words, int num);
void intersection(void* arg, const int key, int count);
void duplicate(void* arg, const int key, int count);
void unionSum(void* arg, const int key, int count);
score_t** score (counters_t* result, int numOfDocs, int numOfFiles);
score_t** sort (score_t** output, int numOfDocs);
void counter (void* arg, const int key, int count);
int numFiles (char* pageDir);
```

Please see querier.c for more detailed information about each specific function.

We also make use of these functions from other modules.

Index:

```c
/*
 * index_new - creates a new index data structue with the specified
 * number of slots
 *
 * employs hashtable_new
 *
 * Input: int slots - number of slots requested
 * Output: initialized index_t*
 */
index_t* index_new(int slots);

/*
 * index_find - searches for the given key in the given index
 * and returns a pointer to the counter if found
 *
 * input: index_t* index- the given index; char*key - the given key
 * 
 * output: the counter if found; null if not
 */
void* index_find(index_t* index, char* key);

/*
 * index_delete - deletes a given index
 *
 * input: the index to be deleted
 * ouput: none 
 */
void index_delete(index_t* index);

/*
 * loads a file into an index data structure by parsing through the file
 *
 * input: the index to be written into and the file to be read
 *
 * output: none
 */
void index_load(index_t* index, FILE* fp);
```

pageDir:

```c
/* pagedir_validate
 *
 * Validates that the given directory name is not null, exists,
 * and is indeed a readable crawler directory
 *
 * It checks by opening the file named ".crawler" in the given
 * directory path
 *
 * Input:
 * char* directory - the name of the directory
 *
 * Output:
 * True if the directory is valid; false if otherwise
 */
bool pagedir_validate(char* directory);
```

Word:

```c
/* normalizeWord - makes words all lowercase
 * 
 * This makes it readable for index and allows the module
 * to accurate count instances of each word
 *
 * Input: char* word - the word to be normalized
 *
 * Output: none
 */
void normalizeWord(char* word);
```

Counters:

```
/* Create a new (empty) counter structure.
 *
 * We return:
 *   pointer to a new counterset; NULL if error (out of memory).
 * We guarantee:
 *   counterset is intialized empty.
 * Caller is responsible for:
 *   later calling counters_delete();
 */
counters_t* counters_new(void);

/**************** counters_add ****************/
/* Increment the counter indicated by key.
 * 
 * Caller provides:
 *   valid pointer to counterset, and key (must be >= 0)
 * We return:
 *   the new value of the counter related to the indicated key.
 *   0 on error (if ctrs is NULL, key is negative, or out of memory)
 * We guarantee:
 *   the counter's value will be >= 1, on successful return.
 * We do:
 *  ignore if ctrs is NULL or key is negative.
 *  if the key does not yet exist, create a counter for it and initialize to 1.
 *  if the key does exist, increment its counter by 1.
 */
int counters_add(counters_t* ctrs, const int key);

/**************** counters_get ****************/
/* Return current value of counter associated with the given key.
 *
 * Caller provides:
 *   valid pointer to counterset, and key (must be >= 0)
 * We return:
 *   current value of counter associated with the given key, if present;
 *   0 if ctrs is NULL, key < 0, or key is not found.   
 * Note:
 *   counterset is unchanged as a result of this call.
 */
int counters_get(counters_t* ctrs, const int key);

/**************** counters_set ****************/
/* Set the current value of counter associated with the given key.
 *
 * Caller provides:
 *   valid pointer to counterset, 
 *   key (must be >= 0), 
 *   counter value (must be >= 0).
 * We return:
 *   false if ctrs is NULL, if key < 0 or count < 0, or if out of memory.
 *   otherwise returns true.
 * We do:
 *   If the key does not yet exist, create a counter for it and initialize to
 *   the given value. 
 *   If the key does exist, update its counter value to the given value.
 */
bool counters_set(counters_t* ctrs, const int key, const int count);

/**************** counters_iterate ****************/
/* Iterate over all counters in the set.
 *
 * Caller provides:
 *   valid pointer to counterset, 
 *   arbitrary void* arg,
 *   valid pointer to itemfunc that can handle one item.
 * We do:
 *   nothing, if ctrs==NULL or itemfunc==NULL.
 *   otherwise, call itemfunc once for each item, with (arg, key, count).
 * Note:
 *   the order in which items are handled is undefined.
 *   the counterset is unchanged by this operation.
 */
void counters_iterate(counters_t* ctrs, void* arg, 
                      void (*itemfunc)(void* arg, 
                                       const int key, const int count));

/**************** counters_delete ****************/
/* Delete the whole counterset.
 *
 * Caller provides:
 *   a valid pointer to counterset.
 * We do:
 *   we ignore NULL ctrs.
 *   we free all memory we allocate for this counterset.
 */
void counters_delete(counters_t* ctrs);
```

## Error handling

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

All code (hopefully) uses defensive-programming tactics to catch and exit.

## Testing Plan

As mentioned in the DEISGN.md, we will use a bash testing script for integration testing. This means that we will completely build the querier and test the entire function of the module through commands in the script.

We will test a number of test cases. Please see DESIGN.md or testing.sh or testing.out for these specific test cases.