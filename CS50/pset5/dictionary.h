/****************************************************************************
 * dictionary.h
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Declares a dictionary's functionality.
 ***************************************************************************/

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include "hashset.h"

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word);

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictName);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/*
 * @param positions - array to store data in
 * @param string - the string to hash
 *
 * @return int - the hash value
 *
 * Hashes a given word in lowercase form and stores it in an array.
 * fnv1a hash - http://www.isthe.com/chongo/tech/comp/fnv/
 */
int fnv1a_hash_lower(const char* string, int positions[2]);

/*
 * @param c - the char
 *
 * @return char - the lowercase char
 *
 * Changes a single character to lowercase if it is uppercase.
 */
char toLower(char c);

#endif // DICTIONARY_H
