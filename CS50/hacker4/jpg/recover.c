/****************************************************************************
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 ***************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

bool isMagic(char char1, char char2, char char3, char char4);

// FIND ALL DEM IMAGES
int main()
{
    // open card file
    FILE* inptr = fopen("card.raw", "r");
    if (inptr == NULL)
    {
        printf("Could not open card.raw");
        return 1;
    }
    
    // Name of the file
    char fileName[8];
    int curFile = 0;
    sprintf(fileName, "%d%d%d%s", curFile/100, (curFile/10)%10, curFile%10, ".jpg");
    
    // create buffer to hold 512 bytes at a time
    char buffer[512];
    
    // boolean true if at the start of a new JPEG
    bool isJPEG = false;
    
    // Scan for the first JPEG trace
    while(isJPEG == false && !feof(inptr))
    {
        // fill the buffer
        fread(&buffer, sizeof(char), 512, inptr);
        
        // if the buffer begins with the magic sequence
        isJPEG = isMagic(buffer[0], buffer[1], buffer[2], buffer[3]);
    }
    
    // assuming JPEG files are contiguous
    // scan until end of file
    while(!feof(inptr))
    {   
        // isJPEG will be false until the next JPEG appears
        isJPEG = false;
        
        // open output file
        FILE* outptr = fopen(fileName, "w");
        
        // increment the name
        curFile++;
        sprintf(fileName, "%d%d%d%s", curFile/100, (curFile/10)%10, curFile%10, ".jpg");
        
        // check if we have an output file    
        if (outptr == NULL)
        {
            fclose(inptr);
            fprintf(stderr, "Could not create output file");
            return 3;
        }
            
        do{
            // first write because we've already read
            fwrite(buffer, sizeof(char), 512, outptr);
            
            // read the next 512 bytes
            fread(&buffer, sizeof(char), 512, inptr);
            
            // check if the bytes mark a new JPEG
            isJPEG = isMagic(buffer[0], buffer[1], buffer[2], buffer[3]);
            
        }while(!isJPEG && !feof(inptr));
        
        // we're done with this JPEG file
        fclose(outptr);
    }   
    
    // and that's it
    fclose(inptr);
    return 0;
}

/**
 @param char1, char2, char3, char4 - the charsequence to check
 @return true - if the buffer starts with a magic sequence
 @return false - the buffer does not begin with a magic sequence
    
 Checks if the buffer starts with a magic sequence.   
*/
bool isMagic(char char1, char char2, char char3, char char4)
{
    // check for the sequence 0xff 0xd8 0xff (0xe0 || 0xe1)
    return char1 == -1 && char2 == -40 && char3 == -1 && (char4 == -32 || char4 == -31);
}
