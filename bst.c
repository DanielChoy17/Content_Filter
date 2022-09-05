#include "bst.h"
#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint64_t branches = 0;

// This function is the constructor for a binary search tree that constructs an empty tree.
// This function returns NULL to indicate an empty tree.
Node *bst_create(void) {
    return NULL;
}

// This function is a helper function for bst_height.
// This function takes in as parameters a uint32_t x and a uint32_t y.
static uint32_t max(uint32_t x, uint32_t y) {
    return x > y ? x : y;
}

// This function returns the height of the binary search tree rooted at root.
// This function takes in as a parameter a Node root which represents the root node of a binary search tree.
uint32_t bst_height(Node *root) {
    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

// This function returns the size of the binary search tree rooted at root.
// This function takes in as a parameter a Node root which represents the root node of a binary search tree.
uint32_t bst_size(Node *root) {
    if (root == NULL) {
        return 0;
    }
    return bst_size(root->left) + bst_size(root->right) + 1;
}

// This function searches for a node containing oldspeak in the binary search tree rooted at root. If a node is
// found, the pointer to the node is returned. Else, a NULL pointer is returned.
// This function takes in as parameters a Node root which represents the root node of a binary search tree and a
// char oldspeak which is what we are trying to find in the binary search tree.
Node *bst_find(Node *root, char *oldspeak) {
    Node *current_node = root;
    if (root != NULL && oldspeak != NULL) {
        while (current_node != NULL && strcmp(current_node->oldspeak, oldspeak) != 0) {
            if (strcmp(current_node->oldspeak, oldspeak) > 0) {
                current_node = current_node->left;
                branches = branches + 1;
            } else {
                current_node = current_node->right;
                branches = branches + 1;
            }
        }
    }
    return current_node;
}

// This function inserts a new node containing the specified oldspeak and newspeak into the binary search tree
// rooted at root. Duplicates should not be inserted.
// This function takes in as parameters a Node root which represents the root node of a binary search tree, a char
// oldspeak, and a char newspeak.
// This function returns the updated binary search tree (the Node root).
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    Node *a = root;
    Node *b = NULL;
    if (root == NULL) {
        b = node_create(oldspeak, newspeak);
        root = b;
        return root;
    }

    if (oldspeak == NULL) {
        return root;
    }

    uint32_t temp = branches;
    if (bst_find(root, oldspeak) != NULL) {
        branches = temp;
        return root;
    }
    branches = temp;

    if (root != NULL && oldspeak != NULL) {
        while (a != NULL) {
            b = a;
            if (strcmp(a->oldspeak, oldspeak) > 0) {
                a = a->left;
                branches = branches + 1;
            } else {
                a = a->right;
                branches = branches + 1;
            }
        }
    }

    if (strcmp(b->oldspeak, oldspeak) > 0) {
        b->left = node_create(oldspeak, newspeak);
    } else {
        b->right = node_create(oldspeak, newspeak);
    }
    return root;
}

// This function prints out each node in the binary search tree through an inorder traversal.
// This function takes in as a parameter a Node root which represents the root node of a binary search tree.
void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}

// This function is the destructor for a binary search tree rooted at root.
// This function takes in as a parameter a double pointer to a Node root which represents the root node of a
// binary search tree.
void bst_delete(Node **root) {
    if (*root != NULL) {
        if ((*root)->left != NULL) {
            bst_delete(&(*root)->left);
        }
        if ((*root)->right != NULL) {
            bst_delete(&(*root)->right);
        }
        node_delete(root);
    }
}
