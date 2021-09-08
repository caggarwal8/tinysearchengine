/*
 * querier.c - an implementation of the querier module
 *
 * The querier module produces a list of urls and their scores in descending order
 * for a given pageDirectory, index, and search query
 * 
 * Please see IMPLEMENTATION.md or DESIGN.md for more information
 *
 * CJ Aggarwal, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"

// this struct will hold the scores and ids of the documents
typedef struct score {
  int id;
  int score;
} score_t;

// this struct will be used for intersection purposes
typedef struct countersHolder {
  counters_t* one;
  counters_t* two;
} countersHolder_t;

typedef struct two_ctrs { counters_t *ctr1, *ctr2; }  two_ctrs_t;

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

/*
 * main - this function calls the other functions to handle the inputed query
 * it does not do anything in and of itself; instead, it merely relies on other
 * methods
 *
 * Input: the number of arguments and the arguments themselves
 * Output: the exit code
 */
int main(int argc, char *argv[])
{
  if (!checkArgs(argc, argv)){
    fprintf(stderr, "Arguments are invalid\n./querier pageDirectory indexFilename\n");
    exit(1);
  }

  FILE* fp;
  fp = fopen(argv[2], "r");
  
  int indexLines = file_numLines(fp);
  index_t* index = index_new(indexLines*10);

  index_load(index, fp);
  startQuery(index, argv[1]);  
  index_delete(index);
  fclose(fp);
  return 0;
}

/*
 * checkArgs - given the arguments provided by the user, this method
 * validates them and makes sure that they are usable and that the program
 * has everything it needs but nothing extra
 *
 * Input: the number of arguments and the arguments themselves
 * Output: true if the arguments are valid; otherwise, false
 */
bool
checkArgs(int argc, char* argv[])
{
  if (argc != 3) {
    return false;
  }
  if (!pagedir_validate(argv[1])) { // check if it is a crawler specific directory
    return false;
  }
  FILE* fp;
  if ((fp = fopen(argv[2], "r")) != NULL) { // must be readable
    fclose(fp);
  }
  else {
    fprintf(stderr, "Invalid indexFilename - not readable\n");
    return false;
  }
  return true;
}

/*
 * query - this function does the brunt of the work for this module
 * It handles the inputs, calling a variety of other methods, and it
 * builds the counter as well as the array of scores
 *
 * It then calls the function to print the scores in ranked order
 *
 * In addition, it communicates with the user to take in more inputs
 *
 * Input: the index and the name of the page directory
 * Output: none
 */
void
startQuery (index_t* index, char* pageDir)
{
  printf("Query? ");
  char* input = NULL;
  int num = 0;
  while ((input = file_readLine(stdin)) != NULL) {
    char* words[strlen(input)];
    if (!validChars(input)) {
      normalize(input, words, &num);
      printf("Query:"); // print out query back to stdout
      for (int i = 0; i < num; i++) {
        printf(" %s", words[i]);
      }
      printf("\n");
      fprintf(stderr, "Input is invalid - contains a non-letter and non-space character\n");
      free(input);
      printf ("\n-----------------------------------\n");
      printf("Query? ");
      num = 0;
      continue; // no need to go through the rest of the loop
    }
    normalize(input, words, &num); // normalize the query
    executeQuery(words, num, input, pageDir, index); // function that calls all other functions
    // also communicates with user
    free(input); // no need to free words because frees itself
    printf ("\n-----------------------------------\n");
    printf("Query? ");
    num = 0; // reset num back to zero so that words[] restarts at beginning
  }
  free(input);
}

/*
 * executeQuery - this function communicates with the user and calls other important functions
 * to build the counter as well as the array of scores
 *
 * Input: the words array, the number of words, the input, the page directory name, and the index
 * Output: none
 */
void
executeQuery(char* words[], int num, char* input, char* pageDir, index_t* index)
{
  if (parse(input, words, &num)) { // ensure that the query is valid
    printf("Query:"); // print out query back to stdout
    for (int i = 0; i < num; i++) {
      printf(" %s", words[i]);
    }
    printf("\n");                                                                                                                     
    counters_t* result = counterCreation(index, words, num); // create the counter
    int numOfMatches = 0;
    counters_iterate(result, &numOfMatches, counter); // count the number of matches with a count over zero
    
    if (numOfMatches == 0) { // if the counter is empty, say so and skip other functions
      printf ("\nNo documents match.\n");
    }
    
    else { // call the other important functions                                                                                                                                                          
      printf ("\nMatches %d documents (ranked):\n", numOfMatches);
      int numOfFiles = numFiles(pageDir); // count the number of files in the directory
      score_t** listOfScores = score(result, numOfMatches, numOfFiles);
      score_t** scoresSorted = sort (listOfScores, numOfMatches);
      print(scoresSorted, numOfMatches, pageDir);
      
      for (int i = 0; i < numOfMatches; i++) {
        free(scoresSorted[i]); // have to free each individual score within the array
      }
      free (scoresSorted);
    }
    counters_delete(result);
  }
}

