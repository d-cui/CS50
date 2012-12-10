#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

#include "linkedlist.h"

/*
 * used to build a node!
 */  
struct word_node* build_node(int string)
{
    // malloc a new led_node defined as "new_node" that is the size of a word_node
    struct word_node* new_node = malloc(sizeof(struct word_node));

    // clear that memory
    memset(new_node, 0, sizeof(struct word_node));
    
    if (new_node == NULL)
    {
        printf("sll: error: Out of heap memory\n");
    }
    
    // make space for the letters
    // new_node->value = malloc(sizeof(int));
    
    // zero that memory
    // memset(new_node->value, 0, sizeof(int));
    
    // copy the string into letters
    new_node->value = string;
    
    new_node->next = NULL;
    return new_node;
}

/*
 * used to build a node and place it at the beginning of the list
 */  
struct word_node* prepend(word_node* first, int string)
{
    word_node* new_node = build_node(string);

    // the first node of our list will be the second node of our new one
    new_node->next = first;
    
    // our new node is now going to be the first node
    return new_node;
}

/*
 * used to build a node and insert it after a node that exists already
 */  
void insert_after(word_node* n, int string)
{
    if (n != NULL)
    {
        word_node* new_node = build_node(string);
        
        // point the new node's next member to n's next node
        word_node* next_node = n->next;
        new_node->next = next_node;
       
        // point the current node's next member to the new node
        n->next = new_node;
    }
}

/*
 * used to append to the last part of the list
 */
void append(word_node* first, int string)
{
    // track the previous node to the current one as well, since it's
    // otherwise impossible to insert *before* the current node
    word_node* prev = NULL;
    
    // TODO!
    // for each led_node; and as long as the led node is not null, the next increament should be equal to the next node :)
    // i.e. not all for-loops HAVE to be ints :)
    for (word_node* cur = first; cur != NULL; cur = cur->next)
    {
        // make sure to update the prev pointer on every iteration!
        prev = cur;
    }

    // if prev is NULL, that means that the element belongs first
    if (prev == NULL)
    {
        prepend(first, string);
    }
    else
    {
        // aren't you glad that you already wrote this one? ;-)
        insert_after(prev, string);   
    }   
}

/*
 * used to check if a matching data structure pin is there - if so - return the address
 */
bool contains(int* first, int string)
{    
    // loop through the nodes
    while(*first)
    {
        // if the node's pin is equal to "pin" - what should you return?
        if(*first == string)
          return true;
        
        first++;
    }
    return false;    
}

int strcmpi(const char* s1, const char* s2)
{
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    
    if(l1 != l2)
        return 1;
        
    for(int i = 0; i < l1; i++)
    {
        if(s1[i] != s2[i] && s1[i] != s2[i] + 32)
            return 1;
    }
    
    return 0;
}

/*
 * used to check the length of the linked list
 */
int length(word_node* first)
{
    int length = 0;
    
    // TODO!
    // for each led_node - starting with first; and as long as the led node is not null, the next increament should be equal to the next node :)
    // i.e. not all for-loops HAVE to be ints :)
    for (word_node* cur = first; cur != NULL; cur = cur->next)
    {
        // TODO!
        length++;
        // what should be incremented here?
    }
    return length;
}
