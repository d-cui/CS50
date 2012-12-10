/****************************************************************************
 * huff.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Compress a file using the Huffman algorithm.
 ***************************************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forest.h"
#include "huffile.h"
#include "tree.h"

// Max number of chars
#define ASCII_MAX 256

// encoding of a character
typedef struct
{
    char info[ASCII_MAX];
}
encodedChar;

// prototypes
void generateCharSequences(encodedChar* infoArray, char* sequence, Tree* tree);
Tree* mergeTrees(Forest* myForest);
void processFrequencies(int* frequencies, Forest* myForest);

// Let's huff this file!
int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: %s input. ./speller <input> <output>\n", argv[0]);
        return 1;
    }

    // open input
    FILE* input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open %s for reading.\n", argv[1]);
        return 1;
    }
    
    // open output
    Huffile* output = hfopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open %s for writing.\n", argv[2]);
        return 1;
    }
    
    // Array of frequencies
    int* frequencies = malloc(sizeof(int) * ASCII_MAX);
    memset(frequencies, 0, sizeof(int) * ASCII_MAX);
    
    // Array of encodings
    encodedChar* information = malloc(sizeof(encodedChar) * ASCII_MAX);
    memset(information, 0, sizeof(encodedChar) * ASCII_MAX); 
    
    int c;
    
    // checksum for the file   
    int sum = 0;
    
    // Read the entire file
    while((c = fgetc(input)) && !feof(input))
    {
        // increment the frequencies
        frequencies[c]++;
        
        // increment the checksum
        sum++;
    }
    
    // seek back to the beginning    
    fseek(input, 0, SEEK_SET);
    
    // Initialize the header
    Huffeader header;
    header.checksum = sum;
    
    // set the frequencies
    for(int i = 0; i < ASCII_MAX; i++)
        header.frequencies[i] = frequencies[i];
        
    header.magic = MAGIC;
    
    // write the header to the outfile
    hwrite(&header, output);
    
    // create a forest
    Forest* myForest = mkforest();
    
    // Create nodes for each character
    processFrequencies(frequencies, myForest);
    
    // merge the nodes into one tree
    Tree* first = mergeTrees(myForest);
    
    // we don't need the forest anymore
    rmforest(myForest);
    
    // if the file is empty
    if(first == NULL)
    {
        printf("Error: nothing to write");
        return 1;
    }
    
    // there's only one node
    if(first->left == NULL && first->right == NULL)
    {
        // write 0
        bwrite(0, output);
        
        // unload data
        rmtree(first);
        free(frequencies);
        free(information);
        hfclose(output);
        fclose(input);
        
        return 0;
    }    
    
    // generate encodings for each character
    generateCharSequences(information, "", first);
    
    // we don't need the tree anymore
    rmtree(first);
    
    // go through file again
    while((c = fgetc(input)) && !feof(input))
    {
        // grab the encoding for the character
        encodedChar sequence = information[c];
        
        // get the array of bits
        char* bits = &(sequence.info[0]);
        
        // read until the end
        while(*bits != '\0')
        {
            // write the bits
            bwrite(*bits - '0', output);
            bits++;
        }
    } 
    
    // done with all of this memory
    free(frequencies);
    free(information);
    hfclose(output);
    fclose(input);

    // that's all folks!
    return 0;
}

/*
 * @param infoArray - the array to store encodings
 * @param sequence - the current encoding sequence
 * @param tree - the node to search from
 *
 * Takes a root node and searches each child node, generating a 
 * sequence of 0's and 1's corresponding to a left or right move.
 * Stores the sequence when it arrives at a leaf.
 */
void generateCharSequences(encodedChar* infoArray, char* sequence, Tree* tree)
{   
    // if it's a leaf 
    if(tree->left == NULL && tree->right == NULL)
    {   
        // get the character value
        int thisChar = (unsigned char)tree->symbol;
        
        // put the sequence into the encoding array
        strcpy(infoArray[thisChar].info, sequence);
        return;
    }
    
    // this is really bad
    // basically creates a new sequence of length 256
    // to store the new sequence in
    int length = strlen(sequence);
    char newSequence[ASCII_MAX];
    strncpy(newSequence, sequence, length);
    // end of really bad
    
    // check the children of this node    
    if(tree->left == NULL && tree->right != NULL)
    {   
        // we're moving to the right... so add 1
        newSequence[length] = '1';
        newSequence[length + 1] = '\0';
        
        // recursively go through the rest of the tree
        generateCharSequences(infoArray, newSequence, tree->right);
    }
    else if(tree->left != NULL && tree->right == NULL)
    {
        // we're shifting to the left... so add 0
        newSequence[length] = '0';
        newSequence[length + 1] = '\0';
        
        // recursively go through the tree
        generateCharSequences(infoArray, newSequence, tree->left);
    }
    else
    {
        // we need to both go to the right and left
        
        // first do right
        newSequence[length] = '1';
        newSequence[length + 1] = '\0';
        
        // recursively go through the right side of the tree
        generateCharSequences(infoArray, newSequence, tree->right);
        
        // then do left
        newSequence[length] = '0';
        
        // recursively go through the left side of the tree
        generateCharSequences(infoArray, newSequence, tree->left);
    }   
}

/*
 * @param frequencies - the array of frequencies
 * @param myForest - forest to put trees in
 *
 * Takes a list of frequencies and adds trees to the forest for each
 * frequency not equal to 0.
 */
void processFrequencies(int* frequencies, Forest* myForest)
{
    int numTrees = 0;
    // loop through the character spectrum
    for(int i = 0; i < ASCII_MAX; i++)
    {
        // if it doesn't appear, don't make a tree
        if(*(frequencies + i) == 0)
            continue;
        
        numTrees++;
        // generate a new tree for the character
        Tree* myTree = mktree();
        myTree->symbol = i;
        myTree->frequency = frequencies[i];
        myTree->left = NULL;
        myTree->right = NULL;
        
        // plant the tree
        plant(myForest, myTree);
    }
}

/*
 * @param myForest - the forest to merge
 *
 * @return left - the root tree
 *
 * Takes a forest and merges it into a single tree of tree
 * nodes. Returns the root to the tree.
 */
Tree* mergeTrees(Forest* myForest)
{
    // Pick the left and right at the beginning
    Tree* left = pick(myForest);
    Tree* right = pick(myForest);
    
    // While there's more than one tree
    while(right != NULL)
    {   
        // make a parent tree
        Tree* parent = mktree();
        parent->frequency = left->frequency + right->frequency;
        parent->left = left;
        parent->right = right;
        
        // plant the parent tree
        plant(myForest, parent);
        
        // pick two more tree
        left = pick(myForest);
        right = pick(myForest);
    }
    
    // return the root tree
    return left;
}
