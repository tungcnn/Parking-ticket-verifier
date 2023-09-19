#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <string.h>
#include <time.h>

int* red;
int* green;
int* blue;

#define ROWS 985
#define COLS 16

#define imageSize ROWS * COLS

int* hex_reader(char* imageFile)
{
    int* image = malloc(imageSize * sizeof(int));
    FILE * fp;

    fp = fopen(imageFile, "r");

    if(fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    // assign FP to image array and read in the hex values
    int i = 0;
    while(fscanf(fp, "%x", &image[i]) != EOF)
    {
        i++;
    }

    fclose(fp);
    return image;

}

int rgb_converter(int* image)
{

    red = malloc(imageSize * sizeof(int));
    green = malloc(imageSize * sizeof(int));
    blue = malloc(imageSize * sizeof(int));

    int i = 0;

    for (i = 0; i < imageSize; i++)
    {
        red[i] = (image[i] >> 16) & 0xFF;
        green[i] = (image[i] >> 8) & 0xFF;
        blue[i] = (image[i] >> 0) & 0xFF;
    }

    return 1;
}

int* grey_scale_converter()
{
    int* greyScale = malloc(imageSize * sizeof(int));

    int i = 0;

    for (i = 0; i < imageSize; i++)
    {
        greyScale[i] = (red[i] * 0.3) + (green[i] * 0.59) + (blue[i] * 0.11);
    }

    return greyScale;
}

// int* black_and_white_converter(int* greyScale, int imageSize)
// {
//     int* blackAndWhite = malloc(imageSize * sizeof(int));

//     int i = 0;

//     for (i = 0; i < imageSize; i++)
//     {
//         if (greyScale[i] > 127)
//         {
//             blackAndWhite[i] = 1;
//         }
//         else
//         {
//             blackAndWhite[i] = 0;
//         }
//     }

//     return blackAndWhite;
// }

// int* inverse_converter(int* red, int* green, int* blue, int imageSize)
// {
//     int* inverse_red = malloc(imageSize * sizeof(int));
//     int* inverse_green = malloc(imageSize * sizeof(int));
//     int* inverse_blue = malloc(imageSize * sizeof(int));

//     int i = 0;

//     for (i = 0; i < imageSize; i++)
//     {
//         inverse_red[i] = 255 - red[i];
//         inverse_green[i] = 255 - green[i];
//         inverse_blue[i] = 255 - blue[i];
//     }

//     return inverse_red, inverse_green, inverse_blue;
// }

int main()
{
    char imageFile[] = "./car.hex";

    int* red_inverse = malloc(imageSize * sizeof(int));
    int* green_inverse = malloc(imageSize * sizeof(int));
    int* blue_inverse = malloc(imageSize * sizeof(int));

    int* greyScale = malloc(imageSize * sizeof(int));
    int* blackAndWhite = malloc(imageSize * sizeof(int));
    int* inverse = malloc(imageSize * sizeof(int));

    int* image = hex_reader(imageFile);

    rgb_converter(image);
    greyScale = grey_scale_converter(red, green, blue);
    // blackAndWhite = black_and_white_converter(greyScale, imageSize);
    // red_inverse, green_inverse, blue_inverse = inverse_converter(red, green, blue, imageSize);

    // Convert the RGB values back to images and save them as .hex files

    //print image as a test to see if it works
    int i = 0;
    for (i = 0; i < imageSize; i++)
    {
        printf("%x ", greyScale[i]);
        if(i % 16 == 0)
        {
            printf("\n");
        }
    }
    
    return 0;
}