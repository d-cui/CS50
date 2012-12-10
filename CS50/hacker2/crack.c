/*
Author: Dennis Z. Cui
Course: CS50
Section Leader: Christopher Bartholomew
PSet: 2, Hacker Edition

This file takes a ciphertext produced by crypt() and attempts to decrypt the ciphertext by
comparing the ciphetext to the ciphertexts of other encrypted words. First, crack runs a dictionary
attack. If the password is not found in the dictionary attack, crack moves on to a brute force 
attack which exhausts all of the possible plaintexts in the character spectrum.
*/

#include <stdio.h>
#include <crypt.h>
#include <unistd.h>
#include <string.h>
#include <cs50.h>
#include <stdlib.h>

// For crypt
#define _XOPEN_SOURCE

// Number of ASCII chars from ASCII 32 to ASCII 126
#define NUM_ASCII_CHARS 95

#define FALSE 0
#define TRUE 1

// A boolean is an integer. FALSE = 0, TRUE = 1.
typedef int boolean;

// Prototype functions
boolean compareCryptToCipher(char*, char*, char*, int);
boolean checkWordsOfLengthN(int, char*, char*, char*);
boolean dictionaryAttack(char*, char*, char*);

/*
@param argc - the number of arguments passed
@param *argv - an array of arguments passed

@return 0 - always returns 0 per the specification

This function initializes the ciphertext and salt. Then, it starts the dictionary attack 
and the brute force algorithm.
*/
int main(int argc, char* argv[])
{
    // This will hold the salt
    char* salt = malloc(3 * sizeof(char));
    
    // Defines all of the possible ASCII characters in a password
    char* charSpectrum = malloc(NUM_ASCII_CHARS * sizeof(char));
    for(int i = 0; i < NUM_ASCII_CHARS; i++)
    {
        // Insert a character into the character spectrum. 
        // Right shift by 32 because we want ASCII 32 to ASCII 126
        charSpectrum[i] = (char) i + 32;     
    }
    
    // If there's more or fewer than one argument, complain and finish.    
    if(argc != 2)
    {
        printf("Invalid arguments\n");
        return 0;
    }
 
    // Grab the cipher text and store it in ciphertext.
    char* ciphertext = malloc(14 * sizeof(char));
    memcpy(ciphertext, argv[1], 13);
    ciphertext[13] = 0;

    // Set the first two characters of ciphertext to salt.
    salt[0] = ciphertext[0];
    salt[1] = ciphertext[1];
    salt[2] = 0;
    
    // First try dictionary attack because it is faster than brute force
    // If there is a result from the dictionary attack, print it and return 0
    if(dictionaryAttack("/usr/share/dict/words", ciphertext, salt))
    {
        //Free memory so Christopher doesn't dock me design points
        free(salt);
        free(charSpectrum);
        free(ciphertext);
        return 0;
    }
    
    
    // Then brute force. The for loop checks all plaintext from length 1 to length 8.
    for(int wordLength = 1; wordLength <= 8; wordLength++)
    {      
        // If a word is found, yay we cracked the password!
        if(checkWordsOfLengthN(wordLength, charSpectrum, ciphertext, salt))
        {
            //Free memory so Christopher doesn't dock me design points
            free(salt);
            free(charSpectrum);
            free(ciphertext);
            return 0;
        }
    }
}

