/****************************************************
 * Collin Gordon 1000215046
 * CIT131 - 3001
 * 12/9/2014
 * Programming Project 10 - PPM Editor
 * This program is designed to read in
 * a Portable Pixel Map (PPM) and transform
 * it according to user choices. The program
 * has several options available including:
 * negate red, green, and blue, flatten red,
 * green, and blue, turn the image to
 * gray scale, and flip the image horizontally.
 * Upon completion, the image is written to
 * a new PPM file that can be displayed via
 * the display command in Linux.
 * NOTE: THIS PROGRAM IS DESIGNED FOR ASCII MACHINERY
 ***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ARRAY_SIZE 1920 /* size of pixel array */
#define FNAME_SIZE 20 /* max length of file name */
#define CHOICES 8 /* max number of choices the user can make */

//File Header Data
typedef struct HEADER
{
    char magicChar1; /* stores the first char of magic number */
    char magicChar2; /* stores the second char of magic number */
    int width;
    int height;
    int colorMax; /* stores color depth */
} Header;

//Pixel Data
typedef struct PIXEL
{
    int red;
    int green;
    int blue;
} Pixel;

Header scan_header( FILE* inFile);
int scan_array(FILE* input, FILE* output, Pixel kodak[],
                  int menuItems[], int arrSize, int height, int width,
                  int color);
void negateRed(Pixel nikon[], int width, int colorDepth);
void negateGreen(Pixel nikon[], int width, int colorDepth);
void negateBlue(Pixel nikon[], int width, int colorDepth);
void flattenRed(Pixel cannon[], int width);
void flattenGreen(Pixel cannon[], int width);
void flattenBlue(Pixel cannon[], int width);
void grayScale(Pixel photo[], int width);
void flipHorizontal(Pixel photo[], int width);
void write_header(Header info, FILE* outFile);
void menu(int selections[], int arrSize);
void transform(int options[], Pixel wolf[], int width, int arrSize,
               int colorInfo);
void write_array(Pixel cannon[], int width, FILE* outFile);
void flush(FILE* infile);
int meanThree(int num1, int num2, int num3);

/***************
 * MAIN FUNCTION
 * This function sets arrays,
 * calls functions, and opens and
 * closes files.
 * ************/

int main(void)
{
    Pixel image[ARRAY_SIZE]; /* Array of pixels for manipulation */
    Header block; /* header information from the input file */
    FILE* pixIn;  /* file pointer for input file. */
    FILE* pixOut; /* file pointer for transformed output file */
    char inFileName[FNAME_SIZE]; /* array for the input file name */
    char outFileName[FNAME_SIZE];/* array for the output file name */
    int flavors[CHOICES] = {0}; /* array for the transformations
                                 * selected. Initialized to all zeros
                                 * to prevent hackers and garbage.*/
    bool_t badFile = TRUE; /* Boolean for readable file. */
    int success; /* integer saying that reading pixels was a success */

    /* loop for opening input file.
     * Validates whether file can be opened. */

    puts("THE PORTABLE PIXEL MAP (PPM) EDITOR");
    puts("---------by Collin Gordon----------");
    putchar('\n');
    do
    {
        printf("Please enter the name of the input file: ");
        scanf("%20s", inFileName);

        if((pixIn = fopen(inFileName, "r")) != NULL)
        {
            badFile = FALSE;
        }
        else
        {
            puts("Error in input file opening. Please try again.");
        }
    } while(badFile);

    badFile = TRUE; /* resetting badFile for opening output file */

    /* loop for opening the output file for writing. */
    do
    {
        printf("Please enter the name of the output file: ");
        scanf("%20s", outFileName);
        if((pixOut = fopen(outFileName, "w")) != NULL)
        {
            badFile = FALSE;
        }
        else
        {
            puts("Error creating file output. Please try again.");
        }
    } while(badFile);

    menu(flavors, CHOICES); /* calling menu */
    block = scan_header(pixIn); /* calling function to read header */

    /* checking to see if dimensions are acceptable */
    if(block.width < ARRAY_SIZE && block.width > 0)
    {
        write_header(block, pixOut); /* writing header to output */
        success = scan_array(pixIn, pixOut, image, flavors,
                                block.width, block.height, CHOICES,
                                block.colorMax); /* scanning pixels */

        if(success == 0) /* checking to see if pixel values correct */
        {
            /* error in pixels */
            puts("Error. A pixel was over the maximum color depth.");
            fclose(pixIn);
            fclose(pixOut);
        }
        else /* successful read */
        {

            fclose(pixIn);
            fclose(pixOut);
            puts("...image created.");
        }
    }
    else /* error in dimensions */
    {
        puts("Error. Width must be in range 1 - 1920 pixels each.");
        fclose(pixIn);
        fclose(pixOut);
    }
    return 0;
}
/***************************************************
 * MENU FUNCTION
 * This function contains the menu
 * that tells the program which transformations
 * to add to the output file. It uses an array
 * to store the integer associated with each option.
 * ************************************************/
