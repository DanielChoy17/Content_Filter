#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This function is the constructor for a node.
// This function takes in as parameters a char oldspeak and another char newspeak.
// This function returns the created node.
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node)); // Allocating memory for the struct n
    if (n) {
        if (oldspeak == NULL) {
            n->oldspeak = NULL;
        } else {
            // Copying over the oldspeak translation that was passed in to the variable oldspeak in the struct n
            n->oldspeak = strdup(oldspeak);
        }
        if (newspeak == NULL) {
            n->newspeak = NULL;
        } else {
            // Copying over the newspeak translation that was passed in to the variable newspeak in the struct n
            n->newspeak = strdup(newspeak);
        }
        n->left = NULL;
        n->right = NULL;
    }
    return n;
}

// This function is the destructor for a node.
// This function takes in as a parameter a double pointer to node n.
void node_delete(Node **n) {
    if (*n) {
        free((*n)->oldspeak); // Freeing the memory used for the char oldspeak
        (*n)->oldspeak = NULL; // Setting the pointer of oldspeak to NULL

        free((*n)->newspeak); // Freeing the memory used for the char newspeak
        (*n)->newspeak = NULL; // Setting the pointer of newspeak to NULL

        free(*n); // Freeing the memory used for the Node n
        *n = NULL; // Setting the pointer of n to NULL
    }
}

// This function will be used to produce correct program output. More specifically, this prints
// out the contents of a node.
// This function takes in as a parameter a pointer to n.
void node_print(Node *n) {
    if (n->oldspeak != NULL && n->newspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else if (n->oldspeak != NULL && n->newspeak == NULL) {
        printf("%s\n", n->oldspeak);
    }
}
