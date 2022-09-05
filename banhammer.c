#include "bf.h"
#include "bst.h"
#include "bv.h"
#include "ht.h"
#include "node.h"
#include "parser.h"
#include "speck.h"
#include "salts.h"
#include "messages.h"

#include <regex.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>
#include <string.h>

#define WORD "[A-Za-z0-9_]+(('|-)[A-Za-z0-9_]+)*"

#define OPTIONS "ht:f:s"

void help_message(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "  A word filtering program for the GPRSC.\n"
                    "  Filters out and reports bad words parsed from stdin.\n"
                    "\n"
                    "USAGE\n"
                    "  ./banhammer [-hs] [-t size] [-f size]\n"
                    "\n"
                    "OPTIONS\n"
                    "  -h           Program usage and help.\n"
                    "  -s           Print program statistics.\n"
                    "  -t size      Specify hash table size (default: 2^16).\n"
                    "  -f size      Specify Bloom filter size (default: 2^20).\n");
}

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t size_ht = 65536;
    uint32_t size_bf = 1048576;
    bool stats = false;
    BloomFilter *bf;
    HashTable *ht;
    char badspeak[1024];
    char oldspeak[1024];
    char newspeak[1024];

    // Parsing command-line options using getopt() and handling them accordingly
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': size_ht = atoi(optarg); break;
        case 'f': size_bf = atoi(optarg); break;
        case 's': stats = true; break;
        case 'h': help_message(); return EXIT_SUCCESS;
        default: help_message(); return EXIT_FAILURE;
        }
    }

    // Initializing Bloom Filter
    if (size_bf <= 0) {
        fprintf(stderr, "Invalid Bloom filter size.\n");
        return EXIT_FAILURE;
    } else {
        bf = bf_create(size_bf);
    }

    // Initializing hash table
    if (size_ht <= 0) {
        fprintf(stderr, "Invalid hash table size.\n");
        return EXIT_FAILURE;
    } else {
        ht = ht_create(size_ht);
    }

    // Reading in a list of badspeak words from badspeak.txt
    FILE *badspeak_file = fopen("badspeak.txt", "r");
    while (fscanf(badspeak_file, "%s\n", badspeak) != EOF) {
        bf_insert(bf, badspeak);
        ht_insert(ht, badspeak, NULL);
    }
    fclose(badspeak_file);

    // Reading in a list of oldspeak and newspeak pairs from newspeak.txt
    FILE *newspeak_file = fopen("newspeak.txt", "r");
    while (fscanf(newspeak_file, "%s %s\n", oldspeak, newspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }
    fclose(newspeak_file);

    // Compiling regular expression
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return EXIT_FAILURE;
    }

    // Reading in words from stdin and filtering them if needed
    char *word = NULL;
    Node *mix_message = bst_create();
    Node *bad_message = bst_create();
    while ((word = next_word(stdin, &re)) != NULL) {
        // Changing word to all lowercase
        for (uint32_t i = 0; i < strlen(word); i++) {
            word[i] = tolower(word[i]);
        }
        // Check if word has been added to the Bloom filter
        if (bf_probe(bf, word)) {
            Node *n = ht_lookup(ht, word);
            // If the hash table contains the word and the word does not have a newspeak translation,
            // insert badspeak word into a list of badspeak words that the citizen used.
            // If the hash table contains the word and the word does have a newspeak translation,
            // insert oldspeak word and newspeak translation into a list of oldspeak words with newspeak translations.
            if (n != NULL && n->newspeak == NULL) {
                bad_message = bst_insert(bad_message, n->oldspeak, n->newspeak);
            } else if (n != NULL && n->newspeak != NULL) {
                mix_message = bst_insert(mix_message, n->oldspeak, n->newspeak);
            }
        }
    }

    // Print statistics if enabled
    // Else, printing the corresponding message based on the crime of the citizen
    if (stats) {
        fprintf(stdout, "Average BST size: %f\n", ht_avg_bst_size(ht));
        fprintf(stdout, "Average BST height: %f\n", ht_avg_bst_height(ht));
        fprintf(stdout, "Average branches traversed: %f\n", ((float) branches / lookups));
        fprintf(stdout, "Hash table load: %0.6f%%\n", (100 * ((float) ht_count(ht) / ht_size(ht))));
        fprintf(
            stdout, "Bloom filter load: %0.6f%%\n", (100 * ((float) bf_count(bf) / bf_size(bf))));
    } else {
        if (bst_size(mix_message) > 0 && bst_size(bad_message) > 0) {
            printf("%s", mixspeak_message);
            bst_print(bad_message);
            bst_print(mix_message);
        } else if (bst_size(mix_message) == 0 && bst_size(bad_message) > 0) {
            printf("%s", badspeak_message);
            bst_print(bad_message);
        } else if (bst_size(mix_message) > 0 && bst_size(bad_message) == 0) {
            printf("%s", goodspeak_message);
            bst_print(mix_message);
        }
    }

    // Deleting the binary search trees used
    bst_delete(&mix_message);
    bst_delete(&bad_message);

    // Deleting the bloom filter used
    bf_delete(&bf);

    // Deleting the hash table used
    ht_delete(&ht);

    // Clearing out the static words buffer
    clear_words();

    // Freeing the memory used by the regular expression
    regfree(&re);

    return EXIT_SUCCESS;
}
