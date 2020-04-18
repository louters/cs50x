#include <cs50.h>
#include <stdio.h>

void print_pyramid(int height);
void print_char(int n, char c);

int main(void)
{
    int height; 
    //get height from user until acceptable
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    //build pyramid
    print_pyramid(height);
}

void print_pyramid(int height)
/*Print a hash pyramid of height <height>.*/
{
    char brick = '#';
    char blank = ' ';

    for (int row = height ; row > 0 ; row--)
    {
        print_char(row - 1, blank);
        print_char(height - row + 1, brick);
        printf(" ");
        print_char(height - row + 1, brick);
        print_char(row - 1, blank);
        printf("\n");

    }
}

void print_char(int n, char c)
/*Print char <c> <n> times.*/
{
    for (int i = 0; i < n; i++)
    {
        printf("%c", c);
    }
}