#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "helpers.h"

// Blur a pixel in an image
void blur_pixel(int height, int width, RGBTRIPLE image[height][width],
                RGBTRIPLE image_copy[height][width], int row, int col);

// Swap two RGBTRIPLE
void swap(RGBTRIPLE *a, RGBTRIPLE *b);

// Copy image
void copy_image(int height, int width, RGBTRIPLE original[height][width],
                RGBTRIPLE copy[height][width]);

// Set pixel color with Sobel operator
void set_pixel_edge_col(int height, int width, RGBTRIPLE image[height][width],
                        RGBTRIPLE image_copy[height][width], int row, int col);

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
    copy_image(height, width, image, image_copy);

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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy original image
    RGBTRIPLE(*image_copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    copy_image(height, width, image, image_copy);

    // 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Set new color channel values for pixel
            set_pixel_edge_col(height, width, image, image_copy, i, j);
        }
    }
    // Free memory from image copy
    free(image_copy);
    return;
}

// Set pixel color with Sobel operator
void set_pixel_edge_col(int height, int width, RGBTRIPLE image[height][width],
                        RGBTRIPLE image_copy[height][width], int row, int col)
{
    // Initialize kernels
    int kernel_x[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int kernel_y[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    // Initialize Gx & Gy {R, G, B}
    int g_x[3] = {0, 0, 0};
    int g_y[3] = {0, 0, 0};

    for (int i = -1; i < 2; i++)
    {
        // Manage edges & corners
        if (i + row >= 0 && i + row < height)
        {
            for (int j = -1; j < 2; j++) 
            {
                // Manage edges & corners
                if (j + col >= 0 && j + col < width)
                {
                    g_x[0] += kernel_x[i + 1][j + 1] * image_copy[row + i][col + j].rgbtRed;
                    g_x[1] += kernel_x[i + 1][j + 1] * image_copy[row + i][col + j].rgbtGreen;
                    g_x[2] += kernel_x[i + 1][j + 1] * image_copy[row + i][col + j].rgbtBlue;

                    g_y[0] += kernel_y[i + 1][j + 1] * image_copy[row + i][col + j].rgbtRed;
                    g_y[1] += kernel_y[i + 1][j + 1] * image_copy[row + i][col + j].rgbtGreen;
                    g_y[2] += kernel_y[i + 1][j + 1] * image_copy[row + i][col + j].rgbtBlue;
                }
            }

        }
    } 
    
    // Setting channel colors
    image[row][col].rgbtRed = fmin(255.0, round(sqrt(pow(g_x[0], 2.0) + pow(g_y[0], 2.0))));
    image[row][col].rgbtGreen = fmin(255.0, round(sqrt(pow(g_x[1], 2.0) + pow(g_y[1], 2.0))));
    image[row][col].rgbtBlue = fmin(255.0, round(sqrt(pow(g_x[2], 2.0) + pow(g_y[2], 2.0))));

}

// Blur pixel in an image.
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

// Swap two RGBTRIPLE
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}

// Copy image
void copy_image(int height, int width, RGBTRIPLE original[height][width],
                RGBTRIPLE copy[height][width])
{
    // Copy each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = original[i][j];
        }
    }
}