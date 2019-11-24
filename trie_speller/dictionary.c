// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"


// Represents number of children for each node in a trie
#define N 27

// global variable to keep track of number of words in dictionary
// used in size() function
unsigned int COUNTER = 0;

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// prototypes
int child(char c);
int traverse(node *temp);
void free_memory(node *temp);


// Represents a trie
node *root;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
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

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // Temporary node to keep track of cursor in trie data structure
        node *temp = root;

        // to eterate over word's letters
        for (int c = 0, wlen = strlen(word); c < wlen; c++)
        {
            int cursor = child(word[c]);

            // to handle situation in wich letter does not exist on trie
            if (temp->children[cursor] == NULL)
            {
                // creat a new node
                node *new_node = malloc(sizeof(node));

                // initilize new node
                new_node->is_word = false;
                for (int i = 0; i < N; i++)
                {
                    new_node->children[i] = NULL;
                }

                // creat relations and move the temporary pointer to new node
                temp->children[cursor] = new_node;
            }

            // move the temporary pointer to new node &
            // handling situation in which letter already exists on trie
            temp = temp->children[cursor];
        }

        // when last letter reched, mark end of word
        temp->is_word = true;
    }


    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // temporary node to start from data structure root
    node *temp = root;

    return traverse(temp);
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // temporary node to start from data structure root
    node *temp = root;

    // to iterate over each word's letters
    for (int c = 0, wlen = strlen(word); c < wlen; c++)
    {
        // map each letter to children[i]
        int cursor = child(tolower(word[c]));

        if (temp->children[cursor])
        {
            // move to next node if exists till end of word reached
            temp = temp->children[cursor];
        }
        else
        {
            // retuen false if hit NULL before end of word reached
            return false;
        }
    }

    // if end of word reached, check whether it's a valid word
    if (temp->is_word == false)
    {
        return false;
    }

    return true;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // temporary node to start from data structure root
    node *temp = root;

    free_memory(temp);

    return true;
}


// To map each character to a children on node
int child(char c)
{
    // to handle apostrophe '
    if (c == 39)
    {
        return 26;
    }
    else
    {
        return (tolower(c) - 'a');
    }
}

// to traverse all pathways trough a dictionary for assigned words
int traverse(node *node_ptr)
{
    // iterate over each node children
    for (int i = 0; i < N; i++)
    {
        // check if current node children are not null
        if (node_ptr->children[i])
        {
            // check if current node is end of a word
            if (node_ptr->children[i]->is_word == true)
            {
                COUNTER++;
            }
            // recursivly repeat process for node under each child
            traverse(node_ptr->children[i]);
        }
    }
    return COUNTER;
}

// to free all memory blocks allocated to the loading dictionary
void free_memory(node *node_ptr)
{
    for (int i = 0; i < N; i++)
    {
        // check if current node children are not null
        if (node_ptr->children[i])
        {
            // move to next node recursivly
            free_memory(node_ptr->children[i]);
        }
    }

    // free a node if its all children are NULL
    free(node_ptr);
}