void menu(int selections[], int arrSize)
{
    char retry = 'y'; /* option to retry entry */
    int i = 0; /* counter variable of outer loop */
    bool flag; /* loop control */
    int remaining = arrSize; /* tells user how many more
                              choices can be made */

    /* Instructions and options with associated integers */
    puts("Please select transformations from the options below.");
    puts("[1] Negate Red    [4] Flatten Red");
    puts("[2] Negate Green  [5] Flatten Green");
    puts("[3] Negate Blue   [6] Flatten Blue");
    puts("[7] Gray Scale    [8] Flip Horizontal");
    puts("To select a transformation, enter the option number below.");
    puts("To exit enter a number greater than 8, then press n.");

    /* Input loop. */
    do {
        printf("%d selections remaining: ", remaining);
        scanf("%d", &selections[i]);
        if(selections[i] > arrSize || selections[i] < 1)
        {
            flag = true; /* making the condition to enter retry loop. */

            while(flag) { /* retry loop */

                printf("%d is not a valid selection.", selections[i]);
                printf("Try again? y/n: ");
                scanf(" %c", &retry);

                if(retry == 'y') {
                    flag = false;
                } else if(retry == 'n')v{
                    puts("Creating image...");
                    i = 999;
                    flag = false;
                } else {
                    puts("Choice invalid.");
                }
            }

        } else {
            i++;
            remaining--;
        }
    } while(i < arrSize);
}

/*******************************
 * SCANHEADERDATA FUNCTION
 * This function scans in information
 * from the input file and assigns it
 * to appropriate spots in the header
 * structure. This function scans in
 * the magic number, the dimensions, and
 * the color depth while ignoring any
 * comments left by the user.
 * ****************************/

Header scan_header(FILE* inFile)
{
    Header fileTop;
    bool sharpHere = true; /* loop control variable */
    int sharpSign; /* stores the return value of fgetc as ASCII code */

    /* reading in magic number */
    fscanf(inFile, "%c%c",&fileTop.magicChar1, &fileTop.magicChar2);

    if(fileTop.magicChar1 == 'P' && fileTop.magicChar2 == '3')
    {
        flush(inFile); /* clearing buffer */

        /* loop for finding and removing comments */
        while(sharpHere) {

            sharpSign = fgetc(inFile);

            if(sharpSign == '#') {
                sharpHere = true; /* maintains loop control */
                flush(inFile); /* removing comments */
            } else {
                /* returning the read value back to the stream,
                 * reading in the dimensions and color depth,
                 * then clearing the buffer of any new lines. */
                ungetc(sharpSign,inFile);
                fscanf(inFile, "%d %d %d", &fileTop.width,
                       &fileTop.height, &fileTop.colorMax);
                flush(inFile);
                sharpHere = false; /* exiting loop */
            }
        }
    } else {
        puts("Error reading magic number."); /* error for wrong file */
    }

    return fileTop;
}

/**************************
 * SCANARRAYDATA FUNCTION
 * This function reads in the
 * pixels from the file and stores
 * them in an array. Then, the function
 * calls the transform and array2output
 * functions to write the array data to the
 * output file. If any red, green, or blue value
 * is over the color depth, then the function returns
 * a zero to the program. If the read is successful the
 * function returns 1 and continues processing from the
 * file.
 * **********************/
int  scan_array(FILE* input, FILE* output, Pixel kodak[],
                   int menuItems[], int width, int height, int arrSize,
                   int color)
{
    int i; /*counter for height of the image */
    int j; /*counter for width of the image */
    bool success = true; /* checks for elements over max color depth */

    for(i = 0; i < height; i++) {

        for(j = 0; j < width;) {

            fscanf(input, "%d %d %d",
                   &kodak[j].red, &kodak[j].green, &kodak[j].blue);

            if(kodak[j].red <= color && kodak[j].green <= color
               && kodak[j].blue <= color)
            {
                j++; /* successful read */
            }
            else
            {
                success = false; /* error */
                j = width; /* setting j to width causing loop exit */
            }
        }
        /* transforming and writing the array before scanning next
         * set of pixels. */
        transform(menuItems, kodak, width, arrSize, color);
        write_array(kodak, width, output);
    }
    return success;

}

/***********************
 * HEADER2OUTPUT FUNCTION
 * This function writes the elements
 * of the header structure to the output
 * file. It also writes a comment containing
 * the name of the program's author.
 * ********************/

