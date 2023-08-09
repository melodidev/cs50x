// Filters image
#include "helpers.h"
#include <stdlib.h>
#include <math.h>

const int MATRIX = 9;
void swap(RGBTRIPLE *a, RGBTRIPLE *b);
RGBTRIPLE Sobel(RGBTRIPLE matrix[]);

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
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
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

    // For each pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Create 3x3 cluster of pixels (matrix)
            RGBTRIPLE matrix[MATRIX];
            int counter = 0;

            // For each matrix
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    // If any element of matrix (pixel) is out of borders, treat them as their value is zero
                    if ((y + i) >= height || (y + i) < 0 || (x + j) >= width || (x + j) < 0)
                    {
                        matrix[counter].rgbtRed = 0;
                        matrix[counter].rgbtGreen = 0;
                        matrix[counter].rgbtBlue = 0;
                    }
                    // Else get the color values from the original image
                    else
                    {
                        matrix[counter].rgbtRed   = copy[y + i][x + j].rgbtRed;
                        matrix[counter].rgbtGreen = copy[y + i][x + j].rgbtGreen;
                        matrix[counter].rgbtBlue  = copy[y + i][x + j].rgbtBlue;
                    }

                    counter++;
                }
            }

            // Compute Sobel
            RGBTRIPLE pixel = Sobel(matrix);

            // Assign new values
            image[y][x].rgbtRed   = pixel.rgbtRed;
            image[y][x].rgbtGreen = pixel.rgbtGreen;
            image[y][x].rgbtBlue  = pixel.rgbtBlue;
        }
    }
}

// Implement Sobel Algorithm
RGBTRIPLE Sobel(RGBTRIPLE matrix[])
{
    RGBTRIPLE pixel;

    // Indicate kernel values for x and y axes
    int x_kernels[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int y_kernels[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    int gx_r = 0;
    int gx_g = 0;
    int gx_b = 0;
    int gy_r = 0;
    int gy_g = 0;
    int gy_b = 0;

    // For each element of matrix
    for (int i = 0; i < MATRIX; i++)
    {
        // Compute Gx and Gy
        gx_r += matrix[i].rgbtRed   * x_kernels[i];
        gx_g += matrix[i].rgbtGreen * x_kernels[i];
        gx_b += matrix[i].rgbtBlue  * x_kernels[i];

        gy_r += matrix[i].rgbtRed   * y_kernels[i];
        gy_g += matrix[i].rgbtGreen * y_kernels[i];
        gy_b += matrix[i].rgbtBlue  * y_kernels[i];
    }

    // Compute each new channel value and limit the maximum to 255
    pixel.rgbtRed   = fmin(round(sqrt(pow(gx_r, 2) + pow(gy_r, 2))), 255);
    pixel.rgbtGreen = fmin(round(sqrt(pow(gx_g, 2) + pow(gy_g, 2))), 255);
    pixel.rgbtBlue  = fmin(round(sqrt(pow(gx_b, 2) + pow(gy_b, 2))), 255);

    return pixel;
}

