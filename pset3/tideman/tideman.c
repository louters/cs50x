#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int i_rank;
    int j_rank;
    for (int i = 0; i < candidate_count; i++)
    {
        // Find i's rank.
        for (int rk = 0; rk < candidate_count; rk++)
        {
            if (ranks[rk] == i)
            {
                i_rank = rk;
                break;
            }
        }

        for (int j = 0; j < candidate_count; j++)
        {
            // Find j's rank.
            for (int rk = 0; rk < candidate_count; rk++)
            {
                if (ranks[rk] == j)
                {
                    j_rank = rk;
                    break;
                }
            }

            // Update pref between i & j.
            if (i_rank < j_rank)
            {
                preferences[i][j]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    bool swap = false;
    // Bubble sort
    for (int i = 0; i < pair_count - 2; i++)
    {
        if (preferences[pairs[i].winner][pairs[i].loser]
            < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
        {
            pair tmp = pairs[i];
            pairs[i] = pairs[i + 1];
            pairs[i + 1] = tmp;
            swap = true;
        }
        if (!swap)
        {
            return;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    pair p;
    int i;
    int j;
    for (int p_idx = 0; p_idx < pair_count; p_idx++)
    {
        p = pairs[p_idx];
        i = p.winner;
        j = p.loser;

        if (j != pairs[0].winner)
        {
            locked[i][j] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    bool is_source;
    for (int j = 0; j < candidate_count; j++)
    {
        is_source = true;
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][j])
            {
                is_source = false;
                break;
            }
        }
        if (is_source)
        {
            printf("%s\n", candidates[j]);
        }
    }
    return;
}

