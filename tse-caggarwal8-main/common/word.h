/*
 * word.h - this module contains a single function - normalizeWord
 * Its purporse is to convert all letters in a given word to lower case
 * This allows indexer to properly do its job and be able to compare across
 * cases.
 *
 * CJ Aggarwal, April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
