// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

int number_of_words = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word to obtain a hash value
    unsigned int index = hash(word);

    // Traverse linked list, looking for the word (strcasecmp)
    for (node *n = table[index]; n != NULL; n = n->next)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Takes input a: word, with alphabetical characters and apostrophes
    char first_letter = tolower(word[0]);
    if (first_letter >= 'a' && first_letter <= 'z')
    {
        // Gives output: an numerical index between 0 and N-1
        unsigned int index = first_letter - 'a';
        return index;
    }
    return N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Read strings from file, one at a time
    char word[LENGTH];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for each world
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;
        }
        strcpy(n->word, word);
        n->next = NULL;

        // Hash word to obtain a hash value
        int index = hash(n->word);

        // Insert node into hash table at that location
        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            n->next = table[index];
            table[index] = n;
        }
        number_of_words++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return number_of_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *list = table[i];
        while (list != NULL)
        {
            node *n = list->next;
            free(list);
            list = n;
        }
    }
    return true;
}
