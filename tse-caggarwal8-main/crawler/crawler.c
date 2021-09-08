/*
 * crawler.c - a program that works as per the specs provided.
 *
 * Crawls a given URL until a certain depth and adds pages and htmls
 * to a given pageDirectory.
 *
 * Part of the larger Tiny Search Engine project.
 *
 * Usage: ./crawler seedURL pageDirectory maxDepth
 * where seedURL is an internal URL, pageDirectory is writable,
 * and maxDepth is between 0 and 10.
 * 
 * CJ Aggarwal, April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "bag.h"
#include "hashtable.h"
#include "webpage.h"
#include "../common/pagedir.h"

/* initializing helper functions to be called later one */
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
void hashFunctionDelete(void* item);

/*
 * main - simply calls the helper functions
 *
 * inputs: command line arguments - see below
 * seedURL pageDirectory maxDepth
 * where seedURL is an internal URL, pageDirectory is writable,
 * and maxDepth is between 0 and 10.
 *
 * output: an exit code - 0 is no errors; non-zero if an error occurs
 */
int main(const int argc, char* argv[])
{
  parseArgs(argc, argv, &argv[1], &argv[2], (int*)(argv[3]));
  crawl(normalizeURL(argv[1]), argv[2], atoi(argv[3]));
  return 0;
}

/*
 * Parses arguments to ensure that they are valid
 * as per specifications 
 *
 * Inputs: command line arguments
 *
 * No outputs
 */
void
parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth)

{
  if (argc != 4) { // checks if the number of arguments is correct
    fprintf(stderr, "Not the right number of arguments\nUsage: seedURL, pageDirectory, maxDepth\n");
    exit(1);
  }

  char* normalized = normalizeURL(argv[1]);
  if (!isInternalURL(normalized)) { // check if link is internal - uses a given method
    fprintf(stderr, "Normalized seedURL is not internal!\n");
    free(normalized); // no memory leaks
    exit(1);
  }
  free(normalized); //no memory leaks b/c normalizeURL creates new memory

  if (!pagedir_init(argv[2])) { // checks if pageDirectory is writeable
    fprintf(stderr, "Page directory is not valid\n");
    exit(1);
  }
  
  if (atoi(argv[3]) < 0 || atoi(argv[3]) > 10) { //checks if maxDepth is within range
    fprintf(stderr, "Max Depth must be between 0 and 10\n");
    exit(1);
  }
}

/*
 * crawl - this method does the brunt of the work in this module
 *
 * it is written according to the pseudocode provided
 *
 * fetches, saves, and scans pages
 * utilizes the bag, hashtable, and webpage modules
 *
 * the hashtable tracks the checked pages to find duplicates
 * the bag tracks pages which are next
 * utilizes the other methods in this module
 *
 * inputs: seedURL, pageDirectory, and maxDepth
 * all have been described above
 *
 * no output
 */
void
crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
  hashtable_t* checked;
  checked = hashtable_new(100);
  hashtable_insert(checked, seedURL, ""); // "" as per the information provided by Prof. Kotz
  
  bag_t* webpages;
  webpages = bag_new();

  webpage_t* seed;
  seed = webpage_new(seedURL, 0, NULL);
  
  
  bag_insert(webpages, seed); // start exploring

  webpage_t* curr;

  int id = 1; // keep track of each unique id
  
  while((curr = bag_extract(webpages)) != NULL) { //keep exploring while there is room
    if (webpage_fetch(curr)) {
      fprintf(stdout, "%2d\tFetched: %s\n", webpage_getDepth(curr), webpage_getURL(curr));
      // log progress to stdout
      pagedir_save(curr, pageDirectory, id); // use pagedir.c
      id++; // increment unique ids
      if (webpage_getDepth(curr) < maxDepth) { // make sure we don't go path the max depth
        fprintf(stdout, "%2d\tScanning: %s\n", webpage_getDepth(curr), webpage_getURL(curr));
        // log progress to stdout
        pageScan(curr, webpages, checked); // call another method
      }
    }
    webpage_delete(curr);
    // make sure of no memory leaks
  }
  bag_delete(webpages, webpage_delete);
  hashtable_delete(checked, hashFunctionDelete);
}

/*
 * pageScan - scans all pages past through this method
 * add seen pages to the aforementioned hashtable and
 * pages to explore to the bag
 *
 * written as per the provided pseudocode
 *
 * inputs: webpage page, bag, and hashtable
 * 
 * no outputs
 */
void
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
  char* nextUrl;
  int pos = 0;
  
  while((nextUrl = webpage_getNextURL(page, &pos)) != NULL) {
    if (isInternalURL(nextUrl)) { // ensure that the URL is valid
      if(hashtable_insert(pagesSeen, nextUrl, "")) { // no duplicates
        webpage_t* seen = webpage_new(nextUrl, webpage_getDepth(page)+1, NULL);
        fprintf(stdout, "%2d\tFound: %s\n", webpage_getDepth(page), webpage_getURL(seen));

        bag_insert(pagesToCrawl, seen);
        // add pagesToCrawl to the bag so we can explore in the future
        fprintf(stdout, "%2d\tAdded: %s\n", webpage_getDepth(page), webpage_getURL(seen));
        //log progress to stdout
      }
      
      else { // then it is a duplicate
        // log progress to stdout
        fprintf(stdout, "%2d\tFound: %s\n", webpage_getDepth(page), nextUrl);
        fprintf(stdout, "%2d\tIgnDupl: %s\n", webpage_getDepth(page), nextUrl);
        free(nextUrl);
        // free so no memory leaks
      }
    }
    else { // then it is an external link
      fprintf(stdout, "%2d\tFound: %s\n", webpage_getDepth(page), nextUrl);
      fprintf(stdout, "%2d\tIgnExtrn: %s\n", webpage_getDepth(page), nextUrl);
      free(nextUrl);
    }
  }
}

/*
 * An empty method written to satisfy the necessary parameters of the 
 * hashtable_delete method
 */
void
hashFunctionDelete(void* item)
{
  ; // does not do anything
}
