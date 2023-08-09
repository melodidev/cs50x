// Prints a pyramid of blocks

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Ask for an integer between 1 and 8 and take as height
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height > 8 || height < 1);

    // Do the following line by line until reach the height of the pyramid
    for (int i = 0; i < height; i++)
    {
        // Print decreasing number of spaces
        for (int j = (height - 1); j > i; j--)
        {
            printf(" ");
        }
        // Print increasing number of blocks
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }
        // Print space for the middle of the pyramid
        printf("  ");
        // Print increasing number of blocks
        for (int x = 0; x <= i; x++)
        {
            printf("#");
        }
        printf("\n");
    }
}