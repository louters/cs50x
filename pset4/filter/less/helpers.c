#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

void blur_pixel(int height, int width, RGBTRIPLE image[height][width],
                RGBTRIPLE image_copy[height][width], int row, int col);

// Swap two RGBTRIPLE
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
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
            mean = round((double) mean / 3);

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

            // Apply sepia formula
            new_red = round(.393 * orig_red + .769 * orig_green + .189 * orig_blue);
            new_green = round(.349 * orig_red + .686 * orig_green + .168 * orig_blue);
            new_blue = round(.272 * orig_red + .534 * orig_green + .131 * orig_blue);

            // Assign new colours with round and 255-cap control
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
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < ((double) width - 1) / 2; j++)
        {
            // Swap both ends of row i
            swap(&image[i][j], &image[i][width - 1 - j]);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy image
    RGBTRIPLE(*image_copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j] = image[i][j];
        }
    }

    // Blur each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blur_pixel(height, width, image, image_copy, i, j);
        }
    }

    // Free memory from image copy
    free(image_copy);

    return;
}

void blur_pixel(int height, int width, RGBTRIPLE image[height][width],
                RGBTRIPLE image_copy[height][width], int row, int col)
{
    int neighbors = 0; // Neighbor count, includes pixel itself
    int mean_red = 0;
    int mean_green = 0;
    int mean_blue = 0;

    for (int i = -1; i < 2; i++)
    {
        // Manage edge & corners
        if (row + i >= 0 && row + i < height)
        {
            for (int j = -1; j < 2; j++)
            {
                // Manage edge & corners
                if (col + j >= 0 && col + j < width)
                {
                    neighbors++; // Increase neighbour count

                    // Sum colors of each pixel in the box
                    mean_red += image_copy[row + i][col + j].rgbtRed;
                    mean_green += image_copy[row + i][col + j].rgbtGreen;
                    mean_blue += image_copy[row + i][col + j].rgbtBlue;
                }
            }
        }
    }
    // Get rounded int mean per color
    mean_red = round((double) mean_red / neighbors);
    mean_green = round((double) mean_green / neighbors);
    mean_blue = round((double) mean_blue / neighbors);

    // Assign mean colors to original image
    image[row][col].rgbtRed = mean_red;
    image[row][col].rgbtGreen = mean_green;
    image[row][col].rgbtBlue = mean_blue;

}