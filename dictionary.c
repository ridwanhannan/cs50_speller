// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "dictionary.h"

// Represents a node in a hash table
// LENGTH = 45
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// Change this to something larger to represent more buckets
// Key times 1.3 that is a prime number (rule of thumb)
// Reference: http://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16-8.html
const unsigned int N = 186019;
unsigned int words = 0;

// Hash table
// GLobal pointer array, table which will represent the hash table you will use
// to keep track of words in the dictionary
// Array contains N node pointers
node *table[N];

// Returns true if word is in dictionary else false
// Check that word is in dictionary
bool check(const char *word)
{
    // Hash word to obtain value
    unsigned int index = hash(word);

    // Access the individual linked list in the has table
    node *cursor = table[index];

    //Traverse Linked List
    while (cursor != NULL)
    {

        // comparing strings
        int result = strcasecmp(word, cursor -> word);
        if (result == 0)
        {
            return true;
        }
        else
        {
        cursor = cursor -> next;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // djb2 Hash function by Dan Bernstein
    // http://www.cse.yorku.ca/~oz/hash.html
    unsigned int hash = 5381;
    int hash_table_size = N;

    while (hash_table_size == *word++)
        hash = ((hash << 5) + hash) + hash_table_size;
    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    // Open Dict File
    // Datatype FILE
    // *file - pointer to a file and call it file
    // fopen - return a pointer to that file
    FILE *file = fopen(dictionary, "r");
    // Check if return value is NULL
    if (file == NULL)
    {
        return 1;
    }

    // Set buffer of LENGTH for array word
    char word[LENGTH + 1];
    // Read Strings from file one at a time. word is the address of the location where the strings are going to go
    while (fscanf(file, "%s", word) != EOF)
    {
        //Create a new node by allocating memory to store node
        node *n = malloc(sizeof(node)); // store the address of that node inside of n. n will now be the start of the link list (i.e. list)
        // check if return value is null:
        if (n == NULL)
        {
            unload();
            return false;
        }
        else
        {
        //copy word into node using strcpy (take string from one location and store into another)
        strcpy(n  -> word, word); // copy the word into the character array n -- > word, which is the word field of this node

        //Hash Word by calling has funciton and returning some number as the index
        //Takes word as input and returns number corresponding to which bucket to store the word in
        unsigned int index = hash(n -> word);

        //Insert word into hash table
        //Index into hashtable to get the linked list you want to use
        //Get the new node to point to the beginning of the linked list(n)
        //set n = the new node | add new node to linked list
        n -> next = table[index];
        table[index] = n;

        node *current = table[index];

        if (current != NULL)
        {
            current = current -> next;
            words ++;
        }

        }
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{

    return words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i ++)
    {
        // Set temp variable and cursor variable to point to the start of the linked list
        node *tmp = table[i];
        node *cursor = table[i];

        // Free memory
        while (cursor != NULL)
        {
            cursor = cursor -> next;
            free(tmp);
            tmp = cursor;
        }

    }

    return true;
}
