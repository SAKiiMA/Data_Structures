// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

void FreeNode(node *temp);

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // index to: which word belongs to which row of hashtable
        int rowindex = hash(word);

        // Initilize memory for words read from dictionary
        node *current_node = malloc(sizeof(node));

        // To check if memory has been allocated
        if (!current_node)
        {
            unload();
            printf("Running out of memory!!\n");
            return false;
        }

        // Populating the created node with word read from dictionary
        strcpy(current_node->word, word);
        current_node->next = NULL;

        // adding current node to begining of linked-list linked
        if (hashtable[rowindex])
        {
            current_node->next = hashtable[rowindex];
            hashtable[rowindex] = current_node;
        }
        // Handling the very first node for each row
        else
        {
            hashtable[rowindex] = current_node;
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // to keep track of word numbers
    int counter = 0;

    // To iterate over hashtable
    for (int i = 0; i < 26; i++)
    {
        if (hashtable[i])
        {
            // Iteartes over each elemn of current link list
            //node *temp = hashtable[i] : temporary node to iterate over
            // linked lists elemnts
            for (node *temp = hashtable[i]; temp; temp = temp->next)
            {
                counter++;
            }
        }
    }
    return counter;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    //**
    // to change word in lower case
    //*
    char wcopy[strlen(word) + 1];
    for (int i = 0; i < strlen(word); i++)
    {
        wcopy[i] = tolower(word[i]);
    }
    wcopy[strlen(word)] = '\0';
    //
    //*

    // Index to: which hashtable row to look up
    int rowindex = hash(word);

    // temporary node to keep track of cursor
    node *temp = hashtable[rowindex];

    // iterate over linked list till word found or end of list reached
    while (temp)
    {
        // check if the word is same as the value in current node
        if (strcmp(wcopy, temp->word) == 0)
        {
            return true;
            break;
        }

        // move cursor to next node if word is not found
        temp = temp->next;
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < 26; i++)
    {
        if (hashtable[i])
        {
            node *current_node = hashtable[i];

            while (current_node)
            {
                node *temp = current_node;
                current_node = current_node->next;
                free(temp);
            }
        }
    }

    return true;
}
