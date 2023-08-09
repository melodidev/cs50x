// Encrypts user input using key as a substitution cipher

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Accept a single command-line arguement as key
    if (argc != 2)
    {
        printf("Usage: ./as3x key\n");
        return 1;
    }
    string key = argv[1];

    // Check if key contains 26 characters
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // For every character, check if they are alphabetical
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if ((key[i] >= 'a' && key[i] <= 'z') ||
            (key[i] >= 'A' && key[i] <= 'Z'))
        {
            continue;
        }
        else
        {
            printf("Only alphabetical characters must be given.\n");
            return 1;
        }
    }

    // Key should have every aphabetical character once
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (key[i] == key[j])
            {
                printf("%c found twice!\n", key[i]);
                return 1;
            }
        }
    }

    // Prompt for plaintext
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");

    // For every character of plaintext
    for (int i = 0, n = strlen(plaintext); i < n; i ++)
    {
        // Lowercase characters remain lowercase, but change them according to key
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            int j = plaintext[i] - 'a';
            printf("%c", tolower(key[j]));
        }
        // Uppercase characters remain uppercase, but change them according to key
        else if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            int j = plaintext[i] - 'A';
            printf("%c", toupper(key[j]));
        }
        // Symbols, numbers, spaces etc remain same
        else
        {
            printf("%c",  plaintext[i]);
        }
    }
    printf("\n");
    return 0;
}