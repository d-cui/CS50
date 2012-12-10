/****************************************************************************
 * hashset.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a hashset's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <cs50.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hashset.h"

/*
 * @param dict - the dictionary to add to
 * @param string - the word to add to the dictionary
 * 
 * @return true - the word was added to the dictionary
 *
 * Adds a word to the dictionary and returns true.    
 */
bool add(struct dictionary* dict, const char* string)
{
    // array for holding hash information
    int positions[2];
    
    // hash the word
    fnv1a_hash(string, positions);
    
    // get the pointer to the array at positions[0]
    int* first = dict->words[positions[0]];
    
    // if it exists
    if(first)
    {
        // set the new value at the end of the array          
        *(first+dict->lengths[positions[0]]) = positions[1];
        
        // increment the number of elements in the array
        (dict->lengths[positions[0]])++;
    }
    else
    {
        // create a new array of "large enough" size (magic number 13 :))
        dict->words[positions[0]] = malloc(15*sizeof(int));
        
        // make sure all of the memory is zeroed
        memset(dict->words[positions[0]], 0, 15*sizeof(int));
        
        // set the first element of the array equal to the string value
        *(dict->words[positions[0]]) = positions[1];
        
        // initialize the array length
        dict->lengths[positions[0]] = 1;
    }
    
    // yay we added a new word!
    return true;
}

/*
 * @param positions - array to store data in
 * @param string - the string to hash
 *
 * @return int - the hash value
 *
 * Hashes a given word and stores it in an array.
 * fnv1a hash - http://www.isthe.com/chongo/tech/comp/fnv/
 */
int fnv1a_hash(const char* string, int positions[2])
{
    // the prime and offset
    int Prime = 0x01000193; //   16777619
    uint32_t hash  = 0x811C9DC5; // 2166136261
    
    // while there are more characters
    // hash the function
    while(*string != '\n')
        hash = (*string++ ^ hash) * Prime;
    
    // store the array position
    positions[0] = hash % SIZE;
    
    // store the value
    positions[1] = hash;
    return hash;
}

/*
 * @param first - the array to search
 * @param string - the value to look for
 * 
 * @return true - if the word was found
 *
 * Looks for the value within the array.
 */
bool contains(int* first, int string)
{    
    // while we're still in the array
    while(*first)
    {
        // check if the value is equal to string
        if(*first == string)
          return true;
        
        // increment
        first++;
    }
    
    // no we didn't find it :(
    return false;    
}
