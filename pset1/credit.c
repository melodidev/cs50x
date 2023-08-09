// Checks if a credit card number is valid and determines it is type

#include <stdio.h>
#include <cs50.h>

long luhns_algorithm(long number);
long digit_length(long number);
long get_digit(long number, long order, long len);

int main(void)
{
    // Prompt for a credit card number
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);

    // Implement Luhn's Algorithm, if last digit is not zero, number is invalid
    if (luhns_algorithm(number) != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Take the length, first and second digits
    long len = digit_length(number);
    long first_digit = get_digit(number, 1, len);
    long second_digit = get_digit(number, 2, len);

    // If digit length is 15 and starts with 34 or 37, it is American Express
    if (len == 15 && first_digit == 3 && (second_digit == 4 || second_digit == 7))
    {
        printf("AMEX\n");
    }
    // If digit length is 16 and starts with 51, 52, 53, 54, or 55, it is MasterCard
    else if (len == 16 && first_digit == 5 && (second_digit >= 1 && second_digit <= 5))
    {
        printf("MASTERCARD\n");
    }
    // If digit length is 13 or 16 and first digit is 4, it is Visa
    else if ((len == 16 || len == 13) && first_digit == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

// Luhn's Algorithm
long luhns_algorithm(long number)
{
    long even_digits = 0;
    long x = number / 10;
    long sum_even_digits = 0;

    while (x > 0)
    {
        // Take digits at even decimal places and multiply them
        even_digits = (x % 10) * 2;
        x = x / 100;

        // If a digit has more than one decimal places seperate it to digits on more time and take sum
        if (even_digits > 9)
        {
            long y = even_digits;
            while (y > 0)
            {
                even_digits = y % 10;
                y = y / 10;
                sum_even_digits = sum_even_digits + even_digits;
            }
        }
        // Else add digits to sum
        else
        {
            sum_even_digits = sum_even_digits + even_digits;
        }
    }

    // Take sum of the odd digits and add it to the sum of evens
    long odd_digits = number;
    long sum_odd_digits = 0;
    long z = number;
    while (z > 0)
    {
        odd_digits = z % 10;
        z = z / 100;
        sum_odd_digits = sum_odd_digits + odd_digits;
    }

    long sum = sum_even_digits + sum_odd_digits;

    // Take the last digit of the sum
    long last_digit = sum % 10;

    return last_digit;
}

// Calculate how many digits credit card number has
long digit_length(long number)
{
    long digits = 0;
    long len = 0;
    while (number > 0)
    {
        digits = number % 10;
        number = number / 10;
        len++;
    }

    return len;
}

// Take the digit which user asks for
long get_digit(long number, long order, long len)
{
    long digit = 0;
    while (len > order - 1)
    {
        digit = number % 10;
        number = number / 10;
        order++;
    }

    return digit;
}