#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    //Prompt user for input text.
    string text = get_string("Text: ");

    //Count letters.
    int l = count_letters(text);

    //Count words.
    int w = count_words(text);

    //Count sentences.
    int s = count_sentences(text);

    // Compute mean nbr of letters per 100 words.
    float L = 100 * (float) l / (float) w;

    // Compute mean nbr of sentences per 100 words.
    float S = 100 * (float) s / (float) w;

    // Compute Coleman-Liau index.
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // Get grade readabality.
    if (index < 1)
    {
        printf("Before grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
    

    return 0;
}

int count_letters(string text)
// Returns nbr of letters in string <text>.
{
    int cnt = 0;
    for (int i = 0, n = strlen(text); i < n ; i++)
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
    for (int i = 0, n = strlen(text); i < n ; i++)
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
    for (int i = 0, n = strlen(text); i < n ; i++)
    {
        if (text[i] == '!' || text[i] == '.' || text[i] == '?')
        {
            cnt++;
        }
    }
    return cnt;
}