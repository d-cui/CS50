/****************************************************************************
 * hashset.h
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Declares a hashset's functionality.
 ***************************************************************************/

#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>

// maximum number of buckets
#define SIZE 50373

// this is the hashset
typedef struct dictionary
{
    int size;
    int* words[SIZE];
    int lengths[SIZE];
} 
dictionary;

/*
 * @param dict - the dictionary to add to
 * @param string - the word to add to the dictionary
 * 
 * @return true - the word was added to the dictionary
 *
 * Adds a word to the dictionary and returns true.    
 */
bool add(dictionary* dict, const char* string);

/*
 * @param positions - array to store data in
 * @param string - the string to hash
 *
 * @return int - the hash value
 *
 * Hashes a given word and stores it in an array.
 * fnv1a hash - http://www.isthe.com/chongo/tech/comp/fnv/
 */
int fnv1a_hash(const char* string, int positions[2]);

/*
 * @param first - the array to search
 * @param string - the value to look for
 * 
 * @return true - if the word was found
 *
 * Looks for the value within the array.
 */
bool contains(int* first, int string);

#endif // HASHSET_H
