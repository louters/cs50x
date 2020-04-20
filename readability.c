#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    //Prompt user for input text.
    string text = get_string("Text: ");

    int l = count_letters(text);
    printf("%i letter(s)\n", l);

    int w = count_words(text);
    printf("%i word(s)\n", w);

    int s = count_sentences(text);
    printf("%i sentence(s)\n", s);

    return 0;
}

int count_letters(string text)
// Returns nbr of letters in string <text>.
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

int count_words(string text)
// Return nbr of words in <text>.
{
    int cnt = 0;
    for (int i=0, n = strlen(text); i < n ; i++)
    {
        if (isblank(text[i]))
        {
            cnt++;
        }
    }
    cnt++;
    return cnt;
}

int count_sentences(string text)
// Return nbr of sentences in <text>.
{
    int cnt = 0;
    for (int i=0, n = strlen(text); i < n ; i++)
    {
        if (text[i] == '!' || text[i] == '.' || text[i] == '?')
        {
            cnt++;
        }
    }
    return cnt;
}