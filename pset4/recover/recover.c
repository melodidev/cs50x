// ./recover card.raw
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Incorrect usage.\n");
        return 1;
    }

    // Open memory card
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // Repeat until reach end of file, read 512 bytes (an image size) into a buffer
    FILE *image = NULL;
    BYTE buffer[512];
    int counter = 0;
    while (fread(&buffer, sizeof(BYTE), 512, file))
    {
        // Check if any JPEG found by looking the first four bytes
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Name it
            char filename[50];
            sprintf(filename, "%03i.jpg", counter);
            counter++;

            // If image exist, close the file
            if (image != NULL)
            {
                fclose(image);
            }

            // Write image to a new file
            image = fopen(filename, "w");
        }

        // if image exist fwrite image
        if (image != NULL)
        {
            fwrite(&buffer, sizeof(BYTE), 512, image);
        }
    }

    fclose(file);

    // ne olur ne olmaz
    if (image != NULL)
    {
        fclose(image);
    }
}