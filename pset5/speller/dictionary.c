// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Count words in dictionary
int word_count = 0;

// Insert word as node into hash table
void insert(char *word);

// Number of buckets in hash table
 #define N 10//524287 

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Return value
    bool res = false;

    // Index of word to search
    unsigned int idx = hash(word);

    if (table[idx] != NULL)
    {
        // Search node
        node *current = table[idx];
        while (true)
        {
            // Case insensitive match
            if (strcasecmp(word, current->word) == 0)
            {
                res = true;
                break;
            }
            if (current->next == NULL)
            {
                break;
            }
            // Setting to next link
            current = current->next;
        }
    }
    return res;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /*Polynomial rolling hash function with m = N
    source: https://cp-algorithms.com/string/string-hashing.html */

    // Prime number
    const unsigned int p = 31;
    
    // Hash index
    long h = 0;

    // Int value of char
    int c_int = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        // Apostrophe
        if (word[i] == 39)
        {
            c_int = 27;
        }
        // Alpha char
        else
        {
            c_int = (tolower(word[i] - 96));
        }

        //h += c_int * pow(p, i);
        h = (h * p + c_int) % N;

    }
    //return h % N;
    return h;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Dictionary
    FILE *dict = fopen(dictionary, "r");
    // Check dictionary opened
    if (dict == NULL)
    {
        fprintf(stderr, "Dicitonary not opened.\n");
        return false;
    }

    // Word in dictionary
    char *word = malloc(LENGTH);
    if (word == NULL)
    {
        fprintf(stderr, "Failed at word creation\n");
        return false;
    }

    // Read words in dictionary and load words in hashtable
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Insert Node into hash table
        insert(word);
        // Update number of words in dictionary
        word_count++;
    }

    // Free memory of word
    free(word);

    // Close dictionary file
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Go through each element of array
    for (int i = 0; i < LENGTH; i++)
    {
        if (table[i] != NULL)
        {
            // Set node at head of linked list
            node *current = table[i];
            // Free linked list at index i
            while (true)
            {
                if (current->next != NULL)
                {
                    node *tmp = current-> next;
                    free(current);
                    current = tmp;
                }
                else
                {
                    free(current);
                    break;
                }
            }
        }
    }
    return true;
}

// Insert word as node into hash table
void insert(char *word)
{
    // Index in hash table
    unsigned int idx = hash(word);

    // Node to insert
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        fprintf(stderr, "Failed at node creation\n");
        exit(1);
    }
    strcpy(n->word, word);
    n->next = NULL;

    // Insert node at beginning
    if (table[idx] == NULL)
    {
        table[idx] = n;
    }
    // Insert at tail
    else
    {
        // Go through linked list until end
        node *tmp = table[idx];
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        // Point last node to new node
        tmp->next = n;
    }
    //free(n);
    return;
}

