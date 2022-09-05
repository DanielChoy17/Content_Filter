#include "ht.h"
#include "bst.h"
#include "salts.h"
#include "speck.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// This function is the constructor for a hash table.
// This function takes in as a parameter uint32_t size which represents the number of indices, or binary search trees,
// the hash table can index up to.
// This function returns the created HashTable ht.
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    ht->salt[0] = SALT_HASHTABLE_LO;
    ht->salt[1] = SALT_HASHTABLE_HI;
    ht->size = size;
    ht->trees = (Node **) calloc(size, sizeof(Node *));
    return ht;
}

// This function is the destructor for a hash table.
// This function takes in as a parameter a double pointer to HashTable ht.
void ht_delete(HashTable **ht) {
    for (uint32_t i = 0; i < (*ht)->size; i++) {
        if ((*ht)->trees[i] != NULL) {
            bst_delete(&(*ht)->trees[i]);
        }
    }
    free((*ht)->trees);
    free(*ht);
    *ht = NULL;
}

// This function returns the hash table's size.
// This function takes in as a parameter a HashTable ht.
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// This function searches for an entry, a node, in the hash table that contains oldspeak, If the node is found, the
// pointer to the node is returned. Else, a NULL pointer is returned.
// This function takes in as parameters a HashTable ht and a char oldspeak.
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups = lookups + 1;
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    return bst_find(ht->trees[index], oldspeak);
}

// This function inserts the specified oldspeak and its corresponding newspeak translation into the hash table.
// This function takes in as parameters a HashTable ht, a char oldspeak, and a char newspeak.
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups = lookups + 1;
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
}

// This function returns the number of non-NULL binary search trees in the hash table.
// This function takes in as a parameter a HashTable ht.
uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i] != NULL) {
            count = count + 1;
        }
    }
    return count;
}

// This function returns the average binary search tree size.
// This function takes in as a parameter a HashTable ht.
double ht_avg_bst_size(HashTable *ht) {
    uint32_t combined_tree_sizes = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        combined_tree_sizes = combined_tree_sizes + bst_size(ht->trees[i]);
    }
    return (double) combined_tree_sizes / ht_count(ht);
}

// This function returns the average binary search tree height.
// This function takes in as a parameter a HashTable ht.
double ht_avg_bst_height(HashTable *ht) {
    uint32_t combined_tree_heights = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        combined_tree_heights = combined_tree_heights + bst_height(ht->trees[i]);
    }
    return (double) combined_tree_heights / ht_count(ht);
}

// This function is a debug function to print out the contents of a hash table.
// This function takes in as a parameter a HashTable ht.
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i++) {
        printf("Contents at index %d:\n", i);
        bst_print(ht->trees[i]);
    }
}