void write_header(Header info, FILE* outFile)
{
    fprintf(outFile, "%c%c\n",info.magicChar1, info.magicChar2);
    fprintf(outFile, "# Collin Gordon\n");
    fprintf(outFile, "%d %d\n%d\n", info.width, info.height,
            info.colorMax);
}

/***********************
 * ARRAY2OUTPUT FUNCTION
 * This function writes the
 * information in the pixel array
 * to the output file.
 * ********************/

void write_array(Pixel frame[], int width, FILE* outFile)
{
    for(int i = 0; i < width; i++)
    {
        fprintf(outFile, "%d %d %d ", frame[i].red,
                frame[i].green, frame[i].blue);
    }
    fputc('\n', outFile);
}

/********************
 * TRANSFORM FUNCTION
 * This function takes
 * the choices the user entered
 * and applies them one at a time
 * to the pixel array.
 * *****************/

void transform(int options[], Pixel wolf[], int width,
               int arrSize, int colorInfo)
{
    int i; /* counter for loop */

    /* scanning the options array for the user choices and performing
     * the requested functions */

    for(i = 0; i < arrSize; i ++) {
        if(options[i] == 1)
        {
            negateRed(wolf, width, colorInfo);
        }
        else if(options[i] == 2)
        {
            negateBlue(wolf, width, colorInfo);
        }
        else if(options[i] == 3)
        {
            negateGreen(wolf, width, colorInfo);
        }
        else if(options[i] == 4)
        {
            flattenRed(wolf, width);
        }
        else if(options[i] == 5)
        {
            flattenBlue(wolf, width);
        }
        else if(options[i] == 6)
        {
            flattenGreen(wolf, width);
        }
        else if(options[i] == 7)
        {
            grayScale(wolf, width);
        }
        else if(options[i] == 8)
        {
            flipHorizontal(wolf, width);
        }
        else
        {
        }
    }
}

/******************
 * NEGATE FUNCTIONS
 * The following three
 * functions produce negatives
 * of their respective colors. This
 * is done by taking the absolute value
 * of the value of the color minus the
 * color depth.The combined effect of
 * all three creates a photo
 * negative of the image.
 * ***************/

void negateRed(Pixel nikon[], int width, int colorDepth)
{
    for(int i = 0; i < width; i++) {
        nikon[i].red = abs(nikon[i].red - colorDepth);
    }
}

void negateGreen(Pixel nikon[], int width, int colorDepth)
{
    for(int i = 0; i < width; i++) {

        nikon[i].green = abs(nikon[i].green - colorDepth);
    }
}

void negateBlue(Pixel nikon[], int width, int colorDepth)
{
    for(int i = 0; i < width; i++) {
        nikon[i].blue = abs(nikon[i].blue - colorDepth);
    }
}

/*******************
 * FLATTEN FUNCTIONS
 * The following three
 * functions set the value
 * of their respective colors
 * to zero.
 * ****************/

void flattenRed(Pixel cannon[], int width)
{
    for(int i = 0; i < width; i++) {
        cannon[i].red = 0;
    }
}

void flattenGreen(Pixel cannon[], int width)
{
    for(i = 0; i < width; i++) {
        cannon[i].green = 0;
    }
}

void flattenBlue(Pixel cannon[], int width)
{
    for(int i = 0; i < width; i++) {
        cannon[i].blue = 0;
    }
}

/********************
 * GRAYSCALE FUNCTION
 * This function creates
 * a gray scale image by
 * setting all the pixels
 * equal to the average of the
 * values of red, green, and blue
 * in each pixel.
 * *****************/

void grayScale(Pixel photo[], int width)
{
    int average; /* stores the average of each pixel */

    for(int i = 0; i < width; i++) {
        average = meanThree(photo[i].red, photo[i].green, photo[i].blue);
        photo[i].red = photo[i].green = photo[i].blue = average;
    }
}

/*************************
 * FLIPHORIZONTAL FUNCTION
 * This function flips the
 * image horizontally by switching
 * the position of each pixel until
 * it hits the middle.
 * **********************/

void flipHorizontal(Pixel photo[], int width)
{
    Pixel temp;

    for (int i = 0, j = width - 1; i < j; i++, j--) {
        temp = photo[i];
        photo[i] = photo[j];
        photo[j] = temp;
    }
}

/********************
 * MEANTHREE FUNCTION
 * This function calculates
 * the average of red, green,
 * and blue in each pixel.
 * *****************/

int meanThree(int num1, int num2, int num3)
{
    const int maxNums = 3;
    return (num1 + num2 + num3) / maxNums;
}

void flush(FILE* inFile)
{
    while (fgetc(inFile) != '\n') {}
}
