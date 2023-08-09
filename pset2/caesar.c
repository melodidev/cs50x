// Encrypts messages according to given key

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string(argv[]))
{
    // Accept a single command-line arguement as key
    if (argc != 2)
    {
        printf("Usage: ./as2 key\n");
        return 1;
    }

    // If any character of key is not a digit, return error
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (isdigit(argv[1][i]) == 0)
        {
            printf("Usage: ./as2 key\n");
            return 1;
        }
    }

    // Convert key from a string to an integer
    int key = atoi(argv[1]);
    printf("%i is a valid key.\n", key);

    // Prompt for plaintext
    string plaintext = get_string("Plaintext: ");

    // Key might be more than number of letters of alphabet so wrap around alphabet
    key = key % 26;

    // Prompt for ciphertext
    printf("Ciphertext: ");

    // Turn each character to ciphertext
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        // Lowercase letters remain lowercase in ciphertext
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            unsigned char c = plaintext[i] + key;
            if (c > 'z')
            {
                c -= 26;
            }
            printf("%c", c);
        }
        // Uppercase letters remain uppercase in ciphertext
        else if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            unsigned char c = plaintext[i] + key;
            if (c > 'Z')
            {
                c -= 26;
            }
            printf("%c", c);
        }
        // Spaces, symbols, numbers etc. remain same
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
    return 0;
}