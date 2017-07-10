/****************************************************************************
 * dictionary.c
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

// pointer to root & dictionary size counter
node root = {false,{NULL}};
unsigned int dicsize = 0;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // initialize the cursor
    node* cursor = &root;
    int nodeindex = 0;
    
    // check to see if the word is in the dictionary
    for (int i = 0, length = strlen(word) + 1; i < length; i++)
    {
        // check if it's upper
        if (isupper(word[i]))
        {
            // change it to lower
            tolower(word[i]);
            
            // see what part of index it is
            nodeindex = tolower(word[i]) - 'a';
            
            // check to see if it's NULL
            if (cursor->children[nodeindex] == NULL)
            {
                return false;
            }
            
            else
            {
                cursor = cursor->children[nodeindex];
            }
        }
        
        // if letter is already lowercase
        else if (isalpha(word[i]))
        {
            // see what part of index it is
            nodeindex = tolower(word[i]) - 'a';
            
            // check to see if it's NULL
            if (cursor->children[nodeindex] == NULL)
            {
                return false;
            }
            
            else
            {
                cursor = cursor->children[nodeindex];
            }
        }
        
        // check to see if the letter is apostrophe
        else if (word[i] == '\'')
        {
            // set the node index
            nodeindex = 26;
            
            // check to see if it's NULL
            if (cursor->children[nodeindex] == NULL)
            {
                return false;
            }
            
            else
            {
                cursor = cursor->children[nodeindex];
            }
            
        }
        
        // if hit null terminator
        else if (word[i] == '\0')
        {
            // if this is a word in the dictionary, return true
            if (cursor->is_word == true)
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // load the dictionary file and check if it was loaded
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        return false;
    }
    
    // create a 'word' buffer with the max size being the length of the largest word + 1 (for the newline character) and indexes
    char word[LENGTH+1];
    int wordindex = 0, nodeindex = 0;
    
    // create root, cursor
    //root = malloc(sizeof(node));
    // make sure malloc didn't return NULL
    //if (root == NULL)
    //{
      //  return false;
    //}
    node* cursor = &root;

    
    // iterate through the entire file & build the dictionary via a trie
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        /*******************************
        **STORE WORD IN THE WORD ARRAY**
        *******************************/
        
        // store the first word of the dictionary up until the newline character
        if (isalpha(c) || c == '\'')
        {
            word[wordindex] = c;
            wordindex++;
        }
        
        // if it's a newline, stop and place the word in the trie
        else
        {
            // place newline in the word buffer
            word[wordindex] = c;
                        
            /*****************************
            **PUT THE WORD INTO THE TRIE**
            *****************************/
            
            // loop through the word
            for (int i = 0 ; i < wordindex + 1; i++)
            {
                // is word[i] an alphabetic character?
                if (isalpha(word[i]))
                {
                    // see what part of the children index it will be
                    nodeindex = tolower(word[i]) - 'a';
                    // is children[nodeindex] == NULL?
                    if (cursor->children[nodeindex] == NULL)
                    {
                         // malloc a new node to a temporary node
                         node* tmp = malloc(sizeof(node));
                             // make sure malloc didn't return NULL
                             if (tmp == NULL)
                             {
                                 return false;
                             }
                         *tmp = (node) {false,{NULL}};
                         // make children[nodeindex] point to the new node
                         cursor->children[nodeindex] = tmp;
                        
                         // move cursor to new node
                         cursor = tmp;
                     }
                    
                     // else if children[nodeindex] != NULL
                     else
                     {
                         // move into new node and continue
                         cursor = cursor->children[nodeindex];
                     }
                }

                // is word[i] an apostrophe?
                else if (word[i] == '\'')
                {
                    // set the nodeindex to 26
                    nodeindex = 26; 
                    // is children[nodeindex] == NULL?
                    if (cursor->children[nodeindex] == NULL)
                    {
                        // malloc a new node
                        node* tmp = malloc(sizeof(node));
                            // make sure malloc didn't return NULL
                            if (tmp == NULL)
                            {
                                return false;
                            }
                        *tmp = (node) {false,{NULL}};
                            
                        // make children[nodeindex] point to new node
                        cursor->children[nodeindex] = tmp;
                        
                        // move cursor to new node
                        cursor = tmp;
                    }
                    // else if children[nodeindex] != NULL
                    else
                    {
                        // move into new node and continue
                        cursor = cursor->children[nodeindex];
                    }
                }
                
                // is word[i] a newline character?
                else if (word[i] == '\n')
                {
                    // set is_word = true
                    cursor->is_word = true;
                    
                    // increase dictionary size
                    dicsize++;
                }
            }
            
            // restart the index and cursor for the next word
            wordindex = 0;
            cursor = &root;
        }
    }
    
    // close file and return true to indicate success
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (dicsize != 0)
    {
        return dicsize;
    }
    
    else
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    return freeNode (&root);
}

bool freeNode (node* node)
{
        // if node == null, you're good ! (simple case)
        if (node == NULL)
        {
            return true;
        }
        // if node != null
        else
        {
            // check it all
            for (int i = 0; i < 27; i++)
            {
                if (node->children[i] != NULL)
                {
                    freeNode(node->children[i]);
                }

            }
            if (node != &root)
            free(node);
            return true;
        }  
}
