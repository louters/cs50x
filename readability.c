#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int count_letters(string text);

int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    return 0;
}

int count_letters(string text)
{
    int cnt = 0;
    for (int i=0, n = strlen(text); i < n ; i++)
    {
        if (isalpha(text[i]))
        {
            cnt++;
        }
    }
    return cnt;
}
