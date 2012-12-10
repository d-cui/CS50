/***************************************************************************
 * scramble.c
 *
 * Author: Dennis Cui
 * TF: Christopher Bartholomew
 *  
 *
 * Problem Set 3
 *
 * Implements Scramble with CS50.
 *
 * Usage: scramble [#]
 *
 * where # is an optional grid number.
 ***************************************************************************/
 
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// duration of a game in seconds
#define DURATION 30

// grid's dimensions
#define DIMENSION 4

// maximum number of words in any dictionary
#define WORDS 172806

// maximum number of letters in any word
#define LETTERS 29

// for logging
FILE* log;

// grid
char grid[DIMENSION][DIMENSION];

// flags with which we can mark grid's letters while searching for words
bool marks[DIMENSION][DIMENSION];

// defines a word as having an array of letters plus a flag
// indicating whether word has been found on grid
typedef struct
{
    bool found;
    char letters[LETTERS+1];
}
word;

// defines a dictionary as having a size and an array of words
struct
{
    int size;
    word words[WORDS];
}
dictionary;

// prototypes
void clear(void);
bool crawl(string word, int x, int y);
void draw(void);
bool find(string word);
void initialize(void);
bool load(string filename);
bool lookup(string word);
void scramble(void);
void inspiration(void);
int scoreFromString(string word);
bool isWordAtIndexAndFlag(string word, int index);

// This is Scramble.
int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc > 2)
    {
        printf("Usage: scramble [#]\n");
        return 1;
    }

    // seed pseudorandom number generator
    if (argc == 2)
    {
        int seed = atoi(argv[1]);
        if (seed <= 0)
        {
            printf("Invalid grid.\n");
            return 1;
        }
        srand(seed);
    }
    else
        srand(time(NULL));
        
    // load dictionary
    // http://www.becomeawordgameexpert.com/wordlists.htm
    if (!load("./words"))
    {
        printf("Could not open dictionary.\n");
        return 1;
    }
        
    // open log
    log = fopen("log.txt", "a");
    if (log == NULL)
        return 1;
    
    // Start the game    
    scramble();

    return 0;
}

/**
 * Clears screen.
 */
void clear()
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Crawls grid recursively for letters starting at grid[x][y].
 * Returns true iff all letters are found.
 */
bool crawl(string letters, int x, int y)
{
    // if out of letters, then we must've found them all!
    if (strlen(letters) == 0)
        return true;

    // don't fall off the grid!
    if (x < 0 || x >= DIMENSION)
        return false;
    if (y < 0 || y >= DIMENSION)
        return false;

    // been here before!
    if (marks[x][y] == true)
        return false;

    // check grid[x][y] for current letter
    if (grid[x][y] != letters[0])
        return false;

    // mark location
    marks[x][y] = true;

    // look left and right for next letter
    for (int i = -1; i <= 1; i++)
    {
        // look down and up for next letter
        for (int j = -1; j <= 1; j++)
        {
            // check grid[x + i][y + j] for next letter
            if (crawl(&letters[1], x + i, y + j) == true)
                return true;
        }
    }

    // unmark location
    marks[x][y] = false;

    // fail
    return false;
}

/** 
 * Prints the grid in its current state.
 */
void draw(void)
{
    for(int row = 0; row < DIMENSION; row++)
    {
        for(int col = 0; col < DIMENSION; col++)
            // Print the (row, col)th character
            printf("%c ", grid[row][col]);
            
        // Move to the next line
        printf("\n");
    }
}

/**
 * Returns true iff word is found in grid.
 */
bool find(string word)
{
    // word must be at least 2 characters in length
    if (strlen(word) < 2)
        return false;

    // search grid for word
    for (int row = 0; row < DIMENSION; row++)
    {
        for (int col = 0; col < DIMENSION; col++)
        {
            // reset marks
            for (int i = 0; i < DIMENSION; i++)
                for (int j = 0; j < DIMENSION; j++)
                    marks[i][j] = false;

            // search for word starting at grid[i][j]
            if (crawl(word, row, col) == true)
                return true;
        }
    }
    return false;
}

