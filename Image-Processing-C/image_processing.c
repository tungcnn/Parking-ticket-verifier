#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "lodepng.h"

// gcc image_processing.c lodepng.c -ansi -pedantic -Wall -Wextra -O3 -o IP.exe -std=c17

char hexImageFile[] = "Hex-Input/HEXFILE.hex";
int IMAGE_WIDTH = 0;
int IMAGE_HEIGHT = 0;
int imageSize = 0;

unsigned char *imageData;
int *red;
int *green;
int *blue;

unsigned char *hex_reader(char *imageFile)
{
    FILE *file = fopen(imageFile, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    // Read the IMAGE_WIDTH and IMAGE_HEIGHT from the file
    if (fscanf(file, "%d %d", &IMAGE_WIDTH, &IMAGE_HEIGHT) != 2)
    {
        printf("Error reading image dimensions\n");
        fclose(file);
        exit(1);
    }

    // Calculate the image size
    imageSize = IMAGE_WIDTH * IMAGE_HEIGHT;

    imageData = (unsigned char *)malloc(imageSize * 3 * sizeof(unsigned char));
    red = (int *)malloc(imageSize * sizeof(int));
    green = (int *)malloc(imageSize * sizeof(int));
    blue = (int *)malloc(imageSize * sizeof(int));

    int arraySize = 0;

    while (fscanf(file, "%hhx", &imageData[arraySize]) != EOF) {
        red[arraySize] = (int)imageData[arraySize];
        green[arraySize] = (int)imageData[arraySize + 1];
        blue[arraySize] = (int)imageData[arraySize + 2];

        arraySize += 3;

        if (arraySize >= imageSize) {
            break; 
        }
    }

    fclose(file);

    return imageData;
}

int *grey_scale_converter()
{
    int *greyScale = (int *)malloc(imageSize * sizeof(int));

    if (greyScale == NULL)
    {
        // Handle memory allocation error
        printf("Error allocating memory for greyScale\n");
        exit(1);
    }

    for (int i = 0; i < imageSize; i++)
    {
        // Calculate grayscale value using the average method
        int greyValue = (red[i] + green[i] + blue[i]) / 3;

        greyScale[i] = greyValue;
    }

    return greyScale;
}

int *black_and_white_converter()
{
    int *blackAndWhite = (int *)malloc(imageSize * sizeof(int));

    if (blackAndWhite == NULL)
    {
        // Handle memory allocation error
        printf("Error allocating memory for blackAndWhite\n");
        exit(1);
    }

    for (int i = 0; i < imageSize; i++)
    {
        int greyValue = (red[i] * 0.3) + (green[i] * 0.59) + (blue[i] * 0.11);

        // Set a threshold value (e.g., 127) to determine black or white
        if (greyValue > 127)
        {
            blackAndWhite[i] = 255; // Set to white (255)
        }
        else
        {
            blackAndWhite[i] = 0; // Set to black (0)
        }
    }

    return blackAndWhite;
}

void inverse_converter()
{
    // Inverse an RGB image
    for (int i = 0; i < imageSize; i++)
    {
        // Invert the red channel
        red[i] = 255 - red[i];
        if (red[i] <= 0)
        {
            red[i] = 0;
        }

        // Invert the green channel
        green[i] = 255 - green[i];
        if (green[i] <= 0)
        {
            green[i] = 0;
        }

        // Invert the blue channel
        blue[i] = 255 - blue[i];
        if (blue[i] <= 0)
        {
            blue[i] = 0;
        }
    }
}

void free_memory(unsigned char *image, int *greyScale, int *blackAndWhite)
{
    // Free memory for other dynamically allocated arrays
    free(greyScale);
    free(blackAndWhite);
    free(red);
    free(green);
    free(blue);

    // Free memory for the image data
    free(image);
}

int main()
{
    unsigned char *image = NULL;
    int *greyScale = NULL;
    int *blackAndWhite = NULL;

    image = hex_reader(hexImageFile);

    // Print test values
    for (int i = 0; i < 10; i++)
    {
        printf("(Red: %d, ", red[i]);
        printf("Green: %d, ", green[i]);
        printf("Blue: %d)", blue[i]);
        printf("\n");
    }

    printf("Image size: %d\n", imageSize);

    greyScale = grey_scale_converter();
    // Print test values
    for (int i = 0; i < 10; i++)
    {
        printf("GreyScale: %d\n", greyScale[i]);
    }

    blackAndWhite = black_and_white_converter();
    inverse_converter();

    unsigned char *rgbImage = (unsigned char *)malloc(imageSize * 3 * sizeof(unsigned char));
    for (int i = 0; i < imageSize; i++)
    {
        rgbImage[i * 3] = (unsigned char)red[i];
        rgbImage[i * 3 + 1] = (unsigned char)green[i];
        rgbImage[i * 3 + 2] = (unsigned char)blue[i];
    }

    // create the file and write the RGB image data to it
    unsigned error = lodepng_encode24_file("Output-Pictures/Output.png", rgbImage, IMAGE_WIDTH, IMAGE_HEIGHT);
    if (error)
    {
        printf("error %u: %s\n", error, lodepng_error_text(error));
    }

    // Greyscale to save
    unsigned char *greyScaleToSave = (unsigned char *)malloc(imageSize * sizeof(unsigned char));

    for (int i = 0; i < imageSize; i++)
    {
        greyScaleToSave[i] = greyScale[i];
    }

    // create the file and write the grayscale image data to it
    error = lodepng_encode_file("Output-Pictures/Output_GreyScale.png", greyScaleToSave, IMAGE_WIDTH, IMAGE_HEIGHT, LCT_GREY, 16);

    // Free allocated memory
    free_memory(image, greyScale, blackAndWhite);

    return 0;
}