#include <math.h>

#include "helpers.h"

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

            image[i][j].rgbtBlue = (BYTE) mean;
            image[i][j].rgbtGreen = (BYTE) mean;
            image[i][j].rgbtRed = (BYTE) mean;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
