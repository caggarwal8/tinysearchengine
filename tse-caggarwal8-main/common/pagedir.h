/*
 * pagedir.h - header file for pagedir.c
 *
 * pagedir.c helps crawler by creating writeable
 * directories with correct names and verifying that
 * given directories are valid
 *
 * these files are written as per specifications
 *
 * CJ Aggarwal, April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "webpage.h"

/* pagedir_init
 *
 * Constructs the pathname for .crawler and opens that file for writing
 *
 * Input: the pageDirectory name
 *
 * Output: true if succesful; false if not
 */
bool pagedir_init(const char* pageDirectory);

/* pagedir_save
 *
 * Constructs the pathname for the page file in the directory
 * prints the URL, depth, and HTML
 *
 * Input:
 * webpage_t* page - pointer to the webpage
 * char* pageDirectory - pointer to the pageDirectory name
 * docID - id of the page
 *
 * Ouput: none
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

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