/**
 * Initializes grid with letters.
 */
void initialize(void)
{
    // http://en.wikipedia.org/wiki/Letter_frequency
    // basically constants
    float frequencies[] = {
     8.167,  // a
     1.492,  // b
     2.782,  // c
     4.253,  // d
     12.702, // e
     2.228,  // f
     2.015,  // g
     6.094,  // h
     6.966,  // i
     0.153,  // j
     0.747,  // k
     4.025,  // l
     2.406,  // m
     6.749,  // n
     7.507,  // o
     1.929,  // p
     0.095,  // q
     5.987,  // r
     6.327,  // s
     9.056,  // t
     2.758,  // u
     1.037,  // v
     2.365,  // w
     0.150,  // x
     1.974,  // y
     0.074   // z
    };
    
    // the length of the frequency array
    int n = sizeof(frequencies) / sizeof(float);

    // iterate over grid
    for (int row = 0; row < DIMENSION; row++)
    {
        for (int col = 0; col < DIMENSION; col++)
        {   
            // generate pseudorandom double in [0, 1]
            double d = rand() / (double) RAND_MAX;

            // map d onto range of frequencies
            for (int k = 0; k < n; k++)
            {
                d -= frequencies[k] / 100;
                if (d < 0.0 || k == n - 1)
                {
                    grid[row][col] = 'A' + k;
                    break;
                }
            }
        }
    }
}

/**
 * Loads words from dictionary with given filename into a global array.
 */
bool load(string filename)
{
    // open dictionary
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        return false;

    // initialize dictionary's size
    dictionary.size = 0;

    // load words from dictionary
    char buffer[LETTERS + 2];
    while (fgets(buffer, LETTERS + 2, file))
    {
        // overwrite \n with \0
        buffer[strlen(buffer) - 1] = '\0';

        // capitalize word
        for (int i = 0, n = strlen(buffer); i < n; i++)
            buffer[i] = toupper(buffer[i]);

        // ignore INSPIRATION
        if (strcmp(buffer, "INSPIRATION") == 0) 
            continue;

        // copy word into dictionary
        dictionary.words[dictionary.size].found = false;
        strncpy(dictionary.words[dictionary.size].letters, buffer, LETTERS + 1);
        dictionary.size++;
    }

    // success!
    return true;
}

/**
 * Looks up word in dictionary.  Iff found (for the first time), flags word
 * as found (so that user can't score with it again) and returns true.
 */
bool lookup(string word)
{   
    // Lower bound of search interval
    int minIndex = 0;
    
    // Upper bound of search interval
    int maxIndex = dictionary.size - 1;
    
    // Go to the middle of the interval
    int currentIndex = (minIndex + maxIndex)/2;
    
    while(maxIndex >= minIndex)
    {
        // If the word is found for the first time, set the flag and return true
        if(isWordAtIndexAndFlag(word, currentIndex))
            return true;
        
        // Change the bounds depending on the alphabetical position of word    
        if(strcmp(word, dictionary.words[currentIndex].letters) > 0) 
            minIndex = currentIndex + 1;
        else
            maxIndex = currentIndex - 1;
        
        // Go to the middle of the new bound
        currentIndex = (minIndex + maxIndex)/2;
    }
    
    //Your word wasn't found :(
    return false;
}

/**
 * @param word  - the word to compare
 * @param index - the index to check
 *
 * @return true - if the word is at the index
 * @return false - if the word is not at the index
 *
 * Checks if the given word appears at the index in the dictionary.
 */
bool isWordAtIndexAndFlag(string word, int index)
{
    // Check if the word is at the index and has not been checked before
    if(strcmp(word, dictionary.words[index].letters) == 0 
            && dictionary.words[index].found == false)
    {
        // Flag it as found
        dictionary.words[index].found = true;
        return true;
    }
    
    return false;
}

/**
 * Gives the player up to three inspirational words of length 3, 4, and 5 
 * respectively if they exist.
 */
