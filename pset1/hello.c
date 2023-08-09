// Asks user for name and greet them by their name or default
#include <cs50.h>
#include <stdio.h>

int main(void)

{
    string name = get_string("What is your name?\n");
    printf("Hello, %s.\n", name);
}