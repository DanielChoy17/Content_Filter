#include "bv.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// This function is the constructor for a bit vector that holds length bits.
// This function takes in as a parameter a uint32_t named length which represents the number of bits
// the bit vector holds.
// This function returns NULL in the event that sufficient memory cannot be allocated for the BitVector
// otherwise it returns a pointer to an allocated BitVector.
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        bv->vector = (uint8_t *) calloc(((length - 1) / 8) + 1, sizeof(uint8_t));
        return bv;
    } else {
        return NULL;
    }
}

// This function is the destructor for a bit vector.
// This function takes in as a parameter a double pointer to a BitVector bv.
void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }
}

// This function returns the length of a bit vector.
// This function takes in as a parameter a BitVector bv.
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// This function sets the ith bit in a bit vector. If i is out of range, returns false. Otherise, returns true
// to indicate success.
// This function takes in as parameters a BitVector bv and a uint32_t i which represents the index of the
// bit we are setting.
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i < bv->length && i >= 0) {
        bv->vector[i / 8] |= (0x1 << i % 8);
        return true;
    } else {
        return false;
    }
}

// This function clears the ith bit in the bit vector. If i is out of range, returns false. Otherwise, returns true
// to indicate success.
// This function takes in as parameters a BitVector bv and a uint32_t i which represents the index of the
// bit we are clearing.
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i < bv->length && i >= 0) {
        bv->vector[i / 8] &= ~(0x1 << i % 8);
        return true;
    } else {
        return false;
    }
}

// This function returns the ith bit in the bit vector. If i is out of range, returns false. Otherwise, returns
// false if the value of bit i is 0 and returns true if the value of bit i is 1.
// This function takes in as parameters a BitVector bv and a uint32_t i which represents the index of the bit we
// are getting.
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i < bv->length && i >= 0 && (((bv->vector[i / 8] >> i % 8) & 0x1) == 1)) {
        return true;
    } else {
        return false;
    }
}

// This function is a debug function to print the bits of a bit vector.
// This function takes in as a parameter a BitVector bv.
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < ((bv->length - 1) / 8) + 1; i++) {
        printf("Bits at index %d: %" PRIu8 "\n", i, bv->vector[i]);
    }
}
