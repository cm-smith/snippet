/* ========================================================================== */
/* File: hashtable.h (refactored from Dartmouth CS Dept.)
 *
 * Project name: Snippet
 *
 * This file contains the definitions for a hashtable structure.
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"

// ---------------- Constants
#define MAX_HASH_SLOT 10000	                     // number of "buckets"

// ---------------- Structures/Types

typedef struct HashTableNode {
    char *entry;                                    // previous entry in table
    struct HashTableNode *next;                     // pointer to next node
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];            // actual hashtable
} HashTable;

// ---------------- Public Variables

// ---------------- Prototypes/Macros

/*
 * jenkins_hash - Bob Jenkins' one_at_a_time hash function
 * @str: char buffer to hash
 * @mod: desired hash modulus
 *
 * Returns hash(str) % mod. Depends on str being null terminated.
 * Implementation details can be found at:
 *     http://www.burtleburtle.net/bob/hash/doobs.html
 */
unsigned long JenkinsHash(const char *str, unsigned long mod);

/* Initializes a hash table */
HashTable* InitHash( );

/* AddToHashTable - returns 1 if successful, 0 otherwise */
int AddToHashTable( char *str_to_insert, HashTable *existing_table );

/* InHashTable - returns 1 if successful, 0 otherwise */
int InHashTable( char *str1, char *str2);

/* free hash table */
void FreeHashTable( HashTable *existing_table );

#endif // HASHTABLE_H
