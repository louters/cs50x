#include <stdio.h>
#include <math.h>
#include <cs50.h>

int min_num_coins(int cents);

int main(void)
{
    float change_owed;
    do
    {
        //Ask how much change is owed.
        change_owed = get_float("Change owed: ");
    }
    while (change_owed < 0);
    //Convert to cents
    int cents = round(change_owed * 100);

    //Compute min of coins possible
    int res = min_num_coins(cents);

    //Print min of coins possible with\n
    printf("%i\n", res);
}

int min_num_coins(int cents)
//Compute min of coins possible on owned cents
{
    int cnt = 0;
    while (cents > 0)
    {
        if (cents >= 25)
        {
            cents -= 25;
        }
        else if (cents >= 10)
        {
            cents -= 10;
        }
        else if (cents >= 5)
        {
            cents -= 5;
        }
        else
        {
            cents--;
        }
        cnt++;
    }
    return cnt;
}