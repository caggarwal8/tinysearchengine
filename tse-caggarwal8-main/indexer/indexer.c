/*
 * indexer.c - a part of the Tiny Search Engine Module
 *
 * Indexer reads webpages from a directory and creates an index
 * of the number of instances of each word greater than or equal to length
 * three for each document.
 *
 * It prints this index to the given writeable file.
 *
 * Please see the specs for more detailed information!
 *
 * CJ Aggarwal, April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "../common/pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"

bool checkArgs(int argc, char* argv[]);
index_t* indexBuild(char* pageDirectory);
void indexPage(index_t* index, webpage_t* webpage, int id);
webpage_t* createWebpage(FILE* fp);

/*
 * main - the driver function of indexer.c
 * Confirms that args are valid, then calls build
 * Also saves index to the provided file to print it
 *
 * Inputs: argc - the number of arguments; argv - the arguments themselves
 * Output: exit code (0 is no error, otherwise non-zero) 
 */
int main(int argc, char* argv[])
{
  if (!checkArgs(argc, argv)){
    fprintf(stderr, "Arguments are invalid\nUsage: ./indexer pageDirectory indexFilename\n");
    exit(1);
  }
  
  index_t* index = indexBuild(argv[1]);
  FILE* fpW = fopen(argv[2], "w");
  index_save(index, fpW);
  index_delete(index);
  fclose(fpW);
  return 0;
}

/*
 * check args - make sure that the arguments given are valid for this module
 *
 * Proper usage: ./indexer pageDirectory indexFilename
 * where pageDirectory is a readable crawler directory and indexFilename is
 * a writeable file
 *
 * Inputs: argc- the number of arguments, and argv[] - the arguments themselves
 * Output: true if arguments are valid; otherwise, false
 */
bool
checkArgs(int argc, char* argv[])
{
  if (argc!=3) {
    return false;
  }

  if (!pagedir_validate(argv[1])) { // check if it is a crawler specific directory
    return false;
  }

  FILE* fp;
  if ((fp = fopen(argv[2], "w")) != NULL) { // must be writeable
    fclose(fp);
  }
  else {
    fprintf(stderr, "Invalid indexFilename - not writeable\n");
    return false;
  }

  return true;
}

/*
 * indexBuild - builds an index from all of the webpages it finds within
 * pageDirectory
 *
 * Input: pageDirectory - the name of the valid crawler directory
 * Ouput: the built index
 */
index_t*
indexBuild(char* pageDirectory)
{
  index_t* index = index_new(400); // between 200 and 900
  int id = 1;
  webpage_t* webpage = NULL;
  FILE* fp;
  char fname[200]; // must be large to ensure that there are no core dumps
  sprintf(fname, "%s/%d", pageDirectory, id);
  
  while ((fp = fopen(fname, "r")) != NULL) { //run through the files
    webpage = createWebpage(fp);
    indexPage(index, webpage, id);
    id++;
    webpage_delete(webpage);
    fclose(fp);
    sprintf(fname, "%s/%d", pageDirectory, id); //next file
  }
  return index;
}

/*
 * indexPage - scans the webpage to add the words to the index
 *
 * Input:
 * index - the index to add to
 * webpage - the webpage to parse
 * id - the document id
 *
 * Output: none
 */
void
indexPage(index_t* index, webpage_t* webpage, int id)
{
  char* nextWord = NULL;
  int pos = 0;
  while ((nextWord = webpage_getNextWord(webpage, &pos)) != NULL) {
    if (strlen(nextWord) >= 3) { // make sure the length is greater than or equal to three
      normalizeWord(nextWord); // make all lower case
      counters_t* counter = index_find(index, nextWord);
      
      if (counter  == NULL) {
        counter = counters_new(); // make a new counter
        counters_add(counter, id);
        index_insert(index, nextWord, counter);
      }
      else {
        counters_add(counter, id);
      }
    }
    free(nextWord); // no memory leaks!
  }
}

/*
 * createWebpage - give na file, creates an instance of a webpage
 *
 * Reads in the url, depth, and html to do so
 *
 * Input: a file
 * Ouptut: the resulting webpage
 */
webpage_t*
createWebpage(FILE* fp)
{
  webpage_t* webpage;
  char* url;
  char* depth;
  int intDepth;
  char* html;

  url = file_readLine(fp);

  depth = file_readLine(fp); // read in as a char*
  intDepth = atoi(depth); // convert to an int
  free(depth); // no memory leaks!

  html = file_readFile(fp);

  webpage = webpage_new(url, intDepth, html);
  return webpage;
}
