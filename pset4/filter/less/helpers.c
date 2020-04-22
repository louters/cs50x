#include <math.h>

#include "helpers.h"

// Swap two RGBTRIPLE
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    // Swap if a and b have diff addresses
    if (&a != &b)
    {
        RGBTRIPLE tmp;
        tmp = *a;
        *a = *b;
        *b = tmp;
    }
    return;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int mean = -1;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            mean = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            mean = round( (double) mean / 3);

            image[i][j].rgbtBlue =  mean;
            image[i][j].rgbtGreen =  mean;
            image[i][j].rgbtRed =  mean;

        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE orig_red, orig_green, orig_blue;
    int new_red, new_green, new_blue;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            orig_red = image[i][j].rgbtRed;
            orig_green = image[i][j].rgbtGreen;
            orig_blue = image[i][j].rgbtBlue;

            new_red = round(.393 * orig_red + .769 * orig_green + .189 * orig_blue);
            new_green = round(.349 * orig_red + .686 * orig_green + .168 * orig_blue);
            new_blue = round(.272 * orig_red + .534 * orig_green + .131 * orig_blue);

            image[i][j].rgbtRed = fmin((double) new_red, 255.0);
            image[i][j].rgbtGreen = fmin((double) new_green, 255.0);
            image[i][j].rgbtBlue = fmin((double) new_blue, 255.0);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int j;
    for (int i = 0; i < height; i++)
    {
        j = width - 1;
        while (j != width - j)
        {
            swap(&image[i][j], &image[i][width - j]);
            j--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}