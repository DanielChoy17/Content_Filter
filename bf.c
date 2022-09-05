#include "bf.h"
#include "salts.h"
#include "bv.h"
#include "speck.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2]; // Primary hash function salt.
    uint64_t secondary[2]; // Secondary hash function salt.
    uint64_t tertiary[2]; // Tertiary hash function salt.
    BitVector *filter;
};

// This function is the constructor for a Bloom filter.
// This function takes in as a parameter a uint32_t size which represents the size in bits of the BitVector filter.
// This function returns the created BloomFilter.
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    bf->primary[0] = SALT_PRIMARY_LO;
    bf->primary[1] = SALT_PRIMARY_HI;
    bf->secondary[0] = SALT_SECONDARY_LO;
    bf->secondary[1] = SALT_SECONDARY_HI;
    bf->tertiary[0] = SALT_TERTIARY_LO;
    bf->tertiary[1] = SALT_TERTIARY_HI;
    bf->filter = bv_create(size);
    return bf;
}

// This function is the destructor for a Bloom filter.
// This function takes in as a parameter a double pointer to the BloomFilter bf
void bf_delete(BloomFilter **bf) {
    bv_delete(&(*bf)->filter);
    free(*bf);
    *bf = NULL;
}

// This function returns the size of the Bloom filter. In other words, the number of bits that the Bloom filter
// can access.
// This function takes in as a parameter a BloomFilter bf.
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// This function takes oldspeak and inserts it into the Bloom filter.
// This function takes in as parameters a BloomFilter bf and char oldspeak.
void bf_insert(BloomFilter *bf, char *oldspeak) {
    bv_set_bit(bf->filter, hash(bf->primary, oldspeak) % bf_size(bf));
    bv_set_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_size(bf));
    bv_set_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_size(bf));
}

// This function probes the Bloom filter for oldspeak. Oldspeak is hashed with each of the three salts for the three
// indices. If all the bits at those indices are set, this function returns true to signify that oldspeak was most
// likely added to the Bloom filter. Else, this function returns false.
// This function takes in as parameters a BloomFilter bf and char oldspeak.
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    return bv_get_bit(bf->filter, hash(bf->primary, oldspeak) % bf_size(bf))
           && bv_get_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_size(bf))
           && bv_get_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_size(bf));
}

// This function returns the number of set bits in the Bloom filter.
// This function takes in as a parameter a BloomFilter bf.
uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < bv_length(bf->filter); i++) {
        count = count + bv_get_bit(bf->filter, i);
    }
    return count;
}

// This function is a debug function to print out the bits of a Bloom filter.
// This function takes in as a parameter a BloomFilter bf.
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