/*
 * print - this function prints out all of the documents with their score in order of highest
 * score to lowest score
 *
 * Input: the array of scores, the number of matches to iterate through, 
 * and the name of the page directory 
 * OUtput: none
 */
void
print(score_t** scoresRanked, int numOfMatches, char* pageDir)
{
  for (int i = 0; i < numOfMatches; i++) {
    char filename [900];
    sprintf (filename, "%s/%d", pageDir, scoresRanked[i]->id);
    FILE* fp = fopen (filename, "r");
    char* link = file_readLine(fp);
    printf ("Score: %d doc: %d: %s\n", scoresRanked[i]->score, scoresRanked[i]->id, link);
    free(link);
    fclose(fp);
  }
}

/*
 * This function makes sure that all chars are either a letter of a space
 * 
 * Input: char* input - the input of the user
 * Output: true if all characters are a space or a letter; otehrwise, false
 */
bool
validChars(char* input)
{
  char* c;
  for (c = input; *c != '\0'; c++) {
    if (!isalpha(*c) && !isspace(*c)) {
      return false;
    }
  }
  return true;
}

/*
 * This function informs the method query whether it should continue or not
 * It essentially lets it know if it is valid
 *
 * Input: the input, array of words, the number of words
 * Output: true if it should continue, otherwise false
 */
bool
parse(char* input, char** words, int* num)
{
  if (num == 0) {
    return false;
  }
  return validOperators(words, num);
}

/*
 * Given an input and array of words, this function normalizes it
 * This means that it gets rid of all extraneous spaces and turns all
 * letters into lower case
 *
 * It also adds the input to the array of words which is given as an empty array
 * In addition, it calculates the number of words
 * 
 * For this function, I attempted a solution without using strtok, but it did not work.
 * After using strtok, the function works as expected without any pain, so I figured it
 * is okay to use it.
 *
 * Input: the input, the array of words, the number of words (to be calculated)
 * Output: none
 */
void
normalize(char* input, char** words, int* num)
{
  char* delim = " \t\n";
  char* word = strtok(input, delim);
  while (word != NULL){
    normalizeWord(word);
    words[*num] = word;
    word = strtok(NULL, delim);
    *num += 1;
  }
  
  /*int count = 0;
  for (char* i = input; *i != '\0'; i++) {
    if (isalpha(*i)) {
      normalizeWord(i);
      words[count] = i;
      count += 1;
      *num = count;
    }

    while(isalpha(*(i+1))) { i++; }

    *(i+1) = '\0';
    i++;
    }*/
  
}

/*
 * This function checks if the array of words correctly uses operators
 * Explained in further detail below
 *
 * Input: the array or words and the number of words
 * Output: true if the operators are valid; otherwise, false 
 */
bool
validOperators(char** words, int* num)
{
  if(isOperator(words[0])){
    fprintf(stderr, "Initial word cannot be 'and' or 'or'\n");
    printf("Query:"); // print out query back to stdout
    for (int i = 0; i < *num; i++) {
      printf(" %s", words[i]);
    }
    printf("\n");
    return false;
  }

  if(isOperator(words[*num-1])){
    printf("Query:"); // print out query back to stdout
    for (int i = 0; i < *num; i++) {
      printf(" %s", words[i]);
    }
    fprintf(stderr, "Final word cannot be 'and' or 'or'\n");
    return false;
  }

  for (int i = 0; i < *num - 1; i++) {
    if(isOperator(words[i]) && isOperator(words[i+1])) {
      printf("Query:"); // print out query back to stdout
      for (int i = 0; i < *num; i++) {
        printf(" %s", words[i]);
      }
      fprintf(stderr, "An operator cannot be directly followed by another operator\n");
      return false;
    }
  }
  
  return true;
}

/*
 * This function checks if a word is an operator or not
 *
 * Input: the word
 * Output: true if it is an operator; otherwise, false
 */
bool
isOperator(char* word)
{
  if (strcmp(word, "or") != 0 && strcmp(word, "and") != 0) {
    return false;
  }
  return true;
}

/*
 * counterCreation - given an index, this function attempts to create
 * a counter that houses the correct score for each document
 * 
 * The score is dependent on the words array provided and the number
 * of ands and ors within it
 *
 * Input: the index, array of words, and number of words
 * 
 * Output: the counter
 */
