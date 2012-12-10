/****************************************************************************
 * linkedlist.h
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Declares a dictionary's functionality.
 ***************************************************************************/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

// data strcuture that is used for Link List
typedef struct word_node
{
    // the word 
	int value;

    // the next word
 	struct word_node* next;
}
word_node;

/**
 * Returns true if word is in dictionary else false.
 */
struct word_node* build_node(int string);

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
struct word_node* prepend(word_node* first, int string);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
void insert_after(word_node* n, int string);

void append(word_node* first, int string);

bool contains(int* first, int string);

int length(word_node* first);

int strcmpi(const char* s1, const char* s2);

#endif // LINKEDLIST_H
