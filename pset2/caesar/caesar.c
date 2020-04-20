#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

bool str_isdigit(string str);

int main(int argc, string argv[])
{
    //ERROR: no arg provided.
    if (argc != 2)
    {
        printf("Error: %s accepts one and one only argument\n", argv[0]);
        printf("Usage: ./caesar key\n");
        return 1;
    }
    
    //ERROR: arg is not a decimal digit.
    string key = argv[1];
    if (!str_isdigit(key))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    printf("Success\n");
    return 0;
}

bool str_isdigit(string str)
{
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}