counters_t*
counterCreation(index_t* index, char** words, int num)
{
  bool comp = true;
  counters_t* unions;
  counters_t* one = counters_new();
  counters_t* two = counters_new(); // initialize for warning flags
  countersHolder_t intersections = { one, two };
  countersHolder_t* inters = &intersections;
  counters_delete(inters->two); // immediately delete for memory leaks
  counters_t* temp = index_find(index, words[0]);
  counters_iterate(temp, inters->one, duplicate);

  for (int i = 1; i<num; i++) { // loop through all words
    if (strcmp(words[i], "and") != 0) {
      if (strcmp(words[i], "or") != 0) {
        inters->two = index_find(index, words[i]);
        counters_iterate(inters->one, inters, intersection); // find the minimum of the two
      }
      else {
        if(comp) {
          unions = inters->one;
          comp = false;
        }
        else {
          counters_iterate(inters->one, unions, unionSum); // find the sum of the two                                                                                                                       
          counters_delete(inters->one); // delete the temp                                                                                                                                                  
        }
        inters->one = counters_new();
        temp = index_find(index, words[i+1]);
        counters_iterate(temp, inters->one, duplicate); // only find docs where score > 0  
      }
    }
  }
  if (comp) {
    unions = inters->one;
  }
  else {
    counters_iterate(inters->one, unions, unionSum);
    counters_delete(inters->one);
  }
  
  temp = counters_new();
  counters_iterate(unions, temp, duplicate); // answer should only contain those docs which have a score > 0
  counters_delete(unions);
  return temp; // return the answer
}

/*
 * intersectMin - a helper function for a counter iterator that
 * finds the minimum between two given counters and sets the arg to that
 *
 * Input: the standard arguments for an iterate fucntion
 * Ouptut: none
 */
void
intersection(void* arg, const int key, int count)
{
  countersHolder_t* combined = arg;
  int min = counters_get(combined->two, key);
  if (min < count) {
    counters_set(combined->one, key, min);
  }
}

/*
 * duplicate - a helper function for a counter iterator that
 * duplicates as long as count is above 0
 *
 * Input: the standard inputs for an iterate function
 * Output: none
 */
void
duplicate(void* arg, const int key, int count)
{
  counters_t* dup = arg;
  if (count > 0) {
    counters_set(dup, key, count);
  }
}

/*
 * unionSum - helper method for a counter iterator
 *
 * Sums up the counts! This works for instances of "or"
 *
 * Input: the standard inputs for a iterator function
 * Output: none
 */

void
unionSum(void* arg, const int key, int count)
{
  counters_t* holder = arg;
  int sum = count + counters_get(holder, key);
  counters_set(holder, key, sum);
}

/*
 * score - a method that, when provided a counter, creates an unsorted
 * array of scores
 *
 * Input: the counter, the number of documents for calloc, and the number
 * of files to iterate through
 *
 * Output: the array of scores
 */
score_t**
score (counters_t* result, int numOfDocs, int numOfFiles) {
  int count = 0;
  score_t** output = calloc(numOfDocs, sizeof(score_t));
  
  for (int i = 1; i <= numOfFiles; i++) {
    int score = counters_get(result, i);
    if (score > 0) {
      score_t* hold = malloc(sizeof(score_t));
      hold->id = i;
      hold->score = score;
      output[count] = hold;
      count++;
    }
  }
  return output;
}

/*
 * sort - a function that sorts an array of scores based on who has the highest
 * score
 *
 * Input: the array (output) and the number of documents to iterater through
 * Output: the sorted array
 */
score_t**
sort (score_t** output, int numOfDocs) {
  for (int i = 0; i < (numOfDocs-1); i++) {
    for (int j = i+1; j < numOfDocs; j++) {
      if (output[i]->score < output[j]->score) {
        score_t* hold = output[i];
        output[i] = output[j];
        output[j] = hold;
      }
    }
  }
  return output;
}

/*
 * counter - a helper method for a counter iterator
 * counts the number of docs in a given counter that has a count
 * about 0
 *
 * Input: the standard inputs for a iterate function
 * Output: none
 */
void
counter (void* arg, const int key, int count) {
  int* numOfDocs = arg;
  if (count > 0) {
    (*numOfDocs)++;
  }
}

/*
 * numFiles - counts the number of files in a pageDirectory in the format of
 * pageDir/id
 *
 * Input: the pageDir name
 * Output: the number of files
 */
int
numFiles (char* pageDir) {
  int count = 1;
  FILE* fp;
  char filename[900];
  sprintf(filename, "%s/%d", pageDir, count);
  while ((fp = fopen(filename, "r")) != NULL) {
    fclose(fp);
    count++;
    sprintf(filename, "%s/%d", pageDir, count);
  }
  int ans = count -1;
  return ans;
}
