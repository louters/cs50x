#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

bool str_isdigit(string str);
void error_message(void);
char encode_char(char c, int key);

int main(int argc, string argv[])
{
    //ERROR: no arg provided.
    if (argc != 2)
    {
        error_message();
        return 1;
    }
    
    //ERROR: arg is not a decimal digit.
    string arg = argv[1];
    if (!str_isdigit(arg))
    {
        error_message();
        return 1;
    }

    // Converting argument to key.
    int key = atoi(arg);

    // ERROR: negative key.
    if (key < 0)
    {
        error_message();
        return 1;
    }
    
    // Prompt user for plain text.
    string plain = get_string("plaintext: ");

    // Print cipher text.
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        printf("%c", encode_char((char) plain[i], key));
    }
    printf("\n");

    return 0;
}

bool str_isdigit(string str)
// Return true if each char in <str> is a digit, false otherwise.
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

void error_message(void)
// Raise error for wrong command-line usage.
{
    printf("ERROR: key must be non-negative integer.\n");
    printf("Usage: ./caesar key\n");
}

char encode_char(char c, int key)
// Return encrypted char <c> with cipher <key>.
{
    char tmp;
    // Lowercase char
    if (c >= 97 && c <= 122)
    {
        tmp = (c + key - 96) % 26 + 96;
        return tmp;
    }
    // Uppercase char
    else if (c >= 65 && c <= 90)
    {
        tmp = (c + key - 64) % 26 + 64;
        return tmp;
    }
    // Case where char <c> is not a letter.
    return c;
}