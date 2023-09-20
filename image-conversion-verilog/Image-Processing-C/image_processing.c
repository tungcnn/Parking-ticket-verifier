#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// Define global arrays for RGB values
unsigned char *red = NULL;
unsigned char *green = NULL;
unsigned char *blue = NULL;

char hexImageFile[] = "./input/HEXFILE.hex";
int IMAGE_WIDTH = 0;
int IMAGE_HEIGHT = 0;

unsigned char **hex_reader(char *imageFile, int *imageSize)
{
    FILE *hexFileOpen = fopen(imageFile, "r");
    if (hexFileOpen == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    // Calculate the number of columns in the file to determine IMAGE_WIDTH
    char line[100];
    fgets(line, sizeof(line), hexFileOpen);
    int cols = strlen(line) / 8; // Assuming each hex value is 8 characters

    // Calculate the number of lines in the file to determine IMAGE_HEIGHT
    int lines = 0;
    while (fgets(line, sizeof(line), hexFileOpen) != NULL)
    {
        lines++;
    }
    fclose(hexFileOpen);

    IMAGE_WIDTH = cols;
    IMAGE_HEIGHT = lines;

    *imageSize = IMAGE_WIDTH * IMAGE_HEIGHT;

    // Allocate memory for the 2D array
    unsigned char **image = (unsigned char **)malloc(lines * sizeof(unsigned char *));
    for (int i = 0; i < lines; i++)
    {
        image[i] = (unsigned char *)malloc(cols * 3 * sizeof(unsigned char));
    }

    // Reopen the file to read the hex values and store them in the 2D array
    hexFileOpen = fopen(imageFile, "r");
    if (hexFileOpen == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < cols * 3; j++)
        {
            fscanf(hexFileOpen, "%hhu", &image[i][j]); // Use "%hhu" to read as unsigned char
        }
    }

    fclose(hexFileOpen);
    return image;
}

void rgb_converter(unsigned char **image, int imageSize)
{
    red = (unsigned char *)malloc(imageSize * sizeof(unsigned char));
    green = (unsigned char *)malloc(imageSize * sizeof(unsigned char));
    blue = (unsigned char *)malloc(imageSize * sizeof(unsigned char));

    for (int i = 0; i < imageSize; i++)
    {
        red[i] = image[i / IMAGE_WIDTH][i % IMAGE_WIDTH * 3];
        green[i] = image[i / IMAGE_WIDTH][i % IMAGE_WIDTH * 3 + 1];
        blue[i] = image[i / IMAGE_WIDTH][i % IMAGE_WIDTH * 3 + 2];
    }
}

int *grey_scale_converter(int imageSize)
{
    int *greyScale = (int *)malloc(imageSize * sizeof(int));

    for (int i = 0; i < imageSize; i++)
    {
        greyScale[i] = (red[i] * 0.3) + (green[i] * 0.59) + (blue[i] * 0.11);
    }

    return greyScale;
}

int *black_and_white_converter(int imageSize)
{
    int *blackAndWhite = (int *)malloc(imageSize * sizeof(int));

    for (int i = 0; i < imageSize; i++)
    {
        int greyValue = (red[i] * 0.3) + (green[i] * 0.59) + (blue[i] * 0.11);

        if (greyValue > 127)
        {
            blackAndWhite[i] = 1;
        }
        else
        {
            blackAndWhite[i] = 0;
        }
    }

    return blackAndWhite;
}

void inverse_converter(int imageSize)
{
    // Inverse an RGB image
    for (int i = 0; i < imageSize; i++)
    {
        red[i] = 255 - red[i];
        green[i] = 255 - green[i];
        blue[i] = 255 - blue[i];
    }
}

void free_memory(int **image, int *greyScale, int *blackAndWhite, int imageSize)
{
    for (int i = 0; i < IMAGE_HEIGHT; i++)
    {
        free(image[i]);
    }
    free(image);
    free(greyScale);
    free(blackAndWhite);
    free(red);
    free(green);
    free(blue);
}

int main()
{
    int **image = NULL;
    int *greyScale = NULL;
    int *blackAndWhite = NULL;
    int imageSize;

    image = hex_reader(hexImageFile, &imageSize);
    rgb_converter(image, imageSize);
    greyScale = grey_scale_converter(imageSize);
    blackAndWhite = black_and_white_converter(imageSize);
    inverse_converter(imageSize);

    // Print image as a test to see if it works
    // for (int i = 0; i < imageSize; i++)
    // {
    //     printf("%hhu %hhu %hhu\n", red[i], green[i], blue[i]); 
    // }

    // Free allocated memory
    free_memory(image, greyScale, blackAndWhite, imageSize);

    return 0;
}
