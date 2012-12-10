/****************************************************************************
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes a BMP by a factor using nearest-neighbor interpolation.
 ***************************************************************************/
       
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

#include "bmp.h"

// prototyping
void upsize(FILE* input, RGBTRIPLE* buffer, int factor, int oldbiHeight, int oldbiWidth, int oldpadding);
void downsize(RGBTRIPLE* input, FILE* output, int factor, int padding, int width, int height);
int gcd(int a, int b);
void processImage(FILE* output, FILE* input, float factor, BITMAPFILEHEADER bf, BITMAPINFOHEADER bi);

// let's resize an image
int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: resize infile outfile\n");
        return 1;
    }

    // get the scale factor to 1 decimal place
    float factor = (int)(10*atof(argv[1]))/10.0;
    
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open temp file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // process the image
    processImage(outptr, inptr, factor, bf, bi);
    
    // we're done with these!
    fclose(inptr);
    fclose(outptr);

    // that's all folks
    return 0;
}

/**
 @param output - the output file
 @param input - the input file
 @param factor - the scale factor
 @param bf - BITMAPFILEHEADER to read
 @param bi - BITMAPINFOHEADER to read
 
 Resizes the image and writes to the output file.
*/
void processImage(FILE* output, FILE* input, float factor, BITMAPFILEHEADER bf, BITMAPINFOHEADER bi)
{
    // Store original width and height
    int oldbiHeight = bi.biHeight;
    int oldbiWidth = bi.biWidth;
    
    // Set the new image dimensions
    bi.biHeight = (int)(bi.biHeight * factor);
    bi.biWidth = (int)(bi.biWidth * factor);
    
    // determine padding for scanlines
    int oldpadding =  (4 - (oldbiWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // Update the image size
    bi.biSizeImage = bi.biWidth * abs(bi.biHeight) * 3 + abs(bi.biHeight) * padding;
    
    // Update the file size
    bf.bfSize = bi.biSizeImage + 54;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, output);
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, output);
    
    // Turn the factor into a numerator and denominator
    int upsizeFactor = (int)(factor * 10);
    int downsizeFactor = 10;
    int GCD = gcd(upsizeFactor, downsizeFactor);
    
    // Optimize by reducing to lowest terms
    upsizeFactor /= GCD;
    downsizeFactor /= GCD;
    
    // IDK how to use multidimensional arrays... i tried :(
    // So instead I used an array big enough to hold all the new pixels
    int bufferLength = upsizeFactor * upsizeFactor * oldbiWidth * abs(oldbiHeight);
    RGBTRIPLE buffer[bufferLength];
    
    // Upscale first
    upsize(input, buffer, upsizeFactor, oldbiHeight, oldbiWidth, oldpadding);
    
    // Then downscale
    downsize(buffer, output, downsizeFactor, padding, oldbiWidth * upsizeFactor, oldbiHeight * upsizeFactor);    
}

/**
 @param a - first integer 
 @param b - second integer
 
 @return - the GCD of a and b
 
 Finds the GCD of a and b.
*/
int gcd(int a, int b)
{
    // Found the GCD
    if(a == 0)
        return b;
    
    // Found the GCD    
    if(b == 0)
        return a;
    
    // Use the Euclidean Algorithm to reduce the problem
    // http://mathworld.wolfram.com/EuclideanAlgorithm.html    
    return (a > b) ? gcd(a%b, b) : gcd(a, b%a);
}

/**
 @param input - the file to read from
 @param buffer - the buffer to store the pixels to
 @param factor - the factor to upscale by
 @param oldbiHeight - the old image height
 @param oldbiWidth - the old image width
 @param oldpadding - the old padding
 
 Upscales the image by a given factor and stores the RGBTRIPLEs in an array.
*/
void upsize(FILE* input, RGBTRIPLE* buffer, int factor, int oldbiHeight, int oldbiWidth, int oldpadding)
{    
    // iterate over infile's scanlines
    for (int i = 0, absbiHeight = abs(oldbiHeight); i < absbiHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < oldbiWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, input);
            
            // Store the triple factor^2 times
            // First loop handles the horizontal factor
            for(int a = 0; a < factor; a++)
            {              
                //store RGB triple in buffer "vertically" factor times
                for(int c = 0; c < factor; c++)
                {
                    // Ugly formula to calculate position in a 1-D array
                    // because I couldn't get a 2D array to work :(
                    int position = i * factor * oldbiWidth * factor + factor * j + a + c * oldbiWidth * factor;
                    
                    // Store the triple at the position
                    buffer[position] = triple; 
                }
            }
        }

        // skip over padding, if any
        fseek(input, oldpadding, SEEK_CUR);
    }
}

/**
 @param input - the buffer to read from
 @param output - the file to write the triples to
 @param factor - the factor to downscale by
 @param padding - the new padding
 @param width - the upscaled image width
 @param height - the upscaled image height
 
 Downscales the image by a given factor and writes the RGBTRIPLEs to an output file.
*/
void downsize(RGBTRIPLE* input, FILE* output, int factor, int padding, int width, int height)
{   
    // iterate over buffer's height
    // skip every factor lines
    for (int i = 0, absbiHeight = abs(height); i < absbiHeight; i += factor)
    {
        // iterate over buffer's width
        for (int j = 0; j < width; j++)
        {
            // we only want one pixel out of every factor pixels           
            if(j % factor == 0)
                // write RGB triple to outfile
                fwrite(&(input[i * width + j]), sizeof(RGBTRIPLE), 1, output);
        }

        // then add the padding
        for (int k = 0; k < padding; k++)
            fputc(0x00, output);
    }
}
