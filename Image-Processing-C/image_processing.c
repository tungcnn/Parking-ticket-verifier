#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "lodepng.h"

// Command to run the program
// gcc image_processing.c lodepng.c -ansi -pedantic -Wall -Wextra -O3 -o image_processing.exe -std=c17

char hexImageFile[] = "Hex-Input/HEXFILE.hex";
int IMAGE_WIDTH = 0;
int IMAGE_HEIGHT = 0;
int imageSize = 0;

unsigned char *imageData;
int *red;
int *green;
int *blue;

unsigned char *hex_reader(char *imageFile) {
    FILE *file = fopen(imageFile, "rb");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    // Read the IMAGE_WIDTH and IMAGE_HEIGHT from the file
    if (fscanf(file, "%d %d", &IMAGE_WIDTH, &IMAGE_HEIGHT) != 2) {
        printf("Error reading image dimensions\n");
        fclose(file);
        exit(1);
    }

    // Calculate the image size
    imageSize = IMAGE_WIDTH * IMAGE_HEIGHT;

    // Allocate memory for the image data
    imageData = (unsigned char *)malloc(imageSize * 3 * sizeof(unsigned char));
    red = (int *)malloc(imageSize * sizeof(int));
    green = (int *)malloc(imageSize * sizeof(int));
    blue = (int *)malloc(imageSize * sizeof(int));

    if (imageData == NULL || red == NULL || green == NULL || blue == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int arraySize = 0;
    unsigned int pixelData;

    while (fscanf(file, "%03x", &pixelData) != EOF) {
        // Store the pixel data in the image data array
        if (arraySize < imageSize * 3) {
            imageData[arraySize] = (unsigned char)pixelData;
            arraySize++;
        } else {
            break;
        }
    }

    // Process the image data to extract RGB values
    int i;
    for (i = 0; i < arraySize; i ++) {
        red[i] = imageData[i];
        green[i] = imageData[i + 1];
        blue[i] = imageData[i + 2];
        
        // Output RGB values
        // printf("RGB(%d, %d, %d)\n", red[i / 3], green[i / 3], blue[i / 3]);
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

        // Store the grayscale value in the greyScale array
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
        int greyValue = (int)((red[i] * 0.3) + (green[i] * 0.59) + (blue[i] * 0.11));

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
    // Invert an RGB image
    for (int i = 0; i < imageSize; i++)
    {
        // Invert the red channel
        red[i] = 255 - red[i];
        if (red[i] < 0)
        {
            red[i] = 0;
        }

        // Invert the green channel
        green[i] = 255 - green[i];
        if (green[i] < 0)
        {
            green[i] = 0;
        }

        // Invert the blue channel
        blue[i] = 255 - blue[i];
        if (blue[i] < 0)
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

    // Read the image in hexadecimal format
    image = hex_reader(hexImageFile);

    printf("Image size: %d\n", imageSize);

    // Convert the image to grayscale
    greyScale = grey_scale_converter();

    // Print the grayscale values for the first 10 pixels (for testing)
    // for (int i = 0; i < 10; i++)
    // {
    //     printf("GreyScale: %d\n", greyScale[i]);
    // }

    // Check how many 0s are in the loaded greyscale image
    int zeroCount = 0;

    for (int i = 0; i < imageSize; i++)
    {
        if (greyScale[i] == 0)
        {
            zeroCount++;
        }
    }

    printf("Zero count: %d\n", zeroCount);

    // Convert the grayscale image to black and white
    // blackAndWhite = black_and_white_converter(greyScale);

    // Invert the colors of the original image
    // inverse_converter();

    // Allocate memory for an RGB image
    unsigned char *rgbImage = (unsigned char *)malloc(imageSize * 3 * sizeof(unsigned char));

    // Create an RGB image from the red, green, and blue channels
    for (int i = 0; i < imageSize -1;)
    {
        rgbImage[i] = (unsigned char)red[i];
        rgbImage[i + 1] = (unsigned char)green[i];
        rgbImage[i + 2] = (unsigned char)blue[i];

        // Create a loading bar to show progress
        if (i % 1000 == 0)
        {
            printf("Loading: %d%%\r", (int)((i / (float)imageSize) * 100));
        }

        i += 3;
    }

    // Save the RGB image to a file using LodePNG
    unsigned error = lodepng_encode_file("Output-Pictures/Output_RGB.png", rgbImage, IMAGE_WIDTH, IMAGE_HEIGHT, LCT_RGB, 16);
    if (error)
    {
        printf("Error %u: %s\n", error, lodepng_error_text(error));
    }

    // Create an array to store grayscale data for saving
    unsigned char *greyScaleToSave = (unsigned char *)malloc(imageSize * sizeof(unsigned char));

    // Copy the grayscale data for saving
    // for (int i = 0; i < imageSize -1; i++)
    // {
    //     greyScaleToSave[i] = greyScale[i];

    //     // Testing values in greyscale for debugging.
    //     if(i % 100000)
    //     {
    //         if(greyScaleToSave[i] != 0)
    //         {
    //             printf("Value: %d\n", greyScale[i]);
    //         }
            
    //     }
    // }

    // Save the grayscale image to a file using LodePNG
    error = lodepng_encode_file("Output-Pictures/Output_GreyScale.png", greyScaleToSave, IMAGE_WIDTH, IMAGE_HEIGHT, LCT_GREY, 255);

    // Free allocated memory
    free_memory(image, greyScale, blackAndWhite);

    return 0;
}
