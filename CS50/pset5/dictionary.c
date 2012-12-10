/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>

#include "dictionary.h"
#include "hashset.h"

// this is the dictionary
struct dictionary* dict;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // allocate space to store the hash info
    int positions[2];
    
    // hash the word making sure every letter
    // is lowercase
    fnv1a_hash_lower(word, positions);
    
    // get the pointer to the array at positions[0]
    int* first = dict->words[positions[0]];
    
    // if there is no array... we're done
    if(!first)
        return false;
    
    // rcheck if the word is in the array
    return contains(first, *(positions + 1));
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictName)
{
    struct stat sb;
    // Open the dictionary file
    int fp = open(dictName, O_RDONLY);
    
    fstat(fp, &sb);
    char* file = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fp, 0);

    // initialize the dictionary
    dict = malloc(sizeof(dictionary));
    
    // zero the memory
    memset(dict, 0, sizeof(dictionary));
   
    // store the total number of words
    int numWords = 0;
    
    // add the first word
    add(dict, file);
    
    // loop through the file space
    for(int i = 0; i < sb.st_size - 1; i++)
    {
        // if at a newline char, start of new word
        if(*file == '\n')
        {
            file++;
            
            // add the new word to the dictionary
            add(dict, file);
            
            // increment the number of words
            numWords++;
            continue;
        }
        
        // move ahead in the file    
        file++;
    }

    // store the dictionary size
    dict->size = numWords;
    
    // close the file stream
    close(fp);
    
    // free the memory map
    munmap(file, sb.st_size);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // return the dictionary size
    return dict->size;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for(int i = 0; i < SIZE; i++)
    {
        free(dict->words[i]);
    }
    free(dict);
    return true;
}

/*
 * @param c - the char
 *
 * @return char - the lowercase char
 *
 * Changes a single character to lowercase if it is uppercase.
 */
char toLower(char c)
{
    if(c >= 65 && c <= 90)
        return c+ 32;
    
    return c;
}

/*
 * @param positions - array to store data in
 * @param string - the string to hash
 *
 * @return int - the hash value
 *
 * Hashes a given word in lowercase form and stores it in an array.
 * fnv1a hash - http://www.isthe.com/chongo/tech/comp/fnv/
 */
int fnv1a_hash_lower(const char* string, int positions[2])
{
    // prime and offset
    int Prime = 0x01000193; //   16777619
    uint32_t hash  = 0x811C9DC5; // 2166136261
    
    // xor the hash with the new bits and multiply by the prime
    while(*string)
        hash = (toLower(*string++) ^ hash) * Prime;
    
    // store the hash data
    positions[0] = hash % SIZE;
    positions[1] = hash;
    return hash;
}
