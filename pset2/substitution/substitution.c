#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

bool is_validkey(string arg);
void error_message(void);
char encode_char(char c, string key);

const int upper_key_sum = 2015;

int main(int argc, string argv[])
{
    //ERROR: no arg provided or too many args.
    if (argc != 2)
    {
        error_message();
        return 1;
    }
    
    //ERROR: not valid key
    string key = argv[1];
    if (!is_validkey(key))
    {
        printf("Key must contain 26 characters.\n");
        error_message();
        return 1;
    }

    // Prompt user for plain text.
    string plain = get_string("plaintext: ");

    // Print cipher text.
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        printf("%c", encode_char(plain[i], key));
    }
    printf("\n");

    return 0;
}

bool is_validkey(string arg)
// Return true if all 26 alpha char appear once and once only.
{
    int key_sum = 0;
    int n = strlen(arg);

    for (int i = 0; i < n; i++)
    {
        key_sum += toupper(arg[i]);
    }
    
    // char unique in key?
    int tmp_cnter;
    for (int i = 0; i < 26; i++)
    {
        tmp_cnter = 0;
        char tmp_c = tolower(arg[i]);

        for (int j = 0; j < 26; j++)
        {
            if (tmp_c == tolower(arg[j]))
            {
                tmp_cnter++;
            }
        }
        if (tmp_cnter > 1)
        // char arg[i] appears more than once
        {
            return false;
        }
    }
    // Length must be 26 and
    // Sum must match sum of upper alphabet int values.
    if (key_sum == upper_key_sum && n == 26)
    {
        return true;
    }
    return false;
}

char encode_char(char c, string key)
//Encode alpha char <c> according to <key>.
{
    //lowercase char
    if (islower(c) && isalpha(c))
    {
        return tolower(key[c - 97]);
    }
    //uppercase char
    else if (isupper(c) && isalpha(c))
    {
        return toupper(key[c - 65]);
    }

    // Return original character if not alpha char.
    return c;
}

void error_message(void)
{
    printf("Usage: ./substitution key\n");
}
