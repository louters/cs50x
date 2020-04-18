#include <stdio.h>
#include <math.h>
#include <cs50.h>

bool checksum(long number);
string which_cc(long card_no);
bool is_even(int n);

int main(void)
{
    //prompt for input
    long card_no = get_long("Number: ");

    string msg = "INVALID";
    if (checksum(card_no)) //if pass checksum
    {
        //which CC?
        msg = which_cc(card_no);
    }

    printf("%s\n", msg);
}

bool checksum(long card_no)
/*Perform Luhn's Algo - Returns True if checksum.*/
{
    int res = 0;
    int cnt = 0;
    int digit = 0;
    long divisor = 1;
    
    do
    {
        cnt++;
        divisor *= 10;
        //get each digit, starting from the end
        digit = (card_no % divisor) / (divisor / 10);

        if(is_even(cnt)) //every other digit?
        {
            digit *= 2;
            res += digit % 10;
            res += digit / 10;
        }
        else
        {
            res += digit;
        }
    }
    while (card_no/divisor > 0); 
    
    //return true if last digit of res is 0
    if (res % 10 == 0)
    {
        return true;
    }
    return false;
}

string which_cc(long card_no)
/*Return the credit card type of card_no or INVALID*/
{
    //Get nbr of digits
    double i = 0;
    long quotient;
    do
    {
        i++;
        quotient = (card_no / pow(10.0, i));
    } while (i < 18 && quotient > 0);
    
    int first_digits;
    // Amex starts with 34 or 37 | length 15 digits
    if (i == 15 )
    {
        first_digits = card_no / pow(10.0, i - 2);
        if (first_digits == 34 || first_digits ==37)
        {
            return "AMEX";
        }
    }

    // MC starts with 51-55 | length 16 digits
    else if (i == 16)
    {
        first_digits = card_no / pow(10.0, i -2);
        
        if( first_digits > 50 && first_digits < 56)
        {
            return "MASTERCARD";
        }
    }

    // VISA starts with 4 | length 13 or 16 digits
    if (i == 13 || i == 16)
    {
        first_digits = card_no / pow(10.0, i - 1);
        if (first_digits == 4)
        {
            return "VISA";
        }
    }

    //Did not match any cards.
    return "INVALID";
}

bool is_even(int n)
/*Return true if n is even*/
{
    if (n % 2 == 0)
    {
        return true;
    }
    return false;
}