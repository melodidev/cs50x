// Calculates grade level of a text

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int coleman_liau_index(int letters, int words, int sentences);

int main(void)
{
    // Prompt user for a text
    string text = get_string("Text: ");

    // Count letters, words and sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Implement Coleman-Liau index to find grade
    int grade = coleman_liau_index(letters, words, sentences);

    // Print grade
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    // Check every character and make them lowercase, if they are letters and count
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = tolower(text[i]);
        if (c >= 'a' && c <= 'z')
        {
            letters++;
        }
    }

    return letters;
}

int count_words(string text)
{
    // Count spaces
    int spaces = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            spaces++;
        }
    }

    // Words are one number greater than the number of spaces
    if (spaces > 0)
    {
        return spaces + 1;
    }

    return 0;
}

int count_sentences(string text)
{
    // Count periods, exclamation points and question marks to get number of sentences
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentences++;
        }
    }

    return sentences;
}

int coleman_liau_index(int letters, int words, int sentences)
{
    // Computes Coleman-Liau index
    float L = letters / (float) words * 100;
    float S = sentences / (float) words * 100;
    float index = (0.0588 * L) - (0.296 * S) - 15.8;

    return round(index);
}