/*
@param n - length of words to check (1 <= n <= 8)
@param spectrum - possible characters in the plaintext space
@param ciphertext - the ciphertext to compare with 
@param salt - the salt used to perturb the encryption algorithm

@return NULL - if no password found
@return char * - pointing to the password if found

This function loops through the plaintext space to generate words of length n.
Each time a word of length n is produced, it is encrypted and compared to the ciphertext.
All words of length n in the plaintext space are checked.
*/
boolean checkWordsOfLengthN(int n, char* spectrum, char* ciphertext, char* salt)
{
    // Create space for the password
    char* password = malloc((n+1)*sizeof(char));
    
    // Each nested for loop represents one character of the generated word.
    // When n characters are inserted into word, word is encrypted and compared to the ciphertext.
    for(int charOne = 0; charOne < NUM_ASCII_CHARS; charOne++)
    {
        password[0] = spectrum[charOne];
        for(int charTwo = 0; charTwo < NUM_ASCII_CHARS; charTwo++)
        {
            // If the word is of length one, null terminate, else keep building the word            
            password[1] = (n < 2) ? 0 : spectrum[charTwo];
            
            // If the word is of length one, compare the hash and break
            if(n < 2)
            {
                if(compareCryptToCipher(ciphertext, password, salt, 13) == TRUE)
                {
                    // Free memory so Christopher doesn't dock design points
                    free(password);
                    return TRUE;
                }
                break;
            }
            
            for(int charThree = 0; charThree < NUM_ASCII_CHARS; charThree++)
            {
                // If the word is of length two, null terminate, else keep building the word
                password[2] = (n < 3) ? 0 : spectrum[charThree];
                
                // If the word is of length two, compare the hash and break
                if( n < 3)
                {
                    if(compareCryptToCipher(ciphertext, password, salt, 13) == TRUE)
                    {
                        // Free memory so Christopher doesn't dock design points
                        free(password);
                        return TRUE;
                    }
                    break;
                }
                
                for(int charFour = 0; charFour < NUM_ASCII_CHARS; charFour++)
                {
                    // If the word is of length three, null terminate, else keep building the word
                    password[3] = (n < 4) ? 0 : spectrum[charFour];
                    
                    // If the word is of length three, compare the hash and break
                    if(n < 4)
                    {
                        if(compareCryptToCipher(ciphertext, password, salt, 13) == TRUE)
                        {
                            // Free memory so Christopher doesn't dock design points
                            free(password);
                            return TRUE;
                        }
                        break;
                    }
                    
                    for(int charFive= 0; charFive < NUM_ASCII_CHARS; charFive++)
                    {
                        // If the word is of length 4, null terminate, else keep building the word
                        password[4] = (n < 5) ? 0 : spectrum[charFive];
                        
                        //If the word is of length 4, compare the hash and break
                        if( n < 5 )
                        {
                            if(compareCryptToCipher(ciphertext, password, salt, 13) == TRUE)
                            {
                                // Free memory so Christopher doesn't dock design points
                                free(password);
                                return TRUE;
                            }
                            break;
                        }
                        
                        for(int charSix = 0; charSix < NUM_ASCII_CHARS; charSix++)
                        {
                            //If the word is of length 5, null terminate, else keep building the word
                            password[5] = (n < 6) ? 0 : spectrum[charSix];
                            
                            //If the word is of length 5, compare the hash and break
                            if( n < 6 )
                            {
                                if(compareCryptToCipher(ciphertext, password, salt, 13) == TRUE)
                                {
                                    // Free memory so Christopher doesn't dock design points
                                    free(password);
                                    return TRUE;
                                }
                                break;
                            }
                            
                            for(int charSeven = 0; charSeven < NUM_ASCII_CHARS; charSeven++)
                            {
                                //If the word is of length 6, null terminate, else keep building the word
                                password[6] = (n < 7) ? 0 : spectrum[charSeven];
                                
                                //If the word is of length 6, compare the hash and break
                                if( n < 7)
                                {
                                    if(compareCryptToCipher(ciphertext, password, salt, 13) == TRUE)
                                    {
                                        // Free memory so Christopher doesn't dock design points
                                        free(password);
                                        return TRUE;
                                    }
                                    break;
                                }
                                
                                for(int charEight = 0; charEight < NUM_ASCII_CHARS; charEight++)
                                {
                                    //If the word is of length 7, null terminate, else keep building the word
                                    password[7] = (n < 8) ? 0 : spectrum[charEight];
                                    
                                    //If the word is of length 7, compare the hash and break
                                    if( n < 8)
                                    {
                                        if(compareCryptToCipher(ciphertext, password, salt, 13) == TRUE)
                                        {
                                            // Free memory so Christopher doesn't dock design points
                                            free(password);
                                            return TRUE;
                                        }
                                        break;
                                    }
                                    
                                    // Null terminate the word 
                                    password[8] = 0;
                                    
                                    if(compareCryptToCipher(ciphertext, password, salt, 13) == TRUE)
                                    {
                                        // Free memory so Christopher doesn't dock design points
                                        free(password);
                                        return TRUE;
                                    }
                                }
                            } 
                        } 
                    }      
                }        
            }
        }
    } 
    
    // Return FALSE if no match found
    free(password);
    return FALSE;
}

/*
@param location - the URL of the word file
@param ciphertext - the ciphertext to compare to
@param salt - the salt used to perturb the ciphertext

@return NULL - if no password if found
@return str - the password found

This function runs a dictionary attack through a set of words found in the file at a specified location.
Each word is encrypted using crypt() and compared to the ciphertext.
*/
boolean dictionaryAttack(char* location, char* ciphertext, char* salt)
{
    //Make a placeholder dictionary file
    FILE* dictionary;
    
    //Try to find the dictionary
    dictionary = fopen(location, "r");

    // If the dictionary is found   
    if(dictionary)
    {
        // Allocate 9 bytes of memory to store read words.
        string str = malloc(9 * sizeof(char)); 
        
        // Loop through the dictionary, copying every 8 characters to str
        while(fgets (str , 9 , dictionary) != NULL)
        {
            // Check if the last character of str is a carriage return
            if(strlen(str) >= 1 && str[strlen(str) - 1] == '\n')
                str[strlen(str) - 1] = '\0';
            
            // Check if the second-to-last character of str is a carriage return    
            if(strlen(str) >= 2 && str[strlen(str) - 2] == '\n')
                str[strlen(str) - 2] = '\0';
      
            // Compare the cipher text to the crypt of str    
            if(compareCryptToCipher(ciphertext, str, salt, 13) == TRUE)
            {
                //Freeing memory so Christopher doesn't dock design points
                free(str);
                
                //Close the dictionary
                fclose(dictionary);
                return TRUE;
            }
        }
        
        //Freeing memory so Christopher doesn't dock design points
        free(str);
    }
    
    // Close the dictionary if password not found
    fclose(dictionary);
    return FALSE;
}

/*
@param ciphertext - the ciphertext to compare to
@param word - the password to encrypt and compare
@param salt - the salt used to perturb the encryption
@param bytes - number of bytes to compare

@return TRUE - if the hash of the word is the same as the ciphertext
@return FALSE - if the hash of the word is not the same as the ciphertext

This function compares the encrypted hash of a word to the ciphertext. If they are
equivalent, the function also prints the word.
*/
boolean compareCryptToCipher(char* ciphertext, char* word, char* salt, int bytes)
{
    // Compare the encrypted word to the ciphertext
    if(memcmp(ciphertext, crypt(word, salt), bytes) == 0)
    {
        // Print the word found
        printf("%s\n", word);
        return TRUE;
    }
    
    return FALSE;
}

