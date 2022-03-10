/* ========================================================================== */
/* File: hashtable.c (refactored from Dartmouth CS Dept.)
 *
 * Project name: Snippet
 * You should include in this file your functionality for the hashtable.
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <string.h>			// strlen
#include <stdlib.h>			// malloc
#include <stdio.h>			// printf (debug)

// ---------------- Local includes  e.g., "file.h"
#include "hashtable.h"                       // hashtable functionality

// ---------------- Constant definitions

// ---------------- Macro definitions
#define SAFEFREE(a) free(a); a=NULL;

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

unsigned long JenkinsHash(const char *str, unsigned long mod)
{
    size_t len = strlen(str);
    unsigned long hash, i;

    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}

/* initialize an empty hash table with each entry in the table
 * beginning at NULL; returns a pointer to the empty table */
HashTable* InitHash( )
{
	HashTable *hash = (HashTable *)malloc(sizeof(HashTable));
	for(int i=0; i<MAX_HASH_SLOT; i++)
	{
		hash->table[i] = NULL;
	}
	return hash;
}

/* adds an entry to the hash table only if it is not a duplicate
 *  returns 1 (true) if successful and 0 (false) if failure */
int AddToHashTable( char *str_to_insert, HashTable *existing_table )
{
	// check the slot in the hash table
	unsigned long slot = JenkinsHash( str_to_insert, MAX_HASH_SLOT );
	HashTableNode* hashed_str = existing_table->table[slot];

	// make sure it's not a duplicate by scanning over nodes
	if(hashed_str != NULL)
	{
		if(InHashTable(hashed_str->entry, str_to_insert))
		{
			return 0;
		}

		while(hashed_str->next!=NULL)
		{
			hashed_str = hashed_str->next;
			if(InHashTable(hashed_str->entry, str_to_insert))
			{
				return 0;
			}
		}
		
		// add the hash node if it's not a duplicate
		HashTableNode *new_entry = (HashTableNode *)malloc(sizeof(HashTableNode));
		new_entry->entry = strdup(str_to_insert);
		new_entry->next = NULL;
		hashed_str->next = new_entry;
	}
	
	// initialize the hash node if nothing's in that slot
	else
	{
		HashTableNode *new_entry = (HashTableNode *)malloc(sizeof(HashTableNode));
		new_entry->entry = strdup(str_to_insert);
		new_entry->next = NULL;
		existing_table->table[slot] = new_entry;
	}
	
	// returns a true value (C booleans) to show the url was new and added
	return 1;
}

/* checks to see if two strings in the hashtable are exactly the same;
* returns 1 (true) if strings match, 0 (false) otherwise */
int InHashTable( char *str1, char *str2)
{
	// check the length of the strings, then how they compare
	if(strlen(str1) == strlen(str2))
	{
		if(strcmp(str1, str2) == 0)
		{
			return 1;
		}
	}
	return 0;
}

/* free a hash table node and set entry to NULL */
void FreeHashTableNode( HashTableNode *hashNode )
{
	while(hashNode != NULL)
	{
		HashTableNode *curr = hashNode;
		hashNode = hashNode->next;
		SAFEFREE(curr->entry);
		SAFEFREE(curr);
	}
}

/* free all nodes in the table, then free the table */
void FreeHashTable( HashTable *hash )
{
	// iterate through entire table to free every entry
	for(int i=0; i<MAX_HASH_SLOT; i++)
	{
		FreeHashTableNode(hash->table[i]);
	}
	// then free the hash table struct itself
	SAFEFREE(hash);
}