void inspiration(void)
{
    // We only want to inspire with one word of each length
    bool foundThree = false;
    bool foundFour = false;
    bool foundFive = false;
    
    // Tell the user that these are inspiring words
    printf("Inspiration: ");
    
    // Loop through the dictionary once
    for(int index = 0; index < dictionary.size; index++)
    {
        // Quit if one of each length word has been found
        if(foundThree && foundFour && foundFive)
            return;
        
        // Get the length of the current word    
        int wordLength = strlen(dictionary.words[index].letters);
        
        // If the word has been found, we don't want it
        // Also check if it's on the grid
        if(dictionary.words[index].found == false && find(dictionary.words[index].letters))
        {
            // Check wordlengths of 3, 4, or 5
            switch(wordLength)
            {
                case 3:
                    if(foundThree == false)
                    {
                        // Print the word
                        printf("%s ", dictionary.words[index].letters);
                        // We don't want any more of these
                        foundThree = true;
                    }
                    break;
                case 4:
                    if(foundFour == false)
                    {
                        // Print the word
                        printf("%s ", dictionary.words[index].letters);
                        // We don't want any more of these
                        foundFour = true;
                    }
                    break;
                case 5:
                    if(foundFive == false)
                    {
                        // Print the word
                        printf("%s ", dictionary.words[index].letters);
                        // We don't want any more of these
                        foundFive = true;
                    }
                    break;
                default:;
            }
        }       
    } 
}

/**
 * Runs the Scramble game.  
 */
void scramble (void)
{
    // initialize the grid
    initialize();

    // initialize user's score
    int score = 0;

    // calculate time of game's end
    int end = time(NULL) + DURATION;
    
    // do you need inspiration yet?
    bool needInspiration = false;
         
    // accept words until timer expires
    while (true)
    {
        
        // clear the screen
        clear();

        // draw the current state of the grid
        draw();

        // log board
        for (int row = 0; row < DIMENSION; row++)
        {
            for (int col = 0; col < DIMENSION; col++)
                fprintf(log, "%c", grid[row][col]);
            fprintf(log, "\n");
        }

        // get current time
        int now = time(NULL);

        // report score
        printf("Score: %d\n", score);
        fprintf(log, "%d\n", score);

        // check for game's end
        if (now >= end)
        {
            printf("\033[31m"); // red
            printf("Time:  %d\n\n", 0);
            printf("\033[39m"); // default
            break;
        }

        // report time remaining
        printf("Time:  %d\n\n", end - now);
      
        //If inspiration is needed
        if(needInspiration)
        {
            //Give some inspiration
            inspiration();
            printf("\n");
            
            //Inspiration already given, so set as false
            needInspiration = false;
        }
        
        // prompt for word
        printf("> ");
        string word = GetString();
        if (word != NULL)
        {
            // log word
            fprintf(log, "%s\n", word);
            
            // Get the length of the word once
            int wordLength = sizeof(word)/sizeof(char);
            
            // Change every character to its uppercase version
            for(int i = 0; i < wordLength; i++)
                word[i] = (char)toupper(word[i]);
                
            //Give hints if word is INSPIRATION
            if(strcmp(word, "INSPIRATION") == 0)
                needInspiration = true;
                
            // or to look for word on grid and in dictionary
            if (find(word) == true && lookup(word) == true) 
                score += scoreFromString(word);
        }
    }

    // close log
    fclose(log);
}

/**
 * @param word - the string to convert into a score
 *
 * @return score - the score of the word
 *
 * Helper method to convert a string to a score based on a  array of
 * values.
 */
int scoreFromString(string word)
{
    //Array of scores from A = scoreArray[0] to Z = scoreArray[25]
    int scoreArray[] = {1, 4, 4, 2, 1, 4, 
                        3, 3, 1, 10, 5, 2, 
                        4, 2, 1, 4, 10, 1, 
                        1, 1, 2, 5, 4, 8, 
                        3, 10};
    // initalize the score to 0                    
    int score = 0;
    
    // Length of passed word
    int length = strlen(word);
    
    // Loop through each character
    for(int index = 0; index < length; index++)
        // Increase the score by the correct value
        // Uppercase letters are ASCII 65 to ASCII 90
        score += scoreArray[word[index] % 65];
    
    // Return final score
    return score;   
}
