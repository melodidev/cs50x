// Implement different filters to images
#include "helpers.h"
#include <stdlib.h>
#include <math.h>

void swap(RGBTRIPLE *a, RGBTRIPLE *b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // For every pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Create a pointer to make the statement short
            RGBTRIPLE *pixel = &image[y][x];

            // Calculate and round the average
            float av = (pixel->rgbtRed + pixel->rgbtGreen + pixel->rgbtBlue) / 3.0;
            int average = round(av);
            pixel->rgbtRed = average;
            pixel->rgbtGreen = average;
            pixel->rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // For every pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Create a pointer to make the statement short
            RGBTRIPLE *pixel = &image[y][x];

            // Make calculations for sepia effect and limit the maximumm value to 255
            float f_sepiaRed = fmin(.393 * pixel->rgbtRed + .769 * pixel->rgbtGreen + .189 * pixel->rgbtBlue, 255);
            float f_sepiaGreen = fmin(.349 * pixel->rgbtRed + .686 * pixel->rgbtGreen + .168 * pixel->rgbtBlue, 255);
            float f_sepiaBlue = fmin(.272 * pixel->rgbtRed + .534 * pixel->rgbtGreen + .131 * pixel->rgbtBlue, 255);
            int sepiaRed = round(f_sepiaRed);
            int sepiaGreen = round(f_sepiaGreen);
            int sepiaBlue = round(f_sepiaBlue);

            // Assign new values
            pixel->rgbtRed = sepiaRed;
            pixel->rgbtGreen = sepiaGreen;
            pixel->rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // For every horizontal pixel
    for (int y = 0; y < height; y++)
    {
        // Till the middle
        for (int x = 0; x < round(width / 2.0); x++)
        {
            // Swap right half with the left half
            swap(&image[y][(width - 1) - x], &image[y][x]);
        }
    }
    return;
}

// Swap two pixels
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp = *a;
    *a = *b;
    *b = temp;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy the original image in order to not overwrite it
    RGBTRIPLE copy[height][width];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            copy[y][x] = image[y][x];
        }
    }

    // Factor means 3x3 box, e.g. it would be 9x9 if factor was 2
    int factor = 1;

    // For every pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float totalRed = 0;
            float totalBlue = 0;
            float totalGreen = 0;
            float count = 0;

            // For every piece of (3x3) box near to the pixel
            for (int i = -factor; i <= factor; i++)
            {
                for (int j = -factor; j <= factor; j++)
                {
                    // If pixel is outside of the corners, do not count it
                    if ((y + i) >= height || (y + i) < 0)
                    {
                        continue;
                    }
                    // If pixel is outside of the edges, do not count it
                    if ((x + j) >= width || (x + j) < 0)
                    {
                        continue;
                    }
                    // Else it is in the middle
                    count++;
                    totalRed = totalRed + copy[y + i][x + j].rgbtRed;
                    totalGreen = totalGreen + copy[y + i][x + j].rgbtGreen;
                    totalBlue = totalBlue + copy[y + i][x + j].rgbtBlue;
                }
            }
            // Take the average and compute new color values
            int red = round(totalRed / count);
            int green = round(totalGreen / count);
            int blue = round(totalBlue / count);

            image[y][x].rgbtRed = red;
            image[y][x].rgbtGreen = green;
            image[y][x].rgbtBlue = blue;
        }
    }
    return;
